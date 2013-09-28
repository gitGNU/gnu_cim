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
                                                     RCOPYTEXTARRTOC         */

/* Denne rutinen brukes for } overf|re en text-array by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc.
 * Rutinen bruker RCOPYTEXTTOC for } kopiere selve tekstene */

char **__rcopytextarrtoc (__arrp p, char byvalue)
{
  long sizeofhode,
    size,
    i;
  __txtvp *p1x;
  char **p2x;
  sizeofhode = sizeof (__ah) + sizeof (__arrlimit) * p->h.dim;
  p1x = (__txtvp *) ((long) p + sizeofhode);
  p2x = (char **) xmalloc ((unsigned) (size = p->h.size - sizeofhode));
  size /= sizeof (__txtvp);
  if (byvalue)
    for (i = 0; i < size; i++)
      p2x[i] = __rcopytexttoc (p1x[i]);
  else
    for (i = 0; i < size; i++)
      p2x[i] = &p1x[i]->obj->string[p1x[i]->start - 1];
  return (p2x);

}
