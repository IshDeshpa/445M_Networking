#!/bin/bash

builddir="build/"
NUM_CORES=$(nproc) # On Linux

inputfile=outbytes.txt
outputfile=outbytes.pcap

case $1 in
-t)
    make clean

    ret=$(bear -- make flash -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/
    #make dump
    echo -e "Used $NUM_CORES for buidling\n"
    echo -e "Running Sim\n\n"
    build/sw/exe.elf

    text2pcap -e 0x0800 ${inputfile} ${outputfile}

    tshark -r ${outputfile} -o ip.check_checksum:TRUE -V
    ;;

*)
    make clean

    ret=$(bear -- make flash -j$NUM_CORES MODE=hw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/
    #make dump
    echo -e "Used $NUM_CORES for buidling\n"
    echo -e "Flashing to Target..."
    ;;
esac
