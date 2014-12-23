#include <stdint.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

const int64_t SIZE	= (1024 * 1024 * 16);	// 1M
const int64_t ROUNDS	= (256);

typedef union {
	struct {
		int32_t	t;
		int16_t	l, r;
	};
	uint8_t	i[8];	// (32 + 16 + 16) / 8 = 64 / 8 = 8
} b64;

typedef union {
	int32_t		t;
	struct {
		int16_t	l, r;
	};
	uint8_t i[4];
} b32;

int32_t neef32 (uint8_t *stream, size_t len) {
	size_t x;
	int y;
	b64 u;
	int32_t r = len;

	for (x = 0; x < len; x += 8) {
		for (y = 0; y < 8; y++)
			u.i[y] = 0;

		for (y = 0; ((x + y) < len) && (y < 8); y++)
			u.i[y] = stream [x + y];

		const int16_t t16	= u.l ^ u.r;
		const int32_t t32a	= ~u.t * t16;
		const int32_t t32b	= (u.t + x) * ~t16;

//		b32 o, s;
//		o.t	= t32b;
//		s.l	= o.r;
//		s.r	= o.l;

		r	^= t32a ^ t32b;
	}

	return r;
}

int main (int argc, char **argv) {
	uint8_t *inbuf = malloc (SIZE);
	assert (inbuf);

	int x;
	for (x = 0; x < ROUNDS; x++) {
		assert (fread (inbuf, sizeof (uint8_t), SIZE, stdin) == SIZE);
		b32 r;
		r.t = neef32 (inbuf, SIZE - (253 * x));
		fprintf (stdout, "Round: %i\tResult: ", x);
		fprintf (stdout,	\
				"%02x%02x%02x%02x"	\
				"\n",	\
				r.i[0], r.i[1], r.i[2], r.i[3]);
	}

	free (inbuf);
	return 0;
}
