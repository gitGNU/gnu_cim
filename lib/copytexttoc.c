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
                                                     RCOPYTEXTTOC            */

/* Denne rutinen brukes for } overf|re en text-variabel by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc, for s}
 * } kopiere teksten over i dette omr}det. Teksten blir terminert med 0 */

char *
__rcopytexttoc (t)
     __txtvp t;
{
  char *p;

  p = (char *) xmalloc ((unsigned) t->length + 1);

  (void) strncpy (p, &t->obj->string[t->start - 1], t->length);
  p[t->length] = '\0';
  return (p);
}
