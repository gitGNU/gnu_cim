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
#include "align.h"

/******************************************************************************
                                                  RutineEndProperProcedure   */

void
__repp ()
{
  __dhp x;
  long size;
  __goto.ent = __pb->ex.ent;
  __goto.ment = __pb->ex.ment;

  x = __pb;
  size = __pb->pp->size;

  __lb = __pb = __pb->dl;

  if ((char *) __fri == (((char *) x) + align (size)) 
      || __roa ((__dhp) ((char *) x + align (size))))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = x;
    }
}
