/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2001 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * 
 *                           All Rights Reserved
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Wrappers for some math functions */

#ifndef _MATHSTUFF_H__
#define _MATHSTUFF_H__

#ifdef __cplusplus
extern "C" {
#endif

  double ai_wrap(double x);
  double bi_wrap(double x);
  double chi_wrap(double x);
  double ci_wrap(double x);
  double ellpe_wrap ( double x );
  double ellpk_wrap ( double x );
  double fresnlc_wrap(double x);
  double fresnls_wrap(double x);
  double fx(double x);
  double iv_wrap(double v, double x);
  double jv_wrap(double v, double x);
  double kn_wrap(int n, double x);
  double max_wrap(double x, double y);
  double min_wrap(double x, double y);
  double irand_wrap(int x);
  double rnorm(double mean, double sdev);
  double shi_wrap(double x);
  double si_wrap(double x);
  double sqr_wrap(double x);
  double yv_wrap(double v, double x);
  double sign_wrap(double x);
  double ceil_wrap(double x);
  double floor_wrap(double x);

double voigt(double gamma, double sigma, double x);

/* constants */
  double pi_const(void);
  double deg_uconst(void);
  double rad_uconst(void);

/* the following functions are modified comparisons including a tolerance-value (the tolerance here is the representable range near b, i.e. the range between the next lower and higher double-value near b) */
/* these functions behave like: if a is inside [b-tolerance,b+tolerance] then a is concidered as a==b */
/* COMPARISON_TOLERANCE gives the amount of steps to be taken lower and higher than b */
  int tolerant_lt(double a,double b); /*1 if a<next_lower_value_than(b)*/
  int tolerant_gt(double a,double b); /*1 if a>next_higher_value_than(b)*/
  int tolerant_leq(double a,double b);/*1 if a<next_higher_value_than(b)*/
  int tolerant_geq(double a,double b);/*1 if a>next_lower_value_than(b)*/
  int tolerant_eq(double a,double b); /*1 if a>next_lower_value_than(b) && a<next_higher_value_than(b)*/
double low_tol_val(double a);
double high_tol_val(double a);

#ifdef __cplusplus
}
#endif

#endif /* _MATHSTUFF_H__ */
