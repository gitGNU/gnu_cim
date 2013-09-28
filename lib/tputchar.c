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
                                 PROCEDURE PUTCHAR (text attributt)          */

__txtvp __rtputchar (__txtvp t, char c)
{
  if (t->obj == __NULL)
    __rerror ("Putchar: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putchar: Constant text object");
  if (t->pos > t->length)
    __rerror ("Putchar: Illegal pos value");
  t->obj->string[t->start + (t->pos++) - 2] = c;
  return (t);
}



