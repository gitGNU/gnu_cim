/* $Id: csjekker.h,v 1.2 1994/07/17 10:41:06 cim Exp $ */

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

#include "cdekl.h"

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

/* Brytere som kan sl}es AV/P} ved hjelp av kompilator-direktiver */
extern char nonetest,
  indextest,
  stripsideeffects,
  nameasvar;
