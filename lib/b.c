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
                                                                RutineBlock  */

void
__rb (ppx)
     __pty ppx;
{
  __dhp x;
  x = __ralloc ((long) ppx->size);
  x->pm = __FALSE;
  /* x->dt=__executing; Er implisitt satt */
  x->pp = ppx;
  x->sl = x->dl = __lb;
  __lb = __pb = x;
  /* __lb->ex.ent=ret;  trengs ikke settes */
}
