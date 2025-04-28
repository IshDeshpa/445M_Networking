#/bin/bash

NUM_CORES=$(nproc) # On Linux

builddir="build/"
dumpdir="temp"

tshark_testfile=outbytes.txt
tshark_outputfile=outbytes.pcap
log_file=log.txt

echoreq_outfile=echoreq.txt
echoreq_pcap=echoreq.pcap
echoresp_outfile=echoresp.txt
echoresp_outfile=echoresp.pcap

tshark_inputfile=inbytes.pcap
case $1 in
-t)
    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/
    #make dump
    echo -e "Running Sim\n\n"

    echo "========================================"
    echo "⚙️ 1. Running test executable which sends dummy data"
    echo "========================================"
    build/sw/exe.elf

    mkdir -p ${dumpdir}/

    echo "========================================"
    echo "📦 2. Converting raw text hex dump to PCAP using text2pcap"
    echo "========================================"
    # Uncomment the line below if raw file needs Ethernet header
    # text2pcap -e 0x0800 ${inputfile} ${outputfile}
    text2pcap ${dumpdir}/${tshark_testfile} ${dumpdir}/${tshark_outputfile}

    echo "========================================"
    echo "🔎 3. Inspecting converted PCAP with tshark"
    echo "========================================"
    tshark -r ${dumpdir}/${tshark_outputfile} -o ip.check_checksum:TRUE -V

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
    python3 testingProtocol/dummy.py random_frame
    deactivate

    echo "========================================"
    echo "🔍 2. Running tshark on original PCAP to inspect IP checksum"
    echo "========================================"

    echo "========================================"
    echo "⚙️ 3. Running test executable to observe parse"
    echo "========================================"
    build/sw/exe.elf

    echo -e "\n\n\n"

    ;;

-p)
    echo "========================================"
    echo "🐍 1. Running Python script to gen echo req"
    echo "========================================"

    mkdir -p ${dumpdir}/
    source .venv/bin/activate
    python3 testingProtocol/echoreq.py
    deactivate

    text2pcap ${dumpdir}/${echoreq_outfile} ${dumpdir}/${echoreq_pcap}
    #tshark -r ${dumpdir}/${echoreq_pcap} -o ip.check_checksum:TRUE -V

    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json ${builddir}
    #make dump
    echo -e "Running Sim\n\n"
    build/sw/exe.elf

    touch ${dumpdir}/${echoresp_outfile}
    text2pcap ${dumpdir}/${echoresp_outfile} ${dumpdir}/${echoresp_pcap}
    tshark -r ${dumpdir}/${echoresp_pcap} -o ip.check_checksum:TRUE -V
    ;;

-dd)
    make clean

    ret=$(bear -- make -j$NUM_CORES MODE=sw -s)
    if [[ $? -ne 0 ]]; then
        exit
    fi

    mv compile_commands.json build/
    #make dump
    echo -e "Used $NUM_CORES for buidling\n"
    echo -e "Running Sim\n\n"

    mkdir ${dumpdir}

    echo "========================================"
    echo "⚙️ 1. Running test executable which sends dummy data"
    echo "========================================"
    build/sw/exe.elf

    echo "========================================"
    echo "📦 2. Converting raw text hex dump to PCAP using text2pcap"
    echo "========================================"
    # Uncomment the line below if raw file needs Ethernet header
    # text2pcap -e 0x0800 ${inputfile} ${outputfile}
    text2pcap temp/dhcp_disc.txt temp/dhcp_disc.pcap

    echo "========================================"
    echo "🔎 3. Inspecting converted PCAP with tshark"
    echo "========================================"
    tshark -r temp/dhcp_disc.pcap -o ip.check_checksum:TRUE -V

    echo "========================================"
    echo "🔎 4. Parsing with python dhcp"
    echo "========================================"

    python3 testingProtocol/dummy.py dhcp

    echo "✅ Done."

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
