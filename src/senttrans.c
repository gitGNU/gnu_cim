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
#include "gen.h"
#include "trans.h"

/******************************************************************************
                                                                 SORTPROCARR */

static void sortProcArr (parentSent) struct SENT *parentSent;
{
  int line= parentSent->line;
  struct SENT *proc=NULL, *sent, *nextsent, *new;
  for (sent= parentSent->first; sent!=NULL ;sent= nextsent)
    {
      nextsent= sent->next;
      switch (sent->token)
	{
	case MCONST:
	  removeSent (parentSent, sent);
	  break;
	case MENDSWITCH:
	case MPROCEDURE:
	case MCLASS:
	  removeSent (parentSent, sent);
	  insertAfterSent (parentSent, proc, sent);
	  proc= sent;
	  break;
	case MENDARRAY:
	  line= sent->line;
	  break;
	default:
	  goto exit;
	}
    }
 exit:
  switch (parentSent->token)
    {
    case MBLOCK:
      new= newSent (MBLOCKENTRY);
      break;
    case MPROCEDURE:
      new= newSent (MPROCEDUREENTRY);
      break;
    default:
      new= newSent (MENTRY);
      break;
    }

  new->cblock= parentSent->cblock;
  insertAfterSent (parentSent, proc, new);
  if (parentSent->token==MCLASS || parentSent->token==MPRBLOCK)
    {
      new= newSent (MENDDECL);
      new->cblock= parentSent->cblock;
      insertBeforeSent (parentSent, sent, new);

      if (parentSent->token==MCLASS)
	{
	  for (; sent!=NULL ;sent=sent->next)
	    {
	      line= sent->line;
	      switch (sent->token)
		{
		case MINNER:
		  return;
		}
	    }
	  new= newSent (MINNER);
	  new->cblock= parentSent->cblock;
	  insertBeforeSent (parentSent, NULL, new);
	}
    }
}

/******************************************************************************
                                                               SENTLISTTRANS */

static void sentListTrans (parentSent) struct SENT *parentSent;
{
  struct SENT *sent;

  for (sent= parentSent->first; sent!=NULL; sent= sent->next)
    {
      lineno= sent->line;
      nonetest= sent->nonetest;
      indextest= sent->indextest;
      stripsideeffects= sent->stripsideeffects;
      sentTrans (sent);
      cblock= parentSent->cblock;
      if (cblock) cblev= cblock->blev;
    }
  lineno= parentSent->lastLine;
}

/******************************************************************************
                                                                 MODULETRANS */

static void moduleTrans (sent) struct SENT *sent;
{     
  if (! separat_comp)
    insertBeforeSent (sent, NULL, newSent (MGOTOSTOP));
  sentListTrans (sent);
}

/******************************************************************************
                                                                  BLOCKTRANS */

static void blockTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sortProcArr (sent);
  sentListTrans (sent);
}

/******************************************************************************
                                                                PRBLOCKTRANS */

static void prblockTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sortProcArr (sent);
  sent->iexp= transcall (sent->exp->up, sent->exp);
  sentListTrans (sent);
}

/******************************************************************************
                                                              PROCEDURETRANS */

static void procedureTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sortProcArr (sent);
  sentListTrans (sent);
}

/******************************************************************************
                                                                  CLASSTRANS */

static void classTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sortProcArr (sent);
  sentListTrans (sent);
}

/******************************************************************************
                                                                INSPECTTRANS */

static void inspectTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sent->iexp= transcall (sent->exp->up, sent->exp); 
  sentListTrans (sent);
}

/******************************************************************************
                                                                     DOTRANS */

static void doTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sentListTrans (sent);
}

/******************************************************************************
                                                                   WHENTRANS */

static void whenTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sentListTrans (sent);
}

/******************************************************************************
                                                              OTHERWISETRANS */

static void otherwiseTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  sentListTrans (sent);
}

/******************************************************************************
                                                                FORELEMTRANS */

static void forelemTrans (re, rex) struct EXP *re, *rex;
{
  struct EXP *rey, *red;
  struct EXP *reinit, *retest, *restep;

  int konst_step, notnegativ;

  rey= rex->left;
  switch (rey->token)
    {
    case MSTEP:
      notnegativ = !(rey->right->left->token == MUSUBI ||
		     (rey->right->left->token == MINTEGERKONST &&
		      rey->right->left->value.ival < 0));
      konst_step = rey->right->left->token == MINTEGERKONST;

      restep= transcall (rey->right, rey->right->left);
      restep= concexp (restep, transcall (rey->right, rey->right->right));

      reinit= transcall (rey, rey->left);
      reinit= concexp (reinit, red= makeexp(MASSIGND,copytree (re->left), 
				       rey->left));
      red->type= red->right->type;
      reinit= concexp (reinit, copytree (restep));

      retest= makeexp (re->type==TINTG?rey->right->right->type==TREAL?
		    MSIGNDX:MSIGNDI:MSIGNDR,
		    copytree (re->left), rey->right->right);
      if (!konst_step)
	retest= makeexp (MMUL, (notnegativ?rey->right->left:
				rey->right->left->left), retest);
      retest= makeexp (notnegativ?MLE:MGE, retest, 
		    makeexp (MINTEGERKONST, NULL, NULL));

      restep= concexp (restep, red= makeexp(MASSIGNADD,copytree (re->left), 
				       rey->right->left));
      red->type= red->right->type;

      rey->left= reinit;
      rey->right->left= retest;
      rey->right->right= restep;
      break;
    case MFORWHILE:
      restep= transcall (rey, rey->left);
      restep= concexp (restep, makeexp (re->left->type==TTEXT?re->token==MFOR?
					MVALASSIGNT:MREFASSIGNT:MASSIGND, 
					copytree (re->left),rey->left));
      restep= concexp (restep, transcall (rey, rey->right));
      rey->left= restep;
      break;
    default:
      restep= transcall (rex, rey);
      restep= concexp (restep, makeexp (re->left->type==TTEXT?re->token==MFOR?
					MVALASSIGNT:MREFASSIGNT:MASSIGND, 
					copytree (re->left),rey));
      rex->left= restep;
      break;
    }
}

/******************************************************************************
                                                                  FORDOTRANS */

static void fordoTrans (sent) struct SENT *sent;
{
  struct EXP *re, *rex;
  cblock= sent->cblock;
  re= sent->exp;
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    forelemTrans (re, rex);

  sentListTrans (sent);
}

/******************************************************************************
                                                                  WHILETRANS */

static void whileTrans (sent) struct SENT *sent;
{
  sent->iexp= transcall (sent->exp->up, sent->exp); 
  sentListTrans (sent);
}

/******************************************************************************
                                                                     IFTRANS */

static void ifTrans (sent) struct SENT *sent;
{
  sent->iexp= transcall (sent->exp->up, sent->exp);
  sentListTrans (sent);
}

/******************************************************************************
                                                                   ELSETRANS */

static void elseTrans (sent) struct SENT *sent;
{
  sentListTrans (sent);
}

/******************************************************************************
                                                                   THENTRANS */

static void thenTrans (sent) struct SENT *sent;
{
  sentListTrans (sent);
}

/******************************************************************************
                                                                   GOTOTRANS */

static void gotoTrans (sent) struct SENT *sent;
{
  sent->iexp= transcall (sent->exp->up, sent->exp);
}

/******************************************************************************
                                                                  INNERTRANS */

static void innerTrans (sent) struct SENT *sent;
{
}

/******************************************************************************
                                                              ENDSWITCHTRANS */

static void endswitchTrans (sent) struct SENT *sent;
{
#if 1
  struct EXP *rex, *rey;
  for (rex = sent->exp->right; rex->token != MENDSEP; rex = rex->right)
    {
      rey= transcall (rex, rex->left); fprintf (ccode, ";");
      if (rey!=NULL)
	{
	  rex->left= makeexp (MSENTCONC, rey, rex->left);
	  rex->left->up= rex;
	}
    }
#endif
}

/******************************************************************************
                                                              ENDASSIGNTRANS */

static void endassignTrans (sent) struct SENT *sent;
{
  sent->iexp= transcall (sent->exp->up, sent->exp);
}

/******************************************************************************
                                                               ENDARRAYTRANS */

static void endarrayTrans (sent) struct SENT *sent;
{
  sent->iexp= transcall (sent->exp, sent->exp->right);
}

/******************************************************************************
                                                               ENDLABELTRANS */

static void endlabelTrans (sent) struct SENT *sent;
{
}

/******************************************************************************
                                                               GOTOSTOPTRANS */

gotoStopTrans (sent) struct SENT *sent;
{
}

/******************************************************************************
                                                                  THUNKTRANS */

thunkTrans (sent) struct SENT *sent;
{
  cblock= sent->cblock;
  inthunk= sent->exp->value.thunk.inthunk;
#if 1
  sent->iexp= transcall (sent->exp, sent->exp->left);
#endif
  inthunk= 0;
}

/******************************************************************************
                                                                   SENTTRANS */

void sentTrans (sent) struct SENT *sent;
{
  switch (sent->token)
    {
    case MMODULE:
      moduleTrans (sent);
      break;
    case MCONST:
      break;
    case MBLOCK:
      blockTrans (sent);
      break;
    case MPRBLOCK:
      prblockTrans (sent);
      break;
    case MPROCEDURE:
      procedureTrans (sent);
      break;
    case MCLASS:
      classTrans (sent);
      break;
    case MINSPECT:
      inspectTrans (sent);
      break;
    case MDO:
      doTrans (sent);
      break;
    case MWHEN:
      whenTrans (sent);
      break;
    case MOTHERWISE:
      otherwiseTrans (sent);
      break;
    case MFORDO:
      fordoTrans (sent);
      break;
    case MWHILE:
      whileTrans (sent);
      break;
    case MIF:
      ifTrans (sent);
      break;
    case MELSE:
      elseTrans (sent);
      break;
    case MTHEN:
      thenTrans (sent);
      break;
    case MGOTO:
      gotoTrans (sent);
      break;
    case MINNER:
      innerTrans (sent);
      break;
    case MENDSWITCH:
      endswitchTrans (sent);
      break;
    case MENDASSIGN:
      endassignTrans (sent);
      break;
    case MENDARRAY:
      endarrayTrans (sent);
      break;
    case MENDLABEL:
      endlabelTrans (sent);
      break;
    case MGOTOSTOP:
      gotoStopTrans (sent);
      break;
    case MTHUNKSIMPLEADDRESS:
    case MTHUNKSIMPLEVALUE:
    case MTHUNKLABLE:
    case MTHUNKARRAY:
    case MTHUNKPROCEDURE:
      thunkTrans (sent);
      break;
    }
}
