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
#include <math.h>

#include "const.h"
#include "mellbuilder.h"
#include "builder.h"
#include "checker.h"
#include "expmacros.h"
#include "newstr.h"

/******************************************************************************
                                                            SETDANGER_CONST  */

/* Setter attributtet danger og const
 * F|lgende attributter blir satt danger:
 *
 *  MNEWARG:      Alltid satt
 *  MARRAYARG:    Alltid satt
 *  MCONC:        Alltid satt
 *  MPROCARG:     Hvis det er en prosedyre som m} skilles ut av uttrykket
 *  MASSIGNR:     For den siste assign i multippel assign. dette fordi
 *                gencall da m} kalle genstack: F|lgende kan v{re farlig:
 *                ra.ra:-ra:-...;
 *  MREFASSIGNT:  I det tilfellet at noden over er MVALASSIGNT.
 *                F|lgend kan f.eks. v{re farlig:
 *                t:=t:-....;
 *  MIDENTIFIER:  I det tilfellet at det er en name
 *  MORELSEE:      I det tilfellet at enten venstregrenen eller h|yregrenen
 *                inneholder et attributt som er danger
 *  MANDTHENE:     Samme som ovenfor
 *  MIFE:         Samme som ovenfor
 *  MELSE:        Samme som ovenfor */

char setdanger_const (re)
     struct EXP *re;
{
  char sub_danger;
  if (LEFT != NULL)
    sub_danger = setdanger_const (LEFT);
  if (RIGHT != NULL)
    sub_danger |= setdanger_const (RIGHT);
  DANGER = FALSE;
  switch (re->token)
    {
    case MNEWARG:
    case MARRAYARG:
    case MCONC:
      DANGER = TRUE;
      break;
    case MPROCARG:
      DANGER = dangerProc (RD);
      if (DANGER == FALSE)
	{
	  struct EXP *rex;
	  KONST = TRUE;
	  for (rex = RIGHT; rex->token != MENDSEP; rex = rex->right)
	    {
	      switch (rex->left->token)
		{
		case MNONE:
		case MREALKONST:
		case MTEXTKONST:
		case MCHARACTERKONST:
		case MINTEGERKONST:
		case MBOOLEANKONST:
		  continue;
		}
	      KONST = FALSE;
	      break;
	    }
	}
      break;
    case MASSIGNR:
      if (UPTOKEN == MASSIGNR && RIGHTTOKEN != MASSIGNR)
	DANGER = TRUE;
      break;
    case MREFASSIGNT:
      if (UPTOKEN == MVALASSIGNT)
	DANGER = TRUE;
      break;
    case MIDENTIFIER:
      if (RD->kind == CNAME)
	DANGER = TRUE;
      break;
    case MORELSEE:
    case MANDTHENE:
    case MIFE:
    case MELSE:
      DANGER = sub_danger;
      break;
    }
  return (sub_danger | DANGER);
}

/******************************************************************************
                                                                SSTRCMP      */

#define convbslstr(s) ((s[i]-'0')*64+(s[i+1]-'0')*8+s[i+2]-'0')
static int 
sstrcmp (s, t)
     char s[],
       t[];
{
  int i,
    ss,
    tt;
  i = 0;
  if (s == NOTEXT)
    if (t == NOTEXT)
      return (0);
    else
      return ((char) -1);
  while (s[i] == t[i])
    {
      if (s[i] == '\0')
	return (0);
      if (s[i++] == '\\')
	{
	  ss = convbslstr (s);
	  tt = convbslstr (t);
	  i += 3;
	  if (ss != tt)
	    return (ss - tt);
	}
    }
  ss = s[i];
  tt = t[i];
  i++;
  if (ss == '\\')
    ss = convbslstr (s);
  if (tt == '\\')
    tt = convbslstr (t);
  return (ss - tt);
}

/******************************************************************************
                                                                     SSTRLEN */

int
sstrlen (s)
     char s[];
{
  int i = 0,
    ii = 0;
  if (s == NOTEXT)
    return (0);
  while (s[i])
    {
      ii++;
      if (s[i++] == '\\')
	if (s[i++] == '\n')
	  ii--;
	else
	  i += 2;
    }
  if (ii >= MAX_TEXT_CHAR)
    serror (44);
  return (ii);
}

/******************************************************************************
                                                                COMPUTECONST */

/* Denne rutinen tar som parameter et uttrykks-tre.
 * Den pr|ver } beregne de deler av uttrykket som er constant
 * og erstatte det med et objekt med token lik M...CONST og
 * som har value lik den beregnede constanten.
 * Rutinen er rekursiv
 * og den returnerer TRUE dersom uttrykket lar seg beregne
 * Den fjerner ogs} noder med token lik MNOOP, med unntak n}
 * typen er lik TTEXT og tokenet til noden over i treet er lik MDOT,
 * og tokenet til venstrenoden er lik MIF, MARRAYARG eller MIDENTIFIER.
 * Den fjerner noder med token lik MREAINT 
 * og MINTREA mellom multippel assign.*/

char 
computeconst (re)
     struct EXP *re;
{
  char lconst,
    rconst;
  switch (TOKEN)
    {
    case MIFE:
      lconst = computeconst (LEFT);
      if (lconst == TRUE)
	{
	  struct EXP *rex;
	  if (LEFTVALUE.ival == TRUE)
	    rex = RIGHTLEFT;
	  else
	    rex = RIGHTRIGHT;
	  re->token = rex->token;
	  re->value = rex->value;
	  re->type = rex->type;
	  re->left = rex->left;
	  re->right = rex->right;
	  re->rd = rex->rd;
	  re->qual = rex->qual;
	  re->seenthrough = rex->seenthrough;
	  return (computeconst (re));
	}
      else
	{
	  computeconst (RIGHT);
	  return (FALSE);
	}
      break;
    case MORELSEE:
      lconst = computeconst (LEFT);
      if (lconst == FALSE)
	{
	  computeconst (RIGHT);
	  return (FALSE);
	}
      if (LEFTVALUE.ival == TRUE)
	{
	  TOKEN = MBOOLEANKONST;
	  VALUE.ival = TRUE;
	  LEFT = RIGHT = NULL;
	  return (TRUE);
	}
      rconst = computeconst (RIGHT);
      if (rconst == FALSE)
	return (FALSE);
      TOKEN = MBOOLEANKONST;
      VALUE = RIGHTVALUE;
      LEFT = RIGHT = NULL;
      return (TRUE);
    case MANDTHENE:
      lconst = computeconst (LEFT);
      if (lconst == FALSE)
	{
	  computeconst (RIGHT);
	  return (FALSE);
	}
      if (LEFTVALUE.ival == FALSE)
	{
	  TOKEN = MBOOLEANKONST;
	  VALUE.ival = FALSE;
	  LEFT = RIGHT = NULL;
	  return (TRUE);
	}
      rconst = computeconst (RIGHT);
      if (rconst == FALSE)
	return (FALSE);
      TOKEN = MBOOLEANKONST;
      VALUE = RIGHTVALUE;
      LEFT = RIGHT = NULL;
      return (TRUE);
    default:
      if (LEFT != NULL)
	lconst = computeconst (LEFT);
      if (RIGHT != NULL)
	rconst = computeconst (RIGHT);
      if (TOKEN== MCONC && (lconst == FALSE || rconst == FALSE))
	{
	  gen_textconst (LEFT);
	  gen_textconst (RIGHT);
	}

      if (lconst == FALSE && LEFT != NULL)
	{
	  if (TOKEN == MNOOP && (TYPE != TTEXT || UPTOKEN != MDOT 
				 || (LEFTTOKEN != MIFE
		    && LEFTTOKEN != MARRAYARG && LEFTTOKEN != MIDENTIFIER)))
	    {
	      LEFT->up = UP;
	      if (ISLEFT)
		UP->left = LEFT;
	      else
		UP->right = LEFT;
	    }
	  else if ((TOKEN == MREAINT | TOKEN == MINTREA) && ISRIGHT &&
		   UPTOKEN == MASSIGN && LEFTTOKEN == MASSIGN)
	    {
	      LEFT->up = UP;
	      UP->right = LEFT;
	    }
	  return (FALSE);
	}
      if (rconst == FALSE && RIGHT != NULL)
	return (FALSE);
      break;
    }
  switch (TOKEN)
    {
    case MCONC:
      {
	VALUE.tval.txt= newstrcat2 (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt);
	TOKEN = MTEXTKONST;
      }
    case MTEXTKONST:
      if (UPTOKEN != MCONC) gen_textconst (re);
      break;
    case MOR:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival | RIGHTVALUE.ival;
      break;
    case MAND:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival & RIGHTVALUE.ival;
      break;
    case MEQV:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival == RIGHTVALUE.ival;
      break;
    case MIMP:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = !(LEFTVALUE.ival & !RIGHTVALUE.ival);
      break;
    case MNOT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = !LEFTVALUE.ival;
      break;
    case MEQT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) == 0;
      break;
    case MNET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) != 0;
      break;
    case MLTT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) < 0;
      break;
    case MLET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) <= 0;
      break;
    case MGTT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) > 0;
      break;
    case MGET:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = sstrcmp (LEFTVALUE.tval.txt, RIGHTVALUE.tval.txt) >= 0;
      break;
    case MEQ:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival == RIGHTVALUE.ival;
      break;
    case MNE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival != RIGHTVALUE.ival;
      break;
    case MLT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival < RIGHTVALUE.ival;
      break;
    case MLE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival <= RIGHTVALUE.ival;
      break;
    case MGT:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival > RIGHTVALUE.ival;
      break;
    case MGE:
      TOKEN = MBOOLEANKONST;
      VALUE.ival = LEFTVALUE.ival >= RIGHTVALUE.ival;
      break;
    case MUADD:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval;
      break;
    case MUSUB:
      TOKEN = MREALKONST;
      VALUE.rval = -LEFTVALUE.rval;
      break;
    case MADD:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval + RIGHTVALUE.rval;
      break;
    case MSUB:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval - RIGHTVALUE.rval;
      break;
    case MMUL:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval * RIGHTVALUE.rval;
      break;
    case MDIV:
      TOKEN = MREALKONST;
      VALUE.rval = LEFTVALUE.rval / RIGHTVALUE.rval;
      break;
    case MPRIMARY:
      TOKEN = MREALKONST;
      VALUE.rval = pow (LEFTVALUE.rval, RIGHTVALUE.rval);
      break;
    case MPRIMARYRI:
      {
	long i;
	VALUE.rval = 1.0;
	if (RIGHTVALUE.ival < 0)
	  serror (4);
	for (i = 1; i <= RIGHTVALUE.ival; i++)
	  VALUE.rval *= LEFTVALUE.rval;
	TOKEN = MREALKONST;
      }
      break;
    case MREAINT:
      TOKEN = MREALKONST;
      VALUE.rval = (double) LEFTVALUE.ival;
      break;
    case MINTREA:
      TOKEN = MINTEGERKONST;
      VALUE.ival = (long) (LEFTVALUE.rval += 0.5);
      if (VALUE.ival > LEFTVALUE.rval)
	VALUE.ival--;
      break;
    case MUADDI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival;
      break;
    case MUSUBI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = -LEFTVALUE.ival;
      break;
    case MADDI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival + RIGHTVALUE.ival;
      break;
    case MSUBI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival - RIGHTVALUE.ival;
      break;
    case MMULI:
      TOKEN = MINTEGERKONST;
      VALUE.ival = LEFTVALUE.ival * RIGHTVALUE.ival;
      break;
    case MINTDIV:
      TOKEN = MINTEGERKONST;
      if (RIGHTVALUE.ival == 0)
	{
	  serror (1);
	  VALUE.ival = LEFTVALUE.ival;
	}
      else
	VALUE.ival = LEFTVALUE.ival / RIGHTVALUE.ival;
      break;
    case MPRIMARYII:
      {
	long i;
	VALUE.ival = 1;
	if (RIGHTVALUE.ival < 0)
	  serror (4);
	for (i = 1; i <= RIGHTVALUE.ival; i++)
	  VALUE.ival *= LEFTVALUE.ival;
	TOKEN = MINTEGERKONST;
      }
      break;
    case MNOOP:
      TOKEN = LEFTTOKEN;
      VALUE = LEFTVALUE;
      break;
    case MREALKONST:
    case MCHARACTERKONST:
    case MINTEGERKONST:
    case MBOOLEANKONST:
      return (TRUE);
    default:
      return (FALSE);
    }
  LEFT = NULL;
  RIGHT = NULL;
  return (TRUE);
}

