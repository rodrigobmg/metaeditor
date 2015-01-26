cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(core CXX C)

include(../cmake/utils.cmake)

set(core_SOURCES
        LuaWrapper/luacompiler.cpp
        LuaWrapper/luaprocessor.cpp
        Proxies/editorproxy.cpp
        DAO/databasemanager.cpp)

set(core_HEADERS
        Interfaces/IEditor.h
        LuaWrapper/luacompiler.h
        LuaWrapper/luaprocessor.h
        LuaWrapper/luna.h
        Proxies/editorproxy.h
        DAO/databasemanager.h)

include_directories(/usr/include/lua5.2/) #Inclusão do diretório onde estão os headers de Lua
link_directories(/usr/local/lib/lua/)

include_directories(/usr/local/include/libbson-1.0)
include_directories(/usr/local/include/libmongoc-1.0)
link_directories(/usr/local/lib/libmongoc-1.0)
link_directories(/usr/local/lib/libbson-1.0)

set(enable_tests ON)
if(enable_tests)
    message(STATUS "Enabling Tests")
    add_definitions(-D__LOG_TEST__)
endif()

add_library(core_static STATIC
                ${core_SOURCES}
                ${core_HEADERS})

target_link_libraries(core_static lua bson-1.0 mongoc-1.0)
