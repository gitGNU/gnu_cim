/* $Id: csjekker.c,v 1.16 1997/01/08 09:49:20 cim Exp $ */

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

/* Deklarasjonsstrukturen er lest inn i pass 1.
 * Sjekker naa semantikken for hver setning.           
 * Gj|r kall paa prosedyrer i deklarasjonslageret      
 * for aa sjekke uttrykk.                              
 * Gj|r kall paa procedyrer i gen.c for aa faa generet 
 * kode for de ulike construksjonene. */

/* Constanter til bruk */
#include <math.h>
#include "cmell.h"
#include "cgen.h"

int lest;

/* Kontakt med deklarasjonslageret */

struct DECL *absfunction;
struct DECL *absfunctionr;
struct DECL *absfunctioni;
struct DECL *minfunction;
struct DECL *minfunctiont;
struct DECL *minfunctionc;
struct DECL *minfunctionr;
struct DECL *minfunctioni;
struct DECL *maxfunction;
struct DECL *maxfunctiont;
struct DECL *maxfunctionc;
struct DECL *maxfunctionr;
struct DECL *maxfunctioni;
struct DECL *signfunction;
struct DECL *signfunctionr;
struct DECL *signfunctioni;
struct DECL *sourcelinefunction;

struct DECL *varargsint;
struct DECL *varargsreal;
struct DECL *varargschar;
struct DECL *varargstext;
struct DECL *varargsintvar;
struct DECL *varargsrealvar;
struct DECL *varargscharvar;
struct DECL *varargstextvalue;
struct DECL *varargs;

extern struct DECL *commonprefiks;

#define new(X) (struct X *)salloc(sizeof(struct X))


/* Makroer for det som kommer inn til andre pass */

#define inmell(x) {x=getc(MELL);lest=1;}

#if SIZEOF_LONG == 8
#define inmellival(x) {x=getc(MELL);x|=((long)getc(MELL))<<8;\
x|=((long)getc(MELL))<<16;x|=((long)getc(MELL))<<24;\
x|=((long)getc(MELL))<<32;x|=((long)getc(MELL))<<40;\
x|=((long)getc(MELL))<<48;x|=((long)getc(MELL))<<56;}
#else
#define inmellival(x) {x=getc(MELL);x|=((long)getc(MELL))<<8;\
x|=((long)getc(MELL))<<16;x|=((long)getc(MELL))<<24;}
#endif

#define inmellrval(x) {inmellival(x.i1);inmellival(x.i2);}
#define inmellid(x) inmellival(x)
#define inmelltval(x) {long x2;inmellival(x2);x.txt=(char *)x2;x.id=NOTEXT;}
/* Kontakt med feilsystemet */

/* Globale deklarasjoner */
int token;
char *ident;
long expline;

struct EXP *expdisplay[MAXEXPDISPLAY];
struct EXP expbuffer[MAXEXPBUFFER];
struct EXP owerflowbuffer;
int cexpr,
  cdisp;

#if 0
struct EXP *classlist,
 *cb				/* Liste av klassene i en prefikskjede,med 
				 * med laveste prefiksnivå først. Brukes
				 * for å få riktig rekkefølge p}
				 * parameterene ved generering av et nytt
				 * klasse objekt. */
#endif
char newarg = 0;		/* Hvis newarg så skal nextclassparam kalles
				 * istedet for nextparam */

int sjerror;
#define cleanbuffer cexpr=cdisp=sjerror=0
#undef pop
#undef push
#undef look

#define pop expdisplay[cdisp--]
#define push expdisplay[++cdisp]
#define look expdisplay[cdisp]

char not_reached;
char computeconst ();
char setdanger_const ();

/******************************************************************************
                                                                COPYTREE     */

struct EXP *
copytree (re)
     struct EXP *re;
{
  struct EXP *rex;
  rex = newexp;
  if (re->left != NULL)
    {
      rex->left = copytree (re->left);
      rex->left->up = rex;
    }
  else
    rex->left = NULL;
  if (re->right != NULL)
    {
      rex->right = copytree (re->right);
      rex->right->up = rex;
    }
  else
    rex->right = NULL;
  rex->token = re->token;
  rex->value = re->value;
  rex->type = re->type;
  rex->danger = re->danger;
  rex->up = re->up;
  rex->rd = re->rd;
  rex->qual = re->qual;
  rex->seenthrough = re->seenthrough;
  return (rex);
}

/******************************************************************************
                                                                TEXTTOKEN    */

#ifdef DEBUG
/* TEXTTOKEN BRUKES TIL TESTUTSKRIFT */
char *
texttoken (m)
     int m;
{
  switch (m)
    {
    case MERROR:
      return ("MERROR");
    case MNEWLINE:
      return ("MNEWLINE");
    case MSTOP:
      return ("MSTOP");
    case MBLOCK:
      return ("MBLOCK");
    case MENDBLOCK:
      return ("MENDBLOCK");
    case MPRBLOCK:
      return ("MPRBLOCK");
    case MENDPRBLOCK:
      return ("MENDPRBLOCK");
    case MPROCEDURE:
      return ("MPROCEDURE");
    case MENDPROCEDURE:
      return ("MENDPROCEDURE");
    case MCLASS:
      return ("MCLASS");
    case MENDCLASS:
      return ("MENDCLASS");
    case MINSPECT:
      return ("MINSPECT");
    case MENDINSPECT:
      return ("MENDINSPECT");
    case MDO:
      return ("MDO");
    case MENDDO:
      return ("MENDDO");
    case MWHEN:
      return ("MWHEN");
    case MENDWHEN:
      return ("MENDWHEN");
    case MOTHERWISE:
      return ("MOTHERWISE");
    case MENDOTHERWISE:
      return ("MENDOTHERWISE");
    case MFOR:
      return ("MFOR");
    case MFORR:
      return ("MFORR");
    case MLISTSEP:
      return ("MLISTSEP");
    case MFORWHILE:
      return ("MFORWHILE");
    case MSTEP:
      return ("MSTEP");
    case MUNTIL:
      return ("MUNTIL");
    case MFORDO:
      return ("MFORDO");
    case MENDFOR:
      return ("MENDFOR");
    case MWHILE:
      return ("MWHILE");
    case MENDWHILE:
      return ("MENDWHILE");
    case MIF:
      return ("MIF");
    case MELSE:
      return ("MELSE");
    case MENDIF:
      return ("MENDIF");
    case MGOTO:
      return ("MGOTO");
    case MINNER:
      return ("MINNER");
    case MSWITCH:
      return ("MSWITCH");
    case MSWITCHSEP:
      return ("MSWITCHSEP");
    case MENDSWITCH:
      return ("MENDSWITCH");
    case MASSIGN:
      return ("MASSIGN");
    case MASSIGNR:
      return ("MASSIGNR");
    case MENDASSIGN:
      return ("MENDASSIGN");
    case MARRAY:
      return ("MARRAY");
    case MARRAYSEP:
      return ("MARRAYSEP");
    case MENDARRAY:
      return ("MENDARRAY");
    case MBOUNDPARSEP:
      return ("MBOUNDPARSEP");
    case MBOUNDSEP:
      return ("MBOUNDSEP");
    case MIFE:
      return ("MIFE");
    case MELSEE:
      return ("MELSEE");
    case MORELSE:
      return ("MORELSE");
    case MANDTHEN:
      return ("MANDTHEN");
    case MEQV:
      return ("MEQV");
    case MIMP:
      return ("MIMP");
    case MOR:
      return ("MOR");
    case MAND:
      return ("MAND");
    case MNOT:
      return ("MNOT");
    case MEQ:
      return ("MEQ");
    case MNE:
      return ("MNE");
    case MLT:
      return ("MLT");
    case MLE:
      return ("MLE");
    case MGT:
      return ("MGT");
    case MGE:
      return ("MGE");
    case MNER:
      return ("MNER");
    case MEQR:
      return ("MEQR");
    case MIS:
      return ("MIS");
    case MINS:
      return ("MIN");
    case MUADD:
      return ("MUADD");
    case MUSUB:
      return ("MUSUB");
    case MADD:
      return ("MADD");
    case MSUB:
      return ("MSUB");
    case MMUL:
      return ("MMUL");
    case MDIV:
      return ("MDIV");
    case MINTDIV:
      return ("MINTDIV");
    case MPRIMARY:
      return ("MPRIMARY");
    case MNOOP:
      return ("MNOOP");
    case MTEXTKONST:
      return ("MTEXTKONST");
    case MCHARACTERKONST:
      return ("MCHARACTERKONST");
    case MREALKONST:
      return ("MREALKONST");
    case MINTEGERKONST:
      return ("MINTEGERKONST");
    case MBOOLEANKONST:
      return ("MBOOLEANKONST");
    case MNONE:
      return ("MNONE");
    case MIDENTIFIER:
      return ("MIDENTIFIER");
    case MTHIS:
      return ("MTHIS");
    case MDOT:
      return ("MDOT");
    case MNEWARG:
      return ("MNEWARG");
    case MQUA:
      return ("MQUA");
    case MARGUMENT:
      return ("MARGUMENT");
    case MARGUMENTSEP:
      return ("MARGUMENTSEP");
    case MENDSEP:
      return ("MENDSEP");
    case MPROCARG:
      return ("MPROCARG");
    case MARRAYARG:
      return ("MARRAYARG");
    case MREFASSIGNT:
      return ("MREFASSIGNT");
    case MEQT:
      return ("MEQT");
    case MNET:
      return ("MNET");
    case MLTT:
      return ("MLTT");
    case MLET:
      return ("MLET");
    case MGTT:
      return ("MGTT");
    case MGET:
      return ("MGET");
    case MNERT:
      return ("MNERT");
    case MEQRT:
      return ("MEQRT");
    case MUADDI:
      return ("MUADDI");
    case MUSUBI:
      return ("MUSUBI");
    case MADDI:
      return ("MADDI");
    case MSUBI:
      return ("MSUBI");
    case MMULI:
      return ("MMULI");
    case MPRIMARYII:
      return ("MPRIMARYII");
    case MPRIMARYRI:
      return ("MPRIMARYI");
    case MQUANOTNONE:
      return ("MQUANOTNONE");
    case MQUANONEAND:
      return ("MQUANONEAND");
    case MREAINT:
      return ("MREAINT");
    case MINTREA:
      return ("MINTREA");
    case MSTACK:
      return ("MSTACK");
    case MARRAYADR:
      return ("MARRAYADR");
    case MDOTARRAYADR:
      return ("MDOTARRAYADR");
    case MVALASSIGNT:
      return ("MVALASSIGNT");
    case MLABEL:
      return ("MLABEL");
    case MENDLABEL:
      return ("MENDLABEL");
    case MCONC:
      return ("MCONC");
    case MCONST:
      return ("MCONST");
    case MDOTCONST:
      return ("MDOTKONST");
    default:
      return ("M_ukjent");
    }
}

/******************************************************************************
                                                                DUMPEXPREK   */

/* Testutskrift av et uttrykstree */

dumpexprek (re)
     struct EXP *re;
{
  (void) printf ("--EXPR: %d\ttoken: %s\t", re, texttoken (re->token));
  if (re->left != NULL)
    printf ("left: %d ", re->left);
  if (re->right != NULL)
    printf ("right: %d ", re->right);
  (void) printf ("type: %c\n", re->type);
  switch (re->token)
    {
    case MIDENTIFIER:
    case MQUA:
    case MQUANONEAND:
    case MQUANOTNONE:
    case MIS:
    case MINS:
    case MNEWARG:
    case MPROCARG:
    case MARRAYARG:
    case MARGUMENT:
      (void) printf ("ident: %s ", re->value.ident);
      if (re->rd != NULL)
	{
	  (void) printf ("-------------> DEKLARASJON:%s\n", re->rd->ident);
	  if (re->rd->descr != NULL)
	    {
	      if (re->rd->kind == KPROC && re->rd->descr->hiprot != NULL)
		{
		  (void) printf ("-------------> KodeKlasse:%d  rtnavn : %s \n",
			       ((exinfop) re->rd->descr->hiprot)->codeclass,
				 ((exinfop) re->rd->descr->hiprot)->rtname);
		}
	    }
	}
      if (re->qual != NULL)
	(void) printf ("-------------> QUAL:%s\n", re->qual->ident);
      break;
    default:
      if (re->value.ival)
	printf ("value: %d/%g ", (int) re->value.ival, re->value.ival);
      if (re->qual != NULL)
	(void) printf ("\n-------------> QUAL:%s\n", re->qual->ident);
    }

  (void) printf ("\n");
  if (re->left != NULL)
    dumpexprek (re->left);
  if (re->right != NULL)
    dumpexprek (re->right);
}

/******************************************************************************
                                                                   DUMPEXP   */

/* Testutskrift av de uttrykk som ligger på stakken */

dumpexp ()
{
  int i;
  if (option_expressions)
    {
      for (i = 1; i <= cdisp; i++)
	{
	  (void) printf ("--EXPRESSION NUMMER: %d yylineno: %ld expline: %ld\n"
			 ,i, yylineno, expline);
	  dumpexprek (expdisplay[i]);
	}
      (void) fflush (stdout);
    }
}

#endif

/******************************************************************************
                                                                INITSJEKKER  */

initsjekker ()
{
  yylineno = 1;
  absfunction = findglobal (tag ("ABS", 3), TRUE);
  minfunction = findglobal (tag ("MIN", 3), TRUE);
  maxfunction = findglobal (tag ("MAX", 3), TRUE);
  absfunctionr = findglobal (tag ("ABS*R", 5), TRUE);
  absfunctioni = findglobal (tag ("ABS*I", 5), TRUE);
  minfunctiont = findglobal (tag ("MIN*T", 5), TRUE);
  minfunctionc = findglobal (tag ("MIN*C", 5), TRUE);
  minfunctionr = findglobal (tag ("MIN*R", 5), TRUE);
  minfunctioni = findglobal (tag ("MIN*I", 5), TRUE);
  maxfunctiont = findglobal (tag ("MAX*T", 5), TRUE);
  maxfunctionc = findglobal (tag ("MAX*C", 5), TRUE);
  maxfunctionr = findglobal (tag ("MAX*R", 5), TRUE);
  maxfunctioni = findglobal (tag ("MAX*I", 5), TRUE);
  signfunction = findglobal (tag ("SIGN", 4), TRUE);
  signfunctionr = findglobal (tag ("SIGN*R", 6), TRUE);
  signfunctioni = findglobal (tag ("SIGN*I", 6), TRUE);
  sourcelinefunction = findglobal (tag ("SOURCELINE", 10), TRUE);

  varargsint = new (DECL);
  varargsint->encl = NULL;
  varargsint->ident = NULL;
  varargsint->type = TINTG;
  varargsint->kind = KSIMPLE;
  varargsint->categ = CDEFLT;
  varargsint->next = NULL;

  varargsreal = new (DECL);
  varargsreal->encl = NULL;
  varargsreal->ident = NULL;
  varargsreal->type = TREAL;
  varargsreal->kind = KSIMPLE;
  varargsreal->categ = CDEFLT;
  varargsreal->next = NULL;

  varargschar = new (DECL);
  varargschar->encl = NULL;
  varargschar->ident = NULL;
  varargschar->type = TCHAR;
  varargschar->kind = KSIMPLE;
  varargschar->categ = CDEFLT;
  varargschar->next = NULL;

  varargstext = new (DECL);
  varargstext->encl = NULL;
  varargstext->ident = NULL;
  varargstext->type = TTEXT;
  varargstext->kind = KSIMPLE;
  varargstext->categ = CDEFLT;
  varargstext->next = NULL;

  varargsintvar = new (DECL);
  varargsintvar->encl = NULL;
  varargsintvar->ident = NULL;
  varargsintvar->type = TINTG;
  varargsintvar->kind = KSIMPLE;
  varargsintvar->categ = CVAR;
  varargsintvar->next = NULL;

  varargsrealvar = new (DECL);
  varargsrealvar->encl = NULL;
  varargsrealvar->ident = NULL;
  varargsrealvar->type = TREAL;
  varargsrealvar->kind = KSIMPLE;
  varargsrealvar->categ = CVAR;
  varargsrealvar->next = NULL;

  varargscharvar = new (DECL);
  varargscharvar->encl = NULL;
  varargscharvar->ident = NULL;
  varargscharvar->type = TCHAR;
  varargscharvar->kind = KSIMPLE;
  varargscharvar->categ = CVAR;
  varargscharvar->next = NULL;

  varargstextvalue = new (DECL);
  varargstextvalue->encl = NULL;
  varargstextvalue->ident = NULL;
  varargstextvalue->type = TTEXT;
  varargstextvalue->kind = KSIMPLE;
  varargstextvalue->categ = CVALUE;
  varargstextvalue->next = NULL;

  varargs = new (DECL);
  varargs->encl = NULL;
  varargs->ident = NULL;
  varargs->type = TNOTY;
  varargs->kind = KSIMPLE;
  varargs->categ = CDEFLT;
  varargs->next = NULL;
}

/******************************************************************************
                                                                      FLAG   */

flag ()
{
  char token;
  inmell (token);
  switch (token)
    {
    case MNONETEST:
      nonetest = OFF;
      break;
    case MNONETEST + 1:
      nonetest = ON;
      break;
    case MINDEXTEST:
      indextest = OFF;
      break;
    case MINDEXTEST + 1:
      indextest = ON;
      break;
    case MSTRIPSIDEEFFECTS:
      stripsideeffects = OFF;
      break;
    case MSTRIPSIDEEFFECTS + 1:
      stripsideeffects = ON;
      break;
    }
}

/******************************************************************************
                                                                 SJEKKER     */

/* Sjekker setningene flatt. Antar at syntaksen er riktig
 * Gjør kall på inblock og outblock for hver blokk */

sjekker ()
{
  struct DECL *rd;
  reinit ();
  initsjekker ();
  initgen ();

  inmell (token);
  while (1)
    {
      switch (token)
	{
	case MCONST:
	  beforegen (look);
	  {
	    int token = look->right->token;
	    if (token != MREALKONST & token != MTEXTKONST 
		& token != MCHARACTERKONST
		& token != MINTEGERKONST & token != MBOOLEANKONST
		& look->right->type != TERROR)
	      serror (6);
	  }
	  look->left->rd->value = look->right->value;
	  look->left->rd->categ = CCONST;
	  break;
	case MBLOCK:
	  if (anterror == 0)
	    skrivenddecl ();
	  inblock ();
	  gblock ();
	  break;
	case MENDBLOCK:
	  if (anterror == 0 && cblev != EXTERNALGLOBALBLEV)
	    gendblock ();
	  outblock ();
	  break;
	case MPRBLOCK:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (look->type != TERROR &&
	      (look->token != MARGUMENT || look->rd->kind != KCLASS))
	    serror (3);
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	    }
	  inblock ();
	  if (anterror == 0)
	    {
	      gprblock (look);
	    }
	  break;
	case MENDPRBLOCK:
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      gendprblock ();
	    }
	  outblock ();
	  break;
	case MPROCEDURE:
	  inblock ();
	  if (anterror == 0)
	    gprocedure ();
	  break;
	case MENDPROCEDURE:
	  if (anterror == 0)
	    gendprocedure ();
	  outblock ();
	  break;
	case MCLASS:
	  inblock ();
	  if (anterror == 0)
	    gclass ();
	  break;
	case MENDCLASS:
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      gendclass ();
	    }
	  outblock ();
	  break;
	case MINSPECT:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (look->type != TREF && look->type != TERROR)
	    serror (73, token);
	  if (anterror == 0)
	    skrivenddecl ();
	  inblock (look->qual);
	  if (anterror == 0)
	    ginspect (look);
	  break;
	case MENDINSPECT:
	  /* Hvis inspectsetningen hadde en otherwise gren s} vil ikke 
	   * utf|relsen komme hit.Dette blir da gjort n}r MENDOTHERWISE
	   * leses.Da leses ogs} MENDINSPECT inn */
	  if (anterror == 0)
	    gendinspect ();
#ifdef DEBUG
	  dumpexp ();
#endif
	  outblock ();
	  break;
	case MDO:
	  inblock (cblock->virt);
	  if (anterror == 0)
	    gdo ();
	  break;
	case MENDDO:
	  if (anterror == 0)
	    genddo ();
	  outblock ();
	  break;
	case MWHEN:
	  {
	    long identtemp;
	    inmellid (identtemp);
	    ident = (char *) identtemp;
	  }
	  /* cblock peker n} p} en inspect blokk.N}r findglobal n} skal lete 
	   * etter klasse identifikatoren som st}r etter when skal den      
	   * ikke ses gjennom inspect.Setter derfor parloc til inspect      
	   * blokken lik NULL (den f}r jo ny verdi av findglobal).Det er    
	   * virt pekeren som peker p} den inspiserte blokkens kvalifikasjon */

	  rd = findglobal (ident, FALSE);
	  if (rd->categ == CNEW)
	    {
	      rd->categ = CERROR;
	      serror (75, rd->ident);
	    }
	  else if (rd->kind != KCLASS)
	    {
	      if (rd->kind != KERROR)
		serror (84);
	    }
	  else
	    /* Sjekker om rd er samme klasse eller en subklasse til * klassen 
	     * som inspiseres,eller omvendt */
	  if (!subclass (rd, cblock->virt))
	    if (!subclass (cblock->virt, rd))
	      {
		serror (83, rd->ident);
		/* Trenger ikke } legge ut kode for denne WHEN  grenen */
	      }
	  inblock (rd);
	  if (anterror == 0)
	    gwhen (rd);
	  break;
	case MENDWHEN:
	  if (anterror == 0)
	    gendwhen ();
	  outblock ();
	  break;
	case MOTHERWISE:
	  /* cblock peker p} inspect blokken. Inne i otherwise grenen skal
	   * ikke inspect objektet inspiseres.Kaller derfor    
	   * outblock().Det m} da ikke gj|res ved MENDINSPECT. Se
	   * nedenfor (case mendotherwise:) */
	  if (anterror == 0)
	    gotherwise ();
	  outblock ();
	  break;
	case MENDOTHERWISE:
	  if (anterror == 0)
	    gendotherwise ();
	  /* Neste m-symbol er MENSINSPECT.Leser inn det slik at det ikke
	   * blir noen behandling for det symbolet Outblock() er
	   * allerede kalt ved MOTHERWISE */
	  inmell (token);
	  if (token != MENDINSPECT)
	    serror (81);
#ifdef DEBUG
	  dumpexp ();
#endif
	  break;
	case MFORDO:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (anterror == 0)
	    skrivenddecl ();
	  inblock ();
	  if (anterror == 0)
	    {
	      gfor (look);
	      gfordo ();
	    }
	  break;
	case MENDFOR:
	  if (anterror == 0)
	    gendfordo ();
	  outblock ();
	  break;
	case MWHILE:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (look->type != TBOOL && look->type != TERROR)
	    serror (77, token);
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      gwhile (look);
	    }
	  break;
	case MENDWHILE:
	  if (anterror == 0)
	    gendwhile ();
	  break;
	case MIF:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (look->type != TBOOL)
	    if (look->type != TERROR)
	      serror (77, token);
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      gif (look);
	    }
	  break;
	case MELSE:
	  if (anterror == 0)
	    gelse ();
	  break;
	case MENDIF:
	  if (anterror == 0)
	    gendif ();
	  break;
	case MGOTO:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (look->type != TLABEL && look->type != TERROR)
	    serror (108, token);
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      ggoto (look);
	    }
	  break;
	case MINNER:
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      ginner ();
	    }
	  break;
	case MENDSWITCH:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  genswitch (look);
	  break;
	case MENDASSIGN:
	  beforegen (look);
	  if (look->token != MPROCARG && look->token != MNEWARG &&
	      look->token != MASSIGN && look->token != MREFASSIGNT &&
	      look->token != MASSIGNR && look->token != MVALASSIGNT &&
	      (look->token != MDOT || look->right->token != MPROCARG) &&
	      look->type != TERROR)
	    serror (115);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (anterror == 0)
	    gen (look);
	  break;
	case MENDARRAY:
	  beforegen (look);
#ifdef DEBUG
	  dumpexp ();
#endif
	  if (anterror == 0)
	    genarray (look);
	  break;
	case MENDLABEL:
	  beforegen (look);
	  if (anterror == 0)
	    {
	      skrivenddecl ();
	      glabel (look);
	    }
	  break;
	case MSTOP:
	  if (separat_comp && (anterror == 0 || option_atr))
	    skrivut_external_spec ();
	  if (anterror == 0)
	    fingen ();
	  return;
	  break;
	case MERROR:
	  /* NOTTHING */
	  break;
	case MNEWLINE:
	  yylineno++;
	  break;
	case MFLAG:
	  flag ();
	  break;
	default:
	  lest = 0;
	  expression ();
	  if (lest == 0)
	    serror (71, token);
	  continue;
	}
      inmell (token);
    }
}

beforegen (re)
     struct EXP *re;
{
  sjekktype (re);
  (void) computeconst (re);
  (void) setdanger_const (re);
  if (not_reached == FALSE)
    genline ();
  not_reached = FALSE;
}


/******************************************************************************
                                                               EXPRESSION    */

/* Bygger opp et uttrykstree, Alle operatorene leses postfix */

expression ()
{
  struct EXP *re;

  expline = 0;
  cleanbuffer;
  while (TRUE)
    {
      switch (token)
	{
	case MNOT:
	case MUADD:
	case MUSUB:
	case MNOOP:
	case MQUA:
	  re = newexp;
	  re->right = NULL;
	  re->left = look;
	  re->left->up = re;
	  look = re;
	  /* Sjekke dobbel unær
	   * Eventuelt optimalisere bort noen noop */
	  break;
	case MTEXTKONST:
	case MCHARACTERKONST:
	case MREALKONST:
	case MINTEGERKONST:
	case MBOOLEANKONST:
	case MENDSEP:
	case MNONE:
	case MIDENTIFIER:
	case MTHIS:
	  re = newexp;
	  re->right = NULL;
	  re->left = NULL;
	  push = re;
	  break;
	case MNEWARG:
	case MARGUMENT:
	  re = newexp;
	  re->right = look;
	  re->left = NULL;
	  re->right->up = re;
	  look = re;
	  break;
	case MLABEL:
	  re = newexp;
	  re->right = re->left = NULL;
	  push = re;
	  break;
	case MNEWLINE:
	  expline++;
	  goto newtoken;
	  break;
	case MFLAG:
	  flag ();
	  break;
	default:
	  if (setntokens (token) || token == MERROR || token == MSTOP)
	    {
	      owerflowbuffer.left = NULL;
	      owerflowbuffer.right = NULL;
#ifdef DEBUG
	      if (token == MERROR)
		dumpexp ();
#endif
	      yylineno += expline;
	      re = look->up = newexp;
	      re->token = token;
	      re->value.rval = 0.0;
	      re->type = 0;
	      re->left = NULL;
	      re->right = NULL;
	      re->up = re;
	      re->rd = NULL;
	      re->qual = NULL;
	      re->seenthrough = NULL;
	      return;
	    }
	  /* VANELIG BEHANDLING */
	  re = newexp;
	  re->right = pop;
	  re->right->up = re;
	  re->left = look;
	  re->left->up = re;
	  look = re;
	}
      re->token = token;
      re->type = 0;
      re->value.rval = 0.0;
      if (expridtokens (token))
	inmellid (re->value.ival)
	  else
	if (token == MTEXTKONST)
	  inmelltval (re->value.tval)
	    else
	  if (token == MREALKONST)
	    inmellrval (re->value.rasiival)
	      else
	    if (exprvaltokens (token))
	      inmellival (re->value.ival);
      re->rd = NULL;
      re->qual = NULL;
      re->seenthrough = NULL;
    newtoken:
      inmell (token);
    }
}

/******************************************************************************
                                                                SAMETYPE     */

sametype (rel, rer)
     struct EXP **rel,
     **rer;
{
  if ((*rel)->type == TINTG && (*rer)->type == TREAL)
    konvtype (rel, TREAL);
  else if ((*rel)->type == TREAL && (*rer)->type == TINTG)
    konvtype (rer, TREAL);
}

/******************************************************************************
                                                                KONVTYPE     */

/*VARARGS2 */
konvtype (re, type, qual)
     struct EXP **re;
     char type;
     struct DECL *qual;
{
  struct EXP *rex;
  struct DECL *rd;
  if (((*re)->type == TINTG && type == TREAL)
      || ((*re)->type == TREAL && type == TINTG))
    {
      rex = newexp;
      rex->left = (*re);
      rex->right = NULL;
      rex->up = (*re)->up;
      rex->rd = NULL;
      rex->qual = NULL;
      rex->value.rval = 0.0;
      if (type == TREAL)
	rex->token = MREAINT;
      else
	rex->token = MINTREA;
      rex->type = type;
      *re = (*re)->up = rex;
    }
  else if ((*re)->type == TREF && type == TREF)
    {
      if ((*re)->qual == NULL) /* OK */ ;
      else if (qual == NULL)
	{
	  if (((*re)->up->left == NULL || (*re)->up->left->type != TERROR)
	  && ((*re)->up->right == NULL || (*re)->up->right->type != TERROR))
	    serror (85, (*re)->up->token);
	  (*re)->type = (*re)->up->type = TERROR;
	}
      else if ((rd = commonqual ((*re)->qual, qual)) == qual) /* OK */ ;
      else if (rd == (*re)->qual && (*re)->token != MNEWARG)
	{
	  rex = newexp;
	  rex->left = (*re);
	  rex->right = NULL;
	  rex->up = (*re)->up;
	  rex->rd = qual;
	  rex->value.ident = qual->ident;
	  rex->qual = qual;
	  rex->token = MQUANONEAND;
	  rex->type = type;
	  *re = (*re)->up = rex;
	}
      else
	{
	  if (((*re)->token == MNEWARG) ||
	      (((*re)->up->left == NULL || (*re)->up->left->type != TERROR)
	  && ((*re)->up->right == NULL || (*re)->up->right->type != TERROR)))
	    serror (85, (*re)->up->token);
	  (*re)->type = (*re)->up->type = TERROR;
	}
    }
}

/* Diverse makroer, slik at vi kan skrive noe mer kompakt kode */

#define UP re->up
#define UPUP re->up->up
#define LEFT re->left
#define RIGHT re->right
#define RIGHTLEFT re->right->left
#define RIGHTRIGHT re->right->right
#define RIGHTRIGHTLEFT re->right->right->left

#define UPTOKEN re->up->token
#define UPUPTOKEN re->up->up->token
#define LEFTTOKEN re->left->token
#define LEFTRIGHTTOKEN re->left->right->token
#define RIGHTTOKEN re->right->token
#define TOKEN re->token

#define UPVALUE re->up->value
#define LEFTVALUE re->left->value
#define LEFTRIGHTVALUE re->left->right->value
#define RIGHTVALUE re->right->value
#define VALUE re->value

#define UPTYPE re->up->type
#define LEFTTYPE re->left->type
#define RIGHTTYPE re->right->type
#define RIGHTLEFTTYPE re->right->left->type
#define RIGHTRIGHTLEFTTYPE re->right->right->left->type
#define TYPE re->type

#define UPRD re->up->rd
#define UPUPRD re->up->up->rd
#define LEFTRD re->left->rd
#define LEFTRIGHTRD re->left->right->rd
#define RIGHTRD re->right->rd
#define RD re->rd

#define UPQUAL re->up->qual
#define UPUPQUAL re->up->up->qual
#define LEFTQUAL re->left->qual
#define LEFTLEFTQUAL re->left->left->qual
#define LEFTRIGHTQUAL re->left->right->qual
#define RIGHTQUAL re->right->qual
#define QUAL re->qual

#define SEENTHROUGH re->seenthrough
#define LEFTSEENTHROUGH re->left->seenthrough
#define LEFTLEFTSEENTHROUGH re->left->left->seenthrough

#define ISLEFT re->up->left==re
#define UPISLEFT re->up->up->left==re->up
#define ISRIGHT re->up->right==re

#undef DANGER
#define DANGER re->danger
#define KONST re->konst

/******************************************************************************
                                                                     SSERROR */

#define SERROR(melding) sserror(melding,re)

sserror (melding, re)
     int melding;
     struct EXP *re;
{
  if (RD && RD->categ == CNEW)
    {
      serror (melding, RD->ident);
      RD->categ = CERROR;
    }
  else if (LEFT && LEFTRD && LEFTRD->categ == CNEW)
    {
      serror (melding, LEFTRD->ident);
      LEFTRD->categ = CERROR;
    }
  else if (RIGHT && RIGHTRD && RIGHTRD->categ == CNEW)
    {
      serror (melding, RIGHTRD->ident);
      RIGHTRD->categ = CERROR;
    }
  else if (QUAL && QUAL->categ == CNEW)
    {
      serror (melding, QUAL->ident);
      QUAL->categ = CERROR;
    }
  else if ((LEFT ? LEFTTYPE != TERROR : TRUE) 
	   && (RIGHT ? RIGHTTYPE != TERROR : TRUE) &&
       (UP ? UPTYPE != TERROR : TRUE) && (RD ? RD->type != TERROR : TRUE) &&
	   (QUAL ? QUAL->type != TERROR : TRUE) 
	   && (UPRD ? UPRD->type != TERROR : TRUE) &&
	   (TYPE != TERROR))
    serror (melding, RD ? RD->ident : 0);
  TYPE = TERROR;
}

/******************************************************************************
                                                               ARGUMENTERROR */

argumenterror (melding, re)
     int melding;
     struct EXP *re;
{
  int i = 1;
  if (TYPE == TERROR)
    return;
  TYPE = TERROR;
  if (UPTYPE == TERROR || (LEFT != NULL && LEFTTYPE == TERROR))
    return;
  for (re = UP; TOKEN == MARGUMENTSEP; re = UP)
    i++;
  if (re->type == TERROR)
    return;
  serror (melding, re->value.ident, i);

}

/******************************************************************************
                                                                SET_PARAM    */

set_param (re)
     struct EXP *re;
{
  re->right->rd = firstparam (re->rd);
  {
    struct EXP *rex;
    for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
      {
	if (moreparam (rex->rd) == FALSE)
	  argumenterror (102, rex);
	rex->right->rd = nextparam (rex->rd);
      }
    if (moreparam (rex->rd) == TRUE)
      argumenterror (102, rex);
  }
}

/******************************************************************************
                                                                SJEKKTYPE    */

sjekktype (re)
     struct EXP *re;
{

  /* SJEKKER F\RST AT NODENE HAR RIKTIGE TYPER */
  switch (TOKEN)
    {
    case MFOR:
    case MFORR:
      sjekktype (LEFT);
      TYPE = LEFTTYPE;
      QUAL = LEFTQUAL;
      sjekktype (RIGHT);
      if (TYPE != RIGHTTYPE)
	SERROR (86);
      else if (TOKEN == MFOR && TYPE == TREF)
	SERROR (86);
      else if (TOKEN == MFORR && (TYPE == TINTG
			|| TYPE == TREAL || TYPE == TCHAR || TYPE == TBOOL))
	SERROR (86);
      if (LEFTTOKEN != MIDENTIFIER)
	SERROR (87);
      else if (LEFTRD->categ == CNAME || LEFTRD->categ == CVAR)
	SERROR (87);
      else if (LEFTRD->kind != KSIMPLE)
	SERROR (87);
      break;
    case MLISTSEP:
    case MFORWHILE:
    case MSTEP:
    case MUNTIL:
    case MSWITCHSEP:
    case MBOUNDPARSEP:
    case MBOUNDSEP:
      TYPE = UPTYPE;
      QUAL = UPQUAL;
      sjekktype (LEFT);
      sjekktype (RIGHT);
      konvtype (&LEFT, TYPE, QUAL);
      TYPE = LEFTTYPE;
      if(TOKEN==MUNTIL && TYPE==TINTG && RIGHTTYPE==TREAL)
	{

	} 
      else
	{
	  konvtype (&RIGHT, TYPE, QUAL);
	  if (TOKEN == MFORWHILE)
	    {
	      if (RIGHTTYPE != TBOOL)
		SERROR (77);
	    }
	  else if (LEFTTYPE != RIGHTTYPE)
	    SERROR (88);
	}
      break;
    case MSWITCH:
      TYPE = TLABEL;
      sjekktype (LEFT);
      TYPE = LEFTTYPE;
      sjekktype (RIGHT);
      TYPE = RIGHTTYPE;
      break;
    case MARRAY:
      sjekktype (LEFT);
      localused = 0;
      TYPE = TINTG;
      sjekktype (RIGHT);
      if (localused)
	SERROR (89);
      break;
    case MARRAYSEP:
      /* Forutsetter at venstre node er en identifier */
      sjekktype (RIGHT);
      LEFTRD = findglobal (LEFTVALUE.ident, FALSE);
      if (LEFTRD->categ == CNEW)
	SERROR (75);
      LEFTTYPE = LEFTRD->type;
      break;
    case MASSIGN:
    case MASSIGNR:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      TYPE = LEFTTYPE;
      if (UPTOKEN != MASSIGN && UPTOKEN != MASSIGNR
	  && UPTOKEN != MENDASSIGN && UPTOKEN != MCONST)
	SERROR (118);
      else if (TYPE != TTEXT && LEFTTOKEN != MIDENTIFIER 
	       && LEFTTOKEN != MPROCASSIGN
	       && LEFTTOKEN != MARRAYARG && LEFTTOKEN != MDOT)
	SERROR (90);
      else if (LEFTTOKEN == MIFE)
	SERROR (90);
      konvtype (&RIGHT, LEFTTYPE, LEFTQUAL);
      if (LEFTTYPE != RIGHTTYPE)
	SERROR (91);
      else if (TOKEN == MASSIGNR)
	{
	  if (TYPE != TTEXT && TYPE != TREF)
	    SERROR (91);
	  if (TYPE == TTEXT)
	    TOKEN = MREFASSIGNT;
	}
      else
	{
	  if (TYPE != TINTG && TYPE != TREAL
	      && TYPE != TCHAR && TYPE != TBOOL && TYPE != TTEXT)
	    SERROR (91);
	  if (TYPE == TTEXT)
	    TOKEN = MVALASSIGNT;
	}
      break;
    case MLABEL:
      RD = findglobal (VALUE.ident, TRUE);
      break;
    case MIFE:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      if (LEFTTYPE != TBOOL)
	SERROR (77);
      else
	TYPE = RIGHTTYPE;
      QUAL = RIGHTQUAL;
      break;
    case MELSEE:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      sametype (&LEFT, &RIGHT);
      if (LEFTTYPE != RIGHTTYPE)
	SERROR (92);
      else if (LEFTTOKEN == MIFE)
	SERROR (93);
      else if ((TYPE = LEFTTYPE) == TREF && (QUAL
				= commonqual (LEFTQUAL, RIGHTQUAL)) == NULL)
	SERROR (94);

      break;
    case MCONC:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      if (LEFTTYPE != TTEXT || RIGHTTYPE != TTEXT)
	SERROR (109);
      else
	TYPE = TTEXT;
      break;
    case MORELSE:
    case MANDTHEN:
    case MEQV:
    case MIMP:
    case MOR:
    case MAND:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      if (LEFTTYPE != TBOOL || RIGHTTYPE != TBOOL)
	SERROR (95);
      else
	TYPE = TBOOL;
      break;
    case MNOT:
      sjekktype (LEFT);
      if (LEFTTYPE != TBOOL)
	SERROR (95);
      else
	TYPE = TBOOL;
      break;
    case MEQ:
    case MNE:
    case MLT:
    case MLE:
    case MGT:
    case MGE:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      sametype (&LEFT, &RIGHT);
      if (LEFTTYPE != RIGHTTYPE)
	SERROR (96);
      else if (LEFTTYPE != TINTG && LEFTTYPE != TREAL
	       && LEFTTYPE != TCHAR && LEFTTYPE != TTEXT)
	SERROR (96);
      else
	TYPE = TBOOL;
      if (LEFTTYPE == TTEXT)
	TOKEN = TOKEN - MEQ + MEQT;
      break;
    case MNER:
    case MEQR:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      if (LEFTTYPE != RIGHTTYPE)
	SERROR (96);
      else if (LEFTTYPE != TREF && LEFTTYPE != TTEXT)
	SERROR (96);
      else
	TYPE = TBOOL;
      if (LEFTTYPE == TTEXT)
	TOKEN = TOKEN - MNER + MNERT;
      break;
    case MIS:
    case MINS:
      VALUE = RIGHTVALUE;
      sjekktype (LEFT);
      RIGHTRD = RD = findglobal (VALUE.ident, FALSE);
      if (RIGHTRD->categ == CNEW)
	SERROR (75);
      if (LEFTTYPE != TREF)
	SERROR (96);
      else if (LEFTQUAL == NULL)
	SERROR (96);
      else if (RD->categ == CNEW)
	SERROR (96);
      else if (RD->kind != KCLASS)
	SERROR (96);
      else if (!commonqual (LEFTQUAL, RD))
	SERROR (85);
      TYPE = TBOOL;
      break;
    case MUADD:
    case MUSUB:
      sjekktype (LEFT);
      if (LEFTTYPE != TREAL && LEFTTYPE != TINTG)
	SERROR (97);
      else if (LEFTTOKEN == MUADD || LEFTTOKEN == MUSUB)
	SERROR (98);
      else
	TYPE = LEFTTYPE;
      if (TYPE == TINTG)
	TOKEN = TOKEN - MUADD + MUADDI;
      break;
    case MADD:
    case MSUB:
    case MMUL:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      sametype (&LEFT, &RIGHT);
      TYPE = LEFTTYPE;
      if (LEFTTYPE != RIGHTTYPE ||
	  (LEFTTYPE != TINTG && LEFTTYPE != TREAL))
	SERROR (97);
      if (TYPE == TINTG)
	TOKEN = TOKEN - MADD + MADDI;
      break;
    case MINTDIV:
      if (TRUE)
	TYPE = TINTG;
      else
    case MDIV:
	TYPE = TREAL;
      sjekktype (LEFT);
      sjekktype (RIGHT);
      konvtype (&LEFT, TYPE);
      konvtype (&RIGHT, TYPE);
      if (LEFTTYPE != TYPE || RIGHTTYPE != TYPE)
	SERROR (97);
      break;
    case MPRIMARY:
      sjekktype (LEFT);
      sjekktype (RIGHT);
      TYPE = TREAL;
      if ((LEFTTYPE == TINTG || LEFTTYPE == TREAL) &&
	  RIGHTTYPE == TREAL)
	konvtype (&LEFT, TREAL);
      else if (LEFTTYPE == TREAL && RIGHTTYPE == TINTG)
	TOKEN = MPRIMARYRI;
      else if (LEFTTYPE == TINTG && RIGHTTYPE == TINTG)
	{
	  TYPE = TINTG;
	  TOKEN = MPRIMARYII;
	}
      else
	SERROR (97);
      break;
    case MNOOP:
      sjekktype (LEFT);
      TYPE = LEFTTYPE;
      QUAL = LEFTQUAL;
      break;
    case MTEXTKONST:
      TYPE = TTEXT;
      break;
    case MCHARACTERKONST:
      TYPE = TCHAR;
      break;
    case MREALKONST:
      TYPE = TREAL;
      break;
    case MINTEGERKONST:
      TYPE = TINTG;
      break;
    case MBOOLEANKONST:
      TYPE = TBOOL;
      break;
    case MNONE:
      TYPE = TREF;
      QUAL = commonprefiks;
      break;
    case MIDENTIFIER:
      if (UPTOKEN == MDOT && ISRIGHT)
	RD = findlocal (VALUE.ident, UPQUAL, TRUE);
      else if (ISLEFT && (UPTOKEN == MASSIGN | UPTOKEN == MASSIGNR))
	RD = findglobal (VALUE.ident, FALSE);	/* Tilordning av
						 * funksjons proc. */
      else
	RD = findglobal (VALUE.ident, TRUE);
      if (RD->categ == CNEW)
	SERROR (75);
      SEENTHROUGH = seenthrough;
      TYPE = RD->type;
      QUAL = RD->prefqual;
      if (RD->type == TERROR)
	SERROR (106);
      if (TYPE == TLABEL && seenthrough != NULL)
	SERROR (8);
      if (RD == sourcelinefunction)
	{
	  TOKEN = MINTEGERKONST;
	  TYPE = TINTG;
	  VALUE.ival = yylineno;
	  RD = NULL;
	  SEENTHROUGH = NULL;
	}
      else if (RD->kind == KARRAY)
	{
	  if (ISLEFT)
	    {
	      if ((UPTOKEN != MARGUMENTSEP || UPRD->kind != KARRAY) &&
		  UPTOKEN != MSWITCH)
		SERROR (119);
	    }
	  else
	    {
	      if (UPTOKEN != MDOT || UPUPTOKEN != MARGUMENTSEP ||
		  UPUPRD->kind != KARRAY)
		SERROR (119);
	    }
	}
      else if (RD->kind == KPROC || RD->kind == KCLASS)
	{
	  if (ISLEFT && (UPTOKEN == MASSIGN | UPTOKEN == MASSIGNR))
	    if (body (RD))
	      TOKEN = MPROCASSIGN;
	    else
	      SERROR (90);
	  else if (moreparam (firstparam (RD)) == TRUE)
	    SERROR (107);
	  else if (RD->kind == KPROC)
	    TOKEN = MPROCARG;
	  else
	    TOKEN = MARGUMENT;
	  RIGHT = newexp;
	  RIGHTTOKEN = MENDSEP;
	  RIGHTVALUE.rval = 0.0;
	  RIGHTRIGHT = NULL;
	  RIGHTLEFT = NULL;
	  RIGHTRD = NULL;
	  RIGHTQUAL = NULL;
	}
      else if (RD->categ == CCONST)
	{
	  if (TYPE == TREAL)
	    TOKEN = MREALKONST;
	  else if (TYPE == TINTG)
	    TOKEN = MINTEGERKONST;
	  else if (TYPE == TTEXT)
	    TOKEN = MTEXTKONST;
	  else if (TYPE == TCHAR)
	    TOKEN = MCHARACTERKONST;
	  else if (TYPE == TBOOL)
	    TOKEN = MBOOLEANKONST;
	  VALUE = RD->value;
	  if (UPTOKEN == MDOT)
	    UPTOKEN = MDOTCONST;
	}
      else if (RD->categ == CCONSTU)
	{
	  if ((ISLEFT && (UPTOKEN == MASSIGN | UPTOKEN == MASSIGNR)) |
	      (ISRIGHT && UPTOKEN == MDOT && UPISLEFT &&
	       (UPUPTOKEN == MASSIGN | UPUPTOKEN == MASSIGNR)))
	    {
	      if (RD->encl->blev != cblev)
		SERROR (7);
	    }
	  else
	    SERROR (7);
	}
      break;
    case MTHIS:
      RD = regthis (VALUE.ident);
      if (RD->categ == CNEW)
	SERROR (75);
      TYPE = TREF;
      QUAL = RD;
      SEENTHROUGH = seenthrough;
      if (RD->kind != KCLASS)
	SERROR (99);
      break;
    case MDOT:
      sjekktype (LEFT);
      TYPE = LEFTTYPE;
      QUAL = LEFTQUAL;
      VALUE = LEFTVALUE;
      SEENTHROUGH = LEFTSEENTHROUGH;
      if (TYPE == TTEXT)
	{
	  QUAL = classtext;
	}
      if (LEFTTOKEN == MNONE)
	SERROR (9);
      if (TYPE == TREF)
	VALUE.ival = QUAL->descr->blev;
      if (TYPE != TTEXT && TYPE != TREF)
	SERROR (100);
      if (RIGHTTOKEN != MIDENTIFIER && RIGHTTOKEN != MARGUMENT
	  && RIGHTTOKEN != MARRAYARG && RIGHTTOKEN != MPROCARG)
	SERROR (116);
      sjekktype (RIGHT);
      if (LEFTTYPE == TREF && RIGHTTYPE == TREF &&
	  LEFTQUAL->descr->blev < RIGHTQUAL->descr->blev)
	SERROR (117);
      TYPE = RIGHTTYPE;
      QUAL = RIGHTQUAL;
      RD = RIGHTRD;
      VALUE = RIGHTVALUE;
      if (TYPE == TLABEL)
	SERROR (8);
      break;
    case MNEWARG:
      RD = findglobal (VALUE.ident, FALSE);
      if (RD->categ == CNEW)
	SERROR (75);
      TYPE = TREF;
      QUAL = RD;
      if (RD->kind != KCLASS)
	SERROR (99);
      set_param (re);
      sjekktype (RIGHT);
      if (RIGHTTYPE == TERROR)
	TYPE = TERROR;
      break;
    case MQUA:
      sjekktype (LEFT);
      QUAL = findglobal (VALUE.ident, FALSE);
      if (QUAL->categ == CNEW)
	SERROR (75);
      TYPE = LEFTTYPE;
      if (LEFTTOKEN == MNONE)
	SERROR (9);
      if (TYPE != TREF)
	SERROR (100);
      else if (QUAL->kind != KCLASS)
	SERROR (99);
      else if ((RD = commonqual (LEFTQUAL, QUAL)) == NULL)
	SERROR (85);
      else if (QUAL == RD)
	TOKEN = MQUANOTNONE;
      else if (RD != LEFTQUAL)
	SERROR (85);
      break;
    case MARGUMENT:
      if (UPTOKEN == MDOT && ISRIGHT)
	RD = findlocal (VALUE.ident, UPQUAL, TRUE);
      else
	RD = findglobal (VALUE.ident, TRUE);
      if (RD->categ == CNEW)
	SERROR (75);
      SEENTHROUGH = seenthrough;
      TYPE = RD->type;
      QUAL = RD->prefqual;
      if (RD->kind == KARRAY)
	TOKEN = MARRAYARG;
      else if (RD->kind == KPROC)
	TOKEN = MPROCARG;
      else if (RD->kind == KCLASS && UPTOKEN == MPRBLOCK);
      else
	SERROR (101);
      set_param (re);
      sjekktype (RIGHT);
      if (RIGHTTYPE == TERROR)
	TYPE = TERROR;
      if (RD == absfunction)
	{
	  if (RIGHTLEFTTYPE == TINTG)
	    RD = absfunctioni;
	  else
	    RD = absfunctionr;
	  TYPE = RD->type;
	  set_param (re);
	  sjekktype (RIGHT);
	  if (RIGHTTYPE == TERROR)
	    TYPE = TERROR;
	}
      else if (RD == signfunction)
	{
	  if (RIGHTLEFTTYPE == TINTG)
	    RD = signfunctioni;
	  else
	    RD = signfunctionr;
	  TYPE = RD->type;
	  set_param (re);
	  sjekktype (RIGHT);
	  if (RIGHTTYPE == TERROR)
	    TYPE = TERROR;
	}
      else if (RD == minfunction && RIGHTRIGHTLEFT != NULL)
	{
	  if (RIGHTLEFTTYPE == TTEXT)
	    RD = minfunctiont;
	  else if (RIGHTLEFTTYPE == TCHAR)
	    RD = minfunctionc;
	  else if (RIGHTLEFTTYPE == TINTG && RIGHTRIGHTLEFTTYPE == TINTG)
	    RD = minfunctioni;
	  else
	    RD = minfunctionr;
	  TYPE = RD->type;
	  set_param (re);
	  sjekktype (RIGHT);
	  if (RIGHTTYPE == TERROR)
	    TYPE = TERROR;
	}
      else if (RD == maxfunction && RIGHTRIGHTLEFT != NULL)
	{
	  if (RIGHTLEFTTYPE == TTEXT)
	    RD = maxfunctiont;
	  else if (RIGHTLEFTTYPE == TCHAR)
	    RD = maxfunctionc;
	  else if (RIGHTLEFTTYPE == TINTG && RIGHTRIGHTLEFTTYPE == TINTG)
	    RD = maxfunctioni;
	  else
	    RD = maxfunctionr;
	  TYPE = RD->type;
	  set_param (re);
	  sjekktype (RIGHT);
	  if (RIGHTTYPE == TERROR)
	    TYPE = TERROR;
	}
      break;
    case MARGUMENTSEP:
      TYPE = RD->type;
      QUAL = RD->prefqual;
      if (RD->type == TVARARGS)
	{
	  struct DECL *rdx;
	  sjekktype (LEFT);
	  rdx = RD;
	  if (RD->categ == CNAME || RD->categ == CVAR)
	    {
	      if (LEFTTYPE == TINTG)
		RD = varargsintvar;
	      else if (LEFTTYPE == TREAL)
		RD = varargsrealvar;
	      else if (LEFTTYPE == TCHAR)
		RD = varargscharvar;
	      else if (LEFTTYPE == TTEXT)
		RD = varargstext;
	      else
		argumenterror (104, re);
	      if (LEFTTOKEN != MDOT && LEFTTOKEN != MIDENTIFIER
		  && LEFTTOKEN != MARRAYARG)
		argumenterror (110, re);
	    }
	  else if (RD->categ == CVALUE || RD->categ == CDEFLT)
	    {
	      if (LEFTTYPE == TINTG)
		RD = varargsint;
	      else if (LEFTTYPE == TREAL)
		RD = varargsreal;
	      else if (LEFTTYPE == TCHAR)
		RD = varargschar;
	      else if (LEFTTYPE == TTEXT && RD->categ == CDEFLT)
		RD = varargstext;
	      else if (LEFTTYPE == TTEXT && RD->categ == CVALUE)
		RD = varargstextvalue;
	      else
		argumenterror (104, re);
	    }
	  RD->encl = rdx->encl;
	  RD->next = rdx;
	}
      else if (RD->kind == KARRAY || RD->kind == KPROC)
	{			/* Parameteren skal v{re ett array eller en
				 * prosedyre */
	  if (LEFTTOKEN == MDOT)
	    {
	      sjekktype (LEFT->left);
	      LEFTRIGHTQUAL = LEFTQUAL = LEFTLEFTQUAL;
	      LEFTSEENTHROUGH = LEFTLEFTSEENTHROUGH;
	      LEFTRIGHTRD = LEFTRD =
		findlocal (LEFTRIGHTVALUE.ident, LEFTQUAL, TRUE);
	      if (LEFTRD->categ == CNEW)
		SERROR (75);
	      if (LEFTRD->kind != RD->kind)
		argumenterror (111, re);
	      LEFTTYPE = LEFTRD->type;
	      if (TYPE != LEFTTYPE && TYPE != TNOTY && TYPE != TALLTY)
		{
		  if (RD->categ != CNAME & RD->categ != CVAR
		      || TYPE != TINTG & TYPE != TREAL
		      || LEFTTYPE != TINTG & LEFTTYPE != TREAL)
		    argumenterror (104, re);
		}
	      if (RD->kind == KPROC)
		{
		  if (RD->encl->quant.categ == CCPROC &&
		      LEFTRD->categ != CCPROC)
		    argumenterror (111, re);
		  if (RD->encl->quant.categ != CCPROC &&
		      LEFTRD->categ == CCPROC)
		    argumenterror (111, re);
		  if (!subordinate (LEFTRD, RD)) argumenterror (112, re);
		  if (!sameparam (RD->descr, LEFTRD->descr))
		    argumenterror (112, re);
		}
	    }
	  else if (LEFTTOKEN != MIDENTIFIER)
	    {
	      argumenterror (103, re);
	      sjekktype (LEFT);
	    }
	  else
	    {
	      LEFTRD = findglobal (LEFTVALUE.ident, TRUE);
	      LEFTSEENTHROUGH = seenthrough;
	      LEFTQUAL = LEFTRD->prefqual;
	      if (LEFTRD->categ == CNEW)
		SERROR (75);
	      if (LEFTRD->kind != RD->kind)
		argumenterror (111, re);
	      LEFTTYPE = LEFTRD->type;
	      if (TYPE != LEFTTYPE && TYPE != TNOTY && TYPE != TALLTY)
		{
		  if (RD->categ != CNAME & RD->categ != CVAR
		      || TYPE != TINTG & TYPE != TREAL
		      || LEFTTYPE != TINTG & LEFTTYPE != TREAL)
		    argumenterror (104, re);
		}
	      if (RD->kind == KPROC)
		{
		  if (RD->encl->quant.categ == CCPROC &&
		      LEFTRD->categ != CCPROC)
		    argumenterror (111, re);
		  if (RD->encl->quant.categ != CCPROC &&
		      LEFTRD->categ == CCPROC)
		    argumenterror (111, re);
		  if (!subordinate (LEFTRD, RD)) argumenterror (112, re);
		  if (!sameparam (RD->descr, LEFTRD->descr))
		    argumenterror (112, re);
		}
	    }
	}
      else if (RD->kind == KSIMPLE)
	{			/* Parameteren skal v{re simple */
	  sjekktype (LEFT);
	  if (RD->categ == CNAME)
	    {
	      if (TYPE != LEFTTYPE && (TYPE != TINTG & TYPE != TREAL
				  || LEFTTYPE != TINTG & LEFTTYPE != TREAL))
		argumenterror (104, re);
	    }
	  else if (RD->categ == CVAR)
	    {
	      if (TYPE != LEFTTYPE && (TYPE != TINTG & TYPE != TREAL
				  || LEFTTYPE != TINTG & LEFTTYPE != TREAL))
		argumenterror (104, re);
	      if (LEFTTOKEN != MDOT && LEFTTOKEN != MIDENTIFIER
		  && LEFTTOKEN != MARRAYARG && LEFTTYPE != TLABEL)
		argumenterror (110, re);
	    }
	  else
	    {
	      konvtype (&LEFT, TYPE, QUAL);
	      if (TYPE != LEFTTYPE)
		argumenterror (104, re);
	    }
	}
      else
	{
	  argumenterror (105, re);
	  sjekktype (LEFT);
	}
      sjekktype (RIGHT);
      if (RIGHTTYPE == TERROR)
	TYPE = TERROR;
      break;
    case MENDSEP:
      newarg = 0;
      switch (UPTOKEN)
	{
	case MSWITCHSEP:
	  TYPE = TLABEL;
	  break;
	case MLISTSEP:
	  TYPE = UPTYPE;
	  break;
	case MARGUMENT:
	case MARGUMENTSEP:
	case MPROCARG:
	case MNEWARG:
	case MARRAYARG:
	case MARRAYSEP:
	  TYPE = TNOTY;
	  break;
	case MBOUNDSEP:
	  TYPE = TINTG;
	  break;
	default:
	  TYPE = TERROR;
	  break;
	}
      break;
    }
}

/******************************************************************************
                                                                SSTRCMP      */

#define convbslstr(s) ((s[i]-'0')*64+(s[i+1]-'0')*8+s[i+2]-'0')
int 
sstrcmp (s, t)
     char s[],
       t[];
{
  int i,
    ss,
    tt;
  i = 0;
  if (s == NOTEXT)
    if (t == NOTEXT)
      return (0);
    else
      return ((char) -1);
  while (s[i] == t[i])
    {
      if (s[i] == '\0')
	return (0);
      if (s[i++] == '\\')
	{
	  ss = convbslstr (s);
	  tt = convbslstr (t);
	  i += 3;
	  if (ss != tt)
	    return (ss - tt);
	}
    }
  ss = s[i];
  tt = t[i];
  i++;
  if (ss == '\\')
    ss = convbslstr (s);
  if (tt == '\\')
    tt = convbslstr (t);
  return (ss - tt);
}

/******************************************************************************
                                                                     SSTRLEN */

int
sstrlen (s)
     char s[];
{
  int i = 0,
    ii = 0;
  if (s == NOTEXT)
    return (0);
  while (s[i])
    {
      ii++;
      if (s[i++] == '\\')
	if (s[i++] == '\n')
	  ii--;
	else
	  i += 2;
    }
  if (ii >= MAX_TEXT_CHAR)
    serror (44);
  return (ii);
}

/******************************************************************************
                                                                COMPUTECONST */

/* Denne rutinen tar som parameter et uttrykks-tre.
 * Den pr|ver } beregne de deler av uttrykket som er constant
 * og erstatte det med et objekt med token lik M...CONST og
 * som har value lik den beregnede constanten.
 * Rutinen er rekursiv
 * og den returnerer TRUE dersom uttrykket lar seg beregne
 * Den fjerner ogs} noder med token lik MNOOP, med unntak n}
 * typen er lik TTEXT og tokenet til noden over i treet er lik MDOT,
 * og tokenet til venstrenoden er lik MIF, MARRAYARG eller MIDENTIFIER.
 * Den fjerner noder med token lik MREAINT 
 * og MINTREA mellom multippel assign.*/

char 
computeconst (re)
     struct EXP *re;
{
  char lconst,
    rconst;
  switch (TOKEN)
    {
    case MIFE:
      lconst = computeconst (LEFT);
      if (lconst == TRUE)
	{
	  struct EXP *rex;
	  if (LEFTVALUE.ival == TRUE)
	    rex = RIGHTLEFT;
	  else
	    rex = RIGHTRIGHT;
	  re->token = rex->token;
	  re->value = rex->value;
	  re->type = rex->type;
	  re->left = rex->left;
	  re->right = rex->right;
	  re->rd = rex->rd;
	  re->qual = rex->qual;
	  re->seenthrough = rex->seenthrough;
	  return (computeconst (re));
	}
      else
	{
	  (void) computeconst (RIGHT);
	  return (FALSE);
	}
      break;
    case MORELSE:
      lconst = computeconst (LEFT);
      if (lconst == FALSE)
	{
	  (void) computeconst (RIGHT);
	  return (FALSE);
	}
      if (LEFTVALUE.ival == TRUE)
	{
	  TOKEN = MBOOLEANKONST;
	  VALUE.ival = TRUE;
	  LEFT = RIGHT = NULL;
	  return (TRUE);
	}
      rconst = computeconst (RIGHT);
      if (rconst == FALSE)
	return (FALSE);
      TOKEN = MBOOLEANKONST;
      VALUE = RIGHTVALUE;
      LEFT = RIGHT = NULL;
      return (TRUE);
    case MANDTHEN:
      lconst = computeconst (LEFT);
      if (lconst == FALSE)
	{
	  (void) computeconst (RIGHT);
	  return (FALSE);
	}
      if (LEFTVALUE.ival == FALSE)
	{
	  TOKEN = MBOOLEANKONST;
	  VALUE.ival = FALSE;
	  LEFT = RIGHT = NULL;
	  return (TRUE);
	}
      rconst = computeconst (RIGHT);
      if (rconst == FALSE)
	return (FALSE);
      TOKEN = MBOOLEANKONST;
      VALUE = RIGHTVALUE;
      LEFT = RIGHT = NULL;
      return (TRUE);
    default:
      if (LEFT != NULL)
	lconst = computeconst (LEFT);
      if (RIGHT != NULL)
	rconst = computeconst (RIGHT);
      if (lconst == FALSE && LEFT != NULL)
	{
	  if (TOKEN == MNOOP && (TYPE != TTEXT || UPTOKEN != MDOT 
				 || (LEFTTOKEN != MIFE
		    && LEFTTOKEN != MARRAYARG && LEFTTOKEN != MIDENTIFIER)))
	    {
	      LEFT->up = UP;
	      if (ISLEFT)
		UP->left = LEFT;
	      else
		UP->right = LEFT;
	    }
	  else if ((TOKEN == MREAINT | TOKEN == MINTREA) && ISRIGHT &&
		   UPTOKEN == MASSIGN && LEFTTOKEN == MASSIGN)
	    {
	      LEFT->up = UP;
	      UP->right = LEFT;
	    }
	  return (FALSE);
	}
      if (rconst == FALSE && RIGHT != NULL)
	return (FALSE);
      break;
    }
  switch (TOKEN)
    {
    case MCONC:
      {
	char *l,
	 *r,
	 *s;
	l = LEFTVALUE.tval.txt;
	r = RIGHTVALUE.tval.txt;
	s = (char *) salloc (strlen (l) + strlen (r) + 1);
	(void) strcpy (s, l);
	(void) strcat (s, r);
	TOKEN = MTEXTKONST;
	VALUE.tval.txt = s;
      }
      break;
    case MOR:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival | RIGHTVALUE.ival;
      break;
    case MAND:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival & RIGHTVALUE.ival;
      break;
    case MEQV:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival == RIGHTVALUE.ival;
      break;
    case MIMP:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = !(LEFTVALUE.ival & !RIGHTVALUE.ival);
      break;
    case MNOT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = !LEFTVALUE.ival;
      break;
    case MEQT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) == 0;
      break;
    case MNET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) != 0;
      break;
    case MLTT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) < 0;
      break;
    case MLET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) <= 0;
      break;
    case MGTT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) > 0;
      break;
    case MGET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) >= 0;
      break;
    case MEQ:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival == RIGHTVALUE.ival;
      break;
    case MNE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival != RIGHTVALUE.ival;
      break;
    case MLT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival < RIGHTVALUE.ival;
      break;
    case MLE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival <= RIGHTVALUE.ival;
      break;
    case MGT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival > RIGHTVALUE.ival;
      break;
    case MGE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival >= RIGHTVALUE.ival;
      break;
#if FLOAT_IMPLEMENTED
    case MUADD:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval;
      break;
    case MUSUB:
      TOKEN = MREALKONST;
      VALUE.rval = -LEFTVALUE.rval;
      break;
    case MADD:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval + RIGHTVALUE.rval;
      break;
    case MSUB:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval - RIGHTVALUE.rval;
      break;
    case MMUL:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval * RIGHTVALUE.rval;
      break;
    case MDIV:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval / RIGHTVALUE.rval;
      break;
    case MPRIMARY:
      TOKEN = MREALKONST;
      VALUE.rval = pow (LEFTVALUE.rval, RIGHTVALUE.rval);
      break;
    case MPRIMARYRI:
      {
	long i;
	VALUE.rval = 1.0;
	if (RIGHTVALUE.ival < 0)
	  serror (4);
	for (i = 1; i <= RIGHTVALUE.ival; i++)
	  VALUE.rval *= LEFTVALUE.rval;
	TOKEN = MREALKONST;
      }
      break;
    case MREAINT:
      TOKEN = MREALKONST;
      VALUE.rval = (double) LEFTVALUE.ival;
      break;
    case MINTREA:
      TOKEN = MINTEGERKONST;
      VALUE.ival = (long) (LEFTVALUE.rval += 0.5);
      if (VALUE.ival > LEFTVALUE.rval)
	VALUE.ival--;
      break;
#endif
    case MUADDI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival;
      break;
    case MUSUBI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = -LEFTVALUE.ival;
      break;
    case MADDI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival + RIGHTVALUE.ival;
      break;
    case MSUBI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival - RIGHTVALUE.ival;
      break;
    case MMULI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival * RIGHTVALUE.ival;
      break;
    case MINTDIV:
      TOKEN = MINTEGERKONST;
      if (RIGHTVALUE.ival == 0)
	{
	  serror (1);
	  VALUE.ival = LEFTVALUE.ival;
	}
      else
	VALUE.ival = LEFTVALUE.ival / RIGHTVALUE.ival;
      break;
    case MPRIMARYII:
      {
	long i;
	VALUE.ival = 1;
	if (RIGHTVALUE.ival < 0)
	  serror (4);
	for (i = 1; i <= RIGHTVALUE.ival; i++)
	  VALUE.ival *= LEFTVALUE.ival;
	TOKEN = MINTEGERKONST;
      }
      break;
    case MNOOP:
      TOKEN = LEFTTOKEN;
      VALUE = LEFTVALUE;
      break;
    case MREALKONST:
    case MTEXTKONST:
    case MCHARACTERKONST:
    case MINTEGERKONST:
    case MBOOLEANKONST:
      return (TRUE);
    default:
      return (FALSE);
    }
  LEFT = NULL;
  RIGHT = NULL;
  return (TRUE);
}

/******************************************************************************
                                                            SETDANGER_CONST  */

/* Setter attributtet danger og const
 * F|lgende attributter blir satt danger:
 *
 *  MNEWARG:      Alltid satt
 *  MARRAYARG:    Alltid satt
 *  MCONC:        Alltid satt
 *  MPROCARG:     Hvis det er en prosedyre som m} skilles ut av uttrykket
 *  MASSIGNR:     For den siste assign i multippel assign. dette fordi
 *                gencall da m} kalle genstack: F|lgende kan v{re farlig:
 *                ra.ra:-ra:-...;
 *  MREFASSIGNT:  I det tilfellet at noden over er MVALASSIGNT.
 *                F|lgend kan f.eks. v{re farlig:
 *                t:=t:-....;
 *  MIDENTIFIER:  I det tilfellet at det er en name
 *  MORELSE:      I det tilfellet at enten venstregrenen eller h|yregrenen
 *                inneholder et attributt som er danger
 *  MANDTHEN:     Samme som ovenfor
 *  MIFE:         Samme som ovenfor
 *  MELSE:        Samme som ovenfor */

char 
setdanger_const (re)
     struct EXP *re;
{
  char sub_danger;
  if (LEFT != NULL)
    sub_danger = setdanger_const (LEFT);
  if (RIGHT != NULL)
    sub_danger |= setdanger_const (RIGHT);
  DANGER = FALSE;
  switch (re->token)
    {
    case MNEWARG:
    case MARRAYARG:
    case MCONC:
      DANGER = TRUE;
      break;
    case MPROCARG:
      DANGER = dangerproc (RD);
      if (DANGER == FALSE)
	{
	  struct EXP *rex;
	  KONST = TRUE;
	  for (rex = RIGHT; rex->token != MENDSEP; rex = rex->right)
	    {
	      switch (rex->left->token)
		{
		case MNONE:
		case MREALKONST:
		case MTEXTKONST:
		case MCHARACTERKONST:
		case MINTEGERKONST:
		case MBOOLEANKONST:
		  continue;
		}
	      KONST = FALSE;
	      break;
	    }
	}
      break;
    case MASSIGNR:
      if (UPTOKEN == MASSIGNR && RIGHTTOKEN != MASSIGNR)
	DANGER = TRUE;
      break;
    case MREFASSIGNT:
      if (UPTOKEN == MVALASSIGNT)
	DANGER = TRUE;
      break;
    case MIDENTIFIER:
      if (RD->kind == CNAME)
	DANGER = TRUE;
      break;
    case MORELSE:
    case MANDTHEN:
    case MIFE:
    case MELSE:
      DANGER = sub_danger;
      break;
    }
  return (sub_danger | DANGER);
}
