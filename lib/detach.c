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

#define INCLUDE_SIMFILE_H
#include "cim.h"

/******************************************************************************
                                                        RutineDetach         */

void __rdetach (__dhp ob, int ret, void (*mret) ())
{
  __dhp p;
  if (ob->pp->kind == __KPRBLK)
    {
      __goto.ent = ret;
      __goto.ment = mret;
      return;
    }
  if (ob->dt == __TERMINATED)
    __rerror ("Detach: Not operating object");
  if (ob->dt == __DETACHED)
    __rerror ("Detach: Already detached object");

  /* Sjekker om ob ligger p} operasjonskjeden. */
  p = __lb;
  while (p != ob)
    {
      if (p == (__dhp) & __blokk0FILE)
	__rerror ("Detach: The object is not on the operating chain");
      if (p->dt == __RESUMED)
	p = p->sl;
      else
	p = p->dl;
    }

  __goto = ob->ex;
  ob->ex.ent = ret;
  ob->ex.ment = mret;
  __er = ob;
  ob->dt = __DETACHED;
  __lb = ob->dl;
  ob->dl = __pb;
  __pb = __lb;

  if (__pb->pp == (__pty) __ACTS)
    {
      __rrs ();
      __lb = __pb;
    }
  while (__lb->pm || __lb->pp == (__pty) __ACTS)
    __lb = __lb->dl;
}
