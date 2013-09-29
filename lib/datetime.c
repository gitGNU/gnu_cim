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
#include "config.h"

/******************************************************************************
                                              TEXT PROCEDURE DATETIME        */

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
#if HAVE_GETTIMEOFDAY
#define DATETIMETEXTLENGTH 22L
#else
#define DATETIMETEXTLENGTH 19L
#endif

__txtvp __rdatetime (long as)
{
#if HAVE_GETTIMEOFDAY
  struct timeval tip;
#else
#if TIME_T
  time_t clock;
#else
  long clock;
#endif
#endif
  struct tm *tmp;
  (void) __rblanks (as, DATETIMETEXTLENGTH);
#if HAVE_GETTIMEOFDAY
  (void) gettimeofday (&tip, (struct timezone *) __NULL);
  tmp = localtime (&tip.tv_sec);
  (void) sprintf (__et.obj->string, "%4d-%02d-%02d %02d:%02d:%02d.%02d",
#else
#if HAVE_TIME
#if TIME_T
  clock = time ((time_t *) __NULL);
#else
  clock = time ((long) __NULL);
#endif
  tmp = localtime (&clock);
  (void) sprintf (__et.obj->string, "%4d-%02d-%02d %02d:%02d:%02d",
#endif
#endif
		  tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday,
		  tmp->tm_hour, tmp->tm_min, tmp->tm_sec
#if HAVE_GETTIMEOFDAY
		  ,tip.tv_usec / 10000
#endif
    );
  return (&__et);
}
