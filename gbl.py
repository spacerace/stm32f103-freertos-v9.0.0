#!/usr/bin/python

# this script sends 'GBL\r' via ttyUSB0. The target controller jumps to
# it's bootloader, if received 'GBL\r'.
#
# see https://www.mikrocontroller.net/articles/STM32_jump_to_bootloader
# for more information.

import serial

ser = serial.Serial('/dev/ttyUSB0', 115200)
print ser.name
ser.write('GBL\r')
ser.close