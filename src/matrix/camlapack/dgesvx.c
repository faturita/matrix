/* DGESVX.F -- translated by f2c (version 19941215).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "camlaimpexp.h"

/* Subroutine */ int __IMPEXP__ dgesvx_(fact, trans, n, nrhs, a, lda, af, ldaf, ipiv,
	equed, r, c, b, ldb, x, ldx, rcond, ferr, berr, work, iwork, info, 
	fact_len, trans_len, equed_len)
char *fact, *trans;
integer *n, *nrhs;
doublereal *a;
integer *lda;
doublereal *af;
integer *ldaf, *ipiv;
char *equed;
doublereal *r, *c, *b;
integer *ldb;
doublereal *x;
integer *ldx;
doublereal *rcond, *ferr, *berr, *work;
integer *iwork, *info;
ftnlen fact_len;
ftnlen trans_len;
ftnlen equed_len;
{
    /* System generated locals */
    integer a_dim1, a_offset, af_dim1, af_offset, b_dim1, b_offset, x_dim1, 
	    x_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal amax;
    static char norm[1];
    static integer i, j;
    extern logical lsame_();
    static doublereal rcmin, rcmax, anorm;
    static logical equil;
    extern doublereal dlamch_(), dlange_();
    extern /* Subroutine */ int dlaqge_(), dgecon_();
    static doublereal colcnd;
    static logical nofact;
    extern /* Subroutine */ int dgeequ_(), dgerfs_(), dgetrf_(), dlacpy_(), 
	    xerbla_();
    static doublereal bignum;
    static integer infequ;
    static logical colequ;
    extern /* Subroutine */ int dgetrs_();
    static doublereal rowcnd;
    static logical notran;
    static doublereal smlnum;
    static logical rowequ;


/*  -- LAPACK driver routine (version 1.1) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     March 31, 1993 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGESVX uses the LU factorization to compute the solution to a real */
/*  system of linear equations */
/*     A * X = B, */
/*  where A is an N-by-N matrix and X and B are N-by-NRHS matrices. */

/*  Error bounds on the solution and a condition estimate are also */
/*  provided. */

/*  Description */
/*  =========== */

/*  The following steps are performed: */

/*  1. If FACT = 'E', real scaling factors are computed to equilibrate */
/*     the system: */
/*        TRANS = 'N':  diag(R)*A*diag(C)     *inv(diag(C))*X = diag(R)*B 
*/
/*        TRANS = 'T': (diag(R)*A*diag(C))**T *inv(diag(R))*X = diag(C)*B 
*/
/*        TRANS = 'C': (diag(R)*A*diag(C))**H *inv(diag(R))*X = diag(C)*B 
*/
/*     Whether or not the system will be equilibrated depends on the */
/*     scaling of the matrix A, but if equilibration is used, A is */
/*     overwritten by diag(R)*A*diag(C) and B by diag(R)*B (if TRANS='N') 
*/
/*     or diag(C)*B (if TRANS = 'T' or 'C'). */

/*  2. If FACT = 'N' or 'E', the LU decomposition is used to factor the */
/*     matrix A (after equilibration if FACT = 'E') as */
/*        A = P * L * U, */
/*     where P is a permutation matrix, L is a unit lower triangular */
/*     matrix, and U is upper triangular. */

/*  3. The factored form of A is used to estimate the condition number */
/*     of the matrix A.  If the reciprocal of the condition number is */
/*     less than machine precision, steps 4-6 are skipped. */

/*  4. The system of equations is solved for X using the factored form */
/*     of A. */

/*  5. Iterative refinement is applied to improve the computed solution */
/*     matrix and calculate error bounds and backward error estimates */
/*     for it. */

/*  6. If FACT = 'E' and equilibration was used, the matrix X is */
/*     premultiplied by diag(C) (if TRANS = 'N') or diag(R) (if */
/*     TRANS = 'T' or 'C') so that it solves the original system */
/*     before equilibration. */

/*  Arguments */
/*  ========= */

/*  FACT    (input) CHARACTER*1 */
/*          Specifies whether or not the factored form of the matrix A is 
*/
/*          supplied on entry, and if not, whether the matrix A should be 
*/
/*          equilibrated before it is factored. */
/*          = 'F':  On entry, AF and IPIV contain the factored form of A. 
*/
/*                  If EQUED is not 'N', the matrix A has been */
/*                  equilibrated with scaling factors given by R and C. */
/*                  A, AF, and IPIV are not modified. */
/*          = 'N':  The matrix A will be copied to AF and factored. */
/*          = 'E':  The matrix A will be equilibrated if necessary, then 
*/
/*                  copied to AF and factored. */

/*  TRANS   (input) CHARACTER*1 */
/*          Specifies the form of the system of equations: */
/*          = 'N':  A * X = B     (No transpose) */
/*          = 'T':  A**T * X = B  (Transpose) */
/*          = 'C':  A**H * X = B  (Transpose) */

/*  N       (input) INTEGER */
/*          The number of linear equations, i.e., the order of the */
/*          matrix A.  N >= 0. */

/*  NRHS    (input) INTEGER */
/*          The number of right-hand sides, i.e., the number of columns */
/*          of the matrices B and X.  NRHS >= 0. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA,N) */
/*          On entry, the N-by-N matrix A.  If FACT = 'F' and EQUED is */
/*          not 'N', then A must have been equilibrated by the scaling */
/*          factors in R and/or C.  A is not modified if FACT = 'F' or */
/*          'N', or if FACT = 'E' and EQUED = 'N' on exit. */

/*          On exit, if EQUED .ne. 'N', A is scaled as follows: */
/*          EQUED = 'R':  A := diag(R) * A */
/*          EQUED = 'C':  A := A * diag(C) */
/*          EQUED = 'B':  A := diag(R) * A * diag(C). */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,N). */

/*  AF      (input or output) DOUBLE PRECISION array, dimension (LDAF,N) 
*/
/*          If FACT = 'F', then AF is an input argument and on entry */
/*          contains the factors L and U from the factorization */
/*          A = P*L*U as computed by DGETRF.  If EQUED .ne. 'N', then */
/*          AF is the factored form of the equilibrated matrix A. */

/*          If FACT = 'N', then AF is an output argument and on exit */
/*          returns the factors L and U from the factorization A = P*L*U 
*/
/*          of the original matrix A. */

/*          If FACT = 'E', then AF is an output argument and on exit */
/*          returns the factors L and U from the factorization A = P*L*U 
*/
/*          of the equilibrated matrix A (see the description of A for */
/*          the form of the equilibrated matrix). */

/*  LDAF    (input) INTEGER */
/*          The leading dimension of the array AF.  LDAF >= max(1,N). */

/*  IPIV    (input or output) INTEGER array, dimension (N) */
/*          If FACT = 'F', then IPIV is an input argument and on entry */
/*          contains the pivot indices from the factorization A = P*L*U */
/*          as computed by DGETRF; row i of the matrix was interchanged */
/*          with row IPIV(i). */

/*          If FACT = 'N', then IPIV is an output argument and on exit */
/*          contains the pivot indices from the factorization A = P*L*U */
/*          of the original matrix A. */

/*          If FACT = 'E', then IPIV is an output argument and on exit */
/*          contains the pivot indices from the factorization A = P*L*U */
/*          of the equilibrated matrix A. */

/*  EQUED   (input/output) CHARACTER*1 */
/*          Specifies the form of equilibration that was done. */
/*          = 'N':  No equilibration (always true if FACT = 'N'). */
/*          = 'R':  Row equilibration, i.e., A has been premultiplied by 
*/
/*                  diag(R). */
/*          = 'C':  Column equilibration, i.e., A has been postmultiplied 
*/
/*                  by diag(C). */
/*          = 'B':  Both row and column equilibration, i.e., A has been */
/*                  replaced by diag(R) * A * diag(C). */
/*          EQUED is an input variable if FACT = 'F'; otherwise, it is an 
*/
/*          output variable. */

/*  R       (input/output) DOUBLE PRECISION array, dimension (N) */
/*          The row scale factors for A.  If EQUED = 'R' or 'B', A is */
/*          multiplied on the left by diag(R); if EQUED = 'N' or 'C', R */
/*          is not accessed.  R is an input variable if FACT = 'F'; */
/*          otherwise, R is an output variable.  If FACT = 'F' and */
/*          EQUED = 'R' or 'B', each element of R must be positive. */

/*  C       (input/output) DOUBLE PRECISION array, dimension (N) */
/*          The column scale factors for A.  If EQUED = 'C' or 'B', A is 
*/
/*          multiplied on the right by diag(C); if EQUED = 'N' or 'R', C 
*/
/*          is not accessed.  C is an input variable if FACT = 'F'; */
/*          otherwise, C is an output variable.  If FACT = 'F' and */
/*          EQUED = 'C' or 'B', each element of C must be positive. */

/*  B       (input/output) DOUBLE PRECISION array, dimension (LDB,NRHS) */
/*          On entry, the N-by-NRHS right-hand side matrix B. */
/*          On exit, if EQUED = 'N', B is not modified; if TRANS = 'N' */
/*          and EQUED = 'R' or 'B', B is overwritten by diag(R)*B; if */
/*          TRANS = 'T' or 'C' and EQUED = 'C' or 'B', B is overwritten */
/*          by diag(C)*B. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of the array B.  LDB >= max(1,N). */

/*  X       (output) DOUBLE PRECISION array, dimension (LDX,NRHS) */
/*          If INFO = 0, the N-by-NRHS solution matrix X to the original 
*/
/*          system of equations.  Note that A and B are modified on exit 
*/
/*          if EQUED .ne. 'N', and the solution to the equilibrated */
/*          system is inv(diag(C))*X if TRANS = 'N' and EQUED = 'C' or */
/*          'B', or inv(diag(R))*X if TRANS = 'T' or 'C' and EQUED = 'R' 
*/
/*          or 'B'. */

/*  LDX     (input) INTEGER */
/*          The leading dimension of the array X.  LDX >= max(1,N). */

/*  RCOND   (output) DOUBLE PRECISION */
/*          The estimate of the reciprocal condition number of the matrix 
*/
/*          A after equilibration (if done).  If RCOND is less than the */
/*          machine precision (in particular, if RCOND = 0), the matrix */
/*          is singular to working precision.  This condition is */
/*          indicated by a return code of INFO > 0, and the solution and 
*/
/*          error bounds are not computed. */

/*  FERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The estimated forward error bounds for each solution vector */
/*          X(j) (the j-th column of the solution matrix X). */
/*          If XTRUE is the true solution, FERR(j) bounds the magnitude */
/*          of the largest entry in (X(j) - XTRUE) divided by */
/*          the magnitude of the largest entry in X(j).  The quality of */
/*          the error bound depends on the quality of the estimate of */
/*          norm(inv(A)) computed in the code; if the estimate of */
/*          norm(inv(A)) is accurate, the error bound is guaranteed. */

/*  BERR    (output) DOUBLE PRECISION array, dimension (NRHS) */
/*          The componentwise relative backward error of each solution */
/*          vector X(j) (i.e., the smallest relative change in */
/*          any entry of A or B that makes X(j) an exact solution). */

/*  WORK    (workspace) DOUBLE PRECISION array, dimension (4*N) */

/*  IWORK   (workspace) INTEGER array, dimension (N) */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  if INFO = i, and i is */
/*                <= N:  U(i,i) is exactly zero.  The factorization has */
/*                       been completed, but the factor U is exactly */
/*                       singular, so the solution and error bounds */
/*                       could not be computed. */
/*                = N+1: RCOND is less than machine precision.  The */
/*                       factorization has been completed, but the */
/*                       matrix is singular to working precision, and */
/*                       the solution and error bounds have not been */
/*                       computed. */

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

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    af_dim1 = *ldaf;
    af_offset = af_dim1 + 1;
    af -= af_offset;
    --ipiv;
    --r;
    --c;
    b_dim1 = *ldb;
    b_offset = b_dim1 + 1;
    b -= b_offset;
    x_dim1 = *ldx;
    x_offset = x_dim1 + 1;
    x -= x_offset;
    --ferr;
    --berr;
    --work;
    --iwork;

    /* Function Body */
    *info = 0;
    nofact = lsame_(fact, "N", 1L, 1L);
    equil = lsame_(fact, "E", 1L, 1L);
    notran = lsame_(trans, "N", 1L, 1L);
    if (nofact || equil) {
	*(unsigned char *)equed = 'N';
	rowequ = FALSE_;
	colequ = FALSE_;
    } else {
	rowequ = lsame_(equed, "R", 1L, 1L) || lsame_(equed, "B", 1L, 1L);
	colequ = lsame_(equed, "C", 1L, 1L) || lsame_(equed, "B", 1L, 1L);
	smlnum = dlamch_("Safe minimum", 12L);
	bignum = 1. / smlnum;
    }

/*     Test the input parameters. */

    if (! nofact && ! equil && ! lsame_(fact, "F", 1L, 1L)) {
	*info = -1;
    } else if (! notran && ! lsame_(trans, "T", 1L, 1L) && ! lsame_(trans, 
	    "C", 1L, 1L)) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*nrhs < 0) {
	*info = -4;
    } else if (*lda < max(1,*n)) {
	*info = -6;
    } else if (*ldaf < max(1,*n)) {
	*info = -8;
    } else if (lsame_(fact, "F", 1L, 1L) && ! (rowequ || colequ || lsame_(
	    equed, "N", 1L, 1L))) {
	*info = -10;
    } else {
	if (rowequ) {
	    rcmin = bignum;
	    rcmax = 0.;
	    i__1 = *n;
	    for (j = 1; j <= i__1; ++j) {
/* Computing MIN */
		d__1 = rcmin, d__2 = r[j];
		rcmin = min(d__1,d__2);
/* Computing MAX */
		d__1 = rcmax, d__2 = r[j];
		rcmax = max(d__1,d__2);
/* L10: */
	    }
	    if (rcmin <= 0.) {
		*info = -11;
	    } else if (*n > 0) {
		rowcnd = max(rcmin,smlnum) / min(rcmax,bignum);
	    } else {
		rowcnd = 1.;
	    }
	}
	if (colequ && *info == 0) {
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
/* L20: */
	    }
	    if (rcmin <= 0.) {
		*info = -12;
	    } else if (*n > 0) {
		colcnd = max(rcmin,smlnum) / min(rcmax,bignum);
	    } else {
		colcnd = 1.;
	    }
	}
	if (*info == 0) {
	    if (*ldb < max(1,*n)) {
		*info = -14;
	    } else if (*ldx < max(1,*n)) {
		*info = -16;
	    }
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGESVX", &i__1, 6L);
	return 0;
    }

    if (equil) {

/*        Compute row and column scalings to equilibrate the matrix A.
 */

	dgeequ_(n, n, &a[a_offset], lda, &r[1], &c[1], &rowcnd, &colcnd, &
		amax, &infequ);
	if (infequ == 0) {

/*           Equilibrate the matrix. */

	    dlaqge_(n, n, &a[a_offset], lda, &r[1], &c[1], &rowcnd, &colcnd, &
		    amax, equed, 1L);
	    rowequ = lsame_(equed, "R", 1L, 1L) || lsame_(equed, "B", 1L, 1L);
	    colequ = lsame_(equed, "C", 1L, 1L) || lsame_(equed, "B", 1L, 1L);
	}
    }

/*     Scale the right-hand side. */

    if (notran) {
	if (rowequ) {
	    i__1 = *nrhs;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = *n;
		for (i = 1; i <= i__2; ++i) {
		    b[i + j * b_dim1] = r[i] * b[i + j * b_dim1];
/* L30: */
		}
/* L40: */
	    }
	}
    } else if (colequ) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
		b[i + j * b_dim1] = c[i] * b[i + j * b_dim1];
/* L50: */
	    }
/* L60: */
	}
    }

    if (nofact || equil) {

/*        Compute the LU factorization of A. */

	dlacpy_("Full", n, n, &a[a_offset], lda, &af[af_offset], ldaf, 4L);
	dgetrf_(n, n, &af[af_offset], ldaf, &ipiv[1], info);

/*        Return if INFO is non-zero. */

	if (*info != 0) {
	    if (*info > 0) {
		*rcond = 0.;
	    }
	    return 0;
	}
    }

/*     Compute the norm of the matrix A. */

    if (notran) {
	*(unsigned char *)norm = '1';
    } else {
	*(unsigned char *)norm = 'I';
    }
    anorm = dlange_(norm, n, n, &a[a_offset], lda, &work[1], 1L);

/*     Compute the reciprocal of the condition number of A. */

    dgecon_(norm, n, &af[af_offset], ldaf, &anorm, rcond, &work[1], &iwork[1],
	     info, 1L);

/*     Return if the matrix is singular to working precision. */

    if (*rcond < dlamch_("Epsilon", 7L)) {
	*info = *n + 1;
	return 0;
    }

/*     Compute the solution matrix X. */

    dlacpy_("Full", n, nrhs, &b[b_offset], ldb, &x[x_offset], ldx, 4L);
    dgetrs_(trans, n, nrhs, &af[af_offset], ldaf, &ipiv[1], &x[x_offset], ldx,
	     info, 1L);

/*     Use iterative refinement to improve the computed solution and */
/*     compute error bounds and backward error estimates for it. */

    dgerfs_(trans, n, nrhs, &a[a_offset], lda, &af[af_offset], ldaf, &ipiv[1],
	     &b[b_offset], ldb, &x[x_offset], ldx, &ferr[1], &berr[1], &work[
	    1], &iwork[1], info, 1L);

/*     Transform the solution matrix X to a solution of the original */
/*     system. */

    if (notran) {
	if (colequ) {
	    i__1 = *nrhs;
	    for (j = 1; j <= i__1; ++j) {
		i__2 = *n;
		for (i = 1; i <= i__2; ++i) {
		    x[i + j * x_dim1] = c[i] * x[i + j * x_dim1];
/* L70: */
		}
/* L80: */
	    }
	    i__1 = *nrhs;
	    for (j = 1; j <= i__1; ++j) {
		ferr[j] /= colcnd;
/* L90: */
	    }
	}
    } else if (rowequ) {
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *n;
	    for (i = 1; i <= i__2; ++i) {
		x[i + j * x_dim1] = r[i] * x[i + j * x_dim1];
/* L100: */
	    }
/* L110: */
	}
	i__1 = *nrhs;
	for (j = 1; j <= i__1; ++j) {
	    ferr[j] /= rowcnd;
/* L120: */
	}
    }

    return 0;

/*     End of DGESVX */

} /* dgesvx_ */

