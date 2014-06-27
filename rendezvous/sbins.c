#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include <assert.h>

#include <math.h>

//#include <blake2.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define NODES	20
#define KEYS	16777216
#define CSKIP	1048576

__uint128_t hashes[NODES]	= {0};
int count[NODES]	= {0};

static inline int abs32 (int v) {
	unsigned r;
	int const mask = v >> (((sizeof (int)) * CHAR_BIT) - 1) ;

	r = (v + mask) ^ mask;
	return r;
}

static inline int rendezvous (__uint128_t *pool, uint64_t id) {
	register int x, bin = 0;
	register __uint128_t max = 0, cache;

	for (x = 0; x < NODES; x++) {
//		blake2 ((void *) &cache, &pool[x], &id, sizeof (cache), sizeof (pool[x]), sizeof (id));
		cache = pool[x] * ~id;
		max = MAX (max, cache);
		bin = max == cache ? x : bin;
	}

	return bin;
}

int main (int argc, char **argv) {
	assert (fread (hashes, sizeof (hashes[0]), NODES, stdin) == NODES);

	register int x, bin;
	for (x = 0; x < KEYS; x++) {
		bin = rendezvous (hashes, x);
		count[bin]++;
		if (!(x % CSKIP)) {fprintf (stdout, "%i\n", x);}
	}

	fputs ("\n\n\n\n\n\n\n\n", stdout);

	float avg = KEYS / NODES;

	float mad = 0;
	for (x = 0; x < NODES; x++) {
		fprintf (stdout, "Bin %i\t: %i\n", x, count[x]);
		mad += abs32 (count[x] - avg);
	}

	fputs ("\n\n\n\n\n\n\n\n", stdout);

	fprintf (stdout, "avg: keys/bin\t= %f\n", avg);

	mad /= NODES;
	fprintf (stdout, "mad: keys/bin\t= %f\n", mad);

	float moa = mad / avg;
	moa *= 100;
	fprintf (stdout, "mad / avg\t= %05.2f %%\n", moa);

	return 0;
}
