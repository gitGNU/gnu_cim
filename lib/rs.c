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
#include "align.h"

/******************************************************************************
                                                        RutineRestoreStack   */

void __rrs (void)
{
  long i;
  __stkp x;
  long size;
  long ar,
    av,
    at;
  ar = __pb->pm;
  av = __pb->dt;
  at = ((__stkp) __pb)->h.at;
  x = (__stkp) __pb;
  size = x->h.size;
  __pb = __pb->dl;
  for (i = av; i; i--)
    {
      __v[i] = x->s[i - 1].v;
    }

  for (i = ar; i; i--)
    __r[i] = x->s[i - 1 + av].r;

  for (i = at; i; i--)
    {
      __t[i].obj = (__textref) x->s[av + ar + i - 1].r;
      __t[i].length = x->s[at + i * 3 - 3 + av + ar].v.i;
      __t[i].pos = x->s[at + i * 3 - 2 + av + ar].v.i;
      __t[i].start = x->s[at + i * 3 - 1 + av + ar].v.i;
    }

  if ((char *) __fri == ((char *) x) + align (size))
    {
      memset ((char *) x, 0, (char *) __fri - (char *) x);
      __fri = (__dhp) x;
    }

}
