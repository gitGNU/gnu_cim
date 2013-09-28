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
                                 TEXT PROCEDURE MAIN (text attributt)        */

__txtvp __rtmain (__txtvp t)
{
  if (t->obj == __NULL)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = 1;
      __et.length = t->obj->h.size;
      __et.start = 1;
    }
  return (&__et);
}
