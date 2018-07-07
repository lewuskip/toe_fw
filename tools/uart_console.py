#!/usr/bin/python
import re
import serial
import pdb
import os

if __name__ == "__main__":
    print("Starting device console")
    p = os.popen('ls /dev/ttyUSB*',"r")
    while 1:
        line = p.readline()
        print(line)
        if not line: 
            break
        else:
            rp = re.compile("ttyUSB\d+") 
            ports = rp.findall(line)

    ser = serial.Serial("/dev/" + ports[0], 76800, timeout=1)
    while True:
        buff = ser.read(100)
        if len(buff) > 0: print(buff)
    ser.close()