//Robot lib
//Alexis Enston - maze
#include "abdrive.h"
#include "simpletools.h"
#include "ping.h"
//#include "calcPos.h"

#define MAXCHANGE 27
#define STARTSPEED 70
#define PROPGAIN 50
#define INTGAIN 4
#define DIFFGAIN 30
#define TARGET 5
#define CHANGEDIV 40
#define SQUARE 120
#define SSPEED 60
#define FSPACE 50
#define DGOTOCHANGE 20
#define LTHRESH 18
#define RTHRESH 18

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

//GOTO in a straight line ish
void dgoto(int gTicks) {
	int l_encoder = 0, r_encoder = 0;
	//read ticks travelled
	drive_getTicks(&l_encoder,&r_encoder);
	drive_speed(SSPEED, SSPEED);
	int startL = l_encoder;
	int startR = r_encoder;
	int dist;
	while(gTicks > (l_encoder - startL)) {
		drive_getTicks(&l_encoder,&r_encoder);
		dist = ping_cm(8);
		if(dist < (FSPACE/3)) 
			break;
		int ldst = ldist();
		int rdst = rdist();
		printf("ldist %d rdist %d \n", ldst, rdst);
		int change = 0;
		if(rdst<RTHRESH/2)
			change+=RTHRESH/2-rdst;
		if(ldst<20 && ldst>(LTHRESH/2))
			change+=(LTHRESH-ldst)*3;
		if(ldst<LTHRESH/2)
			change-=LTHRESH-ldst;
		if(rdst<RTHRESH && rdst>(RTHRESH/3))
			change-=RTHRESH-rdst;
		//drive_speed(SSPEED+change,SSPEED+change);
		//CALC distance from left from both sensors


		//lerror = dist - TARGET;
		//print("propgain = %d \n", change);
		//
		print("change %d \n", change);
		change=change/3;

		if(change > MAXCHANGE) {
			change = MAXCHANGE;
		}
		if(change < -MAXCHANGE) {
			change = -MAXCHANGE;
		}

		//change = 10;
		//change = 0;
		drive_speed(STARTSPEED - change, STARTSPEED + change);
	}
	drive_speed(0,0);
}

int x,y,dir,*grid;

void addNum(int num) {
	dir+=16;
	dir=dir%4;
	if(dir == 0)
		grid[((y*2+2)*8)+x*2+1]=num;
	if(dir == 1)
		grid[((y*2+1)*8)+x*2+2]=num;
	if(dir==2)
		grid[((y*2+0)*8)+x*2+1]=num;
	if(dir==3)
		grid[((y*2+1)*8)+x*2+0]=num;
}
//shortest dddpath
typedef struct {
	int x;
	int y;
	int dist;
	int prev;
} Square;

int *pathdir;
Square *path;


int addSquare(int ix, int iy, int prev, int count, int max) {
	//check isnt already there
	int i;
	for(i=0; i<count; i++) {
		if(path[i].x == ix && path[i].y == iy)
			return max;
	}
	path[count].x=ix;
	path[count].y=iy;
	path[count].dist=path[prev].dist+1;
	path[count].prev=prev;
	return path[count].dist;
}



int shortest() {
	//start at 0,0
	path = calloc(18, sizeof(Square));
	path[0].x=0;
	path[0].y=0;
	path[0].dist=0;
	path[0].prev=0;
	int count = 1;
	while(1) {
		int i;
		int max=100;
		for(i=0; i<count; i++) {	
			if(max>path[i].dist) {
				//max = path[i].dist+1;
				int x = path[i].x;
				int y = path[i].y;
				if(
						grid[((y*2+2)*8)+x*2+1]==7)
					max = addSquare(x,y+1,i,count, max);

				if(
						grid[((y*2+1)*8)+x*2+2]==7) 
					max = addSquare(x+1,y,i,count, max);

				if(
						grid[((y*2+0)*8)+x*2+1]==7) 
					max = addSquare(x,y-1,i,count, max);
				if(
						grid[((y*2+1)*8)+x*2+0]==7) 
					max = addSquare(x-1,y-1,i,count, max);
			}
		}
		printf("x %d y %d dist %d \n", path[count].x, path[count].y, path[count].dist);
		if(path[count].x == 3 && path[count].y == 4)
			break;
		count++;
	}
	return count;
}	






void addPass() {
	addNum(7);
}

void addWall() {
	addNum(8);
}

void checkWallIR() {
	//CLOSED WALLS FROM IR
	int ldst = ldist();
	if(ldst<15) {
		dir--;
		addWall();
		dir++;
	}
	int rdst = rdist();
	if (rdst<15) {
		dir++;
		addWall();
		dir--;
	}
}

int rotatecount = 0;

//rotate x right angles clockwise
void rotate(int count) {
	if(rotatecount%4==0) {
		drive_goto(26, -26);
	}
	else if(rotatecount%2==0) {
		drive_goto(25, -26);
	}
	else {
		drive_goto(26, -25);
	}
	rotatecount++;	
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
	dir = 0;
	int incdistl = 0;
	int incdistr = 0;
	grid = calloc(100, sizeof(int));
	x = 0;
	y = 0;
	int px = 0;
	int py = 0;

	grid[((y*2+1)*8)+x*2+1]++;

	//SIMULATOR
	//dgoto(SQUARE/3);


	//init();

	while(1) {

		//calcPosition();
		//read IR
		//
		//stop and turn
		int fdist = ping_cm(8);

		print("%d fdist \n", fdist);
		if(fdist < 30 && fdist != 0) {

			printf("ROTATE \n");
			drive_speed(0,0);

			int l_encoder = 0, r_encoder = 0;

			//read ticks travelled
			drive_getTicks(&l_encoder,&r_encoder);
			incdistl = l_encoder;

			int ldst = ldist();
			int rdst = rdist();
			printf("ldist %d rdist %d \n", ldst, rdst);
			//rotate until found place to go
			//try clockwise or anticlockwise based on IR, double check with ultrasonic
			int clockwise = 1;
			int doa180 = 0;
			if (ldst > LTHRESH) {
				clockwise = -1;
			}
			if (rdst > RTHRESH) {
				doa180 = 1;
			}
			while(fdist<30) {
				//CALIBRATE LOCATION FROM WALL
				//need to be FSPACE away
				addWall();
				drive_goto(fdist*3 - FSPACE, fdist*3 - FSPACE);
				drive_goto(25 * clockwise, -26 * clockwise);
				dir += clockwise;
				fdist = ping_cm(8);
				if(doa180 && (fdist < 30)) {
					drive_goto(fdist*3 - FSPACE, fdist*3 - FSPACE);
					addWall();
					doa180 = 0;
					drive_goto(51, -51);
					dir += 2;
					fdist = ping_cm(8);
					clockwise = 1;
				}
			}
		} 

		else {

			//open left wall

			int ldst = ldist();
			printf("LDIST %d \n", ldst);
			if(ldst > LTHRESH) {
				drive_goto(-25,26);
				dir--;
				//CHECK WITH ULTRASONIC
				fdist = ping_cm(8);
				if(fdist<30) {
					drive_goto(fdist*3 - FSPACE, fdist*3 - FSPACE);
					drive_goto(25, -26);
					dir++;
				}
			}

		}



		int l_encoder = 0, r_encoder = 0;

		//read ticks travelled
		drive_getTicks(&l_encoder,&r_encoder);

		printf("DIR %d \n", dir);
		//LOCATION
		if(1) {
			//print("in squ x %d y %d \n", x, y, dir);
			//int rotat= (r_encoder - incdistr) - (l_encoder - incdistl);
			//rotat = rotat / 40;
			//dir += rotat;
			addPass();
			dir = dir + 16;
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
			//print maze
			grid[((y*2+1)*8)+x*2+1]=1;
			for(int iy = 10; iy >= 0; iy--) {
				for(int ix = 0; ix < 9; ix++) {
					print("%d ", grid[iy*8+ix]);
				}
				print("\n");
			}
		}


		dgoto(SQUARE);
		checkWallIR();

		//if(y==4 && x==3) {
		//	break;
		//}
		//print maze();

		//phase 2
		if(y==0 && x==0) {
			//dikjstras
			int shpath = shortest();
			int squares[20];
			//go back from end
			for(int i=path[shpath].dist; i >= 0; i--){
				printf("adding %d  x %d y %d \n", shpath, path[shpath].x, path[shpath].y);
				squares[i]=shpath;
				shpath = path[shpath].prev;
			}
			dir = 0;
			drive_goto(51,-51);
			//blink
			//
			for(int z = 0; z < 3; z++) {
				high(26);
				pause(100);
				low(26);
				pause(100);
			}
			pause(1000);
			//drive	
			for(int i = 0; i < 15; i++) {
				printf("square x %d y %d \n", path[squares[i]].x, path[squares[i]].y);
				int dirg = 0;
				if(path[squares[i+1]].x > path[squares[i]].x)
					dirg=1;
				if(path[squares[i+1]].x < path[squares[i]].x)
					dirg=3;
				if(path[squares[i+1]].y > path[squares[i]].y)
					dirg=0;
				if(path[squares[i+1]].y < path[squares[i]].y)
					dirg=2;

				//calc rotation
				printf("dir %d dirg %d \n", dir, dirg);
				dirg = dirg - dir;
				dirg+=16;
				dirg=dirg%4;
				if(dirg==1) {
					drive_goto(25, -26);
					dir++;
				}
				if(dirg==2) {
					drive_goto(-51, 51);
					dir+=2;
				}
				if(dirg==3) {
					drive_goto(-25, 26);
					dir+=3;
				}
				dir = dir%4;
				dgoto(SQUARE);
				fdist =	ping_cm(8);
				if(fdist<30) {
					drive_goto(fdist*3 - FSPACE, fdist*3 - FSPACE); 
				}

				if(path[squares[i+1]].x == 4 && path[squares[i+1]].y==3) 
				{
					drive_speed(0,0);
					pause(100);
					//blink
					//
					for(int z = 0; z < 1000; z++) {
						high(26);
						pause(100);
						low(26);
						pause(100);
					}
				}




			}
			drive_speed(0,0);
			sleep(100);

		}

	}

	//Shortest path




	//	printPosition();
	//	stop and turn
	//stop
	drive_speed(0,0);
}

void followLeftWall() {
	followWall(10);
}
