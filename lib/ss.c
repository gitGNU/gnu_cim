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
                                                        RutineSaveStack      */

void __rss (long as)
{
  long size,
    i,
    ar,
    av,
    at;
  __stkp x;
  /* Tilordner antref og anttext i tilfelle kallet p} ralloc f|rer til */
  /* garbage collection. Gbc kan da lese av antall pekere p} stakken */
  /* den m} ta hensyn til. */
  __as = as;
  av = as >> 16 & 0xff;
  ar = as >> 8 & 0xff;
  at = as & 0xff;

  size = sizeof (__stk) - sizeof (__val_or_ref)
    + (ar + av + (4 * at))
    * sizeof (__val_or_ref);
  x = (__stkp) __ralloc (size);
  __as = 0;
  x->h.pp = (__pty) __ACTS;
  x->h.size = size;
  x->h.pm = ar;
  x->h.dt = av;
  x->h.at = at;
  x->h.dl = __pb;
  __pb = (__dhp) x;

  for (i = av; i; i--)
    {
      x->s[i - 1].v = __v[i];
    }

  for (i = ar; i; i--)
    x->s[i - 1 + av].r = __r[i];

  for (i = at; i; i--)
    {
      x->s[av + ar + i - 1].r = (__dhp) __t[i].obj;
      x->s[at + i * 3 - 3 + av + ar].v.i = (long) __t[i].length;
      x->s[at + i * 3 - 2 + av + ar].v.i = (long) __t[i].pos;
      x->s[at + i * 3 - 1 + av + ar].v.i = (long) __t[i].start;
    }

}
