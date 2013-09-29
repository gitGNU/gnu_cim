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
                                         (SHORT) INTEGER PROCEDURE INBYTE    */

long __rdbinbyte (__bs10FILE *p)
{
  long c;
  if (p->writeonly)
    __rerror ("Inbyte: Writeonly file");
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inbyte: File closed");
  if (p->lastop == __WRITE 
      && fseek (((__bs1FILE *) p)->file, p->loc - 1, 0) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __READ;
  p->loc++;
  if ((c = getc (((__bs1FILE *) p)->file)) == __EOF)
    return (0);
  else
    return (c);
}
