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
                                        PROCEDURE OUTRECORD                  */

__dhp __rpoutrecord (__bs6FILE *p)
{
  char *s;
  register long i,
    j,
    k;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outrecord: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Outrecord: IMAGE equals notext");
  if (p->line > p->lines_per_page)
    __rpeject (p, 1L);
  s = ((__bs2FILE *) p)->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  j = ((__bs2FILE *) p)->IMAGE.pos - 1;
  k = ((__bs2FILE *) p)->IMAGE.start - 1;
  for (i = 0; i < j; i++)
    (void) putc (s[k + i], f);
  for (i = 1; i <= p->spacing; i++)
    {
      (void) putc ('\n', f);
      p->line++;
      if (p->line > p->lines_per_page)
	break;
    }
  if (p->spacing == 0)
    (void) putc ('\r', f);
  p->line += p->spacing;
  ((__bs2FILE *) p)->IMAGE.pos = 1;
  return ((__dhp) p);
}
