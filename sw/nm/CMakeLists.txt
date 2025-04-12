set(LIB_NAME nm)

set(SRC_FILES 
    bsp/nm_bsp.c

    bus_wrapper/nm_bus_wrapper.c

    common/nm_common.c
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_compile_options(${LIB_NAME} PUBLIC -g)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bsp)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bus_wrapper)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common)
