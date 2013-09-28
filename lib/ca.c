/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen and Terje Mjoes,
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
                                             RutineCopyArray                 */

__dhp __rca (__arrp a)
{
  register __dhp *p,
   *q,
   *r;				/* Brukes for } kopiere array objektet */
  __sl = (__dhp) a;
  q = (__dhp *) (__er = (__dhp) __ralloc (((long) a->h.size)));
  a = (__arrp) __sl;
  p = (__dhp *) a;
  r = (__dhp *) (((char *) q) + a->h.size);
  while ((char *) q < (char *) r)
    *q++ = *p++;
  return (__er);
}
