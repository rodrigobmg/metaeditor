cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(core CXX C)

set(core_SOURCES
        LuaWrapper/luacompiler.cpp
        LuaWrapper/luaprocessor.cpp
        Proxies/editorproxy.cpp
        DAO/databasemanager.cpp
        DAO/object.cpp
        DAO/Utils/bsonutils.cpp)

set(core_HEADERS
        Interfaces/IEditor.h
        LuaWrapper/luacompiler.h
        LuaWrapper/luaprocessor.h
        LuaWrapper/luna.h
        Proxies/editorproxy.h
        DAO/databasemanager.h
        DAO/object.h
        DAO/Utils/bsonutils.h)

include_directories(${LUA_INCLUDE_DIR})
link_directories(${LUA_LIBS_DIR})

include_directories(${MONGOC_INCLUDE_DIR}/libbson-1.0)
include_directories(${MONGOC_INCLUDE_DIR}/libmongoc-1.0)
link_directories(${MONGOC_LIBS_DIR})

set(enable_tests ON)
if(enable_tests)
    message(STATUS "Enabling Tests")
    add_definitions(-D__LOG_TEST__)
endif()

add_library(core_static STATIC
                ${core_SOURCES}
                ${core_HEADERS})

target_link_libraries(core_static lua bson-1.0 mongoc-1.0)
