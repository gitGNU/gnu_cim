/* $Id: cdekl.c,v 1.10 1997/01/08 09:49:13 cim Exp $ */

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

/* Deklarasjonslager for Simula */

#include "const.h"
#include "dekl.h"
#include "const.h"
#include "lex.h"
#include "name.h"
#include "salloc.h"
#include "cimcomp.h"

#include <stdio.h>
#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_pref;
static char *first_object_allocated_ptr_pref;


/*****************************************************************************/
/*                            INITIELT                                       */
/*****************************************************************************/



/* KONTAKT MED YACC, LEX OG FEILSYSTEMET */

/* Bruker bufferet til lex da det garantert er stort nokk
 * Dette blir brukt i det tilfellet at det m} lages en ny tag
 * for identen til en parameter som har navnkonflikt med en lokal variabel */

char yaccerror;

char *prefquantident;
int localused;
int arrdim;


block_t *ssblock; /* First system block 
                          (The outermost system block with blev=0) 
                          the system environment is conected to this block */

block_t *cblock; /* Current block */
block_t *sblock; /* First non system block 
                         (The outermost block with blev=1)
                         sblock is connected with ssblock through
                         two INSP blocks (sysin and sysout) */
static block_t *lblock;

static int cblno;


block_t *seenthrough;	/* Settes av find_global og find_local og peker 
				 * p}  en utenforliggende inspect blokk(hvis
				 * den      finnes). Det er fordi jeg onsker
				 * } vite n}r en variable er sett gjennom
				 * inspect. Trenger      denne informasjon i
				 * kode genereringen for }    aksessere
				 * variable fra den inspiserte klassen 
				 * gjennom inspect variabelen */
decl_t *classtext;


int cblev;

decl_t *cprevdecl;
 
/* Har en peker som peker p} en array deklarasjon som ikke har f}tt
 * satt sin dim verdi. */
decl_t *last_array;

/* Under sjekkingen og innlesingen av deklarasjonene
 * trenger jeg å merke de ulike objektene
 * Lar atributter peke paa ulike objekter for merkeingen */
static decl_t *sjekkdeklcalled;
static decl_t *lastunknowns;
static block_t *unknowns;
decl_t *commonprefiks;	/* Felles prefiks til alle ikke prefiksede
				 * klasser Inneholder prosedyren DETACH */
static decl_t *switchparam;
static decl_t *procparam;
static decl_t *sluttparam;
static decl_t *arrayparam;

/******************************************************************************
						      PCLEAN, PPUSH and PPOP */
static void ppush(decl_t *rd)
{
  obstack_grow (&os_pref, &rd, sizeof (void *));
}

static void pclean(void)
{
  void *p;
  p= obstack_finish (&os_pref);
  obstack_free (&os_pref, p);
}

static decl_t *ppop(void)
{
  decl_t *rd;
  if (obstack_next_free (&os_pref) == obstack_base (&os_pref))
    return (NULL);

  rd= * ((decl_t * *) obstack_next_free (&os_pref) - 1);
  obstack_blank (&os_pref, - sizeof (void *));
  return (rd);
}

/******************************************************************************
						        NEW-DECL/BLOCK       */


decl_t *new_decl(void)
{
  return (decl_t *) salloc (sizeof (decl_t));
}

static block_t *new_block(void)
{
  block_t *rb;
  rb= (block_t *) salloc (sizeof (block_t));
  rb->quant.descr = rb;
  return rb;
}

/******************************************************************************
                                                                  DECL_INIT  */

void decl_init (void)
{
  obstack_init(&os_pref);
  first_object_allocated_ptr_pref= obstack_alloc (&os_pref, 0);

  sjekkdeklcalled = new_decl ();
}


/******************************************************************************
                                                             DECL_INIT_PASS1 */

/* decl_init_pass1 kalles før selve innlesingen */

void decl_init_pass1 (void)
{
  block_t *rb;
  decl_t *rd;
  
  cblev= -1;
  unknowns = new_block ();
  unknowns->quant.kind = KERROR;

  cblno = STARTBLNO;
  lastunknowns= NULL;
  lblock= cblock= NULL;
  begin_block (KBLOKK);
  ssblock=cblock;

  /* ssblock->quant.encl= ssblock; Dersom denne er med går kompilatoren inn
     i en evig løkke dersom det er noe som er udeklarert. Er ikke sikker
     på om å bare kommentere det ut er riktig løsning */

  lesinn_external_spec (tag ("TEXTOBJ*"), "simenvir");

  commonprefiks = find_global (tag ("COMMON*"), TRUE);
  commonprefiks->plev = -1;
  classtext = find_global (tag ("TEXTOBJ*"), TRUE);

  begin_block (KINSP);
  begin_block (KINSP);
  rd = find_global (tag ("MAXLONGREAL"), TRUE);
  rd->value.rval = MAX_DOUBLE;
  rd = find_global (tag ("MINLONGREAL"), TRUE);
  rd->value.rval = -MAX_DOUBLE;
  rd = find_global (tag ("MAXREAL"), TRUE);
  rd->value.rval = MAX_DOUBLE;
  rd = find_global (tag ("MINREAL"), TRUE);
  rd->value.rval = -MAX_DOUBLE;
  rd = find_global (tag ("MAXRANK"), TRUE);
  rd->value.ival = MAXRANK;
  rd = find_global (tag ("MAXINT"), TRUE);
  rd->value.ival = MAX_INT;
  rd = find_global (tag ("MININT"), TRUE);
  rd->value.ival = -MAX_INT - 1;
}

/******************************************************************************
                                                                DECL_INIT_PASS2  */

/* decl_init_pass2 kalles før sjekkingen starter */

void decl_init_pass2 (void)
{
  decl_t *rd;
  end_block (NULL,CCNO);
  end_block (NULL,CCNO);
  end_block (NULL,CCNO);
  /* M} gj|re et hack for } f} satt kvalifikasjon p} inspect sysin */
  /* og inspect sysout, da neste blokk ikke er en connection blokk */
  lblock= NULL;
  in_block ();
  in_block ();
  cblock->when = find_global (tag ("INFILE"), TRUE);
  in_block ();
  cblock->when = find_global (tag ("PRINTFILE"), TRUE);
  sblock = cblock = cblock->next_block;

  switchparam = new_decl ();
  switchparam->type = TINTG;
  switchparam->kind = KSIMPLE;
  switchparam->categ = CDEFLT;
  switchparam->encl = unknowns;

  switchparam->next = new_decl ();
  switchparam->next->type = TINTG;
  switchparam->next->kind = KSIMPLE;
  switchparam->next->categ = CDEFLT;
  switchparam->next->encl = unknowns;
  switchparam->next->next = switchparam->next;

  procparam = new_decl ();
  procparam->type = TERROR;
  procparam->kind = TERROR;
  procparam->categ = CNAME;
  procparam->encl = unknowns;
  procparam->next = procparam;

  sluttparam = new_decl ();
  sluttparam->encl = unknowns;
  sluttparam->next = sluttparam;

  arrayparam = new_decl ();
  arrayparam->type = TINTG;
  arrayparam->kind = KSIMPLE;
  arrayparam->categ = CDEFLT;
  arrayparam->encl = unknowns;
  arrayparam->next = arrayparam;
}

/******************************************************************************
                                                                DECL_REINIT  */

/* Decl_reinit kalles før sjekkingen starter */

void decl_reinit (void)
{
   obstack_free (&os_pref, first_object_allocated_ptr_pref);
}

/*****************************************************************************/
/*                               HJELPE-PROSEDYRER                           */
/*****************************************************************************/

/******************************************************************************
                                                           SETARRAYDIM       */

/* Last_array peker på første array i siste arraydeklarasjon og set_array_dim
 * settes disse arrayenes dimensjon (dim). Så lengde next også er en array
 * skal også denne ha dimmensjonen arrdim.( integer array a,b(...); */

void set_array_dim (int arrdim)
{
  while (last_array != NULL)
    {
      last_array->dim = arrdim;
      last_array = (last_array->next == NULL ? NULL :
	      (last_array->next->kind == KARRAY ? last_array->next : NULL));
    }
  arrdim = 0;
}

/******************************************************************************
                                                               NEWNOTSEEN    */

/* Newnotseen kalles hver gang det er noe udeklarert
 * Den legger alle disse inn i en liste med de ukjente */

static decl_t *newnotseen (char *ident)
{
  if (lastunknowns == NULL)
    unknowns->parloc = lastunknowns = new_decl ();
  else
    lastunknowns = lastunknowns->next = new_decl ();
  lastunknowns->ident = ident;
  lastunknowns->type = TERROR;
  lastunknowns->kind = KERROR;
  lastunknowns->categ = CNEW;
  lastunknowns->dim = 1;
  lastunknowns->encl = unknowns;
  lastunknowns->descr = unknowns;
  return (lastunknowns);
}

/******************************************************************************
                                                         FINDDECL            */

/* Find_decl leter etter deklarasjonen ident lokalt i blokken og langs
 *  den prefikskjede.Den kalles rekursivt for hvert BLOCK objekt langs 
 *  prefikskjeden.Ved en inspect blokk kalles den for den ispiserte 
 *  klassen og dens prefikser.Finnes den returneres en peker til 
 *  deklarasjonspakka, hvis ikke returneres NULL                         
 *  HVIS virt==TRUE skal det først letes i evt. virtuell liste */

decl_t *find_decl (char *ident, block_t *rb, char virt)
{
  decl_t *rd;
  if ((rb->quant.kind == KINSP) && (rb->when != NULL))
    {
      seenthrough = rb;
      if ((rd = find_decl (ident, rb->when->descr, virt)) != NULL
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
  /* Går også gjennom prefikskjeden */
  if (rb->quant.kind == KCLASS || rb->quant.kind == KINSP || rb->quant.kind == KPRBLK
      || rb->quant.kind == KFOR || rb->quant.kind == KCON)
    if (rb->quant.plev > -1 && rb->quant.prefqual != NULL)
      if ((rd = find_decl (ident, rb->quant.prefqual->descr,
			  rb->quant.kind == KCLASS | rb->quant.kind == KPRBLK ? FALSE : virt)) != NULL)
	return (rd);

  return (NULL);
}

/******************************************************************************
                                                                FINDGLOBAL   */

/* Find_global  finner  den deklarasjonen som  svarer til et navn 
 * Den leter for  hvert  blokknivaa, i  prefikskjeden  og lokalt 
 * Stopper ved f\rste forekomst, fins den ikke kalles newnotseen 
 * Hvis virt==true skal det først letes i evt. virtuell liste */

decl_t *find_global (char *ident, char virt)
{
  decl_t *rd;
  block_t *rb;

  seenthrough = NULL;
  for (rb= cblock; rb; rb= rb->quant.encl)
    if ((rd= find_decl (ident, rb, virt)) != NULL)
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

int same_param (block_t *rb1, block_t *rb2)
{
  decl_t *rd1,
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
	  same_param (rd2->descr, rd1->descr) == FALSE)
	return (FALSE);
      rd1 = rd1->next;
      rd2 = rd2->next;
    }
  return (TRUE);
}

/******************************************************************************
                                                                MAKEEQUAL    */

/* Gjør rd1 lik rd2 ved å kopiere atributter */

static void makeequal (decl_t *rd1, decl_t *rd2)
{
  rd1->ident = rd2->ident;
  rd1->line = rd2->line;
  rd1->plev = rd2->plev;
  rd1->identqual = rd2->identqual;
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

decl_t *commonqual (decl_t *rdx, decl_t *rdy)
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
  while (rdx != NULL && rdx->plev > rdy->plev)
    rdx = rdx->prefqual;
  if(rdx == NULL) return(rdy);
  while (rdy != NULL && rdy->plev > rdx->plev)
    rdy = rdy->prefqual;
  while (rdx != rdy && rdx!=NULL && rdy != NULL && rdx->plev > 0)
    {
      rdx = rdx->prefqual;
      rdy = rdy->prefqual;
    }
  return (rdx == rdy ? rdx : NULL);
}

/******************************************************************************
                                                            SUBCLASS         */

/* Er rdx en subklasse til rdy, returnerer TRUE eller FALSE */

char subclass (decl_t *rdx, decl_t *rdy)
{
  if (rdx == rdy)
    return (TRUE);
  if (rdx == NULL || rdy == NULL) return(FALSE);
  if (rdx->plev < rdy->plev)
    return (0);
  while (rdx != NULL && rdx->plev > rdy->plev)
    rdx = rdx->prefqual;
  return (rdx == rdy);
}

/******************************************************************************
                                                                SUBORDINATE  */

char subordinate (decl_t *rda, decl_t*rdb)
{
  return ((rda->type != TREF && rda->type == rdb->type)
	  || rdb->type == TNOTY || (rda->type == TREF && rdb->type == TREF
			       && subclass (rda->prefqual, rdb->prefqual)));
}


/*****************************************************************************/
/*                                 INNLESING                                 */
/*****************************************************************************/


/******************************************************************************
                                                             BEGINBLOCK         */

/* Kalles fra  syntakssjekkeren hver gang en ny blokk entres */

void begin_block (char kind)
{
  decl_t *rd2;
  if (yaccerror)
    return;
#ifdef DEBUG
  if (option_input)
    printf ("begin_block---line:%ld kind:%c\t", lineno, kind);
#endif

  {
    block_t *lastcblock= cblock;

    if (kind == KPROC || kind == KCLASS)
      {
	cblock = (block_t *) cprevdecl;
	cprevdecl->match = cprevdecl;
      }
    else
      {
	cblock = new_block ();
	cblock->quant.line = lineno;
	cblock->quant.kind = kind;
#if 1
	if (lastcblock != NULL)
	  {
	    if (lastcblock->lastparloc == NULL)
	      cprevdecl= lastcblock->parloc=lastcblock->lastparloc= 
		&cblock->quant;
	    else
	      cprevdecl= lastcblock->lastparloc= 
		lastcblock->lastparloc->next= &cblock->quant;
	    cblock->quant.type= TNOTY;
	    cblock->quant.categ= CLOCAL;
	  }
#endif
      }
    cblock->quant.encl= lastcblock;
  }

  if (lblock != NULL)
    lblock = lblock->next_block = cblock;
  else
    lblock = cblock;

  cblock->blno = cblno++;
  switch (kind)
    {
    case KPROC:
    case KCLASS:
      cblev++;
      if (staticblock && cblock->quant.categ == CLOCAL)
	cblock->stat = TRUE;
      break;
    case KFOR:
    case KFORLIST:
    case KINSP:
    case KCON:
      cblock->quant.ident = NULL;
      /*      cblock->quant.encl = NULL;*/
      cblock->quant.descr = cblock;
      rd2 = &cblock->quant.encl->quant;

      cblock->quant.prefqual = rd2;
      cblock->quant.plev = rd2->plev + 1;
      if (rd2->kind != KCON && rd2->kind != KINSP &&
	  rd2->kind != KFOR)
	cblock->quant.match = rd2;
      else
	cblock->quant.match = rd2->match;

      switch (cblock->quant.prefqual->kind)
	{
	case KFOR:
	case KCON:
	case KINSP:
	  cblock->fornest= cblock->quant.prefqual->descr->fornest;
	  cblock->connest= cblock->quant.prefqual->descr->connest;
	  break;
	}
      switch (kind)
	{
	case KFORLIST:
	  cblock->fornest++;
	  if (cblock->quant.match->descr->fornest < cblock->fornest)
	    cblock->quant.match->descr->fornest++;
	  cblock->quant.kind = KFOR;
	  break;
	case KINSP:
	  cblock->connest+= 1;
	  if (cblock->quant.match->descr->connest < cblock->connest)
	    cblock->quant.match->descr->connest++;
	  break;
	}

      if (staticblock)
	cblock->stat = TRUE;
      break;
    case KPRBLK:
      cblev++;
      /*      cblock->quant.ident= tag ("blokk");*/
      cblock->quant.descr= cblock;
      cblock->quant.identqual= prefquantident;
      if (staticblock)
	cblock->stat= TRUE;
      break;
    default:
      cblev++;
      if (staticblock)
	cblock->stat = TRUE;
      break;
    }
  cblock->blev = cblev;
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                             ENDBLOCK        */

/* Kalles  fra  syntakssjekkeren hver gang en blokk terminerer */

/*VARARGS0 */
void end_block (char *rtname, char codeclass)
{
#ifdef DEBUG
  if (option_input)
    printf ("end_block---line:%ld\t", lineno);
#endif
  if (yaccerror)
    return;
  switch (cblock->quant.kind)
    {
    case KFOR:
    case KINSP:
    case KCON:
      break;
    default:
      if (codeclass)
	{
	  cblock->rtname = rtname;
	  cblock->codeclass = codeclass;
	}
      cblev--;
    }
  cblock = cblock->quant.encl;
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
}

/******************************************************************************
                                                             REGDECL         */

/* Reg_decl kalles fra syntakssjekkeren
 * hver gang  vi leser  en deklarasjon */

void reg_decl (char *ident, char type, char kind, char categ)
{
  decl_t *pd,
   *pdx = NULL;
#ifdef DEBUG
  if (option_input)
    printf ("reg_decl---line:%ld navn:%s type:%c kind:%c categ:%c\n"
	    ,lineno, ident, type, kind, categ);
#endif
  if (yaccerror)
    return;
  switch (categ)
    {
    case CVALUE:
    case CNAME:
    case CVAR:      /* Denne er kun satt for eksterne moduler */
      if (kind == KNOKD)
	{
	  for (pd = cblock->parloc; 
	       pd != NULL && pd->ident != ident; pd = pd->next);
	  if (pd != NULL || type != TVARARGS)
	    {
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
	    }
	}
    case CDEFLT:
      cblock->napar++;
    case CLOCAL:
    case CCONSTU:
    case CCONST:
    case CEXTR:
    case CEXTRMAIN:
    case CCPROC:
    proceed:
      if (kind == KCLASS || kind == KPROC)
	{
	  pd = (decl_t *) new_block ();
	}
      else
	{
	  pd = new_decl ();
	}
      if (cblock->lastparloc == NULL)
	cprevdecl = cblock->parloc = cblock->lastparloc = pd;
      else
	cprevdecl = cblock->lastparloc = cblock->lastparloc->next = pd;
      cblock->lastparloc->ident = ident;
      cblock->lastparloc->line = lineno;
      cblock->lastparloc->type = type;
      cblock->lastparloc->kind = kind;
      cblock->lastparloc->categ = categ;
      if (categ == CNAME && nameasvar == ON)
	cblock->lastparloc->categ = CVAR;
      cblock->lastparloc->encl = cblock;
      if ((type == TREF || kind == KCLASS)
	  && (categ == CLOCAL || categ == CEXTR		/* ||
							 * categ==CEXTRMAIN */
	      || categ == CCPROC || categ == CDEFLT
	      || (categ == CVALUE || categ == CNAME
		  || categ == CVAR) && kind != KNOKD))
	{
	  if (kind == KCLASS && cblock->quant.kind == KCLASS)
	    cblock->localclasses = TRUE;
	  cblock->lastparloc->identqual = prefquantident;
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
	      pd->identqual = prefquantident;
	    }
	  if (kind == KPROC)
	    {
	      /* Bytter ut dette objektet med et st|rre */
	      cprevdecl = &new_block ()->quant;
	      if (cblock->lastparloc == pd)
		cblock->lastparloc = cprevdecl;
	      makeequal (cprevdecl, pd);
	      cprevdecl->descr = (block_t *) cprevdecl;
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
	    cblock->hiprot = pd = new_decl ();
	  else
	    pd = pd->next = new_decl ();
	  pd->ident = ident;
	  pd->line = lineno;
	  pd->type = TNOTY;
	  pd->kind = KNOKD;
	  pd->categ = categ;
	  pd->encl = cblock;
	}
      break;
    case CVIRT:
      if (kind == KCLASS || kind == KPROC)
	{
	  pd = (decl_t *) new_block ();
	}
      else
	{
	  pd = new_decl ();
	}
      if (cblock->lastvirt == NULL)
	cblock->virt = pd = cblock->lastvirt= pd;
      else
	pd = cblock->lastvirt = cblock->lastvirt->next = pd;
      cprevdecl = pd;
      pd->ident = ident;
      pd->line = lineno;
      pd->type = type;
      pd->kind = kind;
      pd->categ = categ;
      pd->encl = cblock;
      if (type == TREF)
	{
	  pd->identqual = prefquantident;
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

void reg_inner (void)
{
#ifdef DEBUG
  if (option_input)
    printf ("reg_inner---line:%ld cblev:%d\t"
	    ,lineno, cblev);
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

/*****************************************************************************/
/*                    SJEKKING AV DEKLARASJONER                              */
/*****************************************************************************/

/******************************************************************************
                                                                   DUMPDEKL  */

/* Dumpdekl skriver ut tilstanden til en deklarasjon
 * brukes ved uttesting av kompilatoren */

#ifdef DEBUG

static 
dumpdekl (rd)
     decl_t *rd;
{
  printf ("        --DECL:%s=%d, k:%c,t:%c,c:%c, plev:%d, dim:%d, virtno:%d, line:%ld", rd->ident, rd->ident, rd->kind, rd->type, rd->categ, rd->plev, rd->dim, rd->virtno, rd->line);
  if (rd->protected == TRUE)
    printf (" PROTECTED");
  printf ("\n");
  if (rd->descr != NULL)
    printf ("              Blokk:(%d,%d)\n", rd->descr->blno, rd->descr->blev);

  if (rd->match != NULL && rd->categ == CVIRT)
    {
      if (rd->match != rd)
	{
	  if (rd->kind == KPROC)
	    printf ("              match:Blokk(%d,%d)   navn:%s\n",
			   rd->match->descr->blno, rd->match->descr->blev,
			   rd->match->ident);
	  else
	    printf ("               match:%s  i Blokk(%d,%d)\n",
	    rd->match->ident, rd->match->encl->blno, rd->match->encl->blev);
	}
      else
	printf ("              match:INGEN MATCH\n");
    }
  else if (rd->match == NULL && rd->categ == CVIRT)
    printf ("              match:INGEN MATCH\n");

  if (rd->prefqual != NULL && rd->type == TREF)
    printf ("              kvalifikasjon:%s\n", rd->prefqual->ident);
  if (rd->descr != NULL && (rd->categ == CDEFLT || rd->categ == CVIRT))
    {
      if (rd->categ == CVIRT)
	{
	  printf ("             Virtuell spec:\n");
	  printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	  dumpblock (rd->descr);
	  printf ("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	}
      else
	{
	  printf ("             Formell procedure spec:\n");
	  printf (">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	  dumpblock (rd->descr);
	  printf ("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	}
    }
}

/******************************************************************************
                                                               DUMPBLOCK     */

/* Dumpblock skriver ut  tilstanden til en blokk
 * Den gjør i sin tur en rekke kall paa dumpdekl */

dumpblock (rb)
     block_t *rb;
{
  decl_t *rd;
  printf 
    ("->BLOCK:(%d,%d)  k:%c, np:%d, nv:%d, nvl:%d, f:%d, c:%d, l:%ld, ",
     rb->blno, rb->blev, rb->quant.kind,
     rb->napar, rb->navirt, rb->navirtlab, rb->fornest,
     rb->connest, rb->quant.line);
  if (rb->localclasses)
    printf ("l:YES ");
  else
    printf ("l:NO ");
  if (rb->thisused)
    printf ("t:YES ");
  else
    printf ("t:NO ");
  printf ("\n");

  if (rb->quant.categ == CEXTR || rb->quant.categ == CEXTRMAIN)
    {
      if (rb->quant.kind == KCLASS)
	printf ("     Extern klasse %s timestampandfilename:%s %s\n",
		       rb->quant.ident, rb->timestamp, rb->filename);
      else
	printf ("     Extern prosedyre %s timestampandfilename:%s %s\n",
		       rb->quant.ident, rb->timestamp, rb->filename);
    }
  else if (rb->quant.categ == CCPROC)
    printf ("     C-Prosedyre %s\n", rb->rtname);

  if (rb->timestamp != 0)
    printf ("     I extern modul : timestampandfilename:%s %s\n",
		   rb->timestamp, rb->filename);


  printf ("     quant:%s plev:%d\n",
		 rb->quant.ident, rb->quant.plev);

  if (rb->quant.prefqual != NULL)
    {
      printf ("     Prefikskjeden:\n");
      for (rd = rb->quant.prefqual; rd != NULL; rd = rd->prefqual)
	{
	  printf ("                   Blokk:(%d,%d) ", rd->descr->blno,
			 rd->descr->blev);
	  if (rd->ident != 0)
	    printf (" navn : %s\n", rd->ident);
	  else
	    printf ("\n");
	}

    }

  if (rb->parloc != NULL && rb->parloc->encl != rb)
    {

    }
  else if (rb->parloc != NULL)
    {
      printf ("     Parametere:\n");
      for (rd = rb->parloc; rd != NULL &&
	   (rd->categ == CDEFLT || rd->categ == CVALUE ||
	    rd->categ == CNAME || rd->categ == CVAR);
	   rd = rd->next)
	dumpdekl (rd);
      printf ("     Deklarasjoner:\n");
      for (; rd != NULL; rd = rd->next)
	dumpdekl (rd);

    }
  if (rb->virt != NULL && rb->virt->encl != rb)
    {
    }
  else if (rb->virt != NULL)
    {
      printf ("     Virtuelle:\n");
      for (rd = rb->virt; rd != NULL; rd = rd->next)
	dumpdekl (rd);
    }
  if (rb->hiprot != NULL && rb->hiprot->encl != rb)
    {
    }
  else if (rb->hiprot != NULL)
    {
      printf ("     Hidden/Protected:\n");
      for (rd = rb->hiprot; rd != NULL; rd = rd->next)
	dumpdekl (rd);
    }
  printf ("\n");
}

/******************************************************************************
                                                                   DUMP      */

/* Dump skriver ut tilstanden til hele strukturen
 * Den gjør et kall på  dumpblock for hver blokk */

dump ()
{
  block_t *rb;
  printf ("BLOKK:Blno,Blev,kind,napar,navirt,navirtlab,");
  printf ("fornest,connest,line1,line2,localclasses,thisused\n\n");
  printf ("DECL:navn,kind,type,categ,plev,dim,virtno,line\n\n");
  for (rb = sblock; rb != NULL; rb = rb->next_block)
    if (!(rb->quant.kind == KPROC && (rb->quant.categ == CDEFLT ||
				      rb->quant.categ == CVIRT)))
      dumpblock (rb);
  printf ("---UNKNOWNS---\n");
  dumpblock (unknowns);
  fflush (stdout);
}

#endif

/******************************************************************************
                                                             SETPROTECTED    */

/* Setter/fjerner protected merket når klasser entres/forlates */

static setprotectedvirt (block_t *rb, decl_t *rd, char protected)
{
  block_t *rbx;
  decl_t *rdx;
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
    neste:if (rbx->quant.plev > 0)
	rbx = rbx->quant.prefqual->descr;
      else
	break;
    }
}

static void setprotected (block_t *rb, char protected)
{
  block_t *rbx;
  decl_t *rd;
  for (rd = rb->hiprot; rd != NULL; rd = rd->next)
    if (rd->match != NULL && rd->match->encl == rb)
      {
	if (rd->match->categ == CVIRT)
	  setprotectedvirt (rb, rd->match, protected);
	else
	  rd->match->protected = protected;
      }
  rbx = rb;
  while (rbx->quant.plev > 0)
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

/******************************************************************************
                                                              SETPREFCHAIN   */

/* Setter opp prefikskjeden rekursift
 * Oppdager ulovlig prefiks og feil prefiksnivå 
 * Oppdager ved merking sirkulær prefikskjede */

static void setprefchain (decl_t *rd)
{
  decl_t *rdx;
  if (rd->plev <= 0 && rd->identqual==NULL)
    {
      if (rd->plev == 0)
	rd->prefqual = commonprefiks;
    }
  else
    {
      rdx = find_global (rd->identqual, FALSE);
      rd->identqual=NULL;
      rd->plev = 0;
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
	      rd->plev = rdx->plev + 1;
	    }
	}
    }
}

/******************************************************************************
                                                        SETQUALPREFCHAIN     */

/* Setter opp prefikskjeden og kvalifikasjonen til pekere        
 * gjør kall på setprefchain og sjekker  kvalifikasjonen */

static decl_t *setqualprefchain (decl_t *rd, int param)
{
  decl_t *rdx;
  for (; rd != NULL; rd = rd->next)
    {
      if (param && (rd->categ == CLOCAL || rd->categ == CCONSTU
		    || rd->categ == CCPROC || rd->categ == CEXTR
		    || rd->categ == CEXTRMAIN))
	return (rd);
      if (rd->type == TREF)
	{
	  rdx = find_global (rd->identqual, FALSE);
	  rd->plev = 0;
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

static void sjekkdekl (block_t *rb)
{
  decl_t *rd = NULL,
   *rdx = NULL,
   *rdy,
   *va = NULL,
   *vb = NULL,
   *vc = NULL;
  block_t *rbx = NULL;
  int vno,
    vnolab,
    kind;
  switch (kind = rb->quant.kind)
    {
    case KCLASS:
      /* Merker at denne klassen er blitt kalt */
      rb->quant.match = sjekkdeklcalled;
      /* Prefiksen maa først alokeres */
      if (rb->quant.plev > 0)
	{
	  rbx = rb->quant.prefqual->descr;
	  if (rbx->quant.match != sjekkdeklcalled)
	    {
	      cblock = rbx;
	      sjekkdekl (rbx);
	      cblock = rb;
	    }
	  rb->localclasses |= rbx->localclasses;
	  rb->napar += rbx->napar;
	}
      break;
    case KPRBLK:
      /* Må lete på nivået utenfor prefiksblokken */
      cblev--;
      cblock= cblock->quant.encl;
      setprefchain (&rb->quant);
      cblev++;
      cblock= rb;
      if (cblev <= 2 /*|| display[cblev - 1]->stat*/)
	rb->stat = TRUE;
      break;
    case KINSP:
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
      if (rd->ident != NULL)
	{
	  /* Sjekker dobbeltdeklarasjoner */
	  for (rdx = rb->parloc; rdx->ident != rd->ident
		 || rdx->protected == TRUE; rdx = rdx->next);
	  if (rdx != rd)
	    {
	      if (kind == KPROC && (rdx->categ == CDEFLT || 
				    rdx->categ == CVALUE ||
				    rdx->categ == CNAME || 
				    rdx->categ == CVAR) &&
		  rd->categ != CDEFLT && rd->categ != CVALUE &&
		  rd->categ != CNAME && rd->categ != CVAR)
		{
		  char *s;
		  obstack_grow (&os_pref, "__", 2);
		  obstack_grow0 (&os_pref, rdx->ident, strlen(rdx->ident));
		  s= obstack_finish(&os_pref);
		  rdx->ident = tag (s);
		  obstack_free (&os_pref, s);
		}
	      else
		d2error (55, rd);
	    }
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
	  if (rd->type == TLABEL && rb->quant.categ == CCPROC)
	    d2error (82, rd);
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
	  if (rd->type == TLABEL && rb->quant.categ == CCPROC)
	    d2error (82, rd);
	  break;
	case CVAR:
	  if (rd->type == TREF && (rd->kind == KSIMPLE | rd->kind == KARRAY))
	    {
	      rd->categ = CDEFLT;
	    }
	case CNAME:
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
	  if (rd->type == TLABEL && rb->quant.categ == CCPROC)
	    d2error (82, rd);
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
      if (rb->quant.plev > 0)
	for (vc = rb->quant.prefqual->descr->virt;
	     vc != NULL; vc = vc->next)
	  {
	    if (va == NULL)
	      va = vb = new_decl ();
	    else
	      vb = vb->next = new_decl ();
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
    }
  else
    rdx = NULL;

  /* Setter opp kvalifikasjoner og prefiks pekere */
  rd = rb->parloc;
  cblev--;
  cblock= cblock->quant.encl;
  if (rd != NULL)
    rd = setqualprefchain (rd, 1);	/* FOR PARAMETERE */
  cblev++;
  cblock= rb;
  if (rdx != NULL)
    setqualprefchain (rdx, 0);	/* FOR VIRTUELLE */
  if (rd != NULL)
    setqualprefchain (rd, 0);	/* FOR LOKALE */

  cblev++;
  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    /* Sjekker lokal klasse og prosedyre */
    if ((rd->kind == KCLASS && rd->match != sjekkdeklcalled)
     || (rd->kind == KPROC && (rd->categ == CLOCAL || rd->categ == CCPROC)))
      {
	cblock = rd->descr; 
	sjekkdekl (rd->descr);
      }
    else
      /* SJEKKER PROSEDYRE SOM ER OVERF\RT SOM PARAMETER */
    if (rd->kind == KPROC & rd->descr != NULL)
      {
	cblock = rd->descr;
	sjekkdekl (rd->descr);
      }
  for (rd = vb; rd != NULL; rd = rd->next)
    /* Sjekker spesifikasjon av virtuell prosedyre */
    if (rd->kind == KPROC & rd->descr != NULL)
      {
	cblock = rd->descr;
	sjekkdekl (rd->descr);
      }
  cblev--;
  cblock = rb;
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
	      && same_param (vc->descr, va->descr)))
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
	  rdx = find_local (rd->ident, &rb->quant, TRUE);
	  if (rdx->categ == CNEW)
	    {
	      d2error (74, rd);
	      rdx->categ = CERROR;
	    }
	  else if (rd->categ != CHIDEN && rdx->encl != rb)
	    d2error (75, rd);
	  else if (rd->categ != CHIDEN && rdx->categ == CVIRT)
	    {
	      if (rb->quant.plev == 0)
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
		   && rb->quant.plev > 0)
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

block_t *firstclass (void)
{				/* Retunerer med blev for den n{rmeste
				 * klassen eller prefiksblokk sett
				 * fra cblock */

  int i;
  block_t *rb;
  i = cblev;
  for (rb = cblock; rb->quant.kind != KCLASS && rb->quant.kind != KPRBLK; rb = rb->quant.encl)
    if ((rb->quant.kind == KFOR || rb->quant.kind == KINSP
	 || rb->quant.kind == KCON) &&
	rb->quant.match->kind == KCLASS)
      return (rb->quant.match->descr);
  return (rb);
}


/******************************************************************************
                                                                   INBLOCK   */

/* In_block kalles fra sjekkeren hver  gang en  blokk  entres */
static void nextblock (void)
{
  if (lblock == NULL)
    lblock = ssblock;
  else
    lblock = lblock->next_block;

  while (lblock->quant.categ == CDEFLT /* formell proc.spec */  ||
	 lblock->quant.categ == CNAME /* formell proc.spec */  ||
	 lblock->quant.categ == CVAR /* formell proc.spec */  ||
	 lblock->quant.categ == CVIRT /* virtuell proc.spec */  ||
	 lblock->quant.categ == CCPROC ||
	 lblock->timestamp != 0)
    lblock = lblock->next_block;
  cblock= lblock;
}

void in_block (void)
{
  nextblock ();
  cblev = cblock->blev;
  if (cblock->quant.kind != KPROC && cblock->quant.kind != KCLASS)
    sjekkdekl (cblock);
  if (cblock->quant.kind == KCLASS || cblock->quant.kind == KPRBLK)
    setprotected (cblock, FALSE);
}

/******************************************************************************
                                                             OUTBLOCK        */

/* Out_block kalles fra sjekkeren hver gang  en blokk  forlates */

void out_block (void)
{
  if (cblock->quant.kind == KCLASS || cblock->quant.kind == KPRBLK)
    setprotected (cblock, TRUE);
  if (cblock->quant.kind == KCON)
    {
      cblock->quant.prefqual->descr->when = NULL;
    }
  if (cblock->quant.kind == KFOR || cblock->quant.kind == KINSP 
      || cblock->quant.kind == KCON)
    cblock = cblock->quant.prefqual->descr;
  else
    {
      cblev--;
      cblock = cblock->quant.encl;
    }
}

/******************************************************************************
                                                                REGWHEN      */


void regwhen (block_t *rb, decl_t *rd)
{
  rb->quant.prefqual->descr->when= rd;
}

/******************************************************************************
                                                                REGINSP      */


void reginsp (block_t *rb, decl_t *rd)
{
  if (rd == NULL)
    {
      d2error (73, &rb->quant);
      rd = find_global (tag ("Noqual"), FALSE);
      rd->categ = CERROR;
    }
  rb->virt = rd;
}

/******************************************************************************
                                                                REGTHIS      */

/* Kalles fra sjekkeren hver gang this oppdages,
 * sjekker da lovligheten */

decl_t *reg_this (char *ident)
{
  decl_t *rd,
   *rdt,
   *rdx;
  block_t *rb;
#ifdef DEBUG
  if (option_input)
    printf ("reg_this---line:%ld cblev:%d\t"
	    ,lineno, cblev);
#endif
  for (rb = cblock; rb->blev > 0; rb= rb->quant.encl)	/* Skal det v}re i>=0 .(Omgivelsene) */
    {
      rd = &rb->quant;
      do
	{
	  rdx = rd;
	  if (rd->kind == KINSP)
	    {
	      seenthrough = rd->descr;
	      rd = rd->descr->when;
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
	      while (rd = (rdt = rd)->prefqual, rdt->plev > 0);
	    }
	  rd = rdx->prefqual;
	}
      while (rdx->kind == KCON || rdx->kind == KINSP || rdx->kind == KFOR);

    }
#ifdef DEBUG
  if (option_input)
    printf ("---end\n");
#endif
  d2error (79, rd = find_global (ident, FALSE));
  return (rd);
}

/******************************************************************************
                                                                FINDLOCAL    */

/* Find_local  finner  den  deklarasjonen som  svarer til  et navn 
 * Den leter lokalt i den lista den har fåt og dens prefikskjede 
 * Har den ikke  fåt noen liste  leter den slik  find_global gjør 
 * Den registrerer også localused 
 * Hvis virt==TRUE skal det først letes i evt. virtuell liste */

decl_t *find_local (char *ident, decl_t *rd, char virt)
{
  seenthrough = NULL;
  if (rd != NULL && rd->descr != NULL)
    rd = find_decl (ident, rd->descr, virt);
  else
    return (find_global (ident, virt));
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

decl_t *next_param (decl_t *rd)
{
  decl_t *rdx;
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
      for (rd= ppop (); rd!= NULL; rd= ppop ())
	if ((rdx = rd->descr->parloc) != NULL &&
	    (rdx->categ == CDEFLT || rdx->categ == CVALUE
	     || rdx->categ == CNAME || rdx->categ == CVAR))
	  return (rdx);
    }
  return (sluttparam);
}

static decl_t *firstclassparam (decl_t *rd)
{
  decl_t *rdx,
   *rdy;
  if (rd->plev > 0)
    {
      ppush (rd);
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
      ppop ();
    }
  return (rdy);
}



decl_t *first_param (decl_t *rd)
{
  decl_t *rdx;
  if (rd->kind == KCLASS)
    {
      pclean ();
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

int more_param (decl_t *rd)
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

int body (decl_t *rd)
{
  block_t *rb, *rbx;
  rbx = cblock;
  rb = rd->descr;
  for (rbx= cblock; rbx->blev > 0; rbx= rbx->quant.encl)
    {				
      /* Hvis vi er inne i en inspect blokk eller for blokk  */
      /* m} match f|lges for } f} riktig blokk. KAN BARE     */
      /* BRUKES FOR ] UNDERS\KE OM MAN ER INNE I EN PROSEDYRE */
      if (rbx->quant.kind == KCON || rbx->quant.kind == KFOR)
	rbx= rbx->quant.match->descr;
      if (rbx == rb)
	return (TRUE);
    }
  return (FALSE);
}

/******************************************************************************
                                        			DANGERPROC   */

/* Er prosedyren farlig og m] isoleres i uttrykk */

char danger_proc (decl_t *rd)
{
  switch (rd->descr->codeclass)
    {
    case CCCPROC:
      return (rd->type == TTEXT);
    case CCFILEDANGER:
    case CCTEXTDANGER:
    case CCRANDOMRUTDANGER:
    case CCBLANKSCOPY:
    case CCFILEBLANKSCOPY:
    case CCSIMPLEDANGER:
    case CCNO:
      return (TRUE);
    }
  return (FALSE);
}

/*****************************************************************************
                                                                REMOVEBLOCK */

void remove_block (block_t *rb)
{
  decl_t *rd;
  if (rb->quant.encl->parloc->descr == rb) 
    rb->quant.encl->parloc= rb->quant.encl->parloc->next;
  else
    {
      for (rd= rb->quant.encl->parloc; rd->next->descr != rb; rd= rd->next);
      rd->next= rd->next->next;
    }
}
