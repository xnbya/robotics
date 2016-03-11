//Robot lib
//Alexis Enston - week6
#include "abdrive.h"
//#include "simpletools.h"
#include "ping.h"
#include "calcPos.h"

#define MAXCHANGE 27
#define STARTSPEED 50
#define PROPGAIN 50
#define INTGAIN 4
#define DIFFGAIN 30
#define TARGET 10
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

//approximate distance from ir freq response
int ledDist(int irOut, int irIn) {
	int dist = 0;
	int i;
	for(i = 0; i < 260; i += 8) {
		dac_ctr(25, 0, i);
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

int diffTerm(int lerror) {
	int diff = preverror - lerror;
	preverror = lerror;
	return diff * DIFFGAIN;
}

void followWall(int irOut, int irIn, int distance) {
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
	init();


	//store history of speeds
	int *speeds = malloc(sizeof(int) * 500);
	int cloop = 0;

	while(!stopDead(distance)) {
		//read IR
		dist = ledDist(irOut, irIn);

		print("dist = %d \n", dist);
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
		print("change %d \n", change);

		if(change > MAXCHANGE) {
			change = MAXCHANGE;
		}
		if(change < -MAXCHANGE) {
			change = -MAXCHANGE;
		}

		//change = 10;
		//change = 0;
		speeds[cloop] = change;
		cloop++;
		drive_speed(STARTSPEED - change, STARTSPEED + change);
		//drive_speed(20,20);
		//

		calcPosition();
		pause(50);
	}
//	printPosition();
//	stop and turn
	drive_speed(0,0);
	drive_goto(51,-51);

	cloop--;
	//go back
	while(cloop-- > 0) {
		print("cloop %d ch %d \n", cloop, speeds[cloop]);
		drive_speed(STARTSPEED + speeds[cloop], STARTSPEED - speeds[cloop]);
		pause(110);
	}
	//stop
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(11, 10, 20);
}
