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
#include "file.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

/******************************************************************************
                                                    RUTINEOPTIONS            */

static char __roptions (long antarg, char arg1[])
{
  long i,
    size = 0;
  if (antarg < 2)
    return (__TRUE);
  if (!__dynsize)
    return (__TRUE);
  if (arg1[0] != '-')
    return (__TRUE);
  if (arg1[1] != 'm' && arg1[1] != 'k' && arg1[1] != 'M' && arg1[1] != 'K')
    return (__TRUE);
  for (i = 2; arg1[i] >= '0' && arg1[i] <= '9'; i++)
    size = size * 10 + arg1[i] - '0';
  if (arg1[1] == 'm' || arg1[1] == 'M')
    size *= 1024;
  if (arg1[i] == '\0')
    {
      __maxsize = __poolsize = size;
      if (arg1[1] == 'k' || arg1[1] == 'K')
	(void) fprintf (stderr, "Poolsize is changed to %ldK\n", __poolsize);
      else
	(void) fprintf (stderr, "Poolsize is changed to %ldM\n", 
			__poolsize / 1024);
    }
  return (__TRUE);
}

/******************************************************************************
                                                         RutineAllocDynMem   */

static void __rallocdynmem (long size)
{
  __min = (__dhp) calloc ((unsigned) (size), (unsigned) 1);
  if (!__min)
    __rerror ("Allocdynmem: Virtual memory exhausted");
  __fri = __min;
  __max = (__dhp) ((char *) __min + size);
}


/******************************************************************************
                                                               RUTINESTART   */

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#if HAVE_SIGFPE
RETSIGTYPE __rfloat_trap (void)
{
  __rerror ("Arithmetic overflow");
}
#endif

#if HAVE_SIGSEGV
RETSIGTYPE __rseg_trap (void)
{
  __rerror ("System error: Segmentation violation");
}
#endif

#if HAVE_SIGILL
RETSIGTYPE __rillegal_trap (void)
{
  __rerror ("System error: Illegal instruction");
}

#endif

#if HAVE_SIGTRAP
RETSIGTYPE __rtrace_trap (void)
{
  __rerror ("System error: Trace trap");
}
#endif

#if HAVE_SIGSYS
RETSIGTYPE __rsys_trap (void)
{
  __rerror ("System error: Bad argument to system call");
}
#endif

#if HAVE_SIGBUS
RETSIGTYPE __rbus_trap (void)
{
  __rerror ("System error: Bus error");
}
#endif
void __rstart (int argc, char *argv[])
{
#if CLOCK
  clock ();
#endif
  __sysin.s.s.file = stdin;
  __sysout.s.s.s.file = stdout;
  __syserr.s.s.s.file = stderr;

  __argc = (long) argc;
  __argv = (long) argv;
  __progname = argv[0];

  __roptions (argc, argv[1]);

  __init ();

/* SYSIN  :- new infile("...");   
 * SYSOUT :- new printfile("...");
 * SYSIN.open(blanks(INPUT_LINE_LENGTH));
 * SYSOUT.open(blanks(OUTPUT_LINE_LENGTH));
 * inspect SYSIN do
 * inspect SYSOUT do */

  if (__poolsize)
    __rallocdynmem (__poolsize * (long) 1024);
  else
    __rallocdynmem (DYNMEMSIZEKB * (long) 1024);

  __lb = __pb = (__dhp) & __blokk0FILE;
  /* Fyller IMAGE til stdin og stdout med blanke */
  __rtextvalassign (&((__bs2FILE *) __rsysin ())->IMAGE, (__txtvp) & __tk0);
  __rtextvalassign (&((__bs2FILE *) __rsysout ())->IMAGE, (__txtvp) & __tk0);
  __rtextvalassign (&((__bs2FILE *) __rsyserr ())->IMAGE, (__txtvp) & __tk0);

  /* Kobler c1,c2 i blokk0 til objektene av infile og printfile * henholdsvis 
   * 
   * *  * *  * *  * *  * * sysin og sysout. */
  __blokk0FILE.c1 = __rsysin ();
  __blokk0FILE.c2 = __rsysout ();

#if HAVE_SIGFPE
  signal (SIGFPE, __rfloat_trap);
#endif
#if HAVE_SIGSEGV
  signal (SIGSEGV, __rseg_trap);
#endif
#if HAVE_SIGILL
  signal (SIGILL, __rillegal_trap);
#endif
#if HAVE_SIGTRAP
  signal (SIGTRAP, __rtrace_trap);
#endif
#if HAVE_SIGSYS
  signal (SIGSYS, __rsys_trap);
#endif
#if HAVE_SIGBUS
  signal (SIGBUS, __rbus_trap);
#endif
}

/******************************************************************************
                                                               RUTINESLUTT   */
void __rslutt (void)
{
  if (__gc && ((__bs1FILE *) __blokk0FILE.c2)->open)
    {
      __rpoutimage ((__bs6FILE *) __blokk0FILE.c2);

      printf ("%d garbage collection(s) in %.1f seconds.", __gc, __gbctime);
      if (__chpoolsize)
	{
	  printf (" Poolsize changed to ");
	  if (__poolsize >= 1024)
	    printf ("%ldM while running.", __poolsize / 1024);
	  else
	    printf ("%ldK while running.", __poolsize);
	}
      printf ("\n");

    }
  __riclose ((__bs1FILE *) __rsysin ());
  __roclose ((__bs1FILE *) __rsysout ());
  __roclose ((__bs1FILE *) __rsyserr ());
}
