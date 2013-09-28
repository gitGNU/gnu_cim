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

#include "random.h"

/******************************************************************************
                                                      PROCEDURE HISTO	     */

void __rhisto (__arrp A, __arrp B, double c, double d)
{
  long j;
  if (A->h.dim != 1 || B->h.dim != 1)
    __rerror ("Histo: Multi dimensional array");
  if (A->limits[0].size != B->limits[0].size + 1)
    __rerror ("Histo: Illegal size of arrays");
  for (j = 0; j < B->limits[0].size; j++)
    if (c <= ARRELEM (B, j))
      break;
  ARRELEM (A, j) += d;
}
