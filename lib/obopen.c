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

#include "file.h"
#include "simfile.h"

/* Outbytefile */

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

char __robopen (__bs9FILE *p)
{
  if (!((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->create != __ANYCREATE)
	{
	  ((__bs1FILE *) p)->file =
	    fopen (((__bs1FILE *) p)->filename.obj->string, "r");
	  if (((__bs1FILE *) p)->file != __NULL)
	    (void) fclose (((__bs1FILE *) p)->file);
	  if (((__bs1FILE *) p)->file == __NULL
	      && ((__bs1FILE *) p)->create == __NOCREATE ||
	      ((__bs1FILE *) p)->file != __NULL
	      && ((__bs1FILE *) p)->create == __CREATE)
	    return (__FALSE);
	}
      ((__bs1FILE *) p)->file =
	fopen (((__bs1FILE *) p)->filename.obj->string, 
	       ((__bs1FILE *) p)->append == __APPEND ? "a" : "w");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
