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

#define INCLUDE_SIMFILE_H
#include "cim.h"

/******************************************************************************
                                         PROCEDURE OUTBYTE(x)                */

__dhp __rdboutbyte (__bs10FILE *p, long x)
{
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outbyte: File closed");
  if ((x < 0) | (x >= 256))
    __rerror ("Outbyte: Illegal byte value");
  if (p->loc > p->maxloc)
    __rerror ("Outbyte: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Outbyte: Append underflow or read-only file");
  if (p->lastop == __READ 
      && fseek (((__bs1FILE *) p)->file, p->loc - 1, 0) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __WRITE;
  (void) putc ((char) x, ((__bs1FILE *) p)->file);
  p->loc++;
  return ((__dhp) p);
}
