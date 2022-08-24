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

#include "obstack.h"
#include "mellbuilder.h"
#include "config.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "config.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

char *xmalloc();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_mell;
static char *first_object_allocated_ptr_mell;
static char *last_object_allocated_ptr_mell;

char *mpointer;

/******************************************************************************
                                                                MBUILDERINIT */

void mbuilder_init(void)
{
  obstack_init(&os_mell);
}

/******************************************************************************
                                                                        MOUT */

void mout(unsigned char x)
{
  obstack_1grow(&os_mell, x);
}

/******************************************************************************
                                                                    MOUTIVAL */

void mout_ival(long x)
{
  obstack_grow(&os_mell, &x, sizeof (long));
}

/******************************************************************************
                                                                    MOUTRVAL */

void mout_rval(double x)
{
  obstack_grow(&os_mell, &x,sizeof (double));
}

/******************************************************************************
                                                                    MOUTTVAL */

void mout_tval(char *x)
{
  obstack_grow(&os_mell, &x, sizeof (char *));
}

/******************************************************************************
                                                                      MOUTID */

void mout_id(char *x)
{
  obstack_grow(&os_mell, &x, sizeof (char *));
}

/******************************************************************************
                                                              MBUILDERREINIT */

void mbuilder_init_pass2(void)
{
  long i;
  i= obstack_object_size(&os_mell);
  mpointer= first_object_allocated_ptr_mell= (char *)obstack_finish (&os_mell);
  last_object_allocated_ptr_mell= mpointer+i;
}

/******************************************************************************
                                                                         MIN */

int min(void)
{
  unsigned char x;
  if (mpointer>=last_object_allocated_ptr_mell) return -1;
  x= *(mpointer++);
  return (x);
}

/******************************************************************************
                                                                     MINIVAL */

long min_ival(void)
{
  long x;
  memmove (&x,mpointer,sizeof(long));
  mpointer += sizeof(long);
  return (x);
}

/******************************************************************************
                                                                     MINRVAL */

double min_rval(void)
{
  double x;
  memmove (&x,mpointer,sizeof (double));
  mpointer += sizeof (double);
  return (x);
}

/******************************************************************************
                                                                     MINTVAL */

char *min_tval(void)
{
  char *x;
  memmove(&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}

/******************************************************************************
                                                                       MINID */

char *min_id(void)
{
  char *x;
  memmove (&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}

/******************************************************************************
                                                                       MINID */

void mbuilder_reinit(void)
{
  if (first_object_allocated_ptr_mell == 0)
    first_object_allocated_ptr_mell= (char *)obstack_finish (&os_mell);

  obstack_free (&os_mell, first_object_allocated_ptr_mell);

  first_object_allocated_ptr_mell= 0;
}
