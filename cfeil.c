/* $Id: cfeil.c,v 1.19 1995/03/07 10:53:16 cim Exp $ */

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

/* Inneholder de tekstlige feilmeldingene som kompilatoren kan gi.
 * For noen tilfeller b|r det gis bedre og mer spesifike feilmeldinger.
 * Dette gjelder spesielt for feilmeldinger fra sjekkeren.
 *                                                              
 * Siden det kan inkluderes filer m} det lages et tabellverk som
 * holder greie p} hvilke linjenummere internt i kompilator-programmet
 * som h|rer til de enkelte filene.                              
 * Dette tabellverket brukes s} i forbindelse med utskrift              
 * av feilmeldinger. */

#include "cextern.h"
#include "cdekl.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

struct map
  {
    char *fil;
    long line,
      fromline;
    struct map *neste;
  }
mapinit;
struct map *mappos = &mapinit,
 *firstmappos = &mapinit
,
 *lastmappos = &mapinit,
 *mapindeks = &mapinit;
int antmap = 1;

long stackline[INCLUDELEV];
char *stackfil[INCLUDELEV];
int stackpointer;


int anterror = 0;
char stop;


/******************************************************************************
                                                     PUSHFILMAP & POPFILMAP  */
pushfilmap (file, line)
     char *file;
     long line;
{
  stackline[stackpointer] = yylineno + 1 + lastmappos->line;
  stackfil[stackpointer] = lastmappos->fil;
  stackpointer++;
  setfilmap (file, line);
}

popfilmap ()
{
  stackpointer--;
  setfilmap (stackfil[stackpointer], stackline[stackpointer]);
}

/******************************************************************************
                                                     SETFILMAP 		     */
setfilmap (file, line)
     char *file;
     long line;
{
  antmap++;
  mappos->fil = file ? file : lastmappos->fil;
  mappos->line = line - yylineno - 1;
  mappos->fromline = yylineno + 1;
  mappos = (lastmappos = mappos)->neste 
    = (struct map *) salloc (sizeof (struct map));
  mappos->fromline = MAX_INT;
}

/******************************************************************************
  						     GETMAPLINE & GETMAPFILE */

long 
getmapline (line)
     long line;
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (line + mapindeks->line);
}

char *
getmapfile (line)
     long line;
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (mapindeks->fil);
}

/******************************************************************************
                                                                      GENMAP */

genmap ()
{
  int i;
  struct map *m = firstmappos;
  fprintf (HKOD, "__map __map%s[%d]={"
	   ,separat_comp ? timestamp : "main", antmap);
  for (i = 1; i < antmap; i++)
    {
      fprintf (HKOD, "\"%s\",%ldL,%ldL,\n", m->fil, m->line, m->fromline);
      m = m->neste;
    }
  fprintf (HKOD, "\"\",0L,%ldL};\n", MAX_INT);
}


/******************************************************************************
                                                     STARTFEIL & SLUTTFEIL   */
startfeil (line)
     long line;
{
  anterror++;
  if (line == 0)
    return;
  (void) fprintf (stderr, "\"%s\", line %ld: "
		  ,getmapfile (line), getmapline (line));
}

sluttfeil ()
{
  (void) fprintf (stderr, ".\n");
  if (stop)
    exit (TRUE);
}

/******************************************************************************
                               					 LERROR      */

/* Feil som oppdages av LEX */

lerror (errcode)
     int errcode;
{
  if (option_nowarning && (errcode == 8 || errcode == 32))
    return;
  startfeil (yylineno);
  switch (errcode)
    {
    case 1:
      (void) fprintf (stderr, "Comments are not terminated with ';'");
      break;
    case 2:
      (void) fprintf (stderr, "Illegal character constant");
      break;
    case 3:
      (void) fprintf (stderr, "Character constant is not terminated");
      break;
    case 4:
      (void) fprintf (stderr
		      ,"Text constant is not terminated");
      break;
    case 5:
      (void) fprintf (stderr
		      ,"Illegal character in text constant");
      break;
    case 6:
      (void) fprintf (stderr, "Exponent is not started with a digit");
      break;
    case 7:
      (void) fprintf (stderr, "Illegal character (combination)");
      break;
    case 8:
      (void) fprintf (stderr, "Warning: Unknown directive line");
      anterror--;
      break;
    case 9:
      (void) fprintf (stderr, "Too big integer constant");
      break;
    case 10:
      (void) fprintf (stderr, "Illegal radix in integer constant");
      break;
    case 11:
      (void) fprintf (stderr, "Radix integer constant has bad format");
      break;
    case 12:
      (void) fprintf (stderr, "Illegal radix digit in integer constant");
      break;
    case 13:
      (void) fprintf (stderr, "Not enough memory");
      stop = TRUE;
      break;
    case 14:
      (void) fprintf (stderr, "Illegal real constant");
      break;
    case 15:
      (void) fprintf (stderr, "Homeless %%ENDCOMMENT");
      break;
    case 16:
      (void) fprintf (stderr, "Radix integer is illegal in real constant");
      break;
    case 17:
      (void) fprintf (stderr, "%%INCLUDE directive has too high nesting level");
      break;
    case 18:
      (void) fprintf (stderr, "File specified by %%INCLUDE can't be opened");
      break;
    case 19:
      (void) fprintf (stderr, "%%COMMENT is not terminated with %%ENDCOMMENT");
      break;
    case 20:
      (void) fprintf (stderr, "Length of token is too long to scan");
      break;
    case 21:
      (void) fprintf (stderr, "Homeless %%ELSE(NOT)DEF");
      break;
    case 22:
      (void) fprintf (stderr, "Homeless %%ELSE");
      break;
    case 23:
      (void) fprintf (stderr, "Homeless %%ENDIF");
      break;
    case 24:
      (void) fprintf (stderr, "%%IF(NOT)DEF is not terminated with %%ENDIF");
      break;
#if HAVE_SIGFPE
    case 25:
      (void) fprintf (stderr, "Arithmetic overflow");
      break;
#endif
#if HAVE_SIGSEGV
    case 26:
      (void) fprintf (stderr, "System error: Segmentation violation");
      stop = TRUE;
      break;
#endif
#if HAVE_SIGBUS
    case 27:
      (void) fprintf (stderr, "System error: Bus error");
      stop = TRUE;
      break;
#endif
#if HAVE_SIGILL
    case 28:
      (void) fprintf (stderr, "System error: Illegal instruction");
      stop = TRUE;
      break;
#endif
#if HAVE_SIGTRAP
    case 29:
      (void) fprintf (stderr, "System error: Trace trap");
      stop = TRUE;
      break;
#endif
#if HAVE_SIGSYS
    case 30:
      (void) fprintf (stderr, "System error: Bad argument to system call");
      stop = TRUE;
      break;
#endif
#if FLOAT_IMPLEMENTED
#else
    case 31:
      (void) fprintf (stderr, "Real not implemented");
      break;
#endif
    case 32:
      (void) fprintf (stderr, "Warning: May have forgotten ';' after END");
      anterror--;
      break;
    case 44:
      (void) fprintf (stderr, "System error: Too long text constant");
      stop = TRUE;
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}

/******************************************************************************
                           					YERROR       */

/* Feil som oppdages av YACC */

/*VARARGS1 */
yerror (errcode, txt)
     int errcode;
     char *txt;
{
  char c;
  startfeil (yylineno);
  switch (errcode)
    {
    case 1:
      (void) fprintf (stderr, "Illegal kind on external procedure");
      break;
    case 2:
      (void) fprintf (stderr, "IN is illegal in external declaration");
      break;
    case 8:
      (void) fprintf (stderr,
		      "IN is illegal in specification of formal procedure");
      break;
    case 7:
      (void) fprintf (stderr, "Type REF is illegal in constant declaration");
      break;
    case 9:
      (void) fprintf (stderr,
	"Relation operator other than '=' is used in constant declaration");
      break;
    case 13:
      (void) fprintf (stderr, "Syntax error");
      break;
    case 14:
      (void) fprintf (stderr, "System error: Yacc stack overflow");
      stop = TRUE;
      break;
    case 15:
      (void) fprintf (stderr, "IF after THEN is illegal");
      break;
    case 16:
      (void) fprintf (stderr,
		      "System error: No Message specified from Yacc");
      stop = TRUE;
      break;
    case 17:
      (void) fprintf (stderr, "Mode part is misplaced");
      break;
    case 18:
      (void) fprintf (stderr, "Specification part is misplaced");
      break;
    case 19:
      (void) fprintf (stderr, "Protection part is misplaced");
      break;
    case 20:
      (void) fprintf (stderr, "Virtual part is misplaced");
      break;
    case 21:
      c = toupper ((int) txt[0]);
      txt++;
      (void) fprintf (stderr, "%c%s", c, txt);
      break;
    case 24:
      (void) fprintf (stderr, "Illegal with statement after declaration");
      break;
    case 25:
      (void) fprintf (stderr, "Illegal empty statement");
      break;
    case 26:
      (void) fprintf (stderr, "Declaration is found after a sentence");
      break;
    case 27:
      (void) fprintf (stderr, "Declaration is found after a label");
      break;
    case 28:
      (void) fprintf (stderr, "Declaration is not terminated with ';'");
      break;
    case 29:
      (void) fprintf (stderr, "Illegal with declaration as body");
      break;
    case 45:
      (void) fprintf (stderr,
		      "Formal or virtual procedure shall be specified");
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}

/******************************************************************************
                                				D1ERROR      */

/* Feil som oppdages av DECL PASS 1 */

/*VARARGS1 */
d1error (errcode, name)
     int errcode;
     char *name;
{
  startfeil (yylineno);
  switch (errcode)
    {
    case 32:
      (void) fprintf (stderr
		   ,"System error: Illegal declaration on outermost level");
      stop = TRUE;
      break;
    case 33:
      (void) fprintf (stderr
		 ,"System error: Can't find the descr of this declaration");
      stop = TRUE;
      break;
    case 37:
      (void) fprintf (stderr, "System error: Illegal categ in declaration");
      stop = TRUE;
      break;
    case 31:
      (void) fprintf (stderr, "System error: Too high block level");
      stop = TRUE;
      break;
    case 34:
      (void) fprintf (stderr, "Identifier %s is not a parameter", name);
      break;
    case 35:
      (void) fprintf (stderr, "Identifier %s is specified twice in mode part", name);
      break;
    case 36:
      (void) fprintf (stderr, "Identifier %s is specified twice", name);
      break;
    case 41:
      (void) fprintf (stderr, "Identifier %s is specified twice as HIDDEN/PROTECTED", name);
      break;
    case 38:
      (void) fprintf (stderr, "INNER is not on outermost block level in this Class");
      break;
    case 39:
      (void) fprintf (stderr, "More than one inner in this Class");
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}

/******************************************************************************
                                    				D2ERROR      */

/* Feil som oppdages av DECL PASS 2 */

/*VARARGS1 */
d2error (errcode, rd1, rd2)
     int errcode;
     struct DECL *rd1,
      *rd2;
{
  char *name1 = rd1->ident;
  startfeil (errcode == 79 ? yylineno : rd1->line);
  switch (errcode)
    {
    case 49:
      (void) fprintf (stderr, "%s is illegal as prefix to %s (seen through inspect)"
		      ,rd2->ident, name1);
      break;
    case 50:
      (void) fprintf (stderr, "%s is illegal as prefix to %s (not a Class)"
		      ,rd2->ident, name1);
      break;
    case 51:
      (void) fprintf (stderr, "%s is illegal as prefix to %s (not declared at same block level)"
		      ,rd2->ident, name1);
      break;
    case 52:
      (void) fprintf (stderr, "%s is illegal as prefix to %s (circulated prefix chain)"
		      ,rd2->ident, name1);
      break;
    case 53:
      (void) fprintf (stderr, "%s have unknown qualification", name1);
      break;
    case 54:
      (void) fprintf (stderr, "%s have illegal qualification", name1);
      break;
    case 55:
      (void) fprintf (stderr, "%s is declared twice", name1);
      break;
    case 56:
      (void) fprintf (stderr, "%s is illegal parameter to Class", name1);
      break;
    case 57:
      (void) fprintf (stderr, "%s can't be transmitted by value", name1);
      break;
    case 58:
      (void) fprintf (stderr, "%s is illegal as name to a Class", name1);
      break;
    case 59:
      (void) fprintf (stderr, "System error: Illegal categ in declaration");
      stop = TRUE;
      break;
    case 60:
      (void) fprintf (stderr, "%s is already specified as virtual", name1);
      break;
    case 61:
      (void) fprintf (stderr, "%s is illegal as virtual", name1);
      break;
    case 62:
      (void) fprintf (stderr, "%s is illegal match", name1);
      break;
    case 63:
      (void) fprintf (stderr, "%s is not specified", name1);
      break;
    case 64:
      (void) fprintf (stderr, "%s have too high prefix level", name1);
      break;
    case 72:
      (void) fprintf (stderr
	  ,"This %s is illegal since the Class is used as prefix to a block"
		      ,name1);
      break;
    case 73:
      (void) fprintf (stderr, "Illegal inspect");
      break;
    case 74:
      (void) fprintf (stderr, "%s is undeclared"
		      ,name1);
      break;
    case 75:
      (void) fprintf (stderr
		      ,"%s is not declared local", name1);
      break;
    case 76:
      (void) fprintf (stderr, "%s is not Protected", name1);
      break;
    case 77:
      (void) fprintf (stderr
		      ,"Text/ref atributte %s can't be transmitted by name to a C procedure", name1);
      break;
    case 78:
      (void) fprintf (stderr
		      ,"External C procedure %s can't have type ref", name1);
      break;
    case 79:
      (void) fprintf (stderr, "This %s is illegal", name1);
      break;
    case 80:
      (void) fprintf (stderr, "... is not last parameter");
      break;
    case 81:
      (void) fprintf (stderr, "... is only legal to C procedures");
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}

/******************************************************************************
                                				TEXTNUMBER   */

char *
textnumber (i)
     int i;
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
      sprintf (s, "%d.", i);
    }
  return (s);
}

/******************************************************************************
                                 				SERROR       */

/* Feil som oppdages av SJEKKEREN */

/*VARARGS1 */
serror (errcode, name, ant)
     int errcode,
       ant;
     char *name;
{
  char *antt;
  if (option_nowarning && errcode == 83)
    return;
  startfeil (yylineno);
  antt = textnumber (ant);
  switch (errcode)
    {
    case 1:
      (void) fprintf (stderr, "Division by zero");
      break;
    case 3:
      (void) fprintf (stderr, "Illegal prefix to block");
      break;
    case 4:
      (void) fprintf (stderr, "Negative integer exponent is illegal");
      break;
    case 6:
      (void) fprintf (stderr, "No constant expression in constant declaration");
      break;
    case 7:
      (void) fprintf (stderr, "Constant is used before it is defined");
      break;
    case 8:
      (void) fprintf (stderr, "Illegal reference to label");
      break;
    case 9:
      (void) fprintf (stderr, "DOT or QUA is illegal after NONE");
      break;
    case 44:
      (void) fprintf (stderr, "System error: Computed text constant too long");
      stop = TRUE;
      break;
    case 70:
      (void) fprintf (stderr, "System error: Too big expression");
      stop = TRUE;
      break;
    case 71:
      (void) fprintf (stderr, "System error: Illegal symbol in M", name);
      stop = TRUE;
      break;
    case 73:
      (void) fprintf (stderr, "Expression after INSPECT is not of type ref");
      break;
    case 75:
      (void) fprintf (stderr, "Identifier %s is not declared", name);
      break;
    case 77:
      (void) fprintf (stderr, "Expression after IF/WHILE is not of type boolean");
      break;
    case 81:
      (void) fprintf (stderr, "System error: Illegal syntax in M");
      stop = TRUE;
      break;
    case 82:
      (void) fprintf (stderr, "System error: Unexpected EOF in M");
      stop = TRUE;
      break;
    case 83:
      (void) fprintf (stderr, "Warning: When %s will never be executed", name);
      anterror--;
      break;
    case 84:
      (void) fprintf (stderr, "WHEN is not followed by a Class name");
      break;
    case 85:
      (void) fprintf (stderr, "Illegal qualification in expression");
      break;
    case 86:
      (void) fprintf (stderr, "Illegal type on controlling ");
      (void) fprintf (stderr, "variable in for-sentence");
      break;
    case 87:
      (void) fprintf (stderr,
		      "Illegal controlling variable in for-sentence");
      break;
    case 88:
      (void) fprintf (stderr, "Illegal type in FOR/SWITCH sentences or in specification of array bounds");
      break;
    case 89:
      (void) fprintf (stderr, "Illegal (to use a local) variable in specification of array bounds");
      break;
    case 90:
      (void) fprintf (stderr, "Left argument to assignment is illegal");
      break;
    case 91:
      (void) fprintf (stderr, "Arguments to assignment have illegal type");
      break;
    case 92:
      (void) fprintf (stderr, "Different types for THEN- and ELSE-path");
      break;
    case 93:
      (void) fprintf (stderr, "IF after THEN is illegal");
      break;
    case 94:
      (void) fprintf (stderr, "No common qualification for THEN- and ELSE-path");
      break;
    case 95:
      (void) fprintf (stderr, "Argument(s) in boolean operator is not of type boolean");
      break;
    case 96:
      (void) fprintf (stderr, "Arguments to relation operator have illegal type");
      break;
    case 97:
      (void) fprintf (stderr, "Arguments to arithmetic operator have illegal type");
      break;
    case 98:
      (void) fprintf (stderr, "Double unear sub or add operator");
      break;
    case 99:
      (void) fprintf (stderr, "%s is no Class name", name);
      break;
    case 100:
      (void) fprintf (stderr, "Argument before '.'/QUA is not of type ref or text");
      break;
    case 101:
      (void) fprintf (stderr, "Identifier %s is not declared as array or procedure", name);
      break;
    case 102:
      (void) fprintf (stderr, "%s parameter to %s: Wrong number of parameters", antt, name);
      break;
    case 103:
      (void) fprintf (stderr, "%s parameter to %s: Assumed an array or procedure", antt, name);
      break;
    case 104:
      (void) fprintf (stderr, "%s parameter to %s: Illegal type", antt, name);
      break;
    case 105:
      (void) fprintf (stderr, "%s parameter to %s: Illegal parameter", antt, name);
      break;
    case 106:
      (void) fprintf (stderr, "Identifier %s is illegal", name);
      break;
    case 107:
      (void) fprintf (stderr, "Assumed parameter(s) to %s", name);
      break;
    case 108:
      (void) fprintf (stderr, "Expression after GOTO is not of type label");
      break;
    case 109:
      (void) fprintf (stderr, "Arguments to '&' operator is not of type text ");
      break;
    case 110:
      (void) fprintf (stderr, "%s parameter to %s: Illegal to have expression as actual VAR", antt, name);
      break;
    case 111:
      (void) fprintf (stderr, "%s parameter to %s: Illegal kind", antt, name);
      break;
    case 112:
      (void) fprintf (stderr, "%s parameter to %s: Illegal procedure", antt, name);
      break;
    case 115:
      (void) fprintf (stderr, "Illegal expression without assignment");
      break;
    case 116:
      (void) fprintf (stderr, "'.' is not followed by an identifier");
      break;
    case 117:
      (void) fprintf (stderr, "Illegal to have '.' to a local Class");
      break;
    case 118:
      (void) fprintf (stderr,
		      "Assignment is misplaced");
      break;
    case 119:
      (void) fprintf (stderr, "Indexes is missing for array %s", name);
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}

/******************************************************************************
                                				GERROR       */

/* Feil som oppdages av kodeGENERATOREN */

/*VARARGS1 */
gerror (errcode, name)
     int errcode;
     char *name;
{
  startfeil (yylineno);
  switch (errcode)
    {
    case 82:
      (void) fprintf (stderr, "Lower bound is greater than upper bound in array %s",
		      name);
      break;
    case 87:
      (void) fprintf (stderr, "System error: Too deep nesting in temporary expression");
      stop = TRUE;
      break;
    case 88:
      (void) fprintf (stderr, "System error: Too deep nesting of temporary expressions");
      stop = TRUE;
      break;
    case 84:
      (void) fprintf (stderr, "System error: Too deep nesting for the code generator");
      stop = TRUE;
      break;
    case 85:
      (void) fprintf (stderr, "System error: Too many dimensions for array");
      stop = TRUE;
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
      stop = TRUE;
    }
  sluttfeil ();
}


/******************************************************************************
                                                                MERROR       */

/* Feil som oppdages under innlesing av eksterne moduler */

/*VARARGS1 */
merror (errcode, name)
     int errcode;
     char *name;
{
  if (option_nowarning && errcode == 3 && option_atr)
    return;
  startfeil (yylineno);
  stop = TRUE;
  switch (errcode)
    {
    case 1:
      (void) fprintf (stderr, "Length of filename %s is too long", name);
      break;
    case 3:
      if (option_atr)
	{
	  anterror--;
	  stop = FALSE;
	}
      (void) fprintf (stderr, "Can't read atr-file %s", name);
      break;
    case 4:
      (void) fprintf (stderr, "External specified module %s need re-compilation"
		      ,name);
      break;
    case 5:
      (void) fprintf (stderr, "File %s have illegal format"
		      ,name);
      break;
    case 6:
      (void) fprintf (stderr, "Can't read arch-file %s"
		      ,name);
      break;
    case 7:
      (void) fprintf (stderr, "File %s is no arch-file"
		      ,name);
      break;
    case 8:
      (void) fprintf (stderr, "Illegal format on Arch-file %s"
		      ,name);
      break;
    case 9:
      (void) fprintf (stderr, "Can't write atr-file %s"
		      ,name);
      break;
    case 11:
      (void) fprintf (stderr, "Atr-file %s does not have unique time-stamp (re-compile)"
		      ,name);
      break;
    default:
      (void) fprintf (stderr, "System error: No Message specified");
      (void) fprintf (stderr, " (%d)", errcode);
    }
  sluttfeil ();
}
