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
                                                     RADDROFFIRSTELEM        */

/* Denne rutinen brukes for } overf|re en array by reference til en
 * C-prosedyre. Rutinen returnerer med en peker til f|rste element */

char *__raddroffirstelem (__arrp p)
{
  return ((char *) ((long) p + sizeof (__ah) + 
		    sizeof (__arrlimit) * p->h.dim));
}
