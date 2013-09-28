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
                                              INTEGER PROCEDURE RANDINT	     */

long __rrandint (long a, long b, long *U)
{
  long u;
  double r;
  u = CALCU;
  if (b < a)
    __rerror ("Randint: Second parameter is lower than first parameter");
  u = r = (double) u *((double) b - (double) a + 1.0) / ((double) MAXU + 1.0) + (double) a;
  return ((u > r) ? u - 1 : u);
}
