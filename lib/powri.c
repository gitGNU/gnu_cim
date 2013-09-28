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

#include "cim.h"
#include "config.h"

/******************************************************************************
                                                REAL PROCEDURE POWRI(r,i)    */

double __rpowri (double r, long i)
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
