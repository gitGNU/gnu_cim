/* $Id: cmell.h,v 1.7 1994/11/02 14:58:23 cim Exp $ */

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


#define setntokens(t) (t<=39)
#define exprvaltokens(t) (t>=40 && t<=44)
#define expridtokens(t) ((t>=45 && t<=48) || t==81 || t==135)

#define MERROR   127
#define MNEWLINE 0
#define MSTOP    -1

#define MBLOCK          1
#define MENDBLOCK       2

#define MPRBLOCK        3
#define MENDPRBLOCK     4

#define MPROCEDURE      5
#define MENDPROCEDURE   6

#define MCLASS          7
#define MENDCLASS       8

#define MINSPECT        9
#define MENDINSPECT     11
#define MDO             12
#define MENDDO          13
#define MWHEN           35	/* id */
#define MENDWHEN        14
#define MOTHERWISE      15
#define MENDOTHERWISE   16

#define MFOR                    85
#define MFORR                   86
#define MLISTSEP                50
#define MFORWHILE               51
#define MSTEP                   52
#define MUNTIL                  53
#define MFORDO          17
#define MENDFOR         18

#define MWHILE          19
#define MENDWHILE       21

#define MIF             22
#define MELSE           24
#define MENDIF          25

#define MGOTO           26

#define MINNER          28

#define MSWITCH                 87
#define MSWITCHSEP              83
#define MENDSWITCH      29

#define MASSIGN                 84
#define MASSIGNR                91
#define MENDASSIGN      30

#define MARRAY                  88
#define MARRAYSEP               89
#define MENDARRAY       31
#define MBOUNDPARSEP            54
#define MBOUNDSEP               55

#define MENDLABEL       32
#define MCONST          34
#define MIFE                    56
#define MELSEE                  57

#define MORELSE                 58
#define MANDTHEN                59
#define MEQV                    60
#define MIMP                    61
#define MOR                     62
#define MAND                    63
#define MNOT                            100

#define MEQ                     64
#define MNE                     65
#define MLT                     66
#define MLE                     67
#define MGT                     68
#define MGE                     69

#define MNER                    70
#define MEQR                    71

#define MIS                     72
#define MINS                    73

#define MUADD                           101
#define MUSUB                           102
#define MADD                    74
#define MSUB                    75
#define MMUL                    76
#define MDIV                    77
#define MINTDIV                 78
#define MPRIMARY                79

#define MNOOP                           103

#define MTEXTKONST              40	/* val */
#define MCHARACTERKONST         41	/* val */
#define MREALKONST              42	/* val */
#define MINTEGERKONST           43	/* val */
#define MBOOLEANKONST           44	/* val */
#define MNONE                   49

#define MIDENTIFIER             45	/* id */
#define MTHIS                   46	/* id */
#define MDOT                    80
#define MNEWARG                 47	/* id */
#define MQUA                    48	/* id */

#define MARGUMENT               81
#define MARGUMENTSEP            82

#define MENDSEP                 90


#define MPROCARG        105
#define MARRAYARG       106

#define MREFASSIGNT     107


#define MEQT            108
#define MNET            109
#define MLTT            110
#define MLET            111
#define MGTT            112
#define MGET            113

#define MNERT           114
#define MEQRT           115

#define MUADDI          116
#define MUSUBI          117

#define MADDI           118
#define MSUBI           119
#define MMULI           120

#define MPRIMARYII      121
#define MPRIMARYRI      122

#define MQUANOTNONE     123
#define MQUANONEAND     124

#define MREAINT         125
#define MINTREA         126

#define MSTACK                128

#define MARRAYADR       131
#define MDOTARRAYADR    132
#define MVALASSIGNT     133
#define MTEXTADR        134
#define MLABEL          135
#define MCONC           136
#define MDOTCONST	137
#define MNAMEADR        138
#define MPROCASSIGN	139

#define MFLAG		99
#define MNONETEST	2
#define MINDEXTEST	4
#define MSTRIPSIDEEFFECTS 6
