project(SVSCraftNetwork
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS_PRIVATE 
    QT_LINKS Core Network
    QT_INCLUDE_PRIVATE Core Network
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_NETWORK
    FEATURES cxx_std_20
)