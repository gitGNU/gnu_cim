/* Copyright (C) 1998 Sverre Hvammen Johansen
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
#include "gen.h"
#include "extspec.h"

static int dim;
/******************************************************************************
                                                                     SAVEPAR */

/* Denne rutinen saver verdien til en variabel som er brukt som parameter
 * til en av random drawing prosedyrene eller en c-prosedyre.
 * og som samtidig er flere ganger i uttrykket. Er den brukt flere ganger
 * saves den bare en gang */

static int stackno;

static struct EXP *
savepar (ret, re, up, ident, type, first, 
			    minval, minref, mintxt)
     struct EXP *ret,
       *re;
     char up;
     char *ident;
     char type;
     char first;
     int minval, minref, mintxt;
{
  struct EXP *rex;

  if (first) stackno=0;
  
  if (up)
    {
      rex = re->up;

      if (rex == ret)
	;
      else if ((rex->token == MASSIGN || rex->token == MASSIGNR ||
		rex->token == MVALASSIGNT || rex->token == MREFASSIGNT) &&
	       rex->right == re)
	;
      else if (rex->left == re)
	{
	  if (rex->right != NULL)
	    savepar (ret, rex->right, FALSE, ident, type, FALSE,
		     minval, minref, mintxt);
	  savepar (ret, rex, TRUE, ident, type, FALSE,
		   minval, minref, mintxt);
	} 
      else
	{
	  if (rex->left != NULL)
	    savepar (ret, rex->left, FALSE, ident, type, FALSE,
		     minval, minref, mintxt);
	  savepar (ret, rex, TRUE, ident, type, FALSE,
		   minval, minref, mintxt);
	}
    }
  else
    {
      if (re->token == MIDENTIFIER && re->rd->ident == ident)
 	{
	  if (re->up->token == MARGUMENTSEP &&
	      (re->up->rd->categ == CNAME ||
	       re->up->rd->categ == CVAR))
	    ;
	  else
	    {
	      struct EXP *restack1, *restack2, *reconc;
	      
	      if (stackno == 0)
		{
		  switch (type)
		    {
		    case TREF:
		      stackno= findallentry (ret, re, USEDREF, minref);
		      break;
		    case TTEXT:
		      stackno= findallentry (ret, re, USEDTXT, mintxt);
		      break;
		    default:
		      stackno= findallentry (ret, re, USEDVAL, minval);
		      break;
		    }
		  
		  restack1= makeexp (MSTACK, NULL, NULL);
		  if (re->up->left == re)
		    re->up->left= restack1;
		  else
		    re->up->right= restack1;
		  restack1->up= re->up;
		  
		  reconc= 
		    makeexp(rex->type==TTEXT? MREFASSIGNT:MASSIGN, 
			    restack2=makeexp (MSTACK, NULL,NULL), re);
		  
		  restack1->value.entry= restack2->value.ival= stackno;
		  restack1->type= restack2->type= type;
		  return reconc;
		}
	      else
		{
		  re->token = MSTACK;
		  re->value.entry= stackno;
		}
	    }
	}
      else
	{
	  if (re->right != NULL)
	    savepar (ret, re->right, FALSE, ident, type, FALSE,
		     minval, minref, mintxt);
	  if (re->left != NULL)
	    savepar (ret, re->left, FALSE, ident, type, FALSE,
		     minval, minref, mintxt);
	}
    }
  return NULL;
}

/******************************************************************************
                                                           FINDSUBENTRY      */

char usedentry[STACK_SIZE + 1];

static 
findsubentry (re)
     struct EXP *re;
{
  switch (re->token)
    {
    case MSTACK:
      switch (re->type)
	{
	case TREF:
	  usedentry[re->value.ival] |= USEDREF;
	  break;
	case TTEXT:
	  usedentry[re->value.ival] |= USEDTXT;
	  break;
	default:
	  usedentry[re->value.ival] |= USEDVAL;
	  break;
	}
      break;
    case MTEXTADR:
      usedentry[re->value.stack.txt_entry] |= USEDTXT;
    case MARRAYADR:
    case MNAMEADR:
      usedentry[re->value.stack.ref_entry] |= USEDREF;
      usedentry[re->value.stack.val_entry] |= USEDVAL;
      break;
    default:
      if (re->left != NULL)
	findsubentry (re->left);
      if (re->right != NULL)
	findsubentry (re->right);
    }
}

/******************************************************************************
                                                           FINDALLENTRY      */

int 
findallentry (ret, re, type, min)
     struct EXP *ret,
      *re;
     int type, min;
{
  int i,
    max = 0;
  struct EXP *rex;

  if (is_after_dot(re))
    re = re->up;
  rex = re;

  for (i = 1; i <= STACK_SIZE; i++)
    usedentry[i] = 0;
  while (rex != ret)
    {
      rex = re->up;
      while (rex != ret & (rex->left == re | rex->left == NULL 
			   | rex->token == MELSE))
	{
	  re = rex;
	  rex = rex->up;
	}
      if (rex == ret)
	break;
      findsubentry (rex->left);
      re = rex;
    }
  if (type & MAXUSED)
    {
      for (i = min; i <= STACK_SIZE; i++)
	if ((usedentry[i] & type) != 0)
	  max = i;
      return (max);
    }
  else
    for (i = min; i <= STACK_SIZE; i++)
      if ((usedentry[i] & type) == 0)
	{
#if ACSTACK_IN_OBJ
	  struct BLOCK *rb;
	  switch (cblock->quant.kind)
	    {
	    case KFOR:
	    case KINSP:
	    case KCON:
	      rb= cblock->quant.match->descr;
	      break;
	    default:
	      rb=cblock;
	    }
	  switch (type)
	    {
	    case USEDREF:
	      if (i > rb->maxusedref) rb->maxusedref= i;
	      break;
	    case USEDTXT:
	      if (i > rb->maxusedtxt) rb->maxusedtxt= i;
	      break;
	    case USEDVAL:
	      if (i > rb->maxusedval) rb->maxusedval= i;
	      break;
	    }
#endif	  
	  return (i);
	}
  gerror (87);
}

/******************************************************************************
                                                                 ANT_STACK   */

long 
ant_stack (ret, re, minval, minref, mintxt)
     struct EXP *ret,
       *re;
     int minval, minref, mintxt;
{
#if ACSTACK_IN_OBJ
  return (0);
#else
  return 
    (((long) findallentry (ret, re, USEDVAL | MAXUSED, minval)) << 16 
     | ((long) findallentry (ret, re, USEDREF | MAXUSED, minref)) << 8 
     | (findallentry (ret, re, USEDTXT | MAXUSED, mintxt)));
#endif
}

/******************************************************************************
                                                                    GENSTACK */

/* This routine traverse a expression tree upwards.
 * For every leftside */

static struct EXP * 
genstack (ret, re, only_pointers, minval, minref, mintxt)
     struct EXP *ret,
      *re;
     char only_pointers;
     int minval, minref, mintxt;
{
  int i;
  struct EXP *rex, *reconc=NULL;
  rex = re->up;
  while (rex != ret & rex->left == re)
    {
      re = rex;
      rex = rex->up;
    }
  if (rex == ret)
    return reconc;
  reconc= genstack (ret, rex, only_pointers, minval, minref, mintxt);

  if (rex->left == NULL) return reconc;

  /* Nå er alt ovenfor lagt på stakken, og det gjenstår bare å sjekke om
   * denne noden har en ventre-side med noe som må legges på stakken */

  if (rex->token == MPROCARG)
    return reconc;
  if (rex->token == MARRAYARG)
    return reconc;
  if (rex->token == MELSEE)
    return reconc;
  if (rex->token == MIDENTIFIER && rex->rd->categ == CNAME)
    return reconc;
  if (rex->token == MANDTHENE || rex->token == MORELSEE)
    return reconc;

  if (rex->token == MBOUNDSEP || rex->left->token == MDOT)
    rex = rex->left;
  else if (rex->token == MASSIGN || rex->token == MASSIGNR ||
	   rex->token == MREFASSIGNT)
    return reconc;

  switch (rex->left->token)
    {
    case MPROCARG:
      /* M} sjekke om prosedyren (som er en NOTDANGER prosedyre) har noen
       * parametere som ikke er konstante. Attributtet  konst == TRUE hvis
       * dens parametere best}r av konstanter og i s}fall trenger de ikke
       * saves noe. Hvis prosedyren er en av
       * text-prosedyrene, s} saves det ogs} (save - dvs. at prosedyren
       * evalueres og resultatet legges p} stakken. */
      if (!rex->left->konst ||
	  rex->left->rd->descr->codeclass == CCTEXT)
 	goto save;
      break;
    case MTEXTKONST:
    case MCHARACTERKONST:
    case MREALKONST:
    case MINTEGERKONST:
    case MBOOLEANKONST:
    case MNONE:
    case MSTACK:
    case MARRAYADR:
      return reconc;
      break;
    default:
    save:
      if (rex->left->type == TLABEL)
 	return reconc;
      { 
	int entry;
	int type= rex->left->type;
	struct EXP *restack;
	switch (rex->left->type)
	  {
	  case TREF:
	    entry= findallentry (ret, rex->left, USEDREF, minref);
	    break;
	  case TTEXT:
	    /* Sjekker om det er kall paa en av text-attributt prosedyrene
	     * Text-variabelen skal da IKKE saves. */
	    if (rex->token == MDOT)
	      return reconc;
	    if (only_pointers && rex->up->token != MVALASSIGNT)
	      return reconc;
	    entry= findallentry (ret, rex->left, USEDTXT, mintxt);
	    break;
	  default:
	    if (only_pointers)
	      return reconc;
	    entry= findallentry (ret, rex->left, USEDVAL, minval);
	    break;
	  }

	reconc= concexp (reconc, makeexp(rex->left->type==TTEXT? MREFASSIGNT:MASSIGN, 
					 restack=makeexp(MSTACK, NULL,NULL),
					 rex->left));
	rex->left= makeexp (MSTACK, NULL, NULL);
	rex->left->up= rex;
	restack->value.entry= rex->left->value.entry= entry;
	restack->type= rex->left->type= type;

	if (rex->token == MBOUNDPARSEP && rex->right != re && 	
	    !only_pointers)	/* sjekk at det er slik at rex->right!=NULL */
	  {
	    entry= findallentry (ret, rex->right, USEDVAL, minval);
	    reconc= concexp (reconc, makeexp(rex->left->type==TTEXT? MREFASSIGNT:MASSIGN,
					     restack=makeexp (MSTACK, 
							      NULL,NULL),
					     rex->right));
	    type= rex->type;

	    restack->value.entry= rex->right->value.entry= entry;
	    restack->type= rex->right->type= type;

	  }
      }
    }
  return reconc;
}

/******************************************************************************
                                                         WORKBEFORETEST      */

/* G}r gjennom subtreet og ser om det vil bli skrevet ut noen kode f|r      
 * genvalue() kalles. Brukes i forbindelse med if i uttrykk og i forbindelse
 * med ORELSE og ANDTHEN */

static char 
workbeforetest (re)
     struct EXP *re;
{
  int token;			/* token er deklarert som int fordi
				 * kompilatoren ga warning om at constant 136 
				 * is out of range of char comparison etter
				 * at MCONC ble lagt inn. Dette m} ses
				 * n{rmere p}.                      */
  token = re->token;
  if (token == MNEWARG || token == MARRAYARG ||
      token == MQUA || token == MQUANOTNONE || token == MQUANONEAND ||
      token == MCONC || (token == MIDENTIFIER && re->rd->categ == CNAME) ||
      (token == MPROCARG && re->danger))
    return (TRUE);

  if (re->left != NULL && workbeforetest (re->left))
    return (TRUE);
  if (re->right != NULL && workbeforetest (re->right))
    return (TRUE);
  return (FALSE);
}

/******************************************************************************
                                                                  TRANSPARAM */

static
transparam (ret, re, minval, minref, mintxt) 
     struct EXP *ret, *re;
     int minval, minref, mintxt;
{
  struct EXP *rex, *rexp, *rey;
  char index_is_const = TRUE;
  rexp=re;
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    {
      if (rex->rd->categ == CNAME)  
	{
	  if (rex->rd->kind == KSIMPLE)
	    {
	      if (rex->rd->type == TLABEL)
		{
		  if (rex->left->token != MIDENTIFIER)
		    /* Label overført by name
		     * ADDRESS_THUNK */
		    insert_thunk (rex, MTHUNKLABLE);
		}
	      else
		switch (rex->left->token)
		  {
		  case MTEXTKONST:
		  case MINTEGERKONST:
		  case MREALKONST:
		  case MCHARACTERKONST:
		  case MBOOLEANKONST:
		  case MNONE:
		  case MIDENTIFIER:
		    break;
		  case MARRAYARG:
		    for (re = rex->left->right; index_is_const && re->token != MENDSEP;
			 re = re->right)
		      if (re->left->token != MINTEGERKONST)
			index_is_const = FALSE;
		    
		    if (!index_is_const)
		      insert_thunk (rex,  MTHUNKSIMPLEADDRESS);
		    break;
		  case MDOT:
		    /* Dersom det er et dot'et prosedyre-kall, 
		     * s} skal  det genereres
		     * VALUE_THUNK og ikke ADDRESS_THUNK. */
		    if (rex->left->right->rd->kind != KPROC)
		      {
			insert_thunk (rex,  MTHUNKSIMPLEADDRESS);
			break;
		      }
		    else;    /* Denne grenen skal IKKE ha break, Skal gli
			      * rett over i neste case. */
		  default:
		    
		    insert_thunk (rex,  MTHUNKSIMPLEVALUE);
		  }
	    }
	  else if (rex->rd->kind == KARRAY && rex->rd->type != TLABEL && 
		   rex->left->token == MDOT)
	    {
	      /* ADDRESS_THUNK */
	      insert_thunk (rex,  MTHUNKARRAY);
	    }
	  else if (rex->rd->kind == KPROC && rex->left->token == MDOT)
	    {
	      /* ADDRESS_THUNK */
	      insert_thunk (rex, MTHUNKPROCEDURE);
	    }
	}
      else
	{
#if ACSTACK_IN_OBJ
	  rey= transcall (rex, rex->left,
			  findallentry (ret, re, USEDVAL | MAXUSED, minval)+1,
			  findallentry (ret, re, USEDREF | MAXUSED, minref)+1,
			  findallentry (ret, re, USEDTXT | MAXUSED, mintxt)+1);
#else
	  rey= transcall (rex, rex->left, 1, 1, 1);
#endif
	  if (rey!=NULL)
	    {
	      rexp->right=makeexp (MSENTCONC, rey, rex);
	      rexp->right->up= rexp;
	    }
	}
      rexp= rex;
    }
}


/******************************************************************************
                                                              TRANSCALL      */

struct EXP *
transcall (ret, re, minval, minref, mintxt)
     struct EXP *ret,
      *re;
     int minval, minref, mintxt;
{
  struct EXP *rex, *reconc=NULL;
  short entry;

  switch (re->token)
    {
    case MNEWARG:
      transparam (ret, re, minval, minref, mintxt);
      reconc= genstack (ret, re, FALSE, minval, minref, mintxt);
      re->value.n_of_stack_elements= ant_stack (ret, re,
						minval, minref, mintxt);
      reconc= concexp (reconc, replacenode (&re, MSTACK));

      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
		    makeexp (MEXITARGUMENT, NULL, NULL));
      rex->type= rex->left->type= rex->right->type= TREF;
      rex->left->value.entry= re->value.entry= 
	findallentry (ret, re, USEDREF, minref);
      reconc= concexp (reconc, rex);
      break;
    case MPROCARG:

      /* Ekstern C-prosedyre eller en av standard prosedyrene */
      switch (re->rd->descr->codeclass)
	{
	case CCNO:
	  if(is_after_dot(re))
	    reconc= concexp (reconc, genstack (ret, re->up, FALSE,
					       minval, minref, mintxt));
	  else
	    reconc= concexp (reconc, genstack (ret, re, FALSE,
					       minval, minref, mintxt));
	  transparam (ret, re, minval, minref, mintxt);
	  if (re->rd->categ == CNAME)
	    {
	      rex= copytree (re);
	      rex->value.n_of_stack_elements= 
		ant_stack (ret, re, minval, minref, mintxt);
	      rex->token= MNAMEREADACESS;
	      reconc= concexp (reconc, rex);
	    }
	  
	  switch (re->type)
	    {
	    case TREF:
	      entry= findallentry (ret, re, USEDREF, minref);
	      break;
	    case TTEXT:
	      entry= findallentry (ret, re, USEDTXT, mintxt);
	      break;
	    case TNOTY:
	      break;
	    default:
	      entry= findallentry (ret, re, USEDVAL, minval);
	      break;
	    }

	  re->value.n_of_stack_elements= 
	    ant_stack(ret, re, minval, minref, mintxt);

	  reconc= concexp (reconc, replacenode (&re, MSTACK));

	  if (re->type != TNOTY)
	    {
	      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
			    makeexp (MEXITARGUMENT, NULL, NULL));
	      rex->type= rex->left->type= rex->right->type= re->type;
	      rex->left->value.entry= re->value.entry= entry;
	      reconc= concexp (reconc, rex);
	    }
	  break;
	case CCCPROC:
	  /* Bare text-prosedyrer som er danger. Resten er  NOTDANGER.
	   * N}r det gjelder uttrykk som
	   * inneholder flere  C-prosedyrer s} garanterer ikke
	   * kompilatoren  at de blir utf\rt i riktig rekkef\lge. */
	  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	    {
	      /* Kaller savepar for alle formelle NAME eller VAR
	       * parametere. */
	      if ((rex->rd->categ == CNAME || rex->rd->categ == CVAR) &&
		  rex->left->token == MIDENTIFIER)
		reconc= concexp (reconc, savepar (ret, re, TRUE, 
						  rex->left->rd->ident,
						  rex->left->rd->type, TRUE,
						  minval, minref, mintxt));
	    }
	  for (rex = re->right; rex->token != MENDSEP; 
	       rex = rex->right)
	    reconc= concexp (reconc, transcall (ret, rex->left, 
						minval, minref, mintxt));
	  if (re->type == TTEXT)
	    {
	      entry= findallentry (ret, re, USEDTXT, mintxt);
	      re->value.n_of_stack_elements= 
		ant_stack (ret, re, minval, minref, mintxt);

	      reconc= concexp (reconc, replacenode (&re, MSTACK));
	      re->value.entry= entry;

	      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
			    makeexp (MEXITARGUMENT, NULL, NULL));
	      rex->type= rex->left->type= rex->right->type= re->type;
	      rex->left->value.entry= re->value.entry= entry;
	      reconc= concexp (reconc, rex);
	    }
	  break;
	case CCFILEDANGER:
	case CCSIMPLEDANGER:
	case CCTEXTDANGER:
	case CCRANDOMRUTDANGER:
	case CCBLANKSCOPY:
	case CCFILEBLANKSCOPY:
	  if (re->danger)
	    {
	      if (is_after_dot (re) && re->up->left->type == TTEXT)
		reconc= genstack (ret, re->up->left, FALSE,
				  minval, minref, mintxt);
	      else
		reconc= genstack (ret, re, FALSE,
				  minval, minref, mintxt);

	      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
		reconc= concexp (reconc, transcall (ret, rex->left,
						    minval, minref, mintxt));
      
	      if (re->rd->descr->codeclass == CCRANDOMRUTDANGER)
		{
		  /* Leter etter siste aktuelle parameter, som */
		  /* er en NAME parameter. Det skal sjekkes om */
		  /* denne variabelen er brukt flere ganger i uttrykket */
		  for (rex= re; rex->right->token != MENDSEP; rex= rex->right);
		  reconc= concexp (reconc, savepar (ret, re, 
						    rex->left->rd, TRUE,
						    minval, minref, mintxt));
		}

	      switch (re->type)
		{
		case TREF:
		  entry= findallentry (ret, re, USEDREF, minref);
		  break;
		case TTEXT:
		  entry= findallentry (ret, re, USEDTXT, mintxt);
		  break;
		case TNOTY:
		  break;
		default:
		  entry= findallentry (ret, re, USEDVAL, minval);
		  break;
		}
	      
	      re->value.n_of_stack_elements= 
		ant_stack (ret, re, minval, minref, mintxt);

	      if (re->type == TNOTY)
		{
		  reconc= concexp (reconc, replacenode (&re, MSTACK));
		  re->value.entry= entry;
		}
	      else
		{
		  rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
				replacenode (&re, MSTACK));
		  rex->type= rex->left->type= rex->right->type= re->type;
		  rex->left->value.entry= re->value.entry= entry;
		  reconc= concexp (reconc, rex);
		}
	      break;
	    }
	  /* Ingen break her */
	default:
	  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	    reconc= concexp (reconc,transcall (ret, rex->left,
					       minval, minref, mintxt));
	  break;
	}
      break;
    case MASSIGNR:
      if (re->danger)
	reconc= genstack (ret, re, TRUE, minval, minref, mintxt);
      reconc= concexp (reconc, transcall (ret, re->left,
					  minval, minref, mintxt));
      reconc= concexp (reconc, transcall (ret, re->right,
					  minval, minref, mintxt));
      if ((rex = re->left)->token == MNAMEADR && rex->type == TREF)
	{
	  reconc= concexp (reconc, makeexp(MINSTRONGEST,copytree(re->left), 
			    copytree(re->right)));
	}
      break;
    case MREFASSIGNT:
      if (re->danger)
	reconc= genstack (ret, re, TRUE, minval, minref, mintxt);
      reconc= concexp (reconc, transcall (ret, re->left,
					  minval, minref, mintxt));
      reconc= concexp (reconc, transcall (ret, re->right,
					  minval, minref, mintxt));
      break;
    case MARRAYARG:
      reconc= genstack (ret, re, FALSE, minval, minref, mintxt);
      if (re->type == TLABEL)
	{
	  reconc= concexp (reconc, transcall (ret, re->right->left,
					      minval, minref, mintxt));
	  break;
	}

      /* Dersom oversikt om hva som ligger på stacken håndteres anderledes
	 kan den følgende koden fjernes */
      re->left= makeexp (MARRAYADR, NULL, NULL);
      re->left->up = re;
      re->left->value.stack.ref_entry= re->value.stack.ref_entry= 
	findallentry (ret, re, USEDREF, minref);
      re->left->value.stack.val_entry= re->value.stack.val_entry= 
	findallentry (ret, re, USEDVAL, minval);
      if (re->rd->categ == CNAME)
	{
	  reconc= concexp (reconc, genstack (ret, re, FALSE,
					     minval, minref, mintxt));
	  rex= copytree (re);
	  rex->value.n_of_stack_elements= ant_stack (ret, re,
						     minval, minref, mintxt);
	  rex->token= MNAMEREADACESS;
	  reconc= concexp (reconc, rex);

	  rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
			makeexp (MEXITARGUMENT, NULL, NULL));
	  rex->type= rex->left->type= rex->right->type= TREF;
	  rex->left->value.entry= re->value.stack.ref_entry;
	  reconc= concexp (reconc, rex);
	}
      reconc= concexp (reconc, transcall (ret, re->right,
					  minval, minref, mintxt));
      re->left= NULL;

      reconc= concexp (reconc, replacenode (&re, MARRAYADR));
      break;
    case MIDENTIFIER:
      if (re->rd->categ == CNAME)
	{
	  reconc= concexp (reconc, genstack (ret, re, FALSE,
					     minval, minref, mintxt));
	  rex= copytree (re);
	  rex->value.n_of_stack_elements= ant_stack (ret, re,
						     minval, minref, mintxt);

	  if (((re->up->token == MASSIGN || re->up->token == MASSIGNR ||
		re->up->token == MVALASSIGNT || re->up->token == MREFASSIGNT)
	       && re->up->left == re)
	      || (re->up->token == MARGUMENTSEP &&
		  re->up->rd->categ == CVAR))
	    { /* SKRIVE AKSESS */

	      rex->token= MNAMEWRITEACESS;
	      reconc= concexp (reconc, rex);

	      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
			    makeexp (MEXITARGUMENT, NULL, NULL));
	      rex->type= rex->left->type= rex->right->type= TREF;
	      rex->left->value.entry= re->value.stack.ref_entry= 
		findallentry (ret, re, USEDREF, minref);
	      reconc= concexp (reconc, rex);

	      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
			    makeexp (MEXITARGUMENT, NULL, NULL));
	      rex->type= rex->left->type= rex->right->type= TINTG;
	      rex->left->value.entry= re->value.stack.val_entry= 
		findallentry (ret, re, USEDVAL, minval);
	      reconc= concexp (reconc, rex);

	      re->token = MNAMEADR;
	    } /* END-SKRIVEAKSESS NAME-PARAMETER */
	  else
	    
	    { /* LESE  AKSESS */

	      rex->token= MNAMEREADACESS;
	      reconc= concexp (reconc, rex);

	      if (re->rd->kind == KPROC || re->rd->kind == KARRAY ||
		  re->type == TLABEL);
	      else if (re->type == TTEXT)
		{
		  rex= copytree (re);
		  rex->value.stack.val_entry= re->value.stack.val_entry= 
		    findallentry (ret, re, USEDVAL, minval);
		  rex->value.stack.ref_entry= re->value.stack.ref_entry= 
		    findallentry (ret, re, USEDREF, minref);
		  rex->value.stack.txt_entry= re->value.stack.txt_entry= 
		    findallentry (ret, re, USEDTXT, mintxt);
		  rex->token= MNAMEREADTEXT;
		  reconc= concexp (reconc, rex);

		  re->token = MTEXTADR;
		}
	      else
		{
		  rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
				makeexp (MEXITARGUMENT, NULL, NULL));
		  rex->type= rex->left->type= rex->right->type= re->type;
		  rex->left->value.entry= re->value.entry= 
		    findallentry (ret, re, re->type == TREF?USEDREF:USEDVAL,
				  re->type == TREF?minref:minval);
		  reconc= concexp (reconc, rex);
		  re->token = MSTACK;
		}
	    } /* END-LESEAKSESS AV NAME-PAR */
	} /* END-NAME-PARAMETER */
      break;
    case MANDTHENE:
    case MORELSEE:
      reconc= transcall (ret, re->left, minval, minref, mintxt);

      if (workbeforetest (re->right))
	{
	  int i;
	  reconc= concexp (reconc, genstack (ret, re, FALSE,
					     minval, minref, mintxt));
	  rex= makeexp (re->token == MANDTHENE ? MANDTHEN : MORELSE, 
			copytree (re->left), transcall (ret, re->right,
							minval, minref, 
							mintxt));
	  rex->type= re->type;
	  reconc= concexp (reconc, rex);
	}
      break;
    case MIFE:
      reconc= transcall (ret, re->left, minval, minref, mintxt);
      if (workbeforetest (re->right))
	{
	  reconc= concexp (reconc, genstack (ret, re->right, FALSE,
					     minval, minref, mintxt));
	  rex= makeexp (MIF, re->left, transcall (ret, re->right, 
						  minval, minref, mintxt));
	  rex->type= re->type;
	  rex->qual= re->qual;
	  reconc= concexp (reconc, rex);
	}
      break;
    case MELSEE:
      rex= makeexp (MELSE, transcall (ret, re->left, minval, minref, mintxt), 
		    transcall (ret, re->right, minval, minref, mintxt));
      rex->type= re->type;
      rex->qual= re->qual;
      reconc= rex;
      break;
    case MCONC:
      reconc= transcall (ret, re->left, minval, minref, mintxt);
      reconc= concexp (reconc, transcall (ret, re->right,
					  minval, minref, mintxt));

      re->value.n_of_stack_elements= 
	ant_stack  (ret, re, minval, minref, mintxt);
      rex= newexp ();
      *rex= *re;
      rex->left->up= rex->right->up= rex;
      reconc= concexp (reconc, rex);

      rex= makeexp (MASSIGND, makeexp (MSTACK, NULL, NULL),
		    makeexp (MEXITARGUMENT, NULL, NULL));
      rex->type= rex->left->type= rex->right->type= TTEXT;
      rex->left->value.entry= re->value.entry= 
	findallentry (ret, re, USEDTXT, mintxt);
      reconc= concexp (reconc, rex);

      re->token = MSTACK;
      re->left = re->right = NULL;
      break;
    default:
      if (re->left != NULL)
	reconc= transcall (ret, re->left, minval, minref, mintxt);
      if (re->right != NULL)
	reconc= concexp (reconc, transcall (ret, re->right,
					    minval, minref, mintxt));
    }
  return reconc;
}
