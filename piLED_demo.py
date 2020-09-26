# NeoPixel library strandtest example
# Author: Tony DiCola (tony@tonydicola.com)
#
# Direct port of the Arduino NeoPixel library strandtest example.  Showcases
# various animations on a strip of NeoPixels.
import time

from random import randint

from neopixel import *

# LED strip configuration:
LED_1_COUNT      = 12      # Number of LED pixels.
LED_1_PIN        = 18      # GPIO pin connected to the pixels (must support PWM! GPIO 13 and 18 on RPi 3).
LED_1_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_1_DMA        = 5       # DMA channel to use for generating signal (Between 1 and 14)
LED_1_BRIGHTNESS = 255      # Set to 0 for darkest and 255 for brightest
LED_1_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)
LED_1_CHANNEL    = 0       # 0 or 1
LED_1_STRIP      = ws.WS2811_STRIP_GRB

LED_2_COUNT      = 12      # Number of LED pixels.
LED_2_PIN        = 13      # GPIO pin connected to the pixels (must support PWM! GPIO 13 or 18 on RPi 3).
LED_2_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_2_DMA        = 10      # DMA channel to use for generating signal (Between 1 and 14)
LED_2_BRIGHTNESS = 255      # Set to 0 for darkest and 255 for brightest
LED_2_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)
LED_2_CHANNEL    = 1       # 0 or 1
LED_2_STRIP      = ws.WS2811_STRIP_GRB

global strip1
global strip2

def multiColorWipe(color1, color2, wait_ms=5):
	global strip1
	global strip2
	"""Wipe color across multiple LED strips a pixel at a time."""
	for i in range(strip1.numPixels()):
		if i % 2:
			# even number
			strip1.setPixelColor(i, color1)
			strip2.setPixelColor(i / 2, color2)
			strip1.show()
			time.sleep(wait_ms/1000.0)
			strip2.show()
			time.sleep(wait_ms/1000.0)
		else:
			# odd number
			strip1.setPixelColor(i, color1)
			strip1.show()
			time.sleep(wait_ms/1000.0)
	time.sleep(1)

def blackout(strip):
	for i in range(max(strip1.numPixels(), strip1.numPixels())):
		strip.setPixelColor(i, Color(0,0,0))
		strip.show()

def greenbreathwheel(pos):
	"""Generate rainbow colors across 0-255 positions."""
	if pos < 85:
		return Color(255 - pos * 3, 0, 0)
	elif pos < 170:
		pos -= 85
		return Color(pos * 3, 255 - pos * 3, 0)
	else:
		pos -= 170
		return Color(0, pos * 3, 0)

def greenabreath(strip1, wait_ms=10):
	"""Rainbow movie theater light style chaser animation."""
	for j in range(0,120):
		for q in range(1):
			for i in range(0, strip1.numPixels(), 1):
				strip1.setPixelColor(i+q, greenbreathwheel((i+j) % 255))
			strip1.show()
			time.sleep(wait_ms/500.0)
			for i in range(0, strip1.numPixels(), 1):
				strip1.setPixelColor(i+q, 0)

def greenabreath2(strip1, wait_ms=10):
	"""Rainbow movie theater light style chaser animation."""
	for j in range(135, 255):
		for q in range(1):
			for i in range(0, strip1.numPixels(), 1):
				strip1.setPixelColor(i+q, greenbreathwheel((i-j) % 255))
			strip1.show()
			time.sleep(wait_ms/500.0)
			for i in range(0, strip1.numPixels(), 1):
				strip1.setPixelColor(i+q, 0)
def greenbreath(strip2, wait_ms=10):
	"""Rainbow movie theater light style chaser animation."""
	for j in range(0,120):
		for q in range(1):
			for i in range(0, strip2.numPixels(), 1):
				strip2.setPixelColor(i+q, greenbreathwheel((i+j) % 255))
			strip2.show()
			time.sleep(wait_ms/500.0)
			for i in range(0, strip2.numPixels(), 1):
				strip1.setPixelColor(i+q, 0)

def greenbreath2(strip2, wait_ms=10):
	"""Rainbow movie theater light style chaser animation."""
	for j in range(135, 255):
		for q in range(1):
			for i in range(0, strip2.numPixels(), 1):
				strip2.setPixelColor(i+q, greenbreathwheel((i-j) % 255))
			strip2.show()
			time.sleep(wait_ms/500.0)
			for i in range(0, strip2.numPixels(), 1):
				strip2.setPixelColor(i+q, 0)


def nightrider(strip1, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """
        for i in range(strip1.numPixels()):
                strip1.setPixelColor(i,color)
                strip1.setPixelColor(i-1,dimColor(color))
                strip1.setPixelColor(i-2,dimColor2(color))
                strip1.setPixelColor(i-3,dimColor3(color))
                strip1.show()
                time.sleep(wait_ms/2000.0)
                strip1.setPixelColor(i,0)
                strip1.setPixelColor(i-1,0)
        # reverse the direction
        for i in xrange(strip1.numPixels()-1,-1,-1):
                strip1.setPixelColor(i,color)
                strip1.setPixelColor(i+1,dimColor(color))
                strip1.setPixelColor(i+2,dimColor2(color))
                strip1.setPixelColor(i+3,dimColor3(color))
                strip1.show()
                time.sleep(wait_ms/2000.0)
                strip1.setPixelColor(i,0)
                strip1.setPixelColor(i+1,0)

def nightrider2(strip2, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """
        for i in range(strip2.numPixels()):
                strip2.setPixelColor(i,color)
                strip2.setPixelColor(i-1,dimColor(color))
                strip2.setPixelColor(i-2,dimColor2(color))
                strip2.setPixelColor(i-3,dimColor3(color))
                strip2.show()
                time.sleep(wait_ms/2000.0)
                strip2.setPixelColor(i,0)
                strip2.setPixelColor(i-1,0)
        # reverse the direction
        for i in xrange(strip1.numPixels()-1,-1,-1):
                strip2.setPixelColor(i,color)
                strip2.setPixelColor(i+1,dimColor(color))
                strip2.setPixelColor(i+2,dimColor2(color))
                strip2.setPixelColor(i+3,dimColor3(color))
                strip2.show()
                time.sleep(wait_ms/2000.0)
                strip2.setPixelColor(i,0)
                strip2.setPixelColor(i+1,0)

def nightrider3(strip1, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """

        # reverse the direction
        for i in xrange(strip1.numPixels()-1,-1,-1):
                strip1.setPixelColor(i,color)
                strip1.setPixelColor(i+1,dimColor(color))
                strip1.setPixelColor(i+2,dimColor2(color))
                strip1.setPixelColor(i+3,dimColor3(color))
                strip1.show()
                time.sleep(wait_ms/1000.0)
                strip1.setPixelColor(i,0)
                strip1.setPixelColor(i+1,0)

def nightrider4(strip2, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """

        for i in xrange(strip1.numPixels()-1,-1,-1):
                strip2.setPixelColor(i,color)
                strip2.setPixelColor(i+1,dimColor(color))
                strip2.setPixelColor(i+2,dimColor2(color))
                strip2.setPixelColor(i+3,dimColor3(color))
                strip2.show()
                time.sleep(wait_ms/1000.0)
                strip2.setPixelColor(i,0)
                strip2.setPixelColor(i+1,0)

def nightrider5(strip1, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """
        for i in range(strip1.numPixels()):
                strip1.setPixelColor(i,color)
                strip1.setPixelColor(i-1,dimColor(color))
                strip1.setPixelColor(i-2,dimColor2(color))
                strip1.setPixelColor(i-3,dimColor3(color))
                strip1.show()
                time.sleep(wait_ms/1000.0)
                strip1.setPixelColor(i,0)
                strip1.setPixelColor(i-1,0)

def nightrider6(strip2, color, wait_ms=35,iterations=1):
        """ Knight raider kitt scanner """
        for i in range(strip2.numPixels()):
                strip2.setPixelColor(i,color)
                strip2.setPixelColor(i-1,dimColor(color))
                strip2.setPixelColor(i-2,dimColor2(color))
                strip2.setPixelColor(i-3,dimColor3(color))
                strip2.show()
                time.sleep(wait_ms/1000.0)
                strip2.setPixelColor(i,0)
                strip2.setPixelColor(i-1,0)

def night4(strip1, strip2):
    for t in range (0, 1,1):
            nightrider5 (strip1,Color(randint(0,255),randint(0,255),0),65)
    for j in range (0, 1,1):
            nightrider4 (strip2,Color(0,255,0),65)
    for i in range (0, 1,1):
            nightrider6 (strip2,Color(randint(0,255),randint(0,255),0),65)
    for q in range (0, 1,1):
            nightrider3 (strip1,Color(0,255,0),65)

def dimColor (color):
        """ Color is an 32-bit int that merges the values into one """
        return (((color&0xff0000)/3)&0xff0000) + (((color&0x00ff00)/3)&0x00ff00) + (((color&0x0000ff)/3)&0x0000ff)

def dimColor2 (color):
        """ Color is an 32-bit int that merges the values into one """
        return (((color&0xff0000)/100)&0xff0000) + (((color&0x00ff00)/100)&0x00ff00) + (((color&0x0000ff)/100)&0x0000ff)

def dimColor3 (color):
        """ Color is an 32-bit int that merges the values into one """
        return (((color&0xff0000)/randint(1,255))&0) + (((color&0x00ff00)/randint(1,255))&0) + (((color&0x0000ff)/randint(1,255))&0

def night(strip1):
    for t in range (0, 1,1):
            nightrider (strip1,Color(randint(0,255),randint(0,255),0),65)
    for t in range (0, 1,1):
            nightrider (strip1,Color(0,255,0),randint(65,125))

def night2(strip2):
    for t in range (0, 1,1):
            nightrider2 (strip2,Color(randint(0,255),randint(0,255),0),65)
    for t in range (0, 1,1):
            nightrider2 (strip2,Color(0,255,0),randint(65,125))

def night3(strip1, strip2):
    for t in range (0, 1,1):
            nightrider (strip1,Color(randint(0,255),randint(0,255),0),65)
    for j in range (0, 1,1):
            nightrider2 (strip2,Color(0,255,0),65)

# Main program logic follows:
if __name__ == '__main__':
	# Create NeoPixel objects with appropriate configuration for each strip.
	strip1 = Adafruit_NeoPixel(LED_1_COUNT, LED_1_PIN, LED_1_FREQ_HZ, LED_1_DMA, LED_1_INVERT, LED_1_BRIGHTNESS, LED_1_CHANNEL, LED_1_STRIP)
	strip2 = Adafruit_NeoPixel(LED_2_COUNT, LED_2_PIN, LED_2_FREQ_HZ, LED_2_DMA, LED_2_INVERT, LED_2_BRIGHTNESS, LED_2_CHANNEL, LED_2_STRIP)

	# Intialize the library (must be called once before other functions).
	strip1.begin()
	strip2.begin()

	print ('Press Ctrl-C to quit.')

	# Black out any LEDs that may be still on for the last run
	blackout(strip1)
	blackout(strip2)

	while True:
		night4(strip1, strip2)
		blackout(strip1)
		blackout(strip2)

		greenabreath(strip1)
		greenbreath(strip2)
		greenabreath2(strip1)
		greenbreath2(strip2)
		multiColorWipe(Color(0, 0, 0), Color(0, 0, 0))  # bo wipe


		greenabreath(strip1)
		greenabreath2(strip1)
		greenbreath(strip2)
		greenbreath2(strip2)
		multiColorWipe(Color(0, 0, 0), Color(0, 0, 0))  # bo wipe
		greenabreath(strip1)
		greenbreath(strip2)
		greenabreath2(strip1)
		greenbreath2(strip2)
		multiColorWipe(Color(0, 0, 0), Color(0, 0, 0))  # bo wipe

		blackout(strip1)
		blackout(strip2)
		night3(strip1, strip2)
		blackout(strip1)
		blackout(strip2)
		night4(strip1, strip2)
		blackout(strip1)
		blackout(strip2)
