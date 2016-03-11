#include "storeMap.h"
#include "calcPos.h"
#include "abdrive.h"
#define M_PI 3.1415


float getTargetBearing(xTarget,yTarget,currentX,currentY){
  float x = xTarget - currentX;
  float y = yTarget - currentY;
  float targetBearing = atan(y/x);
  return targetBearing;
}

int setDirection(float targetBearing){
  float bearing = targetBearing - currentTheta();
  while(bearing > M_PI){
    bearing -= 2 * M_PI;
  }
  while(bearing <= -M_PI){
    bearing += 2 * M_PI;
  }
  if(bearing < -0.01|| bearing > 0.01){
    int ticks = (int)ceil( bearing * 0.284);
    drive_goto(ticks,-ticks);
  }
  else{
    return 1;
  }
  return 0;
}          
          
int goTarget(xTarget,yTarget){
   float currentXpos = currentPositionX();
   float currentYpos = currentPositionY();
   float targetBearing = getTargetBearing(xTarget,yTarget,currentXpos,currentYpos);
   if(setDirection(targetBearing)){
         drive_goto(25,25);
   }
   else{
     int ticks = 0;
     float bearing = targetBearing - currentTheta();
     ticks = (int)ceil((bearing * 0.284));
     drive_goto(ticks,-ticks);
   }     
   return 0;
 }                   
