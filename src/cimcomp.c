/* $Id: cim.c,v 1.18 1997/01/26 14:30:21 cim Exp $ */

/* Copyright (C) 1987-1997 Sverre Hvammen Johansen,
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
#include "const.h"
#include "navn.h"
#include "filelist.h"
#include "newstr.h"
#include "cimcomp.h"
#include "feil.h"
#include "lex.h"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if STDC_HEADERS
#include <stdlib.h>
#endif

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "getopt.h"

FILE *scode;
FILE *mcode;
FILE *ccode;
FILE *hcode;

char *extcodename;
char *mifcodename;
char *hcodename;

static char *outputname = "";
static char *exekname;
static char *mellname;
static char *sourcename;
static char *listname;
static char *ccodename;
static char *ocodename;
static char *shlname;

static char *ccomp;

static char *ccompargs = "";
static char *linkargs = "";
static char *systemlibdir = LIBDIR;
static char *tmpdir;
char *includedir = INCLUDEDIR;

int option_write_tokens;
int option_nowarning;
int option_atr;
int option_line;
int option_gen_trace;
int option_reuse_timestamp;
int option_verbose;
int option_init_poolsize;
int option_dyn_poolsize;
int option_max_poolsize;
int option_bl_in_dir_line;

static int option_static;
static int option_pic;
static int option_checkdiff;
static int option_nolink;
static int option_nosim;
static int option_nocc;
static int option_checkdif;
static int option_notempdel;
static int option_noexecdel;
static int option_quiet;
static int option_sscheck;
#ifdef DEBUG
static int option_lex,
static int option_gen_test;
static int option_declarations;
static int option_expressions;
static int option_msymbols;
static int option_input;
#endif

#ifdef DEBUG
static int s_out;
#endif
char separat_comp;		/* Sier om kj|ringen er en separat
				 * kompilering eller ikke */

static char *progname;

/******************************************************************************
                                                                     XMALLOC */

char *
xmalloc (size)
     unsigned int size;
{
  char *ptr = malloc (size);
  if (! ptr)
    {
      fprintf (stderr, "%s: virtual memory exhausted\n", progname);
      exit (1);
    }
  return ptr;
}

/******************************************************************************
                                                               TRAP-ROUTINES */
#if HAVE_SIGFPE
static RETSIGTYPE
float_trap ()
{
  lerror (25);
}
#endif

#if HAVE_SIGSEGV
static RETSIGTYPE
seg_trap ()
{
  lerror (26);
}
#endif

#if HAVE_SIGILL
static RETSIGTYPE
illegal_trap ()
{
  lerror (28);
}
#endif

#if HAVE_SIGTRAP
static RETSIGTYPE
trace_trap ()
{
  lerror (29);
}
#endif

#if HAVE_SIGBUS
static RETSIGTYPE
bus_trap ()
{
  lerror (27);
}
#endif

#if HAVE_SIGSYS
static RETSIGTYPE
sys_trap ()
{
  lerror (30);
}
#endif

static init_trap_routines()
{
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
}

/******************************************************************************
                                                                 GET_ALL_ENV */

#if HAVE_GETENV
extern char *getenv ();
static xgetenv (name, var)
     char *name;
     char **var;
{
  char *value;
  value = getenv (name);
  if (value != NULL)
    *var = newstrcat1 (value);
}
#endif

static get_all_env()
{
#if HAVE_GETENV
  xgetenv ("CIMLIBDIR", &systemlibdir);
  xgetenv ("CIMINCLUDEDIR", &includedir);
  xgetenv ("TMPDIR", &tmpdir);
  xgetenv ("CIMTMPDIR", &tmpdir);
#endif
}

/******************************************************************************
                                                                     SIMCOMP */
static simcomp ()
{
  if ((scode = fopen (sourcename, "r")) == NULL)
    {
      perror (newstrcat3 (progname, ": ", sourcename));
      return (TRUE);
    }
  setfilmap (tag (sourcename, strlen (sourcename)), 1L);
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
      if (freopen (listname, "w", stdout) == NULL)
	{
	  perror (newstrcat3 (progname, ": ", listname));
	  return (TRUE);
	}
    }
#endif
#if OPEN_FILE_IN_BINARY_MODE
  if ((mcode = fopen (mellname, "wb")) == NULL)
#else
  if ((mcode = fopen (mellname, "w")) == NULL)
#endif
    {
      perror (newstrcat3 (progname, ": ", mellname));
      return (TRUE);
    }
  if ((ccode = fopen (ccodename, "w")) == NULL)
    {
      perror (newstrcat3 (progname, ": ", ccodename));
      return (TRUE);
    }
  if ((hcode = fopen (hcodename, "w")) == NULL)
    {
      perror (newstrcat3 (progname, ": ", hcodename));
      return (TRUE);
    }
  if (!option_quiet)
    (void) fprintf (stderr, "Compiling %s:\n", sourcename);

  /* PASS 1 AV KOMPILATOREN */
  init_parser ();
  init_structure ();
  initdekl ();
  (void) yyparse ();
  (void) fclose (mcode);
  (void) fclose (scode);
  /* PASS 2 AV KOMPILATOREN */

  if (anterror == 0)
    {
#if OPEN_FILE_IN_BINARY_MODE
      if ((mcode = fopen (mellname, "rb")) == NULL)
#else
      if ((mcode = fopen (mellname, "r")) == NULL)
#endif
	{
	  perror (newstrcat3 (progname, ": ", mellname));
	  return (TRUE);
	}
      sjekker ();
#ifdef DEBUG
      if (option_declarations)
	dump ();
#endif
      (void) fclose (mcode);
    }
  (void) fclose (ccode);
  (void) fclose (hcode);
#ifdef DEBUG
  if (option_declarations || option_expressions
      || option_msymbols || option_input || option_lex)
    {
      (void) fclose (stdout);
      fdopen (s_out, "w");
      (void) dup2 (s_out, fileno (stdout));
    }
#endif
  unlink (mellname);
  if (anterror != 0)
    return (TRUE);
  else
    return (FALSE);
}

/******************************************************************************
                                                               STRINGTOUPPER */

static char *
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
                                                                    BASENAME */
static char *
basename (str) char *str;
{
  int i,j;
  for (i = strlen (str) - 2; i >= 0; i--)
    if (!strncmp (&str[i], "/", 1))
      {
	str= &str[i + 1];
	break;
      }
  str= newstrcat1 (str);
  i= strlen (str);
  if (i > 4 && !(strcmp (&str[i - 4], ".sim")
		 && strcmp (&str[i - 4], ".cim")))
    str[i - 4] = '\0';
  
  return str;
}

/******************************************************************************
                                                                  PRINT_HELP */

static int print_help(status)int status;
{
  fprintf(stderr,"Usage: %s"
	  " [-a] [--atr]"
	  " [-b flags] [--cflags=flags]"
	  "\n      "
	  " [-B flags] [--ldflags=flags]"
          " [-c] [--suppress-linking]"
	  "\n      "
	  " [-d] [--compare]"
	  " [-D NAME] [--define=NAME]"
	  " [-e] [--static]" 
	  "\n      "
	  " [-E] [--preprocess]" 
	  " [-g] [--debug]"
	  " [-G] [--gcc]"
	  "\n      "
	  " [-I DIR] [--includedir=DIR]"
	  " [-h] [--help]"
	  " [-H] [--no-lines]"
	  "\n      "
	  " [-l LIBRARY] [--library=LIBRARY]"
	  " [-L DIR] [--library-dir=DIR]"
	  "\n      "
	  " [-m [N]] [--memory-pool-size[=N]]"
	  " [-M N] [--max-memory-pool-size=N]"
	  "\n      "
	  " [-N FILE] [--input=FILE]"
	  " [-o FILE] [--output=FILE]"
	  " [-O] [-ON]"
	  "\n      "
	  " [--optimize] [--optimize=N]"
	  " [-p] [--pic]"
	  " [-P] [--only-link]"
	  "\n      "
	  " [-q] [--quiet]"
	  " [-R] [--preserve-timestamp]"
	  " [--silent]"
	  "\n      "
	  " [-s] [--no-simula-compile]"
	  " [-S] [--only-simula-compile]"
	  "\n      "
	  " [-t] [--dont-remove-temporaries]"
	  " [-U NAME] [--undefine=NAME]"
	  "\n      "
	  " [-v] [--verbose]"
	  " [-V] [--version]"
	  " [-w] [--no-warn]"
	  "\n      "
	  " [-X] [--trace]"
	  " simula-file [file...]\n", progname);
  exit(status);
}

/******************************************************************************
                                                                PARSEOPTIONS */
static parseoptions (argc, argv)
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
	  {"static", 0, 0, 'e'},
	  {"preprocess", 0, 0, 'E'},
	  {"debug", 0, 0, 'g'},
	  {"gcc", 0, 0, 'G'},
	  {"include-dir", 1, 0, 'I'},
	  {"help", 0, 0, 'h'},
	  {"no-lines", 0, 0, 'H'},
	  {"library", 1, 0, 'l'},
	  {"library-dir", 2, 0, 'L'},
	  {"memory-pool-size", 2, 0, 'm'},
	  {"max-memory-pool-size", 1, 0, 'M'},
	  {"input", 1, 0, 'N'},
	  {"output", 1, 0, 'o'},
	  {"optimize", 2, 0, 'O'},
	  {"pic", 0, 0, 'p'},
	  {"only-link", 0, 0, 'P'},
	  {"quiet", 0, 0, 'q'},
	  {"silent", 0, 0, 'q'},
	  {"preserve-timestamp", 0, 0, 'R'},
	  {"no-simula-compile", 0, 0, 's'},
	  {"only-simula-compile", 0, 0, 'S'},
	  {"dont-remove-temporaries", 0, 0, 't'},
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
			    "ab:B:cC:edD:EgGI:hHl:L::m::M:o:O::pPqRsStTU:vVwX",
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
	  ccompargs = newstrcat3 (ccompargs, optarg, " ");
	  break;
	case 'B':
	  linkargs = newstrcat3 (linkargs, optarg, " ");
	  break;
	case 'c':
	  if (option_nolink || option_nocc)
	    {
	      fprintf (stderr, "%s: Option -c: Specified twise or "
		       "in combination with -S or -C\n", progname);
	      print_help (1);
	    }
	  option_nolink = TRUE;
	  break;
	case 'C':
	  ccomp = newstrcat1 (optarg);
	  break;
	case 'd':
	  option_checkdiff = TRUE;
	  option_reuse_timestamp = TRUE;
	  break;
	case 'D':
	  systag (stringtoupper (optarg), 
		  strlen (optarg))->definition = TRUE;
	  break;
	case 'e':
	  option_static = TRUE;
	  break;
	case 'E':
	  option_write_tokens = TRUE;
	  break;
	case 'g':
	  break;
	case 'G':
	  ccomp = "gcc -g -O2";
	  break;
	case 'h':
	  print_help(0);
	  break;
	case 'H':
	  option_line = TRUE;
	  break;
	case 'I':
	  includedir = newstrcat1 (optarg);
	  break;
	case 'l':
	  new_lib (optarg);
	  break;
	case 'L':
	  insert_name_in_dirlist (optarg);
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
	  if (sourcename != NULL)
	    {
	      fprintf (stderr, "%s: More than one simula-file "
		       "are specified\n", progname);
	      print_help (1);
	    };
	  sourcename = newstrcat1 (optarg);
	  break;
	case 'o':
	  outputname = newstrcat1 (optarg);
	  break;
	case 'O':
	  if (!optarg) optarg="";
	  ccompargs = newstrcat4 (ccompargs, "-O", optarg, " ");
	  break;
	case 'p':
	  option_pic = TRUE;
	  break;
	case 'P':
	  if (option_nolink || option_nocc || option_nosim)
	    {
	      fprintf (stderr, "%s: Option -C: Specified twise or "
		       "in combination with -s, -c or -S\n", progname);
	      print_help (1);
	    }
	  option_nocc = option_nosim = TRUE;
	  break;
	case 'q':
	  option_quiet = TRUE;
	  option_verbose = FALSE;
	  break;
	case 'R':
	  option_reuse_timestamp = TRUE;
	  break;
	case 's':
	  if (option_nocc || option_nosim)
	    {
	      fprintf (stderr, "%s: Option -s: Specified twise or in "
		       "combination with -S or -C\n", progname);
	      print_help (1);
	    }
	  option_nosim = TRUE;
	  break;
	case 'S':
	  if (option_nolink || option_nocc || option_nosim)
	    {
	      fprintf (stderr, "%s: Option -S: Specified twise or "
		       "in combination with -s, -c or -C\n", progname);
	      print_help (1);
	    }
	  option_nolink = option_nocc = TRUE;
	  break;
	case 't':
	  option_notempdel = TRUE;
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
          option_quiet = FALSE;
	  break;
	case 'V':
	  printf ("%s-%s\n", PACKAGE_VERSION, SYSTEM_TYPE);
	  exit (0);
	  break;
	case 'w':
	  option_nowarning = TRUE;
	  break;
	case 'X':
	  option_gen_trace = TRUE;
	  break;
	case '?':
	  return (TRUE);
	  break;
	}
    }
  for (index = optind; index < argc; index++)
    {
      int l = strlen (argv[index]);
      if (l >= 6 && !strcmp (&argv[index][l - 6], "-atr.a"))
	{
	  insert_name_in_archlist (argv[index]);
	  insert_name_in_linklist (transform_name (argv[index], "-atr.a", ".a"), FALSE);
	} else
      if (l >= 2 && !strcmp (&argv[index][l - 2], ".a"))
	{
	  insert_name_in_archlist (transform_name (argv[index], ".a", "-atr.a"));
	  insert_name_in_linklist (argv[index], FALSE);
	}
      else if (l >= 2 && !strcmp (&argv[index][l - 2], ".o"))
	{
	  insert_name_in_linklist (argv[index], FALSE);
	}
      else if (l >= 2 && argv[index][l - 2] == '.')
	{
	  fprintf (stderr, "%s: Illegal extension\n", progname);
	  print_help (1);
	}
      else
	{
	  if (sourcename != NULL)
	    {
	      fprintf (stderr, "%s: More than one simula-files "
		       "are specified\n", progname);
	      print_help (1);
	    };
	  if (l > 4 && !(strcmp (&argv[index][l - 4], ".sim")
			 && strcmp (&argv[index][l - 4], ".cim")))
	    sourcename = newstrcat1 (argv[index]);
	  else
	    sourcename = newstrcat2 (argv[index], ".sim");
	}
    }

  if (sourcename == NULL)
    {
      fprintf (stderr, "%s: No simula-file is specified\n", progname);
      print_help (1);
    };

  exekname= basename (sourcename);
  listname= newstrcat2 (exekname, ".L");
  mellname= newstrcat2 (exekname, ".m");
  extcodename= newstrcat2 (exekname, ".atr");
  mifcodename= newstrcat2 (exekname, ".mif");
  hcodename= newstrcat2 (exekname, ".h");
  ccodename= newstrcat2 (exekname, ".c");
  ocodename= newstrcat2 (exekname, ".o");
  shlname= newstrcat3 ("./", exekname, ".shl");

  if (!strcmp (outputname, ""))
    outputname = exekname;
  else
    exekname = outputname;

  if (strncmp (exekname, "/", 1))
    exekname = newstrcat2 ("./", exekname);

  return (FALSE);
}

/******************************************************************************
                                                                        MAIN */

main (argc, argv, envp)
     int argc;
     char *argv[];
     char *envp[];
{
  char *archname;
  char *kom;
  char differ=TRUE;

  progname = argv[0];

  ccomp = newstrcat3 (SCC, " ", SCFLAGS);

  init_trap_routines();

  get_all_env();
  
  insert_name_in_dirlist (systemlibdir);

  init_define ();

  if (parseoptions (argc, argv))
    return (1);

  new_lib ("cim");

  if (option_verbose)
    {
      (void) fprintf 
	(stderr, 
	 "Cim Compiler (version: %s configuration name: %s).\n", PACKAGE_VERSION, SYSTEM_TYPE);
      (void) fprintf 
	(stderr,
	 "Copyright 1989-1997 by Sverre Hvammen Johansen, Stein Krogdahl and Terje Mjøs.\n");
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
    system (newstrcat6 ("cp -f ", extcodename, " ", extcodename, 
			 ".old", " 2>/dev/null"));

  if(option_checkdiff)
    {
      rename (ccodename, newstrcat2 (ccodename, ".old")); 
      rename (hcodename, newstrcat2 (hcodename, ".old")); 
    }

  if (!option_nosim && simcomp ())
    {
      unlink (ccodename);
      unlink (hcodename);

      if(option_checkdiff)
	{
	  rename (ccodename, newstrcat2 (ccodename, ".old")); 
	  rename (hcodename, newstrcat2 (hcodename, ".old")); 
	}
      return (1);
    }

  if (option_write_tokens)
    return (0);

  if(option_atr)
    {
      char status;
      unlink (ccodename);
      unlink (hcodename);

      status = system (newstrcat5 ("cmp -s ", extcodename, " ", 
				  extcodename, ".old 2>/dev/null"));
      unlink (newstrcat2 (extcodename, ".old"));
      if (status)
	{
	  if (option_verbose)
	    (void) fprintf (stderr, "Atr file differ\n");
	  return (1);
	} else return (0);
    }

  if (!option_nosim)
    {
      FILE *shlfile;

      if ((shlfile = fopen (shlname, "w")) == NULL)
	{
	  perror (newstrcat3 (progname, ": ", shlname));
	  return (1);
	}

      fprintf (shlfile, 
	       "#! /bin/sh\n"
	       "\n"
	       "CC='%s'\n"
	       "CFLAGS='%s'\n"
	       "CFLAGS_ADD=\n"
	       "LDFLAGS='%s'\n"
	       "LDFLAGS_ADD=\n"
	       "wl='%s'\n"
	       "link_static_flag=\"%s\"\n"
	       "pic_flag=\"%s\"\n"
	       "\n"
	       "prev=\n"
	       "for option\n"
	       "do\n"
	       "  if test -n \"$prev\"; then\n"
	       "    eval \"$prev=\\$option\"\n"
	       "    prev=\n"
	       "    if test -n \"$CFLAGS_ADD\"; then\n"
	       "      CFLAGS=\"$CFLAGS $option\"\n"
	       "      CFLAGS_ADD=\n"
	       "    fi\n"
	       "    if test -n \"$LDFLAGS_ADD\"; then\n"
	       "      LDFLAGS=\"$LDFLAGS $option\"\n"
	       "      LDFLAGS_ADD=\n"
	       "    fi\n"
	       "    continue\n"
	       "  fi\n"
	       "\n"
	       "  case \"$option\" in\n"
	       "  -*=*) optarg=`echo \"$option\" | "
	       "sed 's/[-_a-zA-Z0-9]*=//'` ;;\n"
	       "  *) optarg= ;;\n"
	       "  esac\n"
	       "\n"
	       "  case \"$option\" in\n"
	       "  -c | --suppress-linking | -suppress-linking)\n"
	       "    option_nolink=yes ;;\n"
	       "  -e | --static | -static)\n"
	       "  LDFLAGS=\"$LDFLAGS $link_static_flag\" ;;\n"
	       "  -G | --gcc | -gcc)\n"
	       "    CC='gcc -g -O2'; static_flag='-static'; \n"
	       "    pic_flag=' -fPIC' ;;\n"
	       "  -q | --quiet | --silent | -quiet | -silent)\n"
	       "    option_quiet=yes ; option_verbose= ;;\n"
	       "  -v | --verbose | -verbose) "
	       "    option_quiet= ; option_verbose=yes ;;\n"
	       "  -p | --pic | -pic)\n"
	       "  CFLAGS=\"$CFLAGS $pic_flag\" ;;\n"
	       "  -P | --only-link | -only-link)\n"
	       "    option_nocc=yes ;;\n"
	       "  -t | --dont-remove-temporaries | -dont-remove-temporaries)\n"
	       "    option_notempdel=yes ;;\n"
	       "  -s | --no-simula-compile | -no-simula-compile)\n"
	       "    option_notempdel=yes ;;\n"
	       "  -S | --only-simula-compile | -only-simula-compile)\n"
	       "    option_nolink=yes; option_nocc=yes ;;\n"
	       "  -d | --compare | -compare) "
	       "    option_checkdiff=yes; option_notempdel=yes ;;\n"
	       "\n"
	       "  -b | --cflags | -cflags)\n"
	       "    prev=CFLAGS_ADD ;;\n"
	       "  -B | --ldflags | -ldflags)\n"
	       "    prev=LDFLAGS_ADD ;;\n"
	       "  -C | --c-compiler | -c-compiler)\n"
	       "    prev=CC ;;\n"
	       "  -b=* | --cflags=* | -cflags=*)\n"
	       "    CFLAGS=\"$CFLAGS $optarg\" ;;\n"
	       "  -B=* | --ldflags=* | -ldflags=*)\n"
	       "    LDFLAGS=\"$LDFLAGS $optarg\" ;;\n"
	       "  -C=* | --c-compiler=* | -c-compiler=*)\n"
	       "    CC=\"$optarg\" ;;\n"
	       /*	       "  *) { echo \"n.shl: $option: invalid option\" 1>&2; "
	       "exit 1 };;\n"*/
	       "  esac\n"
	       "done\n"
	       "\n"
	       "differ=yes\n"
	       "if test -z \"$option_nocc\"; then\n"
	       "  if test -n \"$option_checkdiff\"; then\n"
	       "    cmp -s %s %s.old &&\n"
	       "      cmp -s %s %s.old &&\n"
	       "      differ=\n"
	       "    rm -f %s.old %s.old\n"
	       "  fi\n"
	       "\n"
	       "  if test -n \"$differ\"; then\n"
	       "    if test -z \"$option_quiet\"; then\n"
	       "      echo $CC $CFLAGS -c %s\n"
	       "    fi\n"
	       "    $CC $CFLAGS -c %s || exit 1\n"
	       "  else\n"
	       "    if test -z\"$option_quiet\"; then\n"
	       "      echo touch %s\n"
	       "    fi\n"
	       "    touch %s\n"
	       "  fi\n"
	       "  if test -z \"$option_notempdel\"; then\n"
	       "    rm -f %s %s\n"
	       "  fi\n"
	       "fi\n"
	       "\n",
	       ccomp, "", SLDFLAGS,
	       WL_FLAG, LINK_STATIC_FLAG, PIC_FLAG,
	       ccodename, ccodename, hcodename, hcodename, 
	       ccodename, hcodename,
	       ccodename, ccodename,
	       ocodename, ocodename,
	       ccodename, hcodename);

      if (!separat_comp)
	{
	  more_modules ();
	  fprintf (shlfile,
		   "if test -z \"$option_nolink\"; then\n"
		   "  if test -z \"$option_quiet\"; then\n"
		   "    echo $CC $LDFLAGS -o %s %s %s\n"
		   "  fi\n"
		   "\n"
		   "  $CC $LDFLAGS -o %s %s %s %s || exit 1\n"
		   "fi\n", 
		   outputname, ocodename, get_names_in_linklist (),
		   outputname, ocodename, get_names_in_linklist (), SLIBS);
	}
      fclose (shlfile);
      system (newstrcat2 ("chmod +x ", shlname));
    }
  argv[0]= shlname;
  execve (shlname, argv, envp);
}
