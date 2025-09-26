project(SVSCraftGui
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)

set(_qt_links)
set(_links_private)
set(_include_private)
set(_link_directories_private)
if(NOT WIN32 AND NOT APPLE)
    set(_qt_links DBus)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GIO REQUIRED gio-2.0)
    set(_links_private ${GIO_LIBRARIES})
    set(_include_private ${GIO_INCLUDE_DIRS})
    set(_link_directories_private ${GIO_LIBRARY_DIRS})
endif()

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS SVSCraftCore
    LINKS_PRIVATE SVSCraft3rdParty_tinyexpr ${_links_private}
    QT_LINKS Core Gui ${_qt_links}
    QT_INCLUDE_PRIVATE Core Gui
    INCLUDE_PRIVATE * ${_include_private}
    PREFIX SVSCRAFT_GUI
    FEATURES cxx_std_20
)

target_link_directories(${PROJECT_NAME} PRIVATE ${_link_directories_private})
