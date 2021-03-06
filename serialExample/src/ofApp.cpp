#include "ofApp.h"

vector<Byte> receivedBuffer;
vector<Byte> received;
Byte send = (Byte)0;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	bSendSerialMessage = false;
	ofBackground(255);	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	font.loadFont("DIN.otf", 11);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
//	serial.setup(0, baud); //open the first device
	//serial.setup("COM4", baud); // windows example
//	serial.setup("/dev/tty.usbmodem621", baud); // mac osx example
  	serial.setup("/dev/tty.usbmodem621", baud); //linux example
	
	readTime = "not";
	memset(bytesReadString, 0, 12);
//    received = new vector<Byte>();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	if (bSendSerialMessage){
		        serial.writeByte((Byte)107);
        serial.writeByte((Byte)send);		bSendSerialMessage = false;
	}
    
    if(receivedBuffer.size() == 1){
        received = receivedBuffer;
        receivedBuffer.clear();
        readTime = ofGetTimestampString();
    }
    
    Byte recievedNow = serial.readByte();
    if((int)recievedNow != 254){
        receivedBuffer.push_back(recievedNow);
        cout << ofToString((int)recievedNow) << " : " << ofToString(recievedNow) << endl;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
//	if (nBytesRead > 0 && ((ofGetElapsedTimef() - readTime) < 0.5f)){
		ofSetColor(0);
//	} else {
//		ofSetColor(220);
//	}
//	string msg;
//	msg += "click to test serial:\n";
//    string receivedMsg;
//    for(int i =0; i < received.size(); i++)
//        receivedMsg += ofToString((int)received[i]) + " : ";
//	msg += "read: " + receivedMsg + "\n";
//	msg += "at: " + ofToString(readTime) + "\n";
//	font.drawString(msg, 50, 100);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
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
	bSendSerialMessage = true;
    send = send == 100 ? 101 : 100;
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

