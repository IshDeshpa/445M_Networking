set(LIB_NAME nm)

set(SRC_FILES 
    bsp/nm_bsp.c

    bus_wrapper/nm_bus_wrapper.c

    common/nm_common.c

    driver/source/m2m_wifi.c
    driver/source/m2m_hif.c
    driver/source/m2m_periph.c
    driver/source/m2m_coex.c
    driver/source/nmdrv.c
    driver/source/nmdrv_crt.c
    driver/source/nmasic.c
    driver/source/nmbus.c
    driver/source/nmi2c.c
    driver/source/nmsdio.c
    driver/source/nmspi.c
    driver/source/nmuart.c
    driver/source/spi_flash.c
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_compile_options(${LIB_NAME} PUBLIC -g)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/bus_wrapper
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/driver
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/source
)
