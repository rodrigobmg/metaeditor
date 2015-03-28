#ifndef LUAPROCESSOR_H
#define LUAPROCESSOR_H

/**
* modulo LuaProcessor
* autor  Rodrigo Monteiro 1313493
* descrição Módulo responsável pelo gerenciamento da máquina Lua
* versão
*   10/2014 1.0 Módulo inicial
*   10/2014 1.1 Módulo finalizado
**/

#include <string>
#include <vector>
#include "lua.hpp"
#include "lobject.h"

#include "luacompiler.h"

/**
 * @brief Classe LuaProcessor
 */
class LuaProcessor
{
public:
    /**
     * @brief LuaProcessor ctor
     */
    explicit LuaProcessor();
    /**
     * @brief ~LuaProcessor dtor
     */
    ~LuaProcessor();

    /**
     * @brief LuaProcessor::compileScript Function to compile a script Lua
     * @param scriptPath Path of script Lua
     * @return true if was compiled. The compiled script will be stored at @see{LuaProcessor::m_data}
     * To retrieve the compiled script call @see{LuaProcessor::data()}
     */
    bool compileScript(std::string& scriptPath);
    /**
     * @brief registerFunction Register a C function to be used at Lua
     * @param fname the name of function
     * @param fn the pointer to function
     */
    void registerFunction(std::string &fname, lua_CFunction fn);
    /**
     * @brief callFunction Call a Lua function. The script will be compiled
     * @param script the name of script Lua
     * @param functionName the name of function
     */
    void callFunction(char* script, char* functionName, int numArgs = 0, int numRet = 0);
    /**
     * @brief doBuffer Put the script on the stack to be executed
     */
    void doBuffer();
    /**
     * @brief doBuffer Put the script on the stack to be executed
     * @param buffer The buffer from a compiled script
     */
    void doBuffer(const char* buffer, size_t size);
    /**
     * @brief state Pointer to current Lua machine
     * @return the pointer to Lua machine
     */
    lua_State* state();
    /**
     * @brief data binary data of compiled script
     * @return const pointer to compiled script
     */
    const char* data();
    /**
     * @brief dataSize
     * @return the size of compiled lua script
     */
    size_t dataSize();
private:

    /**
     * @brief m_state pointer to Lua machine
     */
    lua_State* m_state;
    /**
     * @brief m_data pointer to script lua
     */
    std::string* m_data;
    /**
     * @brief m_compiler pointer to @see{LuaCompiler}
     */
    LuaCompiler* m_compiler;
};

#endif // LUAPROCESSOR_H
