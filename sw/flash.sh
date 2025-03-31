#!/bin/bash

# Get the current directory
Root="$(pwd)"
builddir="build/"

./build.sh
#openocd -f ti_ek-tm4c123gxl.cfg -c "program build/exe.elf verify reset exit"

echo -e "\nflashing...\n"
# Flash the binary to the microcontroller
lm4flash -E "${Root}/${builddir}exe.bin"
