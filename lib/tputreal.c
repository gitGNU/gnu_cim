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
                                 PROCEDURE PUTREAL (text attributt)          */

__txtvp 
__rtputreal (t, r, n)
     __txtvp t;
     double r;
     long n;
{
  static char cs[__RTPUTTEXTLENGTH + 1],
    fcs[__RTPUTTEXTFORMATLENGTH + 1];
  long i,
    j,
    l;
  char *s;
  if (n < 0)
    __rerror ("Putreal: Second parameter is lesser than zero");
  if (n > __RTPUTMAXN)
    __rerror ("Putreal: Last parameter to big");
  if (t->obj == __NULL)
    __rerror ("Putreal: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putreal: Constant text object");
  s = t->obj->string;
  (void) sprintf (fcs, "%%.%d%s", (n > 0) ? n - 1 : 0, "le");
  (void) sprintf		/* ARGSUSED */
    (cs, fcs, r);
  if ((cs[0] == 'I') | (cs[1] == 'I'))	/* Test p} om det er lik uendelig */
    __rerror ("Illegal real number");
  if (n == 0)
    for (i = (cs[0] == '-') ? 1 : 0; cs[i]; i++)
      cs[i] = cs[i + 1];
  if (cs[0] == '-')
    for (i = 1; __TRUE; i++)
      {
	if (cs[i] == '0' || cs[i] == '.')
	  continue;
	if (cs[i] == 'e')
	  {
	    for (i = 0; cs[i]; i++)
	      cs[i] = cs[i + 1];
	    break;
	  }
	break;
      }
  if (__currentdecimalmark != '.')
    {
      for (i = 0; cs[i] != '.'; i++);
      cs[i] = __currentdecimalmark;
    }
  for (i = n; cs[i] != 'e'; i++);
  cs[i] = __currentlowten;
  if (cs[i + 3] == '\0')
    {
      cs[i + 5] = '\0';
      cs[i + 4] = cs[i + 2];
      cs[i + 3] = '0';
      cs[i + 2] = '0';
    }
  else if (cs[i + 4] == '\0')
    {
      cs[i + 5] = '\0';
      cs[i + 4] = cs[i + 3];
      cs[i + 3] = cs[i + 2];
      cs[i + 2] = '0';
    }
  if ((l = strlen (cs)) <= t->length)
    {
      i = t->length - l;
      for (j = 0; j < i; j++)
	s[t->start + j - 1] = ' ';
      for (i = 0; i < l; i++)
	s[t->start + (j++) - 1] = cs[i];
    }
  else
    {
      __rwarning ("Putreal: Text object to short");
      for (j = 0; j < (long) t->length; j++)
	s[t->start + j - 1] = '*';
    }
  t->pos = t->length + 1;
  __rputlen = l;
  return (t);
}
