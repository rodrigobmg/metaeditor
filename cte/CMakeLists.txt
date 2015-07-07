cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(cte CXX C)

##Inclusões do cmakes
include(../cmake/qt_config.cmake)

##Macros definidas
configure_qt()         	#definido no modulo qt_config.cmake

##Especificação dos módulos a serem compilados
set(metaeditor_SOURCES
        main.cpp
        plaineditor.cpp
        Data/linemanager.cpp
        Ui/mainwindow.cpp
        Ui/scriptdialog.cpp
        Ui/bswdialog.cpp
        Ui/listbsw.cpp
        Utils/linenumberwidget.cpp
        Utils/logfile.cpp
##Objetos utilizados pelo editor
        Objects/softwarebase.cpp
        Objects/user.cpp
        Objects/script.cpp)

##Especificação dos cabeçalhos do projeto
set(metaeditor_HEADERS
        plaineditor.h
        Data/linemanager.h
        Ui/mainwindow.h
        Ui/scriptdialog.h
        Ui/bswdialog.h
        Ui/listbsw.h
        Utils/linenumberwidget.h
        Utils/logfile.h
##Objetos utilizados pelo editor
        Objects/softwarebase.h
        Objects/user.h
        Objects/script.h)

set(metaeditor_FORMS
        Ui/Forms/mainwindow.ui
        Ui/Forms/scriptdialog.ui
        Ui/Forms/bswdialog.ui
        Ui/Forms/listbsw.ui)

##QT macros
qt5_wrap_cpp(metaeditor_HEADERS_MOC  ${metaeditor_HEADERS} )
qt5_wrap_ui(metaeditor_FORMS_HEADERS ${metaeditor_FORMS})

FIND_PROGRAM(QT_UIC_EXECUTABLE uic
  $ENV{QTDIR}/bin
)

##diretórios de inclusão
include_directories(${LUA_INCLUDE_DIR}) #Inclusão do diretório onde estão os headers de Lua
include_directories(${MONGOC_INCLUDE_DIR}/libbson-1.0)
include_directories(${MONGOC_INCLUDE_DIR}/libmongoc-1.0)
include_directories(../core)

if(enable_tests)
    message(STATUS "Enabling Tests")
    add_definitions(-D__LOG_TEST__)
endif()

add_executable(metaeditor ${metaeditor_SOURCES}
                          ${metaeditor_FORMS_HEADERS})

target_link_libraries(metaeditor core_static ${Qt5Widgets_LIBRARIES})
install(TARGETS metaeditor ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
