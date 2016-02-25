//Robot lib
#include "abdrive.h"
#include "simpletools.h"
#include "ping.h"
#include "calcPos.h"

#define RANGE 5
#define MINSPEED 20
#define STARTSPEED 40
#define DIVCON 5

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
	for(i = 0; i < 160; i += ) {
		dac_ctr(led, 0, dacVal);
		freqout(irOut, 1, 38000);
		dist += input(irIn);
	}
	return dist;
}

void followWall(int irOut, int irIn, int distance, int left) {
	init();
	int leftspeed = STARTSPEED;
	int rightspeed = STARTSPEED;
	int closeCount = RANGE;
	int farCount = RANGE;
	int irRead;
	while(!stopDead(distance)) {
		//read IR
		freqout(irOut, 1, 38000);
		irRead = input(irIn);
	//	print("ir %d \n", irRead);
		if(irRead) {
			//DEBUG - led
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
		}
		drive_speed(leftspeed, rightspeed);
		calcPosition();
		pause(50);
	}
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(11, 10, 5, 0);
}






