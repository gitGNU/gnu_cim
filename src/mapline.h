/* $Id: cfeil.c,v 1.19 1995/03/07 10:53:16 cim Exp $ */

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

#include <stdio.h>

extern int maplineInit ();
extern int pushfilmap ();
extern void popfilmap ();
extern void setfilmap ();
extern long getmapline ();
extern char *getmapfile ();
extern void genmap ();
extern void * includeIfdefp ();
extern FILE *includeFile ();
extern int noFilemap ();
