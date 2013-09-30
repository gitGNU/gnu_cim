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
#include "salloc.h"
#include "passes.h"
#include "config.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#if STDC_HEADERS
#include <stdlib.h>
#endif

char *xmalloc();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_stack;

sent_t module;

/******************************************************************************
                                                                       SPUSH */

static spush(sent_t *re)
{
  obstack_grow (&os_stack, &re, sizeof (void *));
}

/******************************************************************************
                                                                        SPOP */

static sent_t *spop(void)
{
  sent_t *rs;
  rs= * ((sent_t * *)obstack_next_free (&os_stack) - 1);
  obstack_blank (&os_stack, - (int) sizeof (void *));
  rs->last_line= lineno;
  return (rs);
}

/******************************************************************************
                                                                       SLOOK */

static sent_t *slook(void)
{
  return *((sent_t * *)obstack_next_free (&os_stack) - 1);
}

/******************************************************************************
                                                                SBUILDERINIT */

void sbuilder_init(void)
{
  obstack_init(&os_stack);
  module.token= MMODULE;
  spush (&module);
}

char nonetest = ON;
char indextest = ON;
char stripsideeffects = OFF;

/******************************************************************************
                                                                     NEWSENT */

sent_t * new_sent(int token)
{
  sent_t *new;
  new= (sent_t *) salloc (sizeof (sent_t));

  new->token= token;
  new->line= lineno;

  return new;
}

/******************************************************************************
                                                                  CREATESENT */

static sent_t *create_sent(int token, exp_t *exp)
{
  sent_t *new= new_sent (token), *parent;

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

void insert_after_sent (sent_t *parent, sent_t *after, sent_t *new)
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

void insert_before_sent (sent_t *parent, sent_t *before, sent_t *new)
{
  if (before == NULL)
    insert_after_sent (parent, parent->last, new);
  else
    insert_after_sent (parent, before->prev, new);
}

/******************************************************************************
                                                                  REMOVESENT */

void remove_sent(sent_t *parent, sent_t *rem)
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

void set_flag (void)
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

sent_t *sbuild(void)
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
	  create_sent (token, elook());
	  break;
	case MPRBLOCK:
	case MFORDO:
	case MWHILE:
	case MINSPECT:
	case MDO:
	case MWHEN:
	  spush (create_sent (token, elook()));
	  break;
	case MBLOCK:
	case MPROCEDURE:
	case MCLASS:
	case MOTHERWISE:
	  spush (create_sent (token, NULL));
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
	  spush (create_sent (token, elook()));
	  spush (create_sent (MTHEN, NULL));
	  break;
	case MELSE:
	  spop ();
	  spush (create_sent (token, elook()));
	  break;
	case MENDIF:
	  spop ();
	  spop ();
	  break;
	case MINNER:
	  create_sent (token, NULL);
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
	  set_flag ();
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

void insert_thunk (exp_t *rex, int token)
{
  sent_t *new= new_sent (token);
  if (token == MTHUNKSIMPLEVALUE)
    rex->token= MSENDVALUETHUNKTOFORMALPAR;
  else
    rex->token= MSENDADDRESSTHUNKTOFORMALPAR;
  new->exp= rex;
  new->cblock= cblock;
  rex->value.thunk.label= newlabel ();
  
  rex->value.thunk.inthunk= inthunk+1;
  insert_before_sent (main_sent, NULL, new);
}
