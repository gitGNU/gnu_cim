/* $Id: cgenexp.c,v 1.24 1995/12/02 15:24:43 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen and Terje Mjøs,
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
#include "lex.h"
#include "extspec.h"

#if 0
#define ccode \
(fprintf(ccode,"/* %d */",__LINE__),ccode)
#endif

int stack;
int dim;
static int anttext;
int inthunk;			/* Brukes i forbindelse med uttrykk i
				 * thunker. Sier at statisk link (sl) m}
				 * f|lges en gang ekstra for      variable
				 * som aksesseres ifra thunken. */

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
      usedentry[(int) re->right] |= USEDTXT;
    case MARRAYADR:
    case MNAMEADR:
      usedentry[re->value.ival] |= USEDREF;
      usedentry[(int) re->left] |= USEDVAL;
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
findallentry (ret, re, type)
     struct EXP *ret,
      *re;
     int type;
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
      for (i = 1; i <= STACK_SIZE; i++)
	if ((usedentry[i] & type) != 0)
	  max = i;
      return (max);
    }
  else
    for (i = 1; i <= STACK_SIZE; i++)
      if ((usedentry[i] & type) == 0)
	return (i);
  gerror (87);
}

/******************************************************************************
                                                              GENCHAINRDENCL */

static xgenchainrdencl (re, atr)
     struct EXP *re;
     char atr;
{
  genchain (re->rd->encl, atr);
}

/******************************************************************************
                                                                GENSL        */

gensl (re, atr, nonetest)
     struct EXP *re;
     char atr,
       nonetest;
{
  if (is_after_dot (re))
    {
      if (atr)
	(void) fprintf (ccode, "((__bs%d *)", re->rd->encl->blno);
      if (nonetest == ON)
	(void) fprintf (ccode, "((__bp=");
      genvalue (re->up->left);
      if (nonetest == ON)
	(void) fprintf (ccode,
			")==__NULL?(__dhp)__rerror(__errnone):__bp)");
      if (atr)
	(void) fprintf (ccode, ")->");
    }
  else if (seen_th_insp (re))
    {
      if (atr)
	(void) fprintf (ccode, "((__bs%d *)", re->rd->encl->blno);
      genchain (re->seenthrough->quant.match->descr, TRUE);
      (void) fprintf (ccode, "c%d", re->seenthrough->connest);
      if (atr)
	(void) fprintf (ccode, ")->");
    }
  else
    {
      xgenchainrdencl (re, atr);
    }
}

/******************************************************************************
                                                               GENCHAIN     */

genchain (rb, atr)
     struct BLOCK *rb;
     char atr;
{
  int i;
  if (rb->stat)
    if (atr)
      (void) fprintf (ccode, "(__blokk%d%s).", rb->blno, 
		      rb->blno ? timestamp : "");
    else if (rb == sblock && separat_comp)
      (void) fprintf (ccode, "__NULL");
    else
      (void) fprintf (ccode, "((__dhp)&__blokk%d%s)", rb->blno, 
		      rb->blno ? timestamp : "");
  else
    {
      struct BLOCK *rbx;
      rbx = display[rb->blev];
      while (rbx->quant.kind == KFOR || rbx->quant.kind == KINSP
	     || rbx->quant.kind == KCON)
	rbx = rbx->quant.prefqual->descr;
      if (rbx->stat)
	{
	  if (atr)
	    {
	      (void) fprintf (ccode, "((__bs%d *)&__blokk%d%s)->"
			      ,rb->blno, rbx->blno, timestamp);
	    }
	  else
	    (void) fprintf (ccode, "((__dhp)&__blokk%d%s)", 
			    rbx->blno, timestamp);
	}
      else
	{
	  if (atr)
	    (void) fprintf (ccode, "((__bs%d *)__lb", rb->blno);
	  else
	    (void) fprintf (ccode, "__lb", rb->blno);
	  for (i = cblev + (inthunk ? 1 : 0); i > rb->blev; i--)
	    (void) fprintf (ccode, "->sl");
	  if (atr)
	    (void) fprintf (ccode, ")->", rb->blno);
	}
    }
}

/******************************************************************************
                                                                GENTYPE      */

gentype (re)
     struct EXP *re;
{
  switch (re->type)
    {
    case TINTG:
      (void) fprintf (ccode, "long");
      break;
    case TREAL:
      (void) fprintf (ccode, "double");
      break;
    case TBOOL:
    case TCHAR:
      (void) fprintf (ccode, "char");
      break;
    case TLABEL:
      (void) fprintf (ccode, "__lab");
      break;
    case TTEXT:
      (void) fprintf (ccode, "__txt");
      break;
    case TREF:
      (void) fprintf (ccode, "__dhp");
      break;
    };

}

/******************************************************************************
                                                            GEN_ADR_PROT     */

gen_adr_prot (rd)
     struct DECL *rd;
{
  (void) fprintf (ccode, "&__p%d%s"
	     ,rd->descr->externid == 0 ? rd->descr->blno : rd->descr->ptypno
		  ,rd->descr->externid == 0 ?
	    ((rd->encl != NULL && rd->encl->blev == SYSTEMGLOBALBLEV) ? "" :
	     timestamp) : extract_timestamp (rd->descr->externid));
}

/******************************************************************************
                                                             GEN_ANT_STACK   */

gen_ant_stack (ret, re)
     struct EXP *ret,
       *re;
{
  fprintf (ccode, "%ldL", 
	   ((long) findallentry (ret, re, USEDVAL | MAXUSED)) << 16 
	   | ((long) findallentry (ret, re, USEDREF | MAXUSED)) << 8 
	   | (findallentry (ret, re, USEDTXT | MAXUSED)));
}

/******************************************************************************
                                                                  REMOVE_DOT */

remove_dot (rep)
     struct EXP **rep;
{
  if (is_after_dot ((*rep)))
    {
      *rep = (*rep)->up;
      (*rep)->value = (*rep)->right->value;
      (*rep)->token = (*rep)->right->token;
      (*rep)->left = (*rep)->right = NULL;
    }
}



/******************************************************************************
                                                                         GEN */

gen (re)
     struct EXP *re;
{
  skrivenddecl ();

  gencall (re->up, re);
  genvalue (re);
  (void) fprintf (ccode, ";");
}

/******************************************************************************
                                                                    GENSTACK */

/* This routine traverse a expression tree upwards.
 * For every leftside */

static genstack (ret, re, only_pointers)
     struct EXP *ret,
      *re;
     char only_pointers;
{
  int i;
  struct EXP *rex;
  rex = re->up;
  while (rex != ret & rex->left == re)
    {
      re = rex;
      rex = rex->up;
    }
  if (rex == ret)
    return;
  genstack (ret, rex, only_pointers);

  /* Nå er alt ovenfor lagt på stakken, og det gjenstår bare å sjekke om
   * denne noden har en ventre-side med noe som må legges på stakken */

  if (rex->token == MPROCARG)
    return;
  if (rex->token == MARRAYARG)
    return;
  if (rex->token == MELSEE)
    return;
  if (rex->token == MIDENTIFIER && rex->rd->categ == CNAME)
    return;
  if (rex->token == MANDTHEN || rex->token == MORELSE)
    return;

  if (rex->token == MBOUNDSEP || rex->left->token == MDOT)
    rex = rex->left;
  else if (rex->token == MASSIGN || rex->token == MASSIGNR ||
	   rex->token == MREFASSIGNT)
    return;

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
	  ((exinfop) rex->left->rd->descr->hiprot)->codeclass == CCTEXT)
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
      return;
      break;
    default:
    save:
      if (rex->left->type == TLABEL)
 	return;
      switch (rex->left->type)
 	{
	case TREF:
	  (void) fprintf (ccode, "__r[%d]=(__dhp)"
			  , i = findallentry (ret, rex->left, USEDREF));
	  break;
 	case TREAL:
	  if (only_pointers)
	    return;
	  (void) fprintf (ccode, "__v[%d].f="
			  , i = findallentry (ret, rex->left, USEDVAL));
	  break;
 	case TINTG:
	  if (only_pointers)
	    return; 	
	  (void) fprintf (ccode, "__v[%d].i="
			  , i = findallentry (ret, rex->left, USEDVAL));
	  break;
 	case TTEXT:
	  /* Sjekker om det er kall paa en av text-attributt prosedyrene
	   * Text-variabelen skal da IKKE saves. */
	  if (rex->token == MDOT)
	    return;
	  if (only_pointers && rex->up->token != MVALASSIGNT)
	    return;
	  (void) fprintf (ccode, "__t[%d]= *"
			  , i = findallentry (ret, rex->left, USEDTXT));
	  break;
 	default:
	  if (only_pointers)
	    return; 	
	  (void) fprintf (ccode, "__v[%d].c="
			  , i = findallentry (ret, rex->left, USEDVAL));
 	}
      genvalue (rex->left);
      rex->left->value.ival = i;
      (void) fprintf (ccode, ";");
      rex->left->token = MSTACK;
      rex->left->left = NULL;
      rex->left->right = NULL;
      rex->left->rd = NULL;
      if (rex->token == MBOUNDPARSEP && rex->right != re && 	
	  !only_pointers)	/* sjekk at det er slik at rex->right!=NULL */
 	{
	  (void) fprintf (ccode, "__v[%d].i=",
			  i = findallentry (ret, rex->right, USEDVAL));
	  genvalue (rex->right);
	  rex->right->value.ival = i;
	  (void) fprintf (ccode, ";");
	  rex->right->token = MSTACK;
	  rex->right->left = NULL;
	  rex->right->right = NULL;
	  rex->right->rd = NULL;
	}
    }
}

/******************************************************************************
                                                                     SavePar */

/* Denne rutinen saver verdien til en variabel som er brukt som parameter
 * til en av random drawing prosedyrene eller en c-prosedyre.
 * og som samtidig er flere ganger i uttrykket. Er den brukt flere ganger
 * saves den bare en gang */

short savepar (ret, re, up, ident, type, stackno)
     struct EXP *ret,
       *re;
     char up;
     char *ident;
     char type;
     int stackno;
{
  struct EXP *rex;
  if (up)
    {
      rex = re->up;

      if (rex == ret)
 	return;
      if ((rex->token == MASSIGN || rex->token == MASSIGNR ||
	   rex->token == MVALASSIGNT || rex->token == MREFASSIGNT) &&
	  rex->right == re)
 	return;

      if (rex->left == re)
 	{
	  if (rex->right != NULL && rex->token != MTEXTADR)
	    stackno = savepar (ret, rex->right, FALSE, ident, type, stackno);
	  (void) savepar (ret, rex, TRUE, ident, type, stackno);
 	} 
      else
	{
	  if (rex->left != NULL && rex->token != MARRAYADR
	      && rex->token != MNAMEADR)
	    stackno = savepar (ret, rex->left, FALSE, ident, type, stackno);
	  (void) savepar (ret, rex, TRUE, ident, type, stackno);
 	}
    }
  else
    {
      if (re->token == MIDENTIFIER && re->rd->ident == ident)
 	{
	  if (re->up->token == MARGUMENTSEP &&
	      (re->up->rd->categ == CNAME ||
	       re->up->rd->categ == CVAR))
	    return (stackno);
	  if (stackno == 0)
	    {
	      switch (type)
 		{
 		case TREF:
		  (void) fprintf (ccode, "__r[%d]=",
				  stackno = findallentry (ret, re, USEDREF));
		  break;
		case TTEXT:
		  (void) fprintf (ccode, "__t[%d]= *", 
				  stackno = findallentry (ret, re, USEDTXT));
		  break;
		case TINTG:
		  (void) fprintf (ccode, "__v[%d].i=",
				  stackno = findallentry (ret, re, USEDVAL));
		  break;
		case TREAL:
		  (void) fprintf (ccode, "__v[%d].f=",
				  stackno = findallentry (ret, re, USEDVAL));
		  break;
		default:
		  (void) fprintf (ccode, "__v[%d].c=", 
				  stackno = findallentry (ret, re, USEDVAL));
		  break;
		}
	      genvalue (re); 	
	      re->token = MSTACK;
	      (void) fprintf (ccode, ";");
	      re->value.ival = stackno;
	      return (stackno);
	    }
	  else
	    {
	      re->token = MSTACK;
	      (void) fprintf (ccode, ";");
	      re->value.ival = stackno;
	      return (stackno);
	    }
	}
      else
	{
	 if (re->right != NULL && re->token != MTEXTADR)
 	 stackno = savepar (ret, re->right, FALSE, ident, type, stackno);
 	 if (re->left != NULL && re->token != MARRAYADR
 	 && re->token != MNAMEADR)
	   stackno = savepar (ret, re->left, FALSE, ident, type, stackno);
	 return (stackno);
       }
    }
}

/******************************************************************************
                                                               GENNAMEACCESS */

static gennameaccess (rut, ret, re) char *rut; struct EXP *ret, *re; { int i;
genstack
 (ret, re, FALSE);
  (void) fprintf (ccode, "if(%s&", rut);
  genchainrdencl (re, TRUE);
  (void) fprintf (ccode, "%s,", re->rd->ident);
  gen_ant_stack (ret, re);
  (void) fprintf (ccode, ",%d,", i = newlabel ());
  if (separat_comp)
    (void) fprintf (ccode, "__m_%s))", timestamp);
  else
    (void) fprintf (ccode, "__NULL))");
  exitcondlabel (i);
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
                                                                GENCALL      */

gencall (ret, re)
     struct EXP *ret,
      *re;
{
  int l,
    i;
  int r_elm_nr,
    v_elm_nr;			/* Stakk element nr n}r stakk elementene
				 * brukes som vanlige variable bl.a. i
				 * MARRAYARG,MDOT                         */
  struct EXP *rex;
  char properproc = FALSE;
  exinfop p;

  switch (re->token)
    {
    case MARGUMENT:
      genprocparam (re);
      break;
    case MNEWARG:
      genstack (ret, re, FALSE);
      (void) fprintf (ccode, "__sl=");
      genchainrdencl (re, FALSE);
      (void) fprintf (ccode, ";");
      if (re->rd->descr->stat)
	(void) fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;"
			,re->rd->descr->blno, timestamp);
      (void) fprintf (ccode, "__rcp(");
      gen_adr_prot (re->rd);

      (void) fprintf (ccode, ",");
      gen_ant_stack (ret, re);
      (void) fprintf (ccode, ");");
      genprocparam (re);

      if (re->rd->descr->externid != 0)
	{
	  if (separat_comp)
	    {
#if SPLIT_MODUL
	      (void) fprintf (ccode, "__rccb(0,__l%d%s);", 
			      l = newlabel (), timestamp);
#else
	      (void) fprintf (ccode, "__rccb(%d,__m_%s);", 
			      l = newlabel (), timestamp);
#endif
	      (void) fprintf (ccode, "return;");
	      typelabel (l);
	    }
	  else
	    {
#if SPLIT_MODUL
	      (void) fprintf (ccode, "__rccb(0,__l%d);", l = newlabel ());
#else
	      (void) fprintf (ccode, "__rccb(%d,__NULL);", l = newlabel ());
#endif
	      exitlabel (l);
	    }

	}
      else
	{
	  if (separat_comp)
#if SPLIT_MODUL
	    (void) fprintf (ccode, "__rccb(0,__l%d%s);", 
			    l = newlabel (), timestamp);
#else
	    (void) fprintf (ccode, "__rccb(%d,__m_%s);", 
			    l = newlabel (), timestamp);
#endif
	  else
#if SPLIT_MODUL
	    (void) fprintf (ccode, "__rccb(0,__l%d);", l = newlabel ());
#else
	    (void) fprintf (ccode, "__rccb(%d,__NULL);", l = newlabel ());
#endif
	  exitlabel (l);
	}

      (void) fprintf (ccode, "__r[%d]=__er;", 
		      (int) (re->value.ival 
			     = findallentry (ret, re, USEDREF)));
      re->token = MSTACK;
      break;
    case MPROCARG:

      if ((p = (exinfop) re->rd->descr->hiprot) != NULL)
	{
          /* Ekstern C-prosedyre eller en av standard prosedyrene */
	  switch (p->codeclass)
	    {
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
		    (void) savepar (ret, re, TRUE, rex->left->rd->ident,
				    rex->left->rd->type, 0L);
		}
	      if (re->type == TTEXT)
		{
		  if (is_after_dot (re) && nonetest == ON)
		    {
		      (void) fprintf (ccode, "if(");
		      genvalue (re->up->left);
		      (void) fprintf (ccode, "==__NULL)__rerror(__errnone);");
		    }
		  genctextproccall (ret, re);
		  return;
		}
	      else
		{
		  for (rex = re->right; rex->token != MENDSEP; 
		       rex = rex->right)
		    gencall (ret, rex->left);
		  return;
		}
	    case CCFILEDANGER:
	    case CCSIMPLEDANGER:
	    case CCTEXTDANGER:
	    case CCRANDOMRUTDANGER:
	    case CCBLANKSCOPY:
	    case CCFILEBLANKSCOPY:
	      if (!re->danger)
		goto not_danger;

	      if (is_after_dot (re) && re->up->left->type == TTEXT)
		genstack (ret, re->up->left, FALSE);
	      else
		genstack (ret, re, FALSE);

	      if (is_after_dot (re) && re->up->left->type != TTEXT
		  && nonetest == ON)
		{
		  (void) fprintf (ccode, "if(");
		  genvalue (re->up->left);
		  (void) fprintf (ccode,
				  "==__NULL)__rerror(__errnone);");
		}
	      genpredefproccall (ret, re, TRUE);
	      return;
	      break;
	    default:
	    not_danger:
	      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
		gencall (ret, rex->left);
	      return;
	    }
	}

      if (re->type == TNOTY)
	properproc = TRUE;

      if (is_after_dot (re) && re->up->left->type == TREF
	  && nonetest == ON)	/* Siste test utelukker text-dotting */
	{
	  (void) fprintf (ccode, "if(");
	  genvalue (re->up->left);
	  (void) fprintf (ccode, "==__NULL)__rerror(__errnone);");
	}
      if (re->rd->categ == CVIRT)
	{
	  /* Kall p} en virtuell prosedyre. Prosedyrens prototype  er gitt i
	   * virtuell tabellen. M} teste at den ikke er  NULL, som git
	   * run-time error. */
	  if (re->type != TNOTY)
	    {
	      /* Hvis det er et dott'et kall, er det ikke n|dvendig } kalle
	       * genstack for uttrykket foran doten. */
	      if (is_after_dot (re))
		genstack (ret, re->up, FALSE);
	      else
		genstack (ret, re, FALSE);
	    }
	  (void) fprintf (ccode, "if((__pp=");
	  gensl (re, FALSE, OFF);
	  (void) fprintf (ccode, "->pp->virt[%d])==__NULL)", 
			  re->rd->virtno - 1);
	  (void) fprintf (ccode, "__rerror(__errvirt);");
	}
      else if (re->rd->categ == CNAME)
	{
	  /* Prosedyren er en NAME-parameter. Kaller p} __rgetproc  som
	   * returnerer med statisk link i sl og prototypen    i pp. */
	  gennameaccess ("__rgetproc(", ret, re);
	} else
      if(re->type!=TNOTY)
        {
           /* Hvis det er et dott'et kall, er det ikke nødvendig å
            * kalle genstack for uttrykket foran doten. */
           if(is_after_dot(re))
              genstack(ret,re->up,FALSE);
           else
              genstack(ret,re,FALSE);
        }


      /* Statisk link overf|res i den globale variabelen sl. Genererer kallet 
       * p} rcp. */

      if (re->rd->categ != CNAME)
	{
	  (void) fprintf (ccode, "__sl=");
	  if (re->rd->categ == CDEFLT)
	    {
	      gensl (re, TRUE, OFF);
	      (void) fprintf (ccode, "%s.psl;", re->rd->ident);
	    }
	  else
	    {
	      gensl (re, FALSE, OFF);
	    }
	  (void) fprintf (ccode, ";");
	}

      if (re->rd->descr->stat)
	(void) fprintf (ccode, "__sto= (__dhp)&__blokk%d%s;"
			,re->rd->descr->blno, timestamp);

      if (properproc)
	(void) fprintf (ccode, "__rcpp(");	/* ??? */
      else
	(void) fprintf (ccode, "__rcp(");

      if (re->rd->categ == CNAME || re->rd->categ == CVIRT)
	(void) fprintf (ccode, "__pp");
      else if (re->rd->categ == CDEFLT)
	{
	  genchainrdencl (re, TRUE);
	  (void) fprintf (ccode, "%s.pp", re->rd->ident);
	}
      else
	gen_adr_prot (re->rd);

      if (properproc)
	(void) fprintf (ccode, ");");
      else
	{
	  (void) fprintf (ccode, ",");
	  gen_ant_stack (ret, re);
	  (void) fprintf (ccode, ");");
	}

      /* Kaller p} genprocparam som genererer kode for parameter-
       * overf|ringen. */

      genprocparam (re);

      /* Hvis dette er et dot'et kall s} skal ikke  doten ses n}r genvalue
       * kalles.(Den informasjonen trengs ikke da) */

      remove_dot (&re);

      /* N} er alle parameterene overf}rt, og prosedyren kan  settes i gang. */

      if (re->rd->descr->externid != 0)
	{			/* Kall p} ektern prosedyre. */
	  if (separat_comp)
	    {
#if SPLIT_MODUL
	      (void) fprintf (ccode, "__rcpb(0,__l%d%s);", 
			      l = newlabel (), timestamp);
#else
	      (void) fprintf (ccode, "__rcpb(%d,__m_%s);", 
			      l = newlabel (), timestamp);
#endif
	      (void) fprintf (ccode, "return;");
	      typelabel (l);
	    }
	  else
	    {
#if SPLIT_MODUL
	      (void) fprintf (ccode, "__rcpb(0,__l%d);", l = newlabel ());
#else
	      (void) fprintf (ccode, "__rcpb(%d,__NULL);", l = newlabel ());
#endif
	      exitlabel (l);
	    }
	}			/* END kall p} ektern prosedyre. */
      else
	{
	  if (separat_comp)
#if SPLIT_MODUL
	    (void) fprintf (ccode, "__rcpb(0,__l%d%s);", 
			    l = newlabel (), timestamp);
#else
	    (void) fprintf (ccode, "__rcpb(%d,__m_%s);", 
			    l = newlabel (), timestamp);
#endif
	  else
#if SPLIT_MODUL
	    (void) fprintf (ccode, "__rcpb(0,__l%d);", l = newlabel ());
#else
	    (void) fprintf (ccode, "__rcpb(%d,__NULL);", l = newlabel ());
#endif
	  exitlabel (l);
	}

      if (properproc)
	return;

      /* H}ndterer evt. funksjonsverdier. Sjekker om det
       * er n|dvendig med konvertering av aritm.  returverier eller
       * kvalifikasjonskontroll for type REF   Dette gjelder formelle
       * prosedyrer med categ lik CVAR og CNAME (type = TREF, TINTG og TREAL) 
       */

      switch (re->type)
	{
	case TREF:
	  (void) fprintf (ccode, "__r[%d]=__er;", 
			  (int) (re->value.ival 
				 = findallentry (ret, re, USEDREF)));
	  if (re->rd->categ == CVAR || re->rd->categ == CNAME)
	    {
	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (re, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "if(((__bs%d *)__bp)->%s", 
			      re->rd->encl->blno,
			      re->rd->ident);
	      (void) fprintf (ccode, ".conv==__READTEST ");
	      (void) fprintf (ccode, "&& !__rin(__er,((__bs%d *)__bp)->%s.q))",
			      re->rd->encl->blno, re->rd->ident);
	      (void) fprintf (ccode, "__rerror(__errqual);");
	    }
	  break;
	case TTEXT:
	  (void) fprintf (ccode, "__t[%d]=__et;", 
			  (int) (re->value.ival 
				 = findallentry (ret, re, USEDTXT)));
	  break;
	case TREAL:
	  if (re->rd->categ == CVAR || re->rd->categ == CNAME)
	    {			/* Tre muligheter : ingen, int -> real,
				 * real->int->real */
	      (void) fprintf (ccode, "__v[%d].f=((__conv=", 
			      (int) (re->value.ival 
				     = findallentry (ret, re, USEDVAL)));
	      genchainrdencl (re, TRUE);
	      (void) fprintf (ccode, "%s.conv)==__NOCONV?",
			      re->rd->ident);
	      (void) fprintf (ccode, "__ev.f:__conv==__INTREAL?(double)__ev.i:(double)__rintrea(__ev.f));");
	    }
	  else
	    (void) fprintf (ccode, "__v[%d].f=__ev.f;", 
			    (int) (re->value.ival 
				   = findallentry (ret, re, USEDVAL)));

	  break;
	case TINTG:
	  if (re->rd->categ == CNAME || re->rd->categ == CVAR)
	    {			/* To muligheter : ingen konvertering eller
				 * real->int */
	      (void) fprintf (ccode, "__v[%d].i=(", 
			      (int) (re->value.ival 
				     = findallentry (ret, re, USEDVAL)));
	      genchainrdencl (re, TRUE);
	      (void) fprintf (ccode, "%s.conv==__NOCONV?",
			      re->rd->ident);
	      (void) fprintf (ccode, "__ev.i:__rintrea(__ev.f));");
	    }
	  else
	    (void) fprintf (ccode, "__v[%d].i=__ev.i;", 
			    (int) (re->value.ival 
				   = findallentry (ret, re, USEDVAL)));
	  break;
	default:
	  (void) fprintf (ccode, "__v[%d].c=__ev.c;", 
			  (int) (re->value.ival 
				 = findallentry (ret, re, USEDVAL)));
	  break;
	}
      re->token = MSTACK;
      break;
    case MASSIGNR:
      if (re->danger)
	genstack (ret, re, TRUE);
      gencall (ret, re->left);
      gencall (ret, re->right);
      if ((rex = re->left)->token == MNAMEADR && rex->type == TREF)
	{
	  /* SKRIVE-AKSESS P} REFERANSE NAME-PARAMETER M} da
	   * legge inn kode som ,hvis n|dvendig , sjekker om h|yre side er
	   * 'in' strengeste kvalifikasjon p} aksessveien */

	  (void) fprintf (ccode, "if(((__nrp= &(");
	  genchainrdencl (rex, TRUE);
	  (void) fprintf (ccode, "%s))->conv==__WRITETEST ", rex->rd->ident);
	  (void) fprintf (ccode, "|| __nrp->conv==__READWRITETEST) && !__rin(");
	  genvalue (re->right);
	  (void) fprintf (ccode, ",__nrp->q))");
	  (void) fprintf (ccode, "__rerror(__errqual);");
	}
      break;
    case MREFASSIGNT:
      if (re->danger)
	genstack (ret, re, TRUE);
      gencall (ret, re->left);
      gencall (ret, re->right);
      break;
    case MARRAYARG:
      genstack (ret, re, FALSE);

      if (re->type == TLABEL)
	{
	  gencall (ret, re->right->left);
	  break;
	}
      r_elm_nr = findallentry (ret, re, USEDREF);
      v_elm_nr = findallentry (ret, re, USEDVAL);
      re->left = newexp;
      re->left->value.ival = r_elm_nr;
      re->left->left = (struct EXP *) v_elm_nr;
      re->left->right = NULL;
      re->left->token = MARRAYADR;
      re->left->up = re;
      if (re->rd->categ == CNAME)
	{
	  /* Aksess av ARRAY name-parameter. M} kalle p} __rgeta */
#if 0
	  (void) fprintf (ccode, "if(__rgeta(&");
	  genchain (re->rd->encl, TRUE);
	  (void) fprintf (ccode, "%s,", re->rd->ident);
	  gen_ant_stack (ret, re);
	  (void) fprintf (ccode, ",%d,", i = newlabel ());
	  if (separat_comp)
	    (void) fprintf (ccode, "__m_%s))", timestamp);
	  else
	    (void) fprintf (ccode, "__NULL))");
	  exitcondlabel (i);
#else
	  gennameaccess ("__rgeta(", ret, re);
#endif
	  /* Peker til arrayet er returnert i er */
	  (void) fprintf (ccode, "__r[%d]=__er;", r_elm_nr);
	}
      gencall (ret, re->right);
      /* Legger ut kode som sjekker indeksene og at det antall indekser
       * stemmer med dimmensjonen */
      if (is_after_dot (re))
	{
	  (void) fprintf (ccode, "__r[%d]=(__dhp)", r_elm_nr);
	  genvalue (re->up);
	  (void) fprintf (ccode, ";");
	}
      else if (re->rd->categ != CNAME)	/* Name er behandlet ovenfor */
	{
	  if (seen_th_insp (re))
	    {
	      (void) fprintf (ccode, "__r[%d]=(__dhp)((__bs%d *)", r_elm_nr,
			      re->rd->encl->blno);
	      genchain (re->seenthrough->quant.match->descr, TRUE);
	      (void) fprintf (ccode, "c%d)->%s;", re->seenthrough->connest,
			      re->rd->ident);
	    }
	  else
	    {
	      (void) fprintf (ccode, "__r[%d]=(__dhp)", r_elm_nr);
	      genchain (re->rd->encl, TRUE);
	      (void) fprintf (ccode, "%s;", re->rd->ident);
	    }
	}
      if (re->rd->dim == 0)
	{
	  /* Array som parameter.M} legge ut kode som sjekker dimmensjonen
	   * ved Run-time */
	  dim = 0;
	  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	    dim++;

	  (void) fprintf 
	    (ccode, "((__arrp)__r[%d])->h.dim!=%d?__rerror(__errarr):1;", 
	     r_elm_nr, dim, re->rd->ident);
	}
      dim = 0;
      for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
	{
	  if (dim == MAX_ARRAY_DIM)
	    gerror (85);
	  (void) fprintf (ccode, "__h[%d]=", dim++);
	  genvalue (rex->left);
	  (void) fprintf (ccode, "-((__arrp)__r[%d])->limits[%d].low;", 
			  r_elm_nr, dim - 1);
	}
      (void) fprintf (ccode, "if(");
      for (i = 0; i < dim; i++)
	{
	  (void) fprintf 
	    (ccode, "__h[%d]<0 || __h[%d]>=((__arrp)__r[%d])->limits[%d].size",
	     i, i, r_elm_nr, i);
	  if (i < dim - 1)
	    (void) fprintf (ccode, " ||   ");
	}
      (void) fprintf (ccode, ")__rerror(__errbound);");

      (void) fprintf (ccode, "__v[%d].i=sizeof(__ah)+sizeof(__arrlimit)*%d+((",
		      v_elm_nr, dim);
      for (i = dim - 1; i > 0; i--)
	{
	  (void) fprintf (ccode, "((__arrp)__r[%d])->limits[%d].size*(",
			  r_elm_nr, i);
	}

      (void) fprintf (ccode, "__h[0])");

      for (i = 1; i < dim; i++)
	(void) fprintf (ccode, "+__h[%d])", i);
      (void) fprintf (ccode, "*sizeof(");
      gentype (re);
      (void) fprintf (ccode, "));");

      remove_dot (&re);

      re->token = MARRAYADR;
      re->value.ival = r_elm_nr;
      re->left = (struct EXP *) v_elm_nr;	/* Stakknr hvor offset ligger 
						 * legges i left */
      re->right = NULL;
      break;
    case MIDENTIFIER:

      if (re->rd->kind == KPROC && re->rd->categ == CNAME)
	{
	  /* Procedure overf|rt "by name" som aktuell parameter til en VAR
	   * eller STANDARD parameter. */
	  gennameaccess ("__rgetproc(", ret, re);

	  /* Det er ikke n|dvendig } legge sl og pp p} stakken da den blir
	   * brukt med en gang etter kallet p} gencall, og ikke flere ganger
	   * i dette kallet. */
	}
      else if (re->rd->kind == KARRAY && re->rd->categ == CNAME)
	{
	  /* Array overf|rt "by name" som aktuell parameter */
	  gennameaccess ("__rgeta(", ret, re);

	  /* Det er ikke n|dvendig } legge array-pekeren p}  stakken, da den
	   * blir brukt med en gang etter kallet p} gencall, og ikke flere
	   * ganger i dette kallet. */
	}
      else if (re->rd->categ == CNAME)
	{
	  if (re->type == TLABEL)
	    {
	      gennameaccess ("__rgetlab(", ret, re);

	      /* Det er IKKE n|dvendig } legge ev, er og modul      p}
	       * stakken siden de blir lest av f|r de evt. blir  brukt igjen. 
	       */
	    }
	  else if (((re->up->token == MASSIGN || re->up->token == MASSIGNR ||
	       re->up->token == MVALASSIGNT || re->up->token == MREFASSIGNT)
		    && re->up->left == re)
		   || (re->up->token == MARGUMENTSEP &&
		       re->up->rd->categ == CVAR	/* aktuell NAME til
							 * formell VAR */ ))
	    {			/* SKRIVE AKSESS */

	      gennameaccess ("__rgetsa(", ret, re);

	      /* Trenger ikke legge p} stakken for aktuell NAME til formell
	       * VAR. */
	      /* Adressen til lokasjonen returneres i er(base) og ev(offset). 
	       * Legger disse verdiene p} stakken */
	      (void) fprintf (ccode, "__r[%d]=__er;", 
			      (int) (re->value.ival 
				     = findallentry (ret, re, USEDREF)));
	      (void) fprintf (ccode, "__v[%d].i=__ev.i;", 
			      (int) (re->left 
				     = (struct EXP *) 
				     findallentry (ret, re, USEDVAL)));
	      re->token = MNAMEADR;
	    }
	  else
	    /* END-SKRIVEAKSESS NAME-PARAMETER */
	    {			/* LESE  AKSESS */

	      switch (re->type)
		{
		case TINTG:
		  gennameaccess ("__rgetav(__TINTG,", ret, re);
		  break;
		case TREAL:
		  gennameaccess ("__rgetav(__TREAL,", ret, re);
		  break;
		case TCHAR:
		case TBOOL:
		  gennameaccess ("__rgetcbv(", ret, re);
		  break;
		case TREF:
		  gennameaccess ("__rgetrv(", ret, re);
		  break;
		case TTEXT:
/* TBD I noen tilfeller kan det optimaliseres ved å gjøre __rgettv i stedet */
/* NB, da må tilsvarende kode endres lenger nede */
		  gennameaccess ("__rgetta(", ret, re);
		  break;
		}

	      /* Verdiene er returnert i variablene er (REF),  ef (REAL), ev
	       * (INTEGER), ec (CHAR og BOOL) eller en addressen til en
	       * text-variabel (som kan v{re __et) i __er og __ev.i.
	       * Legger denne verdien p} RT-stakken, og
	       * renamer  nodens token til MSTACK eller til MNAMEADR */
	      switch (re->type)
		{
		case TINTG:
		  (void) fprintf (ccode, "__v[%d].i=__ev.i;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDVAL)));
		  re->token = MSTACK;
		  break;
		case TREAL:
		  (void) fprintf (ccode, "__v[%d].f=__ev.f;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDVAL)));
		  re->token = MSTACK;
		  break;
		case TCHAR:
		case TBOOL:
		  (void) fprintf (ccode, "__v[%d].c=__ev.c;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDVAL)));
		  re->token = MSTACK;
		  break;
		case TREF:
		  (void) fprintf (ccode, "__r[%d]=__er;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDREF)));
		  re->token = MSTACK;
		  break;
		case TTEXT:
#if 0 /* Tilfellet __rgettv */
		  (void) fprintf (ccode, "__t[%d]=__et;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDTXT)));
		  re->token = MSTACK;
		  break;
#endif
		  (void) fprintf (ccode, "switch (");
		  genchainrdencl (re, TRUE);
		  (void) fprintf (ccode, "%s.h.namekind){"
				  , re->rd->ident);
		  (void) fprintf (ccode, 
			     "case __ADDRESS_THUNK: case __ADDRESS_NOTHUNK: ");
		  (void) fprintf (ccode, "__v[%d].i=__ev.i;", 
				  (int) (re->left 
					 = (struct EXP *) 
					 findallentry (ret, re, USEDVAL)));
		  (void) fprintf (ccode, "__r[%d]=__er;", 
				  (int) (re->value.ival 
					 = findallentry (ret, re, USEDREF)));
		  (void) fprintf (ccode, 
			  "break; case __VALUE_THUNK: case __VALUE_NOTHUNK: ");
		  (void) fprintf (ccode, "__t[%d]=__et;", 
				  (int) (re->right 
					 = (struct EXP *) 
					 findallentry (ret, re, USEDTXT)));
		  (void) fprintf (ccode, "__r[%d] = __NULL;",
				  (int) re->value.ival);
		  (void) fprintf (ccode, 
			    "__v[%d].i = ((char *)&__t[%d])-((char *) 0);}",
				  (int) re->left, (int) re->right);
		  re->token = MTEXTADR;
		  break;
		}		/* END-SWITCH */

	    }			/* END-LESEAKSESS AV NAME-PAR */
	}			/* END-NAME-PARAMETER */
      break;
    case MANDTHEN:
    case MORELSE:
      gencall (ret, re->left);
      if (workbeforetest (re->right))
	{
	  genstack (ret, re, FALSE);
	  (void) fprintf (ccode, "if(");
	  if (re->token == MANDTHEN)
	    (void) fprintf (ccode, "!");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")");
	  gotollabel (i = newllabel ());
	  gencall (ret, re->right);
	  typellabel (i);
	}
      break;
    case MIFE:
      gencall (ret, re->left);
      if (workbeforetest (re->right))
	{
	  genstack (ret, re->right, FALSE);
	  (void) fprintf (ccode, "if(!");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")");
	  gotollabel ((int) (re->right->value.ival = newllabel ()));
	  gencall (ret, re->right);
	  /* Labelen legges i value attributtet i noden til MELSEE */
	  typellabel ((int) re->value.ival);
	}
      break;
    case MELSEE:
      gencall (ret, re->left);
      gotollabel ((int) (re->up->value.ival = newllabel ()));
      typellabel ((int) re->value.ival);
      gencall (ret, re->right);
      break;
    case MCONC:
#if 0
      gencall (ret, re->left);
      /* V{r spesielt oppmerksom p} at __rconc tar en peker til en tekst
       * variabel som parameter og det dermed er viktig at denne  variabelen
       * ikke blir flyttet hvis s|ppelt|mming skulle forekomme. Vi m} dermed
       * kopier argumentene over p} stacken om vi ikke har } gj|re med en
       * tekst-konstant eller at tekst-variabelen er deklarert i en blokk som 
       * er allokert statisk */
      if (re->left->token != MDOTCONST & re->left->token != MTEXTKONST &
	  re->left->token != MSTACK &
	  (re->left->token != MIDENTIFIER || !re->left->rd->encl->stat))
	{
	  (void) fprintf (ccode, "__rtextassign(&__t[%d],"
	    ,i = findallentry (ret, re->right, USEDTXT));
	  genvalue (re->left, FREE);
	  re->left->value.ival = i;
	  (void) fprintf (ccode, ");");
	  re->left->token = MSTACK;
	}
      gencall (ret, re->right);
      if (re->right->token != MDOTCONST & re->right->token != MTEXTKONST &
	  re->right->token != MSTACK &
	  (re->right->token != MIDENTIFIER || !re->right->rd->encl->stat))
	{
	  (void) fprintf (ccode, "__rtextassign(&__t[%d],"
	   , i = findallentry (ret, re->right, USEDTXT));
	  genvalue (re->right, FREE);
	  re->right->value.ival = i;
	  (void) fprintf (ccode, ");");

	  re->right->token = MSTACK;
	}
      (void) fprintf (ccode, "__rtextassign(&__t[%d],__rconc("
		  , (int) (re->value.ival 
			   = findallentry (ret, re->right, USEDTXT)));
      gen_ant_stack (ret, re);
      (void) fprintf (ccode, ",");
      genvalue (re->left, FREE);
      (void) fprintf (ccode, ",");
      genvalue (re->right, FREE);
      (void) fprintf (ccode, "));");
      re->token = MSTACK;
      re->left = re->right = NULL;
#endif
      gencall (ret, re->left);
      gencall (ret, re->right);
      (void) fprintf (ccode, "__t[%d]= *__rconc("
		  , (int) (re->value.ival 
			   = findallentry (ret, re, USEDTXT)));
      gen_ant_stack (ret, re);
      (void) fprintf (ccode, ",");
      genvalue (re->left, FREE);
      (void) fprintf (ccode, ",");
      genvalue (re->right, FREE);
      (void) fprintf (ccode, ");");
      re->token = MSTACK;
      re->left = re->right = NULL;
      break;
    default:
      if (re->left != NULL)
	gencall (ret, re->left);
      if (re->right != NULL)
	gencall (ret, re->right);
    }
}

/******************************************************************************
                                                                GENVALUE     */

genvalue (re)
     struct EXP *re;
{
  struct EXP *rex;
  static struct EXP *ree;

  if (re == NULL)
    {
#ifdef DEBUG
      (void) fprintf (stderr, "re==NULL kalt fra token: %s value: %d\n",
		      texttoken (ree->token),
#else
      (void) fprintf (stderr, "re==NULL kalt fra token: %d value: %d\n",
		      ree->token,
#endif
		      (int) ree->value.ival);
      return;
    }
  ree = re;
  switch (re->token)
    {
    case MPROCARG:
      /* Predefinerte prosedyrer, C-prosedyrer eller vanlige 
       * proper-procedures, som er behandlet av gencall. De
       * predefinerte og C-prosedyrene skal behandles her, mens vanlige
       * proper-procedures allerede er behandlet i gencall. */
      if (re->rd->descr->hiprot == NULL)
	break;

      if (re->right == NULL)
	break;
      /* Det siste tilfelle skal egentlig aldri sl} til. Det ville i s}
       * fall bety at en predefinert properprocedure ville v{rt spesifisert
       * som DANGER, noe som ikke er riktig. */

      if (re->rd->categ == CCPROC)
	gencproccall (re);
      else
	genpredefproccall ((struct EXP *) NULL, re, FALSE);
      break;
    case MNOT:
      (void) fprintf (ccode, "(!");
      genvalue (re->left);
      (void) putc (')', ccode);
      break;
    case MIMP:
      (void) fprintf (ccode, "(!");
      genvalue (re->left);
      (void) fprintf (ccode, "|");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MIS:
      (void) fprintf (ccode, "__ris(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");

      gen_adr_prot (re->rd);
      (void) fprintf (ccode, ")");
      break;
    case MINS:
      (void) fprintf (ccode, "__rin(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      gen_adr_prot (re->rd);
      (void) fprintf (ccode, ")");
      break;
    case MEQT:
      (void) fprintf (ccode, "__reqtext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MNET:
      (void) fprintf (ccode, "!__reqtext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MLTT:
      (void) fprintf (ccode, "__rlttext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MLET:
      (void) fprintf (ccode, "__rletext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MGTT:
      (void) fprintf (ccode, "__rlttext(");
      genvalue (re->right);
      (void) fprintf (ccode, ",");
      genvalue (re->left);
      (void) fprintf (ccode, ")");
      break;
    case MGET:
      (void) fprintf (ccode, "__rletext(");
      genvalue (re->right);
      (void) fprintf (ccode, ",");
      genvalue (re->left);
      (void) fprintf (ccode, ")");
      break;
    case MEQRT:
      (void) fprintf (ccode, "__reqrtext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MNERT:
      (void) fprintf (ccode, "!__reqrtext(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MIFE:
      if (re->type == TLABEL)
	{
	  (void) fprintf (ccode, "if(!");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")");
	  gotollabel ((int) (re->right->value.ival = newllabel ()));
	  genvalue (re->right);
	  break;
	}
      (void) putc ('(', ccode);
      genvalue (re->left);
      (void) fprintf (ccode, "?");
      genvalue (re->right);
      (void) putc (')', ccode);
      break;
    case MELSEE:
      if (re->type == TLABEL)
	{
	  genvalue (re->left);
	  typellabel ((int) re->value.ival);
	  genvalue (re->right);
	  break;
	}
      genvalue (re->left);
      (void) fprintf (ccode, ":");
      genvalue (re->right);
      break;
    case MUADD:
    case MUADDI:
      genvalue (re->left);
      break;
    case MUSUB:
    case MUSUBI:
      (void) fprintf (ccode, "(-");
      genvalue (re->left);
      (void) fprintf (ccode, ")");
      break;
    case MPRIMARY:
      (void) fprintf (ccode, "__rpow(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MPRIMARYII:
      (void) fprintf (ccode, "__rpowii(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MPRIMARYRI:
      (void) fprintf (ccode, "__rpowri(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MREAINT:
      (void) fprintf (ccode, "(double)");
      genvalue (re->left);
      break;
    case MINTREA:
      (void) fprintf (ccode, "__rintrea(");
      genvalue (re->left);
      (void) fprintf (ccode, ")");
      break;
    case MTEXTKONST:
      if (re->value.tval.id == NOTEXT)
	{
	  char *t;
	  static int it = 0;
	  int antchar;
	  t = re->value.tval.txt;
	  antchar = sstrlen (t);
	  if (antchar == 0)
	    re->value.tval.id = NOTEXT;
	  else
	    {
	      anttext++;
	      fprintf (hcode, "struct __tt%d {__txt tvar;", anttext);
	      (void) fprintf (hcode, "__th h;char string[%d];}\n", antchar + 1);
	      (void) fflush (hcode);
	      (void) fprintf (hcode, "__tk%d%s={(__textref)&__tk%d%s.h.pp",
			      anttext, timestamp, anttext, timestamp);
	      (void) fprintf 
		(hcode, 
		 ",%d,1,1,(__pty)__TEXT,(__dhp)&__tk%d%s.h.pp,__CONSTANT,%d,\"",
		 antchar, anttext, timestamp, antchar);
	      (void) fprintf (hcode, "%s", t);
	      (void) fprintf (hcode, "\"};\n");
	      re->value.tval.id = anttext;
	    }
	}
      (void) fprintf (ccode, "(__txtvp)&__tk%d%s", re->value.tval.id,
		      re->value.tval.id == NOTEXT ? ""
/***** NOTEXT, ingen timestamp ****/
		      : timestamp);
      break;
    case MCHARACTERKONST:
    case MBOOLEANKONST:
      if (re->value.ival < 0)
	(void) fprintf (ccode, " ");
      (void) fprintf (ccode, "%d", (int) re->value.ival);
      break;
    case MINTEGERKONST:
      if (re->value.ival < 0)
	(void) fprintf (ccode, " ");
      if (re->value.ival == (-MAX_INT - 1))
	{
	  (void) fprintf (ccode, "(%ldL-1L)", re->value.ival+1);
	} else
	{
	  (void) fprintf (ccode, "%ldL", re->value.ival);
	}
      break;
    case MREALKONST:
      if (re->value.rval <= 0.0)
	(void) fprintf (ccode, " ");
      (void) fprintf (ccode, "%.16le", re->value.rval);
      break;
    case MNONE:
      (void) fprintf (ccode, "__NULL");
      break;
    case MSTACK:
      switch (re->type)
	{
	case TREF:
	  (void) fprintf (ccode, "__r[%d]", (int) re->value.ival);
	  break;
	case TINTG:
	  (void) fprintf (ccode, "__v[%d].i", (int) re->value.ival);
	  break;
	case TREAL:
	  (void) fprintf (ccode, "__v[%d].f", (int) re->value.ival);
	  break;
	case TTEXT:
	  (void) fprintf (ccode, "(__txtvp)&__t[%d]", (int) re->value.ival);
	  break;
	default:
	  (void) fprintf (ccode, "__v[%d].c", (int) re->value.ival);
	  break;
	}
      break;
    case MIDENTIFIER:
      if (re->type == TLABEL)
	{
	  switch (re->rd->categ)
	    {
	    case CDEFLT:
	    case CVAR:
	      /* Setter bp, en hjelpevariabel, til } peker p} den aktuelle
	       * parameterens blokk. Dermed blir aksessveien kortere
	       * for de etterf|lgende aksessene */
	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (re, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "__rgoto(((__bs%d *)__bp)->%s.ob);", 
			      re->rd->encl->blno,
			      re->rd->ident);
	      (void) fprintf (ccode, "__goto=((__bs%d *)__bp)->%s.adr;",
			      re->rd->encl->blno, re->rd->ident);
	      gotoswitch ();
	      break;
	    case CNAME:
	      /* Gencall har skrevet ut kallet p} rgetlab, slik at adressen
	       * ligger i modul og ev, og objekt-   pekeren ligger i er. */
	      (void) fprintf (ccode, "__rgoto(__er);");
	      (void) fprintf (ccode, "__goto=__ev.adr;");
	      gotoswitch ();
	      break;
	    case CVIRT:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  (void) fprintf (ccode, "__rgoto(");
		  genchainrdencl (re, FALSE);
		  (void) fprintf (ccode, ");");
		  (void) fprintf (ccode, "if((__pp=__lb");
		}
	      else
		{
		  (void) fprintf (ccode, "if((__pp=");
		  genchain (re->rd->encl, FALSE);
		}
	      (void) fprintf (ccode, "->pp)->virtlab[%d].ent==0)",
			      re->rd->virtno - 1);
	      (void) fprintf (ccode, "__rerror(__errvirt);");
	      (void) fprintf (ccode, "__goto=__pp->virtlab[%d];",
			      re->rd->virtno - 1);
	      gotoswitch ();
	      break;
	    case CLOCAL:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  (void) fprintf (ccode, "__rgoto(");
		  genchainrdencl (re, FALSE);
		  (void) fprintf (ccode, ");");
		}
	      if (re->rd->idplev.plev == 0)
		re->rd->idplev.plev = newlabel ();
	      if (re->rd->encl->externid != 0)
		{
		  /* Skal hoppe til en label i en annen modul */
		  (void) fprintf (ccode, "__goto.ent=%d;", 
				  re->rd->idplev.plev);
		  (void) fprintf (ccode, "__goto.ment=__m_%s;",
				extract_timestamp (re->rd->encl->externid));
		  gotoswitch ();
		}
	      else
		gotolabel (re->rd->idplev.plev);
	      break;
	    }
	  not_reached = TRUE;
	  break;
	}

      if (is_after_dot (re))
	(void) fprintf (ccode, "%s", re->rd->ident);
      else
	{
	  if (seen_th_insp (re))
	    {
	      if (re->type == TTEXT && re->rd->kind != KARRAY)
		(void) fprintf (ccode, "(__txtvp)&");
	      (void) fprintf (ccode, "((__bs%d *)", re->rd->encl->blno);
	      genchain (re->seenthrough->quant.match->descr, TRUE);
	      (void) fprintf (ccode, "c%d)->%s", re->seenthrough->connest,
			      re->rd->ident);
	    }
	  else
	    {
	      if (re->rd->categ == CVAR || re->rd->categ == CNAME)
		{
		  if (re->rd->categ == CVAR && re->rd->kind != KARRAY &&
		      (re->type == TREAL || re->type == TINTG) &&
		      (!(re->up->token == MASSIGN && re->up->left == re)))
		    {		/* Lese aksess av aritm. var-parameter. For
				 *   bare er gjort RT-call for skrive-aksess. 
				 */
		      if (re->type == TINTG)
			{	/* To muligheter : ingen eller real -> int */
			  (void) fprintf (ccode, "((__vvp= &(");
			  genchainrdencl (re, TRUE);
			  (void) fprintf (ccode, "%s))->conv==__NOCONV?",
					  re->rd->ident);

			  (void) fprintf 
			    (ccode, 
			     " *(long *)(((char *)__vvp->bp)+__vvp->ofs):__rintrea(");
			  (void) fprintf 
			    (ccode, 
			     " *(double *)(((char *)__vvp->bp)+__vvp->ofs)))");
			}
		      else
			{	/* Tre muligheter : ingen, int -> real,
				 * real->int->real */
			  (void) fprintf (ccode, "((__vvp= &(");
			  genchainrdencl (re, TRUE);
			  (void) fprintf (ccode, "%s))->conv==__NOCONV?",
					  re->rd->ident);
			  (void) fprintf 
			    (ccode, 
			     " *(double *)(((char *)__vvp->bp)+__vvp->ofs):");
			  (void) fprintf 
			    (ccode, "(__vvp->conv==__INTREAL?(double)");
			  (void) fprintf 
			    (ccode, 
			     " *(long *)(((char *)__vvp->bp)+__vvp->ofs):");
			  (void) fprintf 
			    (ccode, "(double)__rintrea( *(double *)");
			  (void) fprintf 
			    (ccode, "(((char *)__vvp->bp)+__vvp->ofs))))");
			}
		    }
		  else if (re->rd->categ == CNAME 
			   && re->up->token == MASSIGN &&
			   re->up->right == re)
		    {
		      /* Lese-aksess av en name-parameter som det    nettopp
		       * er gjort skrive-aksess p}. Vanligvis  gj|res
		       * konvertering av NAME-parametere av   RT-rutiene, men 
		       * ikke i tilfelle med multippel assignment. Det gj|res 
		       * da her. Noden er     omd|pt fra MNAMEADR til
		       * MIDENTIFER i case   MASSIGN grenen i genvalue. */

		      if (re->type == TINTG)
			{	/* To muligheter : ingen eller real -> int */
			  (void) fprintf (ccode, "((");
			  genchainrdencl (re, TRUE);
			  (void) fprintf (ccode, "%s)->conv==__NOCONV?",
					  re->rd->ident);
			  (void) fprintf 
			    (ccode, 
			     " *(long *)(((char *)__r[%d])+__v[%d].i):__rintrea(",
			     (int) re->value.ival, (int) re->left);
			  (void) fprintf 
			    (ccode, 
			     " *(double *)(((char *)__r[%d])+__v[%d].i)))",
			      (int) re->value.ival, (int) re->left);
			}
		      else
			{       /* Tre muligheter : ingen,int ->
				 * real,real->int->real */
			  (void) fprintf (ccode, "((__nvp= &(");
			  genchainrdencl (re, TRUE);
			  (void) fprintf (ccode, "%s))->conv==__NOCONV?",
					  re->rd->ident);
			  (void) fprintf 
			    (ccode, 
			     " *(double *)(((char *)__r[%d])+__v[%d].i):",
			      (int) re->value.ival, (int) re->left);
			  (void) fprintf 
			     (ccode, "(__nvp->conv==__INTREAL?(double)");
			  (void) fprintf 
			     (ccode, " *(long *)(((char *)__r[%d])+__v[%d].i):",
			      (int) re->value.ival, (int) re->left);
			  (void) fprintf 
			     (ccode, "(double)__rintrea( *(double *)");
			  (void) fprintf 
			     (ccode, "(((char *)__r[%d])+__v[%d].i))))",
			      (int) re->value.ival, (int) re->left);
			}
		    }
		  else if (re->type == TREF && re->rd->categ == CVAR &&
			 !(re->up->token == MASSIGNR && re->up->left == re))
		    {
		      /* Lese-aksess av referanse var-parametere. Legger inn
		       * kode som sjekker om re er "in" strengeste
		       * kvalifikasjon p} aksessveien. */

		      (void) fprintf (ccode, "((((__vrp= &");
		      genchainrdencl (re, TRUE);
		      (void) fprintf (ccode, "%s)->conv==__READTEST ",
				      re->rd->ident);
		      (void) fprintf 
			(ccode, "|| __vrp->conv==__READWRITETEST) &&");
		      (void) fprintf 
			(ccode, 
			 " !__rin((__bp= *(__dhp *)(((char *)__vrp->bp)+__vrp->ofs)),");
		      (void) fprintf 
			(ccode, 
			 "__vrp->q))?(__dhp)__rerror(__errqual):(__bp= *(__dhp *)(((char *)__vrp->bp)+__vrp->ofs)))");
		    }
		  else
		    {
		      /* For parametere av type Character, Boolean,   LESE og 
		       * SKRIVE-AKSESS AV B]DE VAR OG NAME-   PARAMETERE som
		       * ikke er behandlet lengre oppe */

		      if (re->rd->kind == KARRAY) 
			if (re->rd->categ ==CNAME)
			  fprintf (ccode, "(__arrp)__er");
			else
			  {
			    genchainrdencl (re, TRUE);
			    fprintf (ccode, "%s", re->rd->ident);
			  }
		      else
			{
			  if (re->type == TTEXT)
			    (void) fprintf (ccode, " (");
			  else
			    (void) fprintf (ccode, " *(");
			  gentype (re);
			  (void) fprintf (ccode, " *)(((char *)");

			  genchainrdencl (re, TRUE);
			  fprintf (ccode, "%s.", re->rd->ident);

			  if (re->rd->categ == CVAR)
			    (void) fprintf (ccode,
					    "bp)+");
			  else
			    (void) fprintf (ccode, "h.bp)+",
					    re->rd->ident);

			  fprintf (ccode, "%s.", re->rd->ident);
			  genchainrdencl (re, TRUE);

			  if (re->rd->categ == CVAR)
			    (void) fprintf (ccode, "ofs)", re->rd->ident);
			  else
			    (void) fprintf (ccode, "h.v.ofs)",
					    re->rd->ident);
			}

		      break;
		    }
		}		/* End Var eller Name-parameter */
	      else
		{
		  if (re->type == TTEXT && re->rd->kind != KARRAY)
		    (void) fprintf (ccode, "(__txtvp)&");
		  genchainrdencl (re, TRUE);
		  (void) fprintf (ccode, "%s", re->rd->ident);
		}
	    }
	}
      break;
    case MPROCASSIGN:
      if (re->type == TNOTY)
	break;
      if (re->type == TTEXT)
	(void) fprintf (ccode, "&");
      genchain (re->rd->descr, TRUE);
      if (re->type == TREF)
	(void) fprintf (ccode, "er");
      else if (re->type == TTEXT)
	(void) fprintf (ccode, "et");
      else if (re->type == TREAL)
	(void) fprintf (ccode, "ef");
      else if (re->type == TINTG)
	(void) fprintf (ccode, "ev");
      else
	(void) fprintf (ccode, "ec");
      break;
    case MTHIS:
      if (seen_th_insp (re))
	{
	  genchain (re->seenthrough->quant.match->descr, TRUE);
	  (void) fprintf (ccode, "c%d", re->seenthrough->connest);
	}
      else
	genchain (re->qual->descr, FALSE);
      break;
    case MQUA:
      /* Sjekker om det er n\dvendig } utf\re en none-test, eller om den er
       * utf\rt lengre ned i treet. */
      if (re->left->token != MDOT && re->left->token != MQUA &&
	  re->left->token != MQUANOTNONE && nonetest == ON)
	{
	  (void) fprintf (ccode, "((__bp=");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")==__NULL?(__dhp)__rerror(__errnone):(__bp");
	}
      else
	{
	  (void) fprintf (ccode, "(((__bp=");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")");
	}
      if (re->qual->idplev.plev >= DEF_PLEV_TAB_SIZE)
	(void) fprintf (ccode, "->pp->plev<%d || __bp",
			re->qual->idplev.plev);

      (void) fprintf (ccode, "->pp->pref[%d] != ",
		      re->qual->idplev.plev);
      gen_adr_prot (re->qual);
      (void) fprintf (ccode, ")?(__dhp)__rerror(__errqual):__bp)");
      break;
    case MQUANOTNONE:
      /* Sjekker om det er n\dvendig } utf\re en none-test, eller om den er
       * utf\rt lengre ned i treet. */
      if (re->left->token != MDOT && re->left->token != MQUA &&
	  re->left->token != MQUANOTNONE)
	{
	  if (nonetest == ON)
	    (void) fprintf (ccode, "((__bp=");
	  genvalue (re->left);
	  if (nonetest == ON)
	    (void) fprintf (ccode,
			    ")==__NULL?(__dhp)__rerror(__errnone):__bp)");
	}
      else
	genvalue (re->left);
      break;
    case MQUANONEAND:
      if (re->left->token == MNONE)
	(void) fprintf (ccode, "__NULL");
      else
	{
	  (void) fprintf (ccode, "(((__bp=");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")!=__NULL && (");
	  if (re->qual->idplev.plev >= DEF_PLEV_TAB_SIZE)
	    (void) fprintf (ccode, "__bp->pp->plev<%d || ",
			    re->qual->idplev.plev);
	  (void) fprintf (ccode, "__bp->pp->pref[%d]!= ",
			  re->qual->idplev.plev);
	  gen_adr_prot (re->qual);
	  (void) fprintf (ccode, "))?(__dhp)__rerror(__errqual):__bp)");
	}
      break;
    case MDOT:
      if (re->right->rd == NULL)
	{
	  (void) fprintf (stderr, "re->right->rd==NULL\n");
#ifdef DEBUG
	  dumpexprek (re->up);
#endif
	}
      if (re->right->rd->encl == NULL)
	(void) fprintf (stderr, "re->right->rd->encl==NULL\n");

      /* Dersom hoyre-siden er MPROCARG utfores NONE-testen i 
       * genpredefproccall. */
      if (re->right->token == MPROCARG)
	{
	  genvalue (re->right);
	  break;
	}

      if (re->left->type == TTEXT)
	{
	  genvalue (re->right);
	  break;
	}

      if (re->type == TTEXT && re->right->token != MARRAYARG &&
	  (re->right->token == MIDENTIFIER && re->right->rd->kind != KARRAY))
	(void) fprintf (ccode, "(__txtvp)&");

      (void) fprintf (ccode, "((__bs%d *)", re->right->rd->encl->blno);

      if (re->left->token == MQUA || re->left->token == MQUANOTNONE)
	{			/* None test er ikk{ n|dvendig } utf|re. Den
				 * er utf\rt i MQUA eller MQUANOTNONE noden. */
	  genvalue (re->left);
	  (void) fprintf (ccode, ")->");
	  genvalue (re->right);
	}
      else
	{
	  if (nonetest == ON)
	    (void) fprintf (ccode, "((__bp=");
	  genvalue (re->left);
	  if (nonetest == ON)
	    (void) fprintf (ccode,
			    ")==__NULL?(__dhp)__rerror(__errnone):__bp)");

	  (void) fprintf (ccode, ")->");
	  genvalue (re->right);
	}
      break;
    case MDOTCONST:
      if (nonetest == ON)
	(void) fprintf (ccode, "((");
      if (nonetest == ON)
	genvalue (re->left);
      if (nonetest == ON)
	(void) fprintf (ccode, ")==__NULL?(");
      gentype (re);
      if (re->type == TTEXT)
	(void) fprintf (ccode, " *");
      if (nonetest == ON)
	(void) fprintf (ccode, ")__rerror(__errnone):");
      genvalue (re->right);
      if (nonetest == ON)
	(void) fprintf (ccode, ")");
      break;
    case MARRAYARG:
      if (re->type == TLABEL)
	{
	  (void) fprintf (ccode, "__swv=");
	  genvalue (re->right->left);
	  (void) fprintf (ccode, ";");

	  switch (re->rd->categ)
	    {
	    case CDEFLT:
	    case CVAR:
	    case CNAME:
	      (void) fprintf (ccode, "__bp=");
	      genchainrdencl (re, FALSE);
	      (void) fprintf (ccode, ";");
	      (void) fprintf (ccode, "__rgoto(((__bs%d *)__bp)->%s.ob);", 
			      re->rd->encl->blno,
			      re->rd->ident);
	      (void) fprintf (ccode, "__goto=((__bs%d *)__bp)->%s.adr;",
			      re->rd->encl->blno, re->rd->ident);
	      gotoswitch ();
	      break;
	    case CVIRT:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  (void) fprintf (ccode, "__rgoto(");
		  genchainrdencl (re, FALSE);
		  (void) fprintf (ccode, ");");
		  (void) fprintf (ccode, "if((__pp=__lb");
		}
	      else
		{
		  (void) fprintf (ccode, "if((__pp=");
		  genchain (re->rd->encl, FALSE);
		}
	      (void) fprintf (ccode, "->pp)->virtlab[%d].ent==0)",
			      re->rd->virtno - 1);
	      (void) fprintf (ccode, "__rerror(__errvirt);");
	      (void) fprintf (ccode, "__goto=__pp->virtlab[%d];",
			      re->rd->virtno - 1);
	      gotoswitch ();
	      break;
	    case CLOCAL:
	      if (cblock->blev > re->rd->encl->blev)
		{
		  (void) fprintf (ccode, "__rgoto(");
		  genchainrdencl (re, FALSE);
		  (void) fprintf (ccode, ");");
		}
	      if (re->rd->idplev.plev == 0)
		re->rd->idplev.plev = newlabel ();
	      if (re->rd->encl->externid != 0)
		{
		  /* Skal hoppe til en label i en annen modul */
		  (void) fprintf (ccode, "__goto.ent=%d;", 
				  re->rd->idplev.plev);
		  (void) fprintf (ccode, "__goto.ment=__m_%s;",
				  extract_timestamp (re->rd->encl->externid));
		  gotoswitch ();
		}
	      else
		gotolabel (re->rd->idplev.plev);
	      break;
	    }
	  not_reached = TRUE;
	}
      else
	(void) fprintf (ccode, "%s", re->rd->ident);
      break;
    case MTEXTADR:
    case MNAMEADR:
    case MARRAYADR:
      if (re->type == TTEXT)
	(void) fprintf (ccode, " ((");
      else
	(void) fprintf (ccode, " (*(");
      gentype (re);
      (void) fprintf (ccode, " *)(((char *)__r[%d])+__v[%d].i)", 
		      (int) re->value.ival, (int) re->left);
      (void) fprintf (ccode, ")");
      break;
    case MREFASSIGNT:
      (void) fprintf (ccode, "__rtextassign(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MVALASSIGNT:
      (void) fprintf (ccode, "__rtextvalassign(");
      genvalue (re->left);
      (void) fprintf (ccode, ",");
      genvalue (re->right);
      (void) fprintf (ccode, ")");
      break;
    case MASSIGN:
      if (re->right->token == MASSIGN)
	{
	  genvalue (re->right);
	  (void) fprintf (ccode, ";");
	}
      (void) fprintf (ccode, "(");

      if (re->left->rd->kind == KSIMPLE &&
	  (re->left->rd->categ == CVAR || re->left->rd->categ == CNAME)
	  && (re->left->type == TINTG || re->left->type == TREAL))
	{			/* For aritmetiske, for } h{ndtere
				 * konvertering. */
	  if (re->type == TREAL)
	    (void) fprintf (ccode, "__ev.f");
	  else if (re->type == TINTG)
	    (void) fprintf (ccode, "__ev.i");
	}
      else
	genvalue (re->left);
      (void) fprintf (ccode, "=");
      if (re->left->type == TINTG && re->right->type == TREAL)
	(void) fprintf (ccode, "__rintrea");
      (void) fprintf (ccode, "(");
      if (re->right->token == MASSIGN)
	{
	  if (re->right->left->token == MNAMEADR 
	      || re->right->left->token == MTEXTADR)
	    {
	      if (re->right->left->type == TREAL)
		(void) fprintf (ccode, "__ev.f");
	      else if (re->right->left->type == TINTG)
		(void) fprintf (ccode, "__ev.i");
	      else genvalue (re->right->left);
	    }
	  else
	    genvalue (re->right->left);
	}
      else
	genvalue (re->right);
      (void) fprintf (ccode, "))");
      if (re->left->rd->kind == KSIMPLE)
	{
	  if (re->left->rd->categ == CVAR &&
	      (re->type == TINTG || re->type == TREAL))

	    {
	      /* SKRIVE-AKSESS AV ARIT. VAR-PARAMETER */
	      (void) fprintf (ccode, ";");

	      /* M| sjekke ved runtime om det skal gjores
	       * konvertering. */

	      if (re->type == TINTG)
		{
		  /* To muligheter : ingen, real -> int */
		  (void) fprintf (ccode, "if((__vvp= &");
		  genchainrdencl (re->left, TRUE);
		  (void) fprintf (ccode, "%s)->conv==__NOCONV)",
				  re->left->rd->ident);
		  (void) fprintf 
		    (ccode, 
		     " *(long *)(((char *)__vvp->bp)+__vvp->ofs)=__ev.i;");
		  (void) fprintf 
		    (ccode, 
		     "else *(double *)(((char *)__vvp->bp)+__vvp->ofs)=__ev.i");
		}
	      else
		{		/* Tre muligheter : ingen, int -> real, og 
				 * real ->int ->real */
		  (void) fprintf (ccode, "if((__vvp= &");
		  genchainrdencl (re->left, TRUE);
		  (void) fprintf (ccode, "%s)->conv==__NOCONV)",
				  re->left->rd->ident);
		  (void) fprintf 
		    (ccode, 
		     " *(double *)(((char *)__vvp->bp)+__vvp->ofs)=__ev.f;else ");
		  (void) fprintf (ccode, "if(__vvp->conv==__INTREAL)");
		  (void) fprintf 
		    (ccode, 
		     " *(long *)(((char *)__vvp->bp)+__vvp->ofs)=__ev.f;else ");
		  (void) fprintf 
		    (ccode, 
		     " *(double *)(((char *)__vvp->bp)+__vvp->ofs)=__rintrea(__ev.f)");
		}
	    }
	  else if (re->left->rd->categ == CNAME &&
		   (re->type == TINTG || re->type == TREAL))
	    {
	      /* SKRIVE-AKSESS AV ARIT. NAME-PARAMETER */
	      (void) fprintf (ccode, ";");

	      /* M| sjekke ved runtime om det skal gjores konvertering. */

	      if (re->type == TINTG)
		{		/* To muligheter : ingen, real -> int */
		  (void) fprintf (ccode, "if(");
		  genchainrdencl (re->left, TRUE);
		  (void) fprintf (ccode, "%s.conv==__NOCONV)",
				  re->left->rd->ident);
		  (void) fprintf 
		    (ccode, " *(long *)(((char *)__r[%d])+__v[%d].i)=__ev.i;",
		     (int) re->left->value.ival, (int) re->left->left);
		  (void) fprintf 
		    (ccode, 
		     "else *(double *)(((char *)__r[%d])+__v[%d].i)=__ev.i",
		      (int) re->left->value.ival, (int) re->left->left);
		}
	      else
		{		/* Tre muligheter : ingen, int -> real og
				 * real ->int ->real */
		  (void) fprintf (ccode, "if((__nvp= &");
		  genchainrdencl (re->left, TRUE);
		  (void) fprintf (ccode, "%s)->conv==__NOCONV)",
				  re->left->rd->ident);
		  (void) fprintf 
		    (ccode, 
		     " *(double *)(((char *)__r[%d])+__v[%d].i)=__ev.f;else ",
			  (int) re->left->value.ival, (int) re->left->left);
		  (void) fprintf (ccode, "if(__nvp->conv==__INTREAL)");
		  (void) fprintf 
		     (ccode, 
		      " *(long *)(((char *)__r[%d])+__v[%d].i)=__ev.f;else ",
			  (int) re->left->value.ival, (int) re->left->left);
		  (void) fprintf 
		     (ccode, 
		      " *(double *)(((char *)__r[%d])+__v[%d].i)=__rintrea(__ev.f)",
		       (int) re->left->value.ival, (int) re->left->left);
		}
	    }
	}
      break;
    case MASSIGNR:
      genvalue (re->left);
      (void) fprintf (ccode, "=");
      if ((rex = re->left)->token == MIDENTIFIER && rex->rd->categ == CVAR)
	{
	  /* SKRIVE-AKSESS P} REFERANSE VAR-PARAMETER              M} da
	   * legge inn kode som ,hvis n|dvendig , sjekker om h|yre side er
	   * 'in' strengeste kvalifikasjon p}        aksessveien */

	  (void) fprintf (ccode, "((((__vrp= &");
	  genchainrdencl (rex, TRUE);
	  (void) fprintf (ccode, "%s)->conv==__WRITETEST ", rex->rd->ident);
	  (void) fprintf (ccode, "|| __vrp->conv==__READWRITETEST) &&");
	  (void) fprintf (ccode, " !__rin((__bp= ");
	  genvalue (re->right);
	  (void) fprintf 
	    (ccode, "),__vrp->q))?(__dhp)__rerror(__errqual):(__bp=");
	  genvalue (re->right);
	  (void) fprintf (ccode, "))");
	}
      else
	genvalue (re->right);
      break;
    case MNOOP:
      if (re->type == TTEXT)
	{
	  /* Parantes i forbindelse med tekster. Venstre-siden skal legges p} 
	   * en anonym tekst-variabel. */
	  (void) fprintf (ccode, "__rtextassign(&__et,");
	  genvalue (re->left);
	  (void) fprintf (ccode, ")");
	}
      else
	genvalue (re->left);
      break;
    default:
      (void) putc ('(', ccode);
      if (re->left->type == TCHAR)
	(void) fprintf (ccode, "(unsigned char)");
      genvalue (re->left);
      switch (re->token)
	{
	case MORELSE:
	  (void) fprintf (ccode, "||");
	  break;
	case MANDTHEN:
	  (void) fprintf (ccode, "&&");
	  break;
	case MOR:
	  (void) fprintf (ccode, "|");
	  break;
	case MAND:
	  (void) fprintf (ccode, "&");
	  break;
	case MEQV:
	case MEQ:
	case MEQR:
	  (void) fprintf (ccode, "==");
	  break;
	case MNE:
	case MNER:
	  (void) fprintf (ccode, "!=");
	  break;
	case MLT:
	  (void) fprintf (ccode, "<");
	  break;
	case MLE:
	  (void) fprintf (ccode, "<=");
	  break;
	case MGT:
	  (void) fprintf (ccode, ">");
	  break;
	case MGE:
	  (void) fprintf (ccode, ">=");
	  break;
	case MADD:
	case MADDI:
	  (void) fprintf (ccode, "+");
	  break;
	case MSUB:
	case MSUBI:
	  (void) fprintf (ccode, "-");
	  break;
	case MMUL:
	case MMULI:
	  (void) fprintf (ccode, "*");
	  break;
	case MDIV:
	case MINTDIV:
	  (void) fprintf (ccode, "/");
	  break;
	default:
#ifdef DEBUG
	  (void) fprintf (stderr, "Illegal token:%s\n"
			  ,texttoken (re->token));
#else
	  (void) fprintf (stderr, "Illegal token:%d\n"
			  ,re->token);
#endif
	  break;
	}
      if (re->left->type == TCHAR)
	(void) fprintf (ccode, "(unsigned char)");
      genvalue (re->right);
      (void) putc (')', ccode);
    }
}
