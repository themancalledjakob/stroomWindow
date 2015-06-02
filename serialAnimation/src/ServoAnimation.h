#pragma once

#include "ofMain.h"

class ServoAnimation {
	
public:
    void setup();
    void update();
    void startAnimation(unsigned long long start);
    virtual bool * getTurning();
    
    virtual bool * getLastTurning();
    void setLastTurning();
    
    
    static const int AMOUNT_SERVOS = 6;
    
    int totalDuration = 42000;
    int indyDuration = 3000;
    unsigned long long startTime;
    
    bool started;
    
    bool turning[AMOUNT_SERVOS];
    bool lastTurning[AMOUNT_SERVOS];
};

