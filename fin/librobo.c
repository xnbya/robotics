//Robot lib
//Alexis Enston - week6
#include "abdrive.h"
//#include "simpletools.h"
#include "ping.h"
//#include "calcPos.h"

#define MAXCHANGE 27
#define STARTSPEED 70
#define PROPGAIN 50
#define INTGAIN 4
#define DIFFGAIN 30
#define TARGET 5
#define CHANGEDIV 40
#define SQUARE 156

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
	for(i = 0; i < 20; i += 1) {
		//dac_ctr(25, 0, i);
		freqout(irOut, 1, 38000 + i * 200);
		dist += input(irIn);
	}
	return dist;
}

int ldist() {
	return ledDist(11,10);
}

int rdist() {
	return ledDist(1,2);
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

void followWall(int distance) {
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


	int followL = 1;
	int dir = 0;
	int incdistl = 0;
	int incdistr = 0;
	int *grid = calloc(100, sizeof(int));
	int x = 0;
	int y = 0;
	int px = 0;
	int py = 0;

	//init();

	while(1) {

		//calcPosition();
		//read IR
		//
		//stop and turn
		int fdist = ping_cm(8);
		//print("%d fdist \n", fdist);
		if(fdist < 20 && fdist != 0) {
			printf("ROTATE \n");
			drive_speed(0,0);

			int l_encoder = 0, r_encoder = 0;

			//read ticks travelled
			drive_getTicks(&l_encoder,&r_encoder);
			incdistl = l_encoder;

			int ldst = ldist();
			int rdst = rdist();
			if(ldst > 15) { //turn left
				drive_goto(-25,26);
				dir--;
				followL = 0;
			}
			else {
				if (rdst > 15) { //turn right
					drive_goto(25, -26);
					dir++;
					followL = 1;
				}
				else {
					//do a 180
					drive_goto(51, -51);
					dir+=2;
				}
			}
		}

		//open left wall
		/*
		int ldst = ldist();
		if(ldst > 10) {
			//go forward
			drive_speed(0,0);
			drive_goto(SQUARE/2,SQUARE/2);
			drive_goto(-25,26);
			dir--;
			drive_goto(SQUARE/2, SQUARE/2);
		}
		*/

		int l_encoder = 0, r_encoder = 0;

		//read ticks travelled
		drive_getTicks(&l_encoder,&r_encoder);

		printf("#L,%d,R,%d \n", l_encoder, r_encoder);


		if(l_encoder - incdistl > SQUARE) {
			//print("in squ x %d y %d \n", x, y, dir);
			int rotat= (r_encoder - incdistr) - (l_encoder - incdistl);
			rotat = rotat / 40;
			dir += rotat;

			incdistl = l_encoder;

			dir = dir % 4;
			if(dir == 0)
				y++;
			if(dir == 1)
				x++;
			if(dir==2)
				y--;
			if(dir==3)
				x--;
			//grid[(x*2+1)*8+(y*2)+1] = 5;
			//grid[(x+px)*8+(y+py)] = 1;
			grid[(x*8)+y]++;
			px = x;
			py = y;
			//print maze
			for(int iy = 0; iy < 10; iy++) {
				for(int ix = 0; ix < 8; ix++) {
				//print("%d ", grid[iy*8+ix]);
				}
				//print("\n");
			}
		}
			
		if(followL) 
			dist = ldist();
		else
			dist = rdist();

		int rd = rdist();

		//print("ldist %d Rdist = %d \n", dist, rd);
		lerror = dist - TARGET;
		change = PROPGAIN * lerror; //proportional term
		//print("propgain = %d \n", change);
		change += integralTerm(dist);
		//print("int = %d \n", integralTerm(dist));
		int diff = diffTerm(lerror);
		//print("diff term = %d \n", diff);
		change += diff;
		//print("diff term = %d \n", diff);
		//change += diff;

		change = change / CHANGEDIV;
		//print("change %d \n", change);

		if(!followL) {
			change = -change;
		}

		if(change > MAXCHANGE) {
			change = MAXCHANGE;
		}
		if(change < -MAXCHANGE) {
			change = -MAXCHANGE;
		}

		//change = 10;
		//change = 0;
		//
		//speeds[cloop] = change;
		//cloop++;
		drive_speed(STARTSPEED - change, STARTSPEED + change);
		//
		//drive_speed(20,20);
		pause(50);

		//print maze
		
	}
//	printPosition();
//	stop and turn
	//stop
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(10);
}
