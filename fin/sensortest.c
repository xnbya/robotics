#include "simpletools.h"
#include "ping.h"

int distance, irLeft, irRight;

int main() {
	low(26);
	low(27);

	while (1) {
		distance = ping_cm(8);
		print("front distance = %d cm \n", distance);
		freqout(11, 1, 38000);
		irLeft = input(10);

		freqout(1,1,38000);
		irRight = input(2);

		print("left %d right %d \n", irLeft, irRight);
		pause(200);
	}
}
