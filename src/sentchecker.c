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

void sent_check (sent_t *parent_sent, char res_labels)
{
  sent_t *sent, *next_sent, *when_sent;
  char remove_when_clauses=FALSE;

  for (sent= parent_sent->first; sent!=NULL; sent= next_sent)
    {
      next_sent= sent->next;
      lineno= sent->line;
      switch (sent->token)
	{
	case MCONST:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
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
	  in_block ();
	  sent->cblock=cblock;
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MPRBLOCK:
	  main_exp_check (sent->exp);
	  in_block ();
	  sent->cblock=cblock;
	  if (res_labels) 
	    {
	      sent->cblock->ent = newlabel ();
	      newlabel ();
	    }
	  if (sent->exp->type != TERROR &&
	      (sent->exp->token != MARGUMENT || sent->exp->rd->kind != KCLASS))
	    serror (3);
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MPROCEDURE:
	  in_block ();
	  sent->cblock=cblock;
	  if (res_labels) sent->cblock->ent = newlabel ();
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MCLASS:
	  in_block ();
	  sent->cblock=cblock;
	  if (res_labels) 
	    {
	      sent->cblock->ent = newlabel ();
	      newlabel ();		/* Label etter dekl. del */
	      newlabel ();		/* Etter kall p} rinner  */
	    }
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MINSPECT:
	  main_exp_check (sent->exp);
	  if (sent->exp->type != TREF && sent->exp->type != TERROR)
	    serror (73, token);
	  in_block ();
	  sent->cblock=cblock;
          reginsp (sent->cblock, sent->exp->qual);
	  sent_check (sent, res_labels);
	  if (sent->last==NULL || sent->last->token != MOTHERWISE) out_block ();
	  break;
	case MDO:
	  in_block ();
	  sent->cblock=cblock;
	  regwhen (sent->cblock, parent_sent->cblock->virt);
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MWHEN:
	  main_exp_check (sent->exp);
	  in_block ();
	  sent->cblock=cblock;
	  {
	    char not_removed=TRUE;
	    /* Sjekker om rd er samme klasse eller en subklasse til * klassen 
	     * som inspiseres,eller omvendt */
	    if (!subclass (sent->exp->rd, parent_sent->cblock->virt)
		&& !subclass (parent_sent->cblock->virt, sent->exp->rd))
	      {
		serror (83, sent->exp->rd->ident);
		/* Trenger ikke å legge ut kode for denne WHEN grenen */
		remove_block (sent->cblock);
		remove_sent (parent_sent, sent); 
		not_removed= FALSE;
	      }
	    else if (subclass (parent_sent->cblock->virt, sent->exp->rd) &&
		     sent->prev == NULL)
	      {
		serror (82, sent->exp->rd->ident);
	      }
	    else
	      {
		for (when_sent=parent_sent->first; when_sent != sent; 
		     when_sent= when_sent->next)
		  {
		    if (subclass (sent->exp->rd, when_sent->exp->rd))
		      {
			serror (83, sent->exp->rd->ident);
			/* Ingen kode for denne WHEN grenen */
			remove_block (sent->cblock);
			remove_sent (parent_sent, sent);
			not_removed= FALSE;
			break;
		      }
		  }
	      }
	    regwhen (sent->cblock, sent->exp->rd);
	    sent_check (sent, not_removed);
	  }
	  out_block ();
	  break;
	case MOTHERWISE:
	  out_block ();
	  sent->cblock=cblock;
	  sent_check (sent, res_labels);
	  break;
	case MFORDO:
	  in_block ();
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  if (sent->first == NULL) serror (81);
	  sent_check (sent, res_labels);
	  out_block ();
	  break;
	case MWHILE:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  if (sent->exp->type != TBOOL && sent->exp->type != TERROR)
	    serror (77, token);
	  if (sent->first == NULL) serror (81);
	  sent_check (sent, res_labels);
	  break;
	case MIF:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  if (sent->exp->type != TBOOL)
	    if (sent->exp->type != TERROR)
	      serror (77, token);
	  sent_check (sent, res_labels);
	  break;
	case MELSE:
	  sent->cblock=cblock;
	  sent_check (sent, res_labels);
	  break;
	case MTHEN:
	  sent->cblock=cblock;
	  sent_check (sent, res_labels);
	  break;
	case MGOTO:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  if (sent->exp->type != TLABEL && sent->exp->type != TERROR)
	    serror (108, token);
	  break;
	case MINNER:
	  sent->cblock=cblock;
	  break;
	case MENDSWITCH:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  break;
	case MENDASSIGN:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  if (sent->exp->token != MPROCARG && sent->exp->token != MNEWARG &&
	      sent->exp->token != MASSIGN && sent->exp->token != MREFASSIGNT &&
	      sent->exp->token != MASSIGNR && sent->exp->token != MVALASSIGNT &&
	      (sent->exp->token != MDOT || sent->exp->right->token != MPROCARG) &&
	      sent->exp->type != TERROR)
	    serror (115);
	  break;
	case MENDARRAY:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  break;
	case MENDLABEL:
	  sent->cblock=cblock;
	  main_exp_check (sent->exp);
	  break;
	}
    }
}

