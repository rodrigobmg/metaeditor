cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(metaeditor CXX)

##Inclusões do cmakes
include(cmake/utils.cmake)
include(cmake/qt_config.cmake)

##Macros definidas
configure_qt() #incluido no modulo qt_config.cmake
compiler_config() #incluido no modulo utils.cmake

##Declaração de opções para compilação
set(enable_tests ON)


##Especificação dos módulos a serem compilados
set(metaeditor_SOURCES
        cte/main.cpp
        cte/plaineditor.cpp
        cte/Data/dataman.cpp
        cte/Ui/mainwindow.cpp
        cte/Utils/linenumberwidget.cpp
        cte/Utils/logfile.cpp)

set(metaeditor_HEADERS
        cte/plaineditor.h
        cte/Data/dataman.h
        cte/Ui/mainwindow.h
        cte/Utils/linenumberwidget.h
        cte/Utils/logfile.h)

set(metaeditor_FORMS
        cte/Forms/mainwindow.ui)

##QT macros
qt5_wrap_cpp(metaeditor_HEADERS_MOC  ${metaeditor_HEADERS} )
qt5_wrap_ui(metaeditor_FORMS_HEADERS ${metaeditor_FORMS})

FIND_PROGRAM(QT_UIC_EXECUTABLE uic
  $ENV{QTDIR}/bin
)

##Sub-Projetos
add_subdirectory(core)
add_subdirectory(tests)

##diretórios de inclusão
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(core)
include_directories(/usr/include/lua5.2/) #Inclusão do diretório onde estão os headers de Lua
include_directories(/usr/local/include/libbson-1.0)
include_directories(/usr/local/include/libmongoc-1.0)

if(enable_tests)
    message(STATUS "Enabling Tests")
    add_definitions(-D__LOG_TEST__)
endif()

add_executable(metaeditor ${metaeditor_SOURCES}
                          ${metaeditor_FORMS_HEADERS})

target_link_libraries(metaeditor core_static ${Qt5Widgets_LIBRARIES})
install(TARGETS metaeditor ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

