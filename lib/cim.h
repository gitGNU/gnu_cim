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
extern long __rargc ();
extern long __rargv ();

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
            External declaration of run-time routines that do not return int */

/* RTCK.C */
extern __dhp __rca ();
extern __dhp __ralloc ();
extern char __rin ();
extern char __rrin ();
extern char __ris ();
extern char __rgetav ();
extern char __rgetcbv ();
extern char __rgetrv ();
extern char __rgettv ();
extern char __rgetproc ();
extern char __rgetlab ();
extern char __rgeta ();
extern char __rgetsa ();

/* RTBASICIO.C */
extern __dhp __rsysin ();
extern __dhp __rsysout ();
extern __dhp __rsyserr ();

/* ENVIRONMENT.C */

/* Basic operations */
extern double __rabsr ();
extern long __rabsi ();
extern long __rsigndi ();
extern long __rsigndr ();
extern long __rmod ();
extern long __rrem ();
extern long __rentier ();
extern long __rintrea ();
extern double __raddepsilon ();
extern double __rsubepsilon ();
extern long __rlowerbound ();
extern long __rupperbound ();

/* Power functions */
extern long __rpowii ();
extern double __rpowri ();
extern double __rpow ();

/* Text utilities */
extern char __rchar ();
extern char __risochar ();
extern long __rrank ();
extern long __risorank ();
extern char __rdigit ();
extern char __rletter ();
extern char __rlowten ();
extern char __rdecimalmark ();
extern __txtvp __rupcase ();
extern __txtvp __rlowcase ();

/* Mathematical functions */
extern double __rsqrt ();
extern double __rsin ();
extern double __rcos ();
extern double __rtan ();
extern double __rarcsin ();
extern double __rarccos ();
extern double __rarctan ();
extern double __rarctan2 ();
extern double __rsinh ();
extern double __rcosh ();
extern double __rtanh ();
extern double __rln ();
extern double __rlog10 ();
extern double __rexp ();
extern double __rcotan ();

/* Extremum functions */
extern void __rmint ();
extern char __rminc ();
extern long __rmini ();
extern double __rminr ();
extern void __rmaxt ();
extern char __rmaxc ();
extern long __rmaxi ();
extern double __rmaxr ();

/* Environmental enquiries */
extern __txtvp __rsimulaid();

/* Random drawing */
extern char __rdraw ();
extern long __rrandint ();
extern double __runiform ();
extern double __rnormal ();
extern double __rnegexp ();
extern long __rpoisson ();
extern double __rerlang ();
extern long __rdiscrete ();
extern double __rlinear ();
extern long __rhistd ();

/* Calendar and timing utilities */
extern __txtvp __rdatetime ();
extern double __rcputime ();
extern double __rclocktime ();

/* RTTEXT.C */
extern char __rtconstant ();
extern long __rtstart ();
extern long __rtlength ();
extern char __rtmore ();
extern char __rtgetchar ();
extern double __rtgetreal ();
extern long __rtgetfrac ();
extern long __rtgetint ();
extern long __rtpos ();
extern __txtvp __rtmain ();
extern __txtvp __rtputchar ();
extern __txtvp __rtputint ();
extern __txtvp __rtputfix ();
extern __txtvp __rtputreal ();
extern __txtvp __rtputfrac ();
extern __txtvp __rtsetpos ();
extern __txtvp __rtsub ();
extern __txtvp __rtstrip ();
extern __txtvp __rcopy ();
extern __txtvp __rblanks ();
extern __txtvp __rconc ();
extern __txtvp __rtextvalassign ();
extern __txtvp __rtextassign ();
extern char __reqrtext ();
extern char __reqrtext ();
extern char __reqtext ();
extern char __rlttext ();
extern char __rletext ();

/* FILESYSTEM.C */

extern long __rfsize ();

/* Class file */
extern __txtvp __rfilename ();
extern char __risopen ();
extern char __rsetaccess ();

/* File class imagefile */
extern long __rpos ();
extern char __rmore ();
extern long __rlength ();
extern __dhp __rsetpos ();

/* Imagefile class infile */
extern char __riendfile ();
extern char __riopen ();
extern char __riclose ();
extern char __riinrecord ();
extern char __riinchar ();
extern char __rilastitem ();
extern long __riinint ();
extern double __riinreal ();
extern long __riinfrac ();
extern __txtvp __riintext ();
extern __dhp __riinimage ();

/* Imagefile class outfile */
extern char __roopen ();
extern char __roclose ();
extern __dhp __rooutimage ();
extern __dhp __rooutrecord ();
extern __dhp __robreakoutimage ();
extern __dhp __rooutchar ();
extern __dhp __roouttext ();
extern __dhp __rooutint ();
extern __dhp __rooutfix ();
extern __dhp __rooutreal ();
extern __dhp __rooutfrac ();

/* Outfile class printfile */
extern long __rpline ();
extern long __rppage ();
extern char __rpopen ();
extern char __rpclose ();
extern long __rplinesperpage ();
extern __dhp __rpspacing ();
extern __dhp __rpeject ();
extern __dhp __rpoutimage ();
extern __dhp __rpoutrecord ();

/* Imagefile class directfile */
extern long __rdlocation ();
extern char __rdendfile ();
extern char __rdlocked ();
extern char __rdcheckpoint ();
extern char __rdunlock ();
extern long __rdlock ();
extern char __rdopen ();
extern char __rdclose ();
extern long __rdlastloc ();
extern long __rdmaxloc ();
extern char __rddeleteimage ();
extern __dhp __rdlocate ();
extern __dhp __rdoutimage ();
extern __dhp __rdinimage ();

/* File class bytefile */

extern long __rbytesize ();

/* Bytefile class inbytefile */
extern __txtvp __ribintext ();
extern long __ribinbyte ();
extern char __ribendfile ();
extern char __ribopen ();
extern char __ribclose ();

/* Bytefile class outbytefile */
extern char __robopen ();
extern char __robclose ();
extern __dhp __roboutbyte ();
extern __dhp __robouttext ();

/* Bytefile class directbytefile */
extern __txtvp __rdbintext ();
extern long __rdblocation ();
extern long __rdbmaxloc ();
extern char __rdblocked ();
extern long __rdblock ();
extern char __rdbunlock ();
extern char __rdbopen ();
extern char __rdbclose ();
extern long __rdblastloc ();
extern __dhp __rdblocate ();
extern __dhp __rdboutbyte ();
extern __dhp __rdbouttext ();

/* Environment to C */
extern char *__rcopytexttoc ();
extern char *__raddroffirstchar ();
extern char *__raddroffirstelem ();
extern char **__rcopytextarrtoc ();
extern char *__rcopyarrtoc ();

extern char *xmalloc();
