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

#ifndef DEKL_H
#define DEKL_H

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

typedef union val
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
    char *ident;
    struct
    {
      short label, inthunk;
    } thunk;
  } val_t;

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

#define KNOKD    'N'
#define KSIMPLE  'S'
#define KARRAY   'A'
#define KPROC    'P'
#define KCLASS   'C'
#define KBLOKK   'B'
#define KPRBLK   'R'
#define KFOR     'F'
#define KFORLIST 'G'
#define KINSP    'I'
#define KCON     'O'
#define KERROR   'E'

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

typedef struct _block block_t;

typedef struct _decl decl_t;
struct _decl
  {
    char *ident;		/* Navnet til deklarasjonen */
    long plev;			/* Prefix niv�, kan fjernes */
    char *identqual;		/* Navnet til kvalifikasjonen */
    block_t *encl;		/* Omslutningen til deklarasjonen */
    block_t *descr;	/* Representasjonen av deklarasjonen */
    decl_t *match;		/* Matsjende deklarasjoner (virtual) */
    decl_t *next;		/* Neste deklarasjon i lista */
    decl_t *prefqual;	/* Prefiks-kjede eller kval. til peker */
    val_t value;		/* Verdi for konstantdeklarasjon */
    long line;			/* Linjenommer */
    short dim;			/* Dimensjonen */
    short virtno;		/* Virtuelt nommer */
    char type,
      kind,
      categ;			/* Hvilken type deklarasjon */
    char protected;		/* Angir om attributtet er protected */
  };

struct _block
  {
    decl_t quant;		/* Deklarasjonen til denne blokken */
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
      connest;			/* Dybde p� nesting av for-l�kke og inspect */
    short ent;			/* entringspunkt */
    short navirtlab;
#if ACSTACK_IN_OBJ
    short maxusedref;
    short maxusedtxt;
    short maxusedval;
#endif
    char *rtname;

    char *timestamp;
    char *filename;
    decl_t *when;
    decl_t *parloc;	/* Parametere og lokale deklarasjoner */
    decl_t *lastparloc;
    decl_t *virt;		/* Blokkens virtuelle bindinger */
    decl_t *lastvirt;
    decl_t *hiprot;	/* Blokkens protections */
    block_t *next_block;	/* Neste blokk i sekvensen */
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


void begin_block (char kind);
void end_block (char *rtname, char codeclass);
void reg_decl (char *ident, char type, char kind, char categ);
void reg_inner (void);
decl_t *new_decl (void);
block_t *firstclass (void);
void in_block (void);
void out_block (void);

void remove_block (block_t *rb);

extern char *prefquantident;
extern int localused;

extern block_t *cblock;
extern block_t *ssblock;
extern block_t *sblock;
extern block_t *eblock;
extern int cblev;
char subclass (decl_t *rdx, decl_t *rdy);
extern char insert_with_codeclass;
extern decl_t *cprevdecl;

extern decl_t *commonprefiks;
decl_t *commonqual (decl_t *rdx, decl_t *rdy);
decl_t *find_global (char *ident, char virt);
decl_t *find_local (char *ident, decl_t *rd, char virt);
decl_t *find_decl (char *ident, block_t *rb, char virt);

decl_t *first_param (decl_t *rd);
decl_t *next_param (decl_t *rd);
int more_param (decl_t *rd);

extern int arrdim;
extern decl_t *last_array;
void set_array_dim (int arrdim);
char danger_proc (decl_t *rd);

extern block_t *seenthrough;
int body (decl_t *rd);
extern decl_t *classtext;
char subordinate (decl_t *rda, decl_t*rdb);
int same_param (block_t *rb1, block_t *rb2);
void decl_init (void);
void decl_init_pass2 (void);
void decl_init_pass1 (void);
void decl_reinit (void);

void regwhen (block_t *rb, decl_t *rd);
void reginsp (block_t *rb, decl_t *rd);
decl_t *reg_this (char *ident);

extern char yaccerror;

char *xmalloc (unsigned int size);
#endif
