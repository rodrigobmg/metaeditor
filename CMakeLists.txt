cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(metaeditor CXX)

##Inclusões do cmakes
include(cmake/configure_libraries.cmake)
include(cmake/utils.cmake)

compiler_config()      	#definido no modulo utils.cmake
configure_libraries()	  #definido no modulo configure_libraries.cmake

##Declaração de opcoes para compilacao
set(enable_tests ON)

##Sub-Projetos
add_subdirectory(core)
add_subdirectory(cte)
add_subdirectory(data)
add_subdirectory(tests)
