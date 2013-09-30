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

#define INCLUDE_SIMFILE_H
#include "cim.h"
#include "align.h"
#include "config.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

/******************************************************************************
                                                       DO_FOR_STACK_POINTERS */

/* Denne rutinen g}r igjennom alle stakk-pekere og gj|r utf|rer rutinen
 * doit for hver data peker.
 * Denne rutinen kalles fra pass 1 og pass 3 i GBC.
 * Dessuten kalles den ogs} fra add_to_pointers i det tilfellet 
 * at poolen blir flyttet */

static do_for_stack_pointers (void (*doit) ())
{
  int i,
    ar,
    at;
  ar = __as >> 8 & 0xff;
  at = __as & 0xff;
  /* Behandler f|rst ref og text stakken */

  for (i = ar; i; i--)
    (*doit) (&__r[i]);

  for (i = at; i; i--)
    (*doit) ((__dhp *) & __t[i].obj);
}

/******************************************************************************
                                                         DO_FOR_EACH_POINTER */

/* Denne rutinen g}r igjennom alle pekere for et dataobjekt
 * og utf|rer rutinen doit(_notest) for hver data peker.
 * Denne rutinen kalles fra pass 1 og pass 3 i GBC.
 * Dessuten kalles den ogs} fra add_to_pointers i det tilfellet 
 * at poolen blir flyttet */

void __do_for_each_pointer (__dhp p, void (*doit) (), void (*doit_notest) ())
{
  __dhp *ph,
   *qh;
  static __pty ppx;
  long i,
    j;

  switch ((long int) p->pp)
    {
    case __TEXT:
      break;
    case __ARRAY:
      ph = (__dhp *) ((char *) p + sizeof (__ah) +
		      (((__ahp) p)->dim * sizeof (__arrlimit)));
      qh = (__dhp *) ((char *) p + ((__ahp) p)->size);
      if (((__ahp) p)->type == __TREF)
	{
	  for (; (char *) ph < (char *) qh; ph++)
	    (*doit) ((__dhp *) ph);
	}
      else if (((__ahp) p)->type == __TTEXT)
	{
	  __txtvp th;
	  for (th = (__txtvp) ph; (char *) th < (char *) qh; th++)
	    (*doit) ((__dhp *) th);
	}
      break;
    case __ACTS:
      (*doit) (&p->dl);
      /* peker stakken */
      for (i = p->dt; i < (p->dt + p->pm + ((__shp) p)->at); i++)
	(*doit) (&((__stkp) p)->s[i].r);
      break;
    case __THUNK:
      /* Dynamisk link */
      (*doit_notest) (&p->dl);
      /* Statisk link */
      (*doit_notest) (&p->sl);
    case 0:
      break;
    default:
      /* Dynamisk link */
      (*doit_notest) (&p->dl);
      /* Statisk link */
      (*doit_notest) (&p->sl);

      /* Pekertabellen */
      j = 0;
      for (ppx = p->pp; j <=
#if __CHAR_UNSIGNED__
	   (signed char)
#endif
	   p->pp->plev; ppx = p->pp->pref[j++])
	for (i = 0; i < ppx->naref; i++)
	  {
	    (*doit) ((__dhp *) ((char *) p + ppx->ref[i]));
	  }
    }
}

/******************************************************************************
                                                                    GET_SIZE */

/* Denne rutinen finner st|rrelsen p} et data objekt inklusive alignment */

static long get_size (__dhp p)
{
  long size;
  switch ((long int) p->pp)
    {
    case __TEXT:
      size = ((__textref) p)->h.size + sizeof (__th) + 1;
      break;
    case __ARRAY:
      size = ((__ahp) p)->size;
      break;
    case __ACTS:
      size = ((__shp) p)->size;
      break;
    case __THUNK:
      size = sizeof (__thunk);
      break;
    default:
      size = p->pp->size;
    };
  return (align (size));
}

static __dhp p;

/******************************************************************************
                                                              DO_ADD_TO_LIST */

/* Brukes som parameter til do_for_stack_pointers og do_for_each_pointer
 * for } f} lagt inn objekter som ikke allerede ligger i lista.
 * Den gies som parameter til de to nevnte rutinene fra GBC pass 1.
 * Den benytter seg av } kjede alle objekter sammen i en list ved
 * hjelp av GB-ordet. Objekter blir kjedet inn rett bak p.
 * P peker en hver tid p} det objektet som er under prosessering. */

static void do_add_to_list (__dhp *qp)
{
  __dhp q;
  if ((q = *qp) != __NULL)
    if (q->gl == __NULL)
      {
	q->gl = p;
	p = q;
      };
}

/******************************************************************************
                                                           DO_UPDATE_POINTER */

/* Brukes som parameter til do_for_stack_pointers og do_for_each_pointer
 * for } f} oppdatert samtlige pekere til et objekt.
 * Den gies som parameter til de to nevnte rutinene fra GBC pass 3.
 * Den benytter seg av at adressen (etter flytting av objektet) 
 * til et objekt ligger i objektets GB-ord. 
 * Denne informasjonen er lagt i GB-ordet av GBC pass 2 */

static void do_update_pointer (__dhp *qp)
{
  if (*qp != __NULL)
    *qp = (*qp)->gl;
}

/******************************************************************************
                                    Rutine garbage collector   (fire pas)    */

/* GBC algoritme som best}r av 4 pass.
 * 1. pass traverserer og merker alle aksesserbare pekere.
 * 2. pass beregner ny posisjon til data-objektene.
 * 3. pass oppdaterer pekere
 * 4. pass skyver de aksesserbare objektene sammen */

void __rgbc (void)
{
  static __dhp hppp;
  static __dhp hpp;
  static __dhp hp;
  static __dhp q;
  static long i,
    j;
  static long size;
  static __pty ppx;
  register __dhp *ph,
   *qh;				/* Brukes for } flytte objektene */

  __gbctime -= __rcputime ();
  __gc++;

  /* PAS 1 */
  p = &__nil;

  do_add_to_list (&__lb);
  do_add_to_list (&__pb);
  do_add_to_list (&__sl);
  do_add_to_list (&__er);
  do_add_to_list ((__dhp *) &__t1.obj);
  do_add_to_list ((__dhp *) &__t2.obj);

  do_for_stack_pointers (do_add_to_list);

  __do_for_each_stat_pointer (do_add_to_list, do_add_to_list, __FALSE);

  __do_for_each_pointer ((__dhp) &__sysin, do_add_to_list, do_add_to_list);
  __do_for_each_pointer ((__dhp) &__sysout, do_add_to_list, do_add_to_list);
  __do_for_each_pointer ((__dhp) &__syserr, do_add_to_list, do_add_to_list);

  while (p != &__nil)
    {
      q = p;
      p = p->gl;

      __do_for_each_pointer (q, do_add_to_list, do_add_to_list);

    };
  /* PAS 2 */
  p = q = __min;
  while (p < __fri)
    {

      size = get_size (p);

      if (p->gl != __NULL)
	{
	  p->gl = q;
	  q = (__dhp) ((char *) q + size);
	}
      p = (__dhp) ((char *) p + size);
    };

  __update_gl_to_obj ();

  /* PAS 3 */

  do_for_stack_pointers (do_update_pointer);

  __do_for_each_stat_pointer (do_update_pointer, do_update_pointer, __FALSE);
  __do_for_each_pointer ((__dhp) &__sysin, do_update_pointer, do_update_pointer);
  __do_for_each_pointer ((__dhp) &__sysout, do_update_pointer, do_update_pointer);
  __do_for_each_pointer ((__dhp) &__syserr, do_update_pointer, do_update_pointer);

  p = __min;

  while (p < __fri)
    {

      if (p->gl != __NULL)
	__do_for_each_pointer (p, do_update_pointer, do_update_pointer);
      p = (__dhp) ((char *) p + get_size (p));

    };

  do_update_pointer (&__lb);
  do_update_pointer (&__pb);
  do_update_pointer (&__sl);
  do_update_pointer (&__er);
  do_update_pointer ((__dhp *) &__t1.obj);
  do_update_pointer ((__dhp *) &__t2.obj);

  /* PAS 4 */
  p = q = __min;
  while (p < __fri)
    {
      size = get_size (p);

      if (p->gl != __NULL)
	{
	  p->gl = __NULL;
	  ph = (__dhp *) p;
	  qh = (__dhp *) q;
	  q = (__dhp) ((char *) q + size);

	  memmove ( (char *) qh,(char *) ph, size);
	}

      p = (__dhp) ((char *) p + size);

    };
  __fri = q;

  __update_gl_to_null ();


  /* Nuller resten av omr}det */
  memset ((char *) __fri, 0, (char *) p - (char *) __fri);
  __gbctime += __rcputime ();
}

/******************************************************************************
                                                         (DO)ADD_TO_POINTER  */

/* Disse rutinene s|rger for at pekere blir oppdatert etter at pool'en er
 * flyttet. Do_add_to_pointer brukes som parameter til do_for_stack_pointers
 * og do_for_each_pointer, slik at pekerene blir oppdatert riktig.
 * Legg merke til at det er kun de pekere 
 * som peker innenfor poolen som skal oppdateres.
 * Denne oppdateringen gj|res ved } traversere samtlige objekter p}
 * samme m}te som i GBC pass 3. */

static long disp;
static char *new_fri,
 *new_min;

static void do_add_to_pointer (__dhp *qp)
{
  if (*qp >= __min & *qp < __fri)
    *qp = (__dhp) ((char *) (*qp) + disp);
}

static void add_to_pointers (void)
{
  char *p;

  do_for_stack_pointers (do_add_to_pointer);

  __do_for_each_stat_pointer (do_add_to_pointer, do_add_to_pointer, __TRUE);
  __do_for_each_pointer ((__dhp) &__sysin, do_add_to_pointer, do_add_to_pointer);
  __do_for_each_pointer ((__dhp) &__sysout, do_add_to_pointer, do_add_to_pointer);
  __do_for_each_pointer ((__dhp) &__syserr, do_add_to_pointer, do_add_to_pointer);

  p = new_min;

  while (p < new_fri)
    {
      __do_for_each_pointer ((__dhp) p, do_add_to_pointer, do_add_to_pointer);
      p = (char *) p + get_size ((__dhp) p);
    };

  do_add_to_pointer (&__sl);
  do_add_to_pointer (&__lb);
  do_add_to_pointer (&__pb);
}

/******************************************************************************
                                                            RutineAlloc      */

/* Denne rutinen er basis rutinen for allokering av data fra Simula.
 * Hvis det ikke er ledig plass i poolen s} blir GBC kalt.
 * Hvis det fremdeles ikke er nokk plass eller at under halve plassen
 * er frigjort fors|ker rutinen } utvide poolen ved } kalle realloc.
 * Blir poolen flyttet s} blir add_to_pointers kalt for } oppdatere pekere,
 * og nytt omr}de blir fylt med nuller. */

__dhp __ralloc (long size)
{
  void __rgbc ();
  static __dhp mem;
  if (__sto != __NULL)
    {
      __dhp dp;
      dp = __sto;
      __sto = __NULL;
      if (dp->pp != __NULL)
	{
	  memset ((char *) dp, 0, size);
	}
      return (dp);
    }
  size = align (size);
  if (((char *) __fri + size) > (char *) __max)
    {
      __rgbc ();
#if REALLOC
      if (((char *) __fri - (char *) __min > (char *) __max - (char *) __fri)
	  || (char *) __fri + size > (char *) __max)
	{
	  long new_size,
	   *ph;

	  new_size = ((char *) __max - (char *) __min) * 2;
	  if (new_size - ((char *) __fri - (char *) __min) < size)
	    new_size += size;

	  if (new_size > __maxsize * 1024)
	    new_size = __maxsize * 1024;
	  if (new_size == ((char *) __max - (char *) __min))
	    new_min = __NULL;
	  else
	    new_min = (char *) realloc (__min, new_size);

	  if (new_min != __NULL)
	    {
	      disp = new_min - (char *) __min;
              if (new_min != (char *) __min)
                {
	          new_fri = (char *) __fri + disp;
	          new_min = (char *) __min + disp;

		  add_to_pointers ();

		  __fri = (__dhp) new_fri;
		  __min = (__dhp) new_min;
		}

	      /* Nuller resten av omr}det */
	      ph = (long *) ((char *) __max + disp);
	      __max = (__dhp) (new_min + new_size);
	      memset ((char *) ph, 0, (char *) __max - (char *) ph);
	      __chpoolsize = __TRUE;
	      __poolsize = new_size / 1024;
	    }
	}
#endif
      if (((char *) __fri + size) > (char *) __max)
	__rerror ("Alloc: Virtual memory exhausted");
    }
  mem = __fri;
  __fri = (__dhp) (((char *) __fri) + size);
  return (mem);
}
