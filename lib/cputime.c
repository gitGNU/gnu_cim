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
#include "config.h"

/******************************************************************************
                                              LONG REAL PROCEDURE CPUTIME    */

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if HAVE_SYS_RESOURCE_H && HAVE_GETRESOURCE
#include<sys/resource.h>
#else
#if HAVE_SYS_TIMES_H
#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <sys/times.h>
#endif
#endif

double 
__rcputime ()
{
#if HAVE_SYS_RESOURCE_H && HAVE_GETRESOURCE
  struct rusage rus;
#else
#if HAVE_SYS_TIMES_H && HAVE_TIMES
  struct tms buffer;
#endif
#endif
  static double old_cputime;
  double cputime;
#if HAVE_GETRESOURCE
  (void) getrusage (RUSAGE_SELF, &rus);
  cputime = ((double) rus.ru_utime.tv_sec) + ((double) rus.ru_stime.tv_sec) +
    (((double) rus.ru_utime.tv_usec) + ((double) rus.ru_stime.tv_usec)) /
    1000000.0;
#else
#if HAVE_SYS_TIMES_H && HAVE_TIMES
  times (&buffer);
  cputime = ((double) buffer.tms_utime + (double) buffer.tms_stime) /
#ifdef CLK_TCK
    (double) CLK_TCK;
#else
    60.0;
#endif
#else
  cputime = (double) clock () /
#if CLOCK_IN_USEC
    1000000.0;
#else
#ifdef CLOCKS_PER_SEC
    (double) CLOCKS_PER_SEC;
#else
    (double) CLK_TCK;
#endif
#endif
#endif
#endif
  if (cputime <= old_cputime)
    cputime = __raddepsilon (old_cputime);
  return (old_cputime = cputime);
}
