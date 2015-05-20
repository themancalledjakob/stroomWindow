import io
import time
import threading
import picamera
import serial
import random

from PIL import Image

# Create a pool of image processors
done = False
lock = threading.Lock()
pool = []
covered = False
delay = 2.0
lastTimer = 0
animationOrder = [1,2,3,4,5,6]

ser = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2) #wait for arduino initialisation

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
        global ser
        global covered
        global delay
        global lastTimer
        global animationOrder
        while not self.terminated:
            # Wait for an image to be written to the stream
            if self.event.wait(1):
                try:
                    self.stream.seek(0)
                    now = time.clock()
                    if now > lastTimer+delay: 
                        # Read the image and do some processing on it
                        #Image.open(self.stream)
                        image = Image.open(self.stream)
                        pixels = image.load()

                        #image = image.convert ('RGB')
                        #coordinates of the pixel
                        #Get RGB

                        brightness = 0
                        total = 9
                        for i in range(total):
                            x = ((i%3)+1)*8
                            y = (int(i/3)+1)*6
                            R,G,B = pixels[x, y]
                            brightness += B
                        if brightness/total < 70:
                            if not covered:
                                lastTimer = now
                                ser.write(chr(107)) # 7 is offswitch
                                ser.write(chr(101)) # 1 is switch on
                                random.shuffle(animationOrder)
                                covered = True
                        else:
                            if covered:
                                covered = False
                                ser.write(chr(107)) # 7 is offswitch
                                ser.write(chr(100)) # 0 is switch off
                        print covered
                        #if now > lastTimer+delay:
                        #    ser.write(chr(100))
                        #    lastTimer = now
                        #    print " -- "
                        #    print now
                        #    print (lastTimer+delay)
                        #    print " -- "
                        #print brightness/total
                    elif covered:
                        serv = (round(now*12,0)%6) + 1
                        serv2 = (round(now*12+0.5,0)%6) + 1
                        #time.sleep(0.1) #wait for arduino
                        for i in range(0,6):
                            ser.write(chr(100+animationOrder[i]))
                            if serv == animationOrder[i] or serv2 == animationOrder[i]:
                                ser.write(chr(101))
                            else:
                                ser.write(chr(100))
                            time.sleep(0.1) #wait for arduino

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
    #camera.start_preview()
    time.sleep(2)
    camera.capture_sequence(streams(), use_video_port=True)

# Shut down the processors in an orderly fashion
while pool:
    with lock:
        processor = pool.pop()
    processor.terminated = True
    processor.join()
