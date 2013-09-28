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
                                                        RutineResume         */

void __rresume (__dhp ob, int ret, void (*mret) ())
{
  __dhp sh;

  if (ob == __NULL)
    __rerror ("Resume: NONE-pointer");
  if (ob->dt == __RESUMED)
    {
      __goto.ent = ret;
      __goto.ment = mret;
      return;
    }

  if (ob->sl->pp->kind != __KPRBLK && ob->sl->pp->kind != __KBLOKK)
    __rerror ("Resume: Illegal object");

  if (ob->dt == __TERMINATED || ob->dt == __EXECUTING)
    __rerror ("Resume: Object is executing or terminated");

  sh = __NULL;
  while (__lb != ob->sl)
    {
      if (__lb == (__dhp) & __blokk0FILE)
	__rerror ("Resume: Illegal object");
      sh = __lb;
      if (__lb->dt == __EXECUTING || __lb->pp == (__pty) __ACTS)
	__lb = __lb->dl;
      else
	__lb = __lb->sl;
    }

  if (sh == __NULL || (sh->pp != (__pty) __THUNK && sh->pp != (__pty) __ACTS &&
		     sh->pp->kind != __KCLASS))
    {				/* __lb peker p} system-hodet. */
      __goto = ob->ex;
      __lb = ob->dl;
      ob->dl = __pb;
      ob->ex.ent = ret;
      ob->ex.ment = mret;
      ob->dt = __RESUMED;
      __pb = __lb;
    }
  else
    {
      /* Object-component, som sh peker p} */
      __goto = ob->ex;
      __lb = ob->dl;
      if (sh->dt == __RESUMED)
	{
	  ob->dl = sh->dl;
	  ob->ex = sh->ex;
	  sh->dl = __pb;
	  sh->ex.ent = ret;
	  sh->ex.ment = mret;
	  sh->dt = __DETACHED;
	}
      else
	{
	  ob->dl = __pb;
	  ob->ex.ent = ret;
	  ob->ex.ment = mret;
	}
      ob->dt = __RESUMED;
      __pb = __lb;
    }
}
