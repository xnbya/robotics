#include "simpletools.h"


int main() {
	sd_mount(22, 23, 24, 25);

	/*FILE* f = fopen("Position.text", "w");
	float total = 0.5f;
	fwrite(&total, sizeof(total), 1, f);
	fclose(f); */

	FILE* fp = fopen("position.txt", "r");
	float total_distance, angleTurned;
	fread(&total_distance, 5, 1, fp);
	//printf("distance travelled is %.2f\n",total_distance);
	fread(&angleTurned, 5, 1, fp);
	//printf("angle is %.2f\n",angleTurned);
	fclose(fp);
	return 0;
}

