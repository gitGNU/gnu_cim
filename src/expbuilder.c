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

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

int token;

static struct obstack osStack;
static struct obstack osExpr;

/******************************************************************************
								EBUILDERINIT */

ebuilderInit()
{
  obstack_init(&osStack);
  obstack_init(&osExpr);
  lineno= 1;
}

/******************************************************************************
 								      NEWEXP */

struct EXP *newexp()
{
  struct EXP *re;
  re= obstack_alloc (&osExpr, sizeof (struct EXP));
  bzero (re, sizeof (struct EXP));
  re->line= lineno;
  return re;
}

/******************************************************************************
								     MAKEEXP */

struct EXP *makeexp (token, left, right) int token; struct EXP *left, *right;
{
  struct EXP *re;
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

struct EXP * concexp (left, right) struct EXP *left, *right;
{
  if (left==NULL) return right;
  if (right==NULL) return left;
  return makeexp (MSENTCONC, left, right);
}

/******************************************************************************
								 REPLACENODE */

struct EXP *replacenode (rep, token) struct EXP **rep; int token;
{
  struct EXP *rex;
  rex= newexp ();
  if (is_after_dot ((*rep))) *rex= *(*rep)->up;
  else
    *rex= **rep;
  remove_dot (rep);
  if (rex->right) rex->right->up=rex;
  if (rex->left) rex->left->up=rex;
  (*rep)->token= token;
  return (rex);
}

/******************************************************************************
                                                                COPYTREE     */

struct EXP *
copytree (re)
     struct EXP *re;
{
  struct EXP *rex;
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
								      ECLEAN */

static eclean()
{
  void *p;
  p= obstack_finish (&osStack);
  obstack_free (&osStack, p);
}

/******************************************************************************
								       EPUSH */

static epush(re)struct EXP *re;
{
  obstack_ptr_grow (&osStack, re);
}

/******************************************************************************
								        EPOP */

static struct EXP *epop()
{
  struct EXP *re;
  re= * ((struct EXP * *)obstack_next_free (&osStack) - 1);
  obstack_blank (&osStack, - sizeof (void *));
  return (re);
}

/******************************************************************************
								       ELOOK */

struct EXP *elook()
{
  return *((struct EXP * *)obstack_next_free (&osStack) - 1);
}


/******************************************************************************
                                                                   EBUILD    */

/* Bygger opp et uttrykstree, Alle operatorene leses postfix */

ebuild ()
{
  struct EXP *re;

  eclean();
  while (TRUE)
    {
      switch (token)
	{
	case MNOT:
	case MUADD:
	case MUSUB:
	case MNOOP:
	case MQUA:
	  epush(re= makeexp(token, epop(), NULL));
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
	case MLABEL:
	  epush(re= makeexp(token, NULL, NULL));
	  break;
	case MNEWARG:
	case MARGUMENT:
	  epush(re= makeexp(token, NULL, epop()));
	  break;
	case MNEWLINE:
	  lineno++;
	  goto newtoken;
	  break;
	case MFLAG:
	  setFlag ();
	  break;
	default:
	  if (setntokens (token) || token == MERROR || token == MSTOP)
	    {
#ifdef DEBUG
	      if (token == MERROR)
		dumpexp ();
#endif
	      re= elook()->up= makeexp(token, NULL, NULL);
	      re->up= re;
	      return;
	    }
	  /* VANELIG BEHANDLING */
	  {
	    struct EXP *right= epop();
	    epush (re= makeexp (token, epop(), right));
	  }
	}
      if (expridtokens (token))
	re->value.ival= (long) minId();
	  else
	if (token == MTEXTKONST)
	  {
	    re->value.tval.txt= minTval();
	    re->value.tval.id= NOTEXT;
	  }
	    else
	  if (token == MREALKONST)
	    re->value.rval= minRval();
	      else
	    if (exprvaltokens (token))
	      re->value.ival= minIval();
    newtoken:
      token= min();
    }
}


