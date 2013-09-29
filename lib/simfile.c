/* $Id: simfile.c,v 1.7 1994/11/05 12:57:21 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen
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

#define INCLUDE_SIMFILE_H
#include "config.h"
#include "cim.h"
#include "limit.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

void __m_FILE();

/* Ytterste blokk */
__bs0FILE *__bp0FILE;
short __rl0FILE[2]
= {(char *)&((__bs0FILE *)0)->c1-(char *)0, (char *)&((__bs0FILE *)0)->c2-(char *)0};
extern __ptyp __p0FILE;
__pty __pl0FILE[1]
={&__p0FILE};
__ptyp __p0FILE =
{'B', 0, 0, sizeof (__bs0FILE), 1, __m_FILE, 0, 2, 2, 
  0, __rl0FILE, 0, __pl0FILE};

/* class FILE */
__bs1FILE *__bp1FILE;
short __rl1FILE[1]
= {(char *)&((__bs1FILE *)0)->filename-(char *)0};
extern __ptyp __p1FILE;
__pty __pl1FILE[1]
= {&__p1FILE};
__ptyp __p1FILE =
{'C', 0, 1, sizeof (__bs1FILE), 1, __m_FILE, 0, 0, 1, 
  0, __rl1FILE, 0, __pl1FILE};

/* FILE class IMAGEFILE */
__bs2FILE *__bp2FILE;
short __rl2FILE[1]
= {(char *)&((__bs2FILE *)0)->IMAGE-(char *)0};
extern __ptyp __p2FILE;
__pty __pl2FILE[2] =
{&__p1FILE, &__p2FILE};
__ptyp __p2FILE =
{'C', 1, 1, sizeof (__bs2FILE), 4, __m_FILE, 0, 0, 1, 
  0, __rl2FILE, 0, __pl2FILE};

/* IMAGEFILE class OUTFILE */
extern __ptyp __p3FILE;
__pty __pl3FILE[3] =
{&__p1FILE, &__p2FILE, &__p3FILE};
__ptyp __p3FILE =
{'C', 2, 1, sizeof (__bs3FILE), 7, __m_FILE, 0, 0, 0, 0, 0, 0, __pl3FILE};

/* IMAGEFILE class INFILE */
extern __ptyp __p4FILE;
__pty __pl4FILE[3] =
{&__p1FILE, &__p2FILE, &__p4FILE};
__ptyp __p4FILE =
{'C', 2, 1, sizeof (__bs4FILE), 10, __m_FILE, 0, 0, 0, 0, 0, 0, __pl4FILE};

/* IMAGEFILE class DIRECTFILE */
extern __ptyp __p5FILE;
__pty __pl5FILE[3] =
{&__p1FILE, &__p2FILE, &__p5FILE};
__ptyp __p5FILE =
{'C', 2, 1, sizeof (__bs5FILE), 13, __m_FILE, 0, 0, 0, 0, 0, 0, __pl5FILE};

/* OUTFILE class PRINTFILE */
extern __ptyp __p6FILE;
__pty __pl6FILE[4] =
{&__p1FILE, &__p2FILE, &__p3FILE, &__p6FILE};
__ptyp __p6FILE =
{'C', 3, 1, sizeof (__bs6FILE), 16, __m_FILE, 0, 0, 0, 0, 0, 0, __pl6FILE};

/* FILE class BYTEFILE  */
extern __ptyp __p7FILE;
__pty __pl7FILE[2] =
{&__p1FILE, &__p7FILE};
__ptyp __p7FILE =
{'C', 1, 1, sizeof (__bs7FILE), 19, __m_FILE, 0, 0, 0, 0, 0, 0, __pl7FILE};

/* BYTEFILE class INBYTEFILE */
extern __ptyp __p8FILE;
__pty __pl8FILE[3] =
{&__p1FILE, &__p7FILE, &__p8FILE};
__ptyp __p8FILE =
{'C', 2, 1, sizeof (__bs8FILE), 22, __m_FILE, 0, 0, 0, 0, 0, 0, __pl8FILE};

/* BYTEFILE class OUTBYTEFILE */
extern __ptyp __p9FILE;
__pty __pl9FILE[3] =
{&__p1FILE, &__p7FILE, &__p9FILE};
__ptyp __p9FILE =
{'C', 2, 1, sizeof (__bs9FILE), 25, __m_FILE, 0, 0, 0, 0, 0, 0, __pl9FILE};

/* BYTEFILE class DIRECTBYTEFILE */
extern __ptyp __p10FILE;
__pty __pl10FILE[3] =
{&__p1FILE, &__p7FILE, &__p10FILE};
__ptyp __p10FILE =
{'C', 2, 1, sizeof (__bs10FILE), 28, __m_FILE, 0, 0, 0, 0, 0, 0, __pl10FILE};

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

__bs0FILE __blokk0FILE =
{(__pty) & __p0FILE, (__dhp) & __blokk0FILE, (__dhp) & __blokk0FILE, 
  __FALSE, __TERMINATED, 0, 0,
  (__dhp) & __blokk0FILE, __NULL, __NULL};

/* Genererer og initierer et infile objekt (SYSIN) */

__bs4FILE __sysin =
{(__pty) & __p4FILE, (__dhp) & __sysin, (__dhp) & __blokk0FILE, 
  __FALSE, __TERMINATED, 0, __NULL,
  (__dhp) & __blokk0FILE, (__textref) & __stdinname, 5, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stdinimage, INPUT_LINE_LENGTH, INPUT_LINE_LENGTH + 1, 1,
 __FALSE};

/* Genererer og initierer et printfile objekt (SYSOUT) */

__bs6FILE __sysout =
{(__pty) & __p6FILE, (__dhp) & __sysout, (__dhp) & __blokk0FILE, 
 __FALSE, __TERMINATED, 0, __NULL,
 (__dhp) & __blokk0FILE, (__textref) & __stdoutname, 6, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stdoutimage, OUTPUT_LINE_LENGTH, 1, 1,
 1, MAX_INT, 1, 0};

__bs6FILE __syserr =
{(__pty) & __p6FILE, (__dhp) & __syserr, (__dhp) & __blokk0FILE, 
 __FALSE, __TERMINATED, 0, __NULL,
 (__dhp) & __blokk0FILE, (__textref) & __stderrname, 6, 1, 1, __NULL, __TRUE,
 __NOSHARED, __NOAPPEND, __NOCREATE, __READWRITE, __NOREWIND, __NOPURGE,
 (__textref) & __stderrimage, OUTPUT_LINE_LENGTH, 1, 1,
 1, MAX_INT, 1, 0};

void __m_FILE(void)
{ goto __s;

/* C - k o d e n   f o r   C L A S S   B A S I C I O */

                                          /* class FILE */
__l1: __renddecl(0);goto __sw;
__l2: if(((__bs1 *)__lb)->filename.obj==__NULL)
       __rerror("NEW File: Can't open file with no name");
      __rinner(0);goto __sw;
__l3: __rendclass(0);goto __sw; 
                                          /* FILE class IMAGEFILE */

__l4: __renddecl(1);goto __sw;
__l5: __rinner(1);goto __sw;
__l6: __rendclass(0);goto __sw; 
                                          /* IMAGEFILE class OUTFILE */

__l7: __renddecl(2);goto __sw;
__l8: ((__bs1 *)__lb)->create=__ANYCREATE;
      __rinner(2);goto __sw;
__l9: __rendclass(0);goto __sw;
                                          /* IMAGEFILE class INFILE */

__l10: __renddecl(2);goto __sw;
__l11: ((__bs4 *)__lb)->endfile=__TRUE;
       ((__bs1 *)__lb)->shared=__SHARED;
       __rinner(2);goto __sw; 
__l12: __rendclass(0);goto __sw;
                                          /* IMAGEFILE class DIRECTFILE */

__l13: __renddecl(2);goto __sw;
__l14: ((__bs5 *)__lb)->endfile=__TRUE;
       __rinner(2);goto __sw; 
__l15: __rendclass(0);goto __sw;
                                          /* OUTFILE class PRINTFILE */

__l16: __renddecl(3);goto __sw;
__l17: ((__bs6 *)__lb)->spacing=1;
       ((__bs6 *)__lb)->lines_per_page=__lines_per_page;
       ((__bs6 *)__lb)->line=((__bs6 *)__lb)->page=0; 
       __rinner(3);goto __sw; 
__l18: __rendclass(0);goto __sw;
                                          /* FILE class BYTEFILE  */

__l19: __renddecl(1);goto __sw;
__l20: ((__bs7 *)__lb)->endfile=__TRUE;
       ((__bs7 *)__lb)->bytesize=__BYTESIZE;
       __rinner(1);goto __sw; 
__l21: __rendclass(0);goto __sw;
                                          /* BYTEFILE class INBYTEFILE */

__l22: __renddecl(2);goto __sw;
__l23: ((__bs1 *)__lb)->shared=__SHARED;
       __rinner(2);goto __sw; 
__l24: __rendclass(0);goto __sw;

                                          /* BYTEFILE class OUTBYTEFILE */

__l25: __renddecl(2);goto __sw;
__l26: ((__bs1 *)__lb)->create=__ANYCREATE;
       __rinner(2);goto __sw; 
__l27: __rendclass(0);goto __sw;

                                          /* BYTEFILE class DIRECTBYTEFILE */

__l28: __renddecl(2);goto __sw;
__l29: __rinner(2);goto __sw; 
__l30: __rendclass(0);goto __sw;

/* C-koden for koden etter INNER i class BASICIO */
__l31:
__rslutt();

exit(0);

__sw:if(__goto.ment!=(void (*)())__m_FILE)return;
__s:switch(__goto.ent){
case 1: goto __l1;case 2: goto __l2;
case 3: goto __l3;case 4: goto __l4;case 5: goto __l5;
case 6: goto __l6;case 7: goto __l7;case 8: goto __l8;
case 9: goto __l9;case 10: goto __l10;case 11: goto __l11;
case 12: goto __l12;case 13: goto __l13;case 14: goto __l14;
case 15: goto __l15;case 16: goto __l16;case 17: goto __l17;
case 18: goto __l18;case 19: goto __l19;case 20: goto __l20;
case 21: goto __l21;case 22: goto __l22;case 23: goto __l23;
case 24: goto __l24;case 25: goto __l25;case 26: goto __l26;
case 27: goto __l27;case 28: goto __l28;case 29: goto __l29;
case 30: goto __l30;case 31: goto __l31;
}
}

void __init_FILE (void)
{
  /*  I N I T I E R I N G   a v   B A S I C I O */
#if 0  
  __rl0FILE[0]=(char *)&__bp0FILE->c1-(char *)__bp0FILE;
  __rl0FILE[1]=(char *)&__bp0FILE->c2-(char *)__bp0FILE;

  /* class FILE */
  __rl1FILE[0]=(char *)&__bp1FILE->filename-(char *)__bp1FILE;

  /* FILE class IMAGEFILE */
  
  __rl2FILE[0]=(char *)&__bp2FILE->IMAGE-(char *)__bp2FILE;

#endif
}
