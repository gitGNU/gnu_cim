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
                                                         RCOPYARRTOC         */

/* Denne rutinen brukes for } overf|re en array by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc. */

char *__rcopyarrtoc (__arrp p)
{
  long sizeofhode,
    size,
    i;
  char *p1x,
   *p2x;
  sizeofhode = sizeof (__ah) + sizeof (__arrlimit) * p->h.dim;
  p1x = (char *) ((long) p + sizeofhode);
  p2x = (char *) xmalloc ((unsigned) (size = p->h.size - sizeofhode));
  for (i = 0; i < size; i++)
    p2x[i] = p1x[i];
  return (p2x);
}
