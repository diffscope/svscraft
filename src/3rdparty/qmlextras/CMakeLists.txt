project(SVSCraftQmlExtras
    VERSION ${IDECORE_VERSION}
    LANGUAGES CXX
)

file(GLOB_RECURSE _src *.h *.cpp)
file(GLOB_RECURSE _qml_files_abs *.qml *.js *.mjs)
set(_qml_files)
foreach(_file IN LISTS _qml_files_abs)
    file(RELATIVE_PATH _rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${_file})
    list(APPEND _qml_files ${_rel_path})
endforeach()
file(GLOB_RECURSE _resource_files_abs assets/*)
set(_resource_files)
foreach(_file IN LISTS _resource_files_abs)
    file(RELATIVE_PATH _rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${_file})
    list(APPEND _resource_files ${_rel_path})
endforeach()

svs_add_library(${PROJECT_NAME} AUTOGEN
    SOURCES ${_src}
    LINKS
    QT_LINKS Core Gui Qml
    QT_INCLUDE_PRIVATE Core Gui Qml
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_QML_EXTRAS
    FEATURES cxx_std_20
)

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()
if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

qt_add_qml_module(${PROJECT_NAME}
    URI SVSCraft.Extras
    QML_FILES ${_qml_files}
    RESOURCES ${_resource_files}
    NAMESPACE SVS
    OUTPUT_DIRECTORY ${QMSETUP_BUILD_DIR}/qml/SVSCraft/Extras
)

qm_install_qml_modules(${PROJECT_NAME})