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
                                         PROCEDURE INIMAGE                   */

__dhp __rdinimage (__bs5FILE *p)
{
  char *c,
    c1,
    d;
  long i,
    l;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Inimage: IMAGE equals notext");
  if (((__bs2FILE *) p)->IMAGE.obj->h.konstant)
    __rerror ("Inimage: IMAGE is a constant text object");
  if (p->writeonly)
    __rerror ("Inimage: Writeonly file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Inimage: Illegal length of IMAGE");
  ((__bs2FILE *) p)->IMAGE.pos = 1;

  c = &((__bs2FILE *) p)->IMAGE.obj->string[((__bs2FILE *) p)
					    ->IMAGE.start - 1];
  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;

  if (((__bs5FILE *) p)->endfile = (((__bs5FILE *) p)->loc > __rdlastloc (p)))
    {
      *(c++) = 25;
      for (i = 2; i <= l; i++)
	*(c++) = ' ';
    }
  else
    {
      if (p->lastop == __WRITE 
	  && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
	__rerror ("Inimage: Not possible to seek");
      p->lastop = __READ;
      d = '\0';
      for (i = 1; i <= l; i++)
	{
	  if ((c1 = getc (f)) == __EOF)
	    {
	      __rerror ("Inimage: Read error");
	      break;
	    }

	  if ((*(c++) = c1) != '\0')
	    d = '\n';
	}
      if ((c1 = getc (f)) != '\n' && c1 != d)
	__rerror ("Inimage: Read error");
      p->loc++;
    }
  return ((__dhp) p);
}
