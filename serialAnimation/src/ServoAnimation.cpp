#include "ServoAnimation.h"


//--------------------------------------------------------------
void ServoAnimation::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    for ( int i = 0; i < AMOUNT_SERVOS; ++i){
        turning[i] = false;
    }
    started = false;
    
	ofVideoGrabber vidgr;
    vidgr.getPixels();
}

//--------------------------------------------------------------
void ServoAnimation::startAnimation(unsigned long long start){
    if(!started){
        startTime = start;
        started = true;
    }
}

//--------------------------------------------------------------
void ServoAnimation::update(){
    if(started){
        unsigned long long now = ofGetElapsedTimeMillis();
        if( (int)now < (int)(startTime + totalDuration) ){
            int current = int(now/indyDuration)%AMOUNT_SERVOS;
            int current2 = (current+1)%AMOUNT_SERVOS;
            for ( int i = 0; i < AMOUNT_SERVOS; ++i){
                turning[i] = i == current || i == current2 ? true : false;
            }
        } else {
            started = false;
            for ( int i = 0; i < AMOUNT_SERVOS; ++i){
                turning[i] = false;
            }
        }
    }
}

bool* ServoAnimation::getTurning(){
    return turning;
}

bool* ServoAnimation::getLastTurning(){
    return lastTurning;
}

void ServoAnimation::setLastTurning(){
    memcpy(lastTurning, turning, AMOUNT_SERVOS*sizeof(bool));
}