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
#include <stdio.h>

/******************************************************************************
                                         PROCEDURE LOCATE(i)                 */

__dhp __rdblocate (__bs10FILE *p, long i)
{
  if ((i < 1) | (i > p->maxloc))
    __rerror ("Locate: Parameter out of range");
  if (p->loc != i)
    {
      p->loc = i;
      if (fseek (((__bs1FILE *) p)->file, i - 1, 0) == __EOF)
	__rerror ("Locate: Not possible to seek");
      p->lastop = __SEEK;
    }
  return ((__dhp) p);
}
