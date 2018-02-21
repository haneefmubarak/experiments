#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <stdio.h>

#include <mkl.h>

const int matrix_layout = LAPACK_COL_MAJOR;

static inline size_t matrix (size_t row, size_t col, size_t rows, size_t cols, int layout) {
	switch (layout) {
		case LAPACK_COL_MAJOR: {
			return (col * rows) + row;
		}

		case LAPACK_ROW_MAJOR: {
			return (row * cols) + col;
		}

		default: {
			break;
		}
	}

	puts ("matrix(): invalid layout");
	exit (1);
}

void printarray_lapack_i (const char *s, const lapack_int *i, const int n) {
	printf ("\nArray %s:\n", s);

	for (int x = 0; x < n; x++) {
		printf ("%lli\n", i[x]);
	}

	puts ("");
}

void printarray_d (const char *s, const double *d, const int n) {
	printf ("\nArray %s:\t", s);

	for (int x = 0; x < n; x++) {
		printf ("%G\n", d[x]);
	}

	puts ("");
}

void printmatrix_d (const char *s, const double *d, const int rows, const int cols, int layout) {
	printf ("\nMatrix %s:\n", s);

	for (int x = 0; x < rows; x++) {
		for (int y = 0; y < cols; y++) {
			printf ("%G\t", d[matrix (x, y, rows, cols, layout)]);
		}

		puts ("");
	}

	puts ("");
}

int main (const int argc, const char **argv) {
	const int args = 2;

	if ((argc < args) || memcmp (argv[1], "p=", 2)) {
		printf ("usage:\t%s p=degree x_0 y_0 x_1 y_1 [x_n y_n ...]\n", argv[0]);
		return 1;
	}

	const int p = atof (&argv[1][2]) + 1;

	const int n = (argc - 1) / 2;

	if ((argc - args) % 2) {
		printf ("error: missing a y-value for last x-value [%s]\n",
			argv[argc - 1]
		);

		return 1;
	} else if ((p - 1) >= n) {
		printf ("error: cannot regress a polynomial of degree %i with only %i values\n",
			p - 1, n
		);

		return 1;
	}

	const int m = p * p;
	const int s = (p * 2) - 1;

	double *x = malloc (sizeof (double) * n);
	double *y = malloc (sizeof (double) * n);
	double *a = malloc (sizeof (double) * n);
	double *b = malloc (sizeof (double) * n);
	double *mA = malloc (sizeof (double) * m);
	double *mB = malloc (sizeof (double) * p);
	double *mS = malloc (sizeof (double) * s);
	lapack_int *ipiv = malloc (sizeof (lapack_int) * p);
	if (!(x && y && mA && mB && mS && ipiv)) {
		free (x);
		free (y);
		free (mA);
		free (mB);
		free (mS);
		free (ipiv);

		puts ("insufficient memory");

		return 1;
	}

	for (int i = 0; i < n; i++) {
		const int t = (i * 2) + args;
		x[i] = atof (argv[t]);
		y[i] = atof (argv[t + 1]);
	}

	printarray_d ("x", x, n);
	printarray_d ("y", y, n);

	mS[0] = n;
	mS[1] = cblas_dasum (n, x, 1);
	for (int i = 2; i < s; i++) {
		vdPowx (n, x, i, a);
		mS[i] = cblas_dasum (n, a, 1);
	}

	printarray_d ("mS", mS, s);

	for (int i = 0; i < p; i++) {
		for (int j = 0; j < p; j++) {
			mA[matrix (i, j, p, p, matrix_layout)] = mS[i + j];
		}
	}

	printmatrix_d ("mA", mA, p, p, matrix_layout);

	mB[0] = cblas_dasum (n, y, 1);
	vdMul (n, x, y, a);
	mB[1] = cblas_dasum (n, a, 1);
	for (int i = 2; i < p; i++) {
		vdPowx (n, x, i, a);
		vdMul (n, a, y, b);

		mB[i] = cblas_dasum (n, b, 1);
	}

	printmatrix_d ("mB", mB, p, 1, matrix_layout);

	LAPACKE_dgesv (matrix_layout, p, 1, mA, p, ipiv, mB, p);
	double *mX = mB;

	printmatrix_d ("mX", mX, p, 1, matrix_layout);
	printarray_lapack_i ("ipiv", ipiv, p);

	for (int i = 0; i < p; i++) {
		mX[i] = !mX[i] ? 0 : mX[i];
	}

	for (int i = 0; i < p; i++) {
		const int t = ipiv[i], u = t - 1;
//		printf ("a_%i * x ^ %i, a_%i = %f\n", u, u, u, mX[t]);
		printf ("a_%i * x ^ %i, a_%i = %f\n", i, i, i, mX[i]);
	}

	return 0;
}
