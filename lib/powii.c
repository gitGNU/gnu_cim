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
/******************************************************************************
                                             INTEGER PROCEDURE POWII(i,j)    */

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
