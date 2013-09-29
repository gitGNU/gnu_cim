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
#include "file.h"

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

char __ribopen (__bs8FILE *p)
{
  if (!((__bs1FILE *) p)->open)
    {
      ((__bs1FILE *) p)->file 
	= fopen (((__bs1FILE *) p)->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs7FILE *) p)->endfile = __FALSE;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
