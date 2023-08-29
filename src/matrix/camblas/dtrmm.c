/* DTRMM.F -- translated by f2c (version 19941215).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "cblasimpexp.h"

/* DECK DTRMM */
/* Subroutine */ int __IMPEXP__ dtrmm_(side, uplo, transa, diag, m, n, alpha, a, lda, b,
	ldb, side_len, uplo_len, transa_len, diag_len)
char *side, *uplo, *transa, *diag;
integer *m, *n;
doublereal *alpha, *a;
integer *lda;
doublereal *b;
integer *ldb;
ftnlen side_len;
ftnlen uplo_len;
ftnlen transa_len;
ftnlen diag_len;
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer info;
    static doublereal temp;
    static integer i, j, k;
    static logical lside;
    extern logical lsame_();
    static integer nrowa;
    static logical upper;
    extern /* Subroutine */ int xerbla_();
    static logical nounit;

/* ***BEGIN PROLOGUE  DTRMM */
/* ***PURPOSE  Perform one of the matrix-matrix operations. */
/* ***LIBRARY   SLATEC (BLAS) */
/* ***CATEGORY  D1B6 */
/* ***TYPE      DOUBLE PRECISION (STRMM-S, DTRMM-D, CTRMM-C) */
/* ***KEYWORDS  LEVEL 3 BLAS, LINEAR ALGEBRA */
/* ***AUTHOR  Dongarra, J., (ANL) */
/*           Duff, I., (AERE) */
/*           Du Croz, J., (NAG) */
/*           Hammarling, S. (NAG) */
/* ***DESCRIPTION */

/*  DTRMM  performs one of the matrix-matrix operations */

/*     B := alpha*op( A )*B,   or   B := alpha*B*op( A ), */

/*  where  alpha  is a scalar,  B  is an m by n matrix,  A  is a unit, or 
*/
/*  non-unit,  upper or lower triangular matrix  and  op( A )  is one  of 
*/

/*     op( A ) = A   or   op( A ) = A'. */

/*  Parameters */
/*  ========== */

/*  SIDE   - CHARACTER*1. */
/*           On entry,  SIDE specifies whether  op( A ) multiplies B from 
*/
/*           the left or right as follows: */

/*              SIDE = 'L' or 'l'   B := alpha*op( A )*B. */

/*              SIDE = 'R' or 'r'   B := alpha*B*op( A ). */

/*           Unchanged on exit. */

/*  UPLO   - CHARACTER*1. */
/*           On entry, UPLO specifies whether the matrix A is an upper or 
*/
/*           lower triangular matrix as follows: */

/*              UPLO = 'U' or 'u'   A is an upper triangular matrix. */

/*              UPLO = 'L' or 'l'   A is a lower triangular matrix. */

/*           Unchanged on exit. */

/*  TRANSA - CHARACTER*1. */
/*           On entry, TRANSA specifies the form of op( A ) to be used in 
*/
/*           the matrix multiplication as follows: */

/*              TRANSA = 'N' or 'n'   op( A ) = A. */

/*              TRANSA = 'T' or 't'   op( A ) = A'. */

/*              TRANSA = 'C' or 'c'   op( A ) = A'. */

/*           Unchanged on exit. */

/*  DIAG   - CHARACTER*1. */
/*           On entry, DIAG specifies whether or not A is unit triangular 
*/
/*           as follows: */

/*              DIAG = 'U' or 'u'   A is assumed to be unit triangular. */

/*              DIAG = 'N' or 'n'   A is not assumed to be unit */
/*                                  triangular. */

/*           Unchanged on exit. */

/*  M      - INTEGER. */
/*           On entry, M specifies the number of rows of B. M must be at 
*/
/*           least zero. */
/*           Unchanged on exit. */

/*  N      - INTEGER. */
/*           On entry, N specifies the number of columns of B.  N must be 
*/
/*           at least zero. */
/*           Unchanged on exit. */

/*  ALPHA  - DOUBLE PRECISION. */
/*           On entry,  ALPHA specifies the scalar  alpha. When  alpha is 
*/
/*           zero then  A is not referenced and  B need not be set before 
*/
/*           entry. */
/*           Unchanged on exit. */

/*  A      - DOUBLE PRECISION array of DIMENSION ( LDA, k ), where k is m 
*/
/*           when  SIDE = 'L' or 'l'  and is  n  when  SIDE = 'R' or 'r'. 
*/
/*           Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k 
*/
/*           upper triangular part of the array  A must contain the upper 
*/
/*           triangular matrix  and the strictly lower triangular part of 
*/
/*           A is not referenced. */
/*           Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k 
*/
/*           lower triangular part of the array  A must contain the lower 
*/
/*           triangular matrix  and the strictly upper triangular part of 
*/
/*           A is not referenced. */
/*           Note that when  DIAG = 'U' or 'u',  the diagonal elements of 
*/
/*           A  are not referenced either,  but are assumed to be  unity. 
*/
/*           Unchanged on exit. */

/*  LDA    - INTEGER. */
/*           On entry, LDA specifies the first dimension of A as declared 
*/
/*           in the calling (sub) program.  When  SIDE = 'L' or 'l'  then 
*/
/*           LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r' 
*/
/*           then LDA must be at least max( 1, n ). */
/*           Unchanged on exit. */

/*  B      - DOUBLE PRECISION array of DIMENSION ( LDB, n ). */
/*           Before entry,  the leading  m by n part of the array  B must 
*/
/*           contain the matrix  B,  and  on exit  is overwritten  by the 
*/
/*           transformed matrix. */

/*  LDB    - INTEGER. */
/*           On entry, LDB specifies the first dimension of B as declared 
*/
/*           in  the  calling  (sub)  program.   LDB  must  be  at  least 
*/
/*           max( 1, m ). */
/*           Unchanged on exit. */

/* ***REFERENCES  Dongarra, J., Du Croz, J., Duff, I., and Hammarling, S. 
*/
/*                 A set of level 3 basic linear algebra subprograms. */
/*                 ACM TOMS, Vol. 16, No. 1, pp. 1-17, March 1990. */
/* ***ROUTINES CALLED  LSAME, XERBLA */
/* ***REVISION HISTORY  (YYMMDD) */
/*   890208  DATE WRITTEN */
/*   910605  Modified to meet SLATEC prologue standards.  Only comment */
/*           lines were modified.  (BKS) */
/* ***END PROLOGUE  DTRMM */
/*     .. Scalar Arguments .. */
/*     .. Array Arguments .. */
/*     .. External Functions .. */
/*     .. External Subroutines .. */
/*     .. Intrinsic Functions .. */
/*     .. Local Scalars .. */
/*     .. Parameters .. */
/* ***FIRST EXECUTABLE STATEMENT  DTRMM */

/*     Test the input parameters. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = b_dim1 + 1;
    b -= b_offset;

    /* Function Body */
    lside = lsame_(side, "L", 1L, 1L);
    if (lside) {
	nrowa = *m;
    } else {
	nrowa = *n;
    }
    nounit = lsame_(diag, "N", 1L, 1L);
    upper = lsame_(uplo, "U", 1L, 1L);

    info = 0;
    if (! lside && ! lsame_(side, "R", 1L, 1L)) {
	info = 1;
    } else if (! upper && ! lsame_(uplo, "L", 1L, 1L)) {
	info = 2;
    } else if (! lsame_(transa, "N", 1L, 1L) && ! lsame_(transa, "T", 1L, 1L) 
	    && ! lsame_(transa, "C", 1L, 1L)) {
	info = 3;
    } else if (! lsame_(diag, "U", 1L, 1L) && ! lsame_(diag, "N", 1L, 1L)) {
	info = 4;
    } else if (*m < 0) {
	info = 5;
    } else if (*n < 0) {
	info = 6;
    } else if (*lda < max(1,nrowa)) {
	info = 9;
    } else if (*ldb < max(1,*m)) {
	info = 11;
    }
    if (info != 0) {
	xerbla_("DTRMM ", &info, 6L);
	return 0;
    }

/*     Quick return if possible. */

    if (*n == 0) {
	return 0;
    }

/*     And when  alpha.eq.zero. */

    if (*alpha == 0.) {
	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *m;
	    for (i = 1; i <= i__2; ++i) {
		b[i + j * b_dim1] = 0.;
/* L10: */
	    }
/* L20: */
	}
	return 0;
    }

/*     Start the operations. */

    if (lside) {
	if (lsame_(transa, "N", 1L, 1L)) {

/*           Form  B := alpha*A*B. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (k = 1; k <= i__2; ++k) {
			if (b[k + j * b_dim1] != 0.) {
			    temp = *alpha * b[k + j * b_dim1];
			    i__3 = k - 1;
			    for (i = 1; i <= i__3; ++i) {
				b[i + j * b_dim1] += temp * a[i + k * a_dim1];
/* L30: */
			    }
			    if (nounit) {
				temp *= a[k + k * a_dim1];
			    }
			    b[k + j * b_dim1] = temp;
			}
/* L40: */
		    }
/* L50: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (k = *m; k >= 1; --k) {
			if (b[k + j * b_dim1] != 0.) {
			    temp = *alpha * b[k + j * b_dim1];
			    b[k + j * b_dim1] = temp;
			    if (nounit) {
				b[k + j * b_dim1] *= a[k + k * a_dim1];
			    }
			    i__2 = *m;
			    for (i = k + 1; i <= i__2; ++i) {
				b[i + j * b_dim1] += temp * a[i + k * a_dim1];
/* L60: */
			    }
			}
/* L70: */
		    }
/* L80: */
		}
	    }
	} else {

/*           Form  B := alpha*B*A'. */

	    if (upper) {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    for (i = *m; i >= 1; --i) {
			temp = b[i + j * b_dim1];
			if (nounit) {
			    temp *= a[i + i * a_dim1];
			}
			i__2 = i - 1;
			for (k = 1; k <= i__2; ++k) {
			    temp += a[k + i * a_dim1] * b[k + j * b_dim1];
/* L90: */
			}
			b[i + j * b_dim1] = *alpha * temp;
/* L100: */
		    }
/* L110: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    i__2 = *m;
		    for (i = 1; i <= i__2; ++i) {
			temp = b[i + j * b_dim1];
			if (nounit) {
			    temp *= a[i + i * a_dim1];
			}
			i__3 = *m;
			for (k = i + 1; k <= i__3; ++k) {
			    temp += a[k + i * a_dim1] * b[k + j * b_dim1];
/* L120: */
			}
			b[i + j * b_dim1] = *alpha * temp;
/* L130: */
		    }
/* L140: */
		}
	    }
	}
    } else {
	if (lsame_(transa, "N", 1L, 1L)) {

/*           Form  B := alpha*B*A. */

	    if (upper) {
		for (j = *n; j >= 1; --j) {
		    temp = *alpha;
		    if (nounit) {
			temp *= a[j + j * a_dim1];
		    }
		    i__1 = *m;
		    for (i = 1; i <= i__1; ++i) {
			b[i + j * b_dim1] = temp * b[i + j * b_dim1];
/* L150: */
		    }
		    i__1 = j - 1;
		    for (k = 1; k <= i__1; ++k) {
			if (a[k + j * a_dim1] != 0.) {
			    temp = *alpha * a[k + j * a_dim1];
			    i__2 = *m;
			    for (i = 1; i <= i__2; ++i) {
				b[i + j * b_dim1] += temp * b[i + k * b_dim1];
/* L160: */
			    }
			}
/* L170: */
		    }
/* L180: */
		}
	    } else {
		i__1 = *n;
		for (j = 1; j <= i__1; ++j) {
		    temp = *alpha;
		    if (nounit) {
			temp *= a[j + j * a_dim1];
		    }
		    i__2 = *m;
		    for (i = 1; i <= i__2; ++i) {
			b[i + j * b_dim1] = temp * b[i + j * b_dim1];
/* L190: */
		    }
		    i__2 = *n;
		    for (k = j + 1; k <= i__2; ++k) {
			if (a[k + j * a_dim1] != 0.) {
			    temp = *alpha * a[k + j * a_dim1];
			    i__3 = *m;
			    for (i = 1; i <= i__3; ++i) {
				b[i + j * b_dim1] += temp * b[i + k * b_dim1];
/* L200: */
			    }
			}
/* L210: */
		    }
/* L220: */
		}
	    }
	} else {

/*           Form  B := alpha*B*A'. */

	    if (upper) {
		i__1 = *n;
		for (k = 1; k <= i__1; ++k) {
		    i__2 = k - 1;
		    for (j = 1; j <= i__2; ++j) {
			if (a[j + k * a_dim1] != 0.) {
			    temp = *alpha * a[j + k * a_dim1];
			    i__3 = *m;
			    for (i = 1; i <= i__3; ++i) {
				b[i + j * b_dim1] += temp * b[i + k * b_dim1];
/* L230: */
			    }
			}
/* L240: */
		    }
		    temp = *alpha;
		    if (nounit) {
			temp *= a[k + k * a_dim1];
		    }
		    if (temp != 1.) {
			i__2 = *m;
			for (i = 1; i <= i__2; ++i) {
			    b[i + k * b_dim1] = temp * b[i + k * b_dim1];
/* L250: */
			}
		    }
/* L260: */
		}
	    } else {
		for (k = *n; k >= 1; --k) {
		    i__1 = *n;
		    for (j = k + 1; j <= i__1; ++j) {
			if (a[j + k * a_dim1] != 0.) {
			    temp = *alpha * a[j + k * a_dim1];
			    i__2 = *m;
			    for (i = 1; i <= i__2; ++i) {
				b[i + j * b_dim1] += temp * b[i + k * b_dim1];
/* L270: */
			    }
			}
/* L280: */
		    }
		    temp = *alpha;
		    if (nounit) {
			temp *= a[k + k * a_dim1];
		    }
		    if (temp != 1.) {
			i__1 = *m;
			for (i = 1; i <= i__1; ++i) {
			    b[i + k * b_dim1] = temp * b[i + k * b_dim1];
/* L290: */
			}
		    }
/* L300: */
		}
	    }
	}
    }

    return 0;

/*     End of DTRMM . */

} /* dtrmm_ */

