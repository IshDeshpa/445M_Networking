#!/bin/bash

./build.sh
#openocd -f ti_ek-tm4c123gxl.cfg -c "program build/exe.elf verify reset exit"

echo -e "\nflashing...\n"
# Flash the binary to the microcontroller
lm4flash "${Root}/${builddir}exe.bin"
