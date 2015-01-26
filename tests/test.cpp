#include <gtest/gtest.h>
#include <LuaWrapper/luaprocessor.h>
#include <LuaWrapper/luna.h>
#include <Interfaces/IEditor.h>
#include <Proxies/editorproxy.h>

#include <iostream>

class MOC_Editor : public IEditor
{
public:

    char* title;
    char* name;

    ~MOC_Editor()
    {
        delete[] title;
        delete[] name;
    }

    virtual void addTitle(const char* str)
    {
        title = new char[strlen(str)];
        strcpy(title, str);
    }

    virtual void addName(const char* str)
    {
        name = new char[strlen(str)];
        strcpy(name, str);
    }

    virtual int currentLine(){ return 0; }
    virtual int currentColumn(){ return 0; }
    virtual void jumpLine(){}
    virtual int structureLineCount(){ return -1;}
    virtual void writeText(const char* str){}
};

TEST(LuaProcessor, TestCompileAndRunScriptTrue)
{
    LuaProcessor lp;
    lp.callFunction("./scripts/testLua.lua", "hello", 0, 1);
    int ret = lua_tonumber(lp.state(), -1);
    EXPECT_EQ(ret, 1);
}

TEST(LuaProcessor, TestCompileAndRunScriptFalse)
{
    LuaProcessor lp;
    lp.callFunction("./scripts/testLua.lua", "hello", 0, 1);
    int ret = lua_tonumber(lp.state(), -1);
    EXPECT_FALSE(ret==2);
}

TEST(EditorProxy, TestProxy)
{
    LuaProcessor lp;
    MOC_Editor editor;
    EditorProxy proxy(lp.state());
    Luna<EditorProxy>::Register(lp.state(), &proxy);
    proxy.registerEditor(&editor);

    lp.callFunction("./scripts/testLua.lua", "editorTest");
    EXPECT_STREQ(editor.title, "Title");
    EXPECT_STREQ(editor.name, "Name");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
