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
                                                    RutineGetSimpleparAdress */

/* Denne rutinen kalles ved skrive-aksess av enkle name-parametere. Rutinen
 * avgj|r om en thunk skal kalles eller ikke p} bakgrunn av p->namekind
 * attributtet. Den gir ogs} feilmelding hvis aktuell parameter ikke kan
 * tilordnes.
 * Hvis en thunk skal kalles, opprettes et objekt og f|lgende verdier legges i
 * objektet: returadressen. Thunken returnerer med adressen i ef(base pointer)
 * og __ev.i (offset).
 * Hvis ikke en thunk skal kalles s} er adressen gitt direkte av p->bp og
 * p->v.ofs, som ogs} blir lagt ned i ef og __ev.i.
 *
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret -> Returadressen
 * Returnerer: Adressen i er og __ev (også hvis en thunk kalles) */

char __rgetsa (__simplenamepar *p, long as, int ret, void (*mret) ())
{
  switch (p->namekind)
    {
    case __VALUE_NOTHUNK:
    case __VALUE_THUNK:
      __rerror ("Getsa: Write access on name parameter");
      break;
    case __ADDRESS_THUNK:
      __goto = p->adr;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse 
				 * verdiene f|r kallet. */
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet og
				 * overf|re returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      ((__thunkp) __pb)->writeaccess = __TRUE;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __er = p->bp;
      __ev.i = p->v.ofs;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
