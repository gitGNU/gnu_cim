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
#include "simfile.h"
/******************************************************************************
                                         PROCEDURE OUTBYTE(x)                */

__dhp 
__roboutbyte (p, x)
     __bs9FILE *p;
     long x;
{
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outbyte: File closed");
  if ((x < 0) | (x >= 256))
    __rerror ("Outbyte: Illegal byte value");
  (void) putc ((int) x, ((__bs1FILE *) p)->file);
  return ((__dhp) p);
}
