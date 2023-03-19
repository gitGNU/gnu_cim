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
#include <stdio.h>

/******************************************************************************
                                        INTEGER PROCEDURE LASTLOC            */

long __rdlastloc (__bs5FILE *p)
{
  long pos,
    len;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Lastloc: File not open");
  pos = ftell (((__bs1FILE *) p)->file);
  fseek (((__bs1FILE *) p)->file, 0, 2);
  len = ftell (((__bs1FILE *) p)->file);
  fseek (((__bs1FILE *) p)->file, pos, 0);
  return (len / (((__bs5FILE *) p)->imagelength + 1));
}
