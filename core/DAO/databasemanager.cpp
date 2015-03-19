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

///////////////
/// \brief DatabaseManager
///
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
        obj = new Object();

        if( obj->unwrap(doc) == true )
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

bool DatabaseManager::create(Object& o, const char *collection_name, const char *database_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        //Pode levantar exceção
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection (m_client, database_name, collection_name);

    if(findName(o.Name(), collection) == true)
    {
        mongoc_collection_destroy (collection);
        return false;
    }

    bson_error_t error;
    bson_t b = BSON_INITIALIZER;

    o.wrap(&b);

    if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, &b, NULL, &error))
    {
        std::cout << error.message << std::endl;
        mongoc_collection_destroy (collection);
        return false;
    }

    mongoc_collection_destroy (collection);
    return true;
}

bool DatabaseManager::destroy(Object& o, const char *collection_name, const char *database_name) throw (DatabaseException)
{
    if(m_client == nullptr)
    {
        //Pode levantar exceção
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name, collection_name);

    if(!findName(o.Name(), collection))
    {
        mongoc_collection_destroy(collection);
        return false;
    }

    bson_t b = BSON_INITIALIZER;
    o.wrap(&b);

    bson_error_t error;
    if (!mongoc_collection_remove (collection, MONGOC_REMOVE_NONE, &b, NULL, &error))
    {
        std::cout << "Erro ao remover: " << o.Name();
        std::cout << std::endl << "mensagem: " << error.message << std::endl;
        mongoc_collection_destroy(collection);
        return false;
    }

    mongoc_collection_destroy(collection);
    return true;
}

bool DatabaseManager::update(Object& o, const char* collection_name, const char* database_name) throw (DatabaseException)
{
    if(m_client == nullptr)
    {
        //Pode levantar exceção
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name, collection_name);

    //Caso o nome já exista ou o objeto não possui um ID válido
    //retorna falso
    if(findName(o.Name(), collection) || o.Id() == nullptr)
    {
        mongoc_collection_destroy(collection);
        return false;
    }

    //Se o objeto possui um ID válido, mas ele não foi encontrado no
    // banco de dados, retorna falso
    bson_oid_t oid;
    bson_oid_init_from_string(&oid, o.Id());

    if(!findOID(&oid, collection))
    {
        mongoc_collection_destroy(collection);
        return false;
    }

    //Busca por ID
    bson_t query = BSON_INITIALIZER;
    BSON_APPEND_OID(&query, "_id", &oid);

    bson_t update = BSON_INITIALIZER;
    bson_t set = BSON_INITIALIZER;

    bson_append_document_begin(&update, "$set", 4, &set);
    o.wrap(&set);
    bson_append_document_end(&update, &set);

    bson_error_t error;

    if (!mongoc_collection_update (collection, MONGOC_UPDATE_NONE, &query, &update, NULL, &error))
    {
        std::cout << "Erro ao atualizar: " << o.Name();
        std::cout << std::endl << "mensagem: " << error.message << std::endl;
        mongoc_collection_destroy(collection);
        return false;
    }

    mongoc_collection_destroy(collection);
    return true;
}

bool DatabaseManager::findName(const char *object_name, mongoc_collection_t* collection)
{
    bson_t* query = BCON_NEW ("$query", "{", "name", BCON_UTF8(object_name), "}");
    return findQuery(query, collection);
}

bool DatabaseManager::findOID(bson_oid_t* id, mongoc_collection_t *collection)
{
    bson_t* query = BCON_NEW ("$query", "{", "_id", BCON_OID(id), "}");
    return findQuery(query, collection);
}

bool DatabaseManager::findQuery(bson_t *query, mongoc_collection_t *collection)
{
    bool result = false;

    mongoc_cursor_t *cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);
    const bson_t* b;

    if(mongoc_cursor_next(cursor, &b))
    {
        result = true;
    }

    mongoc_cursor_destroy(cursor);
    bson_destroy (query);
    return result;
}
