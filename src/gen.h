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
#include "cimcomp.h"
#include "builder.h"

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

/* transcall.c */
int findallentry (exp_t *ret, exp_t *re, int type, int min);
exp_t *transcall (exp_t *ret, exp_t *re, int minval, int minref, int mintxt);
long ant_stack (exp_t *ret, exp_t *re, int minval, int minref, int mintxt);

/* genexp.c */

void gen_ref_stack (int i);
void gen_int_stack (int i);
void gen_textconst (exp_t *re);

void gen_for_val(int i);
extern void gen_con_val();

void gen (exp_t *re);
void gensl (exp_t *re, char atr, char nonetest);
void genchain (block_t *rb, char atr);
void gentype (exp_t *re);

void genmodulemark(char *maintimestamp);
void gen_adr_prot (FILE *code, decl_t *rd);

void genvalue (exp_t *re);
void gen_con_ref (int i);

/* genpar.c */
void genprocparam (exp_t *rex);
void genpredefproccall (exp_t *rex);
void gencproccall (exp_t *rex);
void gen_thunk_simple_address (exp_t *rex);
void gen_thunk_simple_value (exp_t *rex);
void gen_thunk_lable (exp_t *rex);
void gen_thunk_array (exp_t *rex);
void gen_thunk_procedure (exp_t *rex);

/* genstr.c */
void init_structure(void);
void structure(void);
void stat_pointers (void);

/* sentgen.c */

void sent_gen (sent_t *sent, int lab);

/* linegen.c */
int newlabel(void);
void typelabel (int l);
void gotoswitch (void);
void exitlabel (int l);
void exitcondlabel (int l);
void gotolabel (int l);

int newllabel (void);
void typellabel (int l);
void gotollabel (int l);
void genline (void);

