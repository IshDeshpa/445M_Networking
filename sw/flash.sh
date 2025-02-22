#!/bin/bash

# Get the current directory
Root="$(pwd)"
builddir="build/"

#remove old build bc reasons
rm -r "${Root}"/${builddir}

# Generate build files with CMake
cmake -B ${builddir}

# Build the project
cmake --build ${builddir}

echo -e "\ngenerating binary > ${builddir}/exe.bin\n"
arm-none-eabi-objcopy -O binary ${builddir}/exe.elf ${builddir}/exe.bin

echo -e "\ngenerating objdump dump.txt\n"
arm-none-eabi-objdump -d ${builddir}exe.elf >dump.txt

#openocd -f ti_ek-tm4c123gxl.cfg -c "program build/exe.elf verify reset exit"

echo -e "\nflashing...\n"
# Flash the binary to the microcontroller
lm4flash "${Root}/${builddir}exe.bin"
