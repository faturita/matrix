/* DGEEQU.F -- translated by f2c (version 19941215).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int dgeequ_(m, n, a, lda, r, c, rowcnd, colcnd, amax, info)
integer *m, *n;
doublereal *a;
integer *lda;
doublereal *r, *c, *rowcnd, *colcnd, *amax;
integer *info;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Local variables */
    static integer i, j;
    static doublereal rcmin, rcmax;
    extern doublereal dlamch_();
    extern /* Subroutine */ int xerbla_();
    static doublereal bignum, smlnum;


/*  -- LAPACK routine (version 1.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     March 31, 1993 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGEEQU computes row and column scalings intended to equilibrate an */
/*  M-by-N matrix A and reduce its condition number.  R returns the row */
/*  scale factors and C the column scale factors, chosen to try to make */
/*  the largest entry in each row and column of the matrix B with */
/*  elements B(i,j)=R(i)*A(i,j)*C(j) have absolute value 1. */

/*  R(i) and C(j) are restricted to be between SMLNUM = smallest safe */
/*  number and BIGNUM = largest safe number.  Use of these scaling */
/*  factors is not guaranteed to reduce the condition number of A but */
/*  works well in practice. */

/*  Arguments */
/*  ========= */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  A       (input) DOUBLE PRECISION array, dimension (LDA,N) */
/*          The M-by-N matrix whose equilibration factors are */
/*          to be computed. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,M). */

/*  R       (output) DOUBLE PRECISION array, dimension (M) */
/*          If INFO = 0 or INFO > M, R contains the row scale factors */
/*          for A. */

/*  C       (output) DOUBLE PRECISION array, dimension (N) */
/*          If INFO = 0,  C contains the column scale factors for A. */

/*  ROWCND  (output) DOUBLE PRECISION */
/*          If INFO = 0 or INFO > M, ROWCND contains the ratio of the */
/*          smallest R(i) to the largest R(i).  If ROWCND >= 0.1 and */
/*          AMAX is neither too large nor too small, it is not worth */
/*          scaling by R. */

/*  COLCND  (output) DOUBLE PRECISION */
/*          If INFO = 0, COLCND contains the ratio of the smallest */
/*          C(i) to the largest C(i).  If COLCND >= 0.1, it is not */
/*          worth scaling by C. */

/*  AMAX    (output) DOUBLE PRECISION */
/*          Absolute value of largest matrix element.  If AMAX is very */
/*          close to overflow or very close to underflow, the matrix */
/*          should be scaled. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i,  and i is */
/*                <= M:  the i-th row of A is exactly zero */
/*                >  M:  the (i-M)-th column of A is exactly zero */

/*  ===================================================================== 
*/

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    --r;
    --c;

    /* Function Body */
    *info = 0;
    if (*m < 0) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*m)) {
	*info = -4;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGEEQU", &i__1, 6L);
	return 0;
    }

/*     Quick return if possible */

    if (*m == 0 || *n == 0) {
	*rowcnd = 1.;
	*colcnd = 1.;
	*amax = 0.;
	return 0;
    }

/*     Get machine constants. */

    smlnum = dlamch_("S", 1L);
    bignum = 1. / smlnum;

/*     Compute row scale factors. */

    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
	r[i] = 0.;
/* L10: */
    }

/*     Find the maximum element in each row. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
/* Computing MAX */
	    d__2 = r[i], d__3 = (d__1 = a[i + j * a_dim1], abs(d__1));
	    r[i] = max(d__2,d__3);
/* L20: */
	}
/* L30: */
    }

/*     Find the maximum and minimum scale factors. */

    rcmin = bignum;
    rcmax = 0.;
    i__1 = *m;
    for (i = 1; i <= i__1; ++i) {
/* Computing MAX */
	d__1 = rcmax, d__2 = r[i];
	rcmax = max(d__1,d__2);
/* Computing MIN */
	d__1 = rcmin, d__2 = r[i];
	rcmin = min(d__1,d__2);
/* L40: */
    }
    *amax = rcmax;

    if (rcmin == 0.) {

/*        Find the first zero scale factor and return an error code. 
*/

	i__1 = *m;
	for (i = 1; i <= i__1; ++i) {
	    if (r[i] == 0.) {
		*info = i;
		return 0;
	    }
/* L50: */
	}
    } else {

/*        Invert the scale factors. */

	i__1 = *m;
	for (i = 1; i <= i__1; ++i) {
/* Computing MIN */
/* Computing MAX */
	    d__2 = r[i];
	    d__1 = max(d__2,smlnum);
	    r[i] = 1. / min(d__1,bignum);
/* L60: */
	}

/*        Compute ROWCND = min(R(I)) / max(R(I)) */

	*rowcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }

/*     Compute column scale factors */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	c[j] = 0.;
/* L70: */
    }

/*     Find the maximum element in each column, */
/*     assuming the row scaling computed above. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *m;
	for (i = 1; i <= i__2; ++i) {
/* Computing MAX */
	    d__2 = c[j], d__3 = (d__1 = a[i + j * a_dim1], abs(d__1)) * r[i];
	    c[j] = max(d__2,d__3);
/* L80: */
	}
/* L90: */
    }

/*     Find the maximum and minimum scale factors. */

    rcmin = bignum;
    rcmax = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
	d__1 = rcmin, d__2 = c[j];
	rcmin = min(d__1,d__2);
/* Computing MAX */
	d__1 = rcmax, d__2 = c[j];
	rcmax = max(d__1,d__2);
/* L100: */
    }

    if (rcmin == 0.) {

/*        Find the first zero scale factor and return an error code. 
*/

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    if (c[j] == 0.) {
		*info = *m + j;
		return 0;
	    }
/* L110: */
	}
    } else {

/*        Invert the scale factors. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
/* Computing MAX */
	    d__2 = c[j];
	    d__1 = max(d__2,smlnum);
	    c[j] = 1. / min(d__1,bignum);
/* L120: */
	}

/*        Compute COLCND = min(C(J)) / max(C(J)) */

	*colcnd = max(rcmin,smlnum) / min(rcmax,bignum);
    }

    return 0;

/*     End of DGEEQU */

} /* dgeequ_ */

