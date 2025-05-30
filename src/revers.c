/*							revers.c
 *
 *	Reversion of power series
 *
 *
 *
 * SYNOPSIS:
 *
 * extern int MAXPOL;
 * int n;
 * double x[n+1], y[n+1];
 *
 * polini(n);
 * revers( y, x, n );
 *
 *  Note, polini() initializes the polynomial arithmetic subroutines;
 *  see polyn.c.
 *
 *
 * DESCRIPTION:
 *
 * If
 *
 *          inf
 *           -       i
 *  y(x)  =  >   a  x
 *           -    i
 *          i=1
 *
 * then
 *
 *          inf
 *           -       j
 *  x(y)  =  >   A  y    ,
 *           -    j
 *          j=1
 *
 * where
 *                   1
 *         A    =   ---
 *          1        a
 *                    1
 *
 * etc.  The coefficients of x(y) are found by expanding
 *
 *          inf      inf
 *           -        -      i
 *  x(y)  =  >   A    >  a  x
 *           -    j   -   i
 *          j=1      i=1
 *
 *  and setting each coefficient of x , higher than the first,
 *  to zero.
 *
 *
 *
 * RESTRICTIONS:
 *
 *  y[0] must be zero, and y[1] must be nonzero.
 *
 */

/*
Cephes Math Library Release 2.2:  July, 1992
Copyright 1984, 1987, 1989, 1992 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"
#include "cephes.h"

#include <stdlib.h>

extern int MAXPOL; /* initialized by polini() */

/* See polyn.c.  */
void polmov(), polclr(), poladd(), polmul();

void revers(double y[],double x[],int n)
{
double *yn, *yp, *ysum;
int j;

if( y[1] == 0.0 )
	mtherr( "revers", DOMAIN );
/*	printf( "revers: y[1] = 0\n" );*/
j = (MAXPOL + 1) * sizeof(double);
yn = (double *)malloc(j);
yp = (double *)malloc(j);
ysum = (double *)malloc(j);

polmov( y, n, yn );
polclr( ysum, n );
x[0] = 0.0;
x[1] = 1.0/y[1];
for( j=2; j<=n; j++ )
	{
/* A_(j-1) times the expansion of y^(j-1)  */
	polmul( &x[j-1], 0, yn, n, yp );
/* The expansion of the sum of A_k y^k up to k=j-1 */
	poladd( yp, n, ysum, n, ysum );
/* The expansion of y^j */
	polmul( yn, n, y, n, yn );
/* The coefficient A_j to make the sum up to k=j equal to zero */
	x[j] = -ysum[j]/yn[j];
	}
free(yn);
free(yp);
free(ysum);
}

