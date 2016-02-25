//Robot lib
#include "abdrive.h"
#include "simpletools.h"
#include "ping.h"

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

void followWall(int irOut, int irIn, int distance, int left) {
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
			high(27);
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
			low(27);
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
		pause(50);
	}
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(11, 10, 5, 0);
}






