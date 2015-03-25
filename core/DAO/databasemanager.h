#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "mongoc.h"
#include "object.h"


typedef mongoc_client_t mongo_client;

class DatabaseException : public std::exception
{
private:
    char m_message[512];

public:
    explicit DatabaseException(const char* message);
    ~DatabaseException();
    virtual const char* what() const throw();
};


static const char * DefaultDatabaseName = "talisman";
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

    template<typename T> T* read(const char* collection_name, const char* object_name, const char* database_name=DefaultDatabaseName) throw (DatabaseException)
    {
        if(m_client == nullptr)
        {
            tryReconnection();
        }

        mongoc_collection_t* collection = mongoc_client_get_collection(m_client, database_name, collection_name);
        bson_t* query = BCON_NEW("name", object_name);
        mongoc_cursor_t* cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

        const bson_t* doc;
        while( mongoc_cursor_next(cursor, &doc) )
        {
            T* obj = new T;

            if( obj->unwrap(doc) == true )
            {
                if( strcmp(obj->Name(), object_name) == 0 )
                {
                    return obj;
                }
            }

            destroy_object(obj);
        }

        return nullptr;
    }

    bool create(Object& o, const char* collection_name, const char* database_name=DefaultDatabaseName) throw (DatabaseException);
    bool destroy(Object& o, const char* collection_name, const char* database_name=DefaultDatabaseName) throw (DatabaseException);
    bool update(Object& o, const char* collection_name, const char* database_name=DefaultDatabaseName) throw (DatabaseException);

    bool findName(const char* object_name, mongoc_collection_t *collection);
    bool findOID(bson_oid_t *id, mongoc_collection_t* collection);

private:
    mongo_client* m_client;
    const char* m_ip;
    uint16_t m_port;
};

#endif // DATABASEMANAGER_H
