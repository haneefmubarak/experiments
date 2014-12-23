#include <stdint.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

const int64_t SIZE	= (1024 * 1024 * 16);	// 1M
const int64_t ROUNDS	= (256);

typedef union {
	struct {
		int64_t	t;
		int32_t	l, r;
	};
	uint8_t	i[16];	// (64 + 32 + 32) / 8 = 128 / 8 = 16
} b128;

typedef union {
	int64_t		t;
	struct {
		int32_t	l, r;
	};
	uint8_t i[8];
} b64;

int64_t neef64 (uint8_t *stream, size_t len) {
	size_t x;
	int y;
	b128 u;
	int64_t r = len;

	for (x = 0; x < len; x += 16) {
		for (y = 0; y < 16; y++)
			u.i[y] = 0;

		for (y = 0; ((x + y) < len) && (y < 16); y++)
			u.i[y] = stream [x + y];

		const int32_t t32	= u.l ^ u.r;
		const int64_t t64a	= ~u.t * t32;
		const int64_t t64b	= (u.t + x) * ~t32;

//		b64 o, s;
//		o.t	= t64b;
//		s.l	= o.r;
//		s.r	= o.l;

		r	^= t64a ^ t64b;
	}

	return r;
}

int main (int argc, char **argv) {
	uint8_t *inbuf = malloc (SIZE);
	assert (inbuf);

	int x;
	for (x = 0; x < ROUNDS; x++) {
		assert (fread (inbuf, sizeof (uint8_t), SIZE, stdin) == SIZE);
		b64 r;
		r.t = neef64 (inbuf, SIZE - (253 * x));
		fprintf (stdout, "Round: %i\tResult: ", x);
		fprintf (stdout,	\
				"%02x%02x%02x%02x%02x%02x%02x%02x"	\
				"\n",	\
				r.i[0], r.i[1], r.i[2], r.i[3],	\
				r.i[4], r.i[5], r.i[6], r.i[7]);
	}

	free (inbuf);
	return 0;
}
