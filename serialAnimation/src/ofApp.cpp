#include "ofApp.h"

vector<Byte> receivedBuffer;
vector<Byte> received;
Byte send = (Byte)0;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
    ofSetFrameRate(60);
	
	ofBackground(0);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 11);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	int baud = 9600;
//  	serial.setup("/dev/tty.usbmodem621", baud); //linux example
    
    animation.setup();
	
//    received = new vector<Byte>();
}

//--------------------------------------------------------------
void ofApp::update(){
//        serial.writeByte((Byte)107);
//        serial.writeByte((Byte)send);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofSetColor(0,0,0,20);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    animation.update();
    for (int i = 0; i < 6; ++i){
        ofPushMatrix();
        if(animation.getTurning()[i]){
            if(animation.getTurning()[i] != animation.getLastTurning()[i])
                ofSetColor(255,255,255,255);
            else
                ofSetColor(255,0,0,20);
            
            ofTranslate(i*40+20,20);
            ofRotate(ofGetFrameNum()%360);
        } else {
            if(animation.getTurning()[i] != animation.getLastTurning()[i])
                ofSetColor(0,0,0,255);
            else
                ofSetColor(125,20);
            ofTranslate(i*40+20,20);
        }
        ofRect(-10,-10,20,20);
        ofPopMatrix();
    }
    animation.setLastTurning();
    if(!animation.started)
        animation.startAnimation(ofGetElapsedTimeMillis());
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    cout<< "pressed key" << endl;
    animation.startAnimation(ofGetElapsedTimeMillis());
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}

