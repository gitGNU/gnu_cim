/* $Id: limit.h,v 1.3 1994/12/24 21:04:29 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen,
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

#include "config.h"

/* lex.c */
#define IFDEFMAXNIV 100
#define YYTEXTLENGTH 1010
#define SAFEYYTEXTLENGTH 1000
#define INCLUDELEV 10
#define MAX_TEXT_CHAR 65535

/* simula.c */
#define MAXANTSIMLIB 100
#define MAXANTLIBDIR 100

/* navn.c */
#define NAMEPRIMEVALUE 1951	/* 4877 */

/* dekl.c */
#define DECLTABSIZE 100

/* Parser.y/c */
#define YYMAXDEPTH 1500
#define REMBLOCKSIZE 500

/* sjekker.c */
#define MAXEXPBUFFER 1000
#define MAXEXPDISPLAY 1000

/* gen.c */
#define MAINROUTINELENGTH 30
#define MAXLABELSTAKK 1000
#define REALASDOUBLE 1
#define DEF_PLEV_TAB_SIZE 8	/* Default size on prefix-table. To big value
				 * gives big prototypes. But better code for
				 * subclasses with that plev. */
/* genexp.c */
#define STACK_SIZE 100
#define MAX_ARRAY_DIM 100
