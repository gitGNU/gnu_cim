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
                                                        Rutine for t1 <= t2  */

char __rletext (__txtvp t1x, __txtvp t2x)
{
  long i = 0,
    j = 0;
  register unsigned char *p1x,
   *p2x;
  if (t1x->obj == __NULL && t2x->obj == __NULL)
    return (__TRUE);
  if (t2x->obj == __NULL)
    return (__FALSE);

  p1x = (unsigned char *) &(t1x->obj->string[t1x->start - 1]);
  p2x = (unsigned char *) &(t2x->obj->string[t2x->start - 1]);

  while (i < t1x->length && j < t2x->length)
    {
      i++;
      j++;
      if (*p1x > *p2x)
	return (__FALSE);
      if (*p1x++ < *p2x++)
	return (__TRUE);
    }
  if (i == t1x->length && j == t2x->length)
    return (__TRUE);		/* Like */
  if (j == t2x->length)
    return (__FALSE);		/* t2 < t1 */
  return (__TRUE);
}
