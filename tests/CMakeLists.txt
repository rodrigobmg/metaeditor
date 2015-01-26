cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(tests CXX C)

#configura compilador
include(../cmake/utils.cmake)

#adiciona a biblioteca core
include_directories(../core)
#adiciona biblioteca lua
include_directories(/usr/include/lua5.2/) #Inclusão do diretório onde estão os headers de Lua

#Procura o GTEST
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


set(tests_SOURCES
        test.cpp)

add_executable(tests ${tests_SOURCES})
target_link_libraries(tests core_static ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)

add_test(AllTests tests)
install(TARGETS tests ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
