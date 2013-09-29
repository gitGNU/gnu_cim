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
                                              TEXT PROCEDURE SIMULAID        */

extern char *__progname;
#define MAX_HOSTNAME_LEN 100
#define MAX_SITEID_LEN   100
#define MAX_INT_LEN       12
#define MAX_PROGNAME_LEN 100
#define MAX_SIMULAID_LEN (100+MAX_SITEID_LEN+MAX_HOSTNAME_LEN\
			  +3*MAX_INT_LEN+MAX_PROGNAME_LEN)

#include <stdio.h>
#if HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

__txtvp __rsimulaid (long as)
{
  char simulaid[MAX_SIMULAID_LEN];
  char siteid[MAX_SITEID_LEN];
  char cpu[MAX_HOSTNAME_LEN];
  char user[MAX_INT_LEN];
  char job[MAX_INT_LEN];
  char acc[MAX_INT_LEN];
#if HAVE_UNAME
  static struct utsname name;
#endif
#if HAVE_GETDOMAINNAME
  (void) getdomainname (siteid, MAX_SITEID_LEN);
#endif
#if HAVE_UNAME
  (void) uname (&name);
  strcpy (cpu, name.nodename);
#else
#if HAVE_GETHOSTNAME
  (void) gethostname (cpu, MAX_HOSTNAME_LEN);
#endif
#endif
#if HAVE_GETUID
  (void) sprintf (user, "%d", getuid ());
#endif
#if HAVE_GETPID
  (void) sprintf (job, "%d", getpid ());
#endif
#if HAVE_GETEGID
  (void) sprintf (acc, "%d", getegid ());
#endif
  (void) sprintf (simulaid, "%s!!!%s!!!%s!!!%s!!!%s!!!%s!!!%s!!!%s"
		  ,PACKAGE_VERSION
#if HAVE_GETDOMAINNAME
		  ,siteid
#else
		  ,"???"
#endif
		  ,SYSTEM_TYPE
#if HAVE_UNAME || HAVE_GETHOSTNAME
		  ,cpu
#else
		  ,"???"
#endif
#if HAVE_GETUID
		  ,user
#else
		  ,"???"
#endif
#if HAVE_GETPID
		  ,job
#else
		  ,"???"
#endif
#if HAVE_GETEGID
		  ,acc
#else
		  ,"???"
#endif
		  ,__progname);
  (void) __rblanks (as, (long) strlen (simulaid));
  (void) sprintf (__et.obj->string, "%s", simulaid);
  return (&__et);
}
