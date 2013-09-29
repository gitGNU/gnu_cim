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
#include "cim.h"

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

char __rdclose (__bs5FILE *p)
{
  if (((__bs1FILE *) p)->open)
    {
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      if (p->locked)
	(void) __rdunlock (p);
      p->loc = p->maxloc = 0;
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs1FILE *) p)->open = __FALSE;
      return (p->endfile = __TRUE);
    }
  else
    return (__FALSE);
}
