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

/******************************************************************************
                                            LONG REAL PROCEDURE LINEAR	     */

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
