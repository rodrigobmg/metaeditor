#include "luacompiler.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYINT(s)	(s[0]-'0')
#define VERSION		MYINT(LUA_VERSION_MAJOR)*16+MYINT(LUA_VERSION_MINOR)
#define FORMAT		0		/* this is the official format */


LuaCompiler::LuaCompiler()
{
}

/*
* make header for precompiled chunks
* if you change the code below be sure to update LoadHeader and FORMAT above
* and LUAC_HEADERSIZE in lundump.h
*/
void LuaCompiler::header (lu_byte* h)
{
    int x=1;
    memcpy(h,LUA_SIGNATURE,sizeof(LUA_SIGNATURE)-sizeof(char));
    h+=sizeof(LUA_SIGNATURE)-sizeof(char);
    *h++=cast_byte(VERSION);
    *h++=cast_byte(FORMAT);
    *h++=cast_byte(*(char*)&x);			/* endianness */
    *h++=cast_byte(sizeof(int));
    *h++=cast_byte(sizeof(size_t));
    *h++=cast_byte(sizeof(Instruction));
    *h++=cast_byte(sizeof(lua_Number));
    *h++=cast_byte(((lua_Number)0.5)==0);		/* is lua_Number integral? */
    memcpy(h,LUAC_TAIL,sizeof(LUAC_TAIL)-sizeof(char));
}

void LuaCompiler::dumpBlock(const void* b, size_t size)
{
    if (m_status==0)
    {
        lua_unlock(m_state);
        m_status=(*m_writer)(m_state,b,size,m_data);
        lua_lock(m_state);
    }
}

void LuaCompiler::dumpChar(int y)
{
    char x=(char)y;
    dumpVar(&x, sizeof(x));
}

void LuaCompiler::dumpInt(int x)
{
    dumpVar(&x, sizeof(x));
}

void LuaCompiler::dumpNumber(lua_Number x)
{
    dumpVar(&x, sizeof(x));
}

void LuaCompiler::dumpVector(const void* b, int n, size_t size)
{
    dumpInt(n);
    dumpMem(b,n,size);
}

void LuaCompiler::dumpString(const TString* s)
{
    if (s==NULL)
    {
        size_t size=0;
        dumpVar(&size, sizeof(size));
    }
    else
    {
        size_t size=s->tsv.len+1;		/* include trailing '\0' */
        dumpVar(&size, sizeof(size));
        dumpBlock(getstr(s),size*sizeof(char));
    }
}

void LuaCompiler::dumpCode(const Proto* f)
{
    dumpVector(f->code,f->sizecode,sizeof(Instruction));
}

void LuaCompiler::dumpConstants(const Proto* f)
{
    int i,n=f->sizek;
    dumpInt(n);
    for (i=0; i<n; i++)
    {
        const TValue* o=&f->k[i];
        dumpChar(ttypenv(o));
        switch (ttypenv(o))
        {
        case LUA_TNIL:
            break;
        case LUA_TBOOLEAN:
            dumpChar(bvalue(o));
            break;
        case LUA_TNUMBER:
            dumpNumber(nvalue(o));
            break;
        case LUA_TSTRING:
            dumpString(rawtsvalue(o));
            break;
        default: lua_assert(0);
        }
    }
    n=f->sizep;
    dumpInt(n);
    for (i=0; i<n; i++) dumpFunction(f->p[i]);
}

void LuaCompiler::dumpUpvalues(const Proto* f)
{
    int i,n=f->sizeupvalues;
    dumpInt(n);
    for (i=0; i<n; i++)
    {
        dumpChar(f->upvalues[i].instack);
        dumpChar(f->upvalues[i].idx);
    }
}

void LuaCompiler::dumpDebug(const Proto* f)
{
    int i,n;
    dumpString((m_strip) ? NULL : f->source);
    n= (m_strip) ? 0 : f->sizelineinfo;
    dumpVector(f->lineinfo,n,sizeof(int));
    n= (m_strip) ? 0 : f->sizelocvars;
    dumpInt(n);
    for (i=0; i<n; i++)
    {
        dumpString(f->locvars[i].varname);
        dumpInt(f->locvars[i].startpc);
        dumpInt(f->locvars[i].endpc);
    }
    n= (m_strip) ? 0 : f->sizeupvalues;
    dumpInt(n);
    for (i=0; i<n; i++) dumpString(f->upvalues[i].name);
}

void LuaCompiler::dumpFunction(const Proto* f)
{
    dumpInt(f->linedefined);
    dumpInt(f->lastlinedefined);
    dumpChar(f->numparams);
    dumpChar(f->is_vararg);
    dumpChar(f->maxstacksize);
    dumpCode(f);
    dumpConstants(f);
    dumpUpvalues(f);
    dumpDebug(f);
}

void LuaCompiler::dumpHeader()
{
    lu_byte h[LUAC_HEADERSIZE];
    header(h);
    dumpBlock(h,LUAC_HEADERSIZE);
}

void LuaCompiler::dumpMem(const void* b, int n, size_t size)
{
    dumpBlock(b,(n)*(size));
}

void LuaCompiler::dumpVar(const void* x, size_t size)
{
    dumpMem(x, 1, size);
}
/*
** dump Lua function as precompiled chunk
*/
int LuaCompiler::compile(lua_State* L, const Proto* f, lua_Writer w, void* data, bool strip)
{
    m_state = L;
    m_writer = w;
    m_data = data;
    m_strip = strip;
    m_status = 0;

    dumpHeader();
    dumpFunction(f);

    return m_status;
}
