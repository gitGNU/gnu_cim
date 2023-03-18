/* $Id: limit.h,v 1.3 1994/12/24 21:04:29 cim Exp $ */

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

#include "config.h"
#include <limits.h>

/* lex.c */
#define MAX_TEXT_CHAR 65535  /* Max size of text-objects DO NOT EDIT */

/* genexp.c */
#define STACK_SIZE 100       /* Size of stack used by genexp */
#define MAX_ARRAY_DIM 100    /* The maximum number of dimensions for arrays */

/* Define the adress of the first data location.  */
#define FIRST_DATA_LOCATION ((int)(&__start_data_segment))

/* Define MAX_INT */
#define MAX_INT INT_MAX

/* Define TYPE_32_INT */
#define TYPE_32_INT int
