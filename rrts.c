/* $Id: rrts.c,v 1.23 1995/08/24 06:39:39 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* RUNTIME rutiner for simula skrevet i c. */

#include <stdio.h>
#include <math.h>

#define __FILES FILE
#include "cim.h"
#include "limit.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
/* An ANSI string.h and pre-ANSI memory.h might conflict.  */
#if !STDC_HEADERS && HAVE_MEMORY_H
#include <memory.h>
#endif /* not STDC_HEADERS and HAVE_MEMORY_H */
#define bcopy(s, d, n) memcpy ((d), (s), (n))
#define bzero(s, n) memset ((s), 0, (n))
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#include <strings.h>
/* memory.h and strings.h conflict on some systems.  */
#endif /* not STDC_HEADERS and not HAVE_STRING_H */


#if STDC_HEADERS
#include <stdlib.h>
#endif

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if ALIGN
#define allign(i) ((((long)i)+ALIGN)&(~ALIGN))
#else
#define allign(i) (i)
#endif

#if nolib|rtsglobals

#if REALASDOUBLE != __REALASDOUBLE
/*#error Illegal value on __REALASDOUBLE in cim.h */
#endif

#if FLOAT_IMPLEMENTED
double __gbctime = 0.0;
#endif

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
  (__pty) __TEXT, (__dhp) & __stdinname, __TRUE, 8, STDINNAME
}
,
__stdoutname =
{
  (__pty) __TEXT, (__dhp) & __stdoutname, __TRUE, 8, STDOUTNAME
}
,
__stderrname =
{
  (__pty) __TEXT, (__dhp) & __stderrname, __TRUE, 8, STDERRNAME
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
#if M_I386
sysi86 ()
{
}
#endif
#endif


/******************************************************************************
                                                    RUTINEOPTIONS            */

#if nolib|rstartrut
static char
__roptions (antarg, arg1)
     long antarg;
     char arg1[];
{
  long i,
    size = 0;
  if (antarg < 2)
    return (__TRUE);
  if (!__dynsize)
    return (__TRUE);
  if (arg1[0] != '-')
    return (__TRUE);
  if (arg1[1] != 'm' && arg1[1] != 'k' && arg1[1] != 'M' && arg1[1] != 'K')
    return (__TRUE);
  for (i = 2; arg1[i] >= '0' && arg1[i] <= '9'; i++)
    size = size * 10 + arg1[i] - '0';
  if (arg1[1] == 'm' || arg1[1] == 'M')
    size *= 1024;
  if (arg1[i] == '\0')
    {
      __maxsize = __poolsize = size;
      if (arg1[1] == 'k' || arg1[1] == 'K')
	(void) fprintf (stderr, "Poolsize is changed to %dK\n", __poolsize);
      else
	(void) fprintf (stderr, "Poolsize is changed to %dM\n", 
			__poolsize / 1024);
    }
  return (__TRUE);
}

/******************************************************************************
                                                         RutineAllocDynMem   */

static void
__rallocdynmem (size)
     long size;
{
  __min = (__dhp) calloc ((unsigned) (size), (unsigned) 1);
  if (!__min)
    __rerror ("Allocdynmem: Not enough core");
  __fri = __min;
  __max = (__dhp) ((char *) __min + size);
}


/******************************************************************************
                                                               RUTINESTART   */

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_SIGFPE
RETSIGTYPE
__rfloat_trap ()
{
  __rerror ("Arithmetic overflow");
}
#endif

#if HAVE_SIGSEGV
RETSIGTYPE
__rseg_trap ()
{
  __rerror ("System error: Segmentation violation");
}
#endif

#if HAVE_SIGILL
RETSIGTYPE
__rillegal_trap ()
{
  __rerror ("System error: Illegal instruction");
}

#endif

#if HAVE_SIGTRAP
RETSIGTYPE
__rtrace_trap ()
{
  __rerror ("System error: Trace trap");
}
#endif

#if HAVE_SIGSYS
RETSIGTYPE
__rsys_trap ()
{
  __rerror ("System error: Bad argument to system call");
}
#endif

#if HAVE_SIGBUS
RETSIGTYPE
__rbus_trap ()
{
  __rerror ("System error: Bus error");
}
#endif
__rstart (argc, argv)
     int argc;
     char *argv[];
{
#if CLOCK
  clock ();
#endif
  __sysin.s.s.file = stdin;
  __sysout.s.s.s.file = stdout;
  __syserr.s.s.s.file = stderr;

  __argc = (long) argc;
  __argv = (long) argv;
  __progname = argv[0];

  __roptions (argc, argv[1]);

  __init ();

/* SYSIN  :- new infile("...");   
 * SYSOUT :- new printfile("...");
 * SYSIN.open(blanks(INPUT_LINE_LENGTH));
 * SYSOUT.open(blanks(OUTPUT_LINE_LENGTH));
 * inspect SYSIN do
 * inspect SYSOUT do */

  if (__poolsize)
    __rallocdynmem (__poolsize * (long) 1024);
  else
    __rallocdynmem (DYNMEMSIZEKB * (long) 1024);

  __lb = __pb = (__dhp) & __blokk0;
  /* Fyller IMAGE til stdin og stdout med blanke */
  __rtextvalassign (&((__bs2FILE *) __rsysin ())->IMAGE, (__txtvp) & __tk0);
  __rtextvalassign (&((__bs2FILE *) __rsysout ())->IMAGE, (__txtvp) & __tk0);
  __rtextvalassign (&((__bs2FILE *) __rsyserr ())->IMAGE, (__txtvp) & __tk0);

  /* Kobler c1,c2 i blokk0 til objektene av infile og printfile * henholdsvis 
   * 
   * *  * *  * *  * *  * * sysin og sysout. */
  __blokk0.c1 = __rsysin ();
  __blokk0.c2 = __rsysout ();

#if HAVE_SIGFPE
  signal (SIGFPE, __rfloat_trap);
#endif
#if HAVE_SIGSEGV
  signal (SIGSEGV, __rseg_trap);
#endif
#if HAVE_SIGILL
  signal (SIGILL, __rillegal_trap);
#endif
#if HAVE_SIGTRAP
  signal (SIGTRAP, __rtrace_trap);
#endif
#if HAVE_SIGSYS
  signal (SIGSYS, __rsys_trap);
#endif
#if HAVE_SIGBUS
  signal (SIGBUS, __rbus_trap);
#endif
}

/******************************************************************************
                                                               RUTINESLUTT   */
__rslutt ()
{
  if (__gc && ((__bs1FILE *) __blokk0.c2)->open)
    {
      __rpoutimage (__blokk0.c2);

#if FLOAT_IMPLEMENTED
      printf ("%d garbage collection(s) in %.1f seconds.", __gc, __gbctime);
#else
      printf ("%d garbage collection(s).", __gc);
#endif
      if (__chpoolsize)
	{
	  printf (" Poolsize changed to ");
	  if (__poolsize >= 1024)
	    printf ("%dM while running.", __poolsize / 1024);
	  else
	    printf ("%dK while running.", __poolsize);
	}
      printf ("\n");

    }
  __riclose (__rsysin ());
  __roclose (__rsysout ());
  __roclose (__rsyserr ());
}

#endif


/******************************************************************************
                                                        RutinePRINTFILLINE   */

#if nolib|rprintfillinerut
void
__rprintfilline ()
{
  __map *mapindeks = __curent_map;
  if (__curent_map)
    {
      while ((++mapindeks)->fromline <= __cline);
      mapindeks--;
      fprintf (stderr, "\n\"%s\", line %ld: "
	       ,mapindeks->fil, __cline + mapindeks->line);
    }
  else
    fprintf (stderr, "\n\"\", line : ");
}
#endif

/******************************************************************************
                                                                RutineERROR  */

#if nolib|rerrorrut
__rerror (s)
     char *s;
{
  __rprintfilline ();
  (void) fprintf (stderr, "RUNTIME-ERROR: %s\n", s);
#ifndef lint
  exit (1);
#endif
}
#endif

/******************************************************************************
                                                               RutineWARNING */

#if nolib|rwarningrut
void
__rwarning (s)
     char *s;
{
  __rprintfilline ();
  (void) fprintf (stderr, "RUNTIME-WARNING: %s\n", s);
}
#endif

/******************************************************************************
                                                               RutineTRACE   */

#if nolib|rtracerut
void
__rtrace ()
{
  __rprintfilline ();
  (void) fprintf (stderr, "TRACE:\n");
}
#endif

/******************************************************************************
                                                        RutineSYSTEMERROR    */

#if nolib|rsystemerrorrut
void
__rsystemerror (s)
     char *s;
{
  __rprintfilline ();
  (void) fprintf (stderr, "SYSTEM-ERROR: %s\n", s);
#ifndef lint
  exit (1);
#endif
}
#endif

/******************************************************************************
                                                        RutineInitier        */

#if nolib|rinitierrut
void
__rinitier (fra, til)
     __dhp *fra,
      *til;
{
  while ((long) fra < (long) til)
    *fra++ = 0;
}
#endif

/******************************************************************************
                                                        RutineSaveStack      */

#if nolib|rssrut
void
__rss (as)
     long as;
{
  long size,
    i,
    ar,
    av,
    at;
  __stkp x;
  /* Tilordner antref og anttext i tilfelle kallet p} ralloc f|rer til */
  /* garbage collection. Gbc kan da lese av antall pekere p} stakken */
  /* den m} ta hensyn til. */
  __as = as;
  av = as >> 16 & 0xff;
  ar = as >> 8 & 0xff;
  at = as & 0xff;

  size = sizeof (__stk) - sizeof (__val_or_ref)
    + (ar + av + (4 * at))
    * sizeof (__val_or_ref);
  x = (__stkp) __ralloc (size);
  __as = 0;
  x->h.pp = (__pty) __ACTS;
  x->h.size = size;
  x->h.pm = ar;
  x->h.dt = av;
  x->h.at = at;
  x->h.dl = __pb;
  __pb = (__dhp) x;

  for (i = av; i; i--)
    {
      x->s[i - 1].v = __v[i];
    }

  for (i = ar; i; i--)
    x->s[i - 1 + av].r = __r[i];

  for (i = at; i; i--)
    {
      x->s[av + ar + i - 1].r = (__dhp) __t[i].obj;
      x->s[at + i * 3 - 3 + av + ar].v.i = (long) __t[i].length;
      x->s[at + i * 3 - 2 + av + ar].v.i = (long) __t[i].pos;
      x->s[at + i * 3 - 1 + av + ar].v.i = (long) __t[i].start;
    }

}
#endif

/******************************************************************************
                                                        RutineRestoreStack   */

#if nolib|rrsrut
void
__rrs ()
{
  long i;
  __stkp x;
  long size;
  long ar,
    av,
    at;
  ar = __pb->pm;
  av = __pb->dt;
  at = ((__stkp) __pb)->h.at;
  x = (__stkp) __pb;
  size = x->h.size;
  __pb = __pb->dl;
  for (i = av; i; i--)
    {
      __v[i] = x->s[i - 1].v;
    }

  for (i = ar; i; i--)
    __r[i] = x->s[i - 1 + av].r;

  for (i = at; i; i--)
    {
      __t[i].obj = (__textref) x->s[av + ar + i - 1].r;
      __t[i].length = x->s[at + i * 3 - 3 + av + ar].v.i;
      __t[i].pos = x->s[at + i * 3 - 2 + av + ar].v.i;
      __t[i].start = x->s[at + i * 3 - 1 + av + ar].v.i;
    }

  if ((char *) __fri == ((char *) x) + allign (size))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = (__dhp) x;
    }

}
#endif

/******************************************************************************
                                                        RutineCallProcedure  */

#if nolib|rcprut
void
__rcp (ppx, as)
     long as;
     __pty ppx;
{
  __dhp x;
  if (as)
    __rss (as);
  x = __ralloc ((long) ppx->size);
  x->pm = __TRUE;		
  /* x->dt=__EXECUTING; Er implisitt satt */
  x->pp = ppx;
  x->sl = __sl;			/* sl er global variabel */
  __sl = __NULL;
  x->dl = __pb;
  __pb = x;
}
#endif
/******************************************************************************
                                                  RutineCallProperProcedure  */

#if nolib|rcpprut
void
__rcpp (ppx)
     __pty ppx;
{
  __dhp x;
  x = __ralloc ((long) ppx->size);
  x->pm = __TRUE;
  /* x->dt=__EXECUTING; Er implisitt satt */
  x->pp = ppx;
  x->sl = __sl;
  __sl = __NULL;
  x->dl = __pb;
  __pb = x;
}
#endif

/******************************************************************************
                                                RutineCallProcedureBegin     */

#if nolib|rcpbrut
void
__rcpb (ret, mret)
     int ret;
     void (*mret) ();
{
/* __pb->dt=__executing; Er implisitt satt */
  __pb->pm = __FALSE;
  __goto = __pb->pp->adr;
  __pb->ex.ent = ret;
  __pb->ex.ment = mret;

  __lb = __pb;
}
#endif

/******************************************************************************
                                                RutineCallClassBegin         */

#if nolib|rccbrut
void
__rccb (ret, mret)
     int ret;
     void (*mret) ();
{
/* __pb->dt=__executing; Er implisitt satt */
  __pb->pm = __FALSE;
  __goto = __pb->pp->pref[0]->adr;
  __pb->ex.ent = ret;
  __pb->ex.ment = mret;

  __lb = __pb;
}
#endif

/******************************************************************************
                                                        RutineEndProcedure   */

#if nolib|reprut
void
__rep ()
{
  __dhp x;
  long size;
  __goto.ent = __pb->ex.ent;
  __goto.ment = __pb->ex.ment;
  x = __pb;
  size = __pb->pp->size;
  __pb = __pb->dl;
  if (__pb->pp == (__pty) __ACTS)
    __rrs ();
  __lb = __pb;
  while (__lb->pm || __lb->pp == (__pty) __ACTS)
    __lb = __lb->dl;
  /* Frigj|r memory */
  if ((char *) __fri == (((char *) x) + allign (size)) 
      || __roa ((__dhp) ((char *) x + allign (size))))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = x;
    }
}
#endif

/******************************************************************************
                                                  RutineEndProperProcedure   */

#if nolib|repprut
void
__repp ()
{
  __dhp x;
  long size;
  __goto.ent = __pb->ex.ent;
  __goto.ment = __pb->ex.ment;

  x = __pb;
  size = __pb->pp->size;

  __lb = __pb = __pb->dl;

  if ((char *) __fri == (((char *) x) + allign (size)) 
      || __roa ((__dhp) ((char *) x + allign (size))))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = x;
    }
}
#endif

/******************************************************************************
                                                     RutineOnlyArrays        */

/* Denne rutinen kalles fra __rep(p) og __rbe -- retur fra prosedyre og blokk.
 * Den sjekker om det p} toppen av poolen over blokk-instansen for den
 * terminerende prosedyre eller blokk kun ligger array-objekter.
 * Hvis det er tilfellet, m} disse v{re generert av denne prosedyren/blokken,
 * og de kan deallokeres.
 * Array objektene kan ikke v{re referert av andre 
 * (klasse objekter der arrayen er gitt som parameter)
 * fordi vi da ville funnet disse objektene p} poolen. 
 * Array objektene kan ha v{rt referert av andre prosedyrer,
 * men de har terminert og blitt deallokert.
 * Hvis den kun finner array-objekter returnerer rutinen TRUE eller FALSE. */

#if nolib|roarut
__roa (x)
     __dhp x;
{
  if (x < __min)
    return (__FALSE);
  while (x < __fri && x->pp == (__pty) __ARRAY)
    x = (__dhp) ((char *) x + allign (((__ahp) x)->size));
  return (x == __fri);
}

#endif

/******************************************************************************
                                                     RutineCallPrefiksBlock  */

#if nolib|rcprbrut
void
__rcprb (ppx)
     __pty ppx;
{
  __pb = __ralloc ((long) ppx->size);
  __pb->pp = ppx;
  __pb->sl = __pb->dl = __lb;
  __lb = __pb;
}
#endif

/******************************************************************************
                                               RutineCallPrefiksBlockBegin   */

#if nolib|rcprbbrut
void
__rcprbb (ret, mret)
     int ret;
     void (*mret) ();
{
  __goto = __pb->pp->pref[0]->adr;
  __pb->ex.ent = ret;
  __pb->ex.ment = mret;
}
#endif

/******************************************************************************
                                                                RutineBlock  */

#if nolib|rbrut
void
__rb (ppx)
     __pty ppx;
{
  __dhp x;
  x = __ralloc ((long) ppx->size);
  x->pm = __FALSE;
  /* x->dt=__executing; Er implisitt satt */
  x->pp = ppx;
  x->sl = x->dl = __lb;
  __lb = __pb = x;
  /* __lb->ex.ent=ret;  trengs ikke settes */
}
#endif

/******************************************************************************
                                                        RutineBlockEnd       */

#if nolib|rberut
void
__rbe ()
{
  __dhp x;
  long size;
  x = __pb;
  size = __pb->pp->size;
  __lb = __lb->sl;
  __pb = __lb;
  if ((char *) __fri == (((char *) x) + allign (size))
      || __roa ((__dhp) ((char *) x + allign (size))))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = x;
    }
}
#endif

/******************************************************************************
                                                        RutineResume         */

#if nolib|rresumerut
void
__rresume (ob, ret, mret)
     __dhp ob;
     int ret;
     void (*mret) ();
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

  sh = NULL;
  while (__lb != ob->sl)
    {
      if (__lb == (__dhp) & __blokk0)
	__rerror ("Resume: Illegal object");
      sh = __lb;
      if (__lb->dt == __EXECUTING || __lb->pp == (__pty) __ACTS)
	__lb = __lb->dl;
      else
	__lb = __lb->sl;
    }

  if (sh == NULL || (sh->pp != (__pty) __THUNK && sh->pp != (__pty) __ACTS &&
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
#endif

/******************************************************************************
                                                        RutineExchange       */

#if nolib|rexchangerut
void
__rexchange (sh, ob, ret, mret)
     __dhp sh,
       ob;
     int ret;
     void (*mret) ();
{
  __goto = ob->ex;
  __lb = ob->dl;
  ob->dl = sh->dl;
  ob->ex = sh->ex;
  ob->dt = __RESUMED;
  sh->dl = __pb;
  sh->ex.ent = ret;
  sh->ex.ment = mret;
  sh->dt = __DETACHED;
  __pb = __lb;
}
#endif

/******************************************************************************
                                                        RutineDetach         */

#if nolib|rdetachrut
void
__rdetach (ob, ret, mret)
     __dhp ob;
     int ret;
     void (*mret) ();
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
      if (p == (__dhp) & __blokk0)
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
#endif

/******************************************************************************
                                                        RutineCall           */

#if nolib|rcallrut
void
__rcall (ob, ret, mret)
     __dhp ob;
     int ret;
     void (*mret) ();
{
  if (ob == __NULL)
    __rerror ("Call: NONE-pointer");
  if (ob->dt != __DETACHED)
    __rerror ("Call: The object is not detached");
  ob->dt = __EXECUTING;
  __goto = ob->ex;
  ob->ex.ent = ret;
  ob->ex.ment = mret;
  __lb = ob->dl;
  ob->dl = __pb;
  __pb = __lb;
}
#endif

/******************************************************************************
                                                        RutineGoto           */

#if nolib|rgotorut
void
__rgoto (ob)
     __dhp ob;
{
  __dhp x;

  __lb = ob;
  while (__pb != __lb)
    {				
      /* Hvis ikke __lb ligger p} operasjonskjeden s} skal goto 
       * resultere i en feil. Det må sjekkes her. */

      if (__pb == (__dhp) & __blokk0)
	__rerror ("Goto: Label not on operating chain");

      if (__pb->pp == (__pty) __ACTS || __pb->pp->kind != __KCLASS)
	__pb = __pb->dl;
      else if (__pb->pp->kind == __KCLASS)
	{
	  (x = __pb)->dt = __TERMINATED;
	  __pb = __pb->dl;
	  x->dl = x;
	}
    }
}
#endif

/******************************************************************************
                                                        RutineEndDecl        */

#if nolib|renddeclrut
void
__renddecl (plev)
     int plev;
{
  if (plev <
#if __CHAR_UNSIGNED__
      (signed char)
#endif
      __pb->pp->plev)
    {
      __goto = __pb->pp->pref[plev + 1]->adr;
    }
  else
    {
      __goto = __pb->pp->pref[0]->adr;
      __goto.ent++;
    }
}
#endif

/******************************************************************************
                                                        RutineInner          */

#if nolib|rinnerrut
void
__rinner (plev)
     int plev;
{
  if (plev <
#if __CHAR_UNSIGNED__
      (signed char)
#endif
      __pb->pp->plev)
    {
      __goto = __pb->pp->pref[plev + 1]->adr;
      __goto.ent++;
    }
  else
    {
      __goto = __pb->pp->adr;
      __goto.ent += 2;
    }
}
#endif

/******************************************************************************
                                                        RutineEndClass       */

#if nolib|rendclassrut
void
__rendclass (plev)
     int plev;
{
  __dhp p;
  if (plev == 0)
    {
      __goto = __pb->ex;
      __pb->dt = __TERMINATED;
      __er = __lb;
      __pb = __pb->dl;
      __lb->dl = __lb;
      if (__pb->pp == (__pty) __ACTS)
	__rrs ();
      __lb = __pb;
      while (__lb->pm || __lb->pp == (__pty) __ACTS)
	__lb = __lb->dl;
    }
  else
    {
      __goto = __pb->pp->pref[plev - 1]->adr;
      __goto.ent += 2;
    }
}
#endif



/******************************************************************************
                                             RutineCopyArray                 */

#if nolib|rcarut
__dhp
__rca (a)
     __arrp a;
{
  register __dhp *p,
   *q,
   *r;				/* Brukes for } kopiere array objektet */
  __sl = (__dhp) a;
  q = (__dhp *) (__er = (__dhp) __ralloc (((long) a->h.size)));
  a = (__arrp) __sl;
  p = (__dhp *) a;
  r = (__dhp *) (((char *) q) + a->h.size);
  while ((char *) q < (char *) r)
    *q++ = *p++;
  return (__er);
}
#endif

/******************************************************************************
                                             RutineIn (test)                 */

#if nolib|rinrut
char
__rin (bpx, p)
     __dhp bpx;
     __pty p;
{
  if (bpx == __NULL ||
#if __CHAR_UNSIGNED__
      (signed char)
#endif
      bpx->pp->plev <
#if __CHAR_UNSIGNED__
      (signed char)
#endif
      p->plev || bpx->pp->pref[p->plev] != p)
    return (__FALSE);
  return (__TRUE);
}
#endif

/******************************************************************************
                                              Rutinerin (test)               */

/* Utf|rer en slags intest som brukes ved overforing av name parametere.
 * Rin() brukes ved vanlige in tester */

#if nolib|rrinrut
char
__rrin (q, p)
     __pty q,
       p;
{
  if (
#if __CHAR_UNSIGNED__
       (signed char)
#endif
       q->plev <
#if __CHAR_UNSIGNED__
       (signed char)
#endif
       p->plev || q->pref[p->plev] != p)
    return (__FALSE);
  return (__TRUE);
}
#endif

/******************************************************************************
                                             RutineRis         (test)        */

#if nolib|risrut
char
__ris (bpx, p)
     __dhp bpx;
     __pty p;
{
  if (bpx == __NULL || bpx->pp != p)
    return (__FALSE);
  return (__TRUE);
}
#endif

#if nolib|rgbcrut

/******************************************************************************
                                                       DO_FOR_STACK_POINTERS */

/* Denne rutinen g}r igjennom alle stakk-pekere og gj|r utf|rer rutinen
 * doit for hver data peker.
 * Denne rutinen kalles fra pass 1 og pass 3 i GBC.
 * Dessuten kalles den ogs} fra add_to_pointers i det tilfellet 
 * at poolen blir flyttet */

static
do_for_stack_pointers (doit)
     void (*doit) ();
{
  int i,
    ar,
    at;
  ar = __as >> 8 & 0xff;
  at = __as & 0xff;
  /* Behandler f|rst ref og text stakken */

  for (i = ar; i; i--)
    (*doit) (&__r[i]);

  for (i = at; i; i--)
    (*doit) ((__dhp *) & __t[i].obj);
}

/******************************************************************************
                                                         DO_FOR_EACH_POINTER */

/* Denne rutinen g}r igjennom alle pekere for et dataobjekt
 * og utf|rer rutinen doit(_notest) for hver data peker.
 * Denne rutinen kalles fra pass 1 og pass 3 i GBC.
 * Dessuten kalles den ogs} fra add_to_pointers i det tilfellet 
 * at poolen blir flyttet */

__do_for_each_pointer (p, doit, doit_notest)
     __dhp p;
     void (*doit) (),
  (*doit_notest) ();
{
  __dhp *ph,
   *qh;
  static __pty ppx;
  long i,
    j;

  switch ((int) p->pp)
    {
    case __TEXT:
      break;
    case __ARRAY:
      ph = (__dhp *) ((char *) p + sizeof (__ah) +
		      (((__ahp) p)->dim * sizeof (__arrlimit)));
      qh = (__dhp *) ((char *) p + ((__ahp) p)->size);
      if (((__ahp) p)->type == __TREF)
	{
	  for (; (char *) ph < (char *) qh; ph++)
	    (*doit) ((__dhp *) ph);
	}
      else if (((__ahp) p)->type == __TTEXT)
	{
	  __txtvp th;
	  for (th = (__txtvp) ph; (char *) th < (char *) qh; th++)
	    (*doit) ((__dhp *) th);
	}
      break;
    case __ACTS:
      (*doit) (&p->dl);
      /* peker stakken */
      for (i = p->dt; i < (p->dt + p->pm + ((__shp) p)->at); i++)
	(*doit) (&((__stkp) p)->s[i].r);
      break;
    case __THUNK:
      /* Dynamisk link */
      (*doit_notest) (&p->dl);
      /* Statisk link */
      (*doit_notest) (&p->sl);
    case 0:
      break;
    default:
      /* Dynamisk link */
      (*doit_notest) (&p->dl);
      /* Statisk link */
      (*doit_notest) (&p->sl);

      /* Pekertabellen */
      j = 0;
      for (ppx = p->pp; j <=
#if __CHAR_UNSIGNED__
	   (signed char)
#endif
	   p->pp->plev; ppx = p->pp->pref[j++])
	for (i = 0; i < ppx->naref; i++)
	  {
	    (*doit) ((__dhp *) ((char *) p + ppx->ref[i]));
	  }
    }
}

/******************************************************************************
                                                                    GET_SIZE */

/* Denne rutinen finner st|rrelsen p} et data objekt inklusive allignment */

static long
get_size (p)
     __dhp p;
{
  long size;
  switch ((int) p->pp)
    {
    case __TEXT:
      size = ((__textref) p)->h.size + sizeof (__th) + 1;
      break;
    case __ARRAY:
      size = ((__ahp) p)->size;
      break;
    case __ACTS:
      size = ((__shp) p)->size;
      break;
    case __THUNK:
      size = sizeof (__thunk);
      break;
    default:
      size = p->pp->size;
    };
  return (allign (size));
}

static __dhp p;

/******************************************************************************
                                                              DO_ADD_TO_LIST */

/* Brukes som parameter til do_for_stack_pointers og do_for_each_pointer
 * for } f} lagt inn objekter som ikke allerede ligger i lista.
 * Den gies som parameter til de to nevnte rutinene fra GBC pass 1.
 * Den benytter seg av } kjede alle objekter sammen i en list ved
 * hjelp av GB-ordet. Objekter blir kjedet inn rett bak p.
 * P peker en hver tid p} det objektet som er under prosessering. */

static
do_add_to_list (qp)
     __dhp *qp;
{
  __dhp q;
  if ((q = *qp) != __NULL)
    if (q->gl == __NULL)
      {
	q->gl = p;
	p = q;
      };
}

/******************************************************************************
                                                           DO_UPDATE_POINTER */

/* Brukes som parameter til do_for_stack_pointers og do_for_each_pointer
 * for } f} oppdatert samtlige pekere til et objekt.
 * Den gies som parameter til de to nevnte rutinene fra GBC pass 3.
 * Den benytter seg av at adressen (etter flytting av objektet) 
 * til et objekt ligger i objektets GB-ord. 
 * Denne informasjonen er lagt i GB-ordet av GBC pass 2 */

static
do_update_pointer (qp)
     __dhp *qp;
{
  if (*qp != __NULL)
    *qp = (*qp)->gl;
}

/******************************************************************************
                                    Rutine garbage collector   (fire pas)    */

/* GBC algoritme som best}r av 4 pass.
 * 1. pass traverserer og merker alle aksesserbare pekere.
 * 2. pass beregner ny posisjon til data-objektene.
 * 3. pass oppdaterer pekere
 * 4. pass skyver de aksesserbare objektene sammen */

void
__rgbc ()
{
  static __dhp hppp;
  static __dhp hpp;
  static __dhp hp;
  static __dhp q;
  static long i,
    j;
  static long size;
  static __pty ppx;
  register __dhp *ph,
   *qh;				/* Brukes for } flytte objektene */

#if nolib & 0
  __rwarning ("GBC called");
#endif

#if FLOAT_IMPLEMENTED
  __gbctime -= __rcputime ();
#endif
  __gc++;

  /* PAS 1 */
  p = &__nil;

  do_add_to_list (&__lb);
  do_add_to_list (&__pb);
  do_add_to_list (&__sl);
  do_add_to_list (&__er);
  do_add_to_list (&__t1.obj);
  do_add_to_list (&__t2.obj);

  do_for_stack_pointers (do_add_to_list);

  __do_for_each_stat_pointer (do_add_to_list, do_add_to_list, __FALSE);

  __do_for_each_pointer (&__sysin, do_add_to_list, do_add_to_list);
  __do_for_each_pointer (&__sysout, do_add_to_list, do_add_to_list);
  __do_for_each_pointer (&__syserr, do_add_to_list, do_add_to_list);

  while (p != &__nil)
    {
      q = p;
      p = p->gl;

      __do_for_each_pointer (q, do_add_to_list, do_add_to_list);

    };
  /* PAS 2 */
  p = q = __min;
  while (p < __fri)
    {

      size = get_size (p);

      if (p->gl != __NULL)
	{
	  p->gl = q;
	  q = (__dhp) ((char *) q + size);
	}
      p = (__dhp) ((char *) p + size);
    };

  __update_gl_to_obj ();

  /* PAS 3 */

  do_for_stack_pointers (do_update_pointer);

  __do_for_each_stat_pointer (do_update_pointer, do_update_pointer, __FALSE);
  __do_for_each_pointer (&__sysin, do_update_pointer, do_update_pointer);
  __do_for_each_pointer (&__sysout, do_update_pointer, do_update_pointer);
  __do_for_each_pointer (&__syserr, do_update_pointer, do_update_pointer);

  p = __min;

  while (p < __fri)
    {

      if (p->gl != __NULL)
	__do_for_each_pointer (p, do_update_pointer, do_update_pointer);
      p = (__dhp) ((char *) p + get_size (p));

    };

  do_update_pointer (&__lb);
  do_update_pointer (&__pb);
  do_update_pointer (&__sl);
  do_update_pointer (&__er);
  do_update_pointer (&__t1.obj);
  do_update_pointer (&__t2.obj);

  /* PAS 4 */
  p = q = __min;
  while (p < __fri)
    {
      size = get_size (p);

      if (p->gl != __NULL)
	{
	  p->gl = __NULL;
	  ph = (__dhp *) p;
	  qh = (__dhp *) q;
	  q = (__dhp) ((char *) q + size);

	  bcopy ((char *) ph, (char *) qh, size);
	}

      p = (__dhp) ((char *) p + size);

    };
  __fri = q;

  __update_gl_to_null ();


  /* Nuller resten av omr}det */
  bzero ((char *) __fri, (char *) p - (char *) __fri);
#if FLOAT_IMPLEMENTED
  __gbctime += __rcputime ();
#endif
}

/******************************************************************************
                                                         (DO)ADD_TO_POINTER  */

/* Disse rutinene s|rger for at pekere blir oppdatert etter at pool'en er
 * flyttet. Do_add_to_pointer brukes som parameter til do_for_stack_pointers
 * og do_for_each_pointer, slik at pekerene blir oppdatert riktig.
 * Legg merke til at det er kun de pekere 
 * som peker innenfor poolen som skal oppdateres.
 * Denne oppdateringen gj|res ved } traversere samtlige objekter p}
 * samme m}te som i GBC pass 3. */

static long disp;
static char *new_fri,
 *new_min;

static
do_add_to_pointer (qp)
     __dhp *qp;
{
  if (*qp >= __min & *qp < __fri)
    *qp = (__dhp) ((char *) (*qp) + disp);
}

static
add_to_pointers ()
{
  char *p;

  do_for_stack_pointers (do_add_to_pointer);

  __do_for_each_stat_pointer (do_add_to_pointer, do_add_to_pointer, __TRUE);
  __do_for_each_pointer (&__sysin, do_add_to_pointer, do_add_to_pointer);
  __do_for_each_pointer (&__sysout, do_add_to_pointer, do_add_to_pointer);
  __do_for_each_pointer (&__syserr, do_add_to_pointer, do_add_to_pointer);

  p = new_min;

  while (p < new_fri)
    {
      __do_for_each_pointer ((__dhp) p, do_add_to_pointer, do_add_to_pointer);
      p = (char *) p + get_size ((__dhp) p);
    };

  do_add_to_pointer (&__sl);
  do_add_to_pointer (&__lb);
  do_add_to_pointer (&__pb);
}

/******************************************************************************
                                                            RutineAlloc      */

/* Denne rutinen er basis rutinen for allokering av data fra Simula.
 * Hvis det ikke er ledig plass i poolen s} blir GBC kalt.
 * Hvis det fremdeles ikke er nokk plass eller at under halve plassen
 * er frigjort fors|ker rutinen } utvide poolen ved } kalle realloc.
 * Blir poolen flyttet s} blir add_to_pointers kalt for } oppdatere pekere,
 * og nytt omr}de blir fylt med nuller. */

__dhp
__ralloc (size)
     long size;
{
  void __rgbc ();
  static __dhp mem;
  if (__sto != NULL)
    {
      __dhp dp;
      dp = __sto;
      __sto = __NULL;
      if (dp->pp != __NULL)
	{
	  bzero ((char *) dp, size);
	}
      return (dp);
    }
  size = allign (size);
  if (((char *) __fri + size) > (char *) __max)
    {
      __rgbc ();
#if REALLOC
      if (((char *) __fri - (char *) __min > (char *) __max - (char *) __fri)
	  || (char *) __fri + size > (char *) __max)
	{
	  long new_size,
	   *ph;

	  new_size = ((char *) __max - (char *) __min) * 2;
	  if (new_size - ((char *) __fri - (char *) __min) < size)
	    new_size += size;

	  if (new_size > __maxsize * 1024)
	    new_size = __maxsize * 1024;
	  if (new_size == ((char *) __max - (char *) __min))
	    new_min = NULL;
	  else
	    new_min = (char *) realloc (__min, new_size);

	  if (new_min != NULL)
	    {
	      disp = new_min - (char *) __min;
              if (new_min != (char *) __min)
                {
	          new_fri = (char *) __fri + disp;
	          new_min = (char *) __min + disp;

		  add_to_pointers ();

		  __fri = (__dhp) new_fri;
		  __min = (__dhp) new_min;
		}

	      /* Nuller resten av omr}det */
	      ph = (long *) ((char *) __max + disp);
	      __max = (__dhp) (new_min + new_size);
	      bzero ((char *) ph, (char *) __max - (char *) ph);
	      __chpoolsize = __TRUE;
	      __poolsize = new_size / 1024;
	    }
	}
#endif
      if (((char *) __fri + size) > (char *) __max)
	__rerror ("Alloc: Not enough space for the heap");
    }
  mem = __fri;
  __fri = (__dhp) (((char *) __fri) + size);
  return (mem);
}

#endif


/******************************************************************************
                                                            RutineCallThunk  */

/* Denne rutinen oppretter et thunk-objekt, og setter dens statiske og
 * dynamiske omgivelse. I tillegg kalles rss hvis det ligger noe p} en av rt-
 * stakkene. Den dynamiske omgivelsen er gitt av __pb mens den statiske
 * er overf|rt i den globale variablen sl.
 *
 * Parametere:
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * Returnerer: Ingen ting. */

#if nolib|rctrut
void
__rct (as)
     long as;
{
  __dhp x;
  if (as)
    __rss (as);
  x = __ralloc ((long) sizeof (__thunk));
  x->pp = (__pty) __THUNK;
  x->sl = __sl;
  x->dl = __pb;
  /* p->pm=__FALSE; Er implisitt satt */
  __pb = x;
}
#endif

/******************************************************************************
                                                        RutineEndThunk       */

/* Denne rutinen kalles for aa terminere en thunk og pr|ver } deallokere
 * objektet. */

#if nolib|rethrut
void
__reth ()
{
  __dhp x;
  __goto = __pb->ex;
  x = __pb;
  __pb = __pb->dl;
  if (__pb->pp == (__pty) __ACTS)
    __rrs ();
  __lb = __pb;
  while (__lb->pm || __lb->pp == (__pty) __ACTS)
    __lb = __lb->dl;
  if ((char *) __fri == (((char *) x) + allign (sizeof (__thunk))))
    {
      bzero ((char *) x, (char *) __fri - (char *) x);
      __fri = x;
    }
}
#endif


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

#if nolib|rgetavrut
char
__rgetav (ftype, p, as, ret, mret)
     char ftype;
     __aritnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  char tconv;
  switch (p->h.namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      tconv = p->conv;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse
				 * verdiene f|r kallet. */

      __goto = p->h.adr;	
      __sl = p->h.sl;
      __rct (as);		/* Oppretter objektet */
      ((__thunkp) __pb)->conv = tconv;
      ((__thunkp) __pb)->ftype = ftype;	/* Overf|rer den formelle
					 * typen og returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
#if FLOAT_IMPLEMENTED
      if (ftype == __TINTG)
	__ev.i = (p->conv == __NOCONV 
		  ? *(long *) (((char *) p->h.bp) + p->h.v.ofs) :
#ifdef REALASDOUBLE
		  (long) *(double *) (((char *) p->h.bp) + p->h.v.ofs));
      else
	__ev.f = (p->conv == __NOCONV 
		  ? *(double *) (((char *) p->h.bp) + p->h.v.ofs) :
		  p->conv == __INTREAL 
		  ? (double) *(long *) (((char *) p->h.bp) + p->h.v.ofs) :
	     (double) (long) *(double *) (((char *) p->h.bp) + p->h.v.ofs));
#else
		  (long) *(float *) (((char *) p->h.bp) + p->h.v.ofs));
      else
	__ev.f = (p->conv == __NOCONV 
		  ? *(float *) (((char *) p->h.bp) + p->h.v.ofs) :
		  p->conv == __INTREAL 
		  ? (float) *(long *) (((char *) p->h.bp) + p->h.v.ofs) :
	       (float) (long) *(float *) (((char *) p->h.bp) + p->h.v.ofs));
#endif
#endif
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    case __VALUE_NOTHUNK:
#if FLOAT_IMPLEMENTED
      if (ftype == __TINTG)
#ifdef REALASDOUBLE
	__ev.i = (p->conv == __NOCONV ? p->h.v.i : (long) (double) p->h.v.f);
#else
	__ev.i = (p->conv == __NOCONV ? p->h.v.i : (long) (float) p->h.v.f);
#endif
      else
	__ev.f = (p->conv == __NOCONV ? p->h.v.f : p->conv == __INTREAL ?
#ifdef REALASDOUBLE
		  (double) p->h.v.i : (double) (long) p->h.v.f);
#else
      (float) p->h.v.i:(double) (long) p->h.v.f);
#endif
#endif
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
#endif

/******************************************************************************
                                                  RutineGetCharBooleanValue  */

/* Denne rutinen kalles ved lese-aksess av enkle name-parametere av type
 * character og boolean. Rutinen avgj|r om en thunk skal kalles eller ikke p}
 * bakgrunn av p->namekind attributtet.
 * Hvis en thunk skal kalles, opprettes et objekt og f|lgende verdier legges i
 * objektet: returadressen. Verdien returneres i __ev.c
 * Hvis ikke en thunk skal kalles s} henter rutinen verdien, enten gitt
 * direkte av p->v.cvalue, eller i lokasjon (p->bp+p->v.ofs), og   den legges
 * ned i __ev.c.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret  -> Returadressen
 * Returnerer: Verdien i ec, (ogs} hvis en thunk kalles) */

#if nolib|rgetcbvrut
char
__rgetcbv (p, as, ret, mret)
     __charboolnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->h.namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      __goto = p->h.adr;	/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik at p
				 * ikke lenger peker riktig,
				 * leses disse verdiene f|r kallet. */
      __sl = p->h.sl;
      __rct (as);		/* Oppretter objektet og overf|rer
				 * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __ev.c = *(char *) (((char *) p->h.bp) + p->h.v.ofs);
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    case __VALUE_NOTHUNK:
      __ev.c = p->h.v.c;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
#endif

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

#if nolib|rgetrvrut
char
__rgetrv (p, as, ret, mret)
     __refnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  char tconv;
  __pty q;

  switch (p->h.namekind)
    {
    case __VALUE_THUNK:
    case __ADDRESS_THUNK:
      q = p->q;
      tconv = p->conv;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik at p ikke
				 * lengerpeker riktig,
				 * leses disse verdiene f|r kallet. */
      __goto = p->h.adr;
      __sl = p->h.sl;
      __rct (as);		/* Oppretter objektet */
      ((__thunkp) __pb)->conv = tconv;
      ((__thunkp) __pb)->q = q;	/* Overf|rer strengeste kvalifikasjon p}
				 * aksessveien, og returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __er = *(__dhp *) (((char *) p->h.bp) + p->h.v.ofs);
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
}
#endif

/******************************************************************************
                                                    RutineGetTextAdress      */

#if nolib|rgettarut
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
#endif

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

#if nolib|rgettvrut
char
__rgettv (p, as, ret, mret)
     __textnamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->h.namekind)
    {
    case __VALUE_THUNK:
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
      __lb = __pb; 		/* Gj|r thunken eksikverbar.  */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __et = *(__txt *)(((char *) p->h.bp) + p->h.v.ofs);
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
#endif

/******************************************************************************
                                                    RutineGetProcedure       */

/* Denne rutinen kalles ved aksess av en prosedyre name-parameter.
 * Rutinen avgj|r om en thunk skal kalles eller ikke p} grunnlag av
 * p->namekind attributtet, som kan ha to verdier: ADRESS_THUNK og
 * ADDRESS_NOTHUNK.
 * Hvis en thunk skal kalles, opprettes et objekt og returadressen legges i
 * objektet. Prototypen til prosedyen returneres i pp, mens dens statiske link
 * returneres i sl.
 * Hvis ikke en thunk skal kalles s} er prototypen gitt av p->pp og statisk
 * link er gitt av p->psl.Disse tilordnes ogs} henholdsvis pp og sl.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret -> Returadressen
 * Returnerer: Peker til prototypen i pp og prosedyrens statiske link i sl*/

#if nolib|rgetprocrut
char
__rgetproc (p, as, ret, mret)
     __procname *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->namekind)
    {
    case __ADDRESS_THUNK:
      __goto = p->adr;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse
				 * verdiene f|r kallet. */
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet og overf|rer
				 * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar.              */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __pp = p->pp;
      __sl = p->psl;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
#endif

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

#if nolib|rgetlabrut
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
#endif

/******************************************************************************
                                                    RutineGetArray           */

/* Denne rutinen kalles ved aksess av en array name-parameter.
 * Rutinen avgj|r om en thunk skal kalles eller ikke p} grunnlag av
 * p->namekind attributtet, som kan ha to verdier: ADDRESS_THUNK og
 * ADDRESS_NOTHUNK.
 * Hvis en thunk skal kalles, opprettes et objekt og returadressen legges i
 * objektet. Adressen til arrayet returneres i er.
 * Hvis ikke en thunk skal kalles s} er arrayet gitt av p->ap, som tilordnes
 * er.
 * Parametere:
 * p          -> Peker til den formelle parameter
 * ar, av, at -> henholdsvis antall referanse, value og text-variable
 *               som ligger p} rt-stakkene.
 * ret, mret -> Returadressen
 * Returnerer: Peker til arrayet i er. */

#if nolib|rgetarut
char
__rgeta (p, as, ret, mret)
     __arraynamepar *p;
     long as;
     int ret;
     void (*mret) ();
{
  switch (p->namekind)
    {
    case __ADDRESS_THUNK:
      __goto = p->adr;		/* I tilfelle at kallet p} rct f|rer til
				 * garbage collection, slik  at p ikke
				 * lenger peker riktig, leses disse
				 * verdiene f|r kallet. */
      __sl = p->sl;
      __rct (as);		/* Oppretter objektet og overf|rer 
				 * returadressen. */
      ((__thunkp) __pb)->h.ex.ment = mret;
      ((__thunkp) __pb)->h.ex.ent = ret;
      __lb = __pb;		/* Gj|r thunken eksikverbar. */
      return (__TRUE);
    case __ADDRESS_NOTHUNK:
      __er = (__dhp) p->ap;
#if SPLIT_MODUL
      __goto.ent = ret;
      __goto.ment = mret;
#endif
      return (__FALSE);
    }
  /* NOTREACHED */
}
#endif

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

#if nolib|rgetsarut
char
__rgetsa (p, as, ret, mret)
     __simplenamepar *p;
     long as;
     int ret;
     void (*mret) ();
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
#endif

#if nolib|rdumprut
#if DUMP
#ifdef FIRST_DATA_LOCATION
extern __start_data_segment;
#if HAVE_FCNTL_H
#include <fcntl.h>
#else
#include <sys/file.h>
#endif

/******************************************************************************
                                                       DUMP & UNDUMP         */

/*  Disse prosedyrene kan n}es fra simula ved kall p} Dump og Undump.	
 *  Disse gir mulighet for } lagre en tilstand til en simulakj|ring til fil,
 *  for senere } laste tilstanden inn igjen med undump. For at dette skal g}
 *  bra m} man passe p} at alle filer untatt sysin og sysout er lukket.	
 *  ]pne filer er ikke noe problem for dump, men etter undump s}
 *  starter systemet i en tilstand hvor den tror at et antall filer	
 *  er }pne. Det kan kanskje g} bra hvis de samme filene ogs} var	
 *  }pne f|r kall p} undump, men man m} da v{re sv{rt n|ye med rekkef|lgen
 *  disse filene er }pnet, slik at riktig fildeskriptor blir		
 *  assosiert til riktig fil etter undump.				
 *				
 *  Syntaksen p} dump-filen er som f|lger:    				
 *  Filen starter med to linjer der det p} f|rste linje st}r "/\*Cim_dump*\/".
 *  P} andre linje er det skrevet adressen til |vre kanten p} data-segmentet.
 *  Nedre del av datasegmentet finnes					
 *  ved } ta adressen til variabelen __start_data_segment,		
 *  som er en initialisert variabel som er deklarert f|rst i main-modul	
 *  Deretter kommer innholdet av selve data-segmentet byte for byte,	
 *  men slik at null byte er pakket: For hver ^@ p} filen angir neste	
 *  byte antallet null byte pluss en.			
 *  Deretter f|lger sekvensen "\377\376\375" ... "^c^b^a".		
 *  Til slutt er det fylt opp med bytes opp til en full blokkst|rrelse.	
 *  Disse bytene er en n|yaktig kopi av de i forrige blokk.		
 *  Blokkst|rrelsen er satt til 4096 bytes */

#define DUMP_STR "/*Cim_dump*/\n%12d\n"
#define DUMP_STR_LEN 26
#define DUMP_BUFFER_SIZE 4096

__rdump (t, ret, mret)
     __txtvp t;
     int ret;
     void (*mret) ();
{
  char s[DUMP_BUFFER_SIZE];
  long sp,
    p,
    anull = 0;
  char c;
  long f,
    first_loc = FIRST_DATA_LOCATION,
    last_loc = (long) ((char *) sbrk (0) + 4);
  __goto.ent = ret;
  __goto.ment = mret;
  __rgbc ();
  if ((f = open (__rcopytexttoc (t), O_WRONLY | O_CREAT | O_TRUNC, 0700)) 
      == EOF)
    {
      __rwarning ("Dump: Not possible to open dump-file");
      return;
    }
  sprintf (s, DUMP_STR, last_loc);
  sp = DUMP_STR_LEN;
  for (p = first_loc; p < last_loc;)
    {
      c = *(char *) p;
      if (anull)
	if (c != '\0' || anull == 256)
	  {
	    c = (char) (anull - 1);
	    anull = 0;
	  }
	else
	  {
	    anull++;
	    p++;
	    continue;
	  }
      else
	{
	  if (c == '\0')
	    anull++;
	  p++;
	}
      s[sp++] = c;
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
	    {
	      __rwarning ("Dump: Error on writing to dump-file");
	      close (f);
	      return;
	    }
	  sp = 0;
	}
    }
  if (anull)
    s[sp++] = (char) (anull - 1);
  for (c = '\377'; c != '\0'; c--)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
	    {
	      __rwarning ("Dump: Error on writing to dump-file");
	      close (f);
	      return;
	    }
	  sp = 0;
	}
      s[sp++] = c;
    }
  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
    __rwarning ("Dump: Error on writing to dump-file");
  close (f);
}

__rundump (t, ret, mret)
     __txtvp t;
     int ret;
     void (*mret) ();
{

  char s[DUMP_BUFFER_SIZE];
  long sp,
    p,
    skriv_nuller = __FALSE;
  char c;
  long f,
    first_loc = FIRST_DATA_LOCATION,
    last_loc;
  if ((f = open (__rcopytexttoc (t), O_RDONLY, 0)) == EOF)
    __rerror ("Rundump: Not possible to open dump-file");
  if (read (f, s, DUMP_BUFFER_SIZE) < 0)
    __rerror ("Rundump: Error while reading from dump-file");
  if (sscanf (s, DUMP_STR, &last_loc) < 0)
    __rerror ("Rundump: Illegal format on dump-file");
  brk (last_loc);
  sp = DUMP_STR_LEN;
  for (p = first_loc; p < last_loc;)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (read (f, s, DUMP_BUFFER_SIZE) == EOF)
	    __rerror ("Rundump: Error while reading from dump-file");
	  sp = 0;
	}
      c = s[sp++];

      if (skriv_nuller)
	{
	  char i;
	  for (i = '\0'; i != c; i++)
	    {
	      *(char *) p = '\0';
	      p++;
	      if (p == last_loc)
		__rerror ("Rundump: Dump-file don't match data-region");
	    }
	  *(char *) p = '\0';
	  p++;
	  skriv_nuller = __FALSE;
	}
      else if (c == '\0')
	skriv_nuller = __TRUE;
      else
	{
	  *(char *) p = c;
	  p++;
	}
    }
  for (c = '\377'; c != '\0'; c--)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (read (f, s, DUMP_BUFFER_SIZE) == EOF)
	    __rerror ("Rundump: Error while reading from dump-file");
	  sp = 0;
	}
      if (s[sp++] != c)
	__rerror ("Rundump: Dump-file don't match data-region");
    }

  close (f);
}

#endif
#endif
#endif

/******************************************************************************
                                                              ARGC & ARGV    */

#if nolib|rargcrut

long
__rargc ()
{
  return (__argc);
}

#endif
#if nolib|rargvrut

long
__rargv ()
{
  return (__argv);
}

#endif

/******************************************************************************
                                                                      RETURN */

#if nolib|rreturnrut
__rreturn (vret, ret, mret)
     long vret;
     int ret;
     void (*mret) ();
{
  __vreturn = vret;
  __return.ent = ret;
  __return.ment = mret;
  __goto.ent = -1;
  __goto.ment = __NULL;
}

#endif
