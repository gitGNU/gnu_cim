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

typedef struct _exp exp_t;
struct _exp
  {
    token_t token;
    val_t value;
    char type,
      danger,
      konst;
    exp_t *left,
     *right,
     *up;
    decl_t *rd;
    decl_t *qual;
    block_t *seenthrough;
    long line;
  };

/* expbuilder.c */

extern exp_t *newexp();
extern exp_t *makeexp();
extern exp_t *concexp();
extern exp_t *replacenode(); 
extern exp_t *elook();
extern remove_dot ();

extern int token;
extern int build_lineno;

extern exp_t *copytree ();

/* sentbuilder.c */

typedef struct _sent sent_t;
struct _sent
{
  token_t token;
  int line;
  char nonetest, indextest, stripsideeffects;
  block_t *cblock;
  exp_t *iexp;
  exp_t *exp;
  sent_t *prev, *next;
  sent_t *first,*last;
  int last_line;
};

extern void insert_thunk ();
extern void insert_after_sent ();
extern void insert_before_sent ();
extern void remove_sent ();
sent_t *new_sent();
sent_t *sbuild();
void set_flag ();

extern char nonetest;
extern char indextest;
extern char stripsideeffects;
