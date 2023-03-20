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
                                                    RutineGetRefValue        */

/* Denne rutinen kalles ved lese-aksess av name-parametere av type ref.Rutinen
 * avgj|r om en thunk skal kalles eller ikke p} bakgrunn av p->h.namekind
 * attributtet.
 * Hvis en thunk skal kalles, opprettes et objekt og f|lgende verdier legges i
 * objektet: conv og q (slik at thunken kan foreta den n|dvendige kvalifika-
 * sjonskontrollen) og returadressen. Retur-pekerverdien legges i ef.
 * Hvis ikke en thunk skal kalles s} henter rutinen peker-verdien direkte i
 * lokasjonen angitt av (p->h.bp+p->h.v.ofs).Hvis den aktuell parameteren er
 * konstanten NONE, er dette angitt ved at namekind==__VALUE_NOTHUNK. N|vendige
 * kvalifikasjonstester utf|res av rutinen. Peker-verdien legges i er.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * q          -> Strengeste kvalifikasjon p} aksessveien.
 * ret, mret -> Returadressen.
 * Returnerer: Peker-verdien i er. (ogs} hvis en thunk kalles) */

char __rgetrv (__refnamepar *p, long as, int ret, void (*mret) ())
{
  char tconv;
  __pty q;

  switch (p->namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      q = p->q;
      tconv = p->conv;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik at p ikke
				 * lengerpeker riktig,
				 * leses disse verdiene f|r kallet. */
      __goto = p->adr;
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet */
      ((__thunkp) __pb)->conv = tconv;
      ((__thunkp) __pb)->q = q;	/* Overf|rer strengeste kvalifikasjon p}
				 * aksessveien, og returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __er = *(__dhp *) (((char *) p->bp) + p-> v.ofs);
      if ((p->conv == __READTEST || p->conv == __READWRITETEST)
	  && !__rin (__er, p->q))
	__rerror ("Getrv: Wrong qualification");
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    case __VALUE_NOTHUNK:
      __er = __NULL;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
  return 0;
}
