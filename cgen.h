/* $Id: cgen.h,v 1.4 1994/12/24 21:04:02 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
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

#include "cextern.h"
#include "cmell.h"
#include "cdekl.h"

#define genchainrdencl(x,y) gensl(x,y,ON)

/* Objekter til å generere uttrykk-treet */
struct EXP
  {
    int token;
    union val value;
    char type,
      danger,
      konst;
    struct EXP *left,
     *right,
     *up;
    struct DECL *rd;
    struct DECL *qual;
    struct BLOCK *seenthrough;
  };

#define newexp (cexpr==MAXEXPBUFFER?((sjerror?0:(sjerror=TRUE,serror(70)))\
,&owerflowbuffer):&expbuffer[++cexpr])

/* Brytere som kan sl}es AV/P} ved hjelp av kompilator-direktiver */
extern char nonetest,
  indextest,
  stripsideeffects,
  nameasvar;

#define pop stakk[cur--]
#define push stakk[(cur<MAXLABELSTAKK)?++cur:(gerror(84),1)]
#define look stakk[cur]

#define USEDREF 1
#define USEDTXT 2
#define USEDVAL 4
#define MAXUSED 8

#define is_after_dot(re) (re->up->token==MDOT && re->up->right==re)
#define seen_th_insp(re) (re->seenthrough!=NULL && re->seenthrough->quant.kind==KINSP)

extern int inthunk;

extern char not_reached;
