set(LIB_NAME printf)

add_library(${LIB_NAME} STATIC printf.c)

target_compile_options(${LIB_NAME} PUBLIC -Wall -g -O0)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

