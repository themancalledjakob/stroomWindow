import serial
import time


ser = serial.Serial("/dev/ttyACM0", 9600)
time.sleep(2) #wait for arduino initialisation

onoff = 101

ser.write(chr(107))
ser.write(chr(onoff))
time.sleep(0.1) #wait for arduino 


ser.write(chr(101))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 

ser.write(chr(102))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 

ser.write(chr(103))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 

ser.write(chr(104))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 

ser.write(chr(105))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 

ser.write(chr(106))
ser.write(chr(101))
time.sleep(0.1) #wait for arduino 
