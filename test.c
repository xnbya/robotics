
int power(int a, int b) {
	int z = a;
	for(int i = b; i > 0; i--) {
		z = z * a;
	}
	return z;
}

int power2(int b) {
	return power(2, b);
}

int power3(int b) {
	return power(3, b);
}

int main() {
	printf("%d \n", power(5, 3));
	int (POW*)(int a) = power2;

	printf("%d \n", power2(5));
	POW = power2;
	printf("%d \n", power2(5));
	return 0;
}

