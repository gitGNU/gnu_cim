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
                                         PROCEDURE OUTTEXT(t)                */

__dhp __roouttext (__bs2FILE *p, __txtvp t)
{
  register long i;
  char *s;
  if (p->IMAGE.pos > 1 && t->length > (p->IMAGE.length - p->IMAGE.pos + 1))
    {				/* Sorger for at riktige outimage kalles */
      if (((__bs1FILE *) p)->h.pp == &__p6FILE)
	__rpoutimage (((__bs6FILE *) p));
      else if (((__bs1FILE *) p)->h.pp == &__p5FILE)
	__rdoutimage ((__bs5FILE *) p);
      else
	__rooutimage (p);
    }
  s = t->obj->string;
  for (i = 0; i < t->length; i++)
    __rooutchar (p, s[t->start + i - 1]);
  return ((__dhp) p);
}
