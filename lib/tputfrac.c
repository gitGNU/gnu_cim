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
                                 PROCEDURE PUTFRAC (text attributt)          */

__txtvp 
__rtputfrac (t, i, n)
     __txtvp t;
     long i;
     long n;
{
  char *s,
    minus;
  long j,
    antg = 0,
    k;

  if (minus = i < 0)
    i *= -1;
  if (t->obj == __NULL)
    __rerror ("Putfrac: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putfrac: Constant text object");
  s = t->obj->string;
  j = t->start + t->length - 1;
  if (n > 0)
    {
      if ((n % 3) == 1)
	antg = 2;
      else if ((n % 3) == 2)
	antg = 1;

      for (k = 1; k <= n; k++)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = '0' + (i % 10);
	  i /= 10;
	  if (++antg == 3 && k < n)
	    {
	      if (j < t->start)
		goto forkort;
	      s[--j] = ' ';
	      antg = 0;
	    }
	}
      if (j < t->start)
	goto forkort;
      s[--j] = __currentdecimalmark;
      antg = 0;
    }
  else
    {
      n *= -1;
      for (k = 0; k < n; k++)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = '0';
	  if (++antg == 3)
	    {
	      if (j < t->start)
		goto forkort;
	      s[--j] = ' ';
	      antg = 0;
	    }
	}
    }
  while (i > 0 && j >= t->start)
    {
      if (j < t->start)
	goto forkort;
      s[--j] = '0' + (i % 10);
      i /= 10;
      if (++antg == 3 && i != 0)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = ' ';
	  antg = 0;
	}
    }
  if ((i == 0 && minus && j < t->start) || i != 0)
    goto forkort;
  else
    {
      if (minus)
	s[--j] = '-';
      for (i = t->start - 1; i < j; i++)
	s[i] = ' ';
      t->pos = t->length + 1;
    }
  __rputlen = t->start + t->length - 1 - j;
  return (t);
forkort:
  __rwarning ("Putfrac: Text object to short");
  for (j = 0; (long) j < t->length; j++)
    s[t->start + j - 1] = '*';
  t->pos = t->length + 1;
  __rputlen = j;
  return (t);
}
