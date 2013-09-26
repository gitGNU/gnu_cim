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

#ifndef BUILDER_H
#define BUILDER_H

#include "dekl.h"
#include "lex.h"
#include "mellbuilder.h"

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

void ebuilder_init (void);
void ebuilder_init_pass2 (void);

exp_t *makeexp (int token, exp_t *left, exp_t *right);
exp_t *concexp (exp_t *left, exp_t *right);
exp_t *replacenode (exp_t **rep, int token);
exp_t *elook(void);
void remove_dot (exp_t **rep);

void ebuild (void);

extern int token;
extern int build_lineno;

exp_t *copytree (exp_t *re);

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

void sbuilder_init(void);
void insert_thunk (exp_t *rex, int token);
void insert_after_sent (sent_t *parent, sent_t *after, sent_t *new);
void insert_before_sent (sent_t *parent, sent_t *before, sent_t *new);
void remove_sent(sent_t *parent, sent_t *rem);
sent_t * new_sent(int token);
sent_t *sbuild(void);
void set_flag (void);

extern char nonetest;
extern char indextest;
extern char stripsideeffects;
#endif
