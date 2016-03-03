#include "simpletools.h"
#include <math.h>

float total_distance;
float angleTurned;


struct location{
	float x;
	float y;
	float theta;
};

struct location current_pos;



int main() {
	sd_mount(22, 23, 24, 25);

	/*FILE* f = fopen("Position.text", "w");
	float total = 0.5f;
	fwrite(&total, sizeof(total), 1, f);
	fclose(f); */

	FILE* fp = fopen("position.txt", "r");
	fread(&current_pos, sizeof(current_pos), 1, fp);
	total_distance = sqrt(current_pos.x * current_pos.x +current_pos.y * current_pos.y);
	//fwrite(&total_distance, sizeof(total_distance), 1, fp);
	print("distacne x %f distance y %f \n", current_pos.x, current_pos.y);
	angleTurned = atan(current_pos.x/current_pos.y) * 57.296;//current_pos.theta * 57.29578;
	//fwrite(&angleTurned, sizeof(angleTurned), 1, fp);
	printf("distance travelled is %.2f\n",total_distance);
	printf("angle is %.2f\n",angleTurned);
	fclose(fp);
	return 0;
}

