/* DGECON.F -- translated by f2c (version 19941215).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int dgecon_(norm, n, a, lda, anorm, rcond, work, iwork, info,
	 norm_len)
char *norm;
integer *n;
doublereal *a;
integer *lda;
doublereal *anorm, *rcond, *work;
integer *iwork, *info;
ftnlen norm_len;
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static integer kase, kase1;
    static doublereal scale;
    extern logical lsame_();
    extern /* Subroutine */ int drscl_();
    extern doublereal dlamch_();
    static doublereal sl;
    static integer ix;
    extern /* Subroutine */ int dlacon_();
    extern integer idamax_();
    static doublereal su;
    extern /* Subroutine */ int xerbla_();
    static doublereal ainvnm;
    extern /* Subroutine */ int dlatrs_();
    static logical onenrm;
    static char normin[1];
    static doublereal smlnum;


/*  -- LAPACK routine (version 1.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     February 29, 1992 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGECON estimates the reciprocal of the condition number of a general 
*/
/*  real matrix A, in either the 1-norm or the infinity-norm, using */
/*  the LU factorization computed by DGETRF. */

/*  An estimate is obtained for norm(inv(A)), and the reciprocal of the */
/*  condition number is computed as */
/*     RCOND = 1 / ( norm(A) * norm(inv(A)) ). */

/*  Arguments */
/*  ========= */

/*  NORM    (input) CHARACTER*1 */
/*          Specifies whether the 1-norm condition number or the */
/*          infinity-norm condition number is required: */
/*          = '1' or 'O':  1-norm; */
/*          = 'I':         Infinity-norm. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  A       (input) DOUBLE PRECISION array, dimension (LDA,N) */
/*          The factors L and U from the factorization A = P*L*U */
/*          as computed by DGETRF. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  ANORM   (input) DOUBLE PRECISION */
/*          If NORM = '1' or 'O', the 1-norm of the original matrix A. */
/*          If NORM = 'I', the infinity-norm of the original matrix A. */

/*  RCOND   (output) DOUBLE PRECISION */
/*          The reciprocal of the condition number of the matrix A, */
/*          computed as RCOND = 1/(norm(A) * norm(inv(A))). */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (4*N) */

/*  IWORK   (workspace) INTEGER array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */

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
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    onenrm = *(unsigned char *)norm == '1' || lsame_(norm, "O", 1L, 1L);
    if (! onenrm && ! lsame_(norm, "I", 1L, 1L)) {
	*info = -1;
    } else if (*n < 0) {
	*info = -2;
    } else if (*lda < max(1,*n)) {
	*info = -4;
    } else if (*anorm < 0.) {
	*info = -5;
    }
    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGECON", &i__1, 6L);
	return 0;
    }

/*     Quick return if possible */

    *rcond = 0.;
    if (*n == 0) {
	*rcond = 1.;
	return 0;
    } else if (*anorm == 0.) {
	return 0;
    }

    smlnum = dlamch_("Safe minimum", 12L);

/*     Estimate the norm of inv(A). */

    ainvnm = 0.;
    *(unsigned char *)normin = 'N';
    if (onenrm) {
	kase1 = 1;
    } else {
	kase1 = 2;
    }
    kase = 0;
L10:
    dlacon_(n, &work[*n + 1], &work[1], &iwork[1], &ainvnm, &kase);
    if (kase != 0) {
	if (kase == kase1) {

/*           Multiply by inv(L). */

	    dlatrs_("Lower", "No transpose", "Unit", normin, n, &a[a_offset], 
		    lda, &work[1], &sl, &work[(*n << 1) + 1], info, 5L, 12L, 
		    4L, 1L);

/*           Multiply by inv(U). */

	    dlatrs_("Upper", "No transpose", "Non-unit", normin, n, &a[
		    a_offset], lda, &work[1], &su, &work[*n * 3 + 1], info, 
		    5L, 12L, 8L, 1L);
	} else {

/*           Multiply by inv(U'). */

	    dlatrs_("Upper", "Transpose", "Non-unit", normin, n, &a[a_offset],
		     lda, &work[1], &su, &work[*n * 3 + 1], info, 5L, 9L, 8L, 
		    1L);

/*           Multiply by inv(L'). */

	    dlatrs_("Lower", "Transpose", "Unit", normin, n, &a[a_offset], 
		    lda, &work[1], &sl, &work[(*n << 1) + 1], info, 5L, 9L, 
		    4L, 1L);
	}

/*        Divide X by 1/(SL*SU) if doing so will not cause overflow. 
*/

	scale = sl * su;
	*(unsigned char *)normin = 'Y';
	if (scale != 1.) {
	    ix = idamax_(n, &work[1], &c__1);
	    if (scale < (d__1 = work[ix], abs(d__1)) * smlnum || scale == 0.) 
		    {
		goto L20;
	    }
	    drscl_(n, &scale, &work[1], &c__1);
	}
	goto L10;
    }

/*     Compute the estimate of the reciprocal condition number. */

    if (ainvnm != 0.) {
	*rcond = 1. / ainvnm / *anorm;
    }

L20:
    return 0;

/*     End of DGECON */

} /* dgecon_ */

