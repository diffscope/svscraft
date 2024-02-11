project(tst_SVSCraft_TimeSpinBox)

file(GLOB _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src})

qm_configure_target(${PROJECT_NAME}
    LINKS SVSCraftWidgets
    QT_LINKS Core Gui Widgets
)