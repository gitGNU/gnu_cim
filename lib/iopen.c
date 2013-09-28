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

char __riopen (__bs1FILE *p, __txtvp t)
{
  long i;
  char *s;

  if (!p->open)
    {
      /* Trenger ikke ha med p->filename.start fordi start=1 siden */
      /* filename er overf|rt by value                             */

      ((__bs1FILE *) p)->file = fopen (p->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs4FILE *) p)->endfile = __FALSE;
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = t->length + 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      s = ((__bs2FILE *) p)->IMAGE.obj->string;
      for (i = 0; i < t->length; i++)
	s[t->start + i - 1] = ' ';
      return ((p->open = __TRUE));
    }
  else
    return (__FALSE);
}
