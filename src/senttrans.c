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

static void sort_proc_arr (sent_t *parent_sent)
{
  int line= parent_sent->line;
  sent_t *proc=NULL, *sent, *nextsent, *new;
  for (sent= parent_sent->first; sent!=NULL ;sent= nextsent)
    {
      nextsent= sent->next;
      switch (sent->token)
	{
	case MCONST:
	  remove_sent (parent_sent, sent);
	  break;
	case MENDSWITCH:
	case MPROCEDURE:
	case MCLASS:
	  remove_sent (parent_sent, sent);
	  insert_after_sent (parent_sent, proc, sent);
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
  switch (parent_sent->token)
    {
    case MBLOCK:
      new= new_sent (MBLOCKENTRY);
      break;
    case MPROCEDURE:
      new= new_sent (MPROCEDUREENTRY);
      break;
    default:
      new= new_sent (MENTRY);
      break;
    }

  new->cblock= parent_sent->cblock;
  insert_after_sent (parent_sent, proc, new);
  if (parent_sent->token==MCLASS || parent_sent->token==MPRBLOCK)
    {
      new= new_sent (MENDDECL);
      new->cblock= parent_sent->cblock;
      insert_before_sent (parent_sent, sent, new);

      if (parent_sent->token==MCLASS && !cblock->inner)
	{
	  new= new_sent (MINNER);
	  new->cblock= parent_sent->cblock;
	  insert_before_sent (parent_sent, NULL, new);
	}
    }
}

/******************************************************************************
                                                               SENTLISTTRANS */

static void sent_list_trans (sent_t *parent_sent)
{
  sent_t *sent;

  for (sent= parent_sent->first; sent!=NULL; sent= sent->next)
    {
      lineno= sent->line;
      nonetest= sent->nonetest;
      indextest= sent->indextest;
      stripsideeffects= sent->stripsideeffects;
      sent_trans (sent);
      cblock= parent_sent->cblock;
      if (cblock) cblev= cblock->blev;
    }
  lineno= parent_sent->last_line;
}

/******************************************************************************
                                                                 MODULETRANS */

static void module_trans (sent_t *sent)
{     
  if (! separat_comp)
    insert_before_sent (sent, NULL, new_sent (MGOTOSTOP));
  sent_list_trans (sent);
}

/******************************************************************************
                                                                  BLOCKTRANS */

static void block_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sort_proc_arr (sent);
  sent_list_trans (sent);
}

/******************************************************************************
                                                                PRBLOCKTRANS */

static void prblock_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sort_proc_arr (sent);
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1);
  sent_list_trans (sent);
}

/******************************************************************************
                                                              PROCEDURETRANS */

static void procedure_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sort_proc_arr (sent);
  sent_list_trans (sent);
}

/******************************************************************************
                                                                  CLASSTRANS */

static void class_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sort_proc_arr (sent);
  sent_list_trans (sent);
}

/******************************************************************************
                                                                INSPECTTRANS */

static void inspect_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1); 
  sent_list_trans (sent);
}

/******************************************************************************
                                                                     DOTRANS */

static void do_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sent_list_trans (sent);
}

/******************************************************************************
                                                                   WHENTRANS */

static void when_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sent_list_trans (sent);
}

/******************************************************************************
                                                              OTHERWISETRANS */

static void otherwise_trans (sent_t *sent)
{
  cblock= sent->cblock;
  sent_list_trans (sent);
}

/******************************************************************************
                                                                FORELEMTRANS */

static void forelem_trans (exp_t *re, exp_t *rex)
{
  exp_t *rey, *red;
  exp_t *reinit, *retest, *restep;

  int konst_step, notnegativ;

  rey= rex->left;
  switch (rey->token)
    {
    case MSTEP:
      notnegativ = !(rey->right->left->token == MUSUBI ||
		     (rey->right->left->token == MINTEGERKONST &&
		      rey->right->left->value.ival < 0));
      konst_step = rey->right->left->token == MINTEGERKONST;

      restep= transcall (rey->right, rey->right->left, 1, 1, 1);
      restep= concexp (restep, transcall (rey->right, rey->right->right, 
					  1, 1, 1));

      reinit= transcall (rey, rey->left, 1, 1, 1);
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
      rey->left->up= rey;
      rey->right->left= retest;
      rey->right->left->up= rey->right;
      rey->right->right= restep;
      rey->right->right->up= rey->right;
      break;
    case MFORWHILE:
      restep= transcall (rey, rey->left, 1, 1, 1);
      restep= concexp (restep, makeexp (re->left->type==TTEXT?re->token==MFOR?
					MVALASSIGNT:MREFASSIGNT:MASSIGND, 
					copytree (re->left),rey->left));
      restep= concexp (restep, transcall (rey, rey->right, 1, 1, 1));
      rey->left= restep;
      break;
    default:
      restep= transcall (rex, rey, 1, 1, 1);
      restep= concexp (restep, makeexp (re->left->type==TTEXT?re->token==MFOR?
					MVALASSIGNT:MREFASSIGNT:MASSIGND, 
					copytree (re->left),rey));
      rex->left= restep;
      break;
    }
}

/******************************************************************************
                                                                  FORDOTRANS */

static void fordo_trans (sent_t *sent)
{
  exp_t *re, *rex;
  cblock= sent->cblock;
  re= sent->exp;
  for (rex = re->right; rex->token != MENDSEP; rex = rex->right)
    forelem_trans (re, rex);

  sent_list_trans (sent);
}

/******************************************************************************
                                                                  WHILETRANS */

static void while_trans (sent_t *sent)
{
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1); 
  sent_list_trans (sent);
}

/******************************************************************************
                                                                     IFTRANS */

static void if_trans (sent_t *sent)
{
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1);
  sent_list_trans (sent);
}

/******************************************************************************
                                                                   ELSETRANS */

static void else_trans (sent_t *sent)
{
  sent_list_trans (sent);
}

/******************************************************************************
                                                                   THENTRANS */

static void then_trans (sent_t *sent)
{
  sent_list_trans (sent);
}

/******************************************************************************
                                                                   GOTOTRANS */

static void goto_trans (sent_t *sent)
{
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1);
}

/******************************************************************************
                                                                  INNERTRANS */

static void inner_trans (sent_t *sent)
{
}

/******************************************************************************
                                                              ENDSWITCHTRANS */

static void endswitch_trans (sent_t *sent)
{
  exp_t *rex, *rey;
  for (rex = sent->exp->right; rex->token != MENDSEP; rex = rex->right)
    {
      rey= transcall (rex, rex->left, 1, 1, 1); fprintf (ccode, ";");
      if (rey!=NULL)
	{
	  rex->left= makeexp (MSENTCONC, rey, rex->left);
	  rex->left->up= rex;
	}
    }
}

/******************************************************************************
                                                              ENDASSIGNTRANS */

static void endassign_trans (sent_t *sent)
{
  sent->iexp= transcall (sent->exp->up, sent->exp, 1, 1, 1);
}

/******************************************************************************
                                                               ENDARRAYTRANS */

static void endarray_trans (sent_t *sent)
{
  sent->iexp= transcall (sent->exp, sent->exp->right, 1, 1, 1);
}

/******************************************************************************
                                                               ENDLABELTRANS */

static void endlabel_trans (sent_t *sent)
{
}

/******************************************************************************
                                                               GOTOSTOPTRANS */

static void goto_stop_trans (sent_t *sent)
{
}

/******************************************************************************
                                                                  THUNKTRANS */

static void thunk_trans (sent_t *sent)
{
  cblock= sent->cblock;
  inthunk= sent->exp->value.thunk.inthunk;
  sent->iexp= transcall (sent->exp, sent->exp->left, 1, 1, 1);
  inthunk= 0;
}

/******************************************************************************
                                                                   SENTTRANS */

void sent_trans (sent_t *sent)
{
  switch (sent->token)
    {
    case MMODULE:
      module_trans (sent);
      break;
    case MCONST:
      break;
    case MBLOCK:
      block_trans (sent);
      break;
    case MPRBLOCK:
      prblock_trans (sent);
      break;
    case MPROCEDURE:
      procedure_trans (sent);
      break;
    case MCLASS:
      class_trans (sent);
      break;
    case MINSPECT:
      inspect_trans (sent);
      break;
    case MDO:
      do_trans (sent);
      break;
    case MWHEN:
      when_trans (sent);
      break;
    case MOTHERWISE:
      otherwise_trans (sent);
      break;
    case MFORDO:
      fordo_trans (sent);
      break;
    case MWHILE:
      while_trans (sent);
      break;
    case MIF:
      if_trans (sent);
      break;
    case MELSE:
      else_trans (sent);
      break;
    case MTHEN:
      then_trans (sent);
      break;
    case MGOTO:
      goto_trans (sent);
      break;
    case MINNER:
      inner_trans (sent);
      break;
    case MENDSWITCH:
      endswitch_trans (sent);
      break;
    case MENDASSIGN:
      endassign_trans (sent);
      break;
    case MENDARRAY:
      endarray_trans (sent);
      break;
    case MENDLABEL:
      endlabel_trans (sent);
      break;
    case MGOTOSTOP:
      goto_stop_trans (sent);
      break;
    case MTHUNKSIMPLEADDRESS:
    case MTHUNKSIMPLEVALUE:
    case MTHUNKLABLE:
    case MTHUNKARRAY:
    case MTHUNKPROCEDURE:
      thunk_trans (sent);
      break;
    }
}
