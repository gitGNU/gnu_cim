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
                                          TEXT PROCEDURE UPCASE(t)           */

__txtvp __rupcase (__txtvp t)
{
  register long i;
  unsigned char *s;
  s = (unsigned char *) t->obj->string;

  for (i = 0; i < t->length; i++)
    s[t->start + i - 1] =
      (isalpha (s[t->start + i - 1]) 
       ? (islower (s[t->start + i - 1]) 
	  ? toupper ((int) s[t->start + i - 1]) : s[t->start + i - 1])
       : s[t->start + i - 1]);
  __et.obj = t->obj;
  __et.length = t->length;
  __et.pos = 1;
  __et.start = t->start;
  return(&__et);
}
