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

/******************************************************************************
                                         INTEGER PROCEDURE LASTLOC           */

long 
__rdblastloc (p)
     __bs10FILE *p;
{
  long i;
  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
    __rerror ("Lastloc: Not possible to seek");
  i = ftell (((__bs1FILE *) p)->file);
  if (fseek (((__bs1FILE *) p)->file, p->loc-1, 0) == __EOF)
    __rerror ("Lastloc: Not possible to seek");
  p->lastop = __SEEK;
  return (i);
}
