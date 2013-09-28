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
                                              LONG REAL PROCEDURE CLOCKTIME  */

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

#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
#include<sys/resource.h>
#endif

double __rclocktime (void)
{
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
  struct timeval tip;
#else
#if TIME_T
  time_t clock;
#else
  long clock;
#endif
#endif
  struct tm *tmp;
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
  (void) gettimeofday (&tip, (struct timezone *) __NULL);
  tmp = localtime (&tip.tv_sec);
#else
#if TIME_T
  clock = time ((time_t *) __NULL);
#else
  clock = time ((long) __NULL);
#endif
  tmp = localtime (&clock);
#endif
  return (((double) (tmp->tm_hour * 60 + tmp->tm_min) * 60 + tmp->tm_sec)
#if HAVE_SYS_RESOURCE_H && HAVE_GETTIMEOFDAY
	  + ((double) tip.tv_usec) / 1000000.0
#endif
    );
}
