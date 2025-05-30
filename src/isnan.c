/*							isnan()
 *							signbit()
 *							isfinite()
 *
 *	Floating point numeric utilities
 *
 *
 *
 * SYNOPSIS:
 *
 * double ceil(), floor(), frexp(), ldexp();
 * int signbit(), isnan(), isfinite();
 * double x, y;
 * int expnt, n;
 *
 * y = floor(x);
 * y = ceil(x);
 * y = frexp( x, &expnt );
 * y = ldexp( x, n );
 * n = signbit(x);
 * n = isnan(x);
 * n = isfinite(x);
 *
 *
 *
 * DESCRIPTION:
 *
 * All four routines return a double precision floating point
 * result.
 *
 * floor() returns the largest integer less than or equal to x.
 * It truncates toward minus infinity.
 *
 * ceil() returns the smallest integer greater than or equal
 * to x.  It truncates toward plus infinity.
 *
 * frexp() extracts the exponent from x.  It returns an integer
 * power of two to expnt and the significand between 0.5 and 1
 * to y.  Thus  x = y * 2**expn.
 *
 * ldexp() multiplies x by 2**n.
 *
 * signbit(x) returns 1 if the sign bit of x is 1, else 0.
 *
 * These functions are part of the standard C run time library
 * for many but not all C compilers.  The ones supplied are
 * written in C for either DEC or IEEE arithmetic.  They should
 * be used only if your compiler library does not already have
 * them.
 *
 * The IEEE versions assume that denormal numbers are implemented
 * in the arithmetic.  Some modifications will be required if
 * the arithmetic has abrupt rather than gradual underflow.
 */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#include "cephes.h"
#if 0 //defined(MAC_SYSTEM)
#if !defined(HAVE_ISNAN) && !defined(HAVE_ISNAN_MACRO)

#ifdef UNK
/* ceil(), floor(), frexp(), ldexp() may need to be rewritten. */
#undef UNK
#if BIGENDIAN
#define MIEEE 1
#else
#define IBMPC 1
#endif
#endif


/* Return 1 if the sign bit of x is 1, else 0.  */

int signbit(double x)
{
union
	{
	double d;
	short s[4];
	int i[2];
	} u;

u.d = x;

if( sizeof(int) == 4 )
	{
#ifdef IBMPC
	return( u.i[1] < 0 );
#endif
#ifdef DEC
	return( u.s[3] < 0 );
#endif
#ifdef MIEEE
	return( u.i[0] < 0 );
#endif
	}
else
	{
#ifdef IBMPC
	return( u.s[3] < 0 );
#endif
#ifdef DEC
	return( u.s[3] < 0 );
#endif
#ifdef MIEEE
	return( u.s[0] < 0 );
#endif
	}
}


/* Return 1 if x is a number that is Not a Number, else return 0.  */

int isnan(double x)
{
#ifdef NANS
union
	{
	double d;
	unsigned short s[4];
	unsigned int i[2];
	} u;

u.d = x;

if( sizeof(int) == 4 )
	{
#ifdef IBMPC
	if( ((u.i[1] & 0x7ff00000) == 0x7ff00000)
	    && (((u.i[1] & 0x000fffff) != 0) || (u.i[0] != 0)))
		return 1;
#endif
#ifdef DEC
	if( (u.s[1] & 0x7fff) == 0)
		{
		if( (u.s[2] | u.s[1] | u.s[0]) != 0 )
			return(1);
		}
#endif
#ifdef MIEEE
	if( ((u.i[0] & 0x7ff00000) == 0x7ff00000)
	    && (((u.i[0] & 0x000fffff) != 0) || (u.i[1] != 0)))
		return 1;
#endif
	return(0);
	}
else
	{ /* size int not 4 */
#ifdef IBMPC
	if( (u.s[3] & 0x7ff0) == 0x7ff0)
		{
		if( ((u.s[3] & 0x000f) | u.s[2] | u.s[1] | u.s[0]) != 0 )
			return(1);
		}
#endif
#ifdef DEC
	if( (u.s[3] & 0x7fff) == 0)
		{
		if( (u.s[2] | u.s[1] | u.s[0]) != 0 )
			return(1);
		}
#endif
#ifdef MIEEE
	if( (u.s[0] & 0x7ff0) == 0x7ff0)
		{
		if( ((u.s[0] & 0x000f) | u.s[1] | u.s[2] | u.s[3]) != 0 )
			return(1);
		}
#endif
	return(0);
	} /* size int not 4 */

#else
/* No NANS.  */
return(0);
#endif
}

#endif /* HAVE_ISNAN */

#if defined(HAVE_FINITE)

#endif

#if defined(HAVE_ISFINITE)

#endif

#if defined(HAVE_ISFINITE_MACRO)

#endif

#if !defined(HAVE_FINITE) && !defined(HAVE_ISFINITE) && !defined(HAVE_ISFINITE_MACRO)

/* Return 1 if x is not infinite and is not a NaN.  */

int isfinite(double x)
{
#ifdef INFINITIES
union
	{
	double d;
	unsigned short s[4];
	unsigned int i[2];
	} u;

u.d = x;

if( sizeof(int) == 4 )
	{
#ifdef IBMPC
	if( (u.i[1] & 0x7ff00000) != 0x7ff00000)
		return 1;
#endif
#ifdef DEC
	if( (u.i[0] & 0x7fffffff) != 0x7fffffff)
		return 1;
#endif
#ifdef MIEEE
	if( (u.i[0] & 0x7ff00000) != 0x7ff00000)
		return 1;
#endif
	return(0);
	}
else
	{
#ifdef IBMPC
	if( (u.s[3] & 0x7ff0) != 0x7ff0)
		return 1;
#endif
#ifdef DEC
	if( (u.s[3] & 0x7fff) != 0)
		return 1;
#endif
#ifdef MIEEE
	if( (u.s[0] & 0x7ff0) != 0x7ff0)
		return 1;
#endif
	return(0);
	}
#else
/* No INFINITY.  */
return(1);
#endif
}

#endif /* HAVE_(IS)FINITE */
#endif
