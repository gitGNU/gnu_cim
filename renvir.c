/* $Id: renvir.c,v 1.17 1997/01/08 09:49:22 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
 * Department of Informatics, University of Oslo.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* Run time rutiner for deler av environment rutinene */

#include <stdio.h>
#include <math.h>

#include "cim.h"
#include "config.h"

#if STDC_HEADERS
#include <stdlib.h>
#include <float.h>
#endif

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

/******************************************************************************
                                             INTEGER PROCEDURE MOD(i,j)      */

#if nolib|rmodrut
long 
__rmod (i, j)
     long i,
       j;
{
  long res;
  if (j == 0)
    __rerror ("Mod: Second parameter is equal zero");
  res = i % j;
  return ((res == 0) ? 0 : ((res > 0) != (j > 0)) ? res + j : res);
}
#endif

/******************************************************************************
                                             INTEGER PROCEDURE REM(i,j)      */

#if nolib|rremrut
long 
__rrem (i, j)
     long i,
       j;
{
  if (j == 0)
    __rerror ("Rem: Second parameter is equal zero");
  return (i % j);
}
#endif

/******************************************************************************
                                              INTEGER PROCEDURE SIGN(r)      */

#if nolib|rsignrrut
#if FLOAT_IMPLEMENTED
long 
__rsignr (e)
     double e;
{
  return ((e > 0.0) ? 1 : (e < 0.0) ? -1 : 0);
}
#endif
#endif
#if nolib|rsigndrrut
#if FLOAT_IMPLEMENTED
long 
__rsigndr (e,f)
     double e,f;
{
  return ((e > f) ? 1 : (e < f) ? -1 : 0);
}
#endif
#endif
#if nolib|rsignirut
long 
__rsigni (i)
     long i;
{
  return ((i > 0) ? 1 : (i < 0) ? -1 : 0);
}
#endif
#if nolib|rsigndirut
long 
__rsigndi (i,j)
     long i,j;
{
  return ((i > j) ? 1 : (i < j) ? -1 : 0);
}
#endif
#if nolib|rsigndxrut
long 
__rsigndx (i,j)
     long i;double j;
{
  return (((double)i > j) ? 1 : ((double)i < j) ? -1 : 0);
}
#endif

/******************************************************************************
                                            INTEGER PROCEDURE ENTIER(r)      */

#if nolib|rentierrut
#if FLOAT_IMPLEMENTED
long 
__rentier (r)
     double r;
{
  long j;
  if ((r >= ((double) MAX_INT) + 1.0) ||
      (r < -((double) MAX_INT) + 1.0))
     __rerror ("Entier: Argument out of integer bounds");
  j = r;
  return ((j > r) ? j - 1 : j);
}
#endif
#endif

/******************************************************************************
                                            INTEGER PROCEDURE INTREA(r)      */

#if nolib|rintrearut
#if FLOAT_IMPLEMENTED
long 
__rintrea (r)
     double r;
{
  long j;
  r += 0.5;
  j = r;
  return ((j > r) ? j - 1 : j);
}
#endif
#endif

/******************************************************************************
                                             INTEGER PROCEDURE POWII(i,j)    */

#if nolib|rpowiirut
#if 1
long 
__rpowii (i, j)
     long i,
       j;
{
  long k = 1,
    result;
  if (j < 0 | i == 0 & j == 0)
    __rerror ("Power: Illegal parameters");
  if (j == 0)
    return (1);
  while (__TRUE)
    {
      if (k & j)
	{
	  result = i;
	  if ((j ^= k) == 0)
	    return (result);
	  k <<= 1;
	  i *= i;
	  break;
	}
      k <<= 1;
      i *= i;
    }
  while (__TRUE)
    {
      if (k & j)
	{
	  result *= i;
	  if ((j ^= k) == 0)
	    return (result);
	}
      k <<= 1;
      i *= i;
    }
}
#else
long 
__rpowii (i, j)
     long i,
       j;
{
  long k,
    result;
  if (j < 0 | i == 0 & j == 0)
    __rerror ("Power: Illegal parameters");
  result = 1;
  for (k = 1; k <= j; k++)
    result *= i;
  return (result);
}
#endif
#endif

/******************************************************************************
                                                REAL PROCEDURE POWRI(r,i)    */

#if nolib|rpowrirut
#if FLOAT_IMPLEMENTED
#if 1
double 
__rpowri (r, i)
     double r;
     long i;
{
  long k = 1,
    s;
  double result;
  if (i == 0 & r == 0.0)
    __rerror ("Power: Illegal parameters");
  if (i < 0)
    {
      i = -i;
      s = 1;
    }
  else
    s = 0;
  if (i == 0)
    return (1.0);
  while (__TRUE)
    {
      if (k & i)
	{
	  result = r;
	  if ((i ^= k) == 0)
	    return (s == 1 ? 1.0 / result : result);
	  k <<= 1;
	  r *= r;
	  break;
	}
      k <<= 1;
      r *= r;
    }
  while (__TRUE)
    {
      if (k & i)
	{
	  result *= r;
	  if ((i ^= k) == 0)
	    return (s == 1 ? 1.0 / result : result);
	}
      k <<= 1;
      r *= r;
    }
}
#else
double 
__rpowri (r, i)
     double r;
     long i;
{
  long n;
  double result;
  if (i == 0 & r == 0.0)
    __rerror ("Power: Illegal parameters");
  result = 1.0;
  for (n = (i > 0 ? i : -i); n >= 1; n--)
    result *= r;
  return (i < 0 ? 1.0 / result : result);
}
#endif
#endif
#endif

/******************************************************************************
                                                  REAL PROCEDURE POW(x,r)    */

#if nolib|rpowrut
#if FLOAT_IMPLEMENTED
double 
__rpow (x, r)
     double x,
       r;
{
  if (x < 0.0 | (x == 0.0 & r <= 0.0))
    __rerror ("Power: Illegal parameters");
#if MATHLIB
  return (x > 0 ? exp (r * log (x)) : 0.0);
#else
  return (x > 0 ? __rexp (r * __rln (x)) : 0.0);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ADDEPSILON(r)    */

/* Denne rutinen skal h}ndtere alle floating-point formater.
 * F|rste rutine h}ndterer IEEE-, IBM- og VAX-format effektivt.
 * For disse formatene anntar den 32/64-bits representasjon.
 * IBM-formatet er ikke testet da vi mangler maskin } kj|re det p}
 * Andre rutine er en treg men portabel variant. */

#if nolib|raddepsilonrut
#if FLOAT_IMPLEMENTED
#if FLOAT_IEEE | FLOAT_VAX | FLOAT_IBM
#define reswd (1L<<31)
#define signbit (1L<<31)
#if FLOAT_IBM
#define emask (127<<24)
#define testmask (15<<20)
#define adjustexp (1<<20)
#endif
#if REALASDOUBLE
#if FLOAT_IEEE
#define emask (2047L<<20)
#endif
double 
__raddepsilon (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN
	  TYPE_32_INT i2;
	  unsigned TYPE_32_INT i1;
#else
	  unsigned TYPE_32_INT i1;
	  TYPE_32_INT i2;
#endif
	}
      dasii;
#if WORDS_VAXENDIAN
      struct
	{
	  short s1;
	  short s2;
	  short s3;
	  short s4;
	}
      dasssss;
#endif
    }
  value;
#if WORDS_VAXENDIAN
  short s;
#endif
  if (r == MAX_DOUBLE)
    return (r);
  value.d = r;
#if WORDS_VAXENDIAN
  s = value.dasssss.s1;
  value.dasssss.s1 = value.dasssss.s2;
  value.dasssss.s2 = s;
  s = value.dasssss.s3;
  value.dasssss.s3 = value.dasssss.s4;
  value.dasssss.s4 = s;
#endif
#if FLOAT_VAX
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.dasii.i2 & emask) == emask)
    return (r);
#endif
#endif
  if (r == -MIN_DOUBLE)
    return (0.0);
  if (r == 0.0)
    return (MIN_DOUBLE);
  if (value.dasii.i2 & signbit)
    {
      if ((value.dasii.i1--) == 0)
	value.dasii.i2--;
    }
  else if ((++value.dasii.i1) == 0)
    value.dasii.i2++;
#if FLOAT_IBM
  if (value.dasii.i2 & emask && value.dasii.i2 & tmask)
    if (value.dasii.i2 & signbit)
      value.dasii.i2 -= adexp;
    else
      value.dasii.i2 += adexp;
#endif
#if WORDS_VAXENDIAN
  s = value.dasssss.s1;
  value.dasssss.s1 = value.dasssss.s2;
  value.dasssss.s2 = s;
  s = value.dasssss.s3;
  value.dasssss.s3 = value.dasssss.s4;
  value.dasssss.s4 = s;
#endif
  return (value.d);
}
#else
#if FLOAT_IEEE
#define emask (255<<23)
#endif
float 
__raddepsilon (r)
     float r;
{
  union
    {
      float d;
      long i;
    }
  value;
  if (r == MAX_FLOAT)
    return (r);
  value.d = r;
#if FLOAT_VAX
  if (value.i == reswd)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.i & emask) == emask)
    return (r);
#endif
#endif
  if (r == -MIN_FLOAT)
    return (-0.0);
  if (r == 0.0)
    return (MIN_FLOAT);
  if (value.i & signbit)
    value.i--;
  else
    value.i++;
#if FLOAT_IBM
  if (value.i & emask && value.i & tmask)
    if (value.i & signbit)
      value.i -= adexp;
    else
      value.i += adexp;
#endif
  return (value.d);
}
#endif
#else
double 
__raddepsilon (r)
     double r;
{
#ifdef REALASDOUBLE
  double s = MIN_DOUBLE,
    t,
    u;
  u = r;
  if (u >= MAX_DOUBLE)
    return (r);
#else
  float s = MIN_FLOAT,
    t,
    u;
  u = r;
  if (u >= MAX_FLOAT)
    return (r);
#endif
  t = u + s;
  while (t == u)
    {
      s *= 2.0;
      t = u + s;
    }
  return (r + (t - u));
}
#endif
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE SUBEPSILON(r)    */

#if nolib|rsubepsilonrut
#if FLOAT_IMPLEMENTED
#if FLOAT_IEEE | FLOAT_VAX | FLOAT_IBM
#define reswd (1L<<31)
#define signbit (1L<<31)
#if FLOAT_IBM
#define emask (127<<24)
#define tmask (15<<20)
#define adexp (1<<20)
#endif
#if REALASDOUBLE
#if FLOAT_IEEE
#define emask (2047L<<20)
#endif
double 
__rsubepsilon (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN
	  TYPE_32_INT i2;
	  unsigned TYPE_32_INT i1;
#else
	  unsigned TYPE_32_INT i1;
	  TYPE_32_INT i2;
#endif
	}
      dasii;
#if WORDS_VAXENDIAN
      struct
	{
	  short s1;
	  short s2;
	  short s3;
	  short s4;
	}
      dasssss;
#endif
    }
  value;
#if WORDS_VAXENDIAN
  short s;
#endif
  if (r == -MAX_DOUBLE)
    return (r);
  value.d = r;
#if WORDS_VAXENDIAN
  s = value.dasssss.s1;
  value.dasssss.s1 = value.dasssss.s2;
  value.dasssss.s2 = s;
  s = value.dasssss.s3;
  value.dasssss.s3 = value.dasssss.s4;
  value.dasssss.s4 = s;
#endif
#if FLOAT_VAX
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.dasii.i2 & emask) == emask)
    return (r);
#endif
#endif
  if (r == 0.0)
    return (-MIN_DOUBLE);
  if (r == MIN_DOUBLE)
    return (0.0);
  if (value.dasii.i2 & signbit)
    {
      if ((++value.dasii.i1) == 0)
	value.dasii.i2++;
    }
  else if ((value.dasii.i1--) == 0)
    value.dasii.i2--;
#if FLOAT_IBM
  if (value.dasii.i2 & emask && value.dasii.i2 & tmask)
    if (value.dasii.i2 & signbit)
      value.dasii.i2 += adexp;
    else
      value.dasii.i2 -= adexp;
#endif
#if WORDS_VAXENDIAN
  s = value.dasssss.s1;
  value.dasssss.s1 = value.dasssss.s2;
  value.dasssss.s2 = s;
  s = value.dasssss.s3;
  value.dasssss.s3 = value.dasssss.s4;
  value.dasssss.s4 = s;
#endif
  return (value.d);
}
#else
#if FLOAT_IEEE
#define emask (255<<23)
#endif
float 
__rsubepsilon (r)
     float r;
{
  union
    {
      float d;
      long i;
    }
  value;
  if (r == -MAX_FLOAT)
    return (r);
  value.d = r;
#if FLOAT_VAX
  if (value.i == reswd)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.i & emask) == emask)
    return (r);
#endif
#endif
  if (r == 0.0)
    return (-MIN_FLOAT);
  if (r == MIN_FLOAT)
    return (0.0);
  if (value.i & signbit)
    value.i++;
  else
    value.i--;
#if FLOAT_IBM
  if (value.i & emask && value.i & tmask)
    if (value.i & signbit)
      value.i += adexp;
    else
      value.i -= adexp;
#endif
  return (value.d);
}
#endif
#else
double 
__rsubepsilon (r)
     double r;
{
#ifdef REALASDOUBLE
  double s = MIN_DOUBLE,
    t,
    u;
  u = r;
  if (u <= -MAX_DOUBLE)
    return (r);
#else
  float s = MIN_FLOAT,
    t,
    u;
  u = r;
  if (u <= -MAX_FLOAT)
    return (r);
#endif
  t = u - s;
  while (t == u)
    {
      s *= 2.0;
      t = u - s;
    }
  return (r - (u - t));
}
#endif
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ABS(r)           */

#if nolib|rabsrrut
#if FLOAT_IMPLEMENTED
double 
__rabsr (r)
     double r;
{
  if (r < 0.0)
    return (-r);
  return (r);
}
#endif
#endif

/******************************************************************************
                                             INTEGER PROCEDURE ABS(r)        */

#if nolib|rabsirut
long 
__rabsi (i)
     long i;
{
  if (i < 0)
    return (-i);
  return (i);
}
#endif

/******************************************************************************
                                             REAL PROCEDURE SQRT(r)          */

#if nolib|rsqrtrut
#if FLOAT_IMPLEMENTED
#if MATHLIB
double 
__rsqrt (r)
     double r;
{
  if (r < 0.0)
    __rerror ("Sqrt: Illegal negative argument");
  return (sqrt (r));
}
#else
#if FLOAT_IEEE

/* Algoritm from Numerical Mathematics and computing
 * - Cheney Kincaid page 53 #11 (fast algoritm)      */

#define emask (2047<<20)
double 
__rsqrt (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN
	  long i2;
	  unsigned long i1;
#else
	  unsigned long i1;
	  long i2;
#endif
	}
      dasii;
    }
  rr, x3;
  double x0,
    x1,
    x2;
  long i,
    j,
    k;

  if (r < 0.0)
    __rerror ("Sqrt: Illegal negative argument");
  if (r == 0.0)
    return (0.0);
  rr.d = r;
  if ((rr.dasii.i2 & emask) == emask)
    return (r);
  if ((rr.dasii.i2 & emask) == 0)
    {
      k = 54;
      rr.d *= 18014398509481984.0;
    }
  else
    k = 0;
  i = (rr.dasii.i2) >> 20;
  if (i & 1)
    j = i - 1021;
  else
    j = i - 1022;
  rr.dasii.i2 -= j << 20;

  x0 = 1.27235367 + 0.242693281 * rr.d - 1.02966039 / (1 + rr.d);
  x1 = (rr.d / x0 + x0) * 0.5;
  x2 = (rr.d / x1 + x1) * 0.5;
  x3.d = (rr.d / x2 + x2) * 0.5;
  x3.dasii.i2 += (j - k) << 19;
  return (x3.d);
}
#else

/* Algoritm from Numerical Mathematics and computing
 * - Cheney Kincaid page 57 #9 (slow but portabel)   */

double 
__rsqrt (r)
     double r;
{
  double y,
    w,
    z;
  if (r < 0.0)
    __rerror ("Sqrt: Illegal negative argument");
  if (r == 0.0)
    return (0.0);
  y = r * 0.5;
  z = 0.0;
  while (__TRUE)
    {
      w = (r / y - y) * 0.5;
      if ((w == 0) + (w == z))
	return (y);
      y = y + w;
      z = w;
    }
}
#endif
#endif
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE SIN(r)           */

#if nolib|rsinrut
#if FLOAT_IMPLEMENTED
double 
__rsin (r)
     double r;
{
#if MATHLIB
  return (sin (r));
#else
  return (sin (r));		/* Not implemented */
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE COS(r)           */

#if nolib|rcosrut
#if FLOAT_IMPLEMENTED
double 
__rcos (r)
     double r;
{
#if MATHLIB
  return (cos (r));
#else
  return (cos (r));		/* Not implemented */
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE TAN(r)           */

#if nolib|rtanrut
#if FLOAT_IMPLEMENTED
double 
__rtan (r)
     double r;
{
#if FULL_MATHLIB
  return (tan (r));
#else
  double s;
  s = cos (r);
  if (s == 0.0)
    __rerror ("Tan: Illegal division by zero");
  return (sin (r) / s);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE COTAN(r)         */

#if nolib|rcotanrut
#if FLOAT_IMPLEMENTED
double 
__rcotan (r)
     double r;
{
#if 0
  double t;
  t = __rtan (r);
  if (t == 0.0)
    __rerror ("Cotan: Illegal division by zero");
  return (1.0 / t);
#else
  double s;
  s = sin (r);
  if (s == 0.0)
    __rerror ("Cotan: Illegal division by zero");
  return (cos (r) / s);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ARCSIN(r)        */

#if nolib|rarcsinrut
#if FLOAT_IMPLEMENTED
double 
__rarcsin (r)
     double r;
{
#if FULL_MATHLIB
  return (asin (r));
#else
  return (__rarctan (r / __rsqrt (-r * r + 1.0)));
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ARCCOS(r)        */

#if nolib|rarccosrut
#if FLOAT_IMPLEMENTED
double 
__rarccos (r)
     double r;
{
#if FULL_MATHLIB
  return (acos (r));
#else
  return (-__rarctan (r / __rsqrt (-r * r + 1.0)) + 1.57079632679489661923);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ARCTAN(r)        */

#if nolib|rarctanrut
#if FLOAT_IMPLEMENTED
double 
__rarctan (r)
     double r;
{
  return (atan2 (r, 1.0));
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE ARCTAN2(r)       */

#if nolib|rarctan2rut
#if FLOAT_IMPLEMENTED
double 
__rarctan2 (r, s)
     double r,
       s;
{
  return (atan2 (r, s));
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE SINH(r)          */

#if nolib|rsinhrut
#if FLOAT_IMPLEMENTED
double 
__rsinh (r)
     double r;
{
#if FULL_MATHLIB
  return (sinh (r));
#else
  double e;
  e = __rexp (r);
  return ((e - 1.0 / e) / 2.0);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE COSH(r)          */

#if nolib|rcoshrut
#if FLOAT_IMPLEMENTED
double 
__rcosh (r)
     double r;
{
#if FULL_MATHLIB
  return (cosh (r));
#else
  double e;
  e = __rexp (r);
  return ((e - 1.0 / e) / 2.0);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE TANH(r)          */

#if nolib|rtanhrut
#if FLOAT_IMPLEMENTED
double 
__rtanh (r)
     double r;
{
#if FULL_MATHLIB
  return (tanh (r));
#else
  double e;
  e = __rexp (r);
  return ((-2.0) / e * (e + 1 / e) + 1);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE LN(r)            */

#if nolib|rlnrut
#if FLOAT_IMPLEMENTED
double 
__rln (r)
     double r;
{
#if MATHLIB
  return (log (r));
#else
  return (log (r));		/* Not implemented */
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE LOG10(r)         */

#if nolib|rlog10rut
#if FLOAT_IMPLEMENTED
double 
__rlog10 (r)
     double r;
{
#if MATHLIB
  return (log10 (r));
#else
  return (__rln (r) * 0.43429448190325182765);
#endif
}
#endif
#endif

/******************************************************************************
                                             REAL PROCEDURE EXP(r)           */

#if nolib|rexprut
#if FLOAT_IMPLEMENTED
double 
__rexp (r)
     double r;
{
#if MATHLIB
  return (exp (r));
#else
  return (exp (r));		/* Not implemented */
#endif
}
#endif
#endif

/******************************************************************************
                                             TEXT PROCEDURE MIN(t1,t2)       */

#if nolib|rmintrut
void 
__rmint (t1, t2)
     __txtvp t1,
       t2;
{
  if (__rlttext (t2, t1) == __TRUE)
    __et = *t2;
  else
    __et = *t1;
}
#endif

/******************************************************************************
                                              CHARACTER PROCEDURE MIN(c1,c2) */

#if nolib|rmincrut
char 
__rminc (c1, c2)
     char c1,
       c2;
{
  if ((unsigned char) c1 > (unsigned char) c2)
    return (c2);
  return (c1);
}
#endif

/******************************************************************************
                                              REAL PROCEDURE MIN(r1,r2)      */

#if nolib|rminrrut
#if FLOAT_IMPLEMENTED
double 
__rminr (r1, r2)
     double r1,
       r2;
{
  if (r1 > r2)
    return (r2);
  return (r1);
}
#endif
#endif

/******************************************************************************
                                              INTEGER PROCEDURE MIN(i1,i2)   */

#if nolib|rminirut
long 
__rmini (i1, i2)
     long i1,
       i2;
{
  if (i1 > i2)
    return (i2);
  return (i1);
}
#endif

/******************************************************************************
                                             TEXT PROCEDURE MAX(t1,t2)       */

#if nolib|rmaxtrut
void 
__rmaxt (t1, t2)
     __txtvp t1,
       t2;
{
  if (__rlttext (t2, t1) == __TRUE)
    __et = *t1;
  else
    __et = *t2;
}
#endif

/******************************************************************************
                                              CHARACTER PROCEDURE MAX(c1,c2) */

#if nolib|rmaxcrut
char 
__rmaxc (c1, c2)
     char c1,
       c2;
{
  if ((unsigned char) c1 > (unsigned char) c2)
    return (c1);
  return (c2);
}
#endif

/******************************************************************************
                                              REAL PROCEDURE MAX(r1,r2)      */

#if nolib|rmaxrrut
#if FLOAT_IMPLEMENTED
double 
__rmaxr (r1, r2)
     double r1,
       r2;
{
  if (r1 > r2)
    return (r1);
  return (r2);
}
#endif
#endif

/******************************************************************************
                                              INTEGER PROCEDURE MAX(i1,i2)   */

#if nolib|rmaxirut
long 
__rmaxi (i1, i2)
     long i1,
       i2;
{
  if (i1 > i2)
    return (i1);
  return (i2);
}
#endif

/******************************************************************************
                                              TEXT PROCEDURE SIMULAID        */

extern char *__progname;
#if nolib|rsimulaidrut
#define MAX_HOSTNAME_LEN 100
#define MAX_SITEID_LEN   100
#define MAX_INT_LEN       12
#define MAX_PROGNAME_LEN 100
#define MAX_SIMULAID_LEN (100+MAX_SITEID_LEN+MAX_HOSTNAME_LEN\
			  +3*MAX_INT_LEN+MAX_PROGNAME_LEN)
#if HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

__txtvp 
__rsimulaid (as)
     long as;
{
  char simulaid[MAX_SIMULAID_LEN];
  char siteid[MAX_SITEID_LEN];
  char cpu[MAX_HOSTNAME_LEN];
  char user[MAX_INT_LEN];
  char job[MAX_INT_LEN];
  char acc[MAX_INT_LEN];
#if HAVE_UNAME
  static struct utsname name;
#endif
#if HAVE_GETDOMAINNAME
  (void) getdomainname (siteid, MAX_SITEID_LEN);
#endif
#if HAVE_UNAME
  (void) uname (&name);
  strcpy (cpu, name.nodename);
#else
#if HAVE_GETHOSTNAME
  (void) gethostname (cpu, MAX_HOSTNAME_LEN);
#endif
#endif
#if HAVE_GETUID
  (void) sprintf (user, "%d", getuid ());
#endif
#if HAVE_GETPID
  (void) sprintf (job, "%d", getpid ());
#endif
#if HAVE_GETEGID
  (void) sprintf (acc, "%d", getegid ());
#endif
  (void) sprintf (simulaid, "%s!!!%s!!!%s!!!%s!!!%s!!!%s!!!%s!!!%s"
		  ,SIMID
#if HAVE_GETDOMAINNAME
		  ,siteid
#else
		  ,"???"
#endif
		  ,SYSTEM_TYPE
#if HAVE_UNAME || HAVE_GETHOSTNAME
		  ,cpu
#else
		  ,"???"
#endif
#if HAVE_GETUID
		  ,user
#else
		  ,"???"
#endif
#if HAVE_GETPID
		  ,job
#else
		  ,"???"
#endif
#if HAVE_GETEGID
		  ,acc
#else
		  ,"???"
#endif
		  ,__progname);
  (void) __rblanks (as, (long) strlen (simulaid));
  (void) sprintf (__et.obj->string, "%s", simulaid);
  return (&__et);
}
#endif

/******************************************************************************
                                              TEXT PROCEDURE DATETIME        */

#if nolib|rdatetimerut
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif
#if HAVE_GETTIMEOFDAY
#define DATETIMETEXTLENGTH 22L
#else
#define DATETIMETEXTLENGTH 19L
#endif

__txtvp 
__rdatetime (as)
     long as;
{
#if HAVE_GETTIMEOFDAY
  struct timeval tip;
#else
#if TIME_T
  time_t clock;
#else
  long clock;
#endif
#endif
  struct tm *tmp;
  (void) __rblanks (as, DATETIMETEXTLENGTH);
#if HAVE_GETTIMEOFDAY
  (void) gettimeofday (&tip, (struct timezone *) NULL);
  tmp = localtime (&tip.tv_sec);
  (void) sprintf (__et.obj->string, "%4d-%02d-%02d %02d:%02d:%02d.%02d",
#else
#if HAVE_TIME
#if TIME_T
  clock = time ((time_t *) __NULL);
#else
  clock = time ((long) __NULL);
#endif
  tmp = localtime (&clock);
  (void) sprintf (__et.obj->string, "%4d-%02d-%02d %02d:%02d:%02d",
#endif
#endif
		  tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday,
		  tmp->tm_hour, tmp->tm_min, tmp->tm_sec
#if HAVE_GETTIMEOFDAY
		  ,tip.tv_usec / 10000
#endif
    );
  return (&__et);
}
#endif

/******************************************************************************
                                              LONG REAL PROCEDURE CPUTIME    */

#if nolib|rcputimerut
#if FLOAT_IMPLEMENTED
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if HAVE_SYS_RESOURCE_H && HAVE_GETRESOURCE
#include<sys/resource.h>
#else
#if HAVE_SYS_TIMES_H
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <sys/times.h>
#endif
#endif

double 
__rcputime ()
{
#if HAVE_SYS_RESOURCE_H && HAVE_GETRESOURCE
  struct rusage rus;
#else
#if HAVE_SYS_TIMES_H && HAVE_TIMES
  struct tms buffer;
#endif
#endif
  static double old_cputime;
  double cputime;
#if HAVE_GETRESOURCE
  (void) getrusage (RUSAGE_SELF, &rus);
  cputime = ((double) rus.ru_utime.tv_sec) + ((double) rus.ru_stime.tv_sec) +
    (((double) rus.ru_utime.tv_usec) + ((double) rus.ru_stime.tv_usec)) /
    1000000.0;
#else
#if HAVE_SYS_TIMES_H && HAVE_TIMES
  times (&buffer);
  cputime = ((double) buffer.tms_utime + (double) buffer.tms_stime) /
#ifdef CLK_TCK
    (double) CLK_TCK;
#else
    60.0;
#endif
#else
  cputime = (double) clock () /
#if CLOCK_IN_USEC
    1000000.0;
#else
#ifdef CLOCKS_PER_SEC
    (double) CLOCKS_PER_SEC;
#else
    (double) CLK_TCK;
#endif
#endif
#endif
#endif
  if (cputime <= old_cputime)
    cputime = __raddepsilon (old_cputime);
  return (old_cputime = cputime);
}
#endif
#endif

/******************************************************************************
                                              LONG REAL PROCEDURE CLOCKTIME  */

#if nolib|rclocktimerut
#if FLOAT_IMPLEMENTED
#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY && !(nolib && HAVE_SYS_RESOURCE_H && HAVE_GETRESOURCE)
#include<sys/resource.h>
#endif

double 
__rclocktime ()
{
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
  struct timeval tip;
#else
#if TIME_T
  time_t clock;
#else
  long clock;
#endif
#endif
  struct tm *tmp;
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
  (void) gettimeofday (&tip, (struct timezone *) NULL);
  tmp = localtime (&tip.tv_sec);
#else
#if TIME_T
  clock = time ((time_t *) __NULL);
#else
  clock = time ((long) __NULL);
#endif
  tmp = localtime (&clock);
#endif
  return (((double) (tmp->tm_hour * 60 + tmp->tm_min) * 60 + tmp->tm_sec)
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
	  + ((double) tip.tv_usec) / 1000000.0
#endif
    );
}
#endif
#endif

/******************************************************************************
   		  			  INTEGER PROCEDURE LOWERBOUND(A,I)  */

#if nolib|rlowerboundrut
long 
__rlowerbound (a, i)
     __arrp a;
     long i;
{
  if (i < 1 || i > a->h.dim)
    __rerror ("Lowerbound: Illegal dimension");
  return (a->limits[i - 1].low);
}
#endif

/******************************************************************************
   		  			  INTEGER PROCEDURE UPPERBOUND(A,I)  */

#if nolib|rupperboundrut
long 
__rupperbound (a, i)
     __arrp a;
     long i;
{
  if (i < 1 || i > a->h.dim)
    __rerror ("Upperbound: Illegal dimension");
  return (a->limits[i - 1].low + a->limits[i - 1].size - 1);
}
#endif

/* RANDOM DRAWING PROSEDYRER */

#if SIZEOF_LONG == 8
#define PSURANNUM 7450580596923828125L	/* Se nedenfor (p=13) */
#else
#define PSURANNUM 1220703125L	/* I f|lge standarden lik 5**(2*p+1) (p=6) */
#endif
#if 0
#define CALCU (((unsigned long)(*U= (*U*PSURANNUM)|1))>>2)
#define MAXU (MAX_INT>>1)
#define BASICU ((((double)CALCU)+0.5)/((double)(MAXU+1)))
#else
#define CALCU (((unsigned long)(*U= (*U*PSURANNUM)|1))>>1)
#define MAXU (MAX_INT)
#define BASICU ((((double)CALCU)+0.5)/((double)MAXU+1.0))

#endif

/******************************************************************************
                                              BOOLEAN  PROCEDURE DRAW	     */

#if nolib|rdrawrut
#if FLOAT_IMPLEMENTED
char 
__rdraw (a, U)
     double a;
     long *U;
{
  return (BASICU < a);
}
#endif
#endif

/******************************************************************************
                                              INTEGER PROCEDURE RANDINT	     */

#if nolib|rrandintrut
#if FLOAT_IMPLEMENTED
long 
__rrandint (a, b, U)
     long a,
       b,
      *U;
{
  long u;
  double r;
  u = CALCU;
  if (b < a)
    __rerror ("Randint: Second parameter is lower than first parameter");
  u = r = (double) u *((double) b - (double) a + 1.0) / ((double) MAXU + 1.0) + (double) a;
  return ((u > r) ? u - 1 : u);
}
#else
long 
__rrandint (a, b, U)
     long a,
       b,
      *U;
{
  long u;
  u = CALCU;
  if (b < a)
    __rerror ("Randint: Second parameter is lower than first parameter");
  return (u % (b - a + 1) + a);
}
#endif
#endif

/******************************************************************************
                                            LONG REAL PROCEDURE UNIFORM	     */

#if nolib|runiformrut
#if FLOAT_IMPLEMENTED
double 
__runiform (a, b, U)
     double a,
       b;
     long *U;
{
  if (b < a)
    __rerror ("Runiform: Second parameter is lower than first parameter");
  return (BASICU * (b - a) + a);
}
#endif
#endif

/******************************************************************************
                                             LONG REAL PROCEDURE NORMAL	     */

#if nolib|rnormalrut
#if FLOAT_IMPLEMENTED
#if 0				/* BOX-MULLER METHOD FOR THE NORMAL This
				 * metod is not choosen since it performs at
				 * least two random drawings. Not allowed eg. 
				 * SS. */

#define NORMALPSURANNUM 48828125	/* I f|lge standarden lik 5**(2*p+1)
					 * (p=5) */
#define NORMALCALCU (((unsigned long)(u= u*NORMALPSURANNUM))>>2)
#define NORMALBASICU ((((double)NORMALCALCU)+0.5)/((double)(MAXU+1)))
double 
__rnormal (a, b, U)
     double a,
       b;
     long *U;
{
  static count;
  static double v1,
    v2,
    s,
    x;
  long u;
  u = CALCU;
  if ((count++) % 2 == 0)
    {
      do
	{
	  v1 = NORMALBASICU * 2.0 - 1.0;
	  v2 = NORMALBASICU * 2.0 - 1.0;
	  s = v1 * v1 + v2 * v2;
	}
      while (s >= 1.0);
#if MATHLIB
      x = sqrt (-2.0 * log (s) / s) * v1;
#else
      x = __rsqrt (-2.0 * __rln (s) / s) * v1;
#endif
    }
  else
#if MATHLIB
    x = sqrt (-2.0 * log (s) / s) * v2;
#else
    x = __rsqrt (-2.0 * __rln (s) / s) * v2;
#endif
  return (b * x + a);
}
#else /* PAUL BRATLEY BENNETT L.FOX LINUS E.SCHRAGE: A GUIDE TO SIMULATION p. 
       * 149 */
#define p0 (-0.322232431088)
#define p1 (-1)
#define p2 (-0.342242088547)
#define p3 (-0.0204231210245)
#define p4 (-0.0000453642210148)
#define q0 0.099348462606
#define q1 0.588581570495
#define q2 0.531103462366
#define q3 0.10353775285
#define q4 0.0038560700634

double 
__rnormal (a, b, U)
     double a,
       b;
     long *U;
{
  static double y,
    x,
    p,
    u;
  u = BASICU;
  if (u > 0.5)
    p = 1.0 - u;
  else
    p = u;
#if MATHLIB
  y = sqrt (-log (p * p));
#else
  y = __rsqrt (-__rln (p * p));
#endif
  x = y + ((((y * p4 + p3) * y + p2) * y + p1) * y + p0) 
    / ((((y * q4 + q3) * y + q2) * y + q1) * y + q0);
  if (u < 0.5)
    x = -x;
  return (b * x + a);
}
#endif
#endif
#endif

/******************************************************************************
                                            LONG REAL PROCEDURE NEGEXP	     */

#if nolib|rnegexprut
#if FLOAT_IMPLEMENTED
double 
__rnegexp (a, U)
     double a;
     long *U;
{
  if (a <= 0.0)
    __rerror ("Negexp: First parameter is lower than zero");
#if MATHLIB
  return (-log (BASICU) / a);
#else
  return (-__rln (BASICU) / a);
#endif
}
#endif
#endif

/******************************************************************************
                                              INTEGER PROCEDURE POISSON	     */

#if nolib|rpoissonrut
#if FLOAT_IMPLEMENTED
long 
__rpoisson (a, U)
     double a;
     long *U;
{
  double expna,
    prod;
  long n;
  expna = exp (-a);
  prod = BASICU;
  for (n = 0; prod >= expna; n++)
    prod *= BASICU;
  return (n);
}
#endif
#endif

/******************************************************************************
                                            LONG REAL PROCEDURE ERLANG	     */

#if nolib|rerlangrut
#if FLOAT_IMPLEMENTED
double 
__rerlang (a, b, U)
     double a,
       b;
     long *U;
{
  double sum = 0;
  long ci,
    bi;
  if (a == 0.0)
    __rerror ("Erlang: First parameter is equal zero");
  if (b <= 0.0)
    __rerror ("Erlang: Second parameter is not greater than zero");
  bi = (long) b;
  if (bi == b)
    bi--;
#if MATHLIB
  for (ci = 1; ci <= bi; ci++)
    sum += log (BASICU);
  return (-(sum + (b - (double) (ci - 1)) * log (BASICU)) / (a * b));
#else
  for (ci = 1; ci <= bi; ci++)
    sum += __rln (BASICU);
  return (-(sum + (b - (double) (ci - 1)) * __rln (BASICU)) / (a * b));
#endif
}
#endif
#endif


#ifdef REALASDOUBLE
#define ARRELEM(A,i) (*(double *)(((char *)A)+sizeof(__ah)+\
				 sizeof(__arrlimit)+sizeof(double)*(i)))
#else
#define ARRELEM(A,i) (*(float *)(((char *)A)+sizeof(__ah)+\
				 sizeof(__arrlimit)+sizeof(float)*(i)))
#endif

/******************************************************************************
                                             INTEGER PROCEDURE DISCRETE	     */

#if nolib|rdiscreterut
#if FLOAT_IMPLEMENTED
long 
__rdiscrete (A, U)
     __arrp A;
     long *U;
{
  long j;
  double basic;
  if (A->h.dim != 1)
    __rerror ("Disrete: Multi dimensional array");
  basic = BASICU;
  for (j = 0; j < A->limits[0].size; j++)
    if (ARRELEM (A, j) > basic)
      break;
  return (A->limits[0].low + j);
}
#endif
#endif

/******************************************************************************
                                            LONG REAL PROCEDURE LINEAR	     */

#if nolib|rlinearrut
#if FLOAT_IMPLEMENTED
double 
__rlinear (A, B, U)
     __arrp A,
       B;
     long *U;
{
  long j;
  double basic,
    d;
  if (A->h.dim != 1 || B->h.dim != 1)
    __rerror ("Linear: Multi dimensional array");
  if (A->limits[0].size != B->limits[0].size)
    __rerror ("Linear: Arrays of different sizes");
  if (ARRELEM (A, 0) != 0.0 || ARRELEM (A, (A->limits[0].size) - 1) != 1.0)
    __rerror ("Linear: Illegal value on first array");
  basic = BASICU;
  for (j = 0; j < A->limits[0].size; j++)
    if (ARRELEM (A, j) >= basic)
      break;
  d = ARRELEM (A, j) - ARRELEM (A, j - 1);
  if (d == 0.0)
    return (ARRELEM (B, j - 1));
  return (ARRELEM (B, j - 1) + (ARRELEM (B, j) - ARRELEM (B, j - 1)) *
	  (basic - ARRELEM (A, j - 1)) / d);
}
#endif
#endif

/******************************************************************************
                                             INTEGER PROCEDURE HISTD	     */

#if nolib|rhistdrut
#if FLOAT_IMPLEMENTED
long 
__rhistd (A, U)
     __arrp A;
     long *U;
{
  double sum = 0.0,
    weight;
  long j;
  if (A->h.dim != 1)
    __rerror ("Histd: Multi dimensional array");
  for (j = 0; j < A->limits[0].size; j++)
    sum += ARRELEM (A, j);
  weight = BASICU * sum;
  sum = 0.0;
  for (j = 0; j < A->limits[0].size - 1; j++)
    {
      sum += ARRELEM (A, j);
      if (sum >= weight)
	break;
    }
  return (A->limits[0].low + j);
}
#endif
#endif

/******************************************************************************
                                                      PROCEDURE HISTO	     */

#if nolib|rhistorut
#if FLOAT_IMPLEMENTED
void 
__rhisto (A, B, c, d)
     __arrp A,
       B;
     double c,
       d;
{
  long j;
  if (A->h.dim != 1 || B->h.dim != 1)
    __rerror ("Histo: Multi dimensional array");
  if (A->limits[0].size != B->limits[0].size + 1)
    __rerror ("Histo: Illegal size of arrays");
  for (j = 0; j < B->limits[0].size; j++)
    if (c <= ARRELEM (B, j))
      break;
  ARRELEM (A, j) += d;
}
#endif
#endif

/******************************************************************************
                                                      PROCEDURE TERROR	     */

#if nolib|rterrorrut
void 
__rterror (t)
     __txtvp t;
{
  long i;
  __rprintfilline ();
  (void) fprintf (stderr, "RUNTIME-ERROR: ");
  for (i = t->start; i < t->start + t->length; i++)
    (void) putc (t->obj->string[i - 1], stderr);
  (void) putc ('\n', stderr);
#ifndef lint
  exit (1);
#endif
}
#endif
