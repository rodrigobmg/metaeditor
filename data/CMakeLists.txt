cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(data CXX C)

set(data_SOURCES
        patterns/subject.cpp
        dataaccess.cpp)

set(data_HEADERS
        patterns/observer.h
        patterns/subject.h
        dataaccess.h)

add_library(data_static STATIC
                ${data_SOURCES}
                ${data_HEADERS})

target_link_libraries(data_static hiredis ev pthread)
