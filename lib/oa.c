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
#include "align.h"

/******************************************************************************
                                                     RutineOnlyArrays        */

/* Denne rutinen kalles fra __rep(p) og __rbe -- retur fra prosedyre og blokk.
 * Den sjekker om det p} toppen av poolen over blokk-instansen for den
 * terminerende prosedyre eller blokk kun ligger array-objekter.
 * Hvis det er tilfellet, m} disse v{re generert av denne prosedyren/blokken,
 * og de kan deallokeres.
 * Array objektene kan ikke v{re referert av andre 
 * (klasse objekter der arrayen er gitt som parameter)
 * fordi vi da ville funnet disse objektene p} poolen. 
 * Array objektene kan ha v{rt referert av andre prosedyrer,
 * men de har terminert og blitt deallokert.
 * Hvis den kun finner array-objekter returnerer rutinen TRUE eller FALSE. */

__roa (x)
     __dhp x;
{
  if (x < __min)
    return (__FALSE);
  while (x < __fri && x->pp == (__pty) __ARRAY)
    x = (__dhp) ((char *) x + align (((__ahp) x)->size));
  return (x == __fri);
}

