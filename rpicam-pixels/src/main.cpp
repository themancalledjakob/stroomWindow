#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"

#include "pixelsApp.h"
int main()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(640, 480, OF_WINDOW);
	ofRunApp( new pixelsApp());
}