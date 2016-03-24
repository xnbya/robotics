#include "simpletools.h"
#include "ping.h"
#include "abdrive.h"
#include "stdbool.h"

#define MAZEROWS 4
#define MAZECOLUMNS 4
 //roboDirection
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define FRONTTHRESHOLD 20 // CHANGE LATER for front and side
#define SIDETHRESHOLD 20
/*
    (0,3) (1,3) (2,3) (3,3)    
      ---  --- --- --- 
(0,3)|                |
(0,2)|                |
(0,1)|                |
(0,0)|                |
     |   | --- --- ---
      ---
*/

bool horizontalWalls[MAZEROWS+1][MAZECOLUMNS];
bool verticalWalls[MAZEROWS][MAZECOLUMNS+1];

int robotRow = 0 -1 ,robotColumn = 0; // (row -1 for star cell) current position of robot in grid  ( Need to update at the end of move_forward func) 
int roboDirection = NORTH; // current Direction
int values[MAZEROWS][MAZECOLUMNS];
int targetRow = 3;
int targetColumn = 3;

const int neighbourCells[4][2]= {
  {1,0}, //north
  {0,1}, //east 
  {-1,0}, //south
  {0,-1} //west
};

 
const int neighbourWalls[4][2] = {
   {0,0}, //north
   {0,1},  //east
   {-1,0}, //south
   {0,0}  //west
};  



int ledDist(int irOut, int irIn) {
	int dist = 0;
	int i;
	for(i = 0; i < 50; i += 1) {
		//dac_ctr(led, 0, i);
		freqout(irOut, 1, 38000 + i * 75);
		dist += input(irIn);
	}
	return dist;
}


 
  
// for Phase 1  mapping Walls use if necessary
void addwalls(int direction){
      switch(direction){
        case NORTH:
          horizontalWalls[robotRow][robotColumn] = true;
          break;
        case EAST:
          verticalWalls[robotRow][robotColumn+1] = true;
          break;
        case WEST:
          verticalWalls[robotRow][robotColumn] = true;
          break;
        case SOUTH:
          horizontalWalls[robotRow+1][robotColumn] = true;
          break;   
 }            
}
// for Phase 1  mapping Walls use if necessary

void scanWalls(){
      int front_dist, left_dist, right_dist;
      
      front_dist = ping_cm(8);
      left_dist = ledDist(11,10);
      right_dist = ledDist(1,2);
      
      // 0 for north wall  , 1 for east wall, 2 for south wall, 3 for west wall
      if(front_dist < FRONTTHRESHOLD)
        addwalls(roboDirection);
      if(right_dist < SIDETHRESHOLD)
        addwalls((roboDirection + 1) % 4);
      if(left_dist < SIDETHRESHOLD)
        addwalls((roboDirection + 3) % 4);
        
 }
 
 //Phase 2 solving
 
 void flood(){
   values[targetRow][targetColumn] = 0;
   bool continueFlood = false;
   
  while(continueFlood){ 
   for(int i = 0; i < MAZEROWS; i++){
     for(int j = 0; j < MAZECOLUMNS; j++){
       values[i][j] = 255;
       if(values[i][j] < 255){
         for(int k = 0; k < 4; k++){
          int neighbourCellRow = i + neighbourCells[k][0];
          int neighbourCellColumn = j + neighbourCells[k][1];
          int neighbourWallRow = i + neighbourWalls[k][0];
          int neighbourWallColumn = j + neighbourWalls[k][1];
          bool wallExists = false;
          if((k == NORTH)||(k == SOUTH))
            wallExists = horizontalWalls[neighbourWallRow][neighbourWallColumn];
          else
            wallExists = verticalWalls[neighbourWallRow][neighbourWallColumn];  
         
          if(values[neighbourCellRow][neighbourCellColumn] == 255 && !wallExists){
            values[neighbourCellRow][neighbourCellColumn] = values[i][j] + 1;
            continueFlood = true;
          }            
       }
     }       
    }
   }
  }        
}                                          
 //phase 2 finding next cell to goto            
int findNeighbour(){
  int bestNeighbour = 255;
  int desiredDirection = 0;
  //initNeighbour();
  
  for(int k = 0; k < 4; k++){
          int neighbourCellRow = robotRow + neighbourCells[k][0];
          int neighbourCellColumn = robotColumn + neighbourCells[k][1];
          int neighbourWallRow = robotRow + neighbourWalls[k][0];
          int neighbourWallColumn = robotColumn + neighbourWalls[k][1];
          bool wallExists = false;
          if((k == NORTH)||(k == SOUTH))
            wallExists = horizontalWalls[neighbourWallRow][neighbourWallColumn];
          else
            wallExists = verticalWalls[neighbourWallRow][neighbourWallColumn];  
         
          if(values[neighbourCellRow][neighbourCellColumn] < bestNeighbour && !wallExists){
            bestNeighbour = values[neighbourCellRow][neighbourCellColumn];
            desiredDirection = k;
          }            
       }
  return desiredDirection;
}

 //phase 2 change direction towards desired Neighbouring Cell
void turnTowardsNeighbour() {
  int desiredDir = findNeighbour();
  int difference = roboDirection - desiredDir;
  
  if(difference == 1 || difference == -3){
     drive_goto(-51,0); //turn left
   }     
  else if(difference == 2 || difference == -2){
     drive_goto(51,-51); //turn 180
 
   }     
  else if(difference == 3 || difference == -1){
     drive_goto(51,0); //turn right
   }     
           
   roboDirection = desiredDir;
 }   

/*move forward function to navigate during phase 2 unfinished
void forward(){
  //forward
  robotRow+= neighbouringCells[roboDirection][0];
  robotColumn+=neighbouringCells[roboDirection[1];
} 
*/ 

/*phase2 func
void phaseTwo(){
  do{
    flood();
    turnTowardsNeighbour();
    forward();
    }while(values[robotRow][robotColumn] != 0);
 }*/
    
    


  
              
