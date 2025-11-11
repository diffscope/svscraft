project(SVSCraftFluentSystemIcons
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS_PRIVATE
    QT_LINKS Core Gui Qml Quick
    QT_INCLUDE_PRIVATE
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_FLUENT_SYSTEM_ICONS
    FEATURES cxx_std_20
)

file(GLOB _assets assets/*)

qt_add_resources(${PROJECT_NAME} assets
    PREFIX /svscraft/fluent-system-icons
    BASE assets
    FILES ${_assets}
)
