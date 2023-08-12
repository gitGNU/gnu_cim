/* $Id: $ */

/* Copyright (C) 1998 Sverre Hvammen Johansen,
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
#include "const.h"
#include "builder.h"
#include "checker.h"
#include "expmacros.h"
#include "name.h"
#include "error.h"

static decl_t *absfunction;
static decl_t *absfunctionr;
static decl_t *absfunctioni;
static decl_t *minfunction;
static decl_t *minfunctiont;
static decl_t *minfunctionc;
static decl_t *minfunctionr;
static decl_t *minfunctioni;
static decl_t *maxfunction;
static decl_t *maxfunctiont;
static decl_t *maxfunctionc;
static decl_t *maxfunctionr;
static decl_t *maxfunctioni;
static decl_t *signfunction;
static decl_t *signfunctionr;
static decl_t *signfunctioni;
static decl_t *sourcelinefunction;

static decl_t *varargsint;
static decl_t *varargsreal;
static decl_t *varargschar;
static decl_t *varargstext;
static decl_t *varargsintvar;
static decl_t *varargsrealvar;
static decl_t *varargscharvar;
static decl_t *varargstextvalue;
static decl_t *varargs;

void exp_checker_init_pass2 (void)
{
  absfunction = find_global (tag ("ABS"), TRUE);
  minfunction = find_global (tag ("MIN"), TRUE);
  maxfunction = find_global (tag ("MAX"), TRUE);
  absfunctionr = find_global (tag ("ABS*R"), TRUE);
  absfunctioni = find_global (tag ("ABS*I"), TRUE);
  minfunctiont = find_global (tag ("MIN*T"), TRUE);
  minfunctionc = find_global (tag ("MIN*C"), TRUE);
  minfunctionr = find_global (tag ("MIN*R"), TRUE);
  minfunctioni = find_global (tag ("MIN*I"), TRUE);
  maxfunctiont = find_global (tag ("MAX*T"), TRUE);
  maxfunctionc = find_global (tag ("MAX*C"), TRUE);
  maxfunctionr = find_global (tag ("MAX*R"), TRUE);
  maxfunctioni = find_global (tag ("MAX*I"), TRUE);
  signfunction = find_global (tag ("SIGN"), TRUE);
  signfunctionr = find_global (tag ("SIGN*R"), TRUE);
  signfunctioni = find_global (tag ("SIGN*I"), TRUE);
  sourcelinefunction = find_global (tag ("SOURCELINE"), TRUE);

  varargsint = new_decl ();
  varargsint->type = TINTG;
  varargsint->kind = KSIMPLE;
  varargsint->categ = CDEFLT;

  varargsreal = new_decl ();
  varargsreal->type = TREAL;
  varargsreal->kind = KSIMPLE;
  varargsreal->categ = CDEFLT;

  varargschar = new_decl ();
  varargschar->type = TCHAR;
  varargschar->kind = KSIMPLE;
  varargschar->categ = CDEFLT;

  varargstext = new_decl ();
  varargstext->type = TTEXT;
  varargstext->kind = KSIMPLE;
  varargstext->categ = CDEFLT;

  varargsintvar = new_decl ();
  varargsintvar->type = TINTG;
  varargsintvar->kind = KSIMPLE;
  varargsintvar->categ = CVAR;

  varargsrealvar = new_decl ();
  varargsrealvar->type = TREAL;
  varargsrealvar->kind = KSIMPLE;
  varargsrealvar->categ = CVAR;

  varargscharvar = new_decl ();
  varargscharvar->type = TCHAR;
  varargscharvar->kind = KSIMPLE;
  varargscharvar->categ = CVAR;

  varargstextvalue = new_decl ();
  varargstextvalue->type = TTEXT;
  varargstextvalue->kind = KSIMPLE;
  varargstextvalue->categ = CVALUE;

  varargs = new_decl ();
  varargs->type = TNOTY;
  varargs->kind = KSIMPLE;
  varargs->categ = CDEFLT;

}

/******************************************************************************
                                                                      SERROR */

#define SERROR(melding) sserror(melding,re)

static void sserror (int melding, exp_t *re)
{
  if (RD && RD->categ == CNEW)
    {
      serror (melding, RD->ident, 0);
      RD->categ = CERROR;
    }
  else if (LEFT && LEFTRD && LEFTRD->categ == CNEW)
    {
      serror (melding, LEFTRD->ident, 0);
      LEFTRD->categ = CERROR;
    }
  else if (RIGHT && RIGHTRD && RIGHTRD->categ == CNEW)
    {
      serror (melding, RIGHTRD->ident, 0);
      RIGHTRD->categ = CERROR;
    }
  else if (QUAL && QUAL->categ == CNEW)
    {
      serror (melding, QUAL->ident, 0);
      QUAL->categ = CERROR;
    }
  else if ((LEFT ? LEFTTYPE != TERROR : TRUE)
	   && (RIGHT ? RIGHTTYPE != TERROR : TRUE) &&
       (UP ? UPTYPE != TERROR : TRUE) && (RD ? RD->type != TERROR : TRUE) &&
	   (QUAL ? QUAL->type != TERROR : TRUE)
	   && (UPRD ? UPRD->type != TERROR : TRUE) &&
	   (TYPE != TERROR))
    serror (melding, RD ? RD->ident : 0, 0);
  TYPE = TERROR;
}


/******************************************************************************
                                                                KONVTYPE     */

/*VARARGS2 */
static void konvtype (exp_t **re, char type, decl_t *qual)
{
  exp_t *rex;
  decl_t *rd;
  if (((*re)->type == TINTG && type == TREAL)
      || ((*re)->type == TREAL && type == TINTG))
    {
      rex = newexp();
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
	    serror (85, "", (*re)->up->token);
	  (*re)->type = (*re)->up->type = TERROR;
	}
      else if ((rd = commonqual ((*re)->qual, qual)) == qual) /* OK */ ;
      else if (rd == (*re)->qual && (*re)->token != MNEWARG)
	{
	  rex = newexp();
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
	    serror (85, "", (*re)->up->token);
	  (*re)->type = (*re)->up->type = TERROR;
	}
    }
}

/******************************************************************************
                                                                SAMETYPE     */

static void sametype (exp_t **rel, exp_t **rer)
{
  if ((*rel)->type == TINTG && (*rer)->type == TREAL)
    konvtype (rel, TREAL, NULL);
  else if ((*rel)->type == TREAL && (*rer)->type == TINTG)
    konvtype (rer, TREAL, NULL);
}

/******************************************************************************
                                                               ARGUMENTERROR */

static void argumenterror (int melding, exp_t *re)
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

static void set_param (exp_t *re)
{
  re->right->rd = first_param (re->rd);
  {
    exp_t *rex;
    for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
      {
	if (more_param (rex->rd) == FALSE)
	  argumenterror (102, rex);
	rex->right->rd = next_param (rex->rd);
      }
    if (more_param (rex->rd) == TRUE)
      argumenterror (102, rex);
  }
}

/******************************************************************************
                                                                EXP_CHECK    */

static void exp_check (exp_t *re)
{

  /* Sjekker først at nodene har riktige typer */
  switch (TOKEN)
    {
    case MFOR:
    case MFORR:
      exp_check (LEFT);
      TYPE = LEFTTYPE;
      QUAL = LEFTQUAL;
      exp_check (RIGHT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
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
      exp_check (LEFT);
      TYPE = LEFTTYPE;
      exp_check (RIGHT);
      TYPE = RIGHTTYPE;
      break;
    case MARRAY:
      exp_check (LEFT);
      localused = 0;
      TYPE = TINTG;
      exp_check (RIGHT);
      if (localused)
	SERROR (89);
      break;
    case MARRAYSEP:
      /* Forutsetter at venstre node er en identifier */
      exp_check (RIGHT);
      LEFTRD = find_global (LEFTVALUE.ident, FALSE);
      if (LEFTRD->categ == CNEW)
	SERROR (75);
      LEFTTYPE = LEFTRD->type;
      break;
    case MASSIGN:
    case MASSIGNR:
      exp_check (LEFT);
      exp_check (RIGHT);
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
      RD = find_global (VALUE.ident, TRUE);
      break;
    case MIFE:
      exp_check (LEFT);
      exp_check (RIGHT);
      if (LEFTTYPE != TBOOL)
	SERROR (77);
      else
	TYPE = RIGHTTYPE;
      QUAL = RIGHTQUAL;
      break;
    case MELSEE:
      exp_check (LEFT);
      exp_check (RIGHT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
      if (LEFTTYPE != TTEXT || RIGHTTYPE != TTEXT)
	SERROR (109);
      else
	TYPE = TTEXT;
      break;
    case MORELSEE:
    case MANDTHENE:
    case MEQV:
    case MIMP:
    case MOR:
    case MAND:
      exp_check (LEFT);
      exp_check (RIGHT);
      if (LEFTTYPE != TBOOL || RIGHTTYPE != TBOOL)
	SERROR (95);
      else
	TYPE = TBOOL;
      break;
    case MNOT:
      exp_check (LEFT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
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
      exp_check (LEFT);
      RIGHTRD = RD = find_global (VALUE.ident, FALSE);
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
      exp_check (LEFT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
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
      exp_check (LEFT);
      exp_check (RIGHT);
      konvtype (&LEFT, TYPE, NULL);
      konvtype (&RIGHT, TYPE, NULL);
      if (LEFTTYPE != TYPE || RIGHTTYPE != TYPE)
	SERROR (97);
      break;
    case MPRIMARY:
      exp_check (LEFT);
      exp_check (RIGHT);
      TYPE = TREAL;
      if ((LEFTTYPE == TINTG || LEFTTYPE == TREAL) &&
	  RIGHTTYPE == TREAL)
	konvtype (&LEFT, TREAL, NULL);
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
      exp_check (LEFT);
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
	RD = find_local (VALUE.ident, UPQUAL, TRUE);
      else if (ISLEFT && (UPTOKEN == MASSIGN | UPTOKEN == MASSIGNR))
	RD = find_global (VALUE.ident, FALSE);	/* Tilordning av
						 * funksjons proc. */
      else if (UPTOKEN == MWHEN)
	RD = find_global (VALUE.ident, FALSE);
      else
	RD = find_global (VALUE.ident, TRUE);
      if (RD->categ == CNEW)
	SERROR (75);
      SEENTHROUGH = seenthrough;
      TYPE = RD->type;
      QUAL = RD->prefqual;
      if (RD->type == TERROR)
	SERROR (106);
      if (TYPE == TLABEL && seenthrough != NULL)
	SERROR (8);
      if (UPTOKEN == MWHEN)
	{
	  if (RD->kind != KCLASS)
	    {
	      if (RD->kind != KERROR)
		serror (84, "", 0);
	    }
	} else
      if (RD == sourcelinefunction)
	{
	  TOKEN = MINTEGERKONST;
	  TYPE = TINTG;
	  VALUE.ival = re->line;
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
	  else if (more_param (first_param (RD)) == TRUE)
	    SERROR (107);
	  else if (RD->kind == KPROC)
	    TOKEN = MPROCARG;
	  else
	    TOKEN = MARGUMENT;
	  RIGHT = newexp();
	  RIGHTTOKEN = MENDSEP;
	  RIGHTVALUE.rval = 0.0;
	  RIGHTRIGHT = NULL;
	  RIGHTLEFT = NULL;
	  RIGHTRD = NULL;
	  RIGHTQUAL = NULL;
	  RIGHT->up = re;
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
		SERROR (90);
	    }
	  else
	    SERROR (7);
	}
      break;
    case MTHIS:
      RD = reg_this (VALUE.ident);
      if (RD->categ == CNEW)
	SERROR (75);
      TYPE = TREF;
      QUAL = RD;
      SEENTHROUGH = seenthrough;
      if (RD->kind != KCLASS)
	SERROR (99);
      break;
    case MDOT:
      exp_check (LEFT);
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
      exp_check (RIGHT);
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
      RD = find_global (VALUE.ident, FALSE);
      if (RD->categ == CNEW)
	SERROR (75);
      SEENTHROUGH = seenthrough;
      TYPE = TREF;
      QUAL = RD;
      if (RD->kind != KCLASS)
	SERROR (99);
      set_param (re);
      exp_check (RIGHT);
      if (RIGHTTYPE == TERROR)
	TYPE = TERROR;
      break;
    case MQUA:
      exp_check (LEFT);
      QUAL = find_global (VALUE.ident, FALSE);
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
	RD = find_local (VALUE.ident, UPQUAL, TRUE);
      else
	RD = find_global (VALUE.ident, TRUE);
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
      exp_check (RIGHT);
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
	  exp_check (RIGHT);
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
	  exp_check (RIGHT);
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
	  exp_check (RIGHT);
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
	  exp_check (RIGHT);
	  if (RIGHTTYPE == TERROR)
	    TYPE = TERROR;
	}
      break;
    case MARGUMENTSEP:
      TYPE = RD->type;
      QUAL = RD->prefqual;
      if (RD->type == TVARARGS)
	{
	  decl_t *rdx;
	  exp_check (LEFT);
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
	      exp_check (LEFT->left);
	      LEFTRIGHTQUAL = LEFTQUAL = LEFTLEFTQUAL;
	      LEFTSEENTHROUGH = LEFTLEFTSEENTHROUGH;
	      LEFTRIGHTRD = LEFTRD =
		find_local (LEFTRIGHTVALUE.ident, LEFTQUAL, TRUE);
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
		  if (!same_param (RD->descr, LEFTRD->descr))
		    argumenterror (112, re);
		}
	    }
	  else if (LEFTTOKEN != MIDENTIFIER)
	    {
	      argumenterror (103, re);
	      exp_check (LEFT);
	    }
	  else
	    {
	      LEFTRD = find_global (LEFTVALUE.ident, TRUE);
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
		  if (!same_param (RD->descr, LEFTRD->descr))
		    argumenterror (112, re);
		}
	    }
	}
      else if (RD->kind == KSIMPLE)
	{			/* Parameteren skal v{re simple */
	  exp_check (LEFT);
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
	  exp_check (LEFT);
	}
      exp_check (RIGHT);
      if (RIGHTTYPE == TERROR)
	TYPE = TERROR;
      break;
    case MENDSEP:
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
    default:
      break;
    }
}

/******************************************************************************
                                                           MAIN_EXP_CHECK      */

void main_exp_check (exp_t *re)
{
  exp_check (re);
  computeconst (re);
  setdanger_const (re);
}
