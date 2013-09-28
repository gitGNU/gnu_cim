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
#include "limit.h"

/******************************************************************************
                                            INTEGER PROCEDURE ENTIER(r)      */

long __rentier (double r)
{
  long j;
  if ((r >= ((double) MAX_INT) + 1.0) ||
      (r < -((double) MAX_INT) + 1.0))
     __rerror ("Entier: Argument out of integer bounds");
  j = r;
  return ((j > r) ? j - 1 : j);
}
