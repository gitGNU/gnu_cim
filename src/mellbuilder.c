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

static struct obstack osMell;

static char *mstring,  *mend;
char *mpointer;

/******************************************************************************
                                                                MBUILDERINIT */

mbuilderInit()
{
  obstack_init(&osMell);
}

/******************************************************************************
                                                                        MOUT */

mout(x)unsigned char x;
{
  obstack_1grow(&osMell, x);
}

/******************************************************************************
                                                                    MOUTIVAL */

moutIval(x)long x;
{
  obstack_grow(&osMell, &x, sizeof (long));
}

/******************************************************************************
                                                                    MOUTRVAL */

moutRval(x)double x;
{
  obstack_grow(&osMell, &x,sizeof (double));
}

/******************************************************************************
                                                                    MOUTTVAL */

moutTval(x)char *x;
{
  obstack_grow(&osMell, &x, sizeof (char *));
}

/******************************************************************************
                                                                      MOUTID */

moutId(x)char *x;
{
  obstack_grow(&osMell, &x, sizeof (char *));
}

/******************************************************************************
                                                              MBUILDERREINIT */

mbuilderReinit()
{
  long i;
  i= obstack_object_size(&osMell);
  mpointer= mstring= (char *)obstack_finish (&osMell);
  mend= mpointer+i;
}

/******************************************************************************
                                                                         MIN */

int min()
{
  unsigned char x;
  if (mpointer>=mend) return -1;
  x= *(mpointer++);
  return (x);
}

/******************************************************************************
                                                                     MINIVAL */

long minIval()
{
  long x;
  memmove (&x,mpointer,sizeof(long));
  mpointer += sizeof(long);
  return (x);
}

/******************************************************************************
                                                                     MINRVAL */

double minRval()
{
  double x;
  memmove (&x,mpointer,sizeof (double));
  mpointer += sizeof (double);
  return (x);
}

/******************************************************************************
                                                                     MINTVAL */

char *minTval()
{
  char *x;
  memmove(&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}

/******************************************************************************
                                                                       MINID */

char *minId()
{
  char *x;
  memmove (&x,mpointer,sizeof(char *));
  mpointer += sizeof(char *);
  return(x);
}


