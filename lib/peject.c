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
                                           PROCEDURE EJECT                   */

/* N}r det skal lages et bibliotek kompileres denne rutinen alltid sammen
 * med den neste da de avhenger av hverandre */

__dhp __rpeject (__bs6FILE *p, long n)
{
  long i;

  if (!((__bs1FILE *) p)->open)
    __rerror ("Eject: File not open");
  if (n <= 0)
    __rerror ("Eject: Parameter out of range");
  if (n > p->lines_per_page)
    n = 1;

  if (n <= p->line)
    {
      (void) putc ('\f', ((__bs1FILE *) p)->file);
      p->line = 1;
      p->page += 1;
    }
  for (i = p->line; i < n; i++)
    (void) putc ('\n', ((__bs1FILE *) p)->file);
  p->line = n;
  return ((__dhp) p);
}
