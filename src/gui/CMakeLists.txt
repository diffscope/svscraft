project(SVSCraftGui
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS SVSCraftCore
    LINKS_PRIVATE SVSCraft3rdParty_tinyexpr
    QT_LINKS Core Gui
    QT_INCLUDE_PRIVATE Core Gui
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_GUI
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)