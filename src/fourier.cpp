/*
 * Grace - GRaphing, Advanced Computation and Exploration of data
 * 
 * Home page: http://plasma-gate.weizmann.ac.il/Grace/
 * 
 * Copyright (c) 1991-1995 Paul J Turner, Portland, OR
 * Copyright (c) 1996-2000 Grace Development Team
 * 
 * Maintained by Evgeny Stambulchik
 * 
 * Modified by Andreas Winter 2008-2022
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

/*
 *
 * DFT by definition and FFT
 */

/* This module completely rewritten February 1998 by
Marcus H. Mendenhall,
Vanderbilt University Physics Department
Nashville, Tennessee, USA
email: marcus.h.mendenhall@vanderbilt.edu

The conventional DFT and FFT are reimplemented to avoid massive gratuitous
recalculation of trig functions.  Also, the signs and magnitudes are
rationalized so that a forward DFT and a forward FFT give the same result.

Also, if the variable HAVE_FFTW is defined (probably from ./configure),
it uses the FFTW libraries to do all transforms, making the DFT and FFT
entirely equivalent.  See the FFTW home site 
http://theory.lcs.mit.edu/~fftw/
for copyright and usage information and documentation on the FFTW libraries.
I strongly recommend using them... they work very well.
*/

///#include <config.h>
#include <cmath>

#define _USE_MATH_DEFINES
#include "math.h"

#include <cstdio>
#include <cstdlib>
#include "globals.h"
#include "defines.h"
#include "utils.h"
#include "noxprotos.h"
#include "external_libs.h"

///#ifndef  HAVE_FFTW

static int bit_swap(int i, int nu);
void dft_fftw3(double *jr, double *ji, int n, int iflag);

/*
	DFT by definition
*/
void dft(double *jr, double *ji, int n, int iflag)
{
    if (use_fftw3==TRUE && have_fftw3==TRUE)
    {
    dft_fftw3(jr,ji,n,iflag);
    return;
    }

    int i, j, sgn;
    double sumr, sumi, tpi, w, *xr, *xi, on = 1.0 / n;
    double *cov, *siv, co, si;
    int iwrap;

    sgn = iflag ? -1 : 1;
    tpi = 2.0 * M_PI;
    xr = (double *)xcalloc(n, sizeof(double));
    xi = (double *)xcalloc(n, sizeof(double));
    cov = (double *)xcalloc(n, sizeof(double));
    siv = (double *)xcalloc(n, sizeof(double));
    if (xr == NULL || xi == NULL || cov == NULL || siv == NULL) {
	XCFREE(xr);
	XCFREE(xi);
	XCFREE(cov);
	XCFREE(siv);
	return;
    }
    for (i = 0; i < n; i++) {
	w = tpi * i * on;
	cov[i] = cos(w);
	siv[i] = sin(w)*sgn;
	xr[i] = jr[i];
	xi[i] = ji[i];
    }
    for (j = 0; j < n; j++) {
	sumr = 0.0;
	sumi = 0.0;
	for (i = 0, iwrap=0; i < n; i++, iwrap += j) {
	    if(iwrap >= n) iwrap -= n;
	    co = cov[iwrap];
	    si = siv[iwrap];
	    sumr = sumr + xr[i] * co + sgn * xi[i] * si;
	    sumi = sumi + xi[i] * co - sgn * xr[i] * si;
	}
	jr[j] = sumr;
	ji[j] = sumi;
    }
    if (sgn == 1) {
	on = 1.0 * on;
    } else {
	on = 1.0;
    }
    for (i = 0; i < n; i++) {
	jr[i] = jr[i] * on;
	ji[i] = ji[i] * on;
    }
    XCFREE(xr);
    XCFREE(xi);
    XCFREE(cov);
    XCFREE(siv);
}


/*
   real_data ... ptr. to real part of data to be transformed
   imag_data ... ptr. to imag  "   "   "   "  "      "
   inv ..... Switch to flag normal or inverse transform
   n_pts ... Number of real data points
   nu ...... logarithm in base 2 of n_pts e.g. nu = 5 if n_pts = 32.
*/

void fft(double *real_data, double *imag_data, int n_pts, int nu, int inv)
{
    if (use_fftw3==TRUE && have_fftw3==TRUE)
    {
    dft_fftw3(real_data,imag_data,n_pts,inv);//there is only one function in fftw3 (dft==fft)
    return;
    }

    int n2, i, ib ,mm, k;
    int sgn, tstep;
    double tr, ti, arg;	/* intermediate values in calcs. */
    double c, s;		/* cosine & sine components of Fourier trans. */
    static double *sintab = NULL;
    static int last_n = 0;

    n2 = n_pts / 2;
    
    if(n_pts==0) {
      if(sintab) XCFREE(sintab);
      sintab=NULL;
      last_n=0;
      return; /* just deallocate memory if called with zero points */
    } else if (n_pts != last_n) { /* allocate new sin table */
      arg=2*M_PI/n_pts;
      last_n=0;
      if(sintab) XCFREE(sintab);
      sintab=(double *) xcalloc(n_pts,sizeof(double));
      if(sintab == NULL) return; /* out of memory! */
      for(i=0; i<n_pts; i++) sintab[i] = sin(arg*i);
      last_n=n_pts;
    }

 /*
* sign change for inverse transform
*/
    sgn = inv ? -1 : 1;

    /* do bit reversal of data in advance */
    for (k = 0; k != n_pts; k++) {
	ib = bit_swap(k, nu);
	if (ib > k) {
	    fswap((real_data + k), (real_data + ib));
	    fswap((imag_data + k), (imag_data + ib));
	}
    }
/*
* Calculate the componets of the Fourier series of the function
*/
    tstep=n2;
    for (mm = 1; mm < n_pts; mm*=2) {
      int sinidx=0, cosidx=n_pts/4;
      for(k=0; k<mm; k++) {
	c = sintab[cosidx];
	s = sgn * sintab[sinidx];
	sinidx += tstep; cosidx += tstep;
	if(sinidx >= n_pts) sinidx -= n_pts;
	if(cosidx >= n_pts) cosidx -= n_pts;
	for (i = k; i < n_pts; i+=mm*2) {
	  double re1, re2, im1, im2;  
	  re1=real_data[i]; re2=real_data[i+mm];
	  im1=imag_data[i]; im2=imag_data[i+mm];
	  
	  tr = re2 * c + im2 * s;
	  ti = im2 * c - re2 * s;
	  real_data[i+mm] = re1 - tr;
	  imag_data[i+mm] = im1 - ti;
	  real_data[i] = re1 + tr;
	  imag_data[i] = im1 + ti;
	}
      }
      tstep /= 2;
    }
/*
* If calculating the inverse transform, must divide the data by the number of
* data points.
*/
    if (inv) {
        double fac = 1.0 / n_pts;
        for (k = 0; k != n_pts; k++) {
	    *(real_data + k) *= fac;
	    *(imag_data + k) *= fac;
        }
    }
}

/*
* Bit swapping routine in which the bit pattern of the integer i is reordered.
* See Brigham's book for details
*/
static int bit_swap(int i, int nu)
{
    int ib, i1, i2;

    ib = 0;

    for (i1 = 0; i1 != nu; i1++) {
	i2 = i / 2;
	ib = ib * 2 + i - 2 * i2;
	i = i2;
    }
    return (ib);
}

//#else

/* Start of new FFTW-based transforms by Marcus H. Mendenhall */
/* We use fftw3 here*/
//#include <fftw.h>
#include "fftw3.h"
#include <string.h>

extern Prototype_fftw_export_wisdom_to_string fftw_dll_export_wisdom_to_string;
extern Prototype_fftw_export_wisdom_to_file fftw_dll_export_wisdom_to_file;
extern Prototype_fftw_import_wisdom_from_file fftw_dll_import_wisdom_from_file;
extern Prototype_fftw_destroy_plan fftw_dll_destroy_plan;
extern Prototype_fftw_free fftw_dll_free;
extern Prototype_fftw_malloc fftw_dll_malloc;
extern Prototype_fftw_execute fftw_dll_execute;
extern Prototype_fftw_plan_dft_1d fftw_dll_plan_dft_1d;

static char  *wisdom_file=0;
static char *initial_wisdom=0;
static int using_wisdom=0;

static void save_wisdom(void){
  FILE *wf;
  char *final_wisdom;
  final_wisdom=fftw_dll_export_wisdom_to_string();
  if (!initial_wisdom || strcmp(initial_wisdom, final_wisdom)) {
    wf=fopen(wisdom_file,"w");
    if(wf) {
      fftw_dll_export_wisdom_to_file(wf);
      fclose(wf);
    }
  } 
  fftw_dll_free(final_wisdom);
  if (initial_wisdom) fftw_dll_free(initial_wisdom);
}

void dft_fftw3(double *jr, double *ji, int n, int iflag)
{
  fftw_plan plan;
  int i;
  double ninv;
  fftw_complex *cbuf;
  static int wisdom_inited=0;
  char *ram_cache_wisdom;
  int plan_flags;

  if(!wisdom_inited)  {
    wisdom_inited=1;
    wisdom_file=getenv("GRACE_FFTW_WISDOM_FILE");
    ram_cache_wisdom=getenv("GRACE_FFTW_RAM_WISDOM");

    if(ram_cache_wisdom) sscanf(ram_cache_wisdom, "%d", &using_wisdom);
    /* turn on wisdom if it is requested even without persistent storage */

    if(wisdom_file && wisdom_file[0] ) {
      /* if a file was specified in GRACE_FFTW_WISDOM_FILE, try to read it */
      FILE *wf;
      //int fstat;
      wf=fopen(wisdom_file,"r");
      if(wf) {
    //fstat=fftw_dll_import_wisdom_from_file(wf);
    (void)fftw_dll_import_wisdom_from_file(wf);
	fclose(wf);
    initial_wisdom=fftw_dll_export_wisdom_to_string();
      } else initial_wisdom=0;
      atexit(save_wisdom);
      using_wisdom=1; /* if a file is specified, always use wisdom */
    }
  }

  plan_flags=using_wisdom ? FFTW_MEASURE : FFTW_ESTIMATE;

  //plan=fftw_create_plan(n, iflag?FFTW_BACKWARD:FFTW_FORWARD, plan_flags | FFTW_IN_PLACE);
  //cbuf=xcalloc(n, sizeof(*cbuf));
  cbuf = static_cast<fftw_complex*>( fftw_dll_malloc(sizeof(fftw_complex) * n) );
  if(!cbuf) return;
  for(i=0; i<n; i++) {
    cbuf[i][0]=jr[i]; cbuf[i][1]=ji[i];
  }
  plan=fftw_dll_plan_dft_1d(n,cbuf,cbuf, iflag?FFTW_BACKWARD:FFTW_FORWARD, plan_flags);
  //fftw(plan, 1, cbuf, 1, 1, 0, 1, 1);
  fftw_dll_execute(plan);
  fftw_dll_destroy_plan(plan);
  if(!iflag) {
    ninv=1.0/n;
    for(i=0; i<n; i++) {
    jr[i]=cbuf[i][0]*ninv; ji[i]=cbuf[i][1]*ninv;
    }
  } else {
    for(i=0; i<n; i++) {
      jr[i]=cbuf[i][0]; ji[i]=cbuf[i][1];
    }
  }
  fftw_dll_free(cbuf);
}

void fft_fftw3(double *real_data, double *imag_data, int n_pts, int nu, int inv)
{
    (void)nu;
    /* let FFTW handle DFT's and FFT's identically */
    dft_fftw3(real_data, imag_data, n_pts, inv);
}

//#endif
