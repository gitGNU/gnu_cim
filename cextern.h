/* $Id: cextern.h,v 1.10 1994/12/24 21:03:55 cim Exp $ */

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

#include <stdio.h>
#include "cconst.h"

extern FILE *CKOD;
extern FILE *HKOD;
extern FILE *MELL;
extern FILE *PROG;
extern FILE *positionarch ();

#ifdef TARGETGEN
#ifdef DEBUG
#define CKOD (fprintf(CKOD,"\n/*%s,%d*/\n",__FILE__,__LINE__),CKOD)
#endif
#endif

extern body ();
extern char *extract_filename ();
extern char *extract_timestamp ();
extern char *extkodnavn;
extern char *gettext ();
extern char *gettimestamp ();
extern char *hkodnavn;
extern char *htimestamp;
extern char *link_moduler;
extern char *prefquantident;
extern char *simlib[MAXANTSIMLIB];
extern char *tag ();
extern char *texttoken ();
extern char categ;
extern char directive_timestamp[TIMESTAMPLENGTH + 1];
extern char dangerproc ();
extern char decldel[];
extern char external;
extern char kind;
extern char nameasvar;
extern char separat_comp;
extern char simset;
extern char simulation;
extern char subclass ();
extern char subordinate ();
extern char timestamp[TIMESTAMPLENGTH + 1];
extern char type;
extern char yaccerror;
extern unsigned char yytext[YYTEXTLENGTH];
extern d1error ();
extern d2error ();
extern dump ();
extern dumpexp ();
extern endblock ();
extern gencall ();
extern genvalue ();
extern inblock ();
extern initdekl ();
extern inittext ();
extern lerror ();
extern int localused;
extern int anterror;
extern int cexpr;
extern int sjerror;
extern int antsimlib;
extern int arrdim;
extern int cblno;
extern int cblev;
extern int classniv;
extern int cur;
extern int lextoken;
extern int maxblno;
extern int maxantcase;
extern int newlabel ();
extern int newllabel ();
extern int option_expressions;
extern int option_atr;
extern int option_input;
extern int option_msymbols;
extern int option_lex;
extern int option_gen_test;
extern int option_gen_trace;
extern int option_nowarning;
extern int option_line;
extern int option_verbose;
extern int option_write_tokens;
extern int option_reuse_timestamp;
extern int sstrlen ();
extern int stack;
extern int stakk[MAXLABELSTAKK];
extern int yyleng;
extern long antnewline;
extern long radix ();
extern long salloc ();
extern long yylineno;
extern moreparam ();
extern newblck ();
extern newdekl ();
extern nullblock ();
extern nulldekl ();
extern outblock ();
extern reginner ();
extern reinit ();
extern serror ();
extern settarraydim ();
extern short prelev[];
extern sjekker ();
extern struct BLOCK *sblock;
extern struct BLOCK *cblock;
extern struct BLOCK *display[];
extern struct BLOCK *seenthrough;
extern struct BLOCK *firstclass ();
extern struct DECL *clastdecl;
extern struct DECL *classtext;
extern struct DECL *commonprefiks;
extern struct DECL *commonqual ();
extern struct DECL *cprevdecl;
extern struct DECL *finddecl ();
extern struct DECL *findglobal ();
extern struct DECL *findlocal ();
extern struct DECL *firstparam ();
extern struct DECL *nextparam ();
extern struct DECL *regthis ();
extern struct DECL *sistearray;
extern struct EXP expbuffer[MAXEXPBUFFER];
extern struct EXP owerflowbuffer;
extern struct EXP *copytree ();
extern struct namebuff *systag ();
extern structure ();
extern yerror ();
extern yylex ();
extern yyparse ();
