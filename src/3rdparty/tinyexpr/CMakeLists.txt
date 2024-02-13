project(tinyexpr LANGUAGES C)

add_library(${PROJECT_NAME} STATIC tinyexpr.h tinyexpr.c)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")