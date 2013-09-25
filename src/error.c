/* $Id: cfeil.c,v 1.19 1995/03/07 10:53:16 cim Exp $ */

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

/* Inneholder de tekstlige feilmeldingene som kompilatoren kan gi.
 * For noen tilfeller b|r det gis bedre og mer spesifike feilmeldinger.
 * Dette gjelder spesielt for feilmeldinger fra sjekkeren.
 *                                                              
 * Siden det kan inkluderes filer m} det lages et tabellverk som
 * holder greie p} hvilke linjenummere internt i kompilator-programmet
 * som h|rer til de enkelte filene.                              
 * Dette tabellverket brukes s} i forbindelse med utskrift              
 * av feilmeldinger. */

#include "const.h"
#include "dekl.h"
#include "cimcomp.h"
#include "error.h"
#include "lex.h"
#include "extspec.h"
#include "mapline.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

int anterror;

/******************************************************************************
                                                                  STARTERROR */
static void
starterror (long line)
{
  anterror++;
  if (line == 0)
    return;
  fprintf (stderr, "\"%s\", line %ld: "
		  ,getmapfile (line), getmapline (line));
}

/******************************************************************************
                               					 LERROR      */

/* Feil som oppdages av LEX */

void lerror (int errcode)
{
  if (option_nowarning && (errcode == 8 || errcode == 32))
    return;
  starterror (lineno);
  switch (errcode)
    {
    case 1:
      fprintf (stderr, "Comments are not terminated with ';'.\n");
      break;
    case 2:
      fprintf (stderr, "Illegal character constant.\n");
      break;
    case 3:
      fprintf (stderr, "Character constant is not terminated.\n");
      break;
    case 4:
      fprintf (stderr, "Text constant is not terminated.\n");
      break;
    case 5:
      fprintf (stderr, "Illegal character in text constant.\n");
      break;
    case 7:
      fprintf (stderr, "Illegal character (combination).\n");
      break;
    case 8:
      fprintf (stderr, "Warning: Unknown directive line.\n");
      anterror--;
      break;
    case 9:
      fprintf (stderr, "Too big integer constant.\n");
      break;
    case 11:
      fprintf (stderr, "Radix integer constant has bad format.\n");
      break;
    case 12:
      fprintf (stderr, "Illegal radix digit in integer constant.\n");
      break;
    case 15:
      fprintf (stderr, "Homeless %%ENDCOMMENT.\n");
      break;
    case 16:
      fprintf (stderr, "Radix integer is illegal in real constant.\n");
      break;
    case 17:
      fprintf (stderr, "Recursive inclusion of files.\n");
      break;
    case 18:
      fprintf (stderr, "File specified by %%INCLUDE can't be opened.\n");
      break;
    case 19:
      fprintf (stderr, "%%COMMENT is not terminated with %%ENDCOMMENT.\n");
      break;
    case 21:
      fprintf (stderr, "Homeless %%ELSE(NOT)DEF.\n");
      break;
    case 22:
      fprintf (stderr, "Homeless %%ELSE.\n");
      break;
    case 23:
      fprintf (stderr, "Homeless %%ENDIF.\n");
      break;
    case 24:
      fprintf (stderr, "%%IF(NOT)DEF is not terminated with %%ENDIF.\n");
      break;
#if HAVE_SIGFPE
    case 25:
      fprintf (stderr, "Arithmetic overflow.\n");
      exit (TRUE);
      break;
#endif
#if HAVE_SIGSEGV
    case 26:
      fprintf (stderr, "System error: Segmentation violation.\n");
      exit (TRUE);
      break;
#endif
#if HAVE_SIGBUS
    case 27:
      fprintf (stderr, "System error: Bus error.\n");
      exit (TRUE);
      break;
#endif
#if HAVE_SIGILL
    case 28:
      fprintf (stderr, "System error: Illegal instruction.\n");
      exit (TRUE);
      break;
#endif
#if HAVE_SIGTRAP
    case 29:
      fprintf (stderr, "System error: Trace trap.\n");
      exit (TRUE);
      break;
#endif
#if HAVE_SIGSYS
    case 30:
      fprintf (stderr, "System error: Bad argument to system call.\n");
      exit (TRUE);
      break;
#endif
    case 32:
      fprintf (stderr, "Warning: May have forgotten ';' after END.\n");
      anterror--;
      break;
    case 44:
      fprintf (stderr, "System error: Too long text constant.\n");
      exit (TRUE);
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}

/******************************************************************************
                           					YERROR       */

/* Feil som oppdages av YACC */

/*VARARGS1 */
void yerror (int errcode, char *txt)
{
  char c;
  starterror (lineno);
  switch (errcode)
    {
    case 1:
      fprintf (stderr, "Illegal kind on external procedure.\n");
      break;
    case 2:
      fprintf (stderr, "IN is illegal in external declaration.\n");
      break;
    case 7:
      fprintf (stderr, "Type REF is illegal in constant declaration.\n");
      break;
    case 8:
      fprintf (stderr, "IN is illegal in specification of formal procedure.\n");
      break;
    case 9:
      fprintf (stderr, "Relation operator other than '=' is used in constant declaration.\n");
      break;
    case 13:
      fprintf (stderr, "Syntax error.\n");
      break;
    case 14:
      fprintf (stderr, "System error: Yacc stack overflow.\n");
      exit (TRUE);
      break;
    case 16:
      fprintf (stderr, "System error: No Message specified from Yacc.\n");
      exit (TRUE);
      break;
    case 21:
      c = toupper ((int) txt[0]);
      txt++;
      fprintf (stderr, "%c%s.\n", c, txt);
      break;
    case 25:
      fprintf (stderr, "Illegal empty statement.\n");
      break;
    case 26:
      fprintf (stderr, "Declaration is found after a sentence.\n");
      break;
    case 27:
      fprintf (stderr, "Declaration is found after a label.\n");
      break;
    case 28:
      fprintf (stderr, "Declaration is not terminated with ';'.\n");
      break;
    case 29:
      fprintf (stderr, "Illegal with declaration as body.\n");
      break;
    case 45:
      fprintf (stderr, "Formal or virtual procedure shall be specified.\n");
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}

/******************************************************************************
                                				D1ERROR      */

/* Feil som oppdages av DECL PASS 1 */

/*VARARGS1 */
void d1error (int errcode, char *name)
{
  starterror (lineno);
  switch (errcode)
    {
    case 37:
      fprintf (stderr, "System error: Illegal categ in declaration.\n");
      exit (TRUE);
      break;
    case 34:
      fprintf (stderr, "Identifier %s is not a parameter.\n"
		      , name);
      break;
    case 35:
      fprintf (stderr, "Identifier %s is specified twice in mode part.\n", name);
      break;
    case 36:
      fprintf (stderr, "Identifier %s is specified twice.\n", name);
      break;
    case 41:
      fprintf (stderr, "Identifier %s is specified twice as HIDDEN/PROTECTED.\n", name);
      break;
    case 38:
      fprintf (stderr, "INNER is not on outermost block level in this Class.\n");
      break;
    case 39:
      fprintf (stderr, "More than one inner in this Class.\n");
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}

/******************************************************************************
                                    				D2ERROR      */

/* Feil som oppdages av DECL PASS 2 */

/*VARARGS1 */
void d2error (int errcode, decl_t *rd1, decl_t *rd2)
{
  char *name1 = rd1->ident;
  starterror (errcode == 79 ? lineno : rd1->line);
  switch (errcode)
    {
    case 49:
      fprintf (stderr, "%s is illegal as prefix to %s (seen through inspect).\n"
		      ,rd2->ident, name1);
      break;
    case 50:
      fprintf (stderr, "%s is illegal as prefix to %s (not a Class).\n"
		      ,rd2->ident, name1);
      break;
    case 51:
      fprintf (stderr, "%s is illegal as prefix to %s (not declared at same block level).\n"
		      ,rd2->ident, name1);
      break;
    case 52:
      fprintf (stderr, "%s is illegal as prefix to %s (circulated prefix chain).\n"
		      ,rd2->ident, name1);
      break;
    case 53:
      fprintf (stderr, "%s has unknown qualification.\n", name1);
      break;
    case 54:
      fprintf (stderr, "%s has illegal qualification.\n"
		      , name1);
      break;
    case 55:
      fprintf (stderr, "%s is declared twice.\n"
		      , name1);
      break;
    case 56:
      fprintf (stderr, "%s is illegal parameter to Class.\n"
		      , name1);
      break;
    case 57:
      fprintf (stderr, "%s can't be transmitted by value.\n"
		      , name1);
      break;
    case 58:
      fprintf (stderr, "%s is illegal as name to a Class.\n"
		      , name1);
      break;
    case 59:
      fprintf (stderr, "System error: Illegal categ in declaration.\n");
      exit (TRUE);
      break;
    case 60:
      fprintf (stderr, "%s is already specified as virtual.\n"
		      , name1);
      break;
    case 61:
      fprintf (stderr, "%s is illegal as virtual.\n"
		      , name1);
      break;
    case 62:
      fprintf (stderr, "%s is illegal match.\n"
		      , name1);
      break;
    case 63:
      fprintf (stderr, "%s is not specified.\n"
		      , name1);
      break;
    case 72:
      fprintf (stderr, "This %s is illegal since the Class is used as prefix to a block.\n"
		      ,name1);
      break;
    case 73:
      fprintf (stderr, "Illegal inspect.\n");
      break;
    case 74:
      fprintf (stderr, "%s is undeclared.\n"
		      ,name1);
      break;
    case 75:
      fprintf (stderr, "%s is not declared local.\n"
		      , name1);
      break;
    case 76:
      fprintf (stderr, "%s is not Protected.\n"
		      , name1);
      break;
    case 77:
      fprintf (stderr, "Text/ref atributte %s can't be transmitted by name to a C procedure.\n"
		      , name1);
      break;
    case 78:
      fprintf (stderr, "External C procedure %s can't have type ref.\n", name1);
      break;
    case 79:
      fprintf (stderr, "This %s is illegal.\n"
		      , name1);
      break;
    case 80:
      fprintf (stderr, "... is not last parameter.\n");
      break;
    case 81:
      fprintf (stderr, "... is only legal to C procedures.\n");
      break;
    case 82:
      fprintf (stderr, "Label %s is not legal as parameter to a C procedure.\n"
	       ,name1);
      break;
    default:
      fprintf (stderr, "System error: No Message specified.\n (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}

/******************************************************************************
                                				TEXTNUMBER   */

char *textnumber (int i)
{
  static char s[10];
  switch (i)
    {
    case 1:
      strcpy (s, "First");
      break;
    case 2:
      strcpy (s, "Second");
      break;
    case 3:
      strcpy (s, "Third");
      break;
    case 4:
      strcpy (s, "Fourth");
      break;
    case 5:
      strcpy (s, "Fifth");
      break;
    case 6:
      strcpy (s, "Sixth");
      break;
    case 7:
      strcpy (s, "Seventh");
      break;
    case 8:
      strcpy (s, "Eighth");
      break;
    case 9:
      strcpy (s, "Ninth");
      break;
    case 10:
      strcpy (s, "Tenth");
      break;
    default:
      sprintf (s, "%dth", i);
    }
  return (s);
}

/******************************************************************************
                                 				SERROR       */

/* Feil som oppdages av SJEKKEREN */

/*VARARGS1 */
void serror (int errcode, char *name, int ant)
{
  char *antt;
  if (option_nowarning && errcode >= 81 && errcode <= 83 )
    return;
  starterror (lineno);
  antt = textnumber (ant);
  switch (errcode)
    {
    case 1:
      fprintf (stderr, "Division by zero.\n");
      break;
    case 3:
      fprintf (stderr, "Illegal prefix to block.\n");
      break;
    case 4:
      fprintf (stderr, "Negative integer exponent is illegal.\n");
      break;
    case 6:
      fprintf (stderr, "No constant expression in constant declaration.\n");
      break;
    case 7:
      fprintf (stderr, "Constant is used before it is defined.\n");
      break;
    case 8:
      fprintf (stderr, "Illegal reference to label.\n");
      break;
    case 9:
      fprintf (stderr, "DOT or QUA is illegal after NONE.\n");
      break;
    case 44:
      fprintf (stderr, "System error: Computed text constant too long.\n");
      exit (TRUE);
      break;
    case 71:
      fprintf (stderr, "System error: Illegal symbol in M.\n"
		      , name);
      exit (TRUE);
      break;
    case 73:
      fprintf (stderr, "Expression after INSPECT is not of type ref.\n");
      break;
    case 75:
      fprintf (stderr, "Identifier %s is not declared.\n", name);
      break;
    case 77:
      fprintf (stderr, "Expression after IF/WHILE is not of type boolean.\n");
      break;
    case 81:
      fprintf (stderr, "Warning: FOR/WHILE-statement has empty body.\n");
      anterror--;
      break;
    case 82:
      fprintf (stderr, "Warning: When %s will always be executed, unless NONE.\n"
		      , name);
      anterror--;
      break;
    case 83:
      fprintf (stderr, "Warning: When %s will never be executed.\n"
		      , name);
      anterror--;
      break;
    case 84:
      fprintf (stderr, "WHEN is not followed by a Class name.\n");
      break;
    case 85:
      fprintf (stderr, "Illegal qualification in expression.\n");
      break;
    case 86:
      fprintf (stderr, "Illegal type on controlling variable in for-sentence.\n");
      break;
    case 87:
      fprintf (stderr, "Illegal controlling variable in for-sentence.\n");
      break;
    case 88:
      fprintf (stderr, "Illegal type in FOR/SWITCH sentences or in specification of array bounds.\n");
      break;
    case 89:
      fprintf (stderr, "Illegal (to use a local) variable in specification of array bounds.\n");
      break;
    case 90:
      fprintf (stderr, "Left argument to assignment is illegal.\n");
      break;
    case 91:
      fprintf (stderr, "Arguments to assignment have illegal type.\n");
      break;
    case 92:
      fprintf (stderr, "Different types for THEN- and ELSE-path.\n");
      break;
    case 93:
      fprintf (stderr, "IF after THEN is illegal.\n");
      break;
    case 94:
      fprintf (stderr, "No common qualification for THEN- and ELSE-path.\n");
      break;
    case 95:
      fprintf (stderr, "Argument(s) in boolean operator is not of type boolean.\n");
      break;
    case 96:
      fprintf (stderr, "Arguments to relation operator have illegal type.\n");
      break;
    case 97:
      fprintf (stderr, "Arguments to arithmetic operator have illegal type.\n");
      break;
    case 98:
      fprintf (stderr, "Double unear sub or add operator.\n");
      break;
    case 99:
      fprintf (stderr, "%s is no Class name.\n"
		      , name);
      break;
    case 100:
      fprintf (stderr, "Argument before '.'/QUA is not of type ref or text.\n");
      break;
    case 101:
      fprintf (stderr, "Identifier %s is not declared as array or procedure.\n"
		      , name);
      break;
    case 102:
      fprintf (stderr, "%s parameter to %s: Wrong number of parameters.\n"
		      , antt, name);
      break;
    case 103:
      fprintf (stderr, "%s parameter to %s: Assumed an array or procedure.\n"
		      , antt, name);
      break;
    case 104:
      fprintf (stderr, "%s parameter to %s: Illegal type.\n", antt, name);
      break;
    case 105:
      fprintf (stderr, "%s parameter to %s: Illegal parameter.\n"
		      , antt, name);
      break;
    case 106:
      fprintf (stderr, "Identifier %s is illegal.\n"
		      , name);
      break;
    case 107:
      fprintf (stderr, "Assumed parameter(s) to %s.\n"
		      , name);
      break;
    case 108:
      fprintf (stderr, "Expression after GOTO is not of type label.\n");
      break;
    case 109:
      fprintf (stderr, "Arguments to '&' operator is not of type text .\n");
      break;
    case 110:
      fprintf (stderr, "%s parameter to %s: Illegal to have expression as actual VAR.\n"
		      , antt, name);
      break;
    case 111:
      fprintf (stderr, "%s parameter to %s: Illegal kind.\n"
		      , antt, name);
      break;
    case 112:
      fprintf (stderr, "%s parameter to %s: Illegal procedure.\n"
		      , antt, name);
      break;
    case 115:
      fprintf (stderr, "Illegal expression without assignment.\n");
      break;
    case 116:
      fprintf (stderr, "'.' is not followed by an identifier.\n");
      break;
    case 117:
      fprintf (stderr, "Illegal to have '.' to a local Class.\n");
      break;
    case 118:
      fprintf (stderr, "Assignment is misplaced.\n");
      break;
    case 119:
      fprintf (stderr, "Indexes is missing for array %s.\n"
		      , name);
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}

/******************************************************************************
                                				GERROR       */

/* Feil som oppdages av kodegeneratoren */

/*VARARGS1 */
void gerror (int errcode, char *name)
{
  starterror (lineno);
  switch (errcode)
    {
    case 82:
      fprintf (stderr, "Lower bound is greater than upper bound in array %s.\n"
		      ,name);
      break;
    case 87:
      fprintf (stderr, "System error: Too deep nesting in temporary expression.\n");
      exit (TRUE);
      break;
    case 85:
      fprintf (stderr, "System error: Too many dimensions for array.\n");
      exit (TRUE);
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n"
		      , errcode);
      exit (TRUE);
    }
}


/******************************************************************************
                                                                MERROR       */

/* Feil som oppdages under innlesing av eksterne moduler */

/*VARARGS1 */
void merror (int errcode, char *name)
{
  if (option_nowarning && errcode == 3 && option_atr)
    return;
  starterror (lineno);
  switch (errcode)
    {
    case 1:
      fprintf (stderr, "Length of filename %s is too long.\n"
		      , name);
      break;
    case 3:
      fprintf (stderr, "Can't read atr-file %s.\n", name);
      if (option_atr)
	{
	  anterror--;
	  return;
	}
      break;
    case 4:
      fprintf (stderr, "External specified module %s need re-compilation.\n"
		      ,name);
      break;
    case 5:
      fprintf (stderr, "File %s has illegal format.\n"
		      ,name);
      break;
    case 6:
      fprintf (stderr, "Can't read arch-file %s.\n"
		      ,name);
      break;
    case 7:
      fprintf (stderr, "File %s is no arch-file.\n"
		      ,name);
      break;
    case 8:
      fprintf (stderr, "Illegal format on Arch-file %s.\n"
		      ,name);
      break;
    case 9:
      fprintf (stderr, "Can't write atr-file %s.\n"
		      ,name);
      break;
    case 10:
      fprintf (stderr, "Can't write mif-file %s.\n"
		      ,name);
      break;
    case 11:
      fprintf (stderr, "Atr-file %s does not have unique time-stamp (please re-compile).\n"
		      ,name);
      break;
    default:
      fprintf (stderr, "System error: No Message specified (%d).\n", 
		      errcode);
    }
  exit (TRUE);
}
