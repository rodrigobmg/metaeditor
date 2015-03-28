#include "object.h"
#include "Utils/bsonutils.h"

using utils = BSONUtils;

Object::Object()
{
    m_update = false;
}

Object::Object(const std::string& name) :
    m_name(name), m_update(false)
{
}

Object::~Object()
{
}

void Object::wrap(bson_t *data)
{
    if( m_update == false )
    {
        bson_oid_init(&m_id, NULL);
    }

    BSON_APPEND_OID(data, "_id", &m_id);
    BSON_APPEND_UTF8(data, "name", m_name.data());
}

bool Object::unwrap(const bson_t *data)
{
    m_name = utils::getString(data, "name");

    if( !m_name.empty() )
    {
        m_update = true;
        return BSONUtils::getOID(data, m_id);
    }

    return false;
}

//Properties
const std::string Object::Name()
{
    return m_name;
}

const std::string Object::Name() const
{
    return m_name;
}

void Object::setName(const std::string& name)
{
    m_name.clear();
    m_name.insert(0, name);
}

const bson_oid_t& Object::Id()
{
    return m_id;
}
