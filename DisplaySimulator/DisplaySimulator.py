#!/usr/bin/python

import serial
import os

COLS = 16
ROWS = 24
PIXELSIZE = 10
BORDER = 1


color = (0.0, 0.07, 0.14, 0.21, 0.28, 0.35, 0.42, 0.47, 0.54, 0.63, 0.70, 0.77, 0.83, 0.89, 0.95, 1.0)

data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
port = serial.Serial("/dev/ttyUSB0", 38400)
port.open()
while(1):
    line = port.read(18)
    pos = line.find(chr(170))
    line = "%s%s" % (line[pos:], line[:pos])
    row = ord(line[1])/2  # /2 because address is shifted on to the left for I2C r/w bit
    if (row == 0):
        os.system(['clear','cls'][os.name == 'nt'])
        print "      ___________________________________"
    for i in range(16):
        data[i] = "X" if (ord(line[i+2]) != 0) else " "
    print " %2s:  | %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s |" % (row, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15])
    if (row == 23):
        print "      '---------------------------------'"
port.close()




