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
                                                    RutineGetTextValue       */

/* Denne rutinen kalles ved lese-aksess av enkle name-parametere av type text
 * Rutinen avgj|r om en thunk skal kalles eller ikke p} grunnlag av
 * p->h.namekind attributtet.
 * Hvis en thunk skal kalles, opprettes et objekt og returadressen legges i
 * objektet. Det returneres en peker til Text-Verdien, enten den ligger i __et
 * eller i den aktuelle text-variabelen.
 * Hvis ikke en thunk skal kalles s} returnerer rutinen adressen p->h.bp i
 * __er og p->h.v.ofs i __ev. Dersom den aktuelle er en text-konstant er
 * peker p->tp direkte til variabelen som representerer konstanten. I dette
 * tilfellet returneres __er=p->tp og __ev=0. Dette skulle ikke volde
 * problemer da text-konstanter ligger i et omr}de som ikke kan flyttes p} av
 * garbage collector.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret -> Returadressen
 * Returnerer: med en peker til text-verdien (enten i __et eller til den
 *             text-variabelen. */

char __rgettv (__textnamepar *p, long as, int ret, void (*mret))
{
  switch (p->namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      __goto = p->adr;	/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse 
				 * verdiene f|r kallet. */
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet og overf|rer
			         * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret; 
      __lb = __pb; 		/* Gj|r thunken eksikverbar.  */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __et = *(__txt *)(((char *) p->bp) + p->v.ofs);
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
