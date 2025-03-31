set(LIB_NAME nm)

set(SRC_FILES 
  nm_bsp_tm4c.c
  nm_bus_wrapper_tm4c.c
  nmbus.c
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_compile_options(${LIB_NAME} PUBLIC -g)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

