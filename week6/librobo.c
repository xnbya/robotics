//Robot lib
#include "abdrive.h"
//#include "simpletools.h"
#include "ping.h"
#include "calcPos.h"

#define MAXCHANGE 20
#define STARTSPEED 60
#define PROPGAIN 50
#define INTGAIN 4
#define DIFFGAIN 30
#define TARGET 15
#define CHANGEDIV 40

//int fd = 65;
//char *test = "aaaaaaaaaaaaaaaaaaaaaaaaddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddaHello World!";

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
	for(i = 0; i < 40; i += 1) {
		//dac_ctr(led, 0, i);
		freqout(irOut, 1, 38000 + i * 75);
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

int diffTerm(int lerror) {
	int diff = preverror - lerror;
	preverror = lerror;
	return diff * DIFFGAIN;
}

void followWall(int irOut, int irIn, int distance, int led) {
	//init();
	totalerror = 0;
	int dist, change, lerror;
	//calibrate robot
	/*int totaldist = 0;
	for(int i = 0; i < 10; i++) { 
		dist += ledDist(irOut, irIn, led);
		pause(50);
	}*/
	//distance = totaldist / 10;
	//print("dist = %d \n", totaldist);
	while(!stopDead(distance)) {
		//read IR
		dist = ledDist(irOut, irIn, led);

		//print("dist = %d \n", dist);
		lerror = dist - TARGET;
		change = PROPGAIN * lerror; //proportional term
		//print("propgain = %d \n", change);
		change += integralTerm(dist);
		//print("int = %d \n", integralTerm(dist));
		int diff = diffTerm(lerror);
		//print("diff term = %d \n", diff);
		change -= diff;
		//print("diff term = %d \n", diff);
		change += diff;

		change = change / CHANGEDIV;
		//print("change %d \n", change);

		if(change > MAXCHANGE) {
			change = MAXCHANGE;
		}
		if(change < -MAXCHANGE) {
			change = -MAXCHANGE;
		}



		drive_speed(STARTSPEED - change, STARTSPEED + change);
		//drive_speed(20,20);

		init();
		calcPosition();
		pause(50);
	}
	drive_speed(0,0);
	printPosition();
}

void followLeftWall() {
	followWall(11, 10, 10, 26);
}
