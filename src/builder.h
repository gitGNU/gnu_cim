/* $Id: $ */

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

#include "dekl.h"
#include "lex.h"

#define is_after_dot(re) (re->up->token==MDOT && re->up->right==re)

/* expbuilder.c */

extern struct EXP *newexp();
extern struct EXP *makeexp();
extern struct EXP *concexp();
extern struct EXP *replacenode(); 
extern struct EXP *elook();
extern remove_dot ();

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
    long line;
  };

extern int token;
extern int build_lineno;

extern struct EXP *copytree ();

/* sentbuilder.c */

struct SENT
{
  int token, line;
  char nonetest, indextest, stripsideeffects;
  struct BLOCK *cblock;
  struct EXP *iexp;
  struct EXP *exp;
  struct SENT *prev, *next;
  struct SENT *first,*last;
  int last_line;
};

extern void insert_thunk ();
extern void insert_after_sent ();
extern void insert_before_sent ();
extern void remove_sent ();
struct SENT *new_sent();
struct SENT *sbuild();
void set_flag ();

extern char nonetest;
extern char indextest;
extern char stripsideeffects;
