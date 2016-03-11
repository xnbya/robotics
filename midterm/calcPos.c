#include "abdrive.h"
#include <math.h>
#define M_PI 3.1415
#define wheelRevCount 0.41
#define ROBOT_WIDTH_CM 10.58


float total_distance;
float angleTurned;


struct location{
	float x;
	float y;
	float theta;
};

struct location current_pos;

int lticks, rticks;

void init(){
	current_pos.x = 0.0;
	current_pos.y = 0.0;
	current_pos.theta = 0.0;
	lticks=rticks=0;
}

//update pos each time heading changes or might
void calcPosition(){
	float l_dist;
	float r_dist;
	float current_cos;
	float current_sin;
	int l_encoder = 0, r_encoder = 0;
	float rad_turn;
	
	//read ticks travelled
	drive_getTicks(&l_encoder,&r_encoder);
	l_dist = (float)(l_encoder - lticks) * wheelRevCount;
	r_dist = (float)(r_encoder - rticks) * wheelRevCount;
	lticks = l_encoder;
	rticks = r_encoder;
	
	current_cos = cos(current_pos.theta);
	current_sin = sin(current_pos.theta);
	
	// straight line
	if(l_dist == r_dist) {
		current_pos.x += l_dist * current_cos;
		current_pos.y += r_dist * current_sin;
	}//when the robot is turning
	else{
		rad_turn = (l_dist - r_dist) / ROBOT_WIDTH_CM;//ROBOT_WIDTH_CM * (l_dist + r_dist)/2.0;
		float Rm = ((l_dist/rad_turn) + (r_dist/rad_turn)) / 2;
	//	print("rad %f \n", rad_turn);
		float dy = Rm * (sin(current_pos.theta + rad_turn) - sin(current_pos.theta));
		float dx = Rm * (cos(current_pos.theta + rad_turn) - cos(current_pos.theta));
		current_pos.y += dy;
		current_pos.x += dx;
	//	print("dy %f dx %f \n", dy, dx);	
		
		current_pos.theta += rad_turn;
	}
	//print("x %f y %f\n", angleTurned);
	
	//total_distance = sqrt(current_pos.x * current_pos.x +current_pos.y * current_pos.y);
	//angleTurned = atan(current_pos.x/current_pos.y) * 57.296;//current_pos.theta * 57.29578;
	//print("l_dist %f total distance travelled is %.2f cm at an angle of %.2f degrees\n x %f y %f \n",l_dist, total_distance,angleTurned, current_pos.x, current_pos.y);


}

	
