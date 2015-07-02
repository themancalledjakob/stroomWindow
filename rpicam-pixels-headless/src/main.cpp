#include "ofMain.h"

#include "ofGLProgrammableRenderer.h"

#include "pixelsApp.h"

#include "ofAppNoWindow.h"
int main()
{

	ofAppNoWindow w;	
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(&w,640, 480, OF_WINDOW);
	ofRunApp( new pixelsApp());
}
