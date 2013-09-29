/* $Id: cim.h,v 1.15 1996/03/27 16:42:29 cim Exp $ */

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

/******************************************************************************
                                             Definition of runtime-constants */

#define __FALSE 0
#define __TRUE 1
#define __NULL 0L
#define __EOF (-1)

/* Executing must have the value 0 since it is not set */
#define __EXECUTING 0
#define __DETACHED 1
#define __TERMINATED 2
#define __RESUMED 3
#define __CONSTANT 1

#define __ACTS 1
#define __TEXT 3
#define __ARRAY 5
#define __THUNK 7

#define __FROM_START 0
#define __CURRENT_POS 1
#define __AT_END 2

#define __ADDRESS_NOTHUNK 0
#define __ADDRESS_THUNK 1
#define __VALUE_NOTHUNK 2
#define __VALUE_THUNK 3

#define __NOTEST 0
#define __WRITETEST 1
#define __READTEST 2
#define __READWRITETEST 3

#define __NOCONV 0
#define __INTREAL 1
#define __REALINT 2
#define __REALINTREAL 3

#define __RTPUTTEXTLENGTH 150
#define __RTPUTTEXTFORMATLENGTH 10
#define __RTPUTMAXN 100

/* Definition of kind */
#define __KNOKD   'N'
#define __KSIMPLE 'S'
#define __KARRAY  'A'
#define __KPROC   'P'
#define __KCLASS  'C'
#define __KBLOKK  'B'
#define __KPRBLK  'R'
#define __KFOR    'F'
#define __KINSP   'I'
#define __KCON    'O'
#define __KERROR  'E'

/* Definition of type */
#define __TNOTY   'N'
#define __TINTG   'I'
#define __TSHORT  'I'		/* S */
#define __TREAL   'R'
#define __TLONG   'R'		/* D */
#define __TBOOL   'B'
#define __TCHAR   'C'
#define __TLABEL  'L'
#define __TTEXT   'T'
#define __TREF    'P'
#define __TERROR  'E'

#define __NOSHARED  0
#define __SHARED    1

#define __NOAPPEND  0
#define __APPEND    1

#define __NOCREATE  0
#define __CREATE    1
#define __ANYCREATE 2

#define __READWRITE 0
#define __WRITEONLY 1
#define __READONLY  2

#define __NOREWIND  0
#define __REWIND    1

#define __NOPURGE   0
#define __PURGE     1

#define __READ 0
#define __WRITE 1
#define __SEEK 2

#define __BYTESIZE 8

/******************************************************************************
                                     Structures, unions and type definitions */

typedef struct
  {
    short ent;
    void (*ment) ();
  }
__progadr;

typedef union
  {
    long i;
    int c;
    double f;
    short s;
    long l;
    __progadr adr;
    double d;
  }
__valuetype;

/* A prototype-object */

typedef struct __ptys __ptyp,
 *__pty;
struct __ptys
  {
    char kind,
      plev;
    short blev,
      size;
    __progadr adr;
    short fornest,
      connest;
    short naref,
      navirt;
    short *ref;
    __pty *virt;
    __pty *pref;
    __progadr *virtlab;
  };


/* Head of a data-object */

typedef struct __dhs *__dhp;
typedef struct __dhs
  {
    __pty pp;
    __dhp gl,
      dl;
    char pm,
      dt;
    __progadr ex;
    __dhp sl;
  }
__dh;

/* Var parameter of type boolean, char, or text */
typedef struct
  {
    __dhp bp;			/* Pointer to block to akt.par. */
    int ofs;			/* Relativ adress of akt.par   */
  }
__varpar;

/* Aritmetic var parameter */
typedef struct
  {
    __dhp bp;			/* Pointer to block to akt.par. */
    int ofs;			/* Relativ adress of akt.par   */
    char conv;			/* What kind of converting (integer, real) */
  }
__aritvarpar;

/* Ref var parameter */
typedef struct
  {
    __dhp bp;			/* Pointer to block to akt.par. */
    int ofs;			/* Relativ adress of akt.par   */
    __pty q;			/* Pointer to prototype to strongest
				 * qualification  on the access path */
    char conv;			/* When in test must be done */
  }
__refvarpar;


/* Formal prosedure parameter */
typedef struct
  {
    __pty pp;
    __dhp psl;
  }
__forprocpar;

typedef struct
  {
    __pty pp;
    __dhp psl;
    char conv;
  }
__aritprocpar;

typedef struct
  {
    __pty pp;
    __dhp psl;
    char conv;
    __pty q;
  }
__refprocpar;

typedef union
  {
    __valuetype v;
    __dhp r;
  }
__val_or_ref;

/* Head of a stack-object */
typedef struct
  {
    __pty pp;
    __dhp gl,
      dl;
    char pm,
      dt,
      at;			/* pm="ant ref", at="ant text", and dt="ant
				 * value" */
    short size;
  }
__sh, *__shp;

/* A stack-object */
typedef struct
  {
    __sh h;
    __val_or_ref s[1];
  }
__stk, *__stkp;

/* Head of a tekst-object */
typedef struct
  {

    __pty pp;
    __dhp gl;
    char konstant;
    short size;
  }
__th, *__thp;

/* Tekst-object */
typedef struct
  {
    __th h;
    char string[1];
  }
__tobj, *__textref;

/* Tekst-variable */
typedef struct
  {
    __textref obj;
    unsigned short length,
      pos,
      start;
  }
__txt, *__txtvp;

/* Notext */
typedef struct
  {
    __txt tvar;
  }
__notext;

/* Head of an array-object */
typedef struct
  {
    __pty pp;
    __dhp gl;
    int size;
    short dim;
    char type;
  }
__ah, *__ahp;

/* Upper and lower limit for each dimension */
typedef struct
  {
    int low,
      size;
  }
__arrlimit;

/* Head of an array-object */
typedef struct
  {
    __ah h;
    __arrlimit limits[1];
  }
 *__arrp;

/* Name transmission */

typedef union
  {
    int ofs;			/* Offset adress for a variable, bp is base
				 * adress   */
    int i;			/* Valuie for a constant aktual name par */
    double f;
    char c;
    short s;
    long l;
    double d;
    __dhp r;
  }
__ofsorvalue;

typedef struct
  {
    __dh h;
    char conv;			/* What kind of conversions have to be
				 * performed      */
    __pty q;			/* Strongest qualification on the access
				 * path.                  */
    char ftype;			/* Formal type of an aritmetick name. */
    char writeaccess;		/* TRUE or FALSE. For thunks that is
				 * computing adresses. */
    /* Should the thunk return the adress or an value.        */
  }
__thunk, *__thunkp;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp bp;
    __ofsorvalue v;
    char namekind;
  }
__simplenamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp bp;
    __ofsorvalue v;
    char namekind;
  }
__charboolnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp bp;
    __ofsorvalue v;
    char namekind;
    char conv;
  }
__aritnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp bp;
    __ofsorvalue v;
    char namekind;
    __pty q;
    char conv;
  }
__refnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp bp;
    __ofsorvalue v;
    char namekind;
    __txtvp tp;
  }
__textnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __pty pp;			/* Pointer to the procedures prototype    */
    __dhp psl;			/* Procedures static environment          */
    char namekind;
  }
__procname;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __pty pp;			/* Pointer to the procedures prototype    */
    __dhp psl;			/* Procedures static environment          */
    char namekind;
  }
__simpleprocnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __pty pp;			/* Pointer to the procedures prototype    */
    __dhp psl;			/* Procedures static environment          */
    char namekind;
    char conv;
  }
__aritprocnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __pty pp;			/* Pointer to the procedures prototype    */
    __dhp psl;			/* Procedures static environment          */
    char namekind;
    char conv;
    __pty q;
  }
__refprocnamepar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __dhp ob;
    int swv;
    char namekind;
  }
__labelnamepar;

/* Label var and standard transmitted parameter or 
 * a name, var or standard transmitted switch parameter */

typedef struct			/* No thunk for switch parameter by name */
  {
    __progadr adr;
    __dhp ob;
  }
__labelswitchpar;

typedef struct
  {
    __dhp sl;
    __progadr adr;
    __arrp ap;
    char namekind;
  }
__arraynamepar;

typedef struct
  {
    char *fil;
    long line,
      fromline;
  }
__map;

#ifdef INCLUDE_SIMFILE_H
#include "simfile.h"
#else
typedef void __bs1FILE;
typedef void __bs2FILE;
typedef void __bs4FILE;
typedef void __bs5FILE;
typedef void __bs6FILE;
typedef void __bs7FILE;
typedef void __bs8FILE;
typedef void __bs9FILE;
typedef void __bs10FILE;
#endif

/******************************************************************************
                                                          External variables */

/* Inline error messages */
extern char *__errnone,
 *__errlim,
 *__errarr,
 *__errswitch,
 *__errqual,
 *__errvirt,
 *__errbound;

/* Program counter */
extern __progadr __goto,
  __return;
extern long __vreturn;
extern void __main_modul ();

/* switch */
extern int __swv;

/* Local-block, parameter-block, and static environm,ent to rcp(p)() */
extern __dhp __lb,
  __pb;
extern __dhp __sl;		/* Can't be a parameter since GBC must update 
				 * it */

/* Garbage collections statistics */
extern int __gc;
extern double __gbctime;

/* Stack or return value for evaluation of expressions */
						/* 900526 (1)   */
extern __valuetype __v[],
  __ev;
extern __dhp __r[],
  __er;
extern __txt __t[],
  __et,__t1,__t2;

extern long __as;		/* Antall p} h.h.v ref og text stakken */

/* Pointer to current array-object */
extern __arrp __ap;

/* (indeks - lower bound) for the actual dimensions to an array object */
extern int __h[];

extern char *__progname;

extern __pty __pp;		/* Prototyp pointer */
extern __dhp __bp;		/* In test */
extern __refvarpar *__vrp;	/* In test for var par */
extern __aritvarpar *__vvp;	/* Convertion between real and integer */
extern __refnamepar *__nrp;	/* In test for name par */
extern __aritnamepar *__nvp;	/* Convertion between real and integer for
				 * name */
extern long __rputlen;
extern char __conv;
extern char __currentlowten,
  __currentdecimalmark;

extern __notext __tk0;		/* tk0 == notext */
extern char __ctab[3][4];

extern long __poolsize,
  __maxsize;
extern char __dynsize,
  __chpoolsize;

extern char *__ctext;

extern char *__nullstr;

extern char *__progname;
extern long __argc;
extern long __argv;
long __rargc (void);
long __rargv (void);

extern __map *__curent_map;

extern long __cline;

extern long __lines_per_page;

extern __dhp __sto,
  __max,
  __min,
  __fri;
extern __dh __niu,
  __nil;

extern __dhp __p;

/******************************************************************************
            External declaration of run-time routines                        */

/* RTCK.C */
__dhp __rca (__arrp a);
extern __dhp __ralloc ();
char __rin (__dhp bpx, __pty p);
char __rrin (__pty q, __pty p);
char __ris (__dhp bpx, __pty p);
char __rgeta (__arraynamepar *p, long as, int ret, void (*mret) ());
char __rgetcbv (__charboolnamepar *p, long as, int ret, void (*mret) ());
char __rgetrv (__refnamepar *p, long as, int ret, void (*mret) ());
char __rgettv (__textnamepar *p, long as, int ret, void (*mret));
char __rgetproc (__procname *p, long as, int ret, void (*mret) ());
char __rgetlab (__labelnamepar *p, long as, int ret, void (*mret) ());
char __rgetsa (__simplenamepar *p, long as, int ret, void (*mret) ());
void __rreturn (long vret, int ret, void (*mret) ());
void __rundump (__txtvp t, int ret, void (*mret) ());
void __rdump (__txtvp t, int ret, void (*mret) ());
void __rcall (__dhp ob, int ret, void (*mret) ());
void __rresume (__dhp ob, int ret, void (*mret) ());
void __rexchange (__dhp sh, __dhp ob, int ret, void (*mret) ());
char __rgetav (char ftype, __aritnamepar *p, long as, int ret, void (*mret) ());

void __rcp (__pty ppx, long as);
void __rterror (__txtvp t);
void __renddecl (int plev);
void __rep (void);
void __rdetach (__dhp ob, int ret, void (*mret) ());
void __rrs (void);
void __rcpb (int ret, void (*mret) ());
void __rss (long as);
void __rcprb (__pty ppx);
void __reth (void);
void __rgbc (void);
void __do_for_each_pointer (__dhp p, void (*doit) (), void (*doit_notest) ());
void __rgoto (__dhp ob);
void __rsystemerror (char *s);
void __rendclass (int plev);
void __rinitier (__dhp *fra, __dhp *til);
void __rinner (int plev);
void __rwarning (char *s);
int __roa (__dhp x);
__dhp __ralloc (long size);


void __rccb (int ret, void (*mret) ());
void __rerror (char *s);
void __rct (long as);
void __rslutt (void);
void __rstart (int argc, char *argv[]);
void __rb (__pty ppx);
void __rtrace (void);
void __repp (void);

/* RTBASICIO.C */
__dhp __rsysin (void);
__dhp __rsysout (void);
__dhp __rsyserr (void);

/* ENVIRONMENT.C */

void __init_SIMENVIR (void);
void __rprintfilline (void);
void __rhisto (__arrp A, __arrp B, double c, double d);
/*
RETSIGTYPE __rsys_trap (void);
RETSIGTYPE __rillegal_trap (void);
RETSIGTYPE __rseg_trap (void);
RETSIGTYPE __rfloat_trap (void);
*/

/* Basic operations */
double __rabsr (double r);
long __rabsi (long i);
long __rsigni (long i);
long __rsigndx (long i, double j);
long __rsignr (double e);
long __rsigndi (long i, long j);
long __rsigndr (double e, double f);
long __rmod (long i, long j);
long __rrem (long i, long j);
long __rentier (double r);
long __rintrea (double r);
double __raddepsilon (double r);
double __rsubepsilon (double r);
long __rlowerbound (__arrp a, long i);
long __rupperbound (__arrp a, long i);
long __ridiv0 (long i);
double __rrdiv0(double i);

/* Power functions */
double __rpow (double x, double r);
double __rpowri (double r, long i);
extern double __rpow ();

/* Text utilities */
char __rchar (long i);
char __risochar (long i);
long __rrank (char c);
long __risorank (char c);
char __rdigit (char c);
char __rletter (char c);
char __rlowten (char c);
char __rdecimalmark (char c);
char __rcompstr (char *s1, char *s2, int n);

/* Mathematical functions */
double __rsqrt (double r);
double __rsin (double r);
double __rcos (double r);
double __rtan (double r);
double __rarcsin (double r);
double __rarccos (double r);
double __rarctan (double r);
double __rarctan2 (double r, double s);
double __rsinh (double r);
double __rcosh (double r);
double __rtanh (double r);
double __rln (double r);
double __rlog10 (double r);
double __rexp (double r);
double __rcotan (double r);

/* Extremum functions */
void __rmint (__txtvp t1, __txtvp t2);
char __rminc (char c1, char c2);
long __rmini (long i1, long i2);
double __rminr (double r1, double r2);
void __rmaxt (__txtvp t1, __txtvp t2);
char __rmaxc (char c1, char c2);
long __rmaxi (long i1, long i2);
double __rmaxr (double r1, double r2);

/* Environmental enquiries */
__txtvp __rsimulaid (long as);

/* Random drawing */
char __rdraw (double a, long *U);
long __rrandint (long a, long b, long *U);
double __runiform (double a, double b, long *U);
double __rnormal (double a, double b, long *U);
double __rnegexp (double a, long *U);
long __rpoisson (double a, long *U);
double __rerlang (double a, double b, long *U);
long __rdiscrete (__arrp A, long *U);
double __rlinear (__arrp A, __arrp B, long *U);
long __rhistd (__arrp A, long *U);

/* Calendar and timing utilities */
__txtvp __rdatetime (long as);
double __rcputime (void);
double __rclocktime (void);

/* RTTEXT.C */
char __rtconstant (__txtvp t);
long __rtstart (__txtvp t);
long __rtlength (__txtvp t);
char __rtmore (__txtvp t);
char __rtgetchar (__txtvp t);
double __rtgetreal (__txtvp t);
long __rtgetfrac (__txtvp t);
long __rtgetint (__txtvp t);
long __rtpos (__txtvp t);
__txtvp __rtmain (__txtvp t);
__txtvp __rtputchar (__txtvp t, char c);
__txtvp rtputint (__txtvp t, long i);
__txtvp __rtputfix (__txtvp t, double r, long n);
__txtvp __rtputreal (__txtvp t, double r, long n);
__txtvp __rtputfrac (__txtvp t, long i, long n);
__txtvp __rtsetpos (__txtvp t, long i);
__txtvp __rtsub (__txtvp t, long i, long n);
__txtvp __rtstrip (__txtvp t);
__txtvp __rcopy (long as, __txtvp t);
__txtvp __rblanks (long as, long n);
__txtvp __rconc (long as, __txtvp t1x, __txtvp t2x);
extern __txtvp __rtextvalassign ();
__txtvp __rtextassign (__txtvp t1x, __txtvp t2x);
char __reqrtext (__txtvp t1x, __txtvp t2x);
extern char __reqrtext ();
char __reqtext (__txtvp t1x, __txtvp t2x);
char __rlttext (__txtvp t1x, __txtvp t2x);
char __rletext (__txtvp t1x, __txtvp t2x);
__txtvp __rupcase (__txtvp t);
__txtvp __rlowcase (__txtvp t);
__txtvp __rtputint (__txtvp t, long ii);
void __rleftshift (__txtvp t, long j);

/* FILESYSTEM.C */

extern long __rfsize ();

/* Class file */
__txtvp __rfilename (long as, __bs1FILE *p);
char __risopen (__bs1FILE *p);
char __rsetaccess (__bs1FILE *p, __txtvp t);

/* File class imagefile */
void __rfield (__bs2FILE *p, long w);
long __rpos (__bs2FILE *p);
char __rmore (__bs2FILE *p);
long __rlength (__bs2FILE *p);
__dhp __rsetpos (__bs2FILE *p, long i);

/* Imagefile class infile */
char __riendfile (__bs4FILE *p);
char __riopen (__bs1FILE *p, __txtvp t);
char __riclose (__bs1FILE *p);
char __riinrecord (__bs2FILE *p);
char __riinchar (__bs2FILE *p);
char __rilastitem (__bs2FILE *p);
long __riinint (__bs2FILE *p);
double __riinreal (__bs2FILE *p);
long __riinfrac (__bs2FILE *p);
__txtvp __riintext (long as, __bs2FILE *p, long w);
__dhp __riinimage (__bs2FILE *p);

/* Imagefile class outfile */
char __roopen (__bs1FILE *p, __txtvp t);
char __roclose (__bs1FILE *p);
__dhp __rooutimage (__bs2FILE *p)		/* Skriver ikke ut etterfolgende blanke */;
__dhp __rooutrecord (__bs2FILE *p);
__dhp __robreakoutimage (__bs2FILE *p);
__dhp __rooutchar (__bs2FILE *p, char c);
extern __dhp __roouttext ();
__dhp __rooutint (__bs2FILE *p, long i, long w);
__dhp __rooutfix (__bs2FILE *p, double r, long i, long  w);
__dhp __rooutreal (__bs2FILE *p, double r, long i, long w);
__dhp __rooutfrac (__bs2FILE *p, long i, long n, long  w);

/* Outfile class printfile */
long __rpline (__bs6FILE *p);
long __rppage (__bs6FILE *p);
char __rpopen (__bs6FILE *p, __txtvp t);
char __rpclose (__bs6FILE *p);
long __rplinesperpage (__bs6FILE *p, long n);
__dhp __rpspacing (__bs6FILE *p, long n);
__dhp __rpeject (__bs6FILE *p, long n);
__dhp __rpoutimage (__bs6FILE *p);
__dhp __rpoutrecord (__bs6FILE *p);

/* Imagefile class directfile */
char __rdlocked (__bs5FILE *p);
long __rdlocation (__bs5FILE *p);
char __rdendfile (__bs5FILE *p);
long __rdlock (__bs5FILE *p, double t, long i, long j);
char __rdcheckpoint (__bs5FILE *p);
char __rdunlock (__bs5FILE *p);
char __rdopen (__bs5FILE *p, __txtvp t);
char __rdclose (__bs5FILE *p);
long __rdlastloc (__bs5FILE *p);
long __rdmaxloc (__bs5FILE *p);
char __rddeleteimage (__bs5FILE *p);
__dhp __rdlocate (__bs5FILE *p, long i);
__dhp __rdoutimage (__bs5FILE *p);
__dhp __rdinimage (__bs5FILE *p);

/* File class bytefile */

long __rbytesize (__bs7FILE *p);

/* Bytefile class inbytefile */
__txtvp __ribintext (__bs8FILE *p, __txtvp t);
long __ribinbyte (__bs8FILE *p);
char __ribendfile (__bs8FILE *p);
char __ribopen (__bs8FILE *p);
char __ribclose (__bs8FILE *p);

/* Bytefile class outbytefile */
char __robopen (__bs9FILE *p);
char __robclose (__bs9FILE *p);
__dhp __roboutbyte (__bs9FILE *p, long x);
__dhp __robouttext (__bs9FILE *p, __txtvp t);

/* Bytefile class directbytefile */
__txtvp __rdbintext (__bs10FILE *p, __txtvp t);
long __rdblocation (__bs10FILE *p);
long __rdbmaxloc (__bs10FILE *p);
long __rdblock (__bs10FILE *p, double t, long i, long j);
char __rdbunlock (__bs10FILE *p);
char __rdbopen (__bs10FILE *p);
char __rdbclose (__bs10FILE *p);
long __rdblastloc (__bs10FILE *p);
__dhp __rdblocate (__bs10FILE *p, long i);
__dhp __rdboutbyte (__bs10FILE *p, long x);
__dhp __rdbouttext (__bs10FILE *p, __txtvp t);
long __rdbinbyte (__bs10FILE *p);
char __rdbendfile (__bs10FILE *p);

/* Environment to C */
char *__rcopytexttoc (__txtvp t);
char *__raddroffirstchar (__txtvp t);
char *__raddroffirstelem (__arrp p);
char **__rcopytextarrtoc (__arrp p, char byvalue);
char *__rcopyarrtoc (__arrp p);

char *xmalloc (unsigned int size);
