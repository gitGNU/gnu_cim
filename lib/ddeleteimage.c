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
                                         BOOLEAN PROCEDURE DELETEIMAGE       */

char 
__rddeleteimage (p)
     __bs5FILE *p;
{
  long c1,
    i,
    l,
    last;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Deleteimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Deleteimage: IMAGE equals notext");
  if (p->loc > ((__bs5FILE *) p)->maxloc)
    __rerror ("Deleteimage: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Deleteimage: Append underflow or read-only file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Deleteimage: Illegal length of image");
  if (p->loc > (last = __rdlastloc (p)))
    return (__FALSE);
  if (p->lastop == __READ && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
    __rerror ("Deleteimage: Not possible to seek");
  p->lastop = __WRITE;

  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;
  for (i = 1; i <= l; i++)
    {
      if (putc ('\0', f) == __EOF)
	{
	  __rerror ("Deleteimage: Write error");
	  break;
	}
    }
  if (putc ('\0', f) == __EOF)
    __rerror ("Deleteimage: Write error");

  ((__bs2FILE *) p)->IMAGE.pos = 1;
  if (p->loc == last)
    {
      for (i = p->loc - 1; i > 0; i--)
	{
	  if (fseek (((__bs1FILE *) p)->file, 
		     (((__bs5FILE *) p)->imagelength + 1) * i - 1, 0) == __EOF)
	    __rerror ("Deleteimage: Not possible to seek");
	  if ((c1 = getc (f)) == __EOF)
	    __rerror ("Deleteimage: Read error");
	  if (c1 != '\0')
	    break;
	}
      (void) fflush (f);
#if F_TRUNCATE
#ifdef mips			/* Just to satisfy lint */
      (void) ftruncate ((int) fileno (f), (int) ((i) * (l + 1)));
#else
      (void) ftruncate ((int) fileno (f), ((i) * (l + 1)));
#endif
#endif
      if (i != p->loc - 1)
	{
	  if (fseek (((__bs1FILE *) p)->file, 
		     (((__bs5FILE *) p)->imagelength + 1) * p->loc,
		     0) == __EOF)
	    __rerror ("Deleteimage: Not possible to seek");
	  p->lastop = __SEEK;
	}
    }
  p->loc++;
  return (__TRUE);
}
