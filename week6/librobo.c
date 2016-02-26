//Robot lib
#include "abdrive.h"
#include "simpletools.h"
#include "ping.h"
#include "calcPos.h"

#define MAXCHANGE 20
#define STARTSPEED 40
#define PROPGAIN 50
#define INTGAIN 5
#define DIFFGAIN 50
#define TARGET 18
#define CHANGEDIV 50

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
	for(i = 0; i < 240; i += 8) {
		dac_ctr(led, 0, i);
		freqout(irOut, 1, 38000);
		dist += input(irIn);
	}
	return dist;
}
int totalerror;
int preverror;

int integralTerm(int dist) {
	totalerror += (dist - TARGET);
	return totalerror * INTGAIN;
}

int diffTerm(int error) {
	int diff = preverror - error;
	preverror = error;
	return diff * DIFFGAIN;
}

void followWall(int irOut, int irIn, int distance, int led) {
	//init();
	totalerror = 0;
	int dist, change, error;
	int prev = 0;
	while(!stopDead(distance)) {
		//read IR
		dist = ledDist(irOut, irIn, led);
		print("dist = %d \n", dist);
		error = dist - TARGET;
		change = PROPGAIN * error; //proportional term
		print("propgain = %d \n", change);
		change += integralTerm(dist);
		print("int = %d \n", integralTerm(dist));
		int diff = diffTerm(error);
		print("diff term = %d \n", diff);
		change += diff;

		change = change / CHANGEDIV;
		printf("change %d \n", change);





		drive_speed(STARTSPEED - change, STARTSPEED + change);
		//drive_speed(20,20);

		//init();
		//calcPosition();
		pause(100);
	}
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(11, 10, 5, 26);
}






