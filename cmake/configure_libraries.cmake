macro(configure_libraries)

#Configuração do GTEST
set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/gtest-1.7.0)

#Configuração da biblioteca Lua
set(LUA_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua-5.2.3)
set(LUA_INCLUDE_DIR ${LUA_ROOT}/include)
set(LUA_LIBS_DIR ${LUA_ROOT}/lib)

#Configuração do driver MongoC
set(MONGOC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/libs/mongo-c-driver/bin)
set(MONGOC_INCLUDE_DIR ${MONGOC_ROOT}/include)
set(MONGOC_LIBS_DIR ${MONGOC_ROOT}/lib)

#Configuração do Redis

endmacro()
