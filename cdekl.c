/* $Id: cdekl.c,v 1.10 1997/01/08 09:49:13 cim Exp $ */

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

/* Deklarasjonslager for Simula */

#include "cextern.h"
#include "cdekl.h"
#include "cconst.h"

extern char staticblock;

/*****************************************************************************/
/*                            INITIELT                                       */
/*****************************************************************************/


#define new(X) ((struct X *)salloc(sizeof(struct X)))

/* KONTAKT MED YACC, LEX OG FEILSYSTEMET */

/* Bruker bufferet til lex da det garantert er stort nokk
 * Dette blir brukt i det tilfellet at det m} lages en ny tag
 * for identen til en parameter som har navnkonflikt med en lokal variabel */

char *prefquantident;
char type,
  kind,
  categ;
int localused,
  arrdim = 0;

/* Arraypekere som  peker paa  blokkobjektene
 * Display inneholder alle de aktive blokkene */
struct BLOCK *last_block;
struct BLOCK *display[DECLTABSIZE];
struct BLOCK *cblock;
struct BLOCK *sblock;
struct BLOCK *seenthrough;	/* Settes av findglobal og findlocal og peker 
				 * p}  en utenforliggende inspect blokk(hvis
				 * den      finnes). Det er fordi jeg onsker
				 * } vite n}r en variable er sett gjennom
				 * inspect. Trenger      denne informasjon i
				 * kode genereringen for }    aksessere
				 * variable fra den inspiserte klassen 
				 * gjennom inspect variabelen */
struct DECL *classtext;


int cblev,
  crblev;

/* Arraypekere som peker på aktive blokker som er
 * forskjellig fra en inspekt, conn eller for blokk
 * Forn og conn er dybden paa n\sting av disse blokkene */
short forn[DECLTABSIZE];
short conn[DECLTABSIZE];
int cdisbl;

struct DECL *prefdisplay[DECLTABSIZE];

/* Hvert blokkobjekt har lister av deklarasjoner
 * Hver   ny   deklarasjon   skal   hektes   på
 * slutten  av en  liste,  Trenger  da en  peker
 * til siste  deklarasjon  for hver  aktiv blokk */
 struct DECL *lastdecl[DECLTABSIZE];
 struct DECL *clastdecl;
 struct DECL *cprevdecl;
 struct DECL *lastvirt[DECLTABSIZE];
 
 
/* Har en peker som peker p} en array deklarasjon som ikke har f}tt
 * satt sin dim verdi. */
struct DECL *sistearray = NULL;

/* Under sjekkingen og innlesingen av deklarasjonene
 * trenger jeg å merke de ulike objektene
 * Lar atributter peke paa ulike objekter for merkeingen */
struct DECL *sjekkdeklcalled;
struct DECL *lastunknowns;
struct BLOCK *unknowns;
struct DECL *commonprefiks;	/* Felles prefiks til alle ikke prefiksede
				 * klasser Inneholder prosedyren DETACH */
struct DECL *switchparam;
struct DECL *procparam;
struct DECL *sluttparam;
struct DECL *arrayparam;

char insert_with_codeclass;

/******************************************************************************
                                                        NULLBLOCK & NULLDEKL */

/* Nullblock og nulldekl renser objekter */

nulldekl (rd)
     struct DECL *rd;
{
  rd->ident = NULL;
  rd->line = 0L;
  rd->type = 0;
  rd->kind = 0;
  rd->categ = 0;
  rd->encl = NULL;
  rd->protected = 0;
  rd->idplev.plev = 0;
  rd->dim = 0;
  rd->virtno = 0;
  rd->descr = NULL;
  rd->match = NULL;
  rd->next = NULL;
  rd->prefqual = NULL;
}

nullblock (rb)
     struct BLOCK *rb;
{
  /* Unødig å nullstille last_line, blno, 
   * blev og kind da de blir tilordnet */

  nulldekl (&rb->quant);
  rb->quant.descr = rb;
  rb->inner = 0;
  rb->ptypno = 0;
  rb->napar = 0;
  rb->naloc = 0;
  rb->navirt = 0;
  rb->navirtlab = 0;
  rb->fornest = 0;
  rb->connest = 0;
  rb->localclasses = 0;
  rb->thisused = 0;
  rb->stat = 0;
  rb->enddecl = 0;
  rb->parloc = NULL;
  rb->virt = NULL;
  rb->hiprot = NULL;
  rb->externid = 0;
  rb->next_block = NULL;
  rb->ent = 0;
}

/******************************************************************************
                                                                INITDEKL     */

/* Initdekl kalles før selve innlesingen */

initdekl ()
{
  struct BLOCK *rb;
  struct DECL *rd;

  type = TNOTY;
  cdisbl = cblev = crblev = -1;
  sjekkdeklcalled = new (DECL);
  unknowns = new (BLOCK);
  nullblock (unknowns);
  unknowns->quant.line = 0L;
  unknowns->blno = 0;
  unknowns->blev = 0;
  unknowns->quant.kind = KERROR;

  newblck (KBLOKK);
  lesinn_external_spec (tag ("TEXTOBJ*", 8), "simenvir");

  commonprefiks = findglobal (tag ("COMMON*", 7), TRUE);
  commonprefiks->idplev.plev = -1;
  classtext = findglobal (tag ("TEXTOBJ*", 8), TRUE);
#if 0
  (rb = findglobal (tag ("FILE", 4), TRUE)->descr)->blno = 1;
  rb->ptypno = 1;
  (rb = findglobal (tag ("IMAGEFILE", 9), TRUE)->descr)->blno = 2;
  rb->ptypno = 2;
  (rb = findglobal (tag ("OUTFILE", 7), TRUE)->descr)->blno = 3;
  rb->ptypno = 3;
  (rb = findglobal (tag ("INFILE", 6), TRUE)->descr)->blno = 4;
  rb->ptypno = 4;
  (rb = findglobal (tag ("DIRECTFILE", 10), TRUE)->descr)->blno = 5;
  rb->ptypno = 5;
  (rb = findglobal (tag ("PRINTFILE", 9), TRUE)->descr)->blno = 6;
  rb->ptypno = 6;
  (rb = findglobal (tag ("BYTEFILE", 8), TRUE)->descr)->blno = 7;
  rb->ptypno = 7;
  (rb = findglobal (tag ("INBYTEFILE", 10), TRUE)->descr)->blno = 8;
  rb->ptypno = 8;
  (rb = findglobal (tag ("OUTBYTEFILE", 11), TRUE)->descr)->blno = 9;
  rb->ptypno = 9;
  (rb = findglobal (tag ("DIRECTBYTEFILE", 14), TRUE)->descr)->blno = 10;
  rb->ptypno = 10;
#endif

  newblck (KINSP);
  newblck (KINSP);
#if REALASDOUBLE
  rd = findglobal (tag ("MAXLONGREAL", 11), TRUE);
  rd->value.rval = MAX_DOUBLE;
  rd = findglobal (tag ("MINLONGREAL", 11), TRUE);
  rd->value.rval = -MAX_DOUBLE;
  rd = findglobal (tag ("MAXREAL", 7), TRUE);
  rd->value.rval = MAX_DOUBLE;
  rd = findglobal (tag ("MINREAL", 7), TRUE);
  rd->value.rval = -MAX_DOUBLE;
#else
  rd = findglobal (tag ("MAXLONGREAL", 11), TRUE);
  rd->value.rval = MAX_FLOAT;
  rd = findglobal (tag ("MINLONGREAL", 11), TRUE);
  rd->value.rval = -MAX_FLOAT;
  rd = findglobal (tag ("MAXREAL", 7), TRUE);
  rd->value.rval = MAX_FLOAT;
  rd = findglobal (tag ("MINREAL", 7), TRUE);
  rd->value.rval = -MAX_FLOAT;
#endif
  rd = findglobal (tag ("MAXRANK", 7), TRUE);
  rd->value.ival = MAXRANK;
  rd = findglobal (tag ("MAXINT", 6), TRUE);
  rd->value.ival = MAX_INT;
  rd = findglobal (tag ("MININT", 6), TRUE);
  rd->value.ival = -MAX_INT - 1;
}

/******************************************************************************
                                                                REINIT       */

/* Reinit kalles før sjekkingen starter */

reinit ()
{
  struct DECL *rd;
  struct DECL *findglobal ();
  endblock ();
  endblock ();
  endblock ();
  /* M} gj|re et hack for } f} satt kvalifikasjon p} inspect sysin */
  /* og inspect sysout, da neste blokk ikke er en connection blokk */
  inblock ();
  inblock (findglobal (tag ("INFILE", 6), TRUE));
  cblock->parloc = findglobal (tag ("INFILE", 6), TRUE);
  inblock (findglobal (tag ("PRINTFILE", 9), TRUE));
  cblock->parloc = findglobal (tag ("PRINTFILE", 9), TRUE);
  sblock = cblock = cblock->next_block;
  nulldekl (switchparam = new (DECL));
  switchparam->ident = 0;
  switchparam->line = 0L;
  switchparam->type = TINTG;
  switchparam->kind = KSIMPLE;
  switchparam->categ = CDEFLT;
  switchparam->encl = unknowns;
  nulldekl (switchparam->next = new (DECL));
  switchparam->next->ident = 0;
  switchparam->next->line = 0L;
  switchparam->next->type = TINTG;
  switchparam->next->kind = KSIMPLE;
  switchparam->next->categ = CDEFLT;
  switchparam->next->encl = unknowns;
  switchparam->next->next = switchparam->next;
  nulldekl (procparam = new (DECL));
  procparam->ident = 0;
  procparam->line = 0L;
  procparam->type = TERROR;
  procparam->kind = TERROR;
  procparam->categ = CNAME;
  procparam->encl = unknowns;
  procparam->next = procparam;
  nulldekl (sluttparam = new (DECL));
  sluttparam->ident = 0;
  sluttparam->line = 0L;
  sluttparam->type = 0;
  sluttparam->kind = 0;
  sluttparam->categ = 0;
  sluttparam->encl = unknowns;
  sluttparam->next = sluttparam;
  nulldekl (arrayparam = new (DECL));
  arrayparam->ident = 0;
  arrayparam->line = 0L;
  arrayparam->type = TINTG;
  arrayparam->kind = KSIMPLE;
  arrayparam->categ = CDEFLT;
  arrayparam->encl = unknowns;
  arrayparam->next = arrayparam;
}

/*****************************************************************************/
/*                               HJELPE-PROSEDYRER                           */
/*****************************************************************************/

/******************************************************************************
                                                           SETARRAYDIM       */

/* Sistearray peker på første array i siste arraydeklarasjon og settarraydim
 * settes disse arrayenes dimensjon (dim). Så lengde next også er en array   
 * skal også denne ha dimmensjonen arrdim.( integer array a,b(...); */

settarraydim ()
{
  while (sistearray != NULL)
    {
      sistearray->dim = arrdim;
      sistearray = (sistearray->next == NULL ? NULL :
	      (sistearray->next->kind == KARRAY ? sistearray->next : NULL));
    }
  arrdim = 0;
}

/******************************************************************************
                                                               NEWNOTSEEN    */

/* Newnotseen kalles hver gang det er noe udeklarert
 * Den legger alle disse inn i en liste med de ukjente */

struct DECL *
newnotseen (ident)
     char *ident;
{
  if (lastunknowns == NULL)
    unknowns->parloc = lastunknowns = new (DECL);
  else
    lastunknowns = lastunknowns->next = new (DECL);
  nulldekl (lastunknowns);
  lastunknowns->ident = ident;
  lastunknowns->line = 0L;	/* HVA HER */
  lastunknowns->type = TERROR;
  lastunknowns->kind = KERROR;
  lastunknowns->categ = CNEW;
  lastunknowns->dim = 1;
  lastunknowns->encl = unknowns;
  lastunknowns->descr = unknowns;
  lastunknowns->virtno = unknowns->naloc = unknowns->naloc + 1;
  return (lastunknowns);
}

/******************************************************************************
                                                         FINDDECL            */

/* Finddecl leter etter deklarasjonen ident lokalt i blokken og langs
 *  den prefikskjede.Den kalles rekursivt for hvert BLOCK objekt langs 
 *  prefikskjeden.Ved en inspect blokk kalles den for den ispiserte 
 *  klassen og dens prefikser.Finnes den returneres en peker til 
 *  deklarasjonspakka, hvis ikke returneres NULL                         
 *  HVIS virt==TRUE skal det først letes i evt. virtuell liste */

struct DECL *
finddecl (ident, rb, virt)
     char *ident;
     struct BLOCK *rb;
     char virt;
{
  struct DECL *rd;
  if ((rb->quant.kind == KINSP) && (rb->parloc != NULL))
    {
      seenthrough = rb;
      if ((rd = finddecl (ident, rb->parloc->descr, virt)) != NULL
	  && rd->type != TLABEL)
	return (rd);
      seenthrough = NULL;
    }
  else
    {
      if (virt && rb->quant.kind == KCLASS)
	for (rd = rb->virt; rd != NULL; rd = rd->next)
	  if (rd->ident == ident && rd->protected == FALSE)
	    return (rd);

      for (rd = rb->parloc; rd != NULL; rd = rd->next)
	if (rd->ident == ident && rd->protected == FALSE)
	  return (rd);
    }
  /* G}r ogs} gjennom prefikskjeden */
  if (rb->quant.kind == KCLASS || rb->quant.kind == KINSP || rb->quant.kind == KPRBLK
      || rb->quant.kind == KFOR || rb->quant.kind == KCON)
    if (rb->quant.idplev.plev > -1 && rb->quant.prefqual != NULL)
      if ((rd = finddecl (ident, rb->quant.prefqual->descr,
			  rb->quant.kind == KCLASS | rb->quant.kind == KPRBLK ? FALSE : virt)) != NULL)
	return (rd);

  return (NULL);
}

/******************************************************************************
                                                                FINDGLOBAL   */

/* Findglobal  finner  den deklarasjonen som  svarer til et navn 
 * Den leter for  hvert  blokknivaa, i  prefikskjeden  og lokalt 
 * Stopper ved f\rste forekomst, fins den ikke kalles newnotseen 
 * Hvis virt==true skal det først letes i evt. virtuell liste */

struct DECL *
findglobal (ident, virt)
     char *ident;
     char virt;
{
  int i;
  struct DECL *rd;
  struct BLOCK *rb;

  seenthrough = NULL;
  for (rb = display[i = cblev]; i >= 0; rb = display[--i])
    if ((rd = finddecl (ident, rb, virt)) != NULL)
      {
	if (rd->encl->blev == cblock->blev &&
	    (rd->categ == CLOCAL || rd->categ == CVIRT))
	  localused = TRUE;
	return (rd);
      }

  for (rd = unknowns->parloc; rd != NULL; rd = rd->next)
    if (rd->ident == ident)
      return (rd);
  return (newnotseen (ident));
}

/******************************************************************************
                                  				SAMEPARAM    */

/* Sjekker om parameterene er de samme */

sameparam (rb1, rb2)
     struct BLOCK *rb1,
      *rb2;
{
  struct DECL *rd1,
   *rd2;
  int i;
  if (rb1 == NULL || rb2 == NULL)
    return (FALSE);
  if (rb1->quant.kind != KPROC || rb2->quant.kind != KPROC)
    return (FALSE);
  if (rb1->napar != rb2->napar)
    return (FALSE);
  rd1 = rb1->parloc;
  rd2 = rb2->parloc;
  for (i = 1; i <= rb1->napar; i++)
    {
      if (rd1->type != rd2->type && rd1->type != TNOTY)
	return (FALSE);
      if (rd1->kind != rd2->kind)
	return (FALSE);
      if (rd1->categ != rd2->categ)
	return (FALSE);

      if (rd1->prefqual != rd2->prefqual && rd1->type !=TNOTY)
	{
	  if (subclass (rd2->prefqual, rd1->prefqual))
	    {
	      if (rd1->categ == CNAME) return (FALSE);
	      if (rd1->kind == KPROC) return (FALSE);
	    } else
	  if (subclass (rd1->prefqual, rd2->prefqual)) ;
	    else return (FALSE);
	}
      if (rd1->kind == KPROC &&
	  sameparam (rd2->descr, rd1->descr) == FALSE)
	return (FALSE);
      rd1 = rd1->next;
      rd2 = rd2->next;
    }
  return (TRUE);
}

/******************************************************************************
                                                                MAKEEQUAL    */

/* Gjør rd1 lik rd2 ved å kopiere atributter */

makeequal (rd1, rd2)
     struct DECL *rd1,
      *rd2;
{
  rd1->ident = rd2->ident;
  rd1->line = rd2->line;
  rd1->idplev = rd2->idplev;
  rd1->dim = rd2->dim;
  rd1->virtno = rd2->virtno;
  rd1->type = rd2->type;
  rd1->kind = rd2->kind;
  rd1->categ = rd2->categ;
  rd1->encl = rd2->encl;
  rd1->descr = rd2->descr;
  rd1->match = rd2->match;
  rd1->next = NULL;
  rd1->prefqual = rd2->prefqual;
  rd1->protected = rd2->protected;
}

/******************************************************************************
                                                               COMMONQUAL    */
/* Finner felles kvalifikasjon for to klasser
 * NULL hviss ingen slik finnes */

struct DECL *
commonqual (rdx, rdy)
     struct DECL *rdx,
      *rdy;
{				/* Hvis rdx eller rdy peker på
				 * commonprefiks (som har plev=-1) s} vil 
				 * den leveres som felles kvalifikasjon, som
				 * er ønskelig i den situasjonen. Men hvis 
				 * ikke en av dem peker dit så vil IKKE
				 * commonprefiks være felles kvalifikasjon.
				 * Dette betyr at det ikke er nødvendig 
				 * med spesialbehandling for parametere til 
				 * call, resume. Hvis rdx eller rdy er lik 
				 * NULL, returneres den andre. */
  if (rdx == NULL) return (rdy);
  if (rdy == NULL) return (rdx);
  if (rdx == rdy) return (rdx);
  while (rdx != NULL && rdx->idplev.plev > rdy->idplev.plev)
    rdx = rdx->prefqual;
  if(rdx == NULL) return(rdy);
  while (rdy != NULL && rdy->idplev.plev > rdx->idplev.plev)
    rdy = rdy->prefqual;
  while (rdx != rdy && rdx!=NULL && rdy != NULL && rdx->idplev.plev > 0)
    {
      rdx = rdx->prefqual;
      rdy = rdy->prefqual;
    }
  return (rdx == rdy ? rdx : NULL);
}

/******************************************************************************
                                                            SUBCLASS         */

/* Er rdx en subklasse til rdy, returnerer TRUE eller FALSE */

char 
subclass (rdx, rdy)
     struct DECL *rdx,
      *rdy;
{
  if (rdx == rdy)
    return (TRUE);
  if (rdx == NULL || rdx == NULL) return(FALSE);
  if (rdx->idplev.plev < rdy->idplev.plev)
    return (0);
  while (rdx != NULL && rdx->idplev.plev > rdy->idplev.plev)
    rdx = rdx->prefqual;
  return (rdx == rdy);
}

/******************************************************************************
                                                                SUBORDINATE  */

char 
subordinate (rda, rdb)
     struct DECL *rda,
      *rdb;
{
  return ((rda->type != TREF && rda->type == rdb->type)
	  || rdb->type == TNOTY || (rda->type == TREF && rdb->type == TREF
			       && subclass (rda->prefqual, rdb->prefqual)));
}


/*****************************************************************************/
/*                                 INNLESING                                 */
/*****************************************************************************/


/******************************************************************************
                                                             NEWBLCK         */

/* Kalles fra  syntakssjekkeren hver gang en ny blokk entres */

newblck (kind)
     char kind;
{
  static int cblno = STARTBLNO;
  static struct BLOCK *last_block;
  struct DECL *rd1,
   *rd2;
  if (yaccerror)
    return;
  if (cblev>= 0) lastdecl[cblev] = clastdecl;
  if (++cblev > DECLTABSIZE)
    d1error (31);
#ifdef DEBUG
  if (option_input)
    printf (
	     "newblck---line:%ld cblev:%d type:%c kind:%c categ:%c\t"
	     ,yylineno, cblev, type, kind, categ);
#endif
  if (kind == KPROC || kind == KCLASS)
    {
      cblock = (struct BLOCK *) cprevdecl;
      cprevdecl->match = cprevdecl;
    }
  else
    {
      cblock = new (BLOCK);
      nullblock (cblock);
      cblock->quant.line = yylineno;
      cblock->quant.kind = kind;
    }
  display[cblev] = cblock;

  if (sblock == NULL)
    sblock = cblock;

  if (last_block != NULL)
    last_block = last_block->next_block = cblock;
  else
    last_block = cblock;

  cblock->blno = cblno++;
  switch (kind)
    {
    case KPROC:
    case KCLASS:
      crblev++;
      forn[++cdisbl] = 0;
      conn[cdisbl] = 0;
      if (cblev == 0)
	d1error (32);
      if (cprevdecl != NULL)
	{
	}
      if (staticblock && cblock->quant.categ == CLOCAL)
	cblock->stat = TRUE;
      break;
    case KFOR:
    case KINSP:
    case KCON:
      rd1 = &cblock->quant;
      rd1->ident = NULL;
      rd1->type = rd1->categ = 0;
      rd1->encl = NULL;
      rd1->descr = cblock;
      rd2 = &display[cblev - 1]->quant;

      rd1->prefqual = rd2;
      rd1->idplev.plev = rd2->idplev.plev + 1;
      if (rd2->kind != KCON && rd2->kind != KINSP &&
	  rd2->kind != KFOR)
	rd1->match = rd2;
      else
	rd1->match = rd2->match;
      if (kind == KFOR)
	{
	  if (rd1->match->descr->fornest < (cblock->fornest = ++forn[cdisbl]))
	    rd1->match->descr->fornest++;
	}
      else if (kind == KINSP)
	{
	  if (rd1->match->descr->connest < (cblock->connest = ++conn[cdisbl]))
	    rd1->match->descr->connest++;
	}
      else
	{
	  cblock->connest = conn[cdisbl];
	}
      if (staticblock)
	cblock->stat = TRUE;
      break;
    case KPRBLK:
      crblev++;
      forn[++cdisbl] = 0;
      conn[cdisbl] = 0;
      rd1 = &cblock->quant;
      rd1->type = rd1->categ = 0;
      rd1->ident = tag ("blokk", 5);
      rd1->encl = display[cblev - 1];
      rd1->descr = cblock;
      cblock->quant.idplev.identqual = prefquantident;
      if (staticblock)
	cblock->stat = TRUE;
      break;
    default:
      crblev++;
      forn[++cdisbl] = 0;
      conn[cdisbl] = 0;
      if (staticblock)
	cblock->stat = TRUE;
      break;
    }
  cblock->blev = crblev;
  clastdecl = lastdecl[cblev] = NULL;
  lastvirt[cblev] = NULL;
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                             ENDBLOCK        */

/* Kalles  fra  syntakssjekkeren hver gang en blokk terminerer */

/*VARARGS0 */
endblock (rtname, codeclass)
     char *rtname;
     char codeclass;
{
#ifdef DEBUG
  if (option_input)
    printf ("endblock---line:%ld type:%c kind:%c categ:%c\t"
	    ,yylineno, type, kind, categ);
#endif
  if (yaccerror)
    return;
  lastdecl[cblev--] = clastdecl;
  clastdecl = lastdecl[cblev];
  if (cblock->quant.kind == KFOR)
    forn[cdisbl]--;
  else if (cblock->quant.kind == KINSP)
    conn[cdisbl]--;
  else if (cblock->quant.kind == KCON) /* NOTHING */ ;
  else
    {
      if (cblock->quant.kind == KPROC && cblock->quant.categ == CCPROC
	  || insert_with_codeclass)
	{
	  struct EXINFO *exi;
	  exi = (exinfop) salloc (sizeof (struct EXINFO));
	  exi->rtname = rtname;
	  exi->codeclass = insert_with_codeclass ? codeclass : CCCPROC;
	  cblock->hiprot = (struct DECL *) exi;
	}
      crblev--;
      cdisbl--;
    }
  cblock = display[cblev];
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                             NEWDEKL         */

/* Newdekl kalles fra syntakssjekkeren
 * hver gang  vi leser  en deklarasjon */

newdekl (ident)
     char *ident;
{
  struct DECL *pd,
   *pdx = NULL;
#ifdef DEBUG
  if (option_input)
    printf ("newdekl---line:%ld navn:%s type:%c kind:%c categ:%c\t"
	    ,yylineno, ident, type, kind, categ);
#endif
  if (yaccerror)
    return;
  switch (categ)
    {
    case CLOCAL:
    case CDEFLT:
    case CCONSTU:
    case CCONST:
    case CEXTR:
    case CEXTRMAIN:
    case CCPROC:
      if (kind == KCLASS || kind == KPROC)
	{
	  pd = (struct DECL *) new (BLOCK);
	  nullblock ((struct BLOCK *) pd);
	}
      else
	{
	  pd = new (DECL);
	  nulldekl (pd);
	}
      if (categ != CDEFLT)
	cblock->naloc++;
      else
	cblock->napar++;
      if (clastdecl == NULL)
	cprevdecl = cblock->parloc = clastdecl = pd;
      else
	cprevdecl = clastdecl = clastdecl->next = pd;
      clastdecl->ident = ident;
      clastdecl->line = yylineno;
      clastdecl->type = type;
      clastdecl->kind = kind;
      clastdecl->categ = categ;
      clastdecl->encl = cblock;
      if ((type == TREF || kind == KCLASS)
	  && (categ == CLOCAL || categ == CEXTR		/* ||
							 * categ==CEXTRMAIN */
	      || categ == CCPROC || categ == CDEFLT))
	{
	  if (kind == KCLASS && cblock->quant.kind == KCLASS)
	    cblock->localclasses = TRUE;
	  clastdecl->idplev.identqual = prefquantident;
	}
      break;
    case CVALUE:
    case CNAME:
    case CVAR:			/* Denne er kun satt for eksterne moduler */
      for (pd = cblock->parloc; pd != NULL && pd->ident != ident; pd = pd->next);
      cprevdecl = pd;
      if (pd == NULL)
	d1error (34, ident);
      else
	{
	  if (pd->categ != CDEFLT)
	    d1error (35, ident);
	  pd->categ = categ;
	  if (categ == CNAME && nameasvar == ON)
	    pd->categ = CVAR;
	}
      break;
    case CSPEC:
      for (pd = cblock->parloc; pd != NULL && pd->ident != ident; pd = pd->next)
	pdx = pd;
      cprevdecl = pd;
      if (pd == NULL)
	d1error (34, ident);
      else
	{
	  if (pd->kind != KNOKD)
	    d1error (36, ident);
	  pd->type = type;
	  pd->kind = kind;
	  if (type == TREF)
	    {
	      pd->idplev.identqual = prefquantident;
	    }
	  if (kind == KPROC)
	    {
	      /* Bytter ut dette objektet med et st|rre */
	      cprevdecl = &new (BLOCK)->quant;
	      nullblock ((struct BLOCK *) cprevdecl);
	      if (clastdecl == pd)
		clastdecl = cprevdecl;
	      makeequal (cprevdecl, pd);
	      cprevdecl->descr = (struct BLOCK *) cprevdecl;
	      cprevdecl->next = pd->next;
	      if (pdx == NULL)
		cblock->parloc = cprevdecl;
	      else
		pdx->next = cprevdecl;
	    }
	}
      break;
    case CHIDEN:
    case CPROT:
    case CHIPRO:
      pd = cblock->hiprot;
      if (pd != NULL)
	while (pd->next != NULL && pd->ident != ident)
	  pd = pd->next;
      if (pd != NULL && pd->ident == ident)
	{
	  if (pd->categ != categ && pd->categ != CHIPRO
	      && categ != CHIPRO)
	    pd->categ = CHIPRO;
	  else
	    {
	      d1error (41, ident);
	      if (categ == CHIPRO)
		pd->categ = CHIPRO;
	    }
	  cprevdecl = pd;
	}
      else
	{
	  if (pd == NULL)
	    cblock->hiprot = pd = new (DECL);
	  else
	    pd = pd->next = new (DECL);
	  nulldekl (pd);
	  pd->ident = ident;
	  pd->line = yylineno;
	  pd->type = TNOTY;
	  pd->kind = KNOKD;
	  pd->categ = categ;
	  pd->encl = cblock;
	}
      break;
    case CVIRT:
      if (kind == KCLASS || kind == KPROC)
	{
	  pd = (struct DECL *) new (BLOCK);
	  nullblock ((struct BLOCK *) pd);
	}
      else
	{
	  pd = new (DECL);
	  nulldekl (pd);
	}
      if (lastvirt[cblev] == NULL)
	cblock->virt = pd = lastvirt[cblev] = pd;
      else
	pd = lastvirt[cblev] = lastvirt[cblev]->next = pd;
      cprevdecl = pd;
      pd->ident = ident;
      pd->line = yylineno;
      pd->type = type;
      pd->kind = kind;
      pd->categ = categ;
      pd->encl = cblock;
      if (type == TREF)
	{
	  pd->idplev.identqual = prefquantident;
	}
      break;
    default:
      d1error (37);
      break;
    }
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                       REGINNER              */

/* Kalles fra syntakssjekkeren hver gang  
 * inner oppdages, sjekker da lovligheten */

reginner ()
{
#ifdef DEBUG
  if (option_input)
    printf ("reginner---line:%ld cblev:%d\t"
	    ,yylineno, cblev);
#endif
  if (cblock->quant.kind != KCLASS)
    d1error (38);
  else
    {
      if (cblock->inner)
	d1error (39);
      else
	cblock->inner = TRUE;
    }
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                             REGPRBLOCK      */

/* Kalles for hver prefikset blokk som allokeres */

#if 0
regprblock (rdi)
     struct DECL *rdi;
{
  char error = FALSE;
  struct DECL *rd,
   *rdt;
  rd = rdi;
  do
    {
      if (rd->descr->thisused == TRUE)
	{
	  if (error == FALSE)
	    d2error (73, rdi);
	  error = TRUE;
	}
    }
  while (rd = (rdt = rd)->prefqual, rdt->idplev.plev > 0);
}
#endif

/*****************************************************************************/
/*                    SJEKKING AV DEKLARASJONER                              */
/*****************************************************************************/

/******************************************************************************
                                                                   DUMPDEKL  */

/* Dumpdekl skriver ut tilstanden til en deklarasjon
 * brukes ved uttesting av kompilatoren */

#ifdef DEBUG

dumpdekl (rd)
     struct DECL *rd;
{
  (void) printf ("        --DECL:%s=%d, k:%c,t:%c,c:%c, plev:%d, dim:%d, virtno:%d, line:%ld", rd->ident, rd->ident, rd->kind, rd->type, rd->categ, rd->idplev.plev, rd->dim, rd->virtno, rd->line);
  if (rd->protected == TRUE)
    printf (" PROTECTED");
  (void) printf ("\n");
  if (rd->descr != NULL)
    (void) printf ("              Blokk:(%d,%d)\n", rd->descr->blno, rd->descr->blev);

  if (rd->match != NULL && rd->categ == CVIRT)
    {
      if (rd->match != rd)
	{
	  if (rd->kind == KPROC)
	    (void) printf ("              match:Blokk(%d,%d)   navn:%s\n",
			   rd->match->descr->blno, rd->match->descr->blev,
			   rd->match->ident);
	  else
	    (void) printf ("               match:%s  i Blokk(%d,%d)\n",
	    rd->match->ident, rd->match->encl->blno, rd->match->encl->blev);
	}
      else
	(void) printf ("              match:INGEN MATCH\n");
    }
  else if (rd->match == NULL && rd->categ == CVIRT)
    (void) printf ("              match:INGEN MATCH\n");

  if (rd->prefqual != NULL && rd->type == TREF)
    (void) printf ("              kvalifikasjon:%s\n", rd->prefqual->ident);
  if (rd->descr != NULL && (rd->categ == CDEFLT || rd->categ == CVIRT))
    {
      if (rd->categ == CVIRT)
	{
	  (void) printf ("             Virtuell spec:\n");
	  (void) printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	  dumpblock (rd->descr);
	  (void) printf ("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	}
      else
	{
	  (void) printf ("             Formell procedure spec:\n");
	  (void) printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	  dumpblock (rd->descr);
	  (void) printf ("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	}
    }
}

/******************************************************************************
                                                               DUMPBLOCK     */

/* Dumpblock skriver ut  tilstanden til en blokk
 * Den gjør i sin tur en rekke kall paa dumpdekl */

dumpblock (rb)
     struct BLOCK *rb;
{
  struct DECL *rd;
  (void) printf 
    ("->BLOCK:(%d,%d)  k:%c, np:%d, nl:%d, nv:%d, nvl:%d, f:%d, c:%d, l:%ld, ",
     rb->blno, rb->blev, rb->quant.kind,
     rb->napar, rb->naloc, rb->navirt, rb->navirtlab, rb->fornest,
     rb->connest, rb->quant.line);
  if (rb->localclasses)
    printf ("l:YES ");
  else
    (void) printf ("l:NO ");
  if (rb->thisused)
    printf ("t:YES ");
  else
    (void) printf ("t:NO ");
  (void) printf ("\n");

  if (rb->quant.categ == CEXTR || rb->quant.categ == CEXTRMAIN)
    {
      if (rb->quant.kind == KCLASS)
	(void) printf ("     Extern klasse %s timestampandfilename:%s\n",
		       rb->quant.ident, rb->externid);
      else
	(void) printf ("     Extern prosedyre %s timestampandfilename:%s\n",
		       rb->quant.ident, rb->externid);
    }
  else if (rb->quant.categ == CCPROC)
    (void) printf ("     C-Prosedyre %s\n", ((exinfop) rb->hiprot)->rtname);

  if (rb->externid != 0)
    (void) printf ("     I extern modul : timestampandfilename:%s\n",
		   rb->externid);


  (void) printf ("     quant:%s plev:%d\n",
		 rb->quant.ident, rb->quant.idplev.plev);

  if (rb->quant.prefqual != NULL)
    {
      (void) printf ("     Prefikskjeden:\n");
      for (rd = rb->quant.prefqual; rd != NULL; rd = rd->prefqual)
	{
	  (void) printf ("                   Blokk:(%d,%d) ", rd->descr->blno,
			 rd->descr->blev);
	  if (rd->ident != 0)
	    printf (" navn : %s\n", rd->ident);
	  else
	    (void) printf ("\n");
	}

    }

  if (rb->parloc != NULL && rb->parloc->encl != rb)
    {

    }
  else if (rb->parloc != NULL)
    {
      (void) printf ("     Parametere:\n");
      for (rd = rb->parloc; rd != NULL &&
	   (rd->categ == CDEFLT || rd->categ == CVALUE ||
	    rd->categ == CNAME || rd->categ == CVAR);
	   rd = rd->next)
	dumpdekl (rd);
      (void) printf ("     Deklarasjoner:\n");
      for (; rd != NULL; rd = rd->next)
	dumpdekl (rd);

    }
  if (rb->virt != NULL && rb->virt->encl != rb)
    {
    }
  else if (rb->virt != NULL)
    {
      (void) printf ("     Virtuelle:\n");
      for (rd = rb->virt; rd != NULL; rd = rd->next)
	dumpdekl (rd);
    }
  if (rb->hiprot != NULL && rb->hiprot->encl != rb)
    {
    }
  else if (rb->hiprot != NULL)
    {
      (void) printf ("     Hidden/Protected:\n");
      for (rd = rb->hiprot; rd != NULL; rd = rd->next)
	dumpdekl (rd);
    }
  (void) printf ("\n");
}

/******************************************************************************
                                                                   DUMP      */

/* Dump skriver ut tilstanden til hele strukturen
 * Den gjør et kall på  dumpblock for hver blokk */

dump ()
{
  struct BLOCK *rb;
  (void) printf ("BLOKK:Blno,Blev,kind,napar,naloc,navirt,navirtlab,");
  (void) printf ("fornest,connest,line1,line2,localclasses,thisused\n\n");
  (void) printf ("DECL:navn,kind,type,categ,plev,dim,virtno,line\n\n");
  for (rb = sblock; rb != NULL; rb = rb->next_block)
    if (!(rb->quant.kind == KPROC && (rb->quant.categ == CDEFLT ||
				      rb->quant.categ == CVIRT)))
      dumpblock (rb);
  (void) printf ("---UNKNOWNS---\n");
  dumpblock (unknowns);
  (void) fflush (stdout);
}

#endif

/******************************************************************************
                                                             SETPROTECTED    */

/* Setter/fjerner protected merket når klasser entres/forlates */

setprotected (rb, protected)
     struct BLOCK *rb;
     char protected;
{
  struct BLOCK *rbx;
  struct DECL *rd;
  for (rd = rb->hiprot; rd != NULL; rd = rd->next)
    if (rd->match != NULL && rd->match->encl == rb)
      {
	if (rd->match->categ == CVIRT)
	  setprotectedvirt (rb, rd->match, protected);
	else
	  rd->match->protected = protected;
      }
  rbx = rb;
  while (rbx->quant.idplev.plev > 0)
    {
      rbx = rbx->quant.prefqual->descr;
      for (rd = rbx->hiprot; rd != NULL; rd = rd->next)
	if (rd->categ != CHIPRO && rd->match != NULL)
	  {
	    if (rd->match->categ == CVIRT)
	      setprotectedvirt (rb, rd->match, TRUE - rd->match->protected);
	    else
	      rd->match->protected = TRUE - rd->match->protected;
	  }
    }
}

setprotectedvirt (rb, rd, protected)
     struct BLOCK *rb;
     struct DECL *rd;
     char protected;
{
  struct BLOCK *rbx;
  struct DECL *rdx;
  rbx = rb;
  /* Den virtuelle listen for innerste prefiksniv} er ikke akkumulert opp */
  if (rb->navirt == 0 & rb->navirtlab == 0)
    goto neste;
  while (TRUE)
    {
      if (rd->kind == KPROC && rbx->navirt < rd->virtno)
	break;
      if (rd->kind != KPROC && rbx->navirtlab < rd->virtno)
	break;
      for (rdx = rbx->virt; rdx->virtno != rd->virtno |
	   rdx->kind != rd->kind; rdx = rdx->next);
      rdx->protected = protected;
      if (rdx->match != NULL)
	rdx->match->protected = protected;
    neste:if (rbx->quant.idplev.plev > 0)
	rbx = rbx->quant.prefqual->descr;
      else
	break;
    }
}

/******************************************************************************
                                                              SETPREFCHAIN   */

/* Setter opp prefikskjeden rekursift
 * Oppdager ulovlig prefiks og feil prefiksnivå 
 * Oppdager ved merking sirkulær prefikskjede */

setprefchain (rd)
     struct DECL *rd;
{
  struct DECL *rdx;
  if (rd->idplev.plev <= 0)
#if 0
  /* it's no substitute for a proper fix and obviously
     won't help on a bigendian 64 bit system anyway.*/
  if (rd->idplev.plev == -1 || rd->idplev.plev == 0) /* BOGUS!!!! */
#endif
    {
      if (rd->idplev.plev == 0)
	rd->prefqual = commonprefiks;
    }
  else
    {
      rdx = findglobal (rd->idplev.identqual, FALSE);
      rd->idplev.plev = 0;
      if (rdx->categ == CNEW)
	{
	  rdx->categ = CERROR;
	  d2error (50, rd, rdx);
	  rd->prefqual = commonprefiks;
	}
      else if (rdx->kind != KCLASS)
	{
	  if (rdx->categ != CERROR)
	    d2error (50, rd, rdx);
	  rdx->categ = CERROR;
	  rd->prefqual = commonprefiks;
	}
      else if (seenthrough != NULL)
	{
	  if (rdx->categ != CERROR)
	    d2error (49, rd, rdx);
	  rdx->categ = CERROR;
	  rd->prefqual = commonprefiks;
	}
      else if ((cblock->quant.kind == KFOR && rdx->encl != rd->encl)
	/* For for-block s} blir ikke blokkniv}et |ket. Prefiksen vil aldri
	 * v{re deklarert  i for-blokken (da ville det v{rt lagt p} en ekstra 
	 * blokk), den vil ligge i  prefiksen til for-blokken, og det er 
	 * ulovlig, da en for-blokk alltid skal opptre som om det er en blokk 
	 */
	       || (rdx->encl->blev != rd->encl->blev))
	{
	  if (rdx->categ != CERROR)
	    d2error (51, rd, rdx);
	  rdx->categ = CERROR;
	  rd->prefqual = commonprefiks;
	}
      else
	{
	  if (rdx->prefqual == NULL)
	    setprefchain (rdx);
	  if (rd->prefqual == commonprefiks)
	    d2error (52, rd, rdx);
	  else
	    {
	      rd->prefqual = rdx;
	      rd->idplev.plev = rdx->idplev.plev + 1;
	    }
	}
    }
}

/******************************************************************************
                                                        SETQUALPREFCHAIN     */

/* Setter opp prefikskjeden og kvalifikasjonen til pekere        
 * gjør kall på setprefchain og sjekker  kvalifikasjonen */

struct DECL *
setqualprefchain (rd, param)
     struct DECL *rd;
     int param;
{
  struct DECL *rdx;
  for (; rd != NULL; rd = rd->next)
    {
      if (param && (rd->categ == CLOCAL || rd->categ == CCONSTU
		    || rd->categ == CCPROC || rd->categ == CEXTR
		    || rd->categ == CEXTRMAIN))
	return (rd);
      if (rd->type == TREF)
	{
	  rdx = findglobal (rd->idplev.identqual, FALSE);
	  rd->idplev.plev = 0;
	  if (rdx->categ == CNEW)
	    {
	      d2error (53, rd);
	      rdx->categ = CERROR;
	    }
	  else if (rdx->kind != KCLASS)
	    {
	      if (rdx->categ != CERROR)
		d2error (54, rd);
	      rdx->categ = CERROR;
	      rd->type = TERROR;
	    }
	  rd->prefqual = rdx;
	}
      if (rd->kind == KCLASS && rd->prefqual == NULL)
	setprefchain (rd);
    }
  return (rd);
}

/******************************************************************************
                                                                SJEKKDEKL    */

/* Kalles i pass 2 for hver blokk som ikke er en prosedyre eller klasse 
 * Sjekkdekl tar seg av å sjekke og akumulere opp virtuelle 
 * Prefikskjeden og kvalifikasjoner settes ved kall på setqualprefchain 
 * den sjekker også konsistensen for type kind og categ */

/*VARARGS1 */
sjekkdekl (rb, rdi)
     struct BLOCK *rb;
     struct DECL *rdi;
{
  struct DECL *findlocal ();
  struct DECL *rd = NULL,
   *rdx = NULL,
   *rdy,
   *va = NULL,
   *vb = NULL,
   *vc = NULL;
  struct BLOCK *rbx = NULL;
  int vno,
    vnolab,
    kind;
  switch (kind = rb->quant.kind)
    {
    case KCLASS:
      /* Merker at denne klassen er blitt kalt */
      rb->quant.match = sjekkdeklcalled;
      /* Prefiksen maa først alokeres */
      if (rb->quant.idplev.plev > 0)
	{
	  rbx = rb->quant.prefqual->descr;
	  if (rbx->quant.match != sjekkdeklcalled)
	    {
	      cblock = display[cblev] = rbx;
	      sjekkdekl (rbx);
	      cblock = display[cblev] = rb;
	    }
	  rb->localclasses |= rbx->localclasses;
	  rb->napar += rbx->napar;
	  rb->naloc += rbx->naloc;
	}
      break;
    case KPRBLK:
      /* Må lete på nivået utenfor prefiksblokken */
      cblev--;
      setprefchain (&rb->quant);
      cblev++;
      if (cblev <= 2 /*|| display[cblev - 1]->stat*/)
	rb->stat = TRUE;
      break;
    case KINSP:
      if (rdi == NULL)
	{
	  d2error (73, &rb->quant);
	  rdi = findglobal (tag ("Noqual", 7), FALSE);
	  rdi->categ = CERROR;
	}
      rb->virt = rdi;
/*      if (rb->quant.prefqual->descr->stat)
	rb->stat = TRUE;*/
      return;			/* Sjekker blokken som inspiseres ved dens
				 * deklarasjon */
    case KFOR:
    case KCON:
/*      if (rb->quant.prefqual->descr->stat)
	rb->stat = TRUE;*/
      break;
    case KBLOKK:
      if (cblev <= 2 /*|| display[cblev - 1]->stat*/)
	rb->stat = TRUE;
      break;
    default:
      /* INGEN AKSJON */
      break;
    }
  /* Sjekker alle deklarasjonene til denne blokken */
  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    {
      /* Sjekker dobbeltdeklarasjoner */
      for (rdx = rb->parloc; rdx->ident != rd->ident
	   || rdx->protected == TRUE; rdx = rdx->next);
      if (rdx != rd)
	{
	  if (kind == KPROC && (rdx->categ == CDEFLT || rdx->categ == CVALUE ||
				rdx->categ == CNAME || rdx->categ == CVAR) &&
	      rd->categ != CDEFLT && rd->categ != CVALUE &&
	      rd->categ != CNAME && rd->categ != CVAR)
	    {
	      (void) strcpy (yytext, "__");
	      (void) strcat (yytext, rdx->ident);
	      rdx->ident = tag (yytext, strlen (yytext));
	    }
	  else
	    d2error (55, rd);
	}
      if (rd->kind == KNOKD && rd->type != TVARARGS)
	d2error (63, rd);
      if (rd->kind == KARRAY && rd->type == TNOTY)
	rd->type = TREAL;
      switch (rd->categ)
	{
	case CLOCAL:
	case CCONST:
	case CCONSTU:
	  /* Ikke mer sjekking lokale deklarasjoner */
	  break;
	case CDEFLT:
	  /* Procedyrer, label eller switch er ikke
	   * lovlig som parameter til klasser */
/*	  if (kind == KCLASS)
	    {
	      if (rd->kind == KPROC | rd->type == TLABEL)
		d2error (56, rd);
	    }*/
	  if (rd->type == TVARARGS)
	    {
	      if (rd->next != NULL)
		d2error (80, rd);
	      if (kind != KPROC || rb->quant.categ != CCPROC)
		d2error (81, rd);
	    }
	  break;
	case CVALUE:
	  /* Sjekker om lovlig valueoverføring */
	  if ((rd->type == TINTG | rd->type == TREAL | rd->type == TBOOL |
	   rd->type == TCHAR) && (rd->kind == KSIMPLE | rd->kind == KARRAY))
	    {
	      if (rd->kind == KSIMPLE)
		rd->categ = CDEFLT;
	    }
	  else if (rd->type == TTEXT & rd->kind == KSIMPLE) /* OK */ ;
	  else if (rd->type == TVARARGS)
	    {
	      if (rd->next != NULL)
		d2error (80, rd);
	      if (kind != KPROC || rb->quant.categ != CCPROC)
		d2error (81, rd);
	    }
	  else
	    d2error (57, rd);
	  break;
	case CNAME:
	case CVAR:
	  /* Nameparameter til klasser er ikke lovlig */
/*	  if (kind == KCLASS)
	    d2error (58, rd);*/
	  if (kind == KPROC && rb->quant.categ == CCPROC &&
	      (rd->type == TTEXT || rd->type == TREF))
	    d2error (77, rd);
	  if (rd->type == TVARARGS)
	    {
	      if (rd->next != NULL)
		d2error (80, rd);
	      if (kind != KPROC || rb->quant.categ != CCPROC)
		d2error (81, rd);
	    }
	  break;
	case CEXTR:
	case CEXTRMAIN:
	  break;
	case CCPROC:
	  if (rd->type == TREF)
	    d2error (78, rd);
	  break;
	default:
	  /* ULOVLIG CATEG */
	  d2error (59, rd);
	}
    }
  if (rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK)
    {
      setprotected (rb, FALSE);

      /* Kopierer opp de akumulerte virtuelle
       * Kjeder disse  sammen i en liste hvor
       * va peker paa første og  vb på siste */
      if (rb->quant.idplev.plev > 0)
	for (vc = rb->quant.prefqual->descr->virt;
	     vc != NULL; vc = vc->next)
	  {
	    if (va == NULL)
	      va = vb = new (DECL);
	    else
	      vb = vb->next = new (DECL);
	    makeequal (vb, vc);
	    vb->encl = rb;
	    vb->dim = 0;
	  }
      rdx = rb->virt;
      if (va != NULL || rb->virt != NULL)
	{
	  /* Hekter de akumulerte inn i listen av virtuelle
	   * slik at de blir liggende først i lista */
	  vc = rb->virt;
	  if (va != NULL)
	    {
	      vb->next = rb->virt;
	      rb->virt = va;
	      vno = rb->quant.prefqual->descr->navirt;
	      vnolab = rb->quant.prefqual->descr->navirtlab;
	    }
	  else
	    vno = vnolab = 0;

	  vb = vc;
	  for (; vc != NULL; vc = vc->next)
	    {
	      /* Sjekker dobbel spesifisering av de nye virtuelle */
	      for (va = rb->virt; va->ident != vc->ident ||
		   va->protected == TRUE; va = va->next);
	      if (va != vc)
		{
		  d2error (60, vc);
		  while (va->next != vc)
		    va = va->next;
		  va->next = vc->next;
		  vc = va;
		}
	      else
		{
		  /* Sjekker om det er lovlig virtuell */
		  if (vc->kind != KPROC && vc->type != TLABEL)
		    {
		      d2error (61, vc);
		      vc->type = TERROR;
		      vc->kind = KERROR;
		    }
		  if (vc->kind == KPROC)
		    vc->virtno = ++vno;
		  else
		    vc->virtno = ++vnolab;
		  vc->dim = 1;
		}
	    }
	  rb->navirt = vno;
	  rb->navirtlab = vnolab;
	}
      else
	rb->navirt = 0;
    }
  else
    rdx = NULL;

  /* Setter opp kvalifikasjoner og prefiks pekere */
  rd = rb->parloc;
  cblev--;
  if (rd != NULL)
    rd = setqualprefchain (rd, 1);	/* FOR PARAMETERE */
  cblev++;
  if (rdx != NULL)
    (void) setqualprefchain (rdx, 0);	/* FOR VIRTUELLE */
  if (rd != NULL)
    (void) setqualprefchain (rd, 0);	/* FOR LOKALE */

  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    /* Sjekker lokal klasse og prosedyre */
    if ((rd->kind == KCLASS && rd->match != sjekkdeklcalled)
     || (rd->kind == KPROC && (rd->categ == CLOCAL || rd->categ == CCPROC)))
      {
	cblock = display[++cblev] = rd->descr;
	sjekkdekl (rd->descr);
	cblev--;
      }
    else
      /* SJEKKER PROSEDYRE SOM ER OVERF\RT SOM PARAMETER */
    if (rd->kind == KPROC & rd->descr != NULL)
      {
	cblock = display[++cblev] = rd->descr;
	sjekkdekl (rd->descr);
	cblev--;
      }
  for (rd = vb; rd != NULL; rd = rd->next)
    /* Sjekker spesifikasjon av virtuell prosedyre */
    if (rd->kind == KPROC & rd->descr != NULL)
      {
	cblock = display[++cblev] = rd->descr;
	sjekkdekl (rd->descr);
	cblev--;
      }
  cblock = display[cblev];
  for (vc = rb->virt; vc != NULL; vc = vc->next)
    {
      if (vc->protected)
	continue;
      for (va = rb->parloc; va != NULL && va->ident != vc->ident; 
	   va = va->next);
      if (va != NULL)
	{
	  if ((vc->type == TERROR && (va->kind == KPROC || va->type == TLABEL))
	      || (vc->type == TLABEL && va->type == TLABEL 
		  && vc->kind == va->kind)
	  || (vc->kind == KPROC && va->kind == KPROC && subordinate (va, vc)
	      && sameparam (vc->descr, va->descr)))
	    {
	      vc->match = va;
	      vc->type = va->type;
	      vc->prefqual = va->prefqual;
	    }
	  else
	    d2error (62, va);
	}
      else if (vc->match == vc)
	vc->match = NULL;
    }
  if (rb->quant.kind == KCLASS || rb->quant.kind == KPRBLK)
    {
      /* Listen av hidden og protected sjekkes og match settes opp */
      for (rd = rb->hiprot; rd != NULL; rd = rd->next)
	{
	  rdx = findlocal (rd->ident, &rb->quant, TRUE);
	  if (rdx->categ == CNEW)
	    {
	      d2error (74, rd);
	      rdx->categ = CERROR;
	    }
	  else if (rd->categ != CHIDEN && rdx->encl != rb)
	    d2error (75, rd);
	  else if (rd->categ != CHIDEN && rdx->categ == CVIRT)
	    {
	      if (rb->quant.idplev.plev == 0)
		vno = 0;
	      else if (rdx->kind == KPROC)
		vno = rb->quant.prefqual->descr->navirt;
	      else
		vno = rb->quant.prefqual->descr->navirtlab;
	      if (rdx->virtno <= vno)
		d2error (75, rd);
	      else
		rd->match = rdx;
	    }
	  else if (rd->categ == CHIDEN && rdx->categ == CVIRT 
		   && rb->quant.idplev.plev > 0)
	    {
	      for (rdy = rb->quant.prefqual->descr->virt;
		   rdy->virtno != rdx->virtno || rdy->kind != rdx->kind;
		   rdy = rdy->next);
	      rd->match = rdy;
	    }
	  else
	    rd->match = rdx;
	}
      setprotected (rb, TRUE);
      /* Sjekk at de som er hidden også er protected */
      for (rd = rb->hiprot; rd != NULL; rd = rd->next)
	if (rd->categ == CHIDEN && rd->match != NULL &&
	    rd->match->protected == FALSE)
	  d2error (76, rd);
    }
}


/*****************************************************************************/
/*                         HJELP TIL SJEKKEREN                               */
/*****************************************************************************/

/******************************************************************************
  							      FIRSTCLASS     */

struct BLOCK *
firstclass ()
{				/* Retunerer med blev for den n{rmeste
				 * klassen eller prefiksblokk sett
				 * fra cblock */

  int i;
  struct BLOCK *rb;
  i = cblev;
  for (rb = cblock; rb->quant.kind != KCLASS && rb->quant.kind != KPRBLK; rb = display[--i])
    if ((rb->quant.kind == KFOR || rb->quant.kind == KINSP
	 || rb->quant.kind == KCON) &&
	rb->quant.match->kind == KCLASS)
      return (rb->quant.match->descr);
  return (rb);
}


/******************************************************************************
                                                                   INBLOCK   */

/* Inblock kalles fra sjekkeren hver  gang en  blokk  entres */
/*VARARGS0 */
struct BLOCK *
nextblock ()
{
  struct BLOCK *block;
  if (last_block == NULL)
    block = sblock;
  else
    block = last_block->next_block;

  while (block->quant.categ == CDEFLT /* formell proc.spec */  ||
	 block->quant.categ == CNAME /* formell proc.spec */  ||
	 block->quant.categ == CVAR /* formell proc.spec */  ||
	 block->quant.categ == CVIRT /* virtuell proc.spec */  ||
	 block->quant.categ == CCPROC ||
	 block->externid != 0)
    block = block->next_block;
  return (block);
}

inblock (rd)
     struct DECL *rd;
{
  cblock = last_block = nextblock ();
  cblev = cblock->blev;
  display[cblev] = cblock;
  if (cblock->quant.kind != KPROC && cblock->quant.kind != KCLASS)
    sjekkdekl (cblock, rd);
  cblock = display[cblev];
  if (cblock->quant.kind == KCON)
    cblock->quant.prefqual->descr->parloc = rd;
  if (cblock->quant.kind == KCLASS || cblock->quant.kind == KPRBLK)
    setprotected (cblock, FALSE);
}

/******************************************************************************
                                                             OUTBLOCK        */

/* Outblock kalles fra sjekkeren hver gang  en blokk  forlates */

outblock ()
{
  if (cblock->quant.kind == KCLASS || cblock->quant.kind == KPRBLK)
    setprotected (cblock, TRUE);
  if (cblock->quant.kind == KCON)
    cblock->quant.prefqual->descr->parloc = NULL;
  if (cblock->quant.kind == KFOR || cblock->quant.kind == KINSP 
      || cblock->quant.kind == KCON)
    cblock = display[cblev] = cblock->quant.prefqual->descr;
  else
    cblock = display[--cblev];
}

/******************************************************************************
                                                                REGTHIS      */

/* Kalles fra sjekkeren hver gang this oppdages,
 * sjekker da lovligheten */

struct DECL *
regthis (ident)
     char *ident;
{
  struct DECL *rd,
   *rdt,
   *rdx;
  int i;
#ifdef DEBUG
  if (option_input)
    printf ("regthis---line:%ld cblev:%d\t"
	    ,yylineno, cblev);
#endif
  for (i = cblev; i > 0; i--)	/* Skal det v}re i>=0 .(Omgivelsene) */
    {
      rd = &display[i]->quant;
      do
	{
	  rdx = rd;
	  if (rd->kind == KINSP)
	    {
	      seenthrough = rd->descr;
	      rd = rd->descr->parloc;
	    }
	  else
	    seenthrough = NULL;
	  if (rd->kind == KCLASS)
	    {
	      do
		if (rd->ident == ident)
		  {
		    if (rd->descr->thisused == MAYBEE)
		      d2error (72, rd);
		    rd->descr->thisused |= TRUE;
#ifdef DEBUG
		    if (option_input)
		      printf ("---end\n");
#endif
		    if (rd->descr->blev == cblock->blev)
		      localused = TRUE;
		    return (rd);
		  }
	      while (rd = (rdt = rd)->prefqual, rdt->idplev.plev > 0);
	    }
	  rd = rdx->prefqual;
	}
      while (rdx->kind == KCON || rdx->kind == KINSP || rdx->kind == KFOR);

    }
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
  d2error (79, rd = findglobal (ident, FALSE));
  return (rd);
}

/******************************************************************************
                                                                FINDLOCAL    */

/* Findlocal  finner  den  deklarasjonen som  svarer til  et navn 
 * Den leter lokalt i den lista den har fåt og dens prefikskjede 
 * Har den ikke  fåt noen liste  leter den slik  findglobal gjør 
 * Den registrerer også localused 
 * Hvis virt==TRUE skal det først letes i evt. virtuell liste */

struct DECL *
findlocal (ident, rd, virt)
     char *ident;
     struct DECL *rd;
     char virt;
{
  seenthrough = NULL;
  if (rd != NULL && rd->descr != NULL)
    rd = finddecl (ident, rd->descr, virt);
  else
    return (findglobal (ident, virt));
  if (rd != NULL)
    return (rd);
  for (rd = unknowns->parloc; rd != NULL; rd = rd->next)
    if (rd->ident == ident)
      return (rd);
  return (newnotseen (ident));
}

/******************************************************************************
                                                    NEXTPARAM & FIRSTPARAM   */

/* To prosedyrer for å finne parameterene      
 * til en prosedyre eller klasse 
 * Får som input forrige parameter */

struct DECL *
nextparam (rd)
     struct DECL *rd;
{
  struct DECL *rdx;
  int plev;
  if (rd == NULL)
    return (NULL);
  if (rd->type == TVARARGS)
    return (rd);
  if (rd->next != NULL)
    {
      rd = rd->next;
      if (rd == arrayparam && rd->dim != USPECDIM)
	rd->dim--;
      if (rd->categ == CDEFLT || rd->categ == CVALUE ||
	  rd->categ == CNAME || rd->categ == CVAR || rd->type == TVARARGS)
	return (rd);
    }
  if (rd->encl->quant.kind == KCLASS)
    {
      plev = rd->encl->quant.idplev.plev + 1;
      for (rd = prefdisplay[plev]; rd != NULL; rd = prefdisplay[++plev])
	if ((rdx = rd->descr->parloc) != NULL &&
	    (rdx->categ == CDEFLT || rdx->categ == CVALUE
	     || rdx->categ == CNAME || rdx->categ == CVAR))
	  return (rdx);
    }
  return (sluttparam);
}

struct DECL *
firstclassparam (rd)
     struct DECL *rd;
{
  struct DECL *rdx,
   *rdy;
  if (rd->idplev.plev > 0)
    {
      prefdisplay[rd->idplev.plev] = rd;
      rdy = firstclassparam (rd->prefqual);
    }
  else
    rdy = sluttparam;
  if (rdy == sluttparam)
    {
      if ((rdx = rd->descr->parloc) != NULL &&
	  (rdx->categ == CDEFLT || rdx->categ == CVALUE
	   || rdx->categ == CNAME || rdx->categ == CVAR))
	return (rdx);
    }
  return (rdy);
}



struct DECL *
firstparam (rd)
     struct DECL *rd;
{
  struct DECL *rdx;
  if (rd->kind == KCLASS)
    if (rd->idplev.plev + 1 >= DECLTABSIZE)
      d2error (64, rd);
    else
      {
	prefdisplay[rd->idplev.plev + 1] = NULL;
	return (firstclassparam (rd));
      }
  if (rd->kind == KARRAY)
    {
      if (rd->type == TLABEL)
	return (switchparam);
      if (rd->dim)
	arrayparam->dim = rd->dim;
      else
	arrayparam->dim = USPECDIM;
      return (arrayparam);
    }				
  /* else Kommentertut p.g.a full spesifisering 
   * av parametere til  formelle prosedyrer.
   * if(rd->kind==KPROC && rd->categ==CDEFLT) {
   * return(procparam); } */

  if (rd->descr == NULL)
    return (sluttparam);
  if ((rdx = rd->descr->parloc) != NULL &&
      (rdx->categ == CDEFLT || rdx->categ == CVALUE
       || rdx->categ == CNAME || rdx->categ == CVAR
       || rdx->type == TVARARGS))
    return (rdx);
  return (sluttparam);
}

/******************************************************************************
                                                                MOREPARAM    */

/* Forlanges det flere parametere */

moreparam (rd)
     struct DECL *rd;
{
  if (rd == sluttparam)
    return (FALSE);
  if (rd == switchparam->next)
    return (FALSE);
  if (rd == NULL)
    return (FALSE);
  if (rd->type == TVARARGS)
    return (MAYBEE);
  if (rd == arrayparam)
    {
      if (rd->dim == USPECDIM)
	return (MAYBEE);
      if (rd->dim > 0)
	return (TRUE);
      return (FALSE);
    }
  /* er kommenter ut siden formelle procedyrer er fullt ut spesifisert 
   * if(rd==procparam)return(MAYBEE); */
  return (TRUE);
}


/******************************************************************************
                                                                NOTBODY      */

/* Er vi inne i en prosedyre kropp */

body (rd)
     struct DECL *rd;
{
  int i;
  struct BLOCK *rb;
  i = cblev;
  rb = rd->descr;
  while (i > 0)
    {				/* Hvis vi er inne i en inspect blokk eller
				 * for blokk  */
      /* m} match f|lges for } f} riktig blokk. KAN BARE     */
      /* BRUKES FOR ] UNDERS\KE OM MAN ER INNE I EN PROSEDYRE */
      if (((display[i]->quant.kind == KCON || display[i]->quant.kind == KFOR) ?
	   display[i]->quant.match->descr : display[i]) == rb)
	return (TRUE);
      i--;
    }
  return (FALSE);
}

/******************************************************************************
                                        			DANGERPROC   */

/* Er prosedyren farlig og m] isoleres i uttrykk */

char 
dangerproc (rd)
     struct DECL *rd;
{
  exinfop p;
  if (rd->descr->hiprot == NULL)
    return (TRUE);
  p = (exinfop) rd->descr->hiprot;
  switch (p->codeclass)
    {
    case CCCPROC:
      return (rd->type == TTEXT);
    case CCFILEDANGER:
    case CCTEXTDANGER:
    case CCRANDOMRUTDANGER:
    case CCBLANKSCOPY:
    case CCFILEBLANKSCOPY:
    case CCSIMPLEDANGER:
      return (TRUE);
    }
  return (FALSE);
}
