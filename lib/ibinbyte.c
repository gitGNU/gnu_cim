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
                                         (SHORT) INTEGER PROCEDURE INBYTE    */

long 
__ribinbyte (p)
     __bs8FILE *p;
{
  long c;
  if (((__bs7FILE *) p)->endfile == __TRUE)
    __rerror ("Inbyte: End of file");
  if ((c = getc (((__bs1FILE *) p)->file)) == __EOF)
    {
      ((__bs7FILE *) p)->endfile = __TRUE;
      return (0);
    }
  else
    return (c);
}
