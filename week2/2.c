#include "simpletools.h"

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
	bblSort(intarr, elements);
	pprint(intarr, elements);
	printf(" done \n");
}


