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

struct stamp
  {
    char *timestamp;
    char *filename;
    struct stamp *next;
    char lest_inn;
  };

void gettimestamp ();
extern extspec_init ();
extern extspec_reinit ();
extern char *directive_timestamp;
extern char *timestamp;
extern lesinn_external_spec ();
extern more_modules ();
extern struct stamp *first_stamp;
extern skrivut_external_spec ();
