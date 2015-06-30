#include "pixelsApp.h"
#include <limits>

//Pixel access is not implemented in the player
//so here is how to do it yourself

//it should be implemented along with an "isFrameNew" but that isn't working yet

//--------------------------------------------------------------
void pixelsApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	
	doDrawInfo	= true;
		
	consoleListener.setup(this);
	
	omxCameraSettings.width = 320;
	omxCameraSettings.height = 240;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = true;
	
	filterCollection.setup();

	doPixels = true;
	doReloadPixels = true;
	if (doPixels) 
	{
		omxCameraSettings.enablePixels = true;
		videoTexture.allocate(omxCameraSettings.width, omxCameraSettings.height, GL_RGBA);
	}

	videoGrabber.setup(omxCameraSettings);
	
	covered = false;
	lastCover = std::numeric_limits<unsigned long long>::min();
	coverDuration = 2000;
	closeInteraction = false;
	
	globalDiversity = -1;
	globalAverage = -1;
	
	thresholdAverage = -1; // overriden by load
	thresholdDiversity = -1; // overriden by load
	
	loadSettings();
	
	maxB = -512;
	minB = 512;
	
	
	gpio32  = new GPIO("32");
	gpio32->export_gpio();
        gpio32->setdir_gpio("out");
}	

//--------------------------------------------------------------
void pixelsApp::update()
{
	if (!doPixels)
	{
		return;
	}
	
	unsigned long long now = ofGetElapsedTimeMillis();
	
	if ( now > lastCover + coverDuration ){
		if(doReloadPixels && ofGetFrameNum() > 20)
		{
			if( closeInteraction ){
				closeInteraction = false;
				//videoGrabber.setLEDState(true);
				gpio32 ->setval_gpio("1");
			}
			int _maxB = -512;
			int _minB = 512;
			
			int averB = 0;
			int totalPixels = 320*240;
			unsigned char * pixels = videoGrabber.getPixels();
			int diversity = 0;
			
			for (int i = 0; i < totalPixels; ++i){
				int nowB = (int)pixels[i*4] + (int)pixels[i*4 + 1] + (int)pixels[i*4 + 2];
				nowB /= 3;
				//cout << "r: " << ofToString(i) << " : " << ofToString( (int)pixels[i*4] ) << endl;
				//cout << "g: " << ofToString(i) << " : " << ofToString( (int)pixels[i*4 + 1] ) << endl;
				//cout << "b: " << ofToString(i) << " : " << ofToString( (int)pixels[i*4 + 2] ) << endl;
				_maxB = nowB > _maxB ? nowB : _maxB;
				_minB = nowB < _minB ? nowB : _minB;
				averB += nowB;
			}
			int aver = (averB/totalPixels);
			
			for (int i = 0; i < totalPixels; ++i){
				int nowB = (int)pixels[i*4] + (int)pixels[i*4 + 1] + (int)pixels[i*4 + 2];
				nowB /= 3;
				diversity += abs( nowB - aver );
			}
			
			maxB = _maxB;
			minB = _minB;
			
			int diver = diversity/totalPixels;

			globalAverage = aver;
			globalDiversity = diver;
			
			if ( aver < thresholdAverage && diver < thresholdDiversity ){
				covered = true;
				lastCover = now;
				logCover();
			} else {
				covered = false;
			}
			//cout << " ave: " << ofToString( aver ) << endl;
			//videoTexture.loadData(virdeoGrabber.getPixels(), omxCameraSettings.width, omxCameraSettings.height, GL_RGBA);
		}
	} else {
		closeInteraction = true;
		//videoGrabber.setLEDState(false);
	        gpio32 ->setval_gpio("0");
	}

}


//--------------------------------------------------------------
//void pixelsApp::draw(){
	
//	videoGrabber.draw();
	/*
	if(doPixels && doReloadPixels)
	{
		videoTexture.draw(0, 0, omxCameraSettings.width/2, omxCameraSettings.height/2);
	}
	*/

//	stringstream info;
//	info << "APP FPS: " << ofGetFrameRate() << "\n";
//	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
//	info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
//	info << "PIXELS ENABLED: " << doPixels << "\n";
//	info << "PIXELS RELOADING ENABLED: " << doReloadPixels << "\n";
//	info << "COVERED: " << covered << "\n";
//	info << "THRESHOLD A: " << thresholdAverage << "\n";
//	info << "THRESHOLD D: " << thresholdDiversity << "\n";
//	info << "closeInteraction: " << closeInteraction << "\n";
//	info << "globalDiversity: " << globalDiversity << "\n";
//	info << "globalAverage: " << globalAverage << "\n";
//	info << "maxB: " << maxB << "\n";
//	info << "minB: " << minB << "\n";
	//info <<	filterCollection.filterList << "\n";
	
//	info << "\n";
//	info << "Press e to Increment filter" << "\n";
//	info << "Press p to Toggle pixel processing" << "\n";
//	info << "Press r to Toggle pixel reloading" << "\n";
//	info << "Press g to Toggle info" << "\n";
	
	
//	if (doDrawInfo) 
//	{
//		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
//	}
	
	//
//}

//--------------------------------------------------------------
void pixelsApp::keyPressed  (int key)
{
	ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	
	if (key == 'e')
	{
		videoGrabber.applyImageFilter(filterCollection.getNextFilter());
	}
	if (key == 'g')
	{
		doDrawInfo = !doDrawInfo;
	}
	if (key == 'l')
	{
		loadSettings();
	}
	
	if (key == 'p')
	{
		doPixels = !doPixels;
		if (!doPixels) 
		{
			videoGrabber.disablePixels();
		}else
		{
			videoGrabber.enablePixels();
		}
	}
	if (key == 'r') {
		doReloadPixels = !doReloadPixels;
	}
}

void pixelsApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

void pixelsApp::loadSettings()
{
	
    std::string file = "settings.json";

    // Now parse the JSON
    bool parsingSuccessful = result.open(file);

    if (parsingSuccessful)
    {
        ofLogNotice("pixelsApp::loadSettings") << result.getRawString();
	
	thresholdAverage = result["thresholdAverage"].asInt();
	thresholdDiversity = result["thresholdDiversity"].asInt();
	coverDuration = result["coverDuration"].asInt();
	coverCounter = result["coverCounter"].asInt();
	    /*
        // now write pretty print
        if (!result.save("example_output_pretty.json", true))
        {
            ofLogNotice("pixelsApp::loadSettings") << "example_output_pretty.json written unsuccessfully.";
        }
        else
        {
            ofLogNotice("pixelsApp::loadSettings") << "example_output_pretty.json written successfully.";
        }
	    */
	/*
        // now write without pretty print
        if (!result.save("example_output_fast.json", false))
        {
            ofLogNotice("pixelsApp::loadSettings") << "example_output_pretty.json written unsuccessfully.";
        }
        else
        {
            ofLogNotice("pixelsApp::loadSettings") << "example_output_pretty.json written successfully.";
        }
	*/

    }
    else
    {
        ofLogError("pixelsApp::loadSettings")  << "Failed to parse JSON" << endl;
    }
    
}


void pixelsApp::logCover()
{
	
	ofxJSONElement log;
	string time = ofGetTimestampString();
	log["timeStamp"] = time;
        if (!log.save( "logs/" + time + "_log.json", true))
        {
            ofLogNotice("pixelsApp::logCover") << "log.json written unsuccessfully.";
        }
        else
        {
            ofLogNotice("pixelsApp::logCover") << "log.json written successfully.";
        }
	
	coverCounter++;
	
        if (!result.save( "settings.json", true))
        {
            ofLogNotice("pixelsApp::logCover") << "settings.json written unsuccessfully.";
        }
        else
        {
            ofLogNotice("pixelsApp::logCover") << "settings.json written successfully.";
        }
}

