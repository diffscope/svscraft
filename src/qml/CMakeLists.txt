project(SVSCraftQml
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)
file(GLOB_RECURSE _qml_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.qml *.js *.mjs)
file(GLOB_RECURSE _resource_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} assets/*)

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS SVSCraftCore SVSCraftGui
    QT_LINKS Core Gui Qml
    QT_INCLUDE_PRIVATE Core Gui Qml
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_QML
    FEATURES cxx_std_20
)

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()
if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

qt_add_qml_module(${PROJECT_NAME}
    URI SVSCraft
    QML_FILES ${_qml_files}
    RESOURCES ${_resource_files}
    NAMESPACE SVS
    OUTPUT_DIRECTORY ${QMSETUP_BUILD_DIR}/qml/SVSCraft
)

qm_install_qml_modules(${PROJECT_NAME})