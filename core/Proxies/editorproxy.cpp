#include "editorproxy.h"

const char EditorProxy::className[] = "editor";
const Luna<EditorProxy>::RegType EditorProxy::Register[] =
{
    { "jumpline", &EditorProxy::jumpLine },
    { "createfield", &EditorProxy::createField },
    { "indent", &EditorProxy::indent },
    { 0 }
};

EditorProxy::EditorProxy(lua_State *L)
{
}

void EditorProxy::registerEditor(IEditor *editor)
{
    m_editor = editor;
}

///////////////////////////////////////////
/// Lua Functions
///
int EditorProxy::jumpLine(lua_State* /*L*/)
{
    m_editor->jumpLine();
    return -1;
}

int EditorProxy::createField(lua_State* L)
{
    int type = luaL_checkinteger(L, 2);
    size_t size;
    const char* str = luaL_checklstring(L, 3, &size);
    std::string text(str, size);
    std::string* callback = nullptr;

    if(type != 3)
    {
        callback = (std::string*)lua_touserdata(L, 4);
    }

    m_editor->createField(type, text, callback);
    return -1;
}

int EditorProxy::indent(lua_State *L)
{
    int many = luaL_checkinteger(L, 2);
    m_editor->moveCursor(many);
    return -1;
}
