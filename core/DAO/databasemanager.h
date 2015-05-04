#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>

#include "mongoc.h"
#include "object.h"

typedef mongoc_client_t mongo_client;

class DatabaseException : public std::exception
{
private:
    char m_message[512];

public:
    explicit DatabaseException(const char* message);
    virtual const char* what() const throw();
};


static const std::string DefaultDatabaseName = "talisman";
static const uint16_t MongoPortDefault = 27017;

inline void destroy_object(Object* obj){ delete obj; obj = nullptr;}

class DatabaseManager
{
private:
    explicit DatabaseManager();
    ~DatabaseManager();

    bool findQuery(bson_t *query, mongoc_collection_t* collection);

public:
    static DatabaseManager& instance();
    void tryReconnection();
    void connect(const char* ip = "127.0.0.1", uint16_t port = MongoPortDefault) throw (DatabaseException);

    template<typename T> std::vector<const T*> findAll(const std::string& collection_name, const std::string& database_name=DefaultDatabaseName) throw(DatabaseException);

    bool create(Object& o, const std::string& collection_name, const std::string& database_name=DefaultDatabaseName) throw (DatabaseException);
    template<typename T> T* read(const std::string& collection_name, const std::string& object_name, const std::string& database_name = DefaultDatabaseName) throw (DatabaseException);
    bool update(Object& o, const std::string& collection_name, const std::string& database_name=DefaultDatabaseName) throw (DatabaseException);
    bool destroy(Object& o, const std::string& collection_name, const std::string& database_name=DefaultDatabaseName) throw (DatabaseException);

private:
    bool findName(const std::string& object_name, mongoc_collection_t *collection);
    bool findOID(const bson_oid_t &id, mongoc_collection_t* collection);

private:
    mongo_client* m_client;
    const char* m_ip;
    uint16_t m_port;
};

template<typename T> T* DatabaseManager::read(const std::string& collection_name, const std::string& object_name, const std::string& database_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name.data(), collection_name.data());
    bson_t* query = BCON_NEW("name", object_name.data());
    mongoc_cursor_t* cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    const bson_t* bson;
    while( mongoc_cursor_next(cursor, &bson) )
    {
        T* obj = new T;
        if( obj->unwrap(bson) == true )
        {
            if( obj->Name().compare(object_name) == 0 )
            {
                return obj;
            }
        }

        destroy_object(obj);
    }

    return nullptr;
}

template <typename T> std::vector<const T*> DatabaseManager::findAll(const std::string& collection_name, const std::string& database_name) throw(DatabaseException)
{
    if(m_client == nullptr)
    {
        tryReconnection();
    }

    mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name.data(), collection_name.data());
    bson_t* query = bson_new();
    mongoc_cursor_t* cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

    const bson_t* bson;
    std::vector<const T*> objs;

    while(mongoc_cursor_next(cursor, &bson))
    {
        T* obj = new T;

        if( obj->unwrap(bson) == true )
        {
            objs.push_back(obj);
        }
    }

    return objs;
}

#endif // DATABASEMANAGER_H
