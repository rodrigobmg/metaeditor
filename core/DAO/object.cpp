#include "object.h"
#include "Utils/bsonutils.h"

using utils = BSONUtils;

Object::Object() :
    m_data(nullptr), m_id(nullptr), m_name(nullptr)
{
}

Object::Object(const bson_t *data) :
    m_id(nullptr), m_name(nullptr)
{
    if( m_data != nullptr )
    {
        m_data = bson_copy(data);
    }
}

Object::Object(char *name) :
    m_data(nullptr), m_name(name), m_id(nullptr)
{

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

#include <iostream>
void Object::wrap()
{
    if(m_data == nullptr)
    {
        bson_oid_t oid;
        bson_oid_init(&oid, NULL);
        m_id = new char[MaxIdSize];
        bson_oid_to_string(&oid, m_id);
        m_data = bson_new();
        BSON_APPEND_OID(m_data, "_id", &oid);
        BSON_APPEND_UTF8(m_data, "name", m_name);
    }
}

bool Object::unwrap()
{
    std::cout << "m_data: " << m_data << std::endl;

    if( m_data != nullptr )
    {
        char* name = utils::getString(m_data, "name");
        int name_len = strlen(name);

        if( name_len > 0)
        {
            m_name = new char[name_len+1];
            strcpy(m_name, name);

            m_id = new char[MaxIdSize];
            if( BSONUtils::getOID(m_data, m_id) == true )
            {
                return true;
            }

            return false;
        }
    }

    return false;
}

//Properties
const char* Object::Name()
{
    return m_name;
}

const char *Object::Id()
{
    return m_id;
}

const bson_t* Object::Data()
{
    if( m_data == nullptr )
    {
        this->wrap();
    }

    return m_data;
}
