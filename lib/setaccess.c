/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen,
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "cim.h"
#include <stdio.h>
#include "simfile.h"

/******************************************************************************
                                                BOOLEAN PROCEDURE SETACCESS  */


/* Hjelpe rutine som sammenligner to stringer.Sm} og store bokstaver */
/* omhverandre                                                       */

char 
__rcompstr (s1, s2, n)
     char *s1,
      *s2;
     int n;
{				/* String s2 best}r av store bokstaver */
  int i;
  char c;
  for (i = 0; i < n; i++)
    {
      c = s1[i];
      if ((islower (c) ? toupper ((int) c) : c) != s2[i])
	return (__FALSE);
    }
  return (__TRUE);
}

char 
__rsetaccess (p, t)
     __bs1FILE *p;
     __txtvp t;
{
  long length;
  char *s;
  __pty ppx;
  if (t->obj == __NULL)
    return (__FALSE);
  length = t->length;
  s = &t->obj->string[t->start - 1];
  ppx = p->h.pp;
  if (length == 6 && __rcompstr (s, "SHARED", 6))
    {
      p->shared = __SHARED;
      return (__TRUE);
    }
  if (length == 8 && __rcompstr (s, "NOSHARED", 8))
    {
      p->shared = __NOSHARED;
      return (__TRUE);
    }
#if HAVE_UNLINK
  if (length == 5 && __rcompstr (s, "PURGE", 5))
    {
      p->purge = __PURGE;
      return (__TRUE);
    }
#endif
  if (length == 7 && __rcompstr (s, "NOPURGE", 7))
    {
      p->purge = __NOPURGE;
      return (__TRUE);
    }
  if (ppx->pref[2] == &__p5FILE || ppx->pref[2] == &__p10FILE) 
                                              /* DIRECT FILES */
    {
      if (length == 8 && __rcompstr (s, "READONLY", 8))
	{
	  p->readwrite = __READONLY;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "WRITEONLY", 9))
	{
	  p->readwrite = __WRITEONLY;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "READWRITE", 9))
	{
	  p->readwrite = __READWRITE;
	  return (__TRUE);
	}
    }
  else
    /* IN AND OUT FILES */
    {
      if (length == 6 && __rcompstr (s, "REWIND", 6))
	{
	  p->re_wind = __REWIND;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOREWIND", 8))
	{
	  p->re_wind = __NOREWIND;
	  return (__TRUE);
	}
    }
  if (ppx->pref[1] == &__p7FILE)	/* BYTE FILES */
    {
      if (length == 10 && __rcompstr (s, "BYTESIZE:8", 10))
	return (__TRUE);
      if (length == 10 && __rcompstr (s, "BYTESIZE:0", 10))
	return (__TRUE);
    }
  if (!(ppx->pref[2] == &__p4FILE || ppx->pref[2] == &__p8FILE))	
                                        /* NOT IN FILES */
    {
      if (length == 6 && __rcompstr (s, "APPEND", 6))
	{
	  p->append = __APPEND;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOAPPEND", 8))
	{
	  p->append = __NOAPPEND;
	  return (__TRUE);
	}
      if (length == 6 && __rcompstr (s, "CREATE", 6))
	{
	  p->create = __CREATE;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOCREATE", 8))
	{
	  p->create = __NOCREATE;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "ANYCREATE", 9))
	{
	  p->create = __ANYCREATE;
	  return (__TRUE);
	}
    }
  return (__FALSE);
}
