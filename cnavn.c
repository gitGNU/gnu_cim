/* $Id: cnavn.c,v 1.6 1994/07/17 10:40:51 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs
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

#include "cextern.h"
#include "cdekl.h"
#include "cnavn.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

struct namebuff *hashlist[NAMEPRIMEVALUE];

/******************************************************************************
                                                                SYSTAG       */

struct namebuff *
systag (t, leng)
     unsigned char t[];
     int leng;
{
  long hash;
  struct namebuff *list,
   *prevlist = NULL;
  if (leng > 3)
    hash = t[0] + 8 * t[1] + 64 * t[2] + 512L * t[leng - 1] + leng;
  else
    hash = t[0] + 512L * t[leng - 1] + leng;
  hash = hash % NAMEPRIMEVALUE;
  for (list = hashlist[hash]; list != NULL; list = list->next)
    {
      if (!strcmp (list->text, t))
	return (list);
      prevlist = list;
    }
  (void) strcpy ((list = (struct namebuff *) 
		  salloc (sizeof (struct namebuff *) 
			  + sizeof (long) + leng + 1))->text, t);
  list->next = NULL;
  if (prevlist == NULL)
    hashlist[hash] = list;
  else
    prevlist->next = list;
  list->definition = FALSE;
  return (list);
}

/******************************************************************************
                                                                TAG          */

char *
tag (t, leng)
     unsigned char t[];
     int leng;
{
  return (systag (t, leng)->text);
}

/******************************************************************************
                                                                SALLOC       */

/*VARARGS0 */
long 
salloc (size)
     int size;
{
  long ret;
  ret = (long) malloc ((unsigned) size);
  if (ret == (long) NULL)
    lerror (13);
  return (ret);
}

/******************************************************************************
                                                                 INIT_DEFINE */

init_define ()
{
  systag ("CIM", 3)->definition = TRUE;
#if FLOAT_IMPLEMENTED
#else
  systag ("NO_REAL", 7)->definition = TRUE;
  systag ("INTEGER_SIMULATION", 18)->definition = TRUE;
#endif
#if INT_64
  systag ("INTEGER_64", 10)->definition = TRUE;
#endif

  systag (CPU_TYPE, strlen (CPU_TYPE))->definition = TRUE;
  systag (MANUFACTURER, strlen (MANUFACTURER))->definition = TRUE;
  systag (OS_TYPE, strlen (OS_TYPE))->definition = TRUE;
  systag (OS_TYPE_VERSION, strlen (OS_TYPE_VERSION))->definition = TRUE;
}
