/* $Id: cextspec.c,v 1.17 1997/01/26 14:30:17 cim Exp $ */

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

#include <stdio.h>

extern void initFilelist ();
char insert_name_in_dirlist ();
char insert_name_in_archlist ();
char insert_name_in_linklist ();
char *get_names_in_linklist ();
char * transform_name();
FILE *searc_and_open_name_in_archlist ();
void new_lib ();

