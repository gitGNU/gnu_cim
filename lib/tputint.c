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
                                 PROCEDURE PUTINT (text attributt)          */

#if 0				/* Denne rutinen virker ikke for minint */
__txtvp 
rtputint (t, i)
     __txtvp t;
     long i;
{
  long j;
  char *s,
    minus;
  if (minus = i < 0)
    i *= -1;
  if (t->obj == __NULL)
    __rerror ("Putint: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putint: Constant text object");
  s = t->obj->string;
  j = t->start + t->length - 1;

  if (i == 0)
    s[--j] = '0';
  else
    {
      while (i > 0 && j >= (long) t->start)
	{
	  s[--j] = '0' + (i % 10);
	  i /= 10;
	}
    }
  if ((i == 0 && minus && j < (long) t->start) || i != 0)
    {
      __rwarning ("Putint: Text object to short");
    }
  {
    if (minus)
      s[--j] = '-';
    for (i = t->start - 1; i < j; i++)
      s[i] = ' ';
  }
  t->pos = t->length + 1;
  __rputlen = t->start + t->length - 1 - j;
  return (t);
}

#else

__txtvp 
__rtputint (t, ii)
     __txtvp t;
     long ii;
{
  static char cs[__RTPUTTEXTLENGTH + 1];
  long i,
    j,
    l;
  char *s;
  if (t->obj == __NULL)
    __rerror ("Putint: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putint: Constant text object");
  s = t->obj->string;
  (void) sprintf (cs, "%ld", ii);
  if ((l = strlen (cs)) <= (long) t->length)
    {
      i = t->length - l;
      for (j = 0; j < i; j++)
	s[t->start + j - 1] = ' ';
      for (i = 0; i < l; i++)
	s[t->start + (j++) - 1] = cs[i];
    }
  else
    {
      __rwarning ("Putint: Text object to short");
      for (j = 0; j < (long) t->length; j++)
	s[t->start + j - 1] = '*';
    }
  t->pos = t->length + 1;
  __rputlen = l;
  return (t);
}

#endif
