#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "mongoc.h"


typedef mongoc_client_t mongo_client;
const uint16_t MongoPortDefault = 27017;

class DatabaseManager
{
private:
    explicit DatabaseManager();
    ~DatabaseManager();

public:
    static DatabaseManager& instance();
    bool openConnection(const char* ip = "127.0.0.1", uint16_t port = MongoPortDefault);

private:
    mongo_client* m_client;
};

#endif // DATABASEMANAGER_H
