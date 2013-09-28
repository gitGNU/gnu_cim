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

#include "text.h"

/******************************************************************************
                                  INTEGER PROCEDURE GETFRAC (text attributt) */

long __rtgetfrac (__txtvp t)
{
  char *s;
  int sign;
  long i,
    j;
  long n = 0;

  s = t->obj->string;
  i = t->start - 1;
  j = i + t->length;

  skipblanke;
  if (!more)
    __rerror ("Getfrac: Can't find any grouped item");
  sign = fortegn;
  if (sign)
    i++;
  else
    sign = 1;
  skipblanke;
  if (!more)
    __rerror ("Getfrac: Can't find any grouped item");
  if (!digit)
    {
      if (s[i] == __currentdecimalmark)
	{
	  i++;
	  if (!digit)
	    __rerror ("Getfrac: Illegal grouped item");
	}
      else
	__rerror ("Getfrac: Can't find any grouped item");
    }

  while (1)
    {
      for (; more && digit; i++)
	{
	  if (n > MAX_INT / 10)
	    goto forstort;
	  if (n == MAX_INT / 10)
	    if ((s[i] - '0') > MAX_INT % 10)
	      goto forstort;
	  n = 10 * n + s[i] - '0';
	}
      if (!more)
	goto ut;
      skipblanke;
      if (!more)
	goto ut;
      if (!digit)
	{
	  if (s[i] == __currentdecimalmark)
	    {
	      i++;
	      if (!digit)
		{
		  --i;
		  goto ut;
		}
	    }
	  else
	    goto ut;
	}
    }
ut:
  /* G}r tilbake til siste siffer */
  while (!digit)
    i--;
  t->pos = i - t->start + 3;
  return (sign * n);
forstort:
  __rwarning ("Getfrac: To big grouped item");
  return (sign * n);
}
