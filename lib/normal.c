/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen,
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "random.h"
#include "trigno.h"

/******************************************************************************
                                             LONG REAL PROCEDURE NORMAL	     */

/* PAUL BRATLEY BENNETT L.FOX LINUS E.SCHRAGE: A GUIDE TO SIMULATION p. 149 */
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
