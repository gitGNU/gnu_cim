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
                                         PROCEDURE OUTINT(i,w)              */

__dhp 
__rooutint (p, i, w)
     __bs2FILE *p;
     long i,
       w;
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputint */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputint (&tv, i);
      __rfield (((__bs2FILE *) p), __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k] 
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (((__bs2FILE *) p), -w);		/* Returverdier i et */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputint (&ti, i);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (((__bs2FILE *) p), w);	/* Returverdier i et */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputint (&ti, i);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
