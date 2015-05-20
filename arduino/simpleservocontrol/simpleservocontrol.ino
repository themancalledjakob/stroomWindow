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

int degree = 0;

void setup(){
  
  Serial.begin(9600);
	interval[STATUS_DRIVE] = 1000;
	interval[STATUS_STOP] = 28000;
	lastInterval = 0;
	ACTIVE_STATUS = STATUS_STOP;
	for(int servo = 0; servo < AMOUNT_SERVOS; ++servo){
		SERVO_PIN[servo] = servo + START_PIN;
	}

	pinMode(13, OUTPUT); // for control led

	digitalWrite(13,LOW);
        
}

void loop(){
	if(Serial.available()){
    int received = Serial.read();
		if(received == 0){
			off = true;
		} else {
			off = false;
		}
	}

	unsigned long now = micros();
    if(off){
      for(int servo = 0; servo < AMOUNT_SERVOS; ++servo)
        if(servos[servo].attached()){
            servos[servo].detach();
          }
      digitalWrite(13,LOW);
    } else {          
      digitalWrite(13,HIGH);
      degree++;
      degree%=360;
    	if( now > lastInterval + interval[ACTIVE_STATUS] ){
    		if(ACTIVE_STATUS == STATUS_STOP){ //make it drive
          ACTIVE_STATUS = STATUS_DRIVE;
          delayMicroseconds(0);
          for(int servo = 0; servo < AMOUNT_SERVOS; ++servo)
    				if(!servos[servo].attached()){
    					servos[servo].attach(SERVO_PIN[servo]);
    					servos[servo].write(degree);
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
