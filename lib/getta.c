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
                                                    RutineGetTextAdress      */

char
__rgetta (p, as, ret, mret)
     __textnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->h.namekind)
    {
    case __VALUE_THUNK:
      __goto = p->h.adr;	/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse 
				 * verdiene f|r kallet. */
      __sl = p->h.sl;
      __rct (as);		/* Oppretter objektet og overf|rer
			         * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret; 
      
      __lb = __pb; 		/* Gj|r thunken eksikverbar.  */
      return (__TRUE);
    case __ADDRESS_THUNK:
      __goto = p->h.adr;	/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse 
				 * verdiene f|r kallet. */
      __sl = p->h.sl;
      __rct (as);		/* Oppretter objektet og overf|rer
			         * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret; 
      ((__thunkp) __pb)->writeaccess = __TRUE;
      __lb = __pb; 		/* Gj|r thunken eksikverbar.  */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __er = p->h.bp;
      __ev.i = p->h.v.ofs;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    case __VALUE_NOTHUNK:
      __et = *p->tp;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
