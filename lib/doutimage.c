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
                                         PROCEDURE OUTIMAGE                  */

__dhp 
__rdoutimage (p)
     __bs5FILE *p;
{
  char *c;
  long i,
    l;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Outimage: IMAGE equals notext");
  if (p->loc > ((__bs5FILE *) p)->maxloc)
    __rerror ("Outimage: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Outimage: Append underflow or read-only file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Outimage: Illegal length of image");
  if (p->lastop == __READ && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __WRITE;

  c = &((__bs2FILE *) p)->IMAGE.obj->string[((__bs2FILE *) p)
					    ->IMAGE.start - 1];
  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;
  for (i = 1; i <= l; i++)
    {
      if (putc (*c, f) == __EOF)
	{
	  __rerror ("Outimage: Write error");
	  break;
	}
      *(c++) = ' ';
    }
  if (putc ('\n', f) == __EOF)
    __rerror ("Outimage: Write error");

  p->loc++;
  ((__bs2FILE *) p)->IMAGE.pos = 1;
  return ((__dhp) p);
}
