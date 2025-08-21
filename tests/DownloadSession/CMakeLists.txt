project(tst_SVSCraft_DownloadSession)

set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

file(GLOB _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src})

qm_configure_target(${PROJECT_NAME}
    LINKS SVSCraftCore SVSCraftNetwork
    QT_LINKS Core Gui Widgets Network
)