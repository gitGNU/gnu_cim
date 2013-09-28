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

__dhp __rooutimage (__bs2FILE *p)		/* Skriver ikke ut etterfolgende blanke */
{
  char *s;
  long i;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outimage: File not open");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Outimage: IMAGE equals notext");
  (void) __rtstrip (&((__bs2FILE *) p)->IMAGE);		/* Lengden av teksten 
							 * uten etterfolgende 
							 * blanke  */
  /* ligger i den globale variabelen et.length og  */
  /* start som ikke er forandret) ligger i et.start */
  s = p->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  for (i = 0; i < __et.length; i++)
    {
      (void) putc (s[__et.start + i - 1], f);
      s[__et.start + i - 1] = ' ';
    }
  (void) putc ('\n', f);
  (void) fflush (f);
  p->IMAGE.pos = 1;
  return ((__dhp) p);
}
