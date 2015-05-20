import serial
import time


ser = serial.Serial("/dev/tty.usbmodem411", 9600)
time.sleep(2) #wait for arduino initialisation

ser.write(chr(7))
ser.write(chr(0))