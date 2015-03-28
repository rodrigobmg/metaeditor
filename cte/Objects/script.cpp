#include "script.h"

#include <DAO/Utils/bsonutils.h>
using utils = BSONUtils;

Script::Script()
{
}

Script::Script(std::string& name, char *binaryData, size_t dataSize) :
    Object(name)
{
    m_binaryData.append(binaryData, dataSize);
}

Script::~Script()
{
}

void Script::wrap(bson_t* data)
{
    Object::wrap(data);
    BSON_APPEND_BINARY(data, "binaryData", BSON_SUBTYPE_BINARY, reinterpret_cast<const unsigned char*>(m_binaryData.data()), m_binaryData.size());
}

bool Script::unwrap(const bson_t *data)
{
    if( Object::unwrap(data) == true )
    {
       return utils::getArray(data, "binaryData", m_binaryData);
    }

    return false;
}

void Script::setBinaryData(char *data, size_t size)
{
    m_binaryData.clear();
    m_binaryData.append(data, size);
}

const char* Script::binaryData()
{
    return m_binaryData.data();
}

const char* Script::binaryData() const
{
    return m_binaryData.data();
}

size_t Script::size()
{
    return m_binaryData.size();
}

size_t Script::size() const
{
    return m_binaryData.size();
}
