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
#include "align.h"

/******************************************************************************
                                                        RutineEndThunk       */

/* Denne rutinen kalles for aa terminere en thunk og pr|ver } deallokere
 * objektet. */

void
__reth ()
{
  __dhp x;
  __goto = __pb->ex;
  x = __pb;
  __pb = __pb->dl;
  if (__pb->pp == (__pty) __ACTS)
    __rrs ();
  __lb = __pb;
  while (__lb->pm || __lb->pp == (__pty) __ACTS)
    __lb = __lb->dl;
  if ((char *) __fri == (((char *) x) + align (sizeof (__thunk))))
    {
      memset ((char *) x, 0, (char *) __fri - (char *) x);
      __fri = x;
    }
}
