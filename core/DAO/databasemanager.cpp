#include <iostream>
#include "databasemanager.h"

#include "mongoc.h"

DatabaseManager::DatabaseManager()
{
    m_client = nullptr;
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

bool DatabaseManager::connect(const char *ip, uint16_t port)
{
    mongoc_init();
    char connectionString[256];
    sprintf(connectionString, "mongodb://%s:%hu", ip, port);
    m_client = mongoc_client_new (connectionString);

    BSON_ASSERT(m_client);

    if(!m_client)
    {
        return false;
    }



    return true;
}
