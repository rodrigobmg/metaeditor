cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(tests CXX C)

#configura compilador
include(../cmake/utils.cmake)
include(../cmake/configure_libraries.cmake)

#adiciona a biblioteca core
include_directories(../core)
#adiciona biblioteca lua
include_directories(${LUA_INCLUDE_DIR}) #Inclusão do diretório onde estão os headers de Lua
#adiciona a bibliotecas mongo
include_directories(${MONGOC_INCLUDE_DIR}/libbson-1.0)
include_directories(${MONGOC_INCLUDE_DIR}/libmongoc-1.0)

#Procura o GTEST
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#Executável 1
set(tests_SOURCES
        test.cpp)

add_executable(tests ${tests_SOURCES})
target_link_libraries(tests core_static ${GTEST_BOTH_LIBRARIES} pthread)
add_test(AllTests tests)
install(TARGETS tests ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

#Executável 2
set(core_tests_SOURCES
        test_corelib_dbmng.cpp)

add_executable(test_corelib_dbmng ${core_tests_SOURCES})
target_link_libraries(test_corelib_dbmng core_static ${GTEST_BOTH_LIBRARIES} pthread)
add_test(AllTests test_corelib_dbmng)
install(TARGETS test_corelib_dbmng ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

