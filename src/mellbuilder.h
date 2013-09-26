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

void mbuilder_init(void);
void mbuilder_init_pass2(void);
void mbuilder_reinit(void);
void mout(unsigned char x);
void mout_ival(long x);
void mout_rval(double x);
void mout_tval(char *x);
void mout_id(char *x);
int min(void);
long min_ival(void);
double min_rval(void);
char *min_tval(void);
char *min_id(void);

extern char *mpointer;


/* De forskjellige tokens som legges ut i pass 1
 * og som skal leses inn av pass 2
 * Alle opperatorer i uttrykk  legges ut postfix
 * Uttrykk får et utvidet begrep i pass 2
 *
 * Setntokens   uten   id:       1 - 33         
 * Setntokens   med    id:      35 - 39         
 *                                              
 * Exprtokens med  id/val:      40 - 48         
 * Exprtokens uten id/val:      49 - ->         
 * Exprtokens       binær:      50 - 98         
 * Error som  yacc sender:        127           
 * Newline som lex sender:         0            
 * Filslutt   ->  ferdige:        - 1 */


typedef enum { MSTOP= -1, MERROR, MNEWLINE, MBLOCK, MENDBLOCK, MPRBLOCK,
    MENDPRBLOCK, MPROCEDURE, MENDPROCEDURE, MCLASS, MENDCLASS, MINSPECT,
    MENDINSPECT, MDO, MENDDO, MWHEN, MENDWHEN, MOTHERWISE, MENDOTHERWISE,
    MFOR, MFORR, MLISTSEP, MFORWHILE, MSTEP, MUNTIL, MFORDO, MENDFOR,
    MWHILE, MENDWHILE, MIF, MELSE, MENDIF, MGOTO, MINNER, MSWITCH,
    MSWITCHSEP, MENDSWITCH, MASSIGN, MASSIGNR, MENDASSIGN, MARRAY,
    MARRAYSEP, MENDARRAY, MBOUNDPARSEP, MBOUNDSEP, MENDLABEL, MCONST,
    MIFE, MELSEE, MORELSEE, MANDTHENE, MEQV, MIMP, MOR, MAND, MNOT, MEQ,
    MNE, MLT, MLE, MGT, MGE, MNER, MEQR, MIS, MINS, MUADD, MUSUB, MADD,
    MSUB, MMUL, MDIV, MINTDIV, MPRIMARY, MNOOP, MTEXTKONST,
    MCHARACTERKONST, MREALKONST, MINTEGERKONST, MBOOLEANKONST, MNONE,
    MIDENTIFIER, MTHIS, MDOT, MNEWARG, MQUA, MARGUMENT, MARGUMENTSEP,
    MENDSEP, MPROCARG, MARRAYARG, MREFASSIGNT, MFLAG, MEQT, MNET, MLTT, 
    MLET, MGTT, MGET, MNERT, MEQRT, MUADDI, MUSUBI, MADDI, MSUBI, MMULI,
    MPRIMARYII, MPRIMARYRI, MQUANOTNONE, MQUANONEAND, MREAINT, MINTREA,
    MSTACK, MARRAYADR, MDOTARRAYADR, MVALASSIGNT, MTEXTADR, MLABEL, MCONC,
    MDOTCONST, MNAMEADR, MPROCASSIGN, MMODULE, MTHEN, MENDDECL, MENTRY,
    MBLOCKENTRY, MPROCEDUREENTRY, MSL, MSENTCONC, MINSTRONGEST,
    MEXITARGUMENT, MASSIGND, MNAMEREADTEXT, MNAMEREADACESS,
    MNAMEWRITEACESS, MORELSE, MANDTHEN, MTHUNKSIMPLEADDRESS,
    MTHUNKSIMPLEVALUE, MTHUNKLABLE, MTHUNKARRAY, MTHUNKPROCEDURE,
    MGOTOSTOP, MSIGNDX, MSIGNDI, MSIGNDR, MASSIGNADD,
    MSENDADDRESSTHUNKTOFORMALPAR, MSENDVALUETHUNKTOFORMALPAR} token_t;

#define MNONETEST 0
#define MINDEXTEST 2 
#define MSTRIPSIDEEFFECTS 4








