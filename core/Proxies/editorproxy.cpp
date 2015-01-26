#include "editorproxy.h"

const char EditorProxy::className[] = "editor";
const Luna<EditorProxy>::RegType EditorProxy::Register[] =
{
    { "title", &EditorProxy::title },
    { "name", &EditorProxy::name },
    { "jumpline", &EditorProxy::jumpLine },
#ifdef __LOG_TEST__
    { "lines", &EditorProxy::lineCount },
    { "write", &EditorProxy::write },
#endif
    { 0 }
};

EditorProxy::EditorProxy(lua_State *L)
{
    m_state = L;
}

int EditorProxy::jumpLine(lua_State* /*L*/)
{
    m_editor->jumpLine();
    return -1;
}

int EditorProxy::title(lua_State *L)
{
    size_t size;
    const char* str = luaL_checklstring(L, 2, &size);
    m_editor->addTitle(str);
    return size;
}

int EditorProxy::name(lua_State *L)
{
    size_t size;
    const char* str = luaL_checklstring(L, 2, &size);
    m_editor->addName(str);
    return size;
}

void EditorProxy::registerEditor(IEditor *editor)
{
    m_editor = editor;
}

#ifdef __LOG_TEST__
int EditorProxy::write(lua_State *L)
{
    size_t size;
    const char* str = luaL_checklstring(L, 2, &size);
    m_editor->writeText(str);
    return size;
}

int EditorProxy::lineCount(lua_State *L)
{
    int lineCount = m_editor->structureLineCount();
    lua_pushinteger(L, (lineCount + 1));
    return 1;
}
#endif
