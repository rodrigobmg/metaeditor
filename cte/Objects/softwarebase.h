#ifndef SOFTWAREBASE_H
#define SOFTWAREBASE_H

#include <vector>
#include <map>
#include <DAO/object.h>
#include <LuaWrapper/luna.h>

class SoftwareBase : public Object
{
public:
    SoftwareBase();
    explicit SoftwareBase(const std::string& name);
    explicit SoftwareBase(const std::string& name, const std::string& project);

public:
    void wrap(bson_t *data);
    bool unwrap(const bson_t *data);

public: //Lua Functions
    int setProjectName(lua_State* L);
    int getProjectName(lua_State* L);
    int setCopyrights(lua_State* L);
    int getCopyrights(lua_State* L);
    int setVersion(lua_State* L);
    int getVersion(lua_State* L);
    int setDescription(lua_State* L);
    int getDescription(lua_State* L);
    int setObservations(lua_State* L);
    int getObservations(lua_State* L);

    static const char className[];
    static const Luna<SoftwareBase>::RegType Register[];

private:
    std::string m_projectName;
    std::string m_copyrights;
    std::string m_version;
    std::string m_description;
    std::string m_observations;
};

#endif // SOFTWAREBASE_H
