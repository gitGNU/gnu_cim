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
                                             INTEGER PROCEDURE HISTD	     */

long __rhistd (__arrp A, long *U)
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
