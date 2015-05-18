
# Create a keyboard listener
import thread
import time

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

thread.start_new_thread(keypress, ())

char = None

while True:
	if char is not None:
		if char == "q":
			print "whooa"
		print "you pressed" + char
		break
	print "run"
	time.sleep(5)