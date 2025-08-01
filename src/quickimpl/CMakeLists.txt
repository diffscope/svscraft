project(SVSCraftQuickImpl
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

svs_add_library(${PROJECT_NAME} AUTOGEN STATIC
    SOURCES ${_src}
    LINKS SVSCraftCore SVSCraftGui
    QT_LINKS Core Gui Qml Quick QuickTemplates2
    QT_INCLUDE_PRIVATE Core Gui Qml Quick QuickTemplates2
    INCLUDE_PRIVATE *
    PREFIX SVSCRAFT_QUICK_IMPL
)

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()
if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

qt_add_qml_module(${PROJECT_NAME}
    URI SVSCraft.UIComponents.impl
    QML_FILES ${_qml_files}
    RESOURCES ${_resource_files}
    NAMESPACE SVS
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/SVSCraft/UIComponents/impl
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)