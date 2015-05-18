import io
import time
import threading
import picamera

from PIL import Image

# Create a pool of image processors
done = False
lock = threading.Lock()
pool = []

# Create a keyboard listener
import thread

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

class ImageProcessor(threading.Thread):
    def __init__(self):
        super(ImageProcessor, self).__init__()
        self.stream = io.BytesIO()
        self.event = threading.Event()
        self.terminated = False
        self.start()

    def run(self):
        # This method runs in a separate thread
        global done
        while not self.terminated:
            # Wait for an image to be written to the stream
            if self.event.wait(1):
                try:
                    	self.stream.seek(0)
		    
                    	# Read the image and do some processing on it
                    	#Image.open(self.stream)
		    	image = Image.open(self.stream)
		    	pixels = image.load()

			#image = image.convert ('RGB')
			#coordinates of the pixel
			#Get RGB

			#for x in range(32):
                        #        for y in range(24):
                        #                print pixels[x, y][2]
                                        #pixels[x, y] = value
                                        #pixelRGB = image.getpixel((x,y))
                                        #blue += pixelRGB.B
                                        #print value
                        
			#R,G,B = pixelRGB
			#print(pixelRGB)
			 
		    	#image = image.astype(np.float, copy=False)
		    	#image = image / 255.0
                    	#...
                    	#...
                    	# Set done to True if you want the script to terminate
                    	# at some point
		    	# done=True
		    	if char is not None :
				if(char == 'q'):
					print "whoooa, exit with " + char
					done=True
				thread.start_new_thread(keypress, ())
                finally:
                    # Reset the stream and event
                    self.stream.seek(0)
                    self.stream.truncate()
                    self.event.clear()
                    # Return ourselves to the pool
                    with lock:
                        pool.append(self)

def streams():
    while not done:
        with lock:
            if pool:
                processor = pool.pop()
            else:
                processor = None
        if processor:
            yield processor.stream
            processor.event.set()
        else:
            # When the pool is starved, wait a while for it to refill
            time.sleep(0.1)

with picamera.PiCamera() as camera:
    pool = [ImageProcessor() for i in range(4)]
    camera.resolution = (32, 24)
    #camera.saturation = -100
    #camera.zoom = (0.495,0.495,0.1,0.1)
    camera.framerate = 15
    camera.raw_format = 'rgb'
    camera.start_preview()
    time.sleep(2)
    camera.capture_sequence(streams(), use_video_port=True)

# Shut down the processors in an orderly fashion
while pool:
    with lock:
        processor = pool.pop()
    processor.terminated = True
    processor.join()
