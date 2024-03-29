#include <stdio.h>

#define maxtower 50

//Alexis Enston - towers of hanoi

struct stack {
	int disks[maxtower]; 
	int prev[maxtower];
	int ptr;
};


struct stack towers[3];
int size = 4;
int order[3] = {0, 1, 2};

int popStack(struct stack *inStack) {
	inStack->ptr--;
	return inStack->disks[inStack->ptr+1];
}

void pushStack(struct stack *inStack, int item) {
	inStack->ptr++;
	inStack->disks[inStack->ptr] = item;
}


void printTowers(struct stack towers[],int size) {
	int i, j;
	printf("A \tB \tC \n");
	for(i = size; i >= 0; i--) {
		for(j = 0; j < 3; j++) {
			if(towers[order[j]].ptr >= i)
				printf("%d \t", towers[order[j]].disks[i]);
			else
				printf("| \t");
		}
		printf("\n");
	}
	printf("\n");
}

//move top from stack A to B
void moveS(int A, int B) {
	pushStack(&towers[B], popStack(&towers[A]));
	towers[B].prev[towers[B].ptr] = A;
	printTowers(towers, size);
}

int main() {
	//set parameters
	char start = 'B';
	printf("Enter the start: A,B or C\n");
	scanf("%c", &start);
	printf("Enter number of discs\n");
	scanf("%d", &size);
	
	switch(start) {
		case 'B':
			order[1] = 0;
			order[0] = 1;
			break;
		case 'C':
			order[2] = 0;
			order[0] = 2;
			break;
	}
	//setup the towers
	int i, j;
	for(i = 0; i < 3; i++) {
		towers[i].ptr = -1;
	}
	//add the discs
	for(i = 1; i <= size; i++) {
		pushStack(&towers[0], i);
		towers[0].prev[i] = -1;
	}
	printTowers(towers, size);
	if(size%2)
		moveS(0, 2);
	else
		moveS(0, 1);
	while(towers[2].ptr <= size - 2) {
		for(i = 0; i < 3; i++) {
			for(j = 0; j < 3; j++) {
				if((towers[j].ptr > -1) 
					&& (i != towers[j].prev[towers[j].ptr])
					&& (( (towers[i].disks[towers[i].ptr]%2 != towers[j].disks[towers[j].ptr]%2) 
					&& (towers[i].disks[towers[i].ptr] < towers[j].disks[towers[j].ptr])) || towers[i].ptr == -1) ) {
						moveS(j, i);
						j = i = 5;
				}
			}
		}
	}
				

	return 0;

}
	



