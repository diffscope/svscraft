project(boost_math VERSION 1.88.0 LANGUAGES CXX)

add_library(SVSCraft3rdParty_boost_math INTERFACE)

target_include_directories(SVSCraft3rdParty_boost_math INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)