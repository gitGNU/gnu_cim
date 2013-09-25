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
#include <obstack.h>

#include "const.h"
#include "mellbuilder.h"
#include "builder.h"
#include "salloc.h"

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

int token;

static struct obstack os_stack;

/******************************************************************************
								EBUILDERINIT */

void ebuilder_init (void)
{
  obstack_init(&os_stack);
}



void ebuilder_init_pass2 (void)
{
  lineno=1;
}

/******************************************************************************
 								      NEWEXP */

exp_t *newexp(void)
{
  exp_t *re;
  re= (exp_t *) salloc (sizeof (exp_t));
  re->line= lineno;
  return re;
}

/******************************************************************************
								     MAKEEXP */

exp_t *makeexp (int token, exp_t *left, exp_t *right)
{
  exp_t *re;
  re= newexp();

  re->token= token;

  re->left= left;
  if (left) left->up= re;
  re->right= right;
  if (right) right->up= re;

  return (re);
}

/******************************************************************************
								     CONCEXP */

exp_t *concexp (exp_t *left, exp_t *right)
{
  if (left==NULL) return right;
  if (right==NULL) return left;
  return makeexp (MSENTCONC, left, right);
}

/******************************************************************************
								 REPLACENODE */

exp_t *replacenode (exp_t **rep, int token)
{
  exp_t *rex;
  rex= newexp ();
  if (is_after_dot ((*rep))) *rex= *(*rep)->up;
  else
    *rex= **rep;
  remove_dot (rep);
  if (rex->right) rex->right->up=rex;
  if (rex->left) rex->left->up=rex;
  (*rep)->right= (*rep)->left= NULL;
  (*rep)->token= token;
  return (rex);
}

/******************************************************************************
                                                                COPYTREE     */

exp_t *copytree (exp_t *re)
{
  exp_t *rex;
  if (re == NULL) return NULL;
  rex = newexp();
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
  rex->line = re->line;
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
								  REMOVE_DOT */

void remove_dot (exp_t **rep)
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
								      ECLEAN */

static void eclean(void )
{
  void *p;
  p= obstack_finish (&os_stack);
  obstack_free (&os_stack, p);
}

/******************************************************************************
								       EPUSH */

static epush(exp_t *re)
{
  obstack_grow (&os_stack, &re, sizeof (void *));
}

/******************************************************************************
								        EPOP */

static exp_t *epop(void)
{
  exp_t *re;
  re= * ((exp_t * *)obstack_next_free (&os_stack) - 1);
  obstack_blank (&os_stack, - sizeof (void *));
  return (re);
}

/******************************************************************************
								       ELOOK */

exp_t *elook(void)
{
  return *((exp_t * *)obstack_next_free (&os_stack) - 1);
}


/******************************************************************************
                                                                   EBUILD    */

/* Bygger opp et uttrykstree, Alle operatorene leses postfix */

void ebuild (void)
{
  exp_t *re;

  eclean();
  while (TRUE)
    {
      switch (token)
	{
	case MNOT:
	case MUADD:
	case MUSUB:
	case MNOOP:
	  epush(re= makeexp(token, epop(), NULL));
	  break;
	case MQUA:
	  epush(re= makeexp(token, epop(), NULL));
	  re->value.ival= (long) min_id();
	  break;
	case MIDENTIFIER:
	case MTHIS:
	case MLABEL:
	  epush(re= makeexp(token, NULL, NULL));
	  re->value.ival= (long) min_id();
	  break;
	case MTEXTKONST:
	  epush(re= makeexp(token, NULL, NULL));
	  re->value.tval.txt= min_tval();
	  re->value.tval.id= NOTEXT;
	  break;
	case MREALKONST:
	  epush(re= makeexp(token, NULL, NULL));
	  re->value.rval= min_rval();
	  break;
	case MCHARACTERKONST:
	case MINTEGERKONST:
	case MBOOLEANKONST:
	  epush(re= makeexp(token, NULL, NULL));
	  re->value.ival= min_ival();
	  break;
	case MENDSEP:
	case MNONE:
	  epush(re= makeexp(token, NULL, NULL));
	  break;
	case MNEWARG:
	case MARGUMENT:
	  epush(re= makeexp(token, NULL, epop()));
	  re->value.ival= (long) min_id();
	  break;
	case MNEWLINE:
	  lineno++;
	  break;
	case MFLAG:
	  set_flag ();
	  break;
	case MERROR:
	case MSTOP:
	case MBLOCK:
	case MENDBLOCK:
	case MPRBLOCK:
	case MENDPRBLOCK:
	case MPROCEDURE:
	case MENDPROCEDURE:
	case MCLASS:
	case MENDCLASS:
	case MINSPECT:
	case MENDINSPECT:
	case MDO:
	case MENDDO:
	case MWHEN:
	case MENDWHEN:
	case MOTHERWISE:
	case MENDOTHERWISE:
	case MFORDO:
	case MENDFOR:
	case MWHILE:
	case MENDWHILE:
	case MIF:
	case MELSE:
	case MENDIF:
	case MGOTO:
	case MINNER:
	case MENDSWITCH:
	case MENDASSIGN:
	case MENDARRAY:
	case MENDLABEL:
	case MCONST:
	  re= elook()->up= makeexp(token, NULL, NULL);
	  re->up= re;
	  return;
	default:
	  {
	    exp_t *right= epop();
	    epush (re= makeexp (token, epop(), right));
	  }
	}
      token= min();
    }
}



