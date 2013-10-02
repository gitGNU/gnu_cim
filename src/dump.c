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

#ifdef DEBUG

#include <stdio.h>
#include "builder.h"
#include "dump.h"

/******************************************************************************
                                                                TOKEN_PRINT  */
static void token_print (int token)
{
  switch (token)
    {
    case MERROR: printf ("MERROR");
      break;
    case MNEWLINE: printf ("MNEWLINE");
      break;
    case MSTOP: printf ("MSTOP");
      break;
    case MBLOCK: printf ("MBLOCK");
      break;
    case MENDBLOCK: printf ("MENDBLOCK");
      break;
    case MPRBLOCK: printf ("MPRBLOCK");
      break;
    case MENDPRBLOCK: printf ("MENDPRBLOCK");
      break;
    case MPROCEDURE: printf ("MPROCEDURE");
      break;
    case MENDPROCEDURE: printf ("MENDPROCEDURE");
      break;
    case MCLASS: printf ("MCLASS");
      break;
    case MENDCLASS: printf ("MENDCLASS");
      break;
    case MINSPECT: printf ("MINSPECT");
      break;
    case MENDINSPECT: printf ("MENDINSPECT");
      break;
    case MDO: printf ("MDO");
      break;
    case MENDDO: printf ("MENDDO");
      break;
    case MWHEN: printf ("MWHEN");
      break;
    case MENDWHEN: printf ("MENDWHEN");
      break;
    case MOTHERWISE: printf ("MOTHERWISE");
      break;
    case MFOR: printf ("MFOR");
      break;
    case MFORR: printf ("MFORR");
      break;
    case MLISTSEP: printf ("MLISTSEP");
      break;
    case MFORWHILE: printf ("MFORWHILE");
      break;
    case MSTEP: printf ("MSTEP");
      break;
    case MUNTIL: printf ("MUNTIL");
      break;
    case MFORDO: printf ("MFORDO");
      break;
    case MENDFOR: printf ("MENDFOR");
      break;
    case MWHILE: printf ("MWHILE");
      break;
    case MENDWHILE: printf ("MENDWHILE");
      break;
    case MIF: printf ("MIF");
      break;
    case MELSE: printf ("MELSE");
      break;
    case MENDIF: printf ("MENDIF");
      break;
    case MGOTO: printf ("MGOTO");
      break;
    case MINNER: printf ("MINNER");
      break;
    case MSWITCH: printf ("MSWITCH");
      break;
    case MSWITCHSEP: printf ("MSWITCHSEP");
      break;
    case MENDSWITCH: printf ("MENDSWITCH");
      break;
    case MASSIGN: printf ("MASSIGN");
      break;
    case MASSIGNR: printf ("MASSIGNR");
      break;
    case MENDASSIGN: printf ("MENDASSIGN");
      break;
    case MARRAY: printf ("MARRAY");
      break;
    case MARRAYSEP: printf ("MARRAYSEP");
      break;
    case MENDARRAY: printf ("MENDARRAY");
      break;
    case MBOUNDPARSEP: printf ("MBOUNDPARSEP");
      break;
    case MBOUNDSEP: printf ("MBOUNDSEP");
      break;
    case MENDLABEL: printf ("MENDLABEL");
      break;
    case MCONST: printf ("MCONST");
      break;
    case MIFE: printf ("MIFE");
      break;
    case MELSEE: printf ("MELSEE");
      break;
    case MORELSEE: printf ("MORELSEE");
      break;
    case MANDTHENE: printf ("MANDTHENE");
      break;
    case MEQV: printf ("MEQV");
      break;
    case MIMP: printf ("MIMP");
      break;
    case MOR: printf ("MOR");
      break;
    case MAND: printf ("MAND");
      break;
    case MNOT: printf ("MNOT");
      break;
    case MEQ: printf ("MEQ");
      break;
    case MNE: printf ("MNE");
      break;
    case MLT: printf ("MLT");
      break;
    case MLE: printf ("MLE");
      break;
    case MGT: printf ("MGT");
      break;
    case MGE: printf ("MGE");
      break;
    case MNER: printf ("MNER");
      break;
    case MEQR: printf ("MEQR");
      break;
    case MIS: printf ("MIS");
      break;
    case MINS: printf ("MINS");
      break;
    case MUADD: printf ("MUADD");
      break;
    case MUSUB: printf ("MUSUB");
      break;
    case MADD: printf ("MADD");
      break;
    case MSUB: printf ("MSUB");
      break;
    case MMUL: printf ("MMUL");
      break;
    case MDIV: printf ("MDIV");
      break;
    case MINTDIV: printf ("MINTDIV");
      break;
    case MPRIMARY: printf ("MPRIMARY");
      break;
    case MNOOP: printf ("MNOOP");
      break;
    case MTEXTKONST: printf ("MTEXTKONST");
      break;
    case MCHARACTERKONST: printf ("MCHARACTERKONST");
      break;
    case MREALKONST: printf ("MREALKONST");
      break;
    case MINTEGERKONST: printf ("MINTEGERKONST");
      break;
    case MBOOLEANKONST: printf ("MBOOLEANKONST");
      break;
    case MNONE: printf ("MNONE");
      break;
    case MIDENTIFIER: printf ("MIDENTIFIER");
      break;
    case MTHIS: printf ("MTHIS");
      break;
    case MDOT: printf ("MDOT");
      break;
    case MNEWARG: printf ("MNEWARG");
      break;
    case MQUA: printf ("MQUA");
      break;
    case MARGUMENT: printf ("MARGUMENT");
      break;
    case MARGUMENTSEP: printf ("MARGUMENTSEP");
      break;
    case MENDSEP: printf ("MENDSEP");
      break;
    case MPROCARG: printf ("MPROCARG");
      break;
    case MARRAYARG: printf ("MARRAYARG");
      break;
    case MREFASSIGNT: printf ("MREFASSIGNT");
      break;
    case MEQT: printf ("MEQT");
      break;
    case MNET: printf ("MNET");
      break;
    case MLTT: printf ("MLTT");
      break;
    case MLET: printf ("MLET");
      break;
    case MGTT: printf ("MGTT");
      break;
    case MGET: printf ("MGET");
      break;
    case MNERT: printf ("MNERT");
      break;
    case MEQRT: printf ("MEQRT");
      break;
    case MUADDI: printf ("MUADDI");
      break;
    case MUSUBI: printf ("MUSUBI");
      break;
    case MADDI: printf ("MADDI");
      break;
    case MSUBI: printf ("MSUBI");
      break;
    case MMULI: printf ("MMULI");
      break;
    case MPRIMARYII: printf ("MPRIMARYII");
      break;
    case MPRIMARYRI: printf ("MPRIMARYRI");
      break;
    case MQUANOTNONE: printf ("MQUANOTNONE");
      break;
    case MQUANONEAND: printf ("MQUANONEAND");
      break;
    case MREAINT: printf ("MREAINT");
      break;
    case MINTREA: printf ("MINTREA");
      break;
    case MSTACK: printf ("MSTACK");
      break;
    case MARRAYADR: printf ("MARRAYADR");
      break;
    case MDOTARRAYADR: printf ("MDOTARRAYADR");
      break;
    case MVALASSIGNT: printf ("MVALASSIGNT");
      break;
    case MTEXTADR: printf ("MTEXTADR");
      break;
    case MLABEL: printf ("MLABEL");
      break;
    case MCONC: printf ("MCONC");
      break;
    case MDOTCONST: printf ("MDOTCONST");
      break;
    case MNAMEADR: printf ("MNAMEADR");
      break;
    case MPROCASSIGN: printf ("MPROCASSIGN");
      break;
    case MFLAG: printf ("MFLAG");
      break;
    case MMODULE: printf ("MMODULE");
      break;
    case MTHEN: printf ("MTHEN");
      break;
    case MENDDECL: printf ("MENDDECL");
      break;
    case MENTRY: printf ("MENTRY");
      break;
    case MBLOCKENTRY: printf ("MBLOCKENTRY");
      break;
    case MPROCEDUREENTRY: printf ("MPROCEDUREENTRY");
      break;
    case MSL: printf ("MSL");
      break;
    case MSENTCONC: printf ("MSENTCONC");
      break;
    case MINSTRONGEST: printf ("MINSTRONGEST");
      break;
    case MEXITARGUMENT: printf ("MEXITARGUMENT");
      break;
    case MASSIGND: printf ("MASSIGND");
      break;
    case MNAMEREADTEXT: printf ("MNAMEREADTEXT");
      break;
    case MNAMEREADACESS: printf ("MNAMEREADACESS");
      break;
    case MNAMEWRITEACESS: printf ("MNAMEWRITEACESS");
      break;
    case MORELSE: printf ("MORELSE");
      break;
    case MANDTHEN: printf ("MANDTHEN");
      break;
    case MTHUNKSIMPLEADDRESS: printf ("MTHUNKSIMPLEADDRESS");
      break;
    case MTHUNKSIMPLEVALUE: printf ("MTHUNKSIMPLEVALUE");
      break;
    case MTHUNKLABLE: printf ("MTHUNKLABLE");
      break;
    case MTHUNKARRAY: printf ("MTHUNKARRAY");
      break;
    case MTHUNKPROCEDURE: printf ("MTHUNKPROCEDURE");
      break;
    case MGOTOSTOP: printf ("MGOTOSTOP");
      break;
    case MSIGNDX: printf ("MSIGNDX");
      break;
    case MSIGNDI: printf ("MSIGNDI");
      break;
    case MSIGNDR: printf ("MSIGNDR");
      break;
    case MASSIGNADD: printf ("MASSIGNADD");
      break;
    case MSENDADDRESSTHUNKTOFORMALPAR: printf ("MSENDADDRESSTHUNKTOFORMALPAR");
      break;
    case MSENDVALUETHUNKTOFORMALPAR: printf ("MSENDVALUETHUNKTOFORMALPAR");
      break;
    }
}

/******************************************************************************
                                                                VALUE_PRINT  */
static void value_print (val_t value, int token)
{
  switch (token)
    {
    case MIFE:
    case MELSEE:
    case MIF:
    case MELSE:
    case MORELSEE:
    case MANDTHENE:
    case MCHARACTERKONST:
    case MINTEGERKONST:
    case MBOOLEANKONST:
      printf ("ival: %d ", value.ival);
      break;
    case MREALKONST:
      printf ("rval: %.16le ", value.rval);
      break;
    case MTEXTKONST:
      printf ("tval: %s id: %d ", value.tval.txt, value.tval.id);
      break;
    case MSTACK:
      printf ("entry: %d ", value.entry);
      break;
    case MNEWARG:
    case MNAMEREADACESS:
    case MNAMEWRITEACESS:
    case MPROCARG:
    case MCONC:
      printf ("n_of_stack_elements: %d ", value.n_of_stack_elements);
      break;
    case MNAMEREADTEXT:
    case MTEXTADR:
    case MNAMEADR:
    case MARRAYADR:
      /* Plus name */
      printf ("val_entry: %d ref_entry: %d txt_entry: %d ", 
	      value.stack.val_entry, value.stack.ref_entry, 
	      value.stack.txt_entry);
      break;
      printf ("ident: %s ", value.ident);
      break;
    case MSENDADDRESSTHUNKTOFORMALPAR:
    case MSENDVALUETHUNKTOFORMALPAR:
      printf ("label: %d inthunk: %d ", 
	      value.thunk.label, value.thunk.inthunk);
      break;

    case MGOTO:
    case MFOR:
    case MFORR:
    case MBLOCK:
    case MENDBLOCK:
    case MPRBLOCK:
    case MENDPRBLOCK:
    case MPROCEDURE:
    case MENDPROCEDURE:
    case MCLASS:
    case MENDCLASS:
    case MINSPECT:
    case MENDINSPECT:
    case MDO:
    case MENDDO:
    case MWHEN:
    case MENDWHEN:
    case MOTHERWISE:
    case MLISTSEP:
    case MFORWHILE:
    case MSTEP:
    case MUNTIL:
    case MFORDO:
    case MENDFOR:
    case MWHILE:
    case MENDWHILE:
    case MENDIF:
    case MINNER:
    case MSWITCH:
    case MSWITCHSEP:
    case MENDSWITCH:
    case MASSIGN:
    case MASSIGNR:
    case MENDASSIGN:
    case MARRAY:
    case MARRAYSEP:
    case MENDARRAY:
    case MBOUNDPARSEP:
    case MBOUNDSEP:
    case MENDLABEL:
    case MCONST:
    case MEQV:
    case MIMP:
    case MOR:
    case MAND:
    case MNOT:
    case MEQ:
    case MNE:
    case MLT:
    case MLE:
    case MGT:
    case MGE:
    case MNER:
    case MEQR:
    case MIS:
    case MINS:
    case MUADD:
    case MUSUB:
    case MADD:
    case MSUB:
    case MMUL:
    case MDIV:
    case MINTDIV:
    case MPRIMARY:
    case MNOOP:
    case MNONE:
    case MIDENTIFIER:
    case MTHIS:
    case MDOT:
    case MQUA:
    case MARGUMENT:
    case MARGUMENTSEP:
    case MENDSEP:
    case MARRAYARG:
    case MREFASSIGNT:
    case MEQT:
    case MNET:
    case MLTT:
    case MLET:
    case MGTT:
    case MGET:
    case MNERT:
    case MEQRT:
    case MUADDI:
    case MUSUBI:
    case MADDI:
    case MSUBI:
    case MMULI:
    case MPRIMARYII:
    case MPRIMARYRI:
    case MQUANOTNONE:
    case MQUANONEAND:
    case MREAINT:
    case MINTREA:
    case MDOTARRAYADR:
    case MVALASSIGNT:
    case MLABEL:
    case MDOTCONST:
    case MPROCASSIGN:
    case MFLAG:
    case MMODULE:
    case MTHEN:
    case MENDDECL:
    case MENTRY:
    case MBLOCKENTRY:
    case MPROCEDUREENTRY:
    case MSL:
    case MSENTCONC:
    case MINSTRONGEST:
    case MEXITARGUMENT:
    case MASSIGND:
    case MORELSE:
    case MANDTHEN:
    case MTHUNKSIMPLEADDRESS:
    case MTHUNKSIMPLEVALUE:
    case MTHUNKLABLE:
    case MTHUNKARRAY:
    case MTHUNKPROCEDURE:
    case MGOTOSTOP:
    case MSIGNDX:
    case MSIGNDI:
    case MSIGNDR:
    case MASSIGNADD:
      break;
    }
}

/******************************************************************************
                                                               PRINT_INDENT  */


static void print_indent (int level, char marker)
{
  int i;
  for (i=0; i<level; i++) 
    if (i/3*3 == i) putchar (marker); else putchar (' ');
}

/******************************************************************************
                                                                   EXP_DUMP  */
void exp_dump (exp_t *exp, int level)
{
  print_indent (level, '.');
  token_print (exp->token);
  printf (" ");
  value_print (exp->value, exp->token);
  if (exp->left!= NULL && exp->left->up != exp)
    printf ("exp->left->up != exp ");
  if (exp->right!= NULL && exp->right->up != exp)
    printf ("exp->right->up != exp ");
  printf ("\n");
  if (exp->left != NULL) exp_dump (exp->left, level+1);
  if (exp->right != NULL) exp_dump (exp->right, level+1);
}

/******************************************************************************
                                                                  SENT_DUMP  */
void sent_dump (sent_t *parent_sent, int level)
{
  sent_t *sent;
  print_indent (level, '_');
  token_print (parent_sent->token);
  printf (" line: %d cblock: %d ", parent_sent->line , (int) parent_sent->cblock);
  printf ("\n");
  if (parent_sent->iexp != NULL)
    {
      print_indent (level+1, '_'); 
      printf ("iexp:\n");
      exp_dump (parent_sent->iexp, level+1);
    }
  if (parent_sent->exp != NULL)
    {
      print_indent (level+1, '_'); 
      printf ("exp:\n");
      exp_dump (parent_sent->exp, level+1);
    }
  for (sent= parent_sent->first; sent!=NULL; sent= sent->next)
    sent_dump (sent, level+1);
}

#endif
