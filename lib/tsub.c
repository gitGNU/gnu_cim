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
                                 TEXT PROCEDURE SUB (text attributt)         */

__txtvp __rtsub (__txtvp t, long i, long n)
{
  if (i <= 0 || n < 0 || i + n > t->length + 1)
    __rerror ("Sub: Outside text frame");
  if (n == 0)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = t->start + i - 1;
      __et.length = n;
      __et.pos = 1;
    }
  return (&__et);
}
