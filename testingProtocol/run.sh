#!/bin/bash

bear -- make
mv compile_commands.json build/

inputfile=outbytes.txt
outputfile=outbytes.pcap
./build/exe.elf

text2pcap -e 0x0800 ${inputfile} ${outputfile}

tshark -r ${outputfile} -V
