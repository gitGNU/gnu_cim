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

/* Ytterste blokk */
__bs0FILE *__bp0FILE;
short __rl0FILE[2];
__pty __pl0FILE[1];
__ptyp __p0FILE =
{'B', 0, 0, sizeof (__bs0FILE), 1, __NULL, 0, 2, 2, 
  0, __rl0FILE, 0, __pl0FILE};

/* class FILE */
__bs1FILE *__bp1FILE;
short __rl1FILE[1];
__pty __pl1FILE[1];
__ptyp __p1FILE =
{'C', 0, 1, sizeof (__bs1FILE), 1, __NULL, 0, 0, 1, 
  0, __rl1FILE, 0, __pl1FILE};

/* FILE class IMAGEFILE */
__bs2FILE *__bp2FILE;
short __rl2FILE[1];
__pty __pl2FILE[2] =
{&__p1FILE};
__ptyp __p2FILE =
{'C', 1, 1, sizeof (__bs2FILE), 4, __NULL, 0, 0, 1, 
  0, __rl2FILE, 0, __pl2FILE};

/* IMAGEFILE class OUTFILE */
__pty __pl3FILE[3] =
{&__p1FILE, &__p2FILE};
__ptyp __p3FILE =
{'C', 2, 1, sizeof (__bs3FILE), 7, __NULL, 0, 0, 0, 0, 0, 0, __pl3FILE};

/* IMAGEFILE class INFILE */
__pty __pl4FILE[3] =
{&__p1FILE, &__p2FILE};
__ptyp __p4FILE =
{'C', 2, 1, sizeof (__bs4FILE), 10, __NULL, 0, 0, 0, 0, 0, 0, __pl4FILE};

/* IMAGEFILE class DIRECTFILE */
__pty __pl5FILE[3] =
{&__p1FILE, &__p2FILE};
__ptyp __p5FILE =
{'C', 2, 1, sizeof (__bs5FILE), 13, __NULL, 0, 0, 0, 0, 0, 0, __pl5FILE};

/* OUTFILE class PRINTFILE */
__pty __pl6FILE[4] =
{&__p1FILE, &__p2FILE, &__p3FILE};
__ptyp __p6FILE =
{'C', 3, 1, sizeof (__bs6FILE), 16, __NULL, 0, 0, 0, 0, 0, 0, __pl6FILE};

/* FILE class BYTEFILE  */
__pty __pl7FILE[2] =
{&__p1FILE};
__ptyp __p7FILE =
{'C', 1, 1, sizeof (__bs7FILE), 19, __NULL, 0, 0, 0, 0, 0, 0, __pl7FILE};

/* BYTEFILE class INBYTEFILE */
__pty __pl8FILE[3] =
{&__p1FILE, &__p7FILE};
__ptyp __p8FILE =
{'C', 2, 1, sizeof (__bs8FILE), 22, __NULL, 0, 0, 0, 0, 0, 0, __pl8FILE};

/* BYTEFILE class OUTBYTEFILE */
__pty __pl9FILE[3] =
{&__p1FILE, &__p7FILE};
__ptyp __p9FILE =
{'C', 2, 1, sizeof (__bs9FILE), 25, __NULL, 0, 0, 0, 0, 0, 0, __pl9FILE};

/* BYTEFILE class DIRECTBYTEFILE */
__pty __pl10FILE[3] =
{&__p1FILE, &__p7FILE};
__ptyp __p10FILE =
{'C', 2, 1, sizeof (__bs10FILE), 28, __NULL, 0, 0, 0, 0, 0, 0, __pl10FILE};

/* Variabel som skal ha programm navnet. 
 * Brukes i den predefinerte rutienen simulaid */
char *__progname;
long __argc;
long __argv;

/* Deklarerer tekstobjekter for filnavnene stdin og stdout */
struct __name
  {
    __th h;
    char string[9];
  }
__stdinname =
{
  (__pty) __TEXT, (__dhp) & __stdinname, __TRUE, 8, "/dev/tty"
}
,
__stdoutname =
{
  (__pty) __TEXT, (__dhp) & __stdoutname, __TRUE, 8, "/dev/tty"
}
,
__stderrname =
{
  (__pty) __TEXT, (__dhp) & __stderrname, __TRUE, 8, "/dev/tty"
};

/* Deklarerer tekstobjekter for stdin.IMAGE og stdout.IMAGE */
struct __imagein
  {
    __th h;
    char string[INPUT_LINE_LENGTH];
  }
__stdinimage =
{
  (__pty) __TEXT, (__dhp) & __stdinimage, __FALSE, INPUT_LINE_LENGTH, ""
};

struct __imageout
  {
    __th h;
    char string[OUTPUT_LINE_LENGTH + 1];
  }
__stdoutimage =
{
  (__pty) __TEXT, (__dhp) & __stdoutimage, __FALSE, OUTPUT_LINE_LENGTH, ""
}
,
__stderrimage =
{
  (__pty) __TEXT, (__dhp) & __stderrimage, __FALSE, OUTPUT_LINE_LENGTH, ""
};

/* Genererer og initierer ytterste blokk objekt */

__bs0FILE __blokk0 =
{(__pty) & __p0FILE, (__dhp) & __blokk0, (__dhp) & __blokk0, 
  __FALSE, __TERMINATED, 0, 0,
  (__dhp) & __blokk0, __NULL, __NULL};

/* Genererer og initierer et infile objekt (SYSIN) */

__bs4FILE __sysin =
{(__pty) & __p4FILE, (__dhp) & __sysin, (__dhp) & __blokk0, 
  __FALSE, __TERMINATED, 0, __NULL,
  (__dhp) & __blokk0, (__textref) & __stdinname, 5, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stdinimage, INPUT_LINE_LENGTH, INPUT_LINE_LENGTH + 1, 1,
 __FALSE};

/* Genererer og initierer et printfile objekt (SYSOUT) */

__bs6FILE __sysout =
{(__pty) & __p6FILE, (__dhp) & __sysout, (__dhp) & __blokk0, 
 __FALSE, __TERMINATED, 0, __NULL,
 (__dhp) & __blokk0, (__textref) & __stdoutname, 6, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stdoutimage, OUTPUT_LINE_LENGTH, 1, 1,
 1, MAX_INT, 1, 0};

__bs6FILE __syserr =
{(__pty) & __p6FILE, (__dhp) & __syserr, (__dhp) & __blokk0, 
 __FALSE, __TERMINATED, 0, __NULL,
 (__dhp) & __blokk0, (__textref) & __stderrname, 6, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stderrimage, OUTPUT_LINE_LENGTH, 1, 1,
 1, MAX_INT, 1, 0};

__map *__curent_map;
long __cline;

long __lines_per_page = LINES_PER_PAGE;
