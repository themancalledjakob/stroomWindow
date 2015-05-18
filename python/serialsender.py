import serial
from array import *

# Create a keyboard listener
import thread
import time

# globals
shutdown = False
servoData = array('c', [chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0),chr(0)])

#functions
def initSerial(address):
	ser = serial.Serial(address, 9600)
	time.sleep(2) #wait for arduino initialisation


def initServoData(drive,stop):
	# 1
	servoData[0] = (chr(drive)) #drive
	servoData[1] = chr(stop) #stop
	# 2
	servoData[2] = chr(drive) #drive
	servoData[3] = chr(stop) #stop
	# 3
	servoData[4] = chr(drive) #drive
	servoData[5] = chr(stop) #stop
	# 4
	servoData[6] = chr(drive) #drive
	servoData[7] = chr(stop) #stop
	# 5
	servoData[8] = chr(drive) #drive
	servoData[9] = chr(stop) #stop
	# 6
	servoData[10] = chr(drive) #drive
	servoData[11] = chr(stop) #stop

def writeSerial():
	for i in range(12):
		ser.write( servoData[i] )
	
def readSerial():
	while 1 :
		if shutdown:
			break
		outgo = ser.readline()
		print outgo

def getch():
	import sys, tty, termios
	fd = sys.stdin.fileno()
	old_settings = termios.tcgetattr(fd)
	try:
		tty.setraw(sys.stdin.fileno())
		ch = sys.stdin.read(1)
	finally:
		termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
	return ch

def keypress():
	global char
	char = getch()

	if char == "q":
		shutdown = True

	time.sleep(10)
	thread.start_new_thread(keypress, ())


def init():
	initServoData(25,25)
	initSerial('/dev/ttyACM0')
	readSerial()
	thread.start_new_thread(keypress, ())

# run
init()