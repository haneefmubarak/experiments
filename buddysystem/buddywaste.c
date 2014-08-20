#include <stdint.h>
#include <stdio.h>

const int32_t binary[] = {	16, 32, 64, 128, 256, 512, 1024, 2048, 4096,	\
				8192, 16384, 32768, 65536, 131072, 262144,	\
				524288, 1048576, 2097152, 4194304, 8388608,	\
				16777216 };
const int32_t fibonacci[] = {	16, 32, 48, 80, 128, 208, 336, 544, 880, 1424,	\
				2304, 3728, 6032, 9760, 15792, 25552, 41344,	\
				66896, 108240, 175136, 283376, 458512, 741888,	\
				1200400, 1942288, 3142688, 5084976, 8227664,	\
				13312640 };

#define LOOKUP	binary	// which lookup table to use
			// ie: which algorithm

#define SIZE	8388608	// 8M
			// try for sizes up to this number
			// in bytes

#define INCR	1	// how much to increase the tested
			// size by each time

#define	MINS	1	// the minimum size to test

int main (int argc, char **argv) {
	int32_t x, i;

	double total = 0, wastage = 0;	// max bit length of 45 / 52 fraction

	for (x = 1; x < SIZE; x += INCR) {	// 8M
		i = 0;
		while (x > LOOKUP[i])
			i++;

		total	+= x;
		wastage	+= (double) LOOKUP[i] - (double) x;
	}

	wastage = wastage / total;
	int waste = wastage * 100.0;

	printf ("Wasted Space:\t%i%%\n", waste);

	return 0;
}
