#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"
#include "ofxJSON.h"
#include "ofxGPIO.h"

class pixelsApp : public ofBaseApp, public KeyListener{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

	void onCharacterReceived(KeyListenerEventData& e);
	void loadSettings();
	void logCover();
	
	TerminalListener consoleListener;
	ofxRPiCameraVideoGrabber videoGrabber;
	
	ImageFilterCollection filterCollection;
	
	bool doDrawInfo;
	ofTexture videoTexture;
	OMXCameraSettings omxCameraSettings;
	bool doPixels;
	bool doReloadPixels;
	bool covered;
	unsigned long long lastCover;
	int coverDuration;
	long coverCounter;
	bool closeInteraction;
	int thresholdAverage;
	int thresholdDiversity;
	ofxJSONElement result;
	long globalDiversity;
	int globalAverage;
	int maxB;
	int minB;
	
	ofSerial	serial;
	
	GPIO* gpio32;
};

