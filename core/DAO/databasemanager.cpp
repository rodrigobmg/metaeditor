#include <iostream>
#include "mongoc.h"

//Project imports
#include "databasemanager.h"
#include "object.h"
#include "Utils/bsonutils.h"

DatabaseException::DatabaseException(const char *message)
{
    strcpy(m_message, message);
}

DatabaseException::~DatabaseException()
{
}

const char* DatabaseException::what() const throw()
{
    return m_message;
}

DatabaseManager::DatabaseManager() :
    m_client(nullptr)
{

}

DatabaseManager::~DatabaseManager()
{
    if( m_client )
        mongoc_client_destroy(m_client);

    mongoc_cleanup ();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager inst;
    return inst;
}

void DatabaseManager::tryReconnection()
{
    connect(m_ip, m_port);
}

void DatabaseManager::connect(const char *ip, uint16_t port) throw (DatabaseException)
{
    mongoc_init();
    char connectionString[256];
    sprintf(connectionString, "mongodb://%s:%hu", ip, port);
    m_client = mongoc_client_new (connectionString);

    BSON_ASSERT(m_client);

    if(!m_client)
    {
        throw DatabaseException("Falha ao iniciar cliente para conexão ao banco de dados.");
    }

    bson_t *command;
    bson_t reply;
    bson_error_t error;

    command = BCON_NEW ("ping", BCON_INT32 (1));

    if (!mongoc_client_command_simple (m_client, "admin", command, NULL, &reply, &error))
    {
        bson_destroy (&reply);
        bson_destroy (command);
        throw DatabaseException(error.message);
    }

    m_ip = ip;
    m_port = port;

    bson_destroy (&reply);
    bson_destroy (command);
}

#define destroy_object(obj) delete obj; obj = nullptr;

Object* DatabaseManager::read(const char *collection_name, const char* object_name, const char* database_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name, collection_name);
    bson_t* query = BCON_NEW("name", object_name);
    mongoc_cursor_t* cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    const bson_t* doc;
    Object* obj = nullptr;

    while( mongoc_cursor_next(cursor, &doc) )
    {
        obj = new Object(doc);

        if( obj->unserialize() == true )
        {
            if( strcmp(obj->Name(), object_name) == 0 )
            {
                return obj;
            }
        }

        destroy_object(obj);
    }

    return obj;
}

bool DatabaseManager::create(const bson_t *b, const char *collection_name, const char *database_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        //Pode levantar exceção
        tryReconnection();
    }

    char* object_name = BSONUtils::getString(b, "name");
    mongoc_collection_t* collection = mongoc_client_get_collection (m_client, database_name, collection_name);

    if(exists(object_name, collection) == true)
    {
        mongoc_collection_destroy (collection);
        return false;
    }

    bson_error_t error;
    if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, b, NULL, &error))
    {
        std::cout << error.message << std::endl;
        return false;
    }

    mongoc_collection_destroy (collection);
    return true;
}

bool DatabaseManager::exists(const char *object_name, mongoc_collection_t* collection)
{
    bson_t* query = BCON_NEW ("$query", "{", "name", BCON_UTF8(object_name), "}");
    mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
    const bson_t* b;
    if(mongoc_cursor_next(cursor, &b))
    {
        const char * retName = BSONUtils::getString(b,"name");
        if( strcmp(retName, object_name) == 0)
        {
            mongoc_cursor_destroy(cursor);
            bson_destroy(query);
            return true;
        }
    }

    mongoc_cursor_destroy(cursor);
    bson_destroy (query);
    return false;
}
