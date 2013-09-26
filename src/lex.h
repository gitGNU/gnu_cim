/* $Id: clex.c,v 1.15 1997/02/14 16:33:00 cim Exp $ */

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

#include "parser.h"

extern char external;
extern long lineno;
extern char nameasvar;
extern char sensitive;
extern char staticblock;

int yylex (void);
void lex_init (void);
int lex_init_pass1 (char *sourcename);
void lex_reinit (void);
void scan_and_write_tokens (void);
void print_lexsymbol (int lextok, YYSTYPE *yylvalp);
