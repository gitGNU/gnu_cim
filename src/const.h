/* $Id: cconst.h,v 1.6 1994/10/15 13:30:17 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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

#include "limit.h"

/* lex.c */
#define IFGREN 2
#define ELSEGREN 0
#define SCAN 4
#define DIRTEXTLENGTH 20

/* name.c */
#define NAMEPRIMEVALUE 1951	/* 4877 */

/* setenv.c */
#define SIMENVIR "simenvir.dta"

/* dekl.c */
#define MAXRANK 255
#define USPECDIM -1
#define STARTBLNO 0
#define STOPLABEL 1		/* Related to l1: in cgen.c */
#define STARTLABEL 2

/* The following symbols is used in other modules than dekl.c */
#define EXTERNALGLOBALBLEV 1
#define SYSTEMGLOBALBLEV 0

/* Parser.y/c */
#define SIMULATIONATRFILE "simulation.atr"
#define SIMSETATRFILE "simset.atr"
#define FILEATRFILE "simfile.atr"
#define DIRECT	1
#define AT	2
#define DELAYS	3
#define BEFORE	4
#define AFTER	5

/* gen.c */
#define REALASDOUBLE 1
#define ACSTACK_IN_OBJ 1

/* Some boolean constants and so on */
#define ON 	1
#define OFF 	0
#define FREE 	1
#define NOTFREE 0
#define TEXTATTR 1
#define NOTTEXTATTR 0
#define TRUE 1
#define FALSE 0
#define MAYBEE 2
#define NOSYMBOL 0
#define NOTEXT 0
#define DANGER 1
#define NOTDANGER 0

/* gen.c */
#define DEF_PLEV_TAB_SIZE 8	/* Default size on prefix-table. To big value
				 * gives big prototypes. But better code for
				 * subclasses with that plev. */
