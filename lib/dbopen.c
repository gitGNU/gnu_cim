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

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

char 
__rdbopen (p)
     __bs10FILE *p;
{
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
	   = fopen (((__bs1 *) p)->filename.obj->string
		    ,"r+")) == __NULL)
	return (__FALSE);
#else
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
		    ,((__bs1FILE *) p)->file == __NULL ? "w+" :
		    ((__bs1FILE *) p)->readwrite == __READONLY 
		    ? "r" : "r+")) == __NULL)
	return (__FALSE);
#endif
      if (((__bs1FILE *) p)->append == __APPEND)
	{
	  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
	    {
	      (void) fclose (((__bs1FILE *) p)->file);
	      return (__FALSE);
	    }
	  p->loc = p->minwriteloc = ftell (((__bs1FILE *) p)->file);
	}
      else
	p->loc = p->minwriteloc = 1;
      p->lastop = __SEEK;
      p->maxloc = MAX_INT - 1;
      if (((__bs1FILE *) p)->readwrite == __READONLY)
	p->minwriteloc = MAX_INT;
      if (((__bs1FILE *) p)->readwrite == __WRITEONLY)
	p->writeonly = __TRUE;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
