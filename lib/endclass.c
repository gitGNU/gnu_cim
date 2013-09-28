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
                                                        RutineEndClass       */

void __rendclass (int plev)
{
  __dhp p;
  if (plev == 0)
    {
      __goto = __pb->ex;
      __pb->dt = __TERMINATED;
      __er = __lb;
      __pb = __pb->dl;
      __lb->dl = __lb;
      if (__pb->pp == (__pty) __ACTS)
	__rrs ();
      __lb = __pb;
      while (__lb->pm || __lb->pp == (__pty) __ACTS)
	__lb = __lb->dl;
    }
  else
    {
      __goto = __pb->pp->pref[plev - 1]->adr;
      __goto.ent += 2;
    }
}
