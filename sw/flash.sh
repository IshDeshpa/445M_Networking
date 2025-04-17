#!/bin/bash

builddir="build/"
NUM_CORES=$(nproc) # On Linux

make clean
make flash -j$NUM_CORES -s -B
#make dump
echo -e "Used $NUM_CORES for buidling\n"
echo -e "Flashing to Target..."
