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
                                                 PROCEDURE INIMAGE           */

__dhp __riinimage (__bs2FILE *p)
{
  long i,
    j;
  FILE *f;
  char *s;
  long c;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inimage: File not open");
  if (((__bs4FILE *) p)->endfile)
    __rerror ("Inimage: End of file");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Inimage: IMAGE equals notext");
  if (((__bs2FILE *) p)->IMAGE.obj->h.konstant)
    __rerror ("Inimage: Constant text object");

  f = ((__bs1FILE *) p)->file;
  s = p->IMAGE.obj->string;
  i = p->IMAGE.start - 1;
  j = i + p->IMAGE.length;

  while ((c = getc (f)) != __EOF)
    {
      if (c == '\n')
	goto utinimage;
      if (i == j)
	goto inimageerror;
      s[i++] = c;
    }

  if (i == p->IMAGE.start - 1)
    {
      ((__bs4FILE *) p)->endfile = __TRUE;
      s[i++] = 25;
      for (; i < j; i++)
	s[i] = ' ';
      p->IMAGE.pos = 1;
      return ((__dhp) p);
    }
  else
    (void) ungetc ((int) c, f);

utinimage:
  while (i != j)
    s[i++] = ' ';
  p->IMAGE.pos = 1;
  return ((__dhp) p);
inimageerror:
  __rerror ("Inimage: IMAGE to short");
}
