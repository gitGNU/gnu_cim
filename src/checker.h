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

#include "builder.h"

/* In computeconst.c */

char computeconst (exp_t *re);
char setdanger_const (exp_t *re);
int sstrlen (char s[]);

/* In sentchecker.c */

void sent_check (sent_t *parent_sent, char res_labels);

/* In expchecker.c */

void exp_checker_init_pass2 (void);
void main_exp_check (exp_t *re);
