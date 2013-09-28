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
                                                 Runtine for = og <> testen  */

char __reqtext (__txtvp t1x, __txtvp t2x)
{
  long i = 0;
  register char *p1x,
   *p2x;
  if (t1x->obj == __NULL && t2x->obj == __NULL)
    return (__TRUE);
  if (t1x->obj == __NULL || t2x->obj == __NULL)
    return (__FALSE);
  if (t1x->length != t2x->length)
    return (__FALSE);

  p1x = (char *) &(t1x->obj->string[t1x->start - 1]);
  for (p2x = (char *) &(t2x->obj->string[t2x->start - 1]); i < (long) t2x->length; i++)
    if (!(*p1x++ == *p2x++))
      return (__FALSE);

  return (__TRUE);
}
