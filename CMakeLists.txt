cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(metaeditor CXX)

##Inclusões do cmakes
include(cmake/utils.cmake)
include(cmake/qt_config.cmake)
include(cmake/configure_libraries.cmake)

##Macros definidas
configure_qt()         	#definido no modulo qt_config.cmake
compiler_config()      	#definido no modulo utils.cmake
configure_libraries()	#definido no módulo configure_libraries.cmake

##Declaração de opções para compilação
set(enable_tests ON)

##Especificação dos módulos a serem compilados
set(metaeditor_SOURCES
        cte/main.cpp
        cte/plaineditor.cpp
        cte/Data/dataman.cpp
        cte/Ui/mainwindow.cpp
        cte/Utils/linenumberwidget.cpp
        cte/Utils/logfile.cpp
##Objetos utilizados pelo editor
        cte/Objects/softwarebase.cpp
        cte/Objects/user.cpp
        cte/Objects/script.cpp)

##Especificação dos cabeçalhos do projeto
set(metaeditor_HEADERS
        cte/plaineditor.h
        cte/Data/dataman.h
        cte/Ui/mainwindow.h
        cte/Utils/linenumberwidget.h
        cte/Utils/logfile.h
##Objetos utilizados pelo editor
        cte/Objects/softwarebase.h
        cte/Objects/user.h
        cte/Objects/script.h)

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
include_directories(${LUA_INCLUDE_DIR}) #Inclusão do diretório onde estão os headers de Lua
include_directories(${MONGOC_INCLUDE_DIR}/libbson-1.0)
include_directories(${MONGOC_INCLUDE_DIR}/libmongoc-1.0)
include_directories(core)

if(enable_tests)
    message(STATUS "Enabling Tests")
    add_definitions(-D__LOG_TEST__)
endif()

add_executable(metaeditor ${metaeditor_SOURCES}
                          ${metaeditor_FORMS_HEADERS})

target_link_libraries(metaeditor core_static ${Qt5Widgets_LIBRARIES})
install(TARGETS metaeditor ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

