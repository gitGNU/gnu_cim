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
                                            LONG REAL PROCEDURE ERLANG	     */

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
  for (ci = 1; ci <= bi; ci++)
    sum += log (BASICU);
  return (-(sum + (b - (double) (ci - 1)) * log (BASICU)) / (a * b));
}
