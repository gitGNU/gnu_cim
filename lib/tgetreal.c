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

#ifdef STDC_HEADERS
#include <stdlib.h>
#else
double strtod ();
#endif

/******************************************************************************
                                     REAL PROCEDURE GETREAL (text attributt) */

double __rtgetreal (__txtvp t)
{
  char *s;
  int sign;
  static char cs[__RTPUTTEXTLENGTH + 1];
  long csi = 0,
    i,
    j,
    p;
  double r;
#if FLOAT_IEEE | FLOAT_VAX
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN || FLOAT_VAX
	  long i2;
	  unsigned long i1;
#else
	  unsigned long i1;
	  long i2;
#endif
	}
      dasii;
    }
  value;
#endif
  s = t->obj->string;
  i = t->start - 1;
  j = i + t->length;

  skipblanke;
  if (!more)
    __rerror ("Getreal: Can't find any real item");
  if ((sign = fortegn))
    i++;
  if (sign == -1)
    cs[csi++] = '-';
  skipblanke;
  if (!more)
    __rerror ("Getreal: Can't find any integer item");
  if (!digit) {
    if (s[i] == __currentlowten)
      {
	cs[csi++] = '1';
      }
    else if (s[i] != __currentdecimalmark)
      __rerror ("Getreal: Illegal real item");
  }
  for (; more && digit; i++)
    {
      if (csi >= __RTPUTTEXTLENGTH)
	goto texttolong;
      cs[csi++] = s[i];
    }
  if (s[i] == __currentdecimalmark)
    {
      i++;
      cs[csi++] = '.';
      if (!digit)
	__rerror ("Getreal: Can't find any real item");
      for (; more && digit; i++)
	{
	  if (csi >= __RTPUTTEXTLENGTH)
	    goto texttolong;
	  cs[csi++] = s[i];
	}
    }
  if (s[i] == __currentlowten)
    {
      i++;
      cs[csi++] = 'e';
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      if ((sign = fortegn))
	i++;
      if (csi >= __RTPUTTEXTLENGTH)
	goto texttolong;
      if (sign == -1)
	cs[csi++] = '-';
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      for (; more && digit; i++)
	{
	  if (csi >= __RTPUTTEXTLENGTH)
	    goto texttolong;
	  cs[csi++] = s[i];
	}
    }
  if (csi > __RTPUTTEXTLENGTH)
    goto texttolong;
  cs[csi++] = '\0';
  r= strtod (cs, __NULL);
#if FLOAT_VAX
#define reswd (1<<31)
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    goto texttolong;
#endif
#if FLOAT_IEEE
#define emask (2047L<<20)
  value.d = r;
  if ((value.dasii.i2 & emask) == emask)
    goto texttolong;
#endif
  t->pos = i - t->start + 2;
  return (r);
texttolong:
  __rerror ("Getreal: To big real item");
  /* NOTREACHED */
  return 0;
}
