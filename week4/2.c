#include "simpletools.h"
#include "abdrive.h"


#define maxtower 50
#define GAPS 63

//Alexis Enston - Towers of Hanoi + motor

struct stack {
	int disks[maxtower]; 
	int prev[maxtower];
	int ptr;
};


struct stack towers[3];
int size = 4;
int order[3] = {0, 1, 2};

//current location of robot
int location = 0;

int popStack(struct stack *inStack) {
	inStack->ptr--;
	return inStack->disks[inStack->ptr+1];
}

void pushStack(struct stack *inStack, int item) {
	inStack->ptr++;
	inStack->disks[inStack->ptr] = item;
}

//move top from stack A to B
void moveS(int A, int B) {
	pushStack(&towers[B], popStack(&towers[A]));
	towers[B].prev[towers[B].ptr] = A;
	int from = order[A];
	int to = order[B];
	//show movement
	drive_goto((from - location) * GAPS, (from - location) * GAPS);
	location += (from - location);
	//switch on led
	high(26);
	drive_goto((to - location) * GAPS, (to - location) * GAPS);
	location += (to - location);
	low(26);

}

int main() {
	//set parameters
	char start = 'A';
	size = 3;

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
	



