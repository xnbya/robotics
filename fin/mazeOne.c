#include "simpletools.h"
#include "abdrive.h"
#include "ping.h"
#include <stdbool.h>


#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define ROW 5
#define COLUMN 4

int cellValues[ROW][COLUMN];
bool verticalWalls[ROW][COLUMN + 1];
bool horizontalWalls[ROW+1][COLUMN];

const int nearbyCells[4][2] = {
  {1,0},{0,1},{-1,0},{0,-1}
};

const int nearbyWalls[4][2] = {
  {0,0},{0,1},{-1,0},{0,0}
};


int xPos = 0;
int yPos = 0;
int targetx = 3;
int targety = 3;
int heading = NORTH;




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

int frontdist() {
  return ping_cm(8);
}



void forwardmove(){
  drive_goto(130,130);
  xPos += nearbyCells[heading][0];
  yPos += nearbyCells[heading][1];

  print("in square %d,%d, dir %d\n",xPos,yPos, heading);
}


// adding the outer box of the maze 
void addKnownWalls(){
  int i;
  for(i = 0; i < 4; i++){
    horizontalWalls[4][i] = true;
  }
  int j;
  for(j = 0; j < 5; j++){
    verticalWalls[j][0] = true;
  }
  int k;
  for(k = 1; k < 5; k++){
    verticalWalls[k][4] = true;
  }
  int l;
  for(l = 1; l < 4; l++){
    horizontalWalls[0][l] = true;
  }
}
  
                               




void addWalls(int direction){
  switch(direction){
  case NORTH:
    horizontalWalls[xPos][yPos] = true;
    break;
  case EAST:
    verticalWalls[xPos][yPos+1] = true;
    break;
  case SOUTH:
    horizontalWalls[xPos-1][yPos] = true;
    break;
  case WEST:
    verticalWalls[xPos][yPos] = true;
    break;
  }
}

void buildWalls(){
  if(frontdist() < 25){
    addWalls(heading);
    print("front wall detected\n");
  }
  if(rdist() < 20){
    addWalls((heading+1)%4);
    print("wall on the right detected\n");
  }
  if(ldist() < 20){
    addWalls((heading+3)%4);
    print("wall on the left detected\n");
  }
}


void fillMaze(){
  int i;
  int j;
  int k;
  for(i = 0; i < ROW; i++){
    for(j = 0; j < COLUMN; j++){
      cellValues[i][j] = 255;
    }
  }
  cellValues[targetx][targety] = 0;

  bool continueSolv = true;

  while(continueSolv){
    continueSolv = false;
    for(i = 0; i < ROW; i++){
      for(j = 0; j < COLUMN; j++){
	if(cellValues[i][j] < 255){
	  for(k = 0; k < 4; k++){
	    int nearbyCellRow = i + nearbyCells[k][0];
	    int nearbyCellColumn = j + nearbyCells[k][1];

	    int nearbyWallRow = i + nearbyWalls[k][0];
	    int nearbyWallColumn = j + nearbyWalls[k][1];
	   
	    bool wallExists = false;
	    if(k == NORTH || k == SOUTH){
	      wallExists = horizontalWalls[nearbyWallRow][nearbyWallColumn];
	    }
	    else if(k == EAST || k == WEST){
	      wallExists = verticalWalls[nearbyWallRow][nearbyWallColumn];
	    }

	    if(cellValues[nearbyCellRow][nearbyCellColumn] == 255 && !wallExists){
	      cellValues[nearbyCellRow][nearbyCellColumn] = cellValues[i][j] +1;
	      continueSolv = true;
	    }
	  }
	}
      }
    }
  }
 
}

int findDirection(){
  int bestDirection = 255;
  int desiredDirection = 0;
  int k = 0;
  for(k = 0; k < 4; k++){
	    int nearbyCellRow = xPos + nearbyCells[k][0];
	    int nearbyCellColumn = yPos + nearbyCells[k][1];

	    int nearbyWallRow = xPos + nearbyWalls[k][0];
	    int nearbyWallColumn = yPos + nearbyWalls[k][1];
	   
	    bool wallExists = false;
	    if(k == NORTH || k == SOUTH){
	      wallExists = horizontalWalls[nearbyWallRow][nearbyWallColumn];
	    }
	    else{
	      wallExists = verticalWalls[nearbyWallRow][nearbyWallColumn];
	    }

	    if(cellValues[nearbyCellRow][nearbyCellColumn] < bestDirection && !wallExists){
	      bestDirection = cellValues[nearbyCellRow][nearbyCellColumn];
	      desiredDirection = k;
	    }
  }

  return desiredDirection;
  

}

void turnToBestDir(){
  int direction = findDirection();
  print("desired direction %d\n",direction);
  int difference = heading - direction;
  print("difference is %d\n",difference);
  switch(difference){
  case 0:
    print("not turning\n");
    break;
  case 1:
    drive_goto(-25,26);
    print("turning left\n");
    break;
  case 2:
    drive_goto(51,-51);
    print("turning 180\n");
    break;
  case 3:
    drive_goto(25,-26);
    print("turning right\n");
    break;
  case -1:
    drive_goto(25,-26);
    print("turning right\n");
    break;
  case -2:
    drive_goto(51,-51);
    print("turning 180\n");
    break;
  case -3:
    drive_goto(-25,26);
    print("turning left\n");
    break;
  }
    
  /*(if(difference == 0){
    print("not turning\n");
  }
  else if(difference == 1 || difference == -3){
      drive_goto(-25,26);
      print("Turning Left\n");
    }
  else if(difference = -1 || difference == 3){
    drive_goto(25,-26);
    print("Turning Right\n");
    }
  else{
    if(difference == 2 || difference == -2){
    drive_goto(51,-51);
    print("Turning 180\n");
    }}*/
  heading = direction;
  
}





int main(){
  //calibrate();
  
   addKnownWalls();
   
   do{
    buildWalls();
    fillMaze();
    turnToBestDir();
    forwardmove();
    fillMaze();
    //printmaze();
   }while(cellValues[xPos][yPos] != 0);
   buildWalls();
   fillMaze();
   //printmaze();

   
  
}
    

