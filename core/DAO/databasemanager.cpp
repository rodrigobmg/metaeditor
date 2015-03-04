#include <iostream>
#include "databasemanager.h"

#include "mongoc.h"
#include <bson-iter.h>
#include "object.h"

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

    bson_destroy (&reply);
    bson_destroy (command);
}

Object* DatabaseManager::read(const char *collection_name, const char* object_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        //ToDo: Tentar realizar conexão novamente
        throw DatabaseException("Conexão com o banco de dados não existente.");
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, "talisman", collection_name);
    bson_t* query = bson_new();
    BSON_APPEND_UTF8 (query, "name", object_name);
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

        delete obj;
        obj = nullptr;
    }

    return obj;
}
