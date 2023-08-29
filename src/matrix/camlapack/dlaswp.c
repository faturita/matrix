/* DLASWP.F -- translated by f2c (version 19941215).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int dlaswp_(n, a, lda, k1, k2, ipiv, incx)
integer *n;
doublereal *a;
integer *lda, *k1, *k2, *ipiv, *incx;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;

    /* Local variables */
    static integer i;
    extern /* Subroutine */ int dswap_();
    static integer ip, ix;


/*  -- LAPACK auxiliary routine (version 1.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     October 31, 1992 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DLASWP performs a series of row interchanges on the matrix A. */
/*  One row interchange is initiated for each of rows K1 through K2 of A. 
*/

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N) */
/*          On entry, the matrix of column dimension N to which the row */
/*          interchanges will be applied. */
/*          On exit, the permuted matrix. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A. */

/*  K1      (input) INTEGER */
/*          The first element of IPIV for which a row interchange will */
/*          be done. */

/*  K2      (input) INTEGER */
/*          The last element of IPIV for which a row interchange will */
/*          be done. */

/*  IPIV    (input) INTEGER array, dimension (M*abs(INCX)) */
/*          The vector of pivot indices.  Only the elements in positions 
*/
/*          K1 through K2 of IPIV are accessed. */
/*          IPIV(K) = L implies rows K and L are to be interchanged. */

/*  INCX    (input) INTEGER */
/*          The increment between successive values of IPIV.  If IPIV */
/*          is negative, the pivots are applied in reverse order. */

/* ===================================================================== 
*/

/*     .. Local Scalars .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Interchange row I with row IPIV(I) for each of rows K1 through K2. 
*/

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    --ipiv;

    /* Function Body */
    if (*incx == 0) {
	return 0;
    }
    if (*incx > 0) {
	ix = *k1;
    } else {
	ix = (1 - *k2) * *incx + 1;
    }
    if (*incx == 1) {
	i__1 = *k2;
	for (i = *k1; i <= i__1; ++i) {
	    ip = ipiv[i];
	    if (ip != i) {
		dswap_(n, &a[i + a_dim1], lda, &a[ip + a_dim1], lda);
	    }
/* L10: */
	}
    } else if (*incx > 1) {
	i__1 = *k2;
	for (i = *k1; i <= i__1; ++i) {
	    ip = ipiv[ix];
	    if (ip != i) {
		dswap_(n, &a[i + a_dim1], lda, &a[ip + a_dim1], lda);
	    }
	    ix += *incx;
/* L20: */
	}
    } else if (*incx < 0) {
	i__1 = *k1;
	for (i = *k2; i >= i__1; --i) {
	    ip = ipiv[ix];
	    if (ip != i) {
		dswap_(n, &a[i + a_dim1], lda, &a[ip + a_dim1], lda);
	    }
	    ix += *incx;
/* L30: */
	}
    }

    return 0;

/*     End of DLASWP */

} /* dlaswp_ */

