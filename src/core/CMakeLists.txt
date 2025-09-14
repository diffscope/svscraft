project(SVSCraftCore
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

find_package(semver CONFIG REQUIRED)

file(GLOB_RECURSE _src *.h *.cpp)

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS_PRIVATE SVSCraft3rdParty_boost_math semver::semver
    QT_LINKS Core
    QT_INCLUDE_PRIVATE Core
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_CORE
    FEATURES cxx_std_20
)