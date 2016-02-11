#include <stdio.h>

#define maxtower 50

struct stack {
	int disks[maxtower]; 
	int ptr;
};

int popStack(struct stack *inStack) {
	inStack->ptr--;
	return inStack->disks[inStack->ptr+1];
}

void pushStack(struct stack *inStack, int item) {
	inStack->ptr++;
	inStack->disks[inStack->ptr] = item;
}

void printTowers(struct stack *towers[],int size) {
	int i, j;
	for(i = size; i >= 0; i--) {
		for(j = 0; j < 3; j++) {
			if(*towers[j].ptr >= i)
				printf("%d \t", *towers[j].disks[i]);
			else
				printf("| \t");
		}
		printf("\n");
	}
}

int main() {
	//set parameters
	int size = 5;
	char start = 'A';
	//setup the towers
	int i;
	struct stack towers[3];
	int aTower = start - 'A';
	int bTower = aTower + 1 % 3;
	int cTower = bTower + 1 % 3;
	for(i = 0; i < 3; i++) {
		towers[i].ptr = -1;
	}
	//add the discs
	for(i = size; i > 0; i--) {
		pushStack(&towers[aTower], i);
	}
	//solve
	printTowers(*towers, size);
	return 0;
}
	



