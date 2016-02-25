#include "simpletools.h"


int main() {
	sd_mount(22, 23, 24, 25);
	FILE* fp = fopen("Position.text", "r");
	float total_distance, angleTurned;
	fp = fopen("Position.text", "r");
	fread(&total_distance, 5, 1, fp);
	printf("distance travelled is %.2f\n",total_distance);
	fread(&angleTurned, 5, 1, fp);
	printf("angle is %.2f\n",angleTurned);
	fclose(fp);
	return 0;
}

