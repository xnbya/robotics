#include "stdio.h"
#include "stdlib.h"

void pprint(int intarr[], int elements) {
	int i;
	for(i = 0; i < elements; i++)
		printf("%d \t", intarr[i]);
}

void bblSort(int inarr[], int elements) {
	int i, end, z;
	for(end = elements - 1; end >= 0; end--) {
		for(i = 0; i < end; i++) {
			if(inarr[i] > inarr[i + 1]) {
				//swap them
				z = inarr[i];
				inarr[i] = inarr[i + 1];
				inarr[i + 1] = z;
			}
			//Q2
			pprint(inarr, elements);
			printf("\n");
		}
	}
}

void bogoSort(int inarr[], int elements) {
	int i, end, z;
	int sorted = 0;
	int steps = 0;
	while(!sorted) {
		steps++;
		for(end = elements - 2; end >= 0; end--) {
			if(rand() % 2) {
				//swap them
				z = inarr[end];
				inarr[end] = inarr[end + 1];
				inarr[end + 1] = z;
			}
		}
		sorted = 1;
		for(i = 0; i < elements - 1; i++) {
			if(inarr[i] > inarr[i+1])
				sorted = 0;
		}
			//Q2
			pprint(inarr, elements);
			printf("\n");
	}
	printf("%d steps taken \n", steps);
}






int main(){

	printf("Enter number of elements \n");
	int elements = 0;
	scanf("%d", &elements);
	int intarr[elements];
	int i;
	printf("Enter %d integers \n", elements);
	for(i = 0; i < elements; i++) {
		scanf("%d", &intarr[i]);
	}
	pprint(intarr, elements);
	printf("\n sorting \n");
	bogoSort(intarr, elements);
	pprint(intarr, elements);
	return 0;
}


