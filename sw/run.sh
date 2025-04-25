#!/bin/bash

builddir="build/"
NUM_CORES=$(nproc) # On Linux

tshark_testfile=temp/outbytes.txt
tshark_outputfile=temp/outbytes.pcap
log_file=temp/log.txt

tshark_inputfile=temp/inbytes.pcap
case $1 in
-t)
    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/
    #make dump
    echo -e "Used $NUM_CORES for buidling\n"
    echo -e "Running Sim\n\n"
    
    echo "========================================"
    echo "⚙️ 1. Running test executable which sends dummy data"
    echo "========================================"
    build/sw/exe.elf

    mkdir -t temp/

    echo "========================================"
    echo "📦 2. Converting raw text hex dump to PCAP using text2pcap"
    echo "========================================"
    # Uncomment the line below if raw file needs Ethernet header
    # text2pcap -e 0x0800 ${inputfile} ${outputfile}
    text2pcap ${tshark_testfile} ${tshark_outputfile}

    echo "========================================"
    echo "🔎 3. Inspecting converted PCAP with tshark"
    echo "========================================"
    tshark -r ${tshark_outputfile} -o ip.check_checksum:TRUE -V

    echo "✅ Done."

    ;;
-r)
    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi
    
    mv compile_commands.json build/

    echo "========================================"
    echo "🐍 1. Running Python script to generate test packets"
    echo "========================================"
    source .venv/bin/activate
    python3 testingProtocol/dummy.py
    deactivate

    echo "========================================"
    echo "🔍 2. Running tshark on original PCAP to inspect IP checksum"
    echo "========================================"
    tshark -r ${tshark_inputfile} -o ip.check_checksum:TRUE -V

    echo "========================================"
    echo "⚙️ 3. Running test executable to observe parse"
    echo "========================================"
    build/sw/exe.elf

    echo -e "\n\n\n"
    
    ;;
*)
    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=hw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/

    make flash MODE=hw -s
    #make dump
    echo -e "Used $NUM_CORES for buidling\n"
    echo -e "Flashing to Target..."
    ;;
esac
