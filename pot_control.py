#!/usr/bin/python

import time
# import sys
# print(sys.executable)

# import os 
# print(os.getcwd())

# print(sys.path)
import spidev

spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 976000

def write_pot(input):
    msb = input >> 8
    lsb = input & 0xFF
    spi.xfer([msb, lsb])

while True:
    for i in range(0xFF, 0x00, -1):
        write_pot(i)
        print(i)
        time.sleep(.2)
    time.sleep(5)
    for i in range(0x00, 0xFF, 1):
        write_pot(i)
        print(i)
        time.sleep(.1)
