/* $Id: rcproc.c,v 1.3 1994/07/17 10:41:14 cim Exp $ */

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

/* RT-rutiner for } overf|re parametere til c-prosedyrer */

#include "cim.h"
#include "config.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#include <strings.h>
#endif /* not STDC_HEADERS and not HAVE_STRING_H */

/******************************************************************************
                                                     RCOPYTEXTTOC            */

/* Denne rutinen brukes for } overf|re en text-variabel by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc, for s}
 * } kopiere teksten over i dette omr}det. Teksten blir terminert med 0 */

#if nolib|rcopytexttocrut
char *
__rcopytexttoc (t)
     __txtvp t;
{
  char *p;

  p = (char *) malloc ((unsigned) t->length + 1);

  (void) strncpy (p, &t->obj->string[t->start - 1], t->length);
  p[t->length] = '\0';
  return (p);
}
#endif

/******************************************************************************
                                                     RADDROFFIRSTCHAR        */

/* Denne rutinen brukes for } overf|re en text-variabel by reference til en
 * C-prosedyre. Rutinen returnerer med en peker til f|rste tegn i teksten */

#if nolib|raddroffirstcharrut
char *
__raddroffirstchar (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    return (__nullstr);
  else
    return (&t->obj->string[t->start - 1]);
}
#endif

/******************************************************************************
                                                     RADDROFFIRSTELEM        */

/* Denne rutinen brukes for } overf|re en array by reference til en
 * C-prosedyre. Rutinen returnerer med en peker til f|rste element */

#if nolib|raddroffirstelemrut
char *
__raddroffirstelem (p)
     __arrp p;
{
  return ((char *) ((long) p + sizeof (__ah) + 
		    sizeof (__arrlimit) * p->h.dim));
}
#endif

/******************************************************************************
                                                     RCOPYTEXTARRTOC         */

/* Denne rutinen brukes for } overf|re en text-array by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc.
 * Rutinen bruker RCOPYTEXTTOC for } kopiere selve tekstene */

#if nolib|rcopytextarrtocrut
char **
__rcopytextarrtoc (p, byvalue)
     __arrp p;
     char byvalue;
{
  long sizeofhode,
    size,
    i;
  __txtvp *p1x;
  char **p2x;
  sizeofhode = sizeof (__ah) + sizeof (__arrlimit) * p->h.dim;
  p1x = (__txtvp *) ((long) p + sizeofhode);
  p2x = (char **) malloc ((unsigned) (size = p->h.size - sizeofhode));
  size /= sizeof (__txtvp);
  if (byvalue)
    for (i = 0; i < size; i++)
      p2x[i] = __rcopytexttoc (p1x[i]);
  else
    for (i = 0; i < size; i++)
      p2x[i] = &p1x[i]->obj->string[p1x[i]->start - 1];
  return (p2x);

}
#endif

/******************************************************************************
                                                         RCOPYARRTOC         */

/* Denne rutinen brukes for } overf|re en array by value til en
 * C-prosedyre. Rutinen allokerer plass i C-space ved } bruke malloc. */

#if nolib|rcopyarrtocrut
char *
__rcopyarrtoc (p)
     __arrp p;
{
  long sizeofhode,
    size,
    i;
  char *p1x,
   *p2x;
  sizeofhode = sizeof (__ah) + sizeof (__arrlimit) * p->h.dim;
  p1x = (char *) ((long) p + sizeofhode);
  p2x = (char *) malloc ((unsigned) (size = p->h.size - sizeofhode));
  for (i = 0; i < size; i++)
    p2x[i] = p1x[i];
  return (p2x);
}
#endif
