#include "script.h"

#include <DAO/Utils/bsonutils.h>
using utils = BSONUtils;

Script::Script() :
    m_binaryData(nullptr), m_dataSize(0)
{
}

Script::Script(char *name, char *binaryData, size_t dataSize) :
    Object(name), m_binaryData(nullptr), m_dataSize(dataSize)
{
    m_binaryData = new char[dataSize];
    memcpy(m_binaryData, binaryData, dataSize);
}

Script::~Script()
{
    if( m_binaryData )
    {
        delete m_binaryData;
    }
}

void Script::wrap(bson_t* data)
{
    Object::wrap(data);
    BSON_APPEND_BINARY(data, "binaryData", BSON_SUBTYPE_BINARY, (const unsigned char*)m_binaryData, m_dataSize);
}

bool Script::unwrap(const bson_t *data)
{
    if( Object::unwrap(data) == true )
    {
       m_binaryData = utils::getArray(data, "binaryData", m_dataSize);
       return m_binaryData != nullptr;
    }

    return false;
}

void Script::setBinaryData(char *data, size_t size)
{
    if(m_binaryData)
    {
        delete m_binaryData;
    }

    m_binaryData = new char[size];
    memcpy(m_binaryData, data, size);
}

const char* Script::binaryData()
{
    return m_binaryData;
}

size_t Script::size()
{
    return m_dataSize;
}
