#!/bin/bash

builddir="build/"
NUM_CORES=$(nproc) # On Linux

bear -- make clean
bear -- make flash -j$NUM_CORES -s -B
mv compile_commands.json build/
#make dump
echo -e "Used $NUM_CORES for buidling\n"
echo -e "Flashing to Target..."
