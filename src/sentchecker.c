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
#include "mellbuilder.h"
#include "builder.h"
#include "checker.h"
#include "trans.h"

/******************************************************************************
                                                                   SENTCHECK */

void sentCheck (parentSent, resLabels) struct SENT *parentSent; char resLabels;
{
  struct SENT *sent, *nextSent, *whenSent;
  char removeWhenClauses=FALSE;

  for (sent= parentSent->first; sent!=NULL; sent= nextSent)
    {
      nextSent= sent->next;
      lineno= sent->line;
      switch (sent->token)
	{
	case MCONST:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  {

	    /* TBD Har ikke implementert fremoverreferanser for konstant deklarasjoner */ 
	    int token = sent->exp->right->token;
	    if (token != MREALKONST & token != MTEXTKONST 
		& token != MCHARACTERKONST
		& token != MINTEGERKONST & token != MBOOLEANKONST
		& sent->exp->right->type != TERROR)
	      serror (6);
	  }
	  sent->exp->left->rd->value = sent->exp->right->value;
	  sent->exp->left->rd->categ = CCONST;
	  break;
	case MBLOCK:
	  inBlock ();
	  sent->cblock=cblock;
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MPRBLOCK:
	  mainExpCheck (sent->exp);
	  inBlock ();
	  sent->cblock=cblock;
	  if (resLabels) 
	    {
	      sent->cblock->ent = newlabel ();
	      newlabel ();
	    }
	  if (sent->exp->type != TERROR &&
	      (sent->exp->token != MARGUMENT || sent->exp->rd->kind != KCLASS))
	    serror (3);
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MPROCEDURE:
	  inBlock ();
	  sent->cblock=cblock;
	  if (resLabels) sent->cblock->ent = newlabel ();
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MCLASS:
	  inBlock ();
	  sent->cblock=cblock;
	  if (resLabels) 
	    {
	      sent->cblock->ent = newlabel ();
	      newlabel ();		/* Label etter dekl. del */
	      newlabel ();		/* Etter kall p} rinner  */
	    }
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MINSPECT:
	  mainExpCheck (sent->exp);
	  if (sent->exp->type != TREF && sent->exp->type != TERROR)
	    serror (73, token);
	  inBlock ();
	  sent->cblock=cblock;
          reginsp (sent->cblock, sent->exp->qual);
	  sentCheck (sent, resLabels);
	  if (sent->last==NULL || sent->last->token != MOTHERWISE) outBlock ();
	  break;
	case MDO:
	  inBlock ();
	  sent->cblock=cblock;
	  regwhen (sent->cblock, parentSent->cblock->virt);
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MWHEN:
	  mainExpCheck (sent->exp);
	  inBlock ();
	  sent->cblock=cblock;
	  {
	    char notRemoved=TRUE;
	    /* Sjekker om rd er samme klasse eller en subklasse til * klassen 
	     * som inspiseres,eller omvendt */
	    if (!subclass (sent->exp->rd, parentSent->cblock->virt)
		&& !subclass (parentSent->cblock->virt, sent->exp->rd))
	      {
		serror (83, sent->exp->rd->ident);
		/* Trenger ikke å legge ut kode for denne WHEN grenen */
		removeBlock (sent->cblock);
		removeSent (parentSent, sent); 
		notRemoved= FALSE;
	      }
	    else if (subclass (parentSent->cblock->virt, sent->exp->rd) &&
		     sent->prev == NULL)
	      {
		serror (82, sent->exp->rd->ident);
	      }
	    else
	      {
		for (whenSent=parentSent->first; whenSent != sent; 
		     whenSent= whenSent->next)
		  {
		    if (subclass (sent->exp->rd, whenSent->exp->rd))
		      {
			serror (83, sent->exp->rd->ident);
			/* Ingen kode for denne WHEN grenen */
			removeBlock (sent->cblock);
			removeSent (parentSent, sent);
			notRemoved= FALSE;
			break;
		      }
		  }
	      }
	    regwhen (sent->cblock, sent->exp->rd);
	    sentCheck (sent, notRemoved);
	  }
	  outBlock ();
	  break;
	case MOTHERWISE:
	  outBlock ();
	  sent->cblock=cblock;
	  sentCheck (sent, resLabels);
	  break;
	case MFORDO:
	  inBlock ();
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  if (sent->first == NULL) serror (81);
	  sentCheck (sent, resLabels);
	  outBlock ();
	  break;
	case MWHILE:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  if (sent->exp->type != TBOOL && sent->exp->type != TERROR)
	    serror (77, token);
	  if (sent->first == NULL) serror (81);
	  sentCheck (sent, resLabels);
	  break;
	case MIF:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  if (sent->exp->type != TBOOL)
	    if (sent->exp->type != TERROR)
	      serror (77, token);
	  sentCheck (sent, resLabels);
	  break;
	case MELSE:
	  sent->cblock=cblock;
	  sentCheck (sent, resLabels);
	  break;
	case MTHEN:
	  sent->cblock=cblock;
	  sentCheck (sent, resLabels);
	  break;
	case MGOTO:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  if (sent->exp->type != TLABEL && sent->exp->type != TERROR)
	    serror (108, token);
	  break;
	case MINNER:
	  sent->cblock=cblock;
	  break;
	case MENDSWITCH:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  break;
	case MENDASSIGN:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  if (sent->exp->token != MPROCARG && sent->exp->token != MNEWARG &&
	      sent->exp->token != MASSIGN && sent->exp->token != MREFASSIGNT &&
	      sent->exp->token != MASSIGNR && sent->exp->token != MVALASSIGNT &&
	      (sent->exp->token != MDOT || sent->exp->right->token != MPROCARG) &&
	      sent->exp->type != TERROR)
	    serror (115);
	  break;
	case MENDARRAY:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  break;
	case MENDLABEL:
	  sent->cblock=cblock;
	  mainExpCheck (sent->exp);
	  break;
	}
    }
}

