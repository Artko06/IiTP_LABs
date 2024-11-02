#include <iostream>
int main() {
	long long sum1 = 0, sum2 = 0, N;
	short i = 1;
	while (i <= 30) {
		if (i % 2 != 0) sum1 = sum1 + (i - i * i) * (i - i * i);
		if (i % 2 == 0) sum2 = sum2 + (i / 2.0 - i * i * i) * (i / 2.0 - i * i * i);
		i++;
	}
	N = sum1 + sum2;
	std::cout << "N = " << N;
	return 0;
}