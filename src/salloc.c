/* $Id: salloc.c,v 1.10 1997/01/08 09:49:13 cim Exp $ */

/* Copyright (C) 1999 Sverre Hvammen Johansen,
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
#include "cimcomp.h"
#include <stdio.h>
#include <obstack.h>

#if STDC_HEADERS
#include <stdlib.h>
#endif

void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_salloc;
static char *first_object_allocated_ptr_salloc;

/******************************************************************************
                                                                     XMALLOC */

char *xmalloc (unsigned int size)
{
  char *ptr = malloc (size);
  if (! ptr)
    {
      fprintf (stderr, "%s: virtual memory exhausted\n", progname);
      exit (1);
    }
  return ptr;
}

/******************************************************************************
                                                                      SALLOC */

char *salloc (int size)
{
  char *p;
  p= obstack_alloc (&os_salloc, size);
  memset (p, 0, size);
  return p;
}

/******************************************************************************
                                                                 SALLOC_INIT */

void salloc_init (void)
{
  obstack_init(&os_salloc);
  first_object_allocated_ptr_salloc= obstack_alloc (&os_salloc, 0);
}

/******************************************************************************
                                                               SALLOC_REINIT */

void salloc_reinit (void)
{
  obstack_free (&os_salloc, first_object_allocated_ptr_salloc);
}

#if 0
char *progname;
main(argc,argv)int argc; char *argv[];
{
  char *p1, *p2, *p3, *p4;
  salloc_init ();
  progname= argv[0];
  while (1)
    {
      salloc(1);
      salloc(1);
      salloc(4);
      salloc(8);
      salloc_reinit ();
    }
}
#endif
