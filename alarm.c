#include <stdio.h>
#include <wiringPi.h>
#include "ifttt.h"
#include <time.h>

int main(int argc, char *argv[]){
int state;
int waitingTime;
int blinkingTime;
int blinkingON;
int doneWaiting=0;

wiringPiSetup();
pinMode(0, INPUT);  /*SENSOR*/
pinMode(1, OUTPUT); /*GREEN LED*/
pinMode(2, OUTPUT); /*RED LED*/
pinMode(3, INPUT);  /*BUTTON*/
pinMode(4, OUTPUT); /*BUZZER*/
pullUpDnControl(3, PUD_UP);

while(1){

switch(state){

case 0: /*ALARM_OFF*/
digitalWrite(1, HIGH);
digitalWrite(2, LOW);
digitalWrite(4, LOW);

if(digitalRead(3)==0)
	state = 1;

break;

case 1: /*ALARM_ARMING*/
waitingTime = time(0)+10;
blinkingTime = time(0)+1;
digitalWrite(1, HIGH);
digitalWrite(2, LOW);
blinkingON = 1;
doneWaiting = 0;

while(doneWaiting == 0){
	if(time(0)>=waitingTime)
	doneWaiting = 1;
	if(time(0)>=blinkingTime){
		if(blinkingON ==1){
		digitalWrite(1, LOW);
		blinkingTime = time(0)+1;
		blinkingON = 0;
		}
		else{
		digitalWrite(1, HIGH);
		blinkingTime = time(0)+1;
		blinkingON = 1;
		}
	}
}
state = 2;
break;

case 2: /*ALARM_ARMED*/
digitalWrite(1, LOW);
digitalWrite(2, HIGH);
if(digitalRead(3) == 0){
	state = 0;
	delay(1000);
}
else if(digitalRead(0) == 1)
state = 3;
break;

case 3:/*ALARM_TRIGGERED*/
waitingTime = time(0)+10;
blinkingTime = time(0)+2;
digitalWrite(1, HIGH);
digitalWrite(2, HIGH);
blinkingON = 1;
doneWaiting = 0;

while(doneWaiting == 0){
	if(time(0)>=waitingTime)
	doneWaiting = 1;
	if(digitalRead(3) == 0){
	state = 0;
	doneWaiting = 1;
	delay(1000);
	}
	if(time(0)>=blinkingTime){
		if(blinkingON ==1){
		digitalWrite(1, LOW);
		digitalWrite(2, LOW);
		blinkingTime = time(0)+2;
		blinkingON = 0;
		}
		else{
		digitalWrite(1, HIGH);
		digitalWrite(2, HIGH);
		blinkingTime = time(0)+2;
		blinkingON = 1;
		}
	}

}
if(state == 3)
state = 4;
break;

case 4:/*ALARM_SOUNDING*/
waitingTime = time(0)+2;
doneWaiting = 0;
digitalWrite(1, HIGH);
digitalWrite(2, HIGH);
digitalWrite(4, HIGH);
ifttt("https://maker.ifttt.com/trigger/alarm_triggered/with/key/d99p70RF-oLwjJPN6nVK0a", "alarm triggered!", "", " ");
while(1){
	if(time(0)>=waitingTime){
	digitalWrite(1, LOW);
	digitalWrite(2, LOW);
	digitalWrite(4, LOW);
	}
	if(digitalRead(3) == 0){
	state = 0;
	delay(1000);
	break;
	}
}
break;

}/*switch*/
}/*while*/
/*NOTREACHED*/
return 0;
}
