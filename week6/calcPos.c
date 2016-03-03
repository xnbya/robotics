//#include "simpletools.h"
#include "abdrive.h"
#include <math.h>
#define M_PI 3.1415
#define wheelRevCount 0.41
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

int lticks, rticks;

void init(){
	current_pos.x = 0.0;
	current_pos.y = 0.0;
	current_pos.theta = 0.0;
	lticks=rticks=0;
}


void calcPosition(){
	float l_dist;
	float r_dist;
	float current_cos;
	float current_sin;
//	float difference;
	//float wheelRevCount;
	int l_encoder = 0, r_encoder = 0;
	float rad_turn;
	
	
	
	//wheelRevCount = 3.25;//(M_PI * WHEEL_DIAMETER_CM)/360.0;
	
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
	//	current_pos.x += rad_turn * (sin(difference / ROBOT_WIDTH_CM + current_pos.theta)-current_sin);
	//	current_pos.y += rad_turn * (cos(difference / ROBOT_WIDTH_CM + current_pos.theta)-current_cos);
		
		current_pos.theta += rad_turn;
	/*	
       // to limit it from +PI to -PI
		while(current_pos.theta > M_PI){
			current_pos.theta -= (2.0 * M_PI);
		}
		while(current_pos.theta < -1 * M_PI){
			current_pos.theta += (2.0 * M_PI);
		}
*/	
	}
	//print("x %f y %f\n", angleTurned);
	
	//total_distance = sqrt(current_pos.x * current_pos.x +current_pos.y * current_pos.y);
	//angleTurned = atan(current_pos.x/current_pos.y) * 57.296;//current_pos.theta * 57.29578;
	//print("l_dist %f total distance travelled is %.2f cm at an angle of %.2f degrees\n x %f y %f \n",l_dist, total_distance,angleTurned, current_pos.x, current_pos.y);


}

void printPosition() { 
	sd_mount(22, 23, 24, 25);
	FILE* fp = fopen("POS.TXT", "w");
	fwrite(&current_pos, sizeof(current_pos), 1, fp);
	fwrite(&current_pos, sizeof(current_pos), 1, fp);
	fwrite(&current_pos, sizeof(current_pos), 1, fp);
	fwrite(&current_pos, sizeof(current_pos), 1, fp);
	fwrite(&current_pos, sizeof(current_pos), 1, fp);
	//total_distance = sqrt(current_pos.x * current_pos.x +current_pos.y * current_pos.y);
	//fwrite(&total_distance, sizeof(total_distance), 1, fp);
	//print("distacne %d \n", total_distance);
	//angleTurned = atan(current_pos.x/current_pos.y) * 57.296;//current_pos.theta * 57.29578;
	//fwrite(&angleTurned, sizeof(angleTurned), 1, fp);
	fclose(fp); 
}
	
