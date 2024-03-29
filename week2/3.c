#include "simpletools.h"
#include "abdrive.h"

#define GAPS 63

//bubble sort
void bblSort(int inarr[], int elements) {
	int i, end, z;
	for(end = elements - 1; end >= 0; end--) {
		for(i = 0; i < end; i++) {
			//Move robot
			drive_goto(GAPS,GAPS);

			if(inarr[i] > inarr[i + 1]) {
				//swap them
				z = inarr[i];
				inarr[i] = inarr[i + 1];
				inarr[i + 1] = z;
				//blink
				//
				for(z = 0; z < 10; z++) {
					high(26);
					pause(100);
					low(26);
					pause(100);
				}
			}

		}
		//drive back
		drive_goto(-GAPS * end, -GAPS * end);
	}
}



int main(){

	int elements = 5;
	int intarr[5] = {101,18,3,1,9};
	bblSort(intarr, elements);
	drive_goto(GAPS*2, GAPS*2);
	drive_goto(51,0);
}


