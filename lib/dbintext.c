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
                                         TEXT PROCEDURE INTEXT(t)            */

__txtvp
__rdbintext (p, t)
     __bs10FILE *p;
     __txtvp t;
{
  __rtsetpos (t, 1L);
  while (__rtmore (t) & !((__bs7FILE *) p)->endfile)
    __rtputchar (t, (char) __rdbinbyte (p));
  if (((__bs7FILE *) p)->endfile)
    __rtsetpos (t, __rtpos (t) - 1);
  return(__rtsub (t, 1L, __rtpos (t) - 1));
}
