/* $Id: cgen.h,v 1.4 1994/12/24 21:04:02 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen and Terje Mjøs,
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

#include "const.h"
#include "mell.h"
#include "cimcomp.h"
#include "sjekker.h"

#define genchainrdencl(x,y) gensl(x,y,ON)

#define newexp (cexpr==MAXEXPBUFFER?((sjerror?0:(sjerror=TRUE,serror(70)))\
,&owerflowbuffer):&expbuffer[++cexpr])

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

extern char mainroutine[MAINROUTINELENGTH + 1];

extern int stakk[MAXLABELSTAKK];
extern int cur;

/* gen.c */

extern initgen();
extern fingen();

extern newlabel();
extern typelabel();
extern gotoswitch();
extern exitlabel();
extern exitcondlabel ();
extern gotolabel();

extern newllabel();
extern typellabel();
extern gotollabel();

extern genline();

extern skrivenddecl();

extern gblock();
extern gendblock();

extern gprblock();
extern gendprblock();

extern gprocedure();
extern gendprocedure();

extern gclass();
extern gendclass();

extern ginspect();
extern gendinspect();

extern gdo();
extern genddo();

extern gwhen();
extern gendwhen();

extern gotherwise();
extern gendotherwise();

extern gfor();
extern gfordo();
extern gendfordo();

extern gwhile();
extern gendwhile();

extern gif();
extern gelse();
extern gendif();

extern glabel();
extern ggoto();

extern ginner();

extern genarray();
extern genswitch();

/* genexp.c */
extern int findallentry();
extern gensl();
extern genchain();
extern gentype ();

extern gen_adr_prot();
extern gen_ant_stack();

extern remove_dot();

extern gen();
extern short savepar();

extern gencall();
extern genvalue();

/* genpar.c */
extern genprocparam ();
extern genpredefproccall ();
extern gencproccall ();
extern genctextproccall ();

/* genstr.c*/
extern init_structure();
extern int newlabel();
