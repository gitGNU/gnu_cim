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

#include "gen.h"

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack osStack;
static struct obstack osSent;

struct SENT module;

/******************************************************************************
                                                                       SPUSH */

static spush(re)struct EXP *re;
{
  obstack_ptr_grow (&osStack, re);
}

/******************************************************************************
                                                                        SPOP */

static struct SENT *spop()
{
  struct SENT *rs;
  rs= * ((struct SENT * *)obstack_next_free (&osStack) - 1);
  obstack_blank (&osStack, - sizeof (void *));
  rs->lastLine= lineno;
  return (rs);
}

/******************************************************************************
                                                                       SLOOK */

struct SENT *slook()
{
  return *((struct SENT * *)obstack_next_free (&osStack) - 1);
}

/******************************************************************************
                                                                SBUILDERINIT */

sbuilderInit()
{
  obstack_init(&osStack);
  obstack_init(&osSent);
  module.token= MMODULE;
  spush (&module);
}

char nonetest = ON;
char indextest = ON;
char stripsideeffects = OFF;

/******************************************************************************
                                                                     NEWSENT */

struct SENT *newSent(token) int token;
{
  struct SENT *new;
  new= (struct SENT *)obstack_alloc (&osSent, sizeof (struct SENT));
  bzero (new, sizeof (struct SENT));

  new->token= token;
  new->line= lineno;

  return new;
}

/******************************************************************************
                                                                  CREATESENT */

static struct SENT *createSent(token, exp) int token; struct EXP *exp;
{
  struct SENT *new= newSent (token), *parent;

  new->exp= exp;
  new->nonetest= nonetest;
  new->indextest= indextest;
  new->stripsideeffects= stripsideeffects;

  parent= slook ();
  if (parent->first == NULL)
    {
      parent->first= parent->last= new;
    } 
  else
    {
      parent->last->next= new;
      new->prev= parent->last;
      parent->last= new;
    }
  return new;
}

/******************************************************************************
                                                                  INSERTSENT */

void insertAfterSent (parent, after, new) struct SENT *parent, *after, *new;
{
  if (after == NULL)
    {
      if (parent->first == NULL)
	{
	  parent->first= parent->last= new;
	} 
      else
	{
	  parent->first->prev= new;
	  new->next= parent->first;
	  parent->first= new;
	}
    }
  else
    {
      if (after->next == NULL)
	{
	  new->prev= after;
	  after->next= parent->last= new;
	} 
      else
	{
	  after->next->prev= new;
	  new->next= after->next;
	  after->next= new;
	  new->prev= after;
	}
    }
}

void insertBeforeSent (parent, before, new) struct SENT *parent, *before, *new;
{
  if (before == NULL)
    insertAfterSent (parent, parent->last, new);
  else
    insertAfterSent (parent, before->prev, new);
}

/******************************************************************************
                                                                  REMOVESENT */

void removeSent(parent, rem) struct SENT *parent, *rem;
{
  if (rem->next == NULL)
    {
      parent->last= rem->prev;
    }
  else
    {
      rem->next->prev= rem->prev;
    }

  if (rem->prev == NULL)
    {
      parent->first= rem->next;
    }
  else
    {
      rem->prev->next= rem->next;
      rem->prev=NULL;
    }
  rem->next= NULL;
}

/******************************************************************************
                                                                   SETFLAG   */

void setFlag ()
{
  unsigned char token;
  token= min();
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
                                                                      SBUILD */

struct SENT *sbuild()
{
  token= min();
  while (1)
    {
      switch (token)
	{
	case MCONST:
	case MGOTO:
	case MENDSWITCH:
	case MENDASSIGN:
	case MENDARRAY:
	case MENDLABEL:
	  createSent (token, elook());
	  break;
	case MPRBLOCK:
	case MFORDO:
	case MWHILE:
	case MINSPECT:
	case MDO:
	case MWHEN:
	  spush (createSent (token, elook()));
	  break;
	case MBLOCK:
	case MPROCEDURE:
	case MCLASS:
	case MOTHERWISE:
	  spush (createSent (token, NULL));
	  break;
	case MENDBLOCK:
	case MENDPRBLOCK:
	case MENDPROCEDURE:
	case MENDCLASS:
	case MENDINSPECT:
	case MENDWHEN:
	case MENDFOR:
	case MENDDO:
	case MENDOTHERWISE:
	case MENDWHILE:
	  spop();
	  break;
	case MIF:
	  spush (createSent (token, elook()));
	  spush (createSent (MTHEN, NULL));
	  break;
	case MELSE:
	  spop ();
	  spush (createSent (token, elook()));
	  break;
	case MENDIF:
	  spop ();
	  spop ();
	  break;
	case MINNER:
	  createSent (token, NULL);
	  break;
	case MSTOP:
	  return &module;
	  break;
	case MERROR:
	  /* NOTTHING */
	  break;
	case MNEWLINE:
	  lineno++;
	  break;
	case MFLAG:
	  setFlag ();
	  break;
	default:
	  {
	    char *p;
	    p= mpointer;
	    ebuild ();
	    if (p == mpointer)
	      serror (71, token);
	    continue;
	  }
	}
      token= min();
    }
}

/******************************************************************************
								INSERT_THUNK */

void insert_thunk (rex, token) struct EXP *rex; int token;
{
  struct SENT *new= newSent (token);
  if (token == MTHUNKSIMPLEVALUE)
    rex->token= MSENDVALUETHUNKTOFORMALPAR;
  else
    rex->token= MSENDADDRESSTHUNKTOFORMALPAR;
  new->exp= rex;
  new->cblock= cblock;
  rex->value.thunk.label= newlabel ();
  
  rex->value.thunk.inthunk= inthunk+1;
  insertBeforeSent (mainSent, NULL, new);
}
