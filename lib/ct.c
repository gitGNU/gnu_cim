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


/******************************************************************************
                                                            RutineCallThunk  */

/* Denne rutinen oppretter et thunk-objekt, og setter dens statiske og
 * dynamiske omgivelse. I tillegg kalles rss hvis det ligger noe p} en av rt-
 * stakkene. Den dynamiske omgivelsen er gitt av __pb mens den statiske
 * er overf|rt i den globale variablen sl.
 *
 * Parametere:
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * Returnerer: Ingen ting. */

void
__rct (as)
     long as;
{
  __dhp x;
  if (as)
    __rss (as);
  x = __ralloc ((long) sizeof (__thunk));
  x->pp = (__pty) __THUNK;
  x->sl = __sl;
  x->dl = __pb;
  /* p->pm=__FALSE; Er implisitt satt */
  __pb = x;
}
