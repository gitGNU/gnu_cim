/* $Id: cdekl.h,v 1.9 1997/01/08 09:49:15 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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

#if STDC_HEADERS
#include <float.h>
#endif

#if HAVE_VALUES_H
#include <values.h>
#else
#include <math.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

union val
  {
    long ival;
    double rval;
    struct
      {
	char *txt;
	int id;
      }
    tval;
    int entry;
    long n_of_stack_elements;
    struct
    {
      short val_entry, ref_entry, txt_entry;
    }
    stack;
    struct
    {
      long n_of_stack_elements;
      short entry;
    }
    combined_stack;
    char *ident;
    struct
    {
      short label, inthunk;
    } thunk;
  };

/* Definition of type */

#define TALLTY	 'A'
#define TNOTY    'N'
#define TINTG    'I'
#define TSHORT   'I'		/* S */
#define TREAL    'R'
#define TLONG    'R'		/* D */
#define TBOOL    'B'
#define TCHAR    'C'
#define TLABEL   'L'
#define TTEXT    'T'
#define TREF     'P'
#define TVOIDP   'O'
#define TERROR   'E'
#define TVARARGS 'V'

/* Definition of kind */

#define KNOKD   'N'
#define KSIMPLE 'S'
#define KARRAY  'A'
#define KPROC   'P'
#define KCLASS  'C'
#define KBLOKK  'B'
#define KPRBLK  'R'
#define KFOR    'F'
#define KINSP   'I'
#define KCON    'O'
#define KERROR  'E'

/* Definition of categ */

#define CCONSTU   'U'
#define CCONST    'C'
#define CLOCAL    'L'
#define CDEFLT    'D'
#define CVALUE    'V'
#define CNAME     'N'
#define CVAR      'B'
#define CVIRT     'I'
#define CEXTR     'X'
#define CEXTRMAIN 'M'
#define CERROR    'E'
#define CSPEC     'S'
#define CHIDEN    'H'
#define CPROT     'P'
#define CHIPRO    'A'
#define CNEW      'W'
#define CCPROC	  'R'
#define CEXTROUT  'F'

struct DECL
  {
    char *ident;		/* Navnet til deklarasjonen */
    long plev;			/* Prefix nivå, kan fjernes */
    char *identqual;		/* Navnet til kvalifikasjonen */
    struct BLOCK *encl;		/* Omslutningen til deklarasjonen */
    struct BLOCK *descr;	/* Representasjonen av deklarasjonen */
    struct DECL *match;		/* Matsjende deklarasjoner (virtual) */
    struct DECL *next;		/* Neste deklarasjon i lista */
    struct DECL *prefqual;	/* Prefiks-kjede eller kval. til peker */
    union val value;		/* Verdi for konstantdeklarasjon */
    long line;			/* Linjenommer */
    short dim;			/* Dimensjonen */
    short virtno;		/* Virtuelt nommer */
    char type,
      kind,
      categ;			/* Hvilken type deklarasjon */
    char protected;		/* Angir om attributtet er protected */
  };

struct BLOCK
  {
    struct DECL quant;		/* Deklarasjonen til denne blokken */
    char inner;
    char localclasses,
      thisused;
    char stat;
    char structure_written;

    char codeclass;
    short navirt;		/* Antall virtuelle bindinger proc - lab/swi */
    short ptypno;
    short blno,
      blev;			/* Blokknommer og blokknivaa */
    short napar;		/* Antall parametere */
    short fornest,
      connest;			/* Dybde på nesting av for-løkke og inspect */
    short ent;			/* entringspunkt */
    short navirtlab;

    char *rtname;

    char *timestamp;
    char *filename;
    struct DECL *when;
    struct DECL *parloc;	/* Parametere og lokale deklarasjoner */
    struct DECL *lastparloc;
    struct DECL *virt;		/* Blokkens virtuelle bindinger */
    struct DECL *lastvirt;
    struct DECL *hiprot;	/* Blokkens protections */
    struct BLOCK *next_block;	/* Neste blokk i sekvensen */
  };

/* Kodeklasser */
#define CCNO         0
#define CCSIMPLE     11		/* Ingen ekstra parametere mod, rem, abs
				 * o.s.v. */
#define CCSIMPLEDANGER 10
#define CCBLANKSCOPY 1		/* Kall p} Blanks eller Copy. Kan starte GBC.
				 * Tillordner antvalue, antref og anttext */
#define CCTEXT       2		/* Peker til tekst-variabel som f|rste
				 * parameter */
#define CCTEXTDANGER 12
#define CCFILEBLANKSCOPY 13	/* Kall p} filename */
#define CCFILE       9		/* Peker til file-klasse som f|rste parameter 
				 */
#define CCFILEDANGER 3
#define CCDETACH     4		/* Peker til klasse som skal detach'es som
				 * parameter */
#define CCCALLRESUME 5		/* Siste parameter er returadressen */
#define CCEXIT	     6		/* Main modul: goto ll<exitlabel>; Ekstern
				 * modul: lgoto=<exitlabel>;mgoto=0;return; */
#define CCRANDOMRUTDANGER 7	/* Ranom rutinene har siste parameter 
				 * overfort by name */
#define CCCPROC      8		/* Ekstern C-prosedyre */


extern beginBlock ();
extern endBlock ();
extern regDecl ();
extern regInner ();
extern struct DECL *newDecl ();
extern struct BLOCK *firstclass ();
extern inBlock ();
extern outBlock ();
extern struct DECL *regThis ();

extern removeBlock ();

extern char *prefquantident;
int localused;

extern struct BLOCK *cblock;
extern struct BLOCK *ssblock;
extern struct BLOCK *sblock;
extern struct BLOCK *eblock;
extern int cblev;
extern char subclass ();
extern char insert_with_codeclass;
extern struct DECL *cprevdecl;

extern struct DECL *commonprefiks;
extern struct DECL *commonqual ();
extern struct DECL *findGlobal ();
extern struct DECL *findLocal ();
extern struct DECL *findDecl ();

extern struct DECL *firstParam ();
extern struct DECL *nextParam ();
extern moreParam ();

extern int arrdim;
extern struct DECL *lastArray;
extern setArrayDim ();
extern char dangerProc ();

extern struct BLOCK *seenthrough;
extern body ();
extern struct DECL *classtext;
extern char subordinate ();
extern sameParam ();
extern reinit ();
extern initDecl ();

extern char yaccerror;
