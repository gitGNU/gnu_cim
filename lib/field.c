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

#define INCLUDE_SIMFILE_H
#include "cim.h"

/******************************************************************************
                                          TEXT PROCEDURE FIELD(w)            */

/* ikke en attributt prosedyre */

void __rfield (__bs2FILE *p, long w)
{
  if (w > p->IMAGE.length)
    __rerror ("Outint, outfix etc: Width on out field is greater than IMAGE.length");
  if (p->IMAGE.pos + w - 1 > p->IMAGE.length)
    if (((__bs1FILE *) p)->h.pp == &__p6FILE)
      __rpoutimage (((__bs6FILE *) p));
    else if (((__bs1FILE *) p)->h.pp == &__p5FILE)
      __rdoutimage ((__bs5FILE *) p);
    else
      __rooutimage (p);
  __et.obj = p->IMAGE.obj;
  __et.start = p->IMAGE.start + p->IMAGE.pos - 1;
  __et.length = w;
  __et.pos = 1;
}
