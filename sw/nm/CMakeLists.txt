set(LIB_NAME nm)

set(SRC_FILES 
    bsp/source/nm_bsp.c

    bus_wrapper/source/nm_bus_wrapper.c

    # common/source/nm_common.c
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_compile_options(${LIB_NAME} PUBLIC -g)

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bsp/include)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bus_wrapper/include)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/include)
