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

#include "cim.h"

/******************************************************************************
                                         BOOLEAN PROCEDURE LASTITEM          */

char 
__rilastitem (p)
     __bs2FILE *p;
{
  char c;
  if (p->IMAGE.obj == __NULL)
    return (__TRUE);
  c = ' ';
  while (!(((__bs1FILE *) p)->h.pp == &__p5FILE
	   ? ((__bs5FILE *) p)->endfile : ((__bs4FILE *) p)->endfile)
	 && (c == ' ' || c == 9))
    c = __riinchar (p);

  if (c != ' ')
    p->IMAGE.pos--;

  return (((__bs1FILE *) p)->h.pp == &__p5FILE
	  ? ((__bs5FILE *) p)->endfile : ((__bs4FILE *) p)->endfile);
}
