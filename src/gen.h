/* $Id: cgen.h,v 1.4 1994/12/24 21:04:02 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen and Terje Mjøs,
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
#include "mellbuilder.h"
#include "cimcomp.h"
#include "builder.h"
#include "lex.h"

#define USEDREF 1
#define USEDTXT 2
#define USEDVAL 4
#define MAXUSED 8

#define seen_th_insp(re) (re->seenthrough!=NULL && re->seenthrough->quant.kind==KINSP)

#define gen_sent_marker() fprintf (ccode, ";");

extern int inthunk;

extern char *mainroutine;

extern char not_reached;

extern int lediglabel;

/* gen.c */

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

/* transcall.c */
extern int findallentry();
extern struct EXP *transcall();
extern long ant_stack();

/* genexp.c */
extern gen();
extern gensl();
extern genchain();
extern gentype ();

extern genmodulemark();
extern gen_adr_prot();

extern genvalue();

/* genpar.c */
extern genprocparam ();
extern genpredefproccall ();
extern gencproccall ();
extern gen_thunk_simple_address ();
extern gen_thunk_simple_value ();
extern gen_thunk_lable ();
extern gen_thunk_array ();
extern gen_thunk_procedure ();

/* genstr.c */
extern init_structure();
extern int newlabel();

/* sentgen.c */

extern void sent_gen ();

