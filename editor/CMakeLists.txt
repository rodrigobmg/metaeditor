cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(cte CXX C)

##Inclusões do cmakes
include(../cmake/qt_config.cmake)
include(../cmake/utils.cmake)

##Macros definidas
configure_qt()         	#definido no modulo qt_config.cmake
compiler_config()

##Especificação dos módulos a serem compilados
set(metaeditor_SOURCES
        Ui/mainwindow.cpp
        Data/redislayer.cpp
        main.cpp
        editor.cpp)

##Especificação dos cabeçalhos do projeto
set(metaeditor_HEADERS
        Ui/mainwindow.h
        Data/redislayer.h
        editor.h
        Lua/luna.h)

set(metaeditor_FORMS
        Ui/Forms/mainwindow.ui)

##QT macros
qt5_wrap_cpp(metaeditor_HEADERS_MOC  ${metaeditor_HEADERS})
qt5_wrap_ui(metaeditor_FORMS_HEADERS ${metaeditor_FORMS})

FIND_PROGRAM(QT_UIC_EXECUTABLE uic
  $ENV{QTDIR}/bin
)

##diretórios de inclusão
include_directories(${LUA_INCLUDE_DIR}) #Inclusão do diretório onde estão os headers de Lua

add_executable(metaeditor ${metaeditor_SOURCES}
                          ${metaeditor_FORMS_HEADERS})

target_link_libraries(metaeditor lua redox_static ${Qt5Widgets_LIBRARIES} hiredis ev pthread)
install(TARGETS metaeditor ARCHIVE RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
