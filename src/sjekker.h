/* $Id: csjekker.h,v 1.2 1994/07/17 10:41:06 cim Exp $ */

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

#include "dekl.h"

/* Objekter til aa generere uttrykk-treet */
struct EXP
  {
    int token;
    union val value;
    char type,
      danger,
      konst;
    struct EXP *left,
     *right,
     *up;
    struct DECL *rd;
    struct DECL *qual;
    struct BLOCK *seenthrough;
  };

extern initsjekker();
extern sjekker ();
extern struct EXP *copytree ();
extern int sstrlen ();
extern char not_reached;
extern int cexpr;
extern int sjerror;
extern struct EXP expbuffer[MAXEXPBUFFER];
extern struct EXP owerflowbuffer;
