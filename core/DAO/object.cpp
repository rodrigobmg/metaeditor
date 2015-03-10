#include "object.h"
#include "Utils/bsonutils.h"

using utils = BSONUtils;

Object::Object(const bson_t *data) :
    m_name(nullptr), m_id(nullptr)
{
    m_data = bson_copy(data);
}

Object::~Object()
{
    if( m_data != nullptr)
    {
        bson_destroy(m_data);
    }

    if( m_name != nullptr )
    {
        delete m_name;
    }

    if( m_id != nullptr )
    {
        delete m_id;
    }
}

bool Object::serialize(char *json)
{
    return false;
}

bool Object::unserialize()
{
    char* name = utils::getString(m_data, "name");
    int name_len = strlen(name);

    if( name_len > 0 )
    {
        m_name = new char[name_len+1];
        strcpy(m_name, name);
        return true;
    }

    return false;
}

const char* Object::Name()
{
    return m_name;
}

const char *Object::Id()
{
    return m_id;
}
