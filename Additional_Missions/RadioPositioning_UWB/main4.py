#!/usr/bin/python

# Mission thread
# Created by: Shashikant Gupta
# Last Modified by: Jordi Oller

# Star Tracking: Jordi Oller
# Radio Positioning: Shashikant Gupta

import threading
import os, time, traceback, sys
import cv2
import picamera
import picamera.array
import time
import numpy
import uart3
import rpi	#radio positioning
import starTracker
import led_recognition

class Cam_Ops(threading.Thread):
        """
        This class gets star tracker data.
        """

        def __init__(self):
                super(Cam_Ops , self).__init__(name="Cam_Ops thread")
                print ("Thread Cam_Ops initialized")
                # Constants
                CAMERA_RESOLUTION = (320, 240)
                CAMERA_BRIGHTNESS = 30
                CAMERA_CONTRAST = 95
                debug = False

                # Initialise mode to idle, everything off
                ST = False
                OT = False

                # Read ST catalogs into memory
                star_catalog = starTracker.read_catalog()

        def run(self):
				"""inputs from: Jordi"""
                # Open Pi camera stream
                with picamera.PiCamera() as camera:
                        with picamera.array.PiRGBArray(camera) as stream:
                                CAMERA_RESOLUTION = (320, 240)
                                CAMERA_BRIGHTNESS = 30
                                CAMERA_CONTRAST = 95
                                debug = False
                                # Setup camera
                                camera.resolution = CAMERA_RESOLUTION
                                camera.brightness = CAMERA_BRIGHTNESS
                                camera.contrast = CAMERA_CONTRAST
                                star_catalog = starTracker.read_catalog()

                                # Main program loop
                                while True:
                                        try:
                                                # Check for new command
                                                cmd = uart3.read()
                                                # cmd = "hello"
                                                
                                                # Enable/disable radio location
                                                if cmd == "$OBT#":
                                                        ST = False
                                                        OT = True             
                                                else:
                                                        ST = True
                                                        OT = False
                                                print (ST)
                                                print (OT)
                                                # Get camera stream for ST/OT
                                                camera.capture(stream, 'bgr', use_video_port=True)
                                                
                                                # Convert frame to greyscale
                                                frame = stream.array
                                                image = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

                                                # Star tracking
                                                if ST == True:

													data = starTracker.track_stars(image, star_catalog, True, debug)
													if data:
														# convert mm to m
														x = numpy.round(data['x'] / 1000.0, 4)
														y_n = 2 - x
														y = numpy.round(data['y'] / 1000.0, 4)
														x_n = 2 - y
														theta = data['theta'] + 30
														if theta < 0:
																theta = 360 + theta
														print("going to send")
														time.sleep(5)
														uart3.write("$ST:%4.2f:%4.2f:%4.2f#" % (x_n, y_n, theta))
														print("sended")

                                                # Object tracking
                                                else:	# OT == True
													x_0 = (0.35, 0.05, 0.3)
													(-alpha, g_0, G_0), trusted = led_recognition.detect_leds(image, x_0)

													uart3.write("OT;%f;%f;%f;%d" % (alpha, g_0, G_0, trusted))

                                                # reset camera stream for next frame
                                                stream.seek(0)
                                                stream.truncate()

                                                # allow quitting of video with q
                                                if cv2.waitKey(1) & 0xFF == ord('q'):
                                                        break
                                                        
                                        except:
                                                print (traceback.format_exc())
                                                # return 1

class Radio_Positioning(threading.Thread):
        """
        This class gets radio positioning data.
        """

        def __init__(self):
                super(Radio_Positioning , self).__init__(name="Radio_Positioning thread")
                print ("Thread Radio_Positioning initialized")

        def run(self):
                while True:
                        try:
                                data = rpi.main()
                                if len(data) == 2:
                                        time.sleep(5)
                                        uart3.write("$RD:%4.2f:%4.2f#" % (data[0], data[1]))
                                   
                        except:
                                print (traceback.format_exc())
                                return 1

def initialize():
        
        thread1 = Cam_Ops()
        thread2 = Radio_Positioning()

        thread1.start()
        thread2.start()

        # while thread1.is_alive() or thread2.is_alive():
                # pass

