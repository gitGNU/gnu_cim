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

#include "epsilon.h"
/******************************************************************************
                                             REAL PROCEDURE ADDEPSILON(r)    */

/* Denne rutinen skal h}ndtere alle floating-point formater.
 * F|rste rutine h}ndterer IEEE-, IBM- og VAX-format effektivt.
 * For disse formatene anntar den 32/64-bits representasjon.
 * IBM-formatet er ikke testet da vi mangler maskin } kj|re det p}
 * Andre rutine er en treg men portabel variant. */

#if FLOAT_IEEE | FLOAT_VAX | FLOAT_IBM
double 
__raddepsilon (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN || FLOAT_VAX
	  TYPE_32_INT i2;
	  unsigned TYPE_32_INT i1;
#else
	  unsigned TYPE_32_INT i1;
	  TYPE_32_INT i2;
#endif
	}
      dasii;
#if FLOAT_VAX
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
#if FLOAT_VAX
  short s;
#endif
  if (r == MAX_DOUBLE)
    return (r);
  value.d = r;
#if FLOAT_VAX
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
#if FLOAT_VAX
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
double 
__raddepsilon (r)
     double r;
{
  double s = MIN_DOUBLE,
    t,
    u;
  u = r;
  if (u >= MAX_DOUBLE)
    return (r);
  t = u + s;
  while (t == u)
    {
      s *= 2.0;
      t = u + s;
    }
  return (r + (t - u));
}
#endif
