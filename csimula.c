/* $Id: csimula.c,v 1.18 1997/01/26 14:30:21 cim Exp $ */

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

/* Main for kompilatoren. */

#include <stdio.h>
#include "cextern.h"
#include "cnavn.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "getopt.h"
extern char *nextchar;

#define MIPS_MAXANTCASE 500

int maxantcase;
int antsimlib;
char *simlib[MAXANTSIMLIB];
int maxdirlen;
int antlibdir;
char *libdir[MAXANTLIBDIR];


FILE *PROG;
FILE *MELL;
FILE *CKOD;
FILE *HKOD;

char *basenavn;
char *exeknavn;
char *extkodnavn;
char *mellnavn;
char *prognavn;
char *listnavn;
char *ckodnavn;
char *okodnavn;
char *hkodnavn;

char *ccomp = SCC;
char *outputfile = "";

char *link_moduler = "";
char kom[1000];
char *ccompargs = "";
char *linkargs = "";
char *link_specmoduler = "";
char *link_systemmodul = "";
char *systemlibdir = LIBDIR;
char *tmpdir;
char *includedir = INCLUDEDIR;

int option_nolink,
  option_atr,
  option_nosim,
  option_nocc,
  option_checkdiff,
  option_notempdel,
  option_noexecdel,
  option_run,
  option_nowarning,
  option_line,
  option_write_tokens,
  option_quiet,
  option_verbose,
  option_sscheck,
  option_dyn_poolsize,
  option_bl_in_dir_line,
  option_reuse_timestamp,
  option_gen_trace;
int option_init_poolsize,
  option_max_poolsize;
#ifdef DEBUG
char option_lex,
  option_gen_test,
  option_declarations,
  option_expressions,
  option_msymbols,
  option_input;
#endif
#ifdef DEBUG
int s_out;
#endif
char separat_comp;		/* Sier om kj|ringen er en separat
				 * kompilering eller ikke */

char *stringtoupper ();

/******************************************************************************
                                                               TRAP-ROUTINES */
#if HAVE_SIGFPE
RETSIGTYPE
float_trap ()
{
  lerror (25);
}
#endif

#if HAVE_SIGSEGV
RETSIGTYPE
seg_trap ()
{
  lerror (26);
}
#endif

#if HAVE_SIGILL
RETSIGTYPE
illegal_trap ()
{
  lerror (28);
}
#endif

#if HAVE_SIGTRAP
RETSIGTYPE
trace_trap ()
{
  lerror (29);
}
#endif

#if HAVE_SIGBUS
RETSIGTYPE
bus_trap ()
{
  lerror (27);
}
#endif

#if HAVE_SIGSYS
RETSIGTYPE
sys_trap ()
{
  lerror (30);
}
#endif

#if NOT_GETENV
#else
extern char *getenv ();
testandsetenv (name, var)
     char *name;
     char **var;
{
  char *value;
  value = getenv (name);
  if (value != NULL)
    {
      *var = (char *) salloc (strlen (value) + 1);
      (void) strcpy (*var, value);
    }
}
#endif

#define ERROR(t)   (void)fprintf(stderr,"%s: ",argv[0]);\
		   (void)fprintf(stderr,t);(void)fprintf(stderr,"\n");\
                   return(TRUE)

/******************************************************************************
                                                                   NEWLIBDIR */
newlibdir (argv, dir) char *argv[]; char *dir;
{
  char *newlink_specmoduler;

  if (dir == NULL)
    {
      if (antlibdir) antlibdir--;
      return;
    }

  if (maxdirlen < strlen (dir)) maxdirlen = strlen (dir);

  if (antlibdir == MAXANTLIBDIR)
    {
      ERROR ("to many archive files");
    };
  
  libdir[antlibdir] = (char *) salloc (strlen (dir) + 1);
  (void) strcpy (libdir[antlibdir], dir);
  antlibdir++;

  newlink_specmoduler =
    (char *) salloc (4 + strlen (link_specmoduler) + strlen (dir));
  (void) strcpy (newlink_specmoduler, link_specmoduler);
  (void) strcat (newlink_specmoduler, "-L");
  (void) strcat (newlink_specmoduler, dir);
  (void) strcat (newlink_specmoduler, " ");
  link_specmoduler = newlink_specmoduler;
}

/******************************************************************************
                                                                   NEWSIMLIB */
newsimlib (argv, lib) char *argv[]; char *lib;
{
  char *newlink_specmoduler;
  int i;
  char *name;
  FILE *f;
  name = (char *) salloc (strlen(lib) + maxdirlen + 9);
  for (i = 0; i < antlibdir; i++)
    {
      strcpy (name, libdir[i]);
      strcat (name, DIR_MARKER);
      strcat (name, "lib");
      strcat (name, lib);
      strcat (name, ".a");
#if SPLIT_ARCH_LIB
      strcat (name, SPLIT_SUFFIX);
#endif      
#if OPEN_FILE_IN_BINARY_MODE
      if (f=fopen (name, "rb"))
#else
      if (f=fopen (name, "r"))
#endif
 	{
	  fclose (f);

	  if (antsimlib == MAXANTSIMLIB)
	    {
	      ERROR ("to many archive directories");
	    };
	  simlib[antsimlib] = name;
	  antsimlib++;

	  newlink_specmoduler =
	    (char *) salloc (8 + strlen (link_specmoduler) + strlen (lib)
			     + strlen (libdir[i]));
	  strcpy (newlink_specmoduler, link_specmoduler);
	  strcat (newlink_specmoduler, libdir[i]);
	  strcat (newlink_specmoduler, DIR_MARKER);
	  strcat (newlink_specmoduler, "lib");
	  strcat (newlink_specmoduler, lib);
	  strcat (newlink_specmoduler, ".a ");
	  link_specmoduler = newlink_specmoduler;
	  return;
	} 
    }

  newlink_specmoduler =
    (char *) salloc (4 + strlen (link_specmoduler) + strlen (lib));
  (void) strcpy (newlink_specmoduler, link_specmoduler);
  (void) strcat (newlink_specmoduler, "-l");
  (void) strcat (newlink_specmoduler, lib);
  (void) strcat (newlink_specmoduler, " ");
  link_specmoduler = newlink_specmoduler;
}

/******************************************************************************
                                                                        MAIN */

main (argc, argv)
     int argc;
     char *argv[];
{
  char *archname;
#if HAVE_SIGFPE
  signal (SIGFPE, float_trap);
#endif
#if HAVE_SIGSEGV
  signal (SIGSEGV, seg_trap);
#endif
#if HAVE_SIGILL
  signal (SIGILL, illegal_trap);
#endif
#if HAVE_SIGTRAP
  signal (SIGTRAP, trace_trap);
#endif
#if HAVE_SIGSYS
  signal (SIGSYS, sys_trap);
#endif
#if HAVE_SIGBUS
  signal (SIGBUS, bus_trap);
#endif
#if NOT_GETENV
#else
  testandsetenv ("CIMLIBDIR", &systemlibdir);
  testandsetenv ("CIMINCLUDEDIR", &includedir);
  testandsetenv ("TMPDIR", &tmpdir);
  testandsetenv ("CIMTMPDIR", &tmpdir);
#endif
  init_define ();

  newlibdir (argv, systemlibdir);

  if (argc == 1)
    {
      if (input_command (argv))
	return (SH_FALSE);
    }
  else if (parseoptions (argc, argv))
    return (SH_FALSE);

  newsimlib (argv, "cim");

  if (option_verbose)
    {
      (void) fprintf 
	(stderr, 
	 "Cim Compiler (version: %s system type: %s).\n", SIMID, SYSTEM_TYPE);
      (void) fprintf 
	(stderr,
	 "Copyright 1994 by Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs.\n");
      (void) fprintf (stderr,
      "Department of Informatics, University of Oslo.\n");
      (void) fprintf (stderr, "Cim comes with ABSOLUTELY NO WARRANTY.\n");
      (void) fprintf 
	(stderr,
	 "This is free software, and you are welcome to redistribute it\n");
      (void) fprintf 
	(stderr, 
	 "under the GNU General Public License; version 2.\n");
    }

  if(option_atr)
    {
      sprintf (kom, "%s %s.atr %s.atr.old%s", CP_COMMAND, basenavn, basenavn
	       , CP_SUFFIX);
      system (kom);
    }

  if(option_checkdiff)
    {
      sprintf (kom, "%s %s %s.old%s", MV_COMMAND, ckodnavn, ckodnavn
	       , MV_SUFFIX);
      system (kom);
      sprintf (kom, "%s %s %s.old%s", MV_COMMAND, hkodnavn, hkodnavn
	       , MV_SUFFIX);
      system (kom);
    }

  if (!option_nosim && simcomp ())
    {
      sprintf (kom, "%s %s%s", RM_COMMAND, ckodnavn, RM_SUFFIX);
      system (kom);
      sprintf (kom, "%s %s%s", RM_COMMAND, hkodnavn, RM_SUFFIX);
      system (kom);
      if(option_checkdiff)
	{
	  sprintf (kom, "%s %s.old %s%s", MV_COMMAND, ckodnavn, ckodnavn
		   , MV_SUFFIX);
	  system (kom);
	  sprintf (kom, "%s %s.old %s%s", MV_COMMAND, hkodnavn, hkodnavn
		   , MV_SUFFIX);
	  system (kom);
	}

      return (SH_FALSE);
    }

  if (option_write_tokens)
    return (SH_TRUE);

  if(option_atr)
    {
      char status;
      sprintf (kom, "%s %s%s", RM_COMMAND, ckodnavn, RM_SUFFIX);
      system (kom);
      sprintf (kom, "%s %s%s", RM_COMMAND, hkodnavn, RM_SUFFIX);
      system (kom);
      if(option_checkdiff)
	{
	  sprintf (kom, "%s %s.old %s%s", MV_COMMAND, ckodnavn, ckodnavn
		   , MV_SUFFIX);
	  system (kom);
	  sprintf (kom, "%s %s.old %s%s", MV_COMMAND, hkodnavn, hkodnavn
		   , MV_SUFFIX);
	  system (kom);
	}
      sprintf (kom, "%s %s.atr %s.atr.old", CMP_COMMAND, basenavn, basenavn
	       , CMP_SUFFIX);
      status = system (kom);
      sprintf (kom, "%s %s.atr.old%s", RM_COMMAND, basenavn, RM_SUFFIX);
      system (kom);
      if (status)
	{
	  if (!option_quiet)
	    (void) fprintf (stderr, 
			    "Atr file differ\n");
	  return (SH_FALSE);
	} else return (SH_TRUE);
    }

  if (!option_nocc)
    {
      char differ=TRUE;
      if(option_checkdiff)
        {
	  sprintf (kom, "%s %s %s.old", CMP_COMMAND, ckodnavn, ckodnavn
		   , CMP_SUFFIX);
	  if (!system(kom))
	    {
	      sprintf (kom, "%s %s %s.old", CMP_COMMAND, hkodnavn, hkodnavn
		       , CMP_SUFFIX);
	      if (!system(kom)) differ = FALSE;
	    } 
	  sprintf (kom, "%s %s.old%s", RM_COMMAND, ckodnavn, RM_SUFFIX);
	  system (kom);
	  sprintf (kom, "%s %s.old%s", RM_COMMAND, hkodnavn, RM_SUFFIX);
	  system (kom);
	}
      if (differ)
	{
#ifdef ANT_CASE_WITHOUT_EXTRA_OPTION_TO_C_COMPILER
	  if (maxantcase > ANT_CASE_WITHOUT_EXTRA_OPTION_TO_C_COMPILER &&
	      !strcmp (ccomp, SCC))
	    (void) sprintf (kom, "%s %s%d %s %s%s", ccomp
			    , CASE_OPTION, maxantcase
			    ,SCFLAGS, ccompargs, ckodnavn);
	  else
#endif
	    (void) sprintf (kom, "%s %s %s%s", ccomp
			    ,SCFLAGS, ccompargs, ckodnavn);
	  if (!option_quiet)
	    (void) fprintf (stderr, "%s:\n", kom);
#if EVEN_SH_FALSE
	  if ((system (kom) & 1) == 0)
#else
	    if (system (kom))
#endif
	      {
		(void) fprintf (stderr
				, "Error occured while cc-compilation.\n");
		sprintf (kom, "%s %s%s", RM_COMMAND, okodnavn, RM_SUFFIX);
		system (kom);
		if (!(option_notempdel || option_checkdiff))
		  {
		    sprintf (kom, "%s %s%s", RM_COMMAND, ckodnavn, RM_SUFFIX);
		    system (kom);
		    sprintf (kom, "%s %s%s", RM_COMMAND, hkodnavn, RM_SUFFIX);
		    system (kom);
		  }
		return (SH_FALSE);
	      }
	} else
	  {
	    sprintf(kom,"%s %s", TOUCH_COMMAND, okodnavn);
	    if (!option_quiet)
	      (void) fprintf (stderr, "%s:\n", kom);
	    system (kom);
	  }
      if (!option_nosim && !(option_notempdel || option_checkdiff))
	{
	  sprintf (kom, "%s %s%s", RM_COMMAND, ckodnavn, RM_SUFFIX);
	  system (kom);
	  sprintf (kom, "%s %s%s", RM_COMMAND, hkodnavn, RM_SUFFIX);
	  system (kom);
	} 
    } else
      {
	sprintf (kom, "%s %s%s", RM_COMMAND, okodnavn, RM_SUFFIX);
	system (kom);
      }
  if (!option_nolink && !separat_comp)
    {
      more_modules ();
      (void) sprintf (kom, "%s %s %s %s%s%s%s%s%s %s"
#ifdef C_LINKER
		      ,C_LINKER
#else
		      ,ccomp
#endif
		      ,SLDFLAGS, outputfile, linkargs, okodnavn
		      ,LINK_FILE_SEP_MARKER, link_moduler, link_specmoduler
		      ,link_systemmodul, SLIBS);
      if (!option_quiet)
	(void) fprintf (stderr, "%s:\n", kom);
#if EVEN_SH_FALSE
      if ((system (kom) & 1) == 0)
#else
      if (system (kom))
#endif
	{
	  (void) fprintf (stderr, "Error occured while linking.\n");
	  return (SH_FALSE);;
	}
#ifdef KOM_AFTER_LINK
      sprintf (kom, KOM_AFTER_LINK);
      system (kom);
#endif
      if (!option_nocc && ! (option_notempdel || option_checkdiff))
	{
	  sprintf (kom, "%s %s%s", RM_COMMAND, okodnavn, RM_SUFFIX);
	  system (kom);
	}
      if (option_run)
	{
	  if (!option_quiet)
	    (void) fprintf (stderr, "Executing %s:\n", outputfile);
	  (void) system (exeknavn);
	  if (!(option_notempdel || option_noexecdel))
	    {
	      sprintf (kom, "%s %s%s", RM_COMMAND, outputfile, RM_SUFFIX);
	      system (kom);
	    }
	}
    }
  return (SH_TRUE);
}

char *
stringtoupper (s)
     char *s;
{
  char *st;
  for (st = s; *st != '\0'; st++)
    if (*st >= 'a' && *st <= 'z')
      *st = *st - (char) 32;
  return (s);
}

/******************************************************************************
                                                        SIMCOMP              */
simcomp ()
{
  if ((PROG = fopen (prognavn, "r")) == NULL)
    {
      perror (prognavn);
      return (TRUE);
    }
  setfilmap (tag (prognavn, strlen (prognavn)), 1L);
  if (option_write_tokens)
    {
      scan_and_write_tokens ();
      return (FALSE);
    }
#ifdef DEBUG
  if (option_declarations || option_expressions
      || option_msymbols || option_input || option_lex)
    {
      s_out = dup (stdout->_file);
      if (freopen (listnavn, "w", stdout) == NULL)
	{
	  perror (listnavn);
	  return (TRUE);
	}
    }
#endif
#if OPEN_FILE_IN_BINARY_MODE
  if ((MELL = fopen (mellnavn, "wb")) == NULL)
#else
  if ((MELL = fopen (mellnavn, "w")) == NULL)
#endif
    {
      perror (mellnavn);
      return (TRUE);
    }
  if ((CKOD = fopen (ckodnavn, "w")) == NULL)
    {
      perror (ckodnavn);
      return (TRUE);
    }
  if ((HKOD = fopen (hkodnavn, "w")) == NULL)
    {
      perror (hkodnavn);
      return (TRUE);
    }
  if (!option_quiet)
    (void) fprintf (stderr, "Compiling %s:\n", prognavn);

  /* PASS 1 AV KOMPILATOREN */
  init_parser ();
  init_structure ();
  initdekl ();
  (void) yyparse ();
  (void) fclose (MELL);
  (void) fclose (PROG);
  /* PASS 2 AV KOMPILATOREN */

  if (anterror == 0)
    {
#if OPEN_FILE_IN_BINARY_MODE
      if ((MELL = fopen (mellnavn, "rb")) == NULL)
#else
      if ((MELL = fopen (mellnavn, "r")) == NULL)
#endif
	{
	  perror (mellnavn);
	  return (TRUE);
	}
      sjekker ();
#ifdef DEBUG
      if (option_declarations)
	dump ();
#endif
      (void) fclose (MELL);
    }
  (void) fclose (CKOD);
  (void) fclose (HKOD);
#ifdef DEBUG
  if (option_declarations || option_expressions
      || option_msymbols || option_input || option_lex)
    {
      (void) fclose (stdout);
      fdopen (s_out, "w");
      (void) dup2 (s_out, fileno (stdout));
    }
#endif
  sprintf (kom, "%s %s%s", RM_COMMAND, mellnavn, RM_SUFFIX);
  system (kom);
  if (anterror != 0)
    return (TRUE);
  else
    return (FALSE);
}

/******************************************************************************
                                                               INPUT_COMMAND */

#define ARG_BUFFER_LENGTH 100
#define ARG_ARRAY_LENGTH 20

input_command (argv)
     char *argv[];
{
  char arg_buffer[ARG_BUFFER_LENGTH];
  char *arg_array[ARG_ARRAY_LENGTH];
  int array_ind = 1,
    buffer_ind = 0;
  int c,
    xc = ' ',
    argc = 1;
  fprintf (stderr, "Compile >> ");
  arg_array[0] = argv[0];
  arg_array[array_ind++] = &arg_buffer[buffer_ind];
  for (c = getchar (); c != '\n' && c != EOF; c = getchar ())
    {
      if (c == ' ' && xc == ' ');
      else if (c == ' ')
	{
	  arg_buffer[buffer_ind++] = '\0';
	  if (buffer_ind == ARG_BUFFER_LENGTH - 2)
	    {
	      ERROR ("command line have too many arguments");
	    }
	  arg_array[array_ind++] = &arg_buffer[buffer_ind];
	  while (c == ' ')
	    c = getchar ();
	  ungetc (c, stdin);
	  argc++;
	}
      else
	{
	  if (buffer_ind == ARG_BUFFER_LENGTH - 2)
	    {
	      ERROR ("command line is to long");
	    }
	  arg_buffer[buffer_ind++] = c;
	}
      xc = c;
    }
  arg_buffer[buffer_ind++] = '\0';
  if (xc == ' ')
    array_ind--;
  else
    argc++;

  arg_array[array_ind++] = NULL;

  return (parseoptions (argc, arg_array));
}

/******************************************************************************
                                                        PARSEOPTIONS         */
parseoptions (argc, argv)
     int argc;
     char *argv[];
{
  int c;
  int index;
  while (1)
    {
      int option_index = 0;
      static struct option long_options[] =
	{
	  {"atr", 0, 0, 'a'},
	  {"cflags=", 1, 0, 'b'},
	  {"ldflags", 1, 0, 'B'},
	  {"suppress-linking", 0, 0, 'c'},
	  {"c-compiler", 1, 0, 'C'},
	  {"compare", 0, 0, 'd'},
	  {"define", 1, 0, 'D'},
	  {"preprocess", 0, 0, 'E'},
	  {"debug", 0, 0, 'g'},
	  {"gcc", 0, 0, 'G'},
	  {"include-dir", 1, 0, 'I'},
	  {"no-lines", 0, 0, 'h'},
	  {"library", 1, 0, 'l'},
	  {"library-dir", 2, 0, 'L'},
	  {"memory-pool-size", 2, 0, 'm'},
	  {"max-memory-pool-size", 1, 0, 'M'},
	  {"input", 1, 0, 'N'},
	  {"output", 1, 0, 'o'},
	  {"optimize", 2, 0, 'O'},
	  {"preserve-timestamp", 0, 0, 'p'},
	  {"only-link", 0, 0, 'P'},
	  {"quiet", 0, 0, 'q'},
	  {"silent", 0, 0, 'q'},
	  {"run", 0, 0, 'r'},
	  {"no-simula-compile", 0, 0, 's'},
	  {"only-simula-compile", 0, 0, 'S'},
	  {"dont-remove-temporaries", 0, 0, 't'},
	  {"dont-remove-executable", 0, 0, 'T'},
	  {"undefine", 1, 0, 'U'},
	  {"verbose", 0, 0, 'v'},
	  {"version", 0, 0, 'V'},
	  {"no-warn", 0, 0, 'w'},
	  {"trace", 0, 0, 'X'},
#ifdef DEBUG
	  {"dd", 0, &option_declarations, 1},
	  {"de", 0, &option_expressions, 1},
	  {"dm", 0, &option_msymbols, 1},
	  {"di", 0, &option_input, 1},
	  {"dl", 0, &option_lex, 1},
	  {"dg", 0, &option_gen, 1},
#endif
	  {"oc", 1, 0, 'b'},
	  {"ol", 1, 0, 'B'},
	  {0, 0, 0, 0}
	};

      c = getopt_long_only (argc, argv, 
			    "ab:B:cC:dD:EgGI:hl:L::m::M:o:O::pP:qrsStTU:vVwX",
			    long_options, &option_index);

      if (c == EOF) break;

      switch (c)
	{
     	case 0:
     	  /* If this option set a flag, do nothing else now.   */
     	  break;
     	case 'a':
	  option_atr = TRUE;
	  option_checkdiff = TRUE;
	  option_reuse_timestamp = TRUE;
	  break;
	case 'b':
	  {
	    char *newccompargs;
	    newccompargs = (char *) salloc (2 + strlen (ccompargs) 
					    + strlen (optarg));
	    (void) strcpy (newccompargs, ccompargs);
	    (void) strcat (newccompargs, optarg);
	    (void) strcat (newccompargs, " ");
	    ccompargs = newccompargs;
	  }
	  break;
	case 'B':
	  {
	    char *newlinkargs;
	    newlinkargs = (char *) salloc (2 + strlen (linkargs) 
					 + strlen (optarg));
	    (void) strcpy (newlinkargs, linkargs);
	    (void) strcat (newlinkargs, optarg);
	    (void) strcat (newlinkargs, " ");
	    linkargs = newlinkargs;
	  }
	  break;
	case 'c':
	  if (option_nolink || option_nocc)
	    {
	      ERROR ("option -c: Specified twise or in combination with -S or -C");
	    }
	  option_nolink = TRUE;
	  break;
	case 'C':
	  ccomp = (char *) salloc (strlen (optarg) + 1);
	  (void) strcpy (ccomp, optarg);
	  break;
	case 'd':
	  option_checkdiff = TRUE;
	  option_reuse_timestamp = TRUE;
	  break;
	case 'D':
	  systag (stringtoupper (optarg), 
		  strlen (optarg))->definition = TRUE;
	  break;
	case 'E':
	  option_write_tokens = TRUE;
	  break;
	case 'g':
#ifdef DBX_OPTION
	  {
	    char *newccompargs;
	    char *newlinkargs;
	    newccompargs = (char *) salloc (2 + strlen (ccompargs) 
					  + strlen (DBX_OPTION));
	    (void) strcpy (newccompargs, ccompargs);
	    (void) strcat (newccompargs, DBX_OPTION);
	    (void) strcat (newccompargs, " ");
	    ccompargs = newccompargs;
	    newlinkargs = (char *) salloc (2 + strlen (linkargs) 
					 + strlen (DBX_OPTION));
	    (void) strcpy (newlinkargs, linkargs);
	    (void) strcat (newlinkargs, DBX_OPTION);
	    (void) strcat (newlinkargs, " ");
	    linkargs = newlinkargs;
	  }
#endif
	  break;
	case 'G':
#ifdef GCC_COMMAND
	  ccomp = (char *) salloc (strlen (GCC_COMMAND) + 1);
	  (void) strcpy (ccomp, GCC_COMMAND);
#endif
	  break;
	case 'h':
	  option_line = TRUE;
	  break;
	case 'I':
	  includedir = (char *) salloc (strlen (optarg) + 1);
	  (void) strcpy (includedir, optarg);
	  break;
	case 'l':
	  newsimlib (argv, optarg);
	  break;
	case 'L':
	  newlibdir (argv,optarg);
	  break;
	case 'm':
	  if (optarg)
	    {
	      sscanf (optarg, "%d", &option_init_poolsize);
	      option_init_poolsize *= 1024;
	    } else
	      {
		option_dyn_poolsize = TRUE;
	      }
	  break;
	case 'M':
	  sscanf (optarg, "%d", &option_max_poolsize);
	  option_max_poolsize *= 1024;
	  break;
	case 'N':
	  if (prognavn != NULL)
	    {
	      ERROR ("more than one simula-files are specified");
	    };
	  prognavn = (char *) salloc (strlen(optarg) + 1);
	  (void) strcpy (prognavn, optarg);
	  break;
	case 'o':
	  outputfile = (char *) salloc (strlen (optarg) + 1);
	  (void) strcpy (outputfile, optarg);
	  break;
	case 'O':
	  {
	    char *newccompargs;
	    if (!optarg) optarg="";
	    newccompargs = (char *) salloc (4 + strlen (ccompargs) 
					    + strlen (optarg));
	    (void) strcpy (newccompargs, ccompargs);
	    (void) strcat (newccompargs, "-O");
	    (void) strcat (newccompargs, optarg);
	    (void) strcat (newccompargs, " ");
	    ccompargs = newccompargs;
	  }
	  break;
	case 'p':
	  option_reuse_timestamp = TRUE;
	  break;
	case 'P':
	  if (option_nolink || option_nocc || option_nosim)
	    {
	      ERROR ("option -C: Specified twise or in combination with -s, -c or -S");
	    }
	  option_nocc = option_nosim = TRUE;
	  break;
	case 'q':
	  option_quiet = TRUE;
	  if (option_verbose)
	    {
	      ERROR ("option -q: Specified in combination with -v");
	    }
	  break;
	case 'r':
	  option_run = TRUE;
	  break;
	case 's':
	  if (option_nocc || option_nosim)
	    {
	      ERROR ("option -s: Specified twise or in combination with -S or -C");
	    }
	  option_nosim = TRUE;
	  break;
	case 'S':
	  if (option_nolink || option_nocc || option_nosim)
	    {
	      ERROR ("option -S: Specified twise or in combination with -s, -c or -C");
	    }
	  option_nolink = option_nocc = TRUE;
	  break;
	case 't':
	  option_notempdel = TRUE;
	  break;
	case 'T':
	  option_noexecdel = TRUE;
	  break;
	case 'u':
	  option_bl_in_dir_line = TRUE;
	  break;
	case 'U':
	  systag (stringtoupper (optarg), 
		  strlen (optarg))->definition = FALSE;
	  break;
	case 'v':
	  option_verbose = TRUE;
	  if (option_quiet)
	    {
	      ERROR ("option -v: Specified in combination with -q");
	    }
	  break;
	case 'V':
	  printf ("%s-%s\n", SIMID, SYSTEM_TYPE);
	  return(TRUE);
	  break;
	case 'w':
	  option_nowarning = TRUE;
	  break;
	case 'X':
	  option_gen_trace = TRUE;
	  break;
	case '?':
	  return(TRUE);
	  break;
	}
    }
  for (index = optind; index < argc; index++)
    {
      int l = strlen (argv[index]);
      if (l >= 2 && !strcmp (&argv[index][l - 2], ".a"))
	{
	  char *newlink_specmoduler;
	  if (antsimlib == MAXANTSIMLIB)
	    {
	      ERROR ("to many archive files");
	    };
	  simlib[antsimlib] = 
	    (char *) salloc (strlen (argv[index]) + 3);
	  (void) strcpy (simlib[antsimlib], argv[index]);
#if SPLIT_ARCH_LIB
	  (void) strcat (simlib[antsimlib], SPLIT_SUFFIX);
#endif
	  antsimlib++;
	  newlink_specmoduler =
	    (char *) salloc (2 + strlen (link_specmoduler) 
			     + strlen (argv[index]));
	  (void) strcpy (newlink_specmoduler, link_specmoduler);
	  (void) strcat (newlink_specmoduler, argv[index]);
	  (void) strcat (newlink_specmoduler, " ");
	  link_specmoduler = newlink_specmoduler;
	}
      else if (l >= strlen (OBJECT_SUFFIX) &&
	       !strcmp (&argv[index][l - strlen (OBJECT_SUFFIX)], 
			OBJECT_SUFFIX))
	{
	  char *newlink_specmoduler;
	  newlink_specmoduler =
	    (char *) salloc (2 + strlen (link_specmoduler) 
			     + strlen (argv[index]));
	  (void) strcpy (newlink_specmoduler, argv[index]);
	  (void) strcat (newlink_specmoduler, LINK_FILE_SEP_MARKER);
	  (void) strcat (newlink_specmoduler, link_specmoduler);
	  link_specmoduler = newlink_specmoduler;
	}
      else if (l >= 2 && argv[index][l - 2] == '.')
	{
	  ERROR ("illegal extension");
	}
      else
	{
	  if (prognavn != NULL)
	    {
	      ERROR ("more than one simula-files are specified");
	    };
	  prognavn = (char *) salloc (l + 5);
	  (void) strcpy (prognavn, argv[index]);
	  if (l > 4 && !(strcmp (&prognavn[l - 4], ".sim")
			 && strcmp (&prognavn[l - 4], ".cim")))
	    ;
	  else
	    (void) strcat (prognavn, ".sim");
	}
    }

  if (prognavn == NULL)
    {
      ERROR ("no simula-file is specified");
    };

  {
    int l;
    l = strlen (prognavn);

    basenavn = (char *) salloc (l + 5);
    exeknavn = (char *) salloc (l + 6);
    extkodnavn = (char *) salloc (l + 5);
    mellnavn = (char *) salloc (l + 5);
    listnavn = (char *) salloc (l + 5);
    ckodnavn = (char *) salloc (l + 5);
    okodnavn = (char *) salloc (l + 5);
    hkodnavn = (char *) salloc (l + 5);
	  
    (void) strcpy (basenavn, prognavn);
    if (l > 4 && !(strcmp (&basenavn[l - 4], ".sim")
		   && strcmp (&basenavn[l - 4], ".cim")))
      basenavn[l - 4] = '\0';

    {
      int bp;
      for (bp = strlen (basenavn) - 2; bp >= 0; bp--)
	if (!strncmp (&basenavn[bp], DIR_MARKER, 
		      strlen (DIR_MARKER)))
	  {
	    basenavn = &basenavn[bp + 1];
	    break;
	  }
    }
  }
  (void) strcpy (listnavn, basenavn);
  (void) strcat (listnavn, ".L");
  (void) strcpy (mellnavn, basenavn);
  (void) strcat (mellnavn, ".m");
  (void) strcpy (extkodnavn, basenavn);
  (void) strcat (extkodnavn, ".atr");
  (void) strcpy (hkodnavn, basenavn);
  (void) strcat (hkodnavn, ".h");
  (void) strcpy (ckodnavn, basenavn);
  (void) strcat (ckodnavn, ".c");
  (void) strcpy (okodnavn, basenavn);
  (void) strcat (okodnavn, OBJECT_SUFFIX);
  if (strlen(prognavn) > 4 
      && !(strcmp (&prognavn[strlen(prognavn) - 4], ".sim")
	   && strcmp (&prognavn[strlen(prognavn) - 4], ".cim")))
    (void) strcpy (exeknavn, basenavn);
  else
    (void) strcat (exeknavn, "a.out");

  if (!strcmp (outputfile, ""))
    {
      outputfile = (char *) salloc (strlen (exeknavn) 
				    + strlen (EXE_SUFFIX) + 1);
      (void) strcpy (outputfile, exeknavn);
      (void) strcat (outputfile, EXE_SUFFIX);
    }
  else
    {
      exeknavn = (char *) salloc (strlen (outputfile) + 1);
      (void) strcpy (exeknavn, outputfile);
    }
#ifdef RUN_PREFIX
  {
    char *s;
    s = (char *) salloc (strlen (exeknavn) + strlen (RUN_PREFIX) + 1);
    (void) strcpy (s, RUN_PREFIX);
    (void) strcat (s, exeknavn);
    exeknavn = s;
  }
#else
  if (strncmp (exeknavn, DIR_MARKER, strlen (DIR_MARKER)))
    {
      char *s;
      s = (char *) salloc (strlen (exeknavn) + 3);
      (void) strcpy (s, ".");
      (void) strcat (s, DIR_MARKER);
      (void) strcat (s, exeknavn);
      exeknavn = s;
    }
#endif
  return (FALSE);
}
