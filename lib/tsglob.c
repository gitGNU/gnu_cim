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
#include "config.h"
#include "limit.h"

double __gbctime = 0.0;

char *__errnone = "Inline: None object",
 *__errlim = "Inline: Lower bound greater than upper bound in array",
 *__errarr = "Inline: Actual array does not correspond to formal array parameter",
 *__errswitch = "Inline: Switch index out of bounds",
 *__errbound = "Inline: Array index out of bounds",
 *__errqual = "Inline: Wrong qualification",
 *__errvirt = "Inline: Unmatched virtual entity";

__dhp __sto,
  __min,
  __max,
  __fri;
__dh __nil;

__dhp __sl;			/* Brukes for } overf|re statisk  omgivelse
				 * til rcp() og rcpp 
				 * Kan ikke v{re parameter
				 * fordi rgbc m} kunne oppdatere den */

/* Hopp-variabel settes ved utgang fra diverse runtime-rutiner */
__progadr __goto,
  __return;
long __vreturn;

/* Variabel som brukes i forbindelse med switch */
int __swv;


char __chpoolsize;
int __gc = 0;			/* Teller antall ganger garbage collector
				 * som er utfort */
/* Lokal-blokk og parameter-blokk */
__dhp __lb,
  __pb;				/* lp initseres i rallocdynmem */

/* stakk for __evaluering av uttrykk */

__valuetype __v[STACK_SIZE + 1],
  __ev;
__dhp __r[STACK_SIZE + 1],
  __er;
__txt __t[STACK_SIZE + 1],
  __et,__t1,__t2;

long __as;

/* Peker til arrayobjektet som initsieres eller aksesseres. */
__arrp __ap;

/* Array som inneholder verdiene (indeks - lower bound) for de */
/* forskjellige dimmensjonene til et ARRAY(kodeoptimalisering)    */
int __h[MAX_ARRAY_DIM];

char *__progname;

__pty __pp;			/* Hjelpevariable i forbindelse med inspect
				 * setninger og kall p} 
				 * virtuelle prosedyrer. */
__dhp __bp;			/* Hjelpevariabel for in test */
__refvarpar *__vrp;		/* Hjelpevariabel for om in test er n|dvendig 
				 * for name par */
__refnamepar *__nrp;
__aritvarpar *__vvp;
__aritnamepar *__nvp;

char __conv;

long __rputlen;

char __currentlowten = '&',
  __currentdecimalmark = '.';

__notext __tk0 =
{__NULL, 0, 0, 0};

char __ctab[3][4] =
{__NOCONV, __INTREAL, __REALINT, __REALINTREAL,
 __INTREAL, 0, __REALINTREAL, 0,
 __REALINT, __NOCONV, 0, __REALINT};


long __poolsize;
long __maxsize = 1000000;
char __dynsize;

char *__ctext;			/* Brukes for } holde p} returverdien fra en
				 * text c-prosedyre */

char *__nullstr = "";		/* Brukes for } overf|re notext til en
				 * C-prosedyre. */

/* Variabel som skal ha programm navnet. 
 * Brukes i den predefinerte rutienen simulaid */
char *__progname;
long __argc;
long __argv;

__map *__curent_map;
long __cline;

long __lines_per_page = LINES_PER_PAGE;
