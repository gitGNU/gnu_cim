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

#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_mell;
static char *first_object_allocated_ptr_mell;
static char *last_object_allocated_ptr_mell;

char *mpointer;

/******************************************************************************
                                                                MBUILDERINIT */

mbuilder_init()
{
  obstack_init(&os_mell);
}

/******************************************************************************
                                                                        MOUT */

mout(x)unsigned char x;
{
  obstack_1grow(&os_mell, x);
}

/******************************************************************************
                                                                    MOUTIVAL */

mout_ival(x)long x;
{
  obstack_grow(&os_mell, &x, sizeof (long));
}

/******************************************************************************
                                                                    MOUTRVAL */

mout_rval(x)double x;
{
  obstack_grow(&os_mell, &x,sizeof (double));
}

/******************************************************************************
                                                                    MOUTTVAL */

mout_tval(x)char *x;
{
  obstack_grow(&os_mell, &x, sizeof (char *));
}

/******************************************************************************
                                                                      MOUTID */

mout_id(x)char *x;
{
  obstack_grow(&os_mell, &x, sizeof (char *));
}

/******************************************************************************
                                                              MBUILDERREINIT */

mbuilder_init_pass2()
{
  long i;
  i= obstack_object_size(&os_mell);
  mpointer= first_object_allocated_ptr_mell= (char *)obstack_finish (&os_mell);
  last_object_allocated_ptr_mell= mpointer+i;
}

/******************************************************************************
                                                                         MIN */

int 
min()
{
  unsigned char x;
  if (mpointer>=last_object_allocated_ptr_mell) return -1;
  x= *(mpointer++);
  return (x);
}

/******************************************************************************
                                                                     MINIVAL */

long 
min_ival()
{
  long x;
  memmove (&x,mpointer,sizeof(long));
  mpointer += sizeof(long);
  return (x);
}

/******************************************************************************
                                                                     MINRVAL */

double 
min_rval()
{
  double x;
  memmove (&x,mpointer,sizeof (double));
  mpointer += sizeof (double);
  return (x);
}

/******************************************************************************
                                                                     MINTVAL */

char *
min_tval()
{
  char *x;
  memmove(&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}

/******************************************************************************
                                                                       MINID */

char *
min_id()
{
  char *x;
  memmove (&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}

/******************************************************************************
                                                                       MINID */

void 
mbuilder_reinit()
{
  if (first_object_allocated_ptr_mell == 0)
    first_object_allocated_ptr_mell= (char *)obstack_finish (&os_mell);

  obstack_free (&os_mell, first_object_allocated_ptr_mell);

  first_object_allocated_ptr_mell= 0;
}

