#include <Servo.h>

const int AMOUNT_SERVOS = 6;

int SERVO_PIN[AMOUNT_SERVOS];
const int START_PIN = 30;

const int STATUS_DRIVE = 0;
const int STATUS_STOP = 1;

boolean off = true;

Servo servos[AMOUNT_SERVOS];

int ACTIVE_STATUS;

unsigned long interval[2];

unsigned long lastInterval;

boolean active[AMOUNT_SERVOS];

int degree = 0;

void setup(){
  
  Serial.begin(9600);
	interval[STATUS_DRIVE] = 1000;
	interval[STATUS_STOP] = 28000;
	lastInterval = 0;
	ACTIVE_STATUS = STATUS_STOP;
	for(int servo = 0; servo < AMOUNT_SERVOS; ++servo){
		SERVO_PIN[servo] = servo + START_PIN;
		active[servo] = false;
	}

	pinMode(13, OUTPUT); // for control led

	digitalWrite(13,LOW);
        
}

void loop(){

	if(Serial.available()){
	    int switcher = Serial.read();
	    switch (switcher) {
	    	case 7:
	    		digitalWrite(13,HIGH);
	    		off = (int)Serial.read() == 0 ? true : false;
	    		break;
	    	case 1:
	    	case 2:
	    	case 3:
	    	case 4:
	    	case 5:
	    	case 6:
	    		active[switcher-1] = (int)Serial.read() == 0 ? true : false;
	    		break;
	    	case 8:
	    		interval[STATUS_DRIVE] += (int)Serial.read();
	    		break;
	    	case 9:
	    		interval[STATUS_DRIVE] -= (int)Serial.read();
	    		break;
	    	case 9:
	    		interval[STATUS_STOP] += (int)Serial.read();
	    		break;
	    	case 10:
	    		interval[STATUS_STOP] -= (int)Serial.read();
	    		break;
	    }
	}

    if(off){
      for(int servo = 0; servo < AMOUNT_SERVOS; ++servo)
        if(servos[servo].attached()){
            servos[servo].detach();
          }
      digitalWrite(13,LOW);
    } else {      
	  unsigned long now = micros();    
      digitalWrite(13,HIGH);
      degree++;
      degree%=360;

    	if( now > lastInterval + interval[ACTIVE_STATUS] ){
    		if(ACTIVE_STATUS == STATUS_STOP){ //make it drive
          		ACTIVE_STATUS = STATUS_DRIVE;
          		for(int servo = 0; servo < AMOUNT_SERVOS; ++servo)
          			if(active[servo]){
	    				if(!servos[servo].attached()){
	    					servos[servo].attach(SERVO_PIN[servo]);
	    					servos[servo].write(degree);
	    				}
          			} else {
		    			if(servos[servo].attached()){
		    				servos[servo].detach();
		    			}
          			}
    		} else if(ACTIVE_STATUS == STATUS_DRIVE){ // make it stop
          ACTIVE_STATUS = STATUS_STOP;
          for(int servo = 0; servo < AMOUNT_SERVOS; ++servo)
    			if(servos[servo].attached()){
    				servos[servo].detach();
    			}
    		}
    		lastInterval = now;
    	}
    }
}
