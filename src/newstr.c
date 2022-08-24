/* $Id: cim.c,v 1.18 1997/01/26 14:30:21 cim Exp $ */

/* Copyright (C) 1987-1998 Sverre Hvammen Johansen,
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
#include "obstack.h"
#include "newstr.h"
#include "config.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#if STDC_HEADERS
#include <stdlib.h>
#endif


char *xmalloc();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_newstr;

/******************************************************************************
                                                                  INITNEWSTR */
void init_newstr (void)
{
  obstack_init(&os_newstr);
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstr_grown(char *s1, int n)
{
  obstack_grow (&os_newstr, s1, n);
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstr_grow1(char *s1)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstr_grow2(char *s1, char *s2)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow (&os_newstr, s2, strlen(s2));
}

/******************************************************************************
                                                                NEWSTRFINISH */

char *newstr_finish(void)
{
  obstack_1grow (&os_newstr, 0);
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT1 */

char *newstrcat1(char *s1)
{
  obstack_grow0 (&os_newstr, s1, strlen(s1));
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT2 */

char *newstrcat2(char *s1, char *s2)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow0 (&os_newstr, s2, strlen(s2));
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT3 */

char *newstrcat3(char *s1, char *s2, char *s3)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow (&os_newstr, s2, strlen(s2));
  obstack_grow0 (&os_newstr, s3, strlen(s3));
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT4 */

char *newstrcat4(char *s1, char *s2, char *s3, char *s4)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow (&os_newstr, s2, strlen(s2));
  obstack_grow (&os_newstr, s3, strlen(s3));
  obstack_grow0 (&os_newstr, s4, strlen(s4));
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT5 */

char *newstrcat5(char *s1, char *s2, char *s3, char *s4, char *s5)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow (&os_newstr, s2, strlen(s2));
  obstack_grow (&os_newstr, s3, strlen(s3));
  obstack_grow (&os_newstr, s4, strlen(s4));
  obstack_grow0 (&os_newstr, s5, strlen(s5));
  return obstack_finish (&os_newstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT6 */

char *newstrcat6(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6)
{
  obstack_grow (&os_newstr, s1, strlen(s1));
  obstack_grow (&os_newstr, s2, strlen(s2));
  obstack_grow (&os_newstr, s3, strlen(s3));
  obstack_grow (&os_newstr, s4, strlen(s4));
  obstack_grow (&os_newstr, s5, strlen(s5));
  obstack_grow0 (&os_newstr, s6, strlen(s6));
  return obstack_finish (&os_newstr);;
}
