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
                                                    RutineGetLabel           */

/* Denne rutinen kalles ved aksess av en label name-parameter.
 * Rutinen avgj|r om en thunk skal kalles eller ikke p} grunnlag av
 * p->namekind attributtet, som kan ha to verdier: ADDRESS_THUNK og
 * ADDRESS_NOTHUNK.
 * Hvis en thunk skal kalles, opprettes et objekt og returadressen legges i
 * objektet. Adressen til labelen  returneres i modul og __ev,
 * mens objekt-pekeren hvor den er deklarert returneres i er.
 * Hvis ikke en thunk skal kalles s} er labelen gitt av p->adr.ment og
 * p->adr.ent samt p->ob. Disse tilordnes ogs} henholdsvis modul, __ev  og er.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret -> Returadressen
 * Returnerer: Adressen til labelen i modul og __ev og objekt-pekeren i er */

char
__rgetlab (p, as, ret, mret)
     __labelnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->namekind)
    {
    case __ADDRESS_THUNK:
      __goto = p->adr;		/* I tilfelle at kallet p} ct forer til
                                 * garbage collection, slik  at p ikke
                                 * lenger peker riktig, leses disse
                                 * verdiene forer kallet. */
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet og overf|rer 
				 * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __ev.adr = p->adr;
      __er = p->ob;
      __swv = p->swv;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
