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
#include "simfile.h"

/******************************************************************************
                                                        RutineGoto           */

void
__rgoto (ob)
     __dhp ob;
{
  __dhp x;

  __lb = ob;
  while (__pb != __lb)
    {				
      /* Hvis ikke __lb ligger p} operasjonskjeden s} skal goto 
       * resultere i en feil. Det må sjekkes her. */

      if (__pb == (__dhp) & __blokk0FILE)
	__rerror ("Goto: Label not on operating chain");

      if (__pb->pp == (__pty) __ACTS || __pb->pp->kind != __KCLASS)
	__pb = __pb->dl;
      else if (__pb->pp->kind == __KCLASS)
	{
	  (x = __pb)->dt = __TERMINATED;
	  __pb = __pb->dl;
	  x->dl = x;
	}
    }
}
