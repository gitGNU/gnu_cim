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

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

char 
__rpopen (p, t)
     __bs6FILE *p;
     __txtvp t;
{
  if (!((__bs1FILE *) p)->open)
    {
      /* Trenger ikke ha med p->filename.start fordi start=1 siden */
      /* filename er overf|rt by value                             */

      if (((__bs1FILE *) p)->create != __ANYCREATE)
	{
	  ((__bs1FILE *) p)->file 
	    = fopen (((__bs1FILE *) p)->filename.obj->string, "r");
	  if (((__bs1FILE *) p)->file != __NULL)
	    (void) fclose (((__bs1FILE *) p)->file);
	  if ((((__bs1FILE *) p)->file == __NULL 
	       && ((__bs1FILE *) p)->create == __NOCREATE) ||
	      (((__bs1FILE *) p)->file != __NULL 
	       && ((__bs1FILE *) p)->create == __CREATE))
	    return (__FALSE);
	}
      if (((__bs1FILE *) p)->append == __APPEND)
	((__bs1FILE *) p)->file 
	  = fopen (((__bs1FILE *) p)->filename.obj->string, "a");
      else
	((__bs1FILE *) p)->file 
	  = fopen (((__bs1FILE *) p)->filename.obj->string, "w");

      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      p->page = 1;
      p->line = 1;
      ((__bs1FILE *) p)->open = __TRUE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
