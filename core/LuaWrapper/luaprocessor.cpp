#include "luaprocessor.h"
#include <iostream>

///
/// @brief Macros
/// @author Rodrigo Monteiro
///

/**
 * @brief toproto Function to get lua proto
 * @param L Current lua_State
 * @param pos Position in stack
 * @return The proto
 */
static inline const Proto* toproto(lua_State* L, int pos)
{
    return getproto(L->top+(pos));
}

/**
 * @brief luaL_dobuffer Macro similar to lua_dostring
 * @param L Current lua_State
 * @param buff buffer containing the chunk of bytecodes Lua
 * @param size Size of chunk
 * @param name Name of chunk
 * @return True if the chunk was loaded
 */
static inline bool luaL_dobuffer(lua_State* L, const char* buff, size_t size, char* name)
{
    return luaL_loadbuffer(L, buff, size, name) || lua_pcall(L, 0, LUA_MULTRET, 0);
}

/**
 * @brief writer Function to write a compiled script at some buffer
 * @param L Current lua_State
 * @param chunk Chunk of bytecodes Lua to be writed
 * @param size Size of chunk
 * @param buff Buffer where the chunk will be writed
 * @return 0 is ok
 */
static int writer(lua_State* L, const void* chunk, size_t size, void* buff)
{
    UNUSED(L);
    ((std::string*)buff)->append((const char*)chunk, size);
    return 0;
}

///
/// \brief LuaProcessor::LuaProcessor
///
LuaProcessor::LuaProcessor()
{
    m_state = luaL_newstate();
    luaL_openlibs(m_state);
    m_compiler = new LuaCompiler;
    m_data = new std::string;
}

LuaProcessor::~LuaProcessor()
{
    lua_close(m_state);
    if(m_data)
    {
        m_data->clear();
        delete m_data;
    }

    if(m_compiler)
    {
        delete m_compiler;
    }
}

void LuaProcessor::callFunction(char* script, char* functionName, int numArgs, int numRet)
{
    std::string scr(script);
    compileScript(scr);

    int error = luaL_dobuffer(m_state, m_data->c_str(), m_data->size(), functionName);

    if( error )
    {
        lua_pop(m_state, 1);
    }

    lua_getglobal(m_state, functionName );
    error = lua_pcall(m_state, numArgs, numRet, 0);

    if( error )
    {
        lua_pop(m_state, 1);
    }
}

bool LuaProcessor::compileScript(std::string &scriptPath)
{
    const Proto* f;

    if(luaL_loadfile(m_state, scriptPath.c_str()) != 0)
    {
        lua_pop(m_state, 1);
        return false;
    }

    if( !m_data->empty() )
    {
        m_data->clear();
    }

    f = toproto(m_state, -1);
    lua_lock(m_state);

    if(m_compiler->compile(m_state, f, writer, m_data, true))
    {
        std::cout << "Falha na compilação do script " << scriptPath << std::endl;
        return false;
    }

    std::cout << "m_size = " << m_data->size() << std::endl;

    lua_unlock(m_state);
    return true;
}

void LuaProcessor::doBuffer()
{
    int error = luaL_dobuffer(m_state, m_data->data(), m_data->size(), (char*)"dummy");

    if( error )
    {
        lua_pop(m_state, 1);
    }
}

void LuaProcessor::doBuffer(const char* buffer)
{
    if( !m_data->empty() )
    {
        m_data->clear();
    }

    m_data->append(buffer);

    int error = luaL_dobuffer(m_state, buffer, strlen(buffer), (char*)"dummy");

    if( error )
    {
        lua_pop(m_state, 1);
    }
}

lua_State* LuaProcessor::state()
{
    return m_state;
}

const char* LuaProcessor::data()
{
    return m_data->data();
}

size_t LuaProcessor::dataSize()
{
    return m_data->size();
}

void LuaProcessor::registerFunction(std::string &fname, lua_CFunction fn)
{
    lua_register(m_state, fname.c_str(), fn);
}
