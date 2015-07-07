#include "softwarebase.h"

#include <DAO/Utils/bsonutils.h>
using utils = BSONUtils;

/**
 * @brief Lua
 */

const char SoftwareBase::className[] = "currentObject";
const Luna<SoftwareBase>::RegType SoftwareBase::Register[] =
{
    { "setProjectName", &SoftwareBase::setProjectName },
    { "getProjectName", &SoftwareBase::getProjectName },
    { "setCopyrights", &SoftwareBase::setCopyrights },
    { "getCopyrights", &SoftwareBase::getCopyrights },
    { "setVersion", &SoftwareBase::setVersion },
    { "getVersion", &SoftwareBase::getVersion },
    { "setDescription", &SoftwareBase::setDescription },
    { "getDescription", &SoftwareBase::getDescription },
    { "setObservations", &SoftwareBase::setObservations },
    { "getObservations", &SoftwareBase::getObservations },
    { 0 }
};


SoftwareBase::SoftwareBase()
{
}

SoftwareBase::SoftwareBase(const std::string &name)
    : Object(name), m_projectName(" "), m_copyrights(" "),
      m_version(" "), m_description(" "), m_observations(" ")
{

}

SoftwareBase::SoftwareBase(const std::string &name, const std::string &project)
    : Object(name), m_projectName(project), m_copyrights(" "),
      m_version(" "), m_description(" "), m_observations(" ")
{

}

void SoftwareBase::wrap(bson_t *data)
{
    Object::wrap(data);

    BSON_APPEND_UTF8(data, "projectName", m_projectName.data());
    BSON_APPEND_UTF8(data, "copyrights", m_copyrights.data());
    BSON_APPEND_UTF8(data, "version", m_version.data());
    BSON_APPEND_UTF8(data, "description", m_description.data());
    BSON_APPEND_UTF8(data, "observations", m_observations.data());
}

bool SoftwareBase::unwrap(const bson_t *data)
{
    if( Object::unwrap(data) == true )
    {
        m_projectName = utils::getString(data, "projectName");
        m_copyrights = utils::getString(data, "copyrights");
        m_version = utils::getString(data, "version");
        m_description = utils::getString(data, "description");
        m_observations = utils::getString(data, "observations");
        return true;
    }

    return false;
}

int SoftwareBase::setProjectName(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&m_projectName);
    return 1;
}

int SoftwareBase::getProjectName(lua_State *L)
{
    lua_pushstring(L, this->m_projectName.data());
    return 1;
}

int SoftwareBase::setCopyrights(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&m_copyrights);
    return 1;
}

int SoftwareBase::getCopyrights(lua_State *L)
{
    lua_pushstring(L, this->m_copyrights.data());
    return 1;
}

int SoftwareBase::setVersion(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&m_version);
    return 1;
}

int SoftwareBase::getVersion(lua_State *L)
{
    lua_pushstring(L, this->m_version.data());
    return 1;
}

int SoftwareBase::setDescription(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&m_description);
    return 1;
}

int SoftwareBase::getDescription(lua_State *L)
{
    lua_pushstring(L, this->m_description.data());
    return 1;
}

int SoftwareBase::setObservations(lua_State *L)
{
    lua_pushlightuserdata(L, (void*)&m_observations);
    return 1;
}

int SoftwareBase::getObservations(lua_State *L)
{
    lua_pushstring(L, this->m_observations.data());
    return 1;
}
