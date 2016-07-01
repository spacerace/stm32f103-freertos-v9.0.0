#!/bin/bash

#stm32loader.py -e -w -p /dev/ttyUSB0 -b 115200 -g 0x08000000 output/main.bin
stm32flash -b 921600 -w output/main.bin -g 0x0 /dev/ttyUSB0 
