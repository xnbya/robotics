//Robot lib
#include "abdrive.h"
#include "simpletools.h"
#include "ping.h"
#include "calcPos.h"

#define MINSPEED 20
#define STARTSPEED 40
#define PROPGAIN 10
#define INTGAIN 5
#define TARGET 20

//detect if something is dead in front, return 1 if it is
int stopDead(int distance) {
	int dist = ping_cm(8);
	//print("dist %d \n", dist);
	if(dist < distance)
		return 1;
	else
		return 0;
}

int ledDist(int irOut, int irIn, int led) {
	int dist = 0;
	int i;
	for(i = 0; i < 160; i += 8) {
		dac_ctr(led, 0, i);
		freqout(irOut, 1, 38000);
		dist += input(irIn);
	}
	return dist;
}

int totalerror;

int integralTerm(int dist) {
	totalerror += (dist - TARGET);
	return totalerror * INTGAIN;
}

void followWall(int irOut, int irIn, int distance, int led) {
	//init();
	totalerror = 0;
	int speed = STARTSPEED;
	int dist;
	int change;
	while(!stopDead(distance)) {
		//read IR
		dist = ledDist(irOut, irIn, led);
	//	print("ir %d \n", irRead);
	/*	if(dist > ) {
			if(closeCount == 0) {
				leftspeed = rightspeed = STARTSPEED;
			}
			farCount = 0;
			closeCount++;
			if(closeCount > RANGE && leftspeed > (MINSPEED + closeCount / DIVCON )) { 
				leftspeed -= closeCount / DIVCON;
				rightspeed += closeCount / DIVCON;
			}
		}
		else {
			if(farCount == 0) {
				leftspeed = rightspeed = STARTSPEED;
			}
			closeCount = 0;
			farCount++;
			if(farCount > RANGE && rightspeed > (MINSPEED + closeCount / DIVCON)) {
				leftspeed += farCount / DIVCON;
				rightspeed -= farCount / DIVCON;
			}
		} */

		change = PROPGAIN * (dist - TARGET); //proportional term
		change += integralTerm(dist);





		drive_speed(STARTSPEED - change, STARTSPEED + change);


		//calcPosition();
		pause(50);
	}
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(11, 10, 5, 26);
}






