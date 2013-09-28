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
#include "simfile.h"

/******************************************************************************
                                       PROCEDURE OUTFRAC(i,n,w)              */

__dhp __rooutfrac (__bs2FILE *p, long i, long n, long  w)
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputfrac */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputfrac (&tv, i, n);
      __rfield (p, __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k]
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (p, -w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputfrac (&ti, i, n);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (p, w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputfrac (&ti, i, n);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
