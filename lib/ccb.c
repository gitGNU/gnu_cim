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
                                                RutineCallClassBegin         */

void
__rccb (ret, mret)
     int ret;
     void (*mret) ();
{
/* __pb->dt=__executing; Er implisitt satt */
  __pb->pm = __FALSE;
  __goto = __pb->pp->pref[0]->adr;
  __pb->ex.ent = ret;
  __pb->ex.ment = mret;

  __lb = __pb;
}
