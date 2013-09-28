/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen and Terje Mjoes,
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
#include "limit.h"

/******************************************************************************
                                   TEXT PROCEDURE BLANKS                     */

__txtvp __rblanks (long as, long n)
{
  long pos;
  if (n < 0)
    __rerror ("Blanks: Parameter lesser than zero");
  if (n >= MAX_TEXT_CHAR)
    __rerror ("Blanks: Parameter too high");
  if (n == 0)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __as = as;
      __et.obj = (__textref) __ralloc (((long) sizeof (__th) + n + 1));
      /* En ekstra -> |nsker plass  */
      ((__textref) __et.obj)->h.size = __et.length = n;		/* '\0' p}
								 * slutten av 
								 * og til  */
      ((__textref) __et.obj)->h.konstant = __FALSE;
      ((__textref) __et.obj)->h.pp = (__pty) __TEXT;
      __as = 0;
      __et.start = __et.pos = 1;
      for (pos = 0; pos < n; pos++)
	((__textref) __et.obj)->string[pos] = ' ';
      ((__textref) __et.obj)->string[pos] = '\0';	/* Avslutter med \0 */
    }
  return (&__et);
}
