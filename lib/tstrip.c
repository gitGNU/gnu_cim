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
                                  TEXT PROCEDURE STRIP (text attributt)      */

__txtvp 
__rtstrip (t)
     __txtvp t;
{
  long pos;

  for (pos = (long) t->start + (long) t->length - 1;
       (long) t->start <= pos && t->obj->string[pos - 1] == ' '; pos--);

  if (pos < (long) t->start)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = t->start;
      __et.pos = 1;
      __et.length = pos - (long) t->start + 1;
    }
  return (&__et);
}
