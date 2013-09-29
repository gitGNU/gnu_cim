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

char __rdopen (__bs5FILE *p, __txtvp t)
{
  long loc;
  if (!((__bs1FILE *) p)->open)
    {
      ((__bs1FILE *) p)->file 
	= fopen (((__bs1FILE *) p)->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file != __NULL)
	(void) fclose (((__bs1FILE *) p)->file);
      if (((__bs1FILE *) p)->create == __CREATE 
	  && ((__bs1FILE *) p)->file != __NULL)
	return (__FALSE);
      if (((__bs1FILE *) p)->create == __NOCREATE 
	  && ((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
#if VMS_DIRECTFILE
      if (((__bs1FILE *) p)->file == __NULL)
	{
	  if ((((__bs1FILE *) p)->file 
	       = fopen (((__bs1FILE *) p)->filename.obj->string
			,"w")) == __NULL)
	    return (__FALSE);
	  (void) fclose (((__bs1FILE *) p)->file);
	}
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
					    ,"r+")) == __NULL)
	return (__FALSE);
#else
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
				 ,((__bs1FILE *) p)->file == __NULL ? "w+" :
	((__bs1FILE *) p)->readwrite == __READONLY ? "r" : "r+")) == __NULL)
	return (__FALSE);
#endif
      if (((__bs1FILE *) p)->append == __APPEND)
	{
	  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
	    {
	      (void) fclose (((__bs1FILE *) p)->file);
	      return (__FALSE);
	    }
	  loc = ftell (((__bs1FILE *) p)->file);
	}
      else
	loc = 0;
      p->loc = loc / (t->length + 1) + 1;
      if (loc % (t->length + 1))
	__rerror ("Open: Illegal size on direct-file");
      p->minwriteloc = p->loc;
      p->lastop = __SEEK;
      p->maxloc = (MAX_INT - 1) / (t->length + 1);
      if (((__bs1FILE *) p)->readwrite == __READONLY)
	p->minwriteloc = MAX_INT;
      if (((__bs1FILE *) p)->readwrite == __WRITEONLY)
	p->writeonly = __TRUE;
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      ((__bs5FILE *) p)->endfile = __FALSE;
      ((__bs5FILE *) p)->imagelength = t->length;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
