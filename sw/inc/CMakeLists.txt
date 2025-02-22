set(LIB_NAME ValvanoWare)

set(SRC_FILES 
  LaunchPad.c
  Timer5A.c
  Timer4A.c
  Timer0A.c
  Timer1A.c
  PLL.c
  ADCSWTrigger.c
  LPF.c
  IRDistance.c
  ADCT0ATrigger.c
)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

target_compile_options(${LIB_NAME} PUBLIC -g)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

