

#include "simpletools.h"                      // Include simple tools
#include "abdrive.h"

#define meter 308

int main()                                    // Main function
{
  
  int i;
  for(i = 0; i < 4; i++) {
    //1m square
    drive_goto(meter, meter);
    //rotate 90deg
    drive_goto(51,0);
  }  
  
  
}
