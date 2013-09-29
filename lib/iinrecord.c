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
                                         BOOLEAN PROCEDURE INRECORD          */

char __riinrecord (__bs2FILE *p)
{
  long i,
    j;
  FILE *f;
  char *s;
  long c;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inrecord: File not open");
  if (((__bs4FILE *) p)->endfile)
    __rerror ("Inrecord: End of file");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Inrecord: IMAGE equals notext");

  f = ((__bs1FILE *) p)->file;
  s = p->IMAGE.obj->string;
  i = p->IMAGE.start - 1;
  j = i + p->IMAGE.length;

  while ((c = getc (f)) != __EOF)
    {
      if (c == '\n')
	goto utinrecord;
      if (i == j)
	{
	  (void) ungetc ((int) c, f);
	  goto inrecordfull;
	}
      s[i++] = c;
    }

  if (i == p->IMAGE.start - 1)
    {
      ((__bs4FILE *) p)->endfile = __TRUE;
      s[i] = 25;
      p->IMAGE.pos = 2;
      return (__FALSE);
    }
  else
    (void) ungetc ((int) c, f);

utinrecord:
  p->IMAGE.pos = i - p->IMAGE.start + 2;
  return (__FALSE);
inrecordfull:
  p->IMAGE.pos = p->IMAGE.length + 1;
  return (__TRUE);
}
