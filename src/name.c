/* $Id: cnavn.c,v 1.6 1994/07/17 10:40:51 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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

/* Navnelager for Simula */

#include "const.h"
#include "dekl.h"
#include "name.h"

#include <stdio.h>

#if STDC_HEADERS
#include <stdlib.h>
#endif

#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_name;

static void *hashlist[NAMEPRIMEVALUE];

/******************************************************************************
                                                                   TAG       */

char *
tag (t)
     unsigned char t[];
{
  int leng;
  long hash;
  void * * list, * *prevlist = NULL;

  leng= strlen (t);
  if (leng > 3)
    hash = t[0] + 8 * t[1] + 64 * t[2] + 512L * t[leng - 1] + leng;
  else
    hash = t[0] + 512L * t[leng - 1] + leng;
  hash = hash % NAMEPRIMEVALUE;
  for (list= hashlist[hash]; list != NULL; list= * list)
    {
      if (!strcmp ((char *) (list+1), t))
	return (char *) (list+1);
      prevlist = list;
    }

  obstack_ptr_grow(&os_name, NULL);
  obstack_grow0 (&os_name, t, leng);
  obstack_1grow (&os_name, FALSE);
  list= obstack_finish (&os_name);

  if (prevlist == NULL)
    hashlist[hash] = list;
  else
    *prevlist = list;

  return (char *) (list + 1);
}

/******************************************************************************
                                                    DEFINENAME & IFDEFNAME   */

define_name (t, d) char *t; char d;
{
  while (*t != 0) t++;
  t++;
  *t= d;
}

char
ifdef_name (t) char *t;
{
  while (*t != 0) t++;
  t++;
  return *t;
}


/******************************************************************************
                                                                    INITNAME */

init_name ()
{
  obstack_init(&os_name);
  define_name (tag ("CIM"), TRUE);
#if INT_64
  define_name (tag ("INTEGER_64"), TRUE);
#endif

  define_name (tag (CPU_TYPE), TRUE);
  define_name (tag (MANUFACTURER), TRUE);
  define_name (tag (OS_TYPE), TRUE);
  define_name (tag (OS_TYPE_VERSION), TRUE);
}
