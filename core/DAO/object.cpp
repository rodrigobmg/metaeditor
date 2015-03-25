#include "object.h"
#include "Utils/bsonutils.h"

using utils = BSONUtils;

Object::Object() :
    /*m_data(nullptr), */m_id(nullptr), m_name(nullptr)
{
}
/*
Object::Object(const bson_t *data) :
    m_id(nullptr), m_name(nullptr)
{
    if( m_data != nullptr )
    {
        m_data = bson_copy(data);
    }
}
*/
Object::Object(char *name) :
    /*m_data(nullptr),*/ m_id(nullptr), m_name(name)
{
}

Object::~Object()
{
}

void Object::wrap(bson_t *data)
{
    bson_oid_t oid;

    if( m_id == nullptr )
    {
        bson_oid_init(&oid, NULL);
        m_id = new char[MaxIdSize];
        bson_oid_to_string(&oid, m_id);
    }
    else
    {
        bson_oid_init_from_string(&oid, m_id);
    }

    BSON_APPEND_OID(data, "_id", &oid);
    BSON_APPEND_UTF8(data, "name", m_name);
}

/*{
    if(m_data != nullptr)
    {
        bson_destroy(m_data);
    }

    //Caso estejamos lidando com um objeto novo,
    //é necessário criar um novo id para ele.
    //Caso contrário estamos atualizando o objeto,
    //logo usamos o id existente.
    bson_oid_t oid;

    if( m_id == nullptr )
    {
        bson_oid_init(&oid, NULL);
        m_id = new char[MaxIdSize];
    }

    bson_oid_to_string(&oid, m_id);
    m_data = bson_new();
    BSON_APPEND_OID(m_data, "_id", &oid);
    BSON_APPEND_UTF8(m_data, "name", m_name);
}*/

bool Object::unwrap(const bson_t *data)
{
    //if( m_data != nullptr )
    //{
        char* name = utils::getString(data, "name");
        int name_len = strlen(name);

        if( name_len > 0)
        {
            m_name = new char[name_len+1];
            strcpy(m_name, name);

            m_id = new char[MaxIdSize];
            return BSONUtils::getOID(data, m_id);
        }

        return false;
    //}

    //return false;
}

//Properties
const char* Object::Name()
{
    return m_name;
}

void Object::setName(char *name)
{
    if( name != nullptr )
    {
        if( m_name != nullptr )
        {
            delete m_name;
        }

        m_name = name;
    }
}

const char *Object::Id()
{
    return m_id;
}
/*
const bson_t* Object::Data()
{
    /*if( m_data == nullptr)
    {
        this->wrap();
    }*

    return m_data;
}
*/
