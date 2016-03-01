//#include "simpletools.h"
#include "abdrive.h"
#include <math.h>
#define M_PI 3.1415
#define wheelRevCount 3.25
#define ROBOT_WIDTH_CM 10.58
//#include "dimensions.h"


float total_distance;
float angleTurned;


struct location{
	float x;
	float y;
	float theta;
};

struct location current_pos;


void init(){
	current_pos.x = 0.0;
	current_pos.y = 0.0;
	current_pos.theta = 0.0;
}

void calcPosition(){
	float l_dist;
	float r_dist;
	float current_cos;
	float current_sin;
	float difference;
	//float wheelRevCount;
	int l_encoder = 0, r_encoder = 0;
  float rad_turn;
	
	
	
	//wheelRevCount = 3.25;//(M_PI * WHEEL_DIAMETER_CM)/360.0;
	
	drive_getTicks(&l_encoder,&r_encoder);
	l_dist = (float)l_encoder * wheelRevCount;
	r_dist = (float)r_encoder * wheelRevCount;
	
	current_cos = cos(current_pos.theta);
	current_sin = sin(current_pos.theta);
	
	// straight line
	if(l_encoder == r_encoder){
		current_pos.x += l_dist * current_cos;
		current_pos.y += r_dist * current_sin;
	}//when the robot is turning
	else{
		rad_turn = ROBOT_WIDTH_CM * (l_dist + r_dist)/2.0/(l_dist - r_dist);
		difference = r_dist - l_dist;
		
		current_pos.x += rad_turn * (sin(difference / ROBOT_WIDTH_CM + current_pos.theta)-current_sin);
		current_pos.y += rad_turn * (cos(difference / ROBOT_WIDTH_CM + current_pos.theta)-current_cos);
		
		current_pos.theta += difference / ROBOT_WIDTH_CM;
		
       // to limit it from +PI to -PI
		while(current_pos.theta > M_PI){
			current_pos.theta -= (2.0 * M_PI);
		}
		while(current_pos.theta < -1 * M_PI){
			current_pos.theta += (2.0 * M_PI);
		}
	
	}
	sd_mount(22, 23, 24, 25);
	FILE* fp = fopen("position.txt", "w");
	total_distance = sqrt(current_pos.x * current_pos.x +current_pos.y * current_pos.y);
	fwrite(&total_distance, sizeof(total_distance), 1, fp);
	//print("distacne %d \n", total_distance);
	//angleTurned = atan(current_pos.x/current_pos.y) * 57.296;;//current_pos.theta * 57.29578;
	fwrite(&angleTurned, sizeof(angleTurned), 1, fp);
	fclose(fp);
	//print("angle %f \n", angleTurned);
	
	//print("total distance travelled is %.2f cm at an angle of %.2f degrees\n x %f y %f \n",total_distance,angleTurned, current_pos.x, current_pos.y);
}
