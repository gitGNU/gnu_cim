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
                                                        RutineExchange       */

void __rexchange (__dhp sh, __dhp ob, int ret, void (*mret) ())
{
  __goto = ob->ex;
  __lb = ob->dl;
  ob->dl = sh->dl;
  ob->ex = sh->ex;
  ob->dt = __RESUMED;
  sh->dl = __pb;
  sh->ex.ent = ret;
  sh->ex.ment = mret;
  sh->dt = __DETACHED;
  __pb = __lb;
}
