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
                                                 rtextvalassign              */

__txtvp 
__rtextvalassign (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  long i = 0;
  register char *p1x,
   *p2x;

  if ((t1x->obj == __NULL && t2x->obj != __NULL) || (t1x->length < t2x->length))
    __rerror ("Textvalassign: Text area too small");

  if (t1x->obj == __NULL)
    return (t1x);

  if (t1x->obj->h.konstant)
    __rerror ("Textvalassign: Attempt to alter constant text");

  p1x = (char *) &(t1x->obj->string[t1x->start - 1]);

  if (t2x->obj != __NULL)
    for (p2x = (char *) &(t2x->obj->string[t2x->start - 1]); i < (long) t2x->length; i++)
      *p1x++ = *p2x++;

  for (i = 0; i < (long) t1x->length - (long) t2x->length; i++)
    *p1x++ = ' ';

  return (t1x);
}
