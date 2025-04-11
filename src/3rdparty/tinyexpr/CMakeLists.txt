project(tinyexpr LANGUAGES C)

if(${CMAKE_C_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

add_library(SVSCraft3rdParty_tinyexpr STATIC tinyexpr/tinyexpr.h tinyexpr/tinyexpr.c)

target_include_directories(SVSCraft3rdParty_tinyexpr PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tinyexpr>")