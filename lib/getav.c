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
                                                    RutineGetAritmetiskValue */

/* Denne rutinen kalles ved lese-aksess av arit. name-parameter.Rutinen avgj|r
 * om en thunk skal kalles eller ikke p} bakgrunn av p->h.namekind attributtet
 * Hvis en thunk skal kalles, opprettes et objekt og f|lgende verdier legges i
 * objektet: conv og formell type (slik at thunken kan foreta den n|dvendige
 * konvertering og returadressen. Verdien legges i __ev.i (for integer)
 * eller i __ev.r for real.
 *
 * Hvis ikke en thunk skal kalles s} henter rutinen verdien, enten gitt
 * direkte av p->h.v.ivalue (integer) eller p->h.v.rvalue (real), eller i
 * lokasjon (p->h.bp+p->h.v.ofs).N|dvendige konverteringer foretas avhengig av
 * p->conv og formell type overf|rt i ftype. Verdien legges i __ev.i (for int)
 * eller i __ev.r (for verdi av type real). Thunkens statiske omgivelse er
 * overf|rt i den globale variablen sl.
 *
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ftype      -> Den formelle typen.
 * ret, mret -> Returadressen
 * Returnerer: Verdien i __ev eller ef, (ogs} hvis en thunk kalles) */

char
__rgetav (ftype, p, as, ret, mret)
     char ftype;
     __aritnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  char tconv;
  switch (p->namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      tconv = p->conv;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse
				 * verdiene f|r kallet. */

      __goto = p->adr;	
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet */
      ((__thunkp) __pb)->conv = tconv;
      ((__thunkp) __pb)->ftype = ftype;	/* Overf|rer den formelle
					 * typen og returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      if (ftype == __TINTG)
	__ev.i = (p->conv == __NOCONV 
		  ? *(long *) (((char *) p->bp) + p->v.ofs) :
		  (long) *(double *) (((char *) p->bp) + p->v.ofs));
      else
	__ev.f = (p->conv == __NOCONV 
		  ? *(double *) (((char *) p->bp) + p->v.ofs) :
		  p->conv == __INTREAL 
		  ? (double) *(long *) (((char *) p->bp) + p->v.ofs) :
	     (double) (long) *(double *) (((char *) p->bp) + p->v.ofs));
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    case __VALUE_NOTHUNK:
      if (ftype == __TINTG)
	__ev.i = (p->conv == __NOCONV ? p->v.i : (long) (double) p->v.f);
      else
	__ev.f = (p->conv == __NOCONV ? p->v.f : p->conv == __INTREAL ?
		  (double) p->v.i : (double) (long) p->v.f);
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
