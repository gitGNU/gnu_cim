/* $Id: simfile.c,v 1.7 1994/11/05 12:57:21 cim Exp $ */

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

#include "config.h"
#include "cim.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

void __m_FILE()
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
__l23: ((__bs8 *)__lb)->endfile=__TRUE;
       ((__bs1 *)__lb)->shared=__SHARED;
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

exit(SH_TRUE);

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

void __init_FILE()
{
  /*  I N I T I E R I N G   a v   B A S I C I O */
  
  /* Initiering av prototyper for omgivelsene (class ENVIRONMENT ) */
  /* Ytterste blokk */
  __rl0FILE[0]=(char *)&__bp0FILE->c1-(char *)__bp0FILE;
  __rl0FILE[1]=(char *)&__bp0FILE->c2-(char *)__bp0FILE;
  __pl0FILE[0] = &__p0FILE;
  __p0FILE.adr.ment=__m_FILE;
  /* class FILE */
  __rl1FILE[0]=(char *)&__bp1FILE->filename-(char *)__bp1FILE;
  __pl1FILE[0] = &__p1FILE;
  __p1FILE.adr.ment=__m_FILE;
  /* FILE class IMAGEFILE */
  
  __rl2FILE[0]=(char *)&__bp2FILE->IMAGE-(char *)__bp2FILE;
  __pl2FILE[1] = &__p2FILE; 
  __p2FILE.adr.ment=__m_FILE;
  /* IMAGEFILE class OUTFILE */
  
  __pl3FILE[2] = &__p3FILE;
  __p3FILE.adr.ment=__m_FILE;
  /* IMAGEFILE class INFILE */
  
  __pl4FILE[2] = &__p4FILE;
  __p4FILE.adr.ment=__m_FILE;
  /* IMAGEFILE class DIRECTFILE */
  
  __pl5FILE[2] = &__p5FILE;
  __p5FILE.adr.ment=__m_FILE;
  /* OUTFILE class PRINTFILE */
  
  __pl6FILE[3] = &__p6FILE;
  __p6FILE.adr.ment=__m_FILE;
  /* FILE class BYTEFILE  */
  
  __pl7FILE[1] = &__p7FILE;
  __p7FILE.adr.ment=__m_FILE;
  /* BYTEFILE class INBYTEFILE */
  
  __pl8FILE[2] = &__p8FILE;
  __p8FILE.adr.ment=__m_FILE;
  /* BYTEFILE class OUTBYTEFILE */
  
  __pl9FILE[2] = &__p9FILE;
  __p9FILE.adr.ment=__m_FILE;
  /* BYTEFILE class DIRECTBYTEFILE */
  
  __pl10FILE[2] = &__p10FILE;
  __p10FILE.adr.ment=__m_FILE;
}
