#!/bin/bash

bear -- make -j MODE=sw
mv compile_commands.json build/

inputfile=outbytes.txt
outputfile=outbytes.pcap
./build/sw/exe.elf

text2pcap -e 0x0800 ${inputfile} ${outputfile}
tshark -r ${outputfile} -o ip.check_checksum:TRUE -V
