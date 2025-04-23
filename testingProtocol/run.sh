#!/bin/bash

bear -- make
mv compile_commands.json build/

./build/exe.elf
