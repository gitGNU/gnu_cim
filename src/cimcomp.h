/* $Id: cim.c,v 1.18 1997/01/26 14:30:21 cim Exp $ */

/* Copyright (C) 1987-1998 Sverre Hvammen Johansen,
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

#include <stdio.h>

extern char separat_comp;

extern char *includedir;
extern char *extcodename;
extern char *mifcodename;
extern char *sourcename;
extern char *ccodename;
extern char *progname;

extern FILE *ccode;

extern int option_write_tokens;
extern int option_write_mif;
extern int option_nowarning;
extern int option_atr;
extern int option_line;
extern int option_reuse_timestamp;
extern int option_verbose;
extern int option_init_poolsize;
extern int option_max_poolsize;
extern int option_dyn_poolsize;
extern int option_bl_in_dir_line;
extern int option_quiet;

extern int option_lex;
extern int option_declarations;
extern int option_input;
extern int option_dump_build;
extern int option_dump_check;
extern int option_dump_trans;

extern struct SENT *main_sent;

