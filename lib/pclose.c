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
                                         BOOLEAN PROCEDURE CLOSE             */

char 
__rpclose (p)
     __bs6FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      if (((__bs2FILE *) p)->IMAGE.pos > 1)
	__rpoutimage (p);
      if (p->lines_per_page != MAX_INT)
	__rpeject (((__bs6FILE *) p), p->lines_per_page);
      p->line = 0;
      p->spacing = 1;
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      ((__bs1FILE *) p)->open = __FALSE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
