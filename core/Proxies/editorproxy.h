#ifndef EDITORPROXY_H
#define EDITORPROXY_H

/**
* @modulo EditorProxy
* @autor  Rodrigo Monteiro 1313493
* @descrição Módulo responsável por gerenciar a conexão entre o ambiente Lua e
*            a interface do editor de textos
* @versão
*   12/2014 1.0 Módulo inicial
*   12/2014 1.1 Módulo finalizado
**/

#include "../Interfaces/IEditor.h"
#include "../LuaWrapper/luna.h"

/**
 * @brief The EditorProxy class
 */
class EditorProxy
{
public:
    /**
     * @brief EditorProxy ctor
     * @param L the Lua environment pointer
     */
    EditorProxy(lua_State* L);

    /**
     * @brief write method to write a text at editor
     * @param L the Lua environment pointer
     * @return number of arguments to Lua
     */
    int write(lua_State* L);
    /**
     * @brief title method to write a title at editor
     * @param L the Lua environment pointer
     * @return number of arguments to Lua
     */
    int title(lua_State* L);
    /**
     * @brief name method to write a name at editor
     * @param L the Lua environment pointer
     * @return number of arguments to Lua
     */
    int name(lua_State* L);
    /**
     * @brief jumpLine jump a line at editor
     * @param L the Lua environment pointer
     * @return number of arguments to Lua
     */
    int jumpLine(lua_State* L);
    /**
     * @brief lineCount method to pass to Lua the number of lines in editor structure
     * @param L the Lua environment pointer
     * @return the line count
     */
    int lineCount(lua_State* L);
    /**
     * @brief registerEditor method to register an editor
     * @param editor the editor to be used in Lua
     */
    void registerEditor(IEditor* editor);

public:
    /**
     * methods used by Luna wrapper
     */
    static const char className[];
    static const Luna<EditorProxy>::RegType Register[];

private:
    /**
     * @brief m_editor pointer to editor
     */
    IEditor* m_editor;
    /**
     * @brief m_state pointer to Lua environment
     */
    lua_State* m_state;
};

#endif // EDITORPROXY_H
