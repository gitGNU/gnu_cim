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
                                                        RutineCall           */

void
__rcall (ob, ret, mret)
     __dhp ob;
     int ret;
     void (*mret) ();
{
  if (ob == __NULL)
    __rerror ("Call: NONE-pointer");
  if (ob->dt != __DETACHED)
    __rerror ("Call: The object is not detached");
  ob->dt = __EXECUTING;
  __goto = ob->ex;
  ob->ex.ent = ret;
  ob->ex.ment = mret;
  __lb = ob->dl;
  ob->dl = __pb;
  __pb = __lb;
}
