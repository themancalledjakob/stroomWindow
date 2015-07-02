#include <Servo.h>

const int AMOUNT_SERVOS = 6;

int SERVO_PIN[AMOUNT_SERVOS];
const int START_PIN = 33;

const int STATUS_DRIVE = 0;
const int STATUS_STOP = 1;

boolean off = false;

Servo servos[AMOUNT_SERVOS];

int ACTIVE_STATUS;

unsigned long interval[2];

unsigned long lastInterval;

boolean active[AMOUNT_SERVOS];

int degree = 0;

long onInterval;
long onDuration;
unsigned long lastOn;
unsigned long now;
long aroundTheCorner;

void setup(){
  
	interval[STATUS_DRIVE] = 2000;
	interval[STATUS_STOP] = 28000;
	lastInterval = 0;
	ACTIVE_STATUS = STATUS_STOP;
	for(int servo = 0; servo < AMOUNT_SERVOS; ++servo){
		SERVO_PIN[servo] = servo + START_PIN;
		active[servo] = true;
	}

	pinMode(13, OUTPUT); // for control led

	digitalWrite(13,LOW);

	onInterval = 60000000 * 5; // 15 minutes in microseconds
	onDuration = 60000000; // 60 seconds
	now = micros();
	lastOn = now;
	aroundTheCorner = onInterval;

}

void loop(){
	unsigned long newNow = micros();
	if ( now > newNow ){
		lastOn = 0;
        lastInterval = 0;
		aroundTheCorner = now - lastOn;
	}
	now = newNow;

	if( now + aroundTheCorner > lastOn + onInterval ){
		if(off){
			off = false;
		}
		if( now + aroundTheCorner > lastOn + onInterval + onDuration ){
			off = true;
			lastOn = now;
			aroundTheCorner = 0;
		}
	}
	if (!off) {
        float calc = (now*0.0000002);
		int activeServo = (((int)calc)%AMOUNT_SERVOS);
		int activeServo2 = (int)((int)(calc+0.5)%AMOUNT_SERVOS);
//
		for (int i = 0; i < AMOUNT_SERVOS; ++i){
			if (i == activeServo || i == activeServo2){
				active[i] = true;
			} else {
				active[i] = false;
			}
		}
	}

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
