/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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

/* Initialization */

#include <stdio.h>
#include "const.h"
#include "name.h"
#include "filelist.h"
#include "newstr.h"
#include "cimcomp.h"
#include "error.h"
#include "lex.h"
#include "checker.h"
#include "gen.h"
#include "trans.h"
#include "passes.h"

sent_t *main_sent;

void passes_init (void)
{
  extspec_init ();
  mbuilder_init();
  salloc_init ();
  decl_init ();
  parser_init ();
  lex_init ();
  ebuilder_init ();
}

int passes_do (void)
{
  passes_init ();
  if (lex_init_pass1 (sourcename)) return (TRUE);
  if (option_write_tokens)
    {
      scan_and_write_tokens ();
      return (FALSE);
    }

  if ((ccode = fopen (ccodename, "w")) == NULL)
    {
      perror (newstrcat3 (progname, ": ", ccodename));
      return (TRUE);
    }
  if (!option_quiet)
    fprintf (stderr, "Compiling %s:\n", sourcename);

  /* PASS 1 */

  parser_init_pass1 ();
  decl_init_pass1 ();
  yyparse ();

  /* PASS 2 AV KOMPILATOREN */

  if (anterror == 0)
    {
      mbuilder_init_pass2 ();
      ebuilder_init_pass2 ();
      sbuilder_init();
      main_sent= sbuild();
      mbuilder_reinit();
      decl_init_pass2 ();
      exp_checker_init_pass2 ();
      sent_gen (main_sent, 0);

      if (separat_comp && (anterror == 0 || option_atr))
	write_all_ext ();
#ifdef DEBUG
      if (option_declarations)
	dump ();
#endif
    }
  fclose (ccode);

  if (anterror != 0)
    return (TRUE);
  else
    return (FALSE);
}

/* Reinitialization */

void passes_reinit (void)
{
  mbuilder_reinit();
  decl_reinit ();
  salloc_reinit ();
  parser_reinit ();
  lex_reinit ();
  extspec_reinit ();
}
