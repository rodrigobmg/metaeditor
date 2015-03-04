#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "mongoc.h"

#include "object.h"


typedef mongoc_client_t mongo_client;
const uint16_t MongoPortDefault = 27017;

class DatabaseException : public std::exception
{
private:
    char m_message[512];

public:
    explicit DatabaseException(const char* message);
    ~DatabaseException();
    virtual const char* what() const throw();
};

class DatabaseManager
{
private:
    explicit DatabaseManager();
    ~DatabaseManager();

public:
    static DatabaseManager& instance();
    void connect(const char* ip = "127.0.0.1", uint16_t port = MongoPortDefault) throw (DatabaseException);
    Object* read(const char* collection_name, const char* object_name) throw (DatabaseException);
    bool create(const bson_t* b);
    bool destroy(bson_t* b);
    bool update(const bson_t*b);

    bool exists(const char* object_name);

private:
    mongo_client* m_client;
};

#endif // DATABASEMANAGER_H
