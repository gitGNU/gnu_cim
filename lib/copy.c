/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen and Terje Mjoes,
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
                                   TEXT PROCEDURE COPY                      */

__txtvp 
__rcopy (as, t)
     __txtvp t;
     long as;
{
  long i,
    j = 0;

  if (t->obj == __NULL)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __t1 = *t;
      __as = as;
      __et.obj = (__textref) __ralloc (((long) sizeof (__th) + __t1.length + 1));
      /* En ekstra byte |nsker */
      __et.obj->h.size = __et.length = __t1.length;	/* plass 
								 * til 
								 * '\0' */
      __et.obj->h.konstant = __FALSE;
      __et.obj->h.pp = (__pty) __TEXT;
      __as = 0;
      __et.pos = __et.start = 1;

      for (i = __t1.start; i < (long) __t1.start + __t1.length; i++)
	__et.obj->string[j++] = __t1.obj->string[i - 1];

      __et.obj->string[j++] = '\0';	/* Avslutter med \0 */
      __t1.obj = __NULL;
    }
  return (&__et);
}
