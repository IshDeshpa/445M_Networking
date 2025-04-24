#!/bin/bash

builddir="build/"
NUM_CORES=$(nproc) # On Linux

tshark_testfile=outbytes.txt
tshark_outputfile=outbytes.pcap
log_file=log.txt

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

    #python3 testingProtocol/dummy.py
    build/sw/exe.elf

    #text2pcap -e 0x0800 ${inputfile} ${outputfile}
    text2pcap ${tshark_testfile} ${tshark_outputfile}
    tshark -r ${tshark_outputfile} -o ip.check_checksum:TRUE -V
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
