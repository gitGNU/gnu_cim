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
#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack osNewstr;

/******************************************************************************
                                                                  INITNEWSTR */
void initNewstr ()
{
  obstack_init(&osNewstr);
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstrGrown(s1, n)
     char *s1; int n;
{
  obstack_grow (&osNewstr, s1, n);
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstrGrow1(s1)
     char *s1;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
}

/******************************************************************************
                                                                 NEWSTRGROW1 */

void newstrGrow2(s1, s2)
     char *s1, *s2;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow (&osNewstr, s2, strlen(s2));
}

/******************************************************************************
                                                                NEWSTRFINISH */

char *newstrFinish()
{
  obstack_1grow (&osNewstr, 0);
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT1 */

char * newstrcat1(s1)
     char *s1;
{
  obstack_grow0 (&osNewstr, s1, strlen(s1));
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT2 */

char * newstrcat2(s1, s2)
     char *s1, *s2;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow0 (&osNewstr, s2, strlen(s2));
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT3 */

char * newstrcat3(s1, s2, s3)
     char *s1, *s2, *s3;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow (&osNewstr, s2, strlen(s2));
  obstack_grow0 (&osNewstr, s3, strlen(s3));
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT4 */

char * newstrcat4(s1, s2, s3, s4)
     char *s1, *s2, *s3, *s4;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow (&osNewstr, s2, strlen(s2));
  obstack_grow (&osNewstr, s3, strlen(s3));
  obstack_grow0 (&osNewstr, s4, strlen(s4));
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT5 */

char * newstrcat5(s1, s2, s3, s4, s5)
     char *s1, *s2, *s3, *s4, *s5;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow (&osNewstr, s2, strlen(s2));
  obstack_grow (&osNewstr, s3, strlen(s3));
  obstack_grow (&osNewstr, s4, strlen(s4));
  obstack_grow0 (&osNewstr, s5, strlen(s5));
  return obstack_finish (&osNewstr);;
}

/******************************************************************************
                                                                  NEWSTRCAT6 */

char * newstrcat6(s1, s2, s3, s4, s5, s6)
     char *s1, *s2, *s3, *s4, *s5, *s6;
{
  obstack_grow (&osNewstr, s1, strlen(s1));
  obstack_grow (&osNewstr, s2, strlen(s2));
  obstack_grow (&osNewstr, s3, strlen(s3));
  obstack_grow (&osNewstr, s4, strlen(s4));
  obstack_grow0 (&osNewstr, s6, strlen(s6));
  return obstack_finish (&osNewstr);;
}

