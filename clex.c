/* $Id: clex.c,v 1.15 1997/02/14 16:33:00 cim Exp $ */

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

/* Scanner for Simula */

#include "cextern.h"
#include <ctype.h>
#include "cdekl.h"
#include "cmell.h"
#include "y.tab.h"
#include "cnavn.h"

#define unput(c) {(void)ungetc(c,filstack[fillev]);}
int notintext = TRUE;

FILE *filstack[INCLUDELEV];
int fillev;

#define  newlexchar (lexchar=input())
int newsymbole = 0;
long yylineno,
  antnewline;
int lexradix,
  i,
  yyleng;
int lexchar;
unsigned char firstchar,
  secondchar,
  thirdchar;
unsigned char yytext[YYTEXTLENGTH];

extern char mainroutine[MAINROUTINELENGTH + 1];
int pardeep = 0;
int antsimchar = 0;
int antchar = 0;
char leerror = FALSE;
char *str;
int str_length;

char end_of_file;

char external = FALSE;		/* Har man sett "EXTERNAL PROC/CLASS =" angir 
				 * external at man venter et filnavn 
				 * som ikke skal behandles som
				 * en text-konstant. */

int ifdefniv;

char ifdef[IFDEFMAXNIV];
char includeif[INCLUDELEV + 1];

char scan,
  elsedef,
  notdef;

/* Brytere som kan slåes AV/P} ved hjelp av kompilator-direktiver */
char nonetest = ON,
  indextest = ON,
  stripsideeffects = OFF,
  nameasvar = OFF,
  sensitive = OFF,
  flaggpass2,
  staticblock = OFF;

/******************************************************************************
                                                                       INPUT */

/* Egen definisjon  på input for å  gjøre små  bokstaver om  til store
 * Samt fjerne ^@ fra input
 * Dette skal dog ikke gj\res innenfor text eller karakter -konstanter */

int 
input ()
{
  int yytchar;
  if (end_of_file)
    {
      end_of_file--;
      yytchar = EOF;
    }
  else
    do
      {
	yytchar = getc (filstack[fillev]);
      }
    while (yytchar == 0L);
  if (iscntrl (yytchar) && !isspace (yytchar) && yytchar != '\b'
      && yytchar != EOF
#if ISO_LATIN
      && yytchar<160
#endif
      )
    lerror (7);
  if (yytchar == '#' && notintext) return('%');
  return (islower (yytchar) && notintext && sensitive == OFF 
	  ? toupper (yytchar) : yytchar);
}

/* Behandler kompilator direktiver.
 *            F|lgende kompilator-direktiv er implementert:
 *                      %comment    Kommentar frem til matchende %endcomment.
 *                                  Dette direktivet kan v{re nestet.
 *                      %endcomment Se ovenfor.
 *                      %include    Inkludere en annen fil.
 *                      %indextest  on/off initielt satt lik on.
 *                      %line       Si hvilken line/fil source kommer fra.
 *                      %nameasvar  on/off Name-parameter overf|rt som var.
 *                      %nonetest   on/off Skal nonetest utf|res ved referanse
 *                                  til objekter.
 *                      %stripsideeffects  Ikke ta hensyn til sideeffekter i
 *                                  uttrykk.
 *                      %casesensitive 
 *                                  on/off Case-sensitive p} samtlige symboler.
 *                                  Hvis on er satt s} m} n|kkelord skrives
 *                                  med store bokstaver.
 *                                  Beskyttede attributter i simset og simu-
 *                                  lation er implementert ved hjelp av dette
 *                                  direktivet.
 *                      %nocomment  Resten av linje blir behandlet p} vanlig
 *                                  m}te. Dette direktivet er nyttig p} den
 *                                  m}ten at andre kompilatorer vanligvis vil
 *                                  ignorere denne linjen, mens cim ikke 
 *                                  gj|r det. F.eks. Lund gir bare en warning
 *                                  for slike linjer.
 *                      %define     Definerer et symbol.
 *                      %undef      Fjern en definisjon av et symbol.
 *                      %ifdef      Sjekker om symbolet er definert.
 *                      %ifnotdef   Sjekker om symbolet ikke er definert.
 *                      %else       Se nedenfor.
 *                      %endif      %ifdef og %ifnotdef er etterfulgt av 
 *                                  et antall linjer muligens etterfulgt
 *                                  av %else og deretter etterfulgt av %endif.
 *                                  Hvis betingelsen er sann da vil alle
 *                                  linjer mellom %else og %endif bli ignorert.
 *                                  Ellers blir alle linjer mellom testen
 *                                  og %else eller %endif
 *                                  (hvis %else er utelatt) ignorert.
 *                      %elsedef    Forkortelse for %else - %ifdef - %endif.
 *                      %elsenotdef Forkortelse for %else - %ifnotdef - %endif.
 *                      %timestamp  Setter opp et tidsmerke for en 
 *                                  ekstern modul
 *                      %eof        End of file. Brukes for include-filer
 *                                  som er lagt i ar-biblioteker. */

/******************************************************************************
                                                             PRINT_LEXSYMBOL */

print_lexsymbol (lextok, yylvalp)
     int lextok;
     YYSTYPE *yylvalp;
{
  switch (lextok)
    {
    case HACTIVATE:
      printf ("ACTIVATE ");
      break;
    case HAFTER:
      printf ("AFTER ");
      break;
    case HARRAY:
      printf ("ARRAY ");
      break;
    case HAT:
      printf ("AT ");
      break;
    case HBEFORE:
      printf ("BEFORE ");
      break;
    case HBEGIN:
      printf ("BEGIN ");
      break;
    case HBOOLEAN:
      printf ("BOOLEAN ");
      break;
    case HCHARACTER:
      printf ("CHARACTER ");
      break;
    case HCLASS:
      printf ("CLASS ");
      break;
    case HDELAY:
      printf ("DELAY ");
      break;
    case HDO:
      printf ("DO ");
      break;
    case HELSE:
      printf ("ELSE ");
      break;
    case HEND:
      printf ("END ");
      break;
    case HEXTERNAL:
      printf ("EXTERNAL ");
      break;
    case HFOR:
      printf ("FOR ");
      break;
    case HGO:
      printf ("GO ");
      break;
    case HGOTO:
      printf ("GOTO ");
      break;
    case HHIDDEN:
      printf ("HIDDEN ");
      break;
    case HIF:
      printf ("IF ");
      break;
    case HINNER:
      printf ("INNER ");
      break;
    case HINSPECT:
      printf ("INSPECT ");
      break;
    case HINTEGER:
      printf ("INTEGER ");
      break;
    case HLABEL:
      printf ("LABEL ");
      break;
    case HLONG:
      printf ("LONG ");
      break;
    case HNAME:
      printf ("NAME ");
      break;
    case HNEW:
      printf ("NEW ");
      break;
    case HNONE:
      printf ("NONE ");
      break;
    case HNOTEXT:
      printf ("NOTEXT ");
      break;
    case HOTHERWISE:
      printf ("OTHERWISE ");
      break;
    case HPRIOR:
      printf ("PRIOR ");
      break;
    case HPROCEDURE:
      printf ("PROCEDURE ");
      break;
    case HPROTECTED:
      printf ("PROTECTED ");
      break;
    case HQUA:
      printf ("QUA ");
      break;
    case HREACTIVATE:
      printf ("REACTIVATE ");
      break;
    case HREAL:
      printf ("REAL ");
      break;
    case HREF:
      printf ("REF ");
      break;
    case HSHORT:
      printf ("SHORT ");
      break;
    case HSTEP:
      printf ("STEP ");
      break;
    case HSWITCH:
      printf ("SWITCH ");
      break;
    case HTEXT:
      printf ("TEXT ");
      break;
    case HTHEN:
      printf ("THEN ");
      break;
    case HTHIS:
      printf ("THIS ");
      break;
    case HTO:
      printf ("TO ");
      break;
    case HUNTIL:
      printf ("UNTIL ");
      break;
    case HVALUE:
      printf ("VALUE ");
      break;
    case HVIRTUAL:
      printf ("VIRTUAL ");
      break;
    case HWHEN:
      printf ("WHEN ");
      break;
    case HWHILE:
      printf ("WHILE ");
      break;
    case HPAREXPSEPARATOR:
      printf (", ");
      break;
    case HLABELSEPARATOR:
      printf (": ");
      break;
    case HSTATEMENTSEPARATOR:
      printf ("; ");
      break;
    case HBEGPAR:
      printf ("( ");
      break;
    case HENDPAR:
      printf (") ");
      break;
    case HEQV:
      printf ("EQV ");
      break;
    case HIMP:
      printf ("IMP ");
      break;
    case HOR:
      printf ("OR ");
      break;
    case HAND:
      printf ("AND ");
      break;
    case HNOT:
      printf ("NOT ");
      break;
    case HPRIMARYOPERATOR:
      printf ("** ");
      break;
    case HDOT:
      printf (". ");
      break;
    case HDOTDOTDOT:
      printf ("... ");
      break;
    case HCONC:
      printf ("& ");
      break;
    case HINTEGERKONST:
      printf ("%d ", yylvalp->ival);
      break;
    case HREALKONST:
      sprintf ((char *) yytext, "%.16e", yylvalp->rval);
      for (yyleng = 0; yytext[yyleng]; yyleng++)
	if (yytext[yyleng] == 'e')
	  putchar ('&');
	else
	  putchar (yytext[yyleng]);
      putchar (' ');
      break;
    case HCHARACTERKONST:
      if (iscntrl (yylvalp->ival)
	  || yylvalp->ival == '\'')
	printf ("'!%d!'", yylvalp->ival);
      else
	printf ("'%c' ", yylvalp->ival);
      break;
    case HTEXTKONST:
      sprintf ((char *) yytext, "\"%s\"", yylvalp->ival);
      for (yyleng = 0; yytext[yyleng]; yyleng++)
	if (yytext[yyleng] == '\\')
	  {
	    char i = 0;
	    i = i * 8 + yytext[++yyleng] - '0';
	    i = i * 8 + yytext[++yyleng] - '0';
	    i = i * 8 + yytext[++yyleng] - '0';
	    if (i == ' ')
	      putchar (' ');
	    else
	      printf ("!%d!", i);
	  }
	else
	  putchar (yytext[yyleng]);
      putchar (' ');

      break;
    case HBOOLEANKONST:
      printf (yylvalp->ival ? "TRUE " : "FALSE ");
      break;
    case HIDENTIFIER:
      printf ("%s ", yylvalp->ident);
      break;

    case HASSIGN:
    case HVALRELOPERATOR:
    case HREFRELOPERATOR:
    case HOBJRELOPERATOR:
    case HTERMOPERATOR:
    case HFACTOROPERATOR:
      switch (yylvalp->token)
	{
	case HASSIGNVALUE:
	  printf (":=  ");
	  break;
	case HASSIGNREF:
	  printf (":- ");
	  break;

	case HEQ:
	  printf ("= ");
	  break;
	case HGE:
	  printf (">= ");
	  break;
	case HGT:
	  printf ("> ");
	  break;
	case HLE:
	  printf ("<= ");
	  break;
	case HLT:
	  printf ("< ");
	  break;
	case HNE:
	  printf ("<> ");
	  break;

	case HEQR:
	  printf ("== ");
	  break;
	case HNER:
	  printf ("=/= ");
	  break;

	case HIN:
	  printf ("IN ");
	  break;
	case HIS:
	  printf ("IS ");
	  break;

	case HADD:
	  printf ("+ ");
	  break;
	case HSUB:
	  printf ("- ");
	  break;

	case HMUL:
	  printf ("* ");
	  break;
	case HDIV:
	  printf ("/ ");
	  break;
	case HINTDIV:
	  printf ("// ");
	  break;

	default:
	  printf ("# ");
	}
      break;
    default:
      printf ("# ");
    }
}

/******************************************************************************
                                                       SCAN_AND_WRITE_TOKENS */

scan_and_write_tokens ()
{
  int token;
  long line = 1L;
  /* printf("% Cim_pp\n%line 1 %s\n",prognavn); */
  while (token = yylex ())
    {
      while (line < yylineno)
	{
	  putchar ('\n');
	  line++;
	}
      print_lexsymbol (token, &yylval);
    }
}

extern char option_bl_in_dir_line;

/******************************************************************************
                                                                   DIRLINE   */

dirline ()
{
  static char dirtext[DIRTEXTLENGTH];
  int dirpos = 0;
  char *flag,
    mellflag,
    bl_in_dir_line;
  yylineno += antnewline;
  for (; antnewline; antnewline--)
    if (!option_write_tokens)
      (void) putc (MNEWLINE, MELL);
  if (newlexchar == ' ' && option_bl_in_dir_line)
    {
      bl_in_dir_line = TRUE;
      newlexchar;
    }
  else
    bl_in_dir_line = FALSE;
  if (isalpha (lexchar))
    {
      while (isalnum (lexchar) || lexchar == '_')
	{
	  dirtext[dirpos++] = lexchar;
	  if (dirpos == DIRTEXTLENGTH)
	    {
	      if (!bl_in_dir_line)
		lerror (8);
	      goto skipline;
	    }
	  newlexchar;
	}
      dirtext[dirpos] = '\0';
      if (lexchar != '\n' && lexchar != EOF
	  && lexchar != ' ' && lexchar != '\t')
	{
	  if (!bl_in_dir_line)
	    lerror (8);
	  goto skipline;
	}
      switch (dirtext[0])
	{
	case 'C':
	  if (!strcmp (dirtext, "CASESENSITIVE"))
	    {
	      flag = &sensitive;
	      goto dirflags;
	    }
	  if (!strcmp (dirtext, "COMMENT"))
	    {
	      int comlev = 1;
	      while (comlev)
		{
		  while (lexchar != '\n' && lexchar != EOF)
		    newlexchar;
		  if (lexchar == EOF)
		    {
		      lerror (19);
		      goto skipline;
		    }
		  yylineno++;
		  if (!option_write_tokens)
		    (void) putc (MNEWLINE, MELL);
		  dirpos = 0;
		  if (newlexchar == '%' && ((newlexchar == ' ' &&
				       option_bl_in_dir_line) ? newlexchar :
					    0, TRUE) && isalpha (lexchar))
		    {
		      while (isalnum (lexchar) || lexchar == '_')
			{
			  dirtext[dirpos++] = lexchar;
			  if (dirpos == DIRTEXTLENGTH)
			    goto cont;
			  newlexchar;
			}
		      dirtext[dirpos] = '\0';
		      if (!strcmp (dirtext, "COMMENT"))
			comlev++;
		      if (!strcmp (dirtext, "ENDCOMMENT"))
			comlev--;
		    }
		cont:;
		}
	      goto skipline;
	    }
	  break;
	case 'D':
	  if (!strcmp (dirtext, "DEFINE"))
	    {
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (lexchar == '\n' | lexchar == EOF)
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      while (lexchar != '\n' && lexchar != EOF
		     && lexchar != ' ' && lexchar != '\t')
		if (yyleng > SAFEYYTEXTLENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  {
		    yytext[yyleng++] = lexchar;
		    newlexchar;
		  }
	      yytext[yyleng] = '\0';
	      systag (yytext, yyleng)->definition = TRUE;
	      goto skipline;
	    }
	  break;
	case 'E':
	  if (!strcmp (dirtext, "EOF"))
	    {
	      fclose (filstack[fillev]);
	      end_of_file = 2;
	      return;
	    }
	  if (!strcmp (dirtext, "ELSE"))
	    goto pelse;
	  if (!strcmp (dirtext, "ELSEDEF"))
	    {
	      elsedef = TRUE;
	      notdef = FALSE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "ELSENOTDEF"))
	    {
	      elsedef = TRUE;
	      notdef = TRUE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "ENDIF"))
	    goto pendif;
	  if (!strcmp (dirtext, "ENDCOMMENT"))
	    {
	      lerror (15);
	      goto skipline;
	    }
	  break;
	case 'I':
	  if (!strcmp (dirtext, "IFDEF"))
	    {
	      elsedef = FALSE;
	      notdef = FALSE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "IFNOTDEF"))
	    {
	      elsedef = FALSE;
	      notdef = TRUE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "INCLUDE"))
	    {
	      notintext = FALSE;
	      systag ("INCLUDED", 8)->definition = TRUE;
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (lexchar == '\n' | lexchar == EOF)
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      while (lexchar != '\n' && lexchar != EOF
		     && lexchar != ' ' && lexchar != '\t')
		if (yyleng > SAFEYYTEXTLENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  {
		    yytext[yyleng++] = lexchar;
		    newlexchar;
		  }
	      notintext = TRUE;
	      yytext[yyleng] = '\0';
	      if (fillev++ == INCLUDELEV)
		{
		  lerror (17);
		  fillev--;
		}
	      else
		filstack[fillev] = positionarch (yytext);
	      if (filstack[fillev] == NULL)
		{
		  lerror (18);
		  fillev--;
		}
	      else
		{
		  pushfilmap (tag (yytext, yyleng), 1L);
		  includeif[fillev] = ifdefniv;
		  if (option_verbose)
		    fprintf (stderr, "Reading include file %s\n", yytext);
		}
	      /* Resten av linja er kommentar */
	      goto skipline;
	    }
	  else if (!strcmp (dirtext, "INDEXTEST"))
	    {
	      flag = &flaggpass2;
	      mellflag = MINDEXTEST;
	      goto dirflags;
	    }
	  break;
	case 'L':
	  if (!strcmp (dirtext, "LIST")) goto skipline;
	  else if (!strcmp (dirtext, "LINE"))
	    {
	      long nylinje;
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (!isdigit (lexchar))
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      yytext[yyleng++] = lexchar;
	      while (isdigit (newlexchar))
		if (yyleng > SAFEYYTEXTLENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  yytext[yyleng++] = lexchar;
	      yytext[yyleng++] = '\0';
	      nylinje = radix (10, yytext);
	      notintext = FALSE;
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (lexchar != '\n' & lexchar != EOF)
		{
		  yyleng = 0;
		  while (lexchar != '\n' && lexchar != EOF
			 && lexchar != ' ' && lexchar != '\t')
		    if (yyleng > SAFEYYTEXTLENGTH)
		      {
			lerror (20);
			yyleng = 1;
		      }
		    else
		      {
			yytext[yyleng++] = lexchar;
			newlexchar;
		      }
		  yytext[yyleng] = '\0';
		  setfilmap (tag (yytext, yyleng), nylinje);
		}
	      else
		setfilmap ((char *) NULL, nylinje);
	      /* Resten av linja er kommentar */
	      notintext = TRUE;
	      goto skipline;
	    }
	  break;
	case 'M':
	  if (!strcmp (dirtext, "MAIN"))
	    {
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (!(isalnum (lexchar) || lexchar == '_'))
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      mainroutine[yyleng++] = lexchar;
	      while ((isalnum (newlexchar) || lexchar == '_'))
		if (yyleng == MAINROUTINELENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  mainroutine[yyleng++] = lexchar;
	      mainroutine[yyleng++] = '\0';
	      goto skipline;
	    }
	  break;
	case 'N':
	  if (!strcmp (dirtext, "NOCOMMENT"))
	    return;
	  if (!strcmp (dirtext, "NAMEASVAR"))
	    {
	      flag = &nameasvar;
	      goto dirflags;
	    }
	  else if (!strcmp (dirtext, "NONETEST"))
	    {
	      flag = &flaggpass2;
	      mellflag = MNONETEST;
	      goto dirflags;
	    }
	  break;
	case 'P':
	  if (!strcmp (dirtext, "PAGE")) goto skipline;
	  break;
	case 'S':
	  if (!strcmp (dirtext, "STRIPSIDEEFFECTS"))
	    {
	      flag = &flaggpass2;
	      mellflag = MSTRIPSIDEEFFECTS;
	      goto dirflags;
	    }
	  else if (!strcmp (dirtext, "STATICBLOCK"))
	    {
	      flag = &staticblock;
	      goto dirflags;
	    }
	  break;
	case 'T':
	  if (!strcmp (dirtext, "TIMESTAMP"))
	    {
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (!(isalnum (lexchar) || lexchar == '_'))
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      directive_timestamp[yyleng++] = lexchar;
	      while ((isalnum (newlexchar) || lexchar == '_'))
		if (yyleng == TIMESTAMPLENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  directive_timestamp[yyleng++] = lexchar;
	      directive_timestamp[yyleng++] = '\0';
	      goto skipline;
	    }
	  else if (!strcmp (dirtext, "TITLE")) goto skipline;
	  break;
	case 'U':
	  if (!strcmp (dirtext, "UNDEF"))
	    {
	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (lexchar == '\n' | lexchar == EOF)
		{
		  if (!bl_in_dir_line)
		    lerror (8);
		  goto skipline;
		}
	      yyleng = 0;
	      while (lexchar != '\n' && lexchar != EOF
		     && lexchar != ' ' && lexchar != '\t')
		if (yyleng > SAFEYYTEXTLENGTH)
		  {
		    lerror (20);
		    yyleng = 1;
		  }
		else
		  {
		    yytext[yyleng++] = lexchar;
		    newlexchar;
		  }
	      yytext[yyleng] = '\0';
	      systag (yytext, yyleng)->definition = FALSE;
	      goto skipline;
	    }
	  break;

	}
      if (!bl_in_dir_line)
	lerror (8);
    }
  else
    /* Hvis det er en blank eller tom linje er hele linja en kommentar */
    if (lexchar == ' ' | lexchar == '\t' | lexchar == '\n'
	| lexchar == '!' | lexchar == EOF)
    goto skipline;
  else
    {
      if (!bl_in_dir_line)
	lerror (8);
      goto skipline;
    }
  goto skipline;
pifdef:
  while (lexchar == ' ' | lexchar == '\t')
    newlexchar;
  if (lexchar == '\n' | lexchar == EOF)
    {
      if (!bl_in_dir_line)
	lerror (8);
      goto skipline;
    }
  yyleng = 0;
  while (lexchar != '\n' && lexchar != EOF
	 && lexchar != ' ' && lexchar != '\t')
    if (yyleng > SAFEYYTEXTLENGTH)
      {
	lerror (20);
	yyleng = 1;
      }
    else
      {
	yytext[yyleng++] = lexchar;
	newlexchar;
      }
  yytext[yyleng] = '\0';
  if (elsedef == TRUE)
    {
      if (!(ifdef[ifdefniv] & IFGREN))
	lerror (21);
      if (ifdef[ifdefniv] & (TRUE | SCAN))
	scan = SCAN;
      else
	scan = FALSE;
    }
  else
    {
      ifdefniv++;
      scan = FALSE;
    }
  ifdef[ifdefniv] = (systag (yytext, yyleng)->definition) | IFGREN | scan;
  goto scanifdef;
pelse:
  if (!(ifdef[ifdefniv] & IFGREN))
    lerror (22);
  ifdef[ifdefniv] &= TRUE;
  goto scanifdef;
pendif:
  if (ifdefniv == includeif[fillev])
    lerror (23);
  else
    ifdefniv--;
  goto scan;
scanifdef:
  if (notdef)
    {
      if ((ifdef[ifdefniv] & TRUE) == TRUE)
	ifdef[ifdefniv]--;
      else
	ifdef[ifdefniv]++;
    }
scan:
  if (ifdefniv > includeif[fillev] + 1)
    {
      if (!((ifdef[ifdefniv - 1] == (IFGREN | TRUE)) ||
	    (ifdef[ifdefniv - 1] == (ELSEGREN | FALSE))))
	ifdef[ifdefniv] |= SCAN;
    }
  if ((ifdef[ifdefniv] == (IFGREN | TRUE)) ||
      (ifdef[ifdefniv] == (ELSEGREN | FALSE)))
    goto skipline;
  while (TRUE)
    {
      while (lexchar != '\n' && lexchar != EOF)
	newlexchar;
      if (lexchar == EOF)
	goto skipline;		/* FEILMELDING VIL BLI SKREVET UT SENERE SE
				 * lerror(24) */
      yylineno++;
      if (!option_write_tokens)
	(void) putc (MNEWLINE, MELL);
      dirpos = 0;
      if (newlexchar == '%' && ((newlexchar == ' ' && option_bl_in_dir_line) ?
				(newlexchar, bl_in_dir_line = TRUE) :
		       ((bl_in_dir_line = FALSE), TRUE)) && isalpha (lexchar))
	{
	  while (isalnum (lexchar) || lexchar == '_')
	    {
	      dirtext[dirpos++] = lexchar;
	      if (dirpos == DIRTEXTLENGTH)
		goto pcont;
	      newlexchar;
	    }
	  dirtext[dirpos] = '\0';
	  if (!strcmp (dirtext, "ELSE"))
	    goto pelse;
	  if (!strcmp (dirtext, "ELSEDEF"))
	    {
	      elsedef = TRUE;
	      notdef = FALSE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "ELSENOTDEF"))
	    {
	      elsedef = TRUE;
	      notdef = TRUE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "ENDIF"))
	    goto pendif;
	  if (!strcmp (dirtext, "IFDEF"))
	    {
	      elsedef = FALSE;
	      notdef = FALSE;
	      goto pifdef;
	    }
	  if (!strcmp (dirtext, "IFNOTDEF"))
	    {
	      elsedef = FALSE;
	      notdef = TRUE;
	      goto pifdef;
	    }
	}
    pcont:;
    }


dirflags:
  while (lexchar == ' ' | lexchar == '\t')
    newlexchar;
  if (lexchar == '\n' | lexchar == EOF)
    {
      if (!bl_in_dir_line)
	lerror (8);
      goto skipline;
    }
  dirpos = 0;
  if (isalpha (lexchar))
    {
      while (isalnum (lexchar) || lexchar == '_')
	{
	  dirtext[dirpos++] = lexchar;
	  if (dirpos == DIRTEXTLENGTH)
	    {
	      if (!bl_in_dir_line)
		lerror (8);
	      goto skipline;
	    }
	  newlexchar;
	}
      dirtext[dirpos] = '\0';
      if (lexchar != '\n' && lexchar != EOF
	  && lexchar != ' ' && lexchar != '\t')
	{
	  if (!bl_in_dir_line)
	    lerror (8);
	  goto skipline;
	}
    }
  else
    {
      if (!bl_in_dir_line)
	lerror (8);
      goto skipline;
    }
  if (!strcmp (dirtext, "ON"))
    {
      if (flag != &flaggpass2)
	*flag = ON;
      else
	{
	  if (!option_write_tokens)
	    (void) putc (MFLAG, MELL);
	  if (!option_write_tokens)
	    (void) putc (mellflag + 1, MELL);
	}
    }
  else if (!strcmp (dirtext, "OFF"))
    {
      if (flag != &flaggpass2)
	*flag = OFF;
      else
	{
	  if (!option_write_tokens)
	    (void) putc (MFLAG, MELL);
	  if (!option_write_tokens)
	    (void) putc (mellflag, MELL);
	}
    }
  else if (!bl_in_dir_line)
    lerror (8);
  /* Resten av linja er kommentar */
  goto skipline;

skipline:
  while (lexchar != '\n' && lexchar != EOF)
    newlexchar;
  yylineno++;
  if (!option_write_tokens)
    (void) putc (MNEWLINE, MELL);
}


#if DAD_LIB

#define reswd (1L<<31)
#define signbit (1L<<31)
#define emask (2047L<<20)

/******************************************************************************
                                                                 ADDEPSILON  */

double 
__raddepsilon (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN
	  long i2;
	  unsigned long i1;
#else
	  unsigned long i1;
	  long i2;
#endif
	}
      dasii;
    }
  value;
  if (r == MAX_DOUBLE)
    return (r);
  value.d = r;
#if FLOAT_VAX
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.dasii.i2 & emask) == emask)
    return (r);
#endif
  if (r == -MIN_DOUBLE)
    return (-0.0);
  if (r == 0.0)
    return (MIN_DOUBLE);
#endif
  if (value.dasii.i2 & signbit)
    {
      if ((value.dasii.i1--) == 0)
	value.dasii.i2--;
    }
  else if ((++value.dasii.i1) == 0)
    value.dasii.i2++;
#if FLOAT_IBM
  if (value.dasii.i2 & emask && value.dasii.i2 & tmask)
    if (value.dasii.i2 & signbit)
      value.dasii.i2 -= adexp;
    else
      value.dasii.i2 += adexp;
#endif
  return (value.d);
}

/******************************************************************************
                                                                 SUBEPSILON  */

double 
__rsubepsilon (r)
     double r;
{
  union
    {
      double d;
      struct
	{
#if WORDS_BIGENDIAN
	  long i2;
	  unsigned long i1;
#else
	  unsigned long i1;
	  long i2;
#endif
	}
      dasii;
    }
  value;
  if (r == -MAX_DOUBLE)
    return (r);
  value.d = r;
#if FLOAT_VAX
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    return (r);
#else
#if FLOAT_IEEE
  if ((value.dasii.i2 & emask) == emask)
    return (r);
#endif
  if (r == 0.0)
    return (-MIN_DOUBLE);
  if (r == MIN_DOUBLE)
    return (0.0);
#endif
  if (value.dasii.i2 & signbit)
    {
      if ((++value.dasii.i1) == 0)
	value.dasii.i2++;
    }
  else if ((value.dasii.i1--) == 0)
    value.dasii.i2--;
#if FLOAT_IBM
  if (value.dasii.i2 & emask && value.dasii.i2 & tmask)
    if (value.dasii.i2 & signbit)
      value.dasii.i2 += adexp;
    else
      value.dasii.i2 -= adexp;
#endif
  return (value.d);
}

#include "dad.c"

/******************************************************************************
                                                                GET_C_DOUBLE */

double
get_c_double (text)
     char *text;
{
  int i = 0,
    j = 0,
    e = 0,
    p = 0;
  ascii a;
  double d;
  char ill = 0;
  a.sign = POS;
  while (text[i] == '0')
    i++;
  while (isdigit (text[i]))
    {
      if (j < 17)
	a.frac[j++] = text[i];
      i++;
      p++;
    }
  if (text[i] == '.')
    {
      i++;
      if (p == 0)
	{
	  while (text[i] == '0')
	    {
	      i++;
	      p--;
	    }
	}
    }
  while (isdigit (text[i]))
    {
      if (j < 17)
	a.frac[j++] = text[i];
      i++;
    }
  a.frac[j] = '\0';
  if (text[i] == 'e')
    sscanf (&text[i + 1], "%d", &e);
  a.exp = e + p - 1;
  d = __rextendedd_to_double (__rascii_to_extendedd (&a), &ill);
  if (ill)
    {
      lerror (14);
      return (0.0);
    }
  return (d);
}
#endif



/******************************************************************************
                                                                   YYLEX     */

/* Returnerer et token til parser. */

char forstegang = TRUE;
int 
yylex ()
{
  if (forstegang)
    {
      forstegang = FALSE;
      yylineno = 1L;
      filstack[0] = PROG;
      while (newlexchar == '%')
	dirline ();
      unput (lexchar);
    }
newtoken:
  yylineno += antnewline;
  for (; antnewline; antnewline--)
    if (!option_write_tokens)
      (void) putc (MNEWLINE, MELL);
  if (newsymbole)
    {
      int symbol;
      symbol = newsymbole;
      newsymbole = NOSYMBOL;
      if (symbol == HEND)
	goto end;
      return (symbol);
    }
  if (isalpha (newlexchar))
    {
      yyleng = 0;
      while (isalnum (lexchar) || lexchar == '_')
	if (yyleng > SAFEYYTEXTLENGTH)
	  {
	    lerror (20);
	    yyleng = 1;
	  }
	else
	  {
	    yytext[yyleng++] = lexchar;
	    newlexchar;
	  }
      unput (lexchar);
      yytext[yyleng] = '\0';
      switch (yytext[0])
	{
	case 'A':
	  if (!strcmp (yytext, "ACTIVATE"))
	    return (HACTIVATE);
	  if (!strcmp (yytext, "AFTER"))
	    return (HAFTER);
	  if (!strcmp (yytext, "AND"))
	    return (HAND);
	  if (!strcmp (yytext, "ARRAY"))
	    return (HARRAY);
	  if (!strcmp (yytext, "AT"))
	    return (HAT);
	  break;
	case 'B':
	  if (!strcmp (yytext, "BEFORE"))
	    return (HBEFORE);
	  if (!strcmp (yytext, "BEGIN"))
	    return (HBEGIN);
	  if (!strcmp (yytext, "BOOLEAN"))
	    return (HBOOLEAN);
	  break;
	case 'C':
	  if (!strcmp (yytext, "CHARACTER"))
	    return (HCHARACTER);
	  if (!strcmp (yytext, "CLASS"))
	    return (HCLASS);
	  if (!strcmp (yytext, "COMMENT"))
	    goto comment;
	  break;
	case 'D':
	  if (!strcmp (yytext, "DELAY"))
	    return (HDELAY);
	  if (!strcmp (yytext, "DO"))
	    return (HDO);
	  break;
	case 'E':
	  if (!strcmp (yytext, "ELSE"))
	    return (HELSE);
	  if (!strcmp (yytext, "END"))
	    goto end;
	  if (!strcmp (yytext, "EQ"))
	    {
	      yylval.token = HEQ;
	      return (HVALRELOPERATOR);
	    }
	  if (!strcmp (yytext, "EQV"))
	    return (HEQV);
	  if (!strcmp (yytext, "EXTERNAL"))
	    return (HEXTERNAL);
	  break;
	case 'F':
	  if (!strcmp (yytext, "FALSE"))
	    {
	      yylval.ival = FALSE;
	      return (HBOOLEANKONST);
	    }
	  if (!strcmp (yytext, "FOR"))
	    return (HFOR);
	  break;
	case 'G':
	  if (!strcmp (yytext, "GE"))
	    {
	      yylval.token = HGE;
	      return (HVALRELOPERATOR);
	    }
	  if (!strcmp (yytext, "GO"))
	    return (HGO);
	  if (!strcmp (yytext, "GOTO"))
	    return (HGOTO);
	  if (!strcmp (yytext, "GT"))
	    {
	      yylval.token = HGT;
	      return (HVALRELOPERATOR);
	    }
	  break;
	case 'H':
	  if (!strcmp (yytext, "HIDDEN"))
	    return (HHIDDEN);
	  break;
	case 'I':
	  if (!strcmp (yytext, "IF"))
	    return (HIF);
	  if (!strcmp (yytext, "IMP"))
	    return (HIMP);
	  if (!strcmp (yytext, "IN"))
	    {
	      yylval.token = HIN;
	      return (HOBJRELOPERATOR);
	    }
	  if (!strcmp (yytext, "INNER"))
	    return (HINNER);
	  if (!strcmp (yytext, "INSPECT"))
	    return (HINSPECT);
	  if (!strcmp (yytext, "INTEGER"))
	    return (HINTEGER);
	  if (!strcmp (yytext, "IS"))
	    {
	      yylval.token = HIS;
	      return (HOBJRELOPERATOR);
	    }
	  break;
	case 'L':
	  if (!strcmp (yytext, "LABEL"))
	    return (HLABEL);
	  if (!strcmp (yytext, "LE"))
	    {
	      yylval.token = HLE;
	      return (HVALRELOPERATOR);
	    }
	  if (!strcmp (yytext, "LONG"))
	    return (HLONG);
	  if (!strcmp (yytext, "LT"))
	    {
	      yylval.token = HLT;
	      return (HVALRELOPERATOR);
	    }
	  break;
	case 'N':
	  if (!strcmp (yytext, "NAME"))
	    return (HNAME);
	  if (!strcmp (yytext, "NE"))
	    {
	      yylval.token = HNE;
	      return (HVALRELOPERATOR);
	    }
	  if (!strcmp (yytext, "NEW"))
	    return (HNEW);
	  if (!strcmp (yytext, "NONE"))
	    return (HNONE);
	  if (!strcmp (yytext, "NOT"))
	    return (HNOT);
	  if (!strcmp (yytext, "NOTEXT"))
	    {
	      yylval.tval = NOTEXT;
	      return (HTEXTKONST);
	    }
	  break;
	case 'O':
	  if (!strcmp (yytext, "OR"))
	    return (HOR);
	  if (!strcmp (yytext, "OTHERWISE"))
	    return (HOTHERWISE);
	  break;
	case 'P':
	  if (!strcmp (yytext, "PRIOR"))
	    return (HPRIOR);
	  if (!strcmp (yytext, "PROCEDURE"))
	    return (HPROCEDURE);
	  if (!strcmp (yytext, "PROTECTED"))
	    return (HPROTECTED);
	  break;
	case 'Q':
	  if (!strcmp (yytext, "QUA"))
	    return (HQUA);
	  break;
	case 'R':
	  if (!strcmp (yytext, "REACTIVATE"))
	    return (HREACTIVATE);
	  if (!strcmp (yytext, "REAL"))
	    {
#if FLOAT_IMPLEMENTED
#else
	      lerror (31);
#endif
	      return (HREAL);
	    }
	  if (!strcmp (yytext, "REF"))
	    return (HREF);
	  break;
	case 'S':
	  if (!strcmp (yytext, "SHORT"))
	    return (HSHORT);
	  if (!strcmp (yytext, "STEP"))
	    return (HSTEP);
	  if (!strcmp (yytext, "SWITCH"))
	    return (HSWITCH);
	  break;
	case 'T':
	  if (!strcmp (yytext, "TEXT"))
	    return (HTEXT);
	  if (!strcmp (yytext, "THEN"))
	    return (HTHEN);
	  if (!strcmp (yytext, "THIS"))
	    return (HTHIS);
	  if (!strcmp (yytext, "TO"))
	    return (HTO);
	  if (!strcmp (yytext, "TRUE"))
	    {
	      yylval.ival = TRUE;
	      return (HBOOLEANKONST);
	    }
	  break;
	case 'U':
	  if (!strcmp (yytext, "UNTIL"))
	    return (HUNTIL);
	  break;
	case 'V':
	  if (!strcmp (yytext, "VALUE"))
	    return (HVALUE);
	  if (!strcmp (yytext, "VIRTUAL"))
	    return (HVIRTUAL);
	  break;
	case 'W':
	  if (!strcmp (yytext, "WHEN"))
	    return (HWHEN);
	  if (!strcmp (yytext, "WHILE"))
	    return (HWHILE);
	  break;
	}
      /* IDENTIFIKATORER    BLIR */
      /* LAGT INN I NAVNELAGERET */
      yylval.ident = tag (yytext, yyleng);
      return (HIDENTIFIER);
    }
  switch (lexchar)
    {
    case '=':
      if (newlexchar == '=')
	{
	  yylval.token = HEQR;
	  return (HREFRELOPERATOR);
	}
      if (lexchar == '/')
	if (newlexchar == '=')
	  {
	    yylval.token = HNER;
	    return (HREFRELOPERATOR);
	  }
	else
	  lerror (7);
      unput (lexchar);
      yylval.token = HEQ;
      return (HVALRELOPERATOR);
    case '>':
      if (newlexchar == '=')
	{
	  yylval.token = HGE;
	  return (HVALRELOPERATOR);
	}
      unput (lexchar);
      yylval.token = HGT;
      return (HVALRELOPERATOR);
    case '<':
      if (newlexchar == '=')
	{
	  yylval.token = HLE;
	  return (HVALRELOPERATOR);
	}
      if (lexchar == '>')
	{
	  yylval.token = HNE;
	  return (HVALRELOPERATOR);
	}
      unput (lexchar);
      yylval.token = HLT;
      return (HVALRELOPERATOR);
    case '+':
      yylval.token = HADD;
      return (HTERMOPERATOR);
    case '-':
      yylval.token = HSUB;
      return (HTERMOPERATOR);
    case '*':
      if (newlexchar == '*')
	{
	  yylval.token = HEXP;
	  return (HPRIMARYOPERATOR);
	}
      unput (lexchar);
      yylval.token = HMUL;
      return (HFACTOROPERATOR);
    case '/':
      if (newlexchar == '/')
	{
	  yylval.token = HINTDIV;
	  return (HFACTOROPERATOR);
	}
      unput (lexchar);
      yylval.token = HDIV;
#if FLOAT_IMPLEMENTED
#else
      lerror (31);
#endif
      return (HFACTOROPERATOR);
    case '.':
      if (isdigit (newlexchar))
	goto dotdigit;
      if (lexchar == '.')
	if (newlexchar == '.')
	  return (HDOTDOTDOT);
	else
	  lerror (7);
      unput (lexchar);
      return (HDOT);
    case ',':
      return (HPAREXPSEPARATOR);
    case ':':
      if (newlexchar == '=')
	{
	  yylval.token = HASSIGNVALUE;
	  return (HASSIGN);
	}
      if (lexchar == '-' && pardeep == 0)
	{
	  yylval.token = HASSIGNREF;
	  return (HASSIGN);
	}
      unput (lexchar);
      return (HLABELSEPARATOR);
    case ';':
      pardeep = 0;
      return (HSTATEMENTSEPARATOR);
    case '(':
      pardeep++;
      return (HBEGPAR);
    case ')':
      pardeep--;
      return (HENDPAR);
    case '&':
      if (newlexchar == '&' || lexchar == '-' || lexchar == '+'
	  || isdigit (lexchar))
	{
	  yyleng = 0;
	  lexradix = 10;
	  yytext[yyleng++] = '1';
	  goto digitsexp;
	}
      unput (lexchar);
      return (HCONC);
    case '!':
      goto comment;
    case '\'':
      goto character;
    case '\"':
      goto text;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      goto digit;
    case EOF:
      if (ifdefniv != includeif[fillev])
	{
	  lerror (24);
	  ifdefniv = includeif[fillev];
	}
      if (fillev == 0)
	return (NOSYMBOL);
      systag ("INCLUDED", 8)->definition = FALSE;
      popfilmap ();
      (void) fclose (filstack[fillev--]);	/* Ingen break her */
    case '\n':			/* NL (LF) */
      yylineno++;
      if (!option_write_tokens)
	(void) putc (MNEWLINE, MELL);
      while (newlexchar == '%')
	dirline ();
      unput (lexchar);
      goto newtoken;
    case ' ':
    case '\b':			/* BS */
    case '\t':			/* HT */
    case '\v':			/* VT */
    case '\f':			/* FF */
    case '\r':			/* CR */
      goto newtoken;
    }
  /* HVIS IKKE NOEN AV DE ANDRE REGLENE */
  /* SL]R TIL S] ER DET ET ULOVLIG TEGN */
  lerror (7);
  goto newtoken;

comment:
  {				/* Behandling av komentarer */
    while ((newlexchar != ';') && lexchar != EOF)
      if (lexchar == '\n')
	{
	  antnewline++;
	  while (newlexchar == '%')
	    dirline ();
	  unput (lexchar);
	}
    if (lexchar != ';')
      {
	lerror (1);
	unput (lexchar);
      }
    goto newtoken;
  }

end:
  {				/* Behandling av end-komentarer * Her må jeg
				 * bruke unput */
    int reported=0;
    newlexchar;
    while (TRUE)
      {
	while (!isalpha (lexchar))
	  if (lexchar == '\n')
	    {
	      antnewline++;
	      while (newlexchar == '%')
		dirline ();
	      notintext = TRUE;
	    }
	  else if (lexchar == EOF)
	    {
	      unput (lexchar);
	      return (HEND);
	    }
	  else if (lexchar == ';')
	    {
	      unput (lexchar);
	      return (HEND);
	    }
	  else
	    newlexchar;
	if (lexchar == 'E')
	  {
	    if (newlexchar == 'N')
	      {
		if (newlexchar == 'D' && !isalnum (newlexchar) && lexchar != '_')
		  {		/* END is found and comment is terminated */
		    unput (lexchar);
		    newsymbole = HEND;
		    return (HEND);
		  } else 
		    if (antnewline && !reported) {lerror(32); reported = 1;}
	      }
	    else if (lexchar == 'L' && newlexchar == 'S' && newlexchar == 'E'
		     && !isalnum (newlexchar) && lexchar != '_')
	      {			/* ELSE is found and comment is terminated */
		unput (lexchar);
		newsymbole = HELSE;
		return (HEND);
	      } else if (antnewline && !reported) {lerror(32); reported = 1;}
	  }
	else if (lexchar == 'W')
	  {
	    if (newlexchar == 'H' && newlexchar == 'E' && newlexchar == 'N'
		&& !isalnum (newlexchar) && lexchar != '_')
	      {			/* WHEN is found and comment is terminated */
		unput (lexchar);
		newsymbole = HWHEN;
		return (HEND);
	      } else if (antnewline && !reported) {lerror(32); reported = 1;}
	  }
	else if (lexchar == 'O' && newlexchar == 'T' && newlexchar == 'H'
	      && newlexchar == 'E' && newlexchar == 'R' && newlexchar == 'W'
	      && newlexchar == 'I' && newlexchar == 'S' && newlexchar == 'E'
		 && !isalnum (newlexchar) && lexchar != '_')
	  {			/* OTHERWISE is found and comment is
				 * terminated */
	    unput (lexchar);
	    newsymbole = HOTHERWISE;
	    return (HEND);
	  } else if (antnewline && !reported) {lerror(32); reported = 1;}
	while (isalpha (lexchar) || lexchar == '_')
	  newlexchar;
      }
  }

character:
  {				/* Behandling av karakter konstanter */
    notintext = FALSE;
    if ((isprint (newlexchar)
#if ISO_LATIN
	 || lexchar >= 160
#endif
	) && lexchar != '!')
      {
	yylval.ival = lexchar;
	newlexchar;
      }
    else if (lexchar == '!')
      {
	if (isdigit (newlexchar))
	  {
	    firstchar = lexchar;
	    if (isdigit (newlexchar))
	      {
		secondchar = lexchar;
		if (isdigit (newlexchar))
		  {
		    thirdchar = lexchar;
		    if (newlexchar == '!')
		      {
			if (firstchar < '2'
			    || (firstchar == '2' && (secondchar < '5'
						     || (secondchar == '5'
						      && thirdchar < '6'))))
			  {
			    yylval.ival = ((firstchar - '0') * 10
					   + secondchar - '0') * 10
			      + thirdchar - '0';
			    newlexchar;
			  }
			else
			  {
			    lerror (2);
			    newlexchar;
			  }
		      }
		    else
		      {
			lerror (2);
		      }
		  }
		else if (lexchar == '!')
		  {
		    yylval.ival = (firstchar - '0') * 10
		      + secondchar - '0';
		    newlexchar;
		  }
		else
		  {
		    lerror (2);
		  }
	      }
	    else if (lexchar == '!')
	      {
		yylval.ival = firstchar - '0';
		newlexchar;
	      }
	    else
	      {
		lerror (2);
	      }
	  }
	else
	  {
	    yylval.ival = '!';
	  }
      }
    else
      lerror (2);
    if (lexchar != '\'')
      {
	unput (lexchar);
	lerror (3);
      }
    notintext = TRUE;
    return (HCHARACTERKONST);
  }

text:
  {				/* Behandling av text-konstanter * Her må
				 * også unput brukes */
    notintext = FALSE;
    newlexchar;
    while (TRUE)
      {
	while ((isgraph (lexchar)
#if ISO_LATIN
		|| lexchar >= 160
#endif
	       ) && lexchar != '!' && lexchar != '"')
	  {
	    putchartext ((unsigned char) lexchar);
	    newlexchar;
	  }
	if (lexchar == ' ')
	  {
	    putchartext ((unsigned char) lexchar);
	    newlexchar;
	  }
	else if (lexchar == '!')
	  {
	    if (isdigit (newlexchar))
	      {
		firstchar = lexchar;
		if (isdigit (newlexchar))
		  {
		    secondchar = lexchar;
		    if (isdigit (newlexchar))
		      {
			thirdchar = lexchar;
			if (newlexchar == '!')
			  {
			    if (firstchar < '2'
				|| (firstchar == '2' && (secondchar < '5'
						       || (secondchar == '5'
						      && thirdchar < '6'))))
			      {
				putchartext 
				  ((unsigned char) (((firstchar - '0') * 10
						     + secondchar - '0') * 10
						    + thirdchar - '0'));
				newlexchar;
			      }
			    else
			      {
				putchartext ('!');
				putchartext (firstchar);
				putchartext (secondchar);
				putchartext (thirdchar);
			      }
			  }
			else
			  {
			    putchartext ('!');
			    putchartext (firstchar);
			    putchartext (secondchar);
			    putchartext (thirdchar);
			  }
		      }
		    else if (lexchar == '!')
		      {
			putchartext ((unsigned char) ((firstchar - '0') * 10
						      + secondchar - '0'));
			newlexchar;
		      }
		    else
		      {
			putchartext ('!');
			putchartext (firstchar);
			putchartext (secondchar);
		      }
		  }
		else if (lexchar == '!')
		  {
		    putchartext (firstchar - '0');
		    newlexchar;
		  }
		else
		  {
		    putchartext ('!');
		    putchartext (firstchar);
		  }
	      }
	    else
	      {
		putchartext ('!');
	      }
	  }
	else if (lexchar == '\"')
	  {
	    if (newlexchar == '\"')
	      {
		putchartext ((unsigned char) lexchar);
		newlexchar;
	      }
	    else
	      {
		while (isspace (lexchar) || lexchar == '\b')
		  {
		    if (lexchar == '\n')
		      antnewline++;
		    newlexchar;
		  }
		if (lexchar == '\"')
		  newlexchar;
		else
		  {
		    unput (lexchar);
		    yylval.tval = gettext ();
		    notintext = TRUE;
		    return (HTEXTKONST);
		  }
	      }
	  }
	else if (lexchar == '\n')
	  {
	    antnewline++;
	    lerror (4);
	    yylval.tval = gettext ();
	    notintext = TRUE;
	    return (HTEXTKONST);
	  }
	else
	  {
	    unput (lexchar);
	    lerror (5);
	    yylval.tval = gettext ();
	    notintext = TRUE;
	    return (HTEXTKONST);
	  }
      }
  }

dotdigit:
  /* Behandling av tall som starter med tegnet '.' */
  yyleng = 0;
  lexradix = 10;
digitsdot:
  yytext[yyleng++] = '.';
  if (lexchar >= '0' & lexchar <= '9')
    yytext[yyleng++] = lexchar;
  while (newlexchar >= '0' && lexchar <= '9' || lexchar == '_')
    if (yyleng > SAFEYYTEXTLENGTH)
      {
	lerror (20);
	yyleng = 1;
      }
    else if (lexchar != '_')
      yytext[yyleng++] = lexchar;
  if (lexchar == '&')
    {
      newlexchar;
    digitsexp:
      if (lexchar == '&')
	newlexchar;
      yytext[yyleng++] = 'e';
      if (lexchar == '-')
	{
	  yytext[yyleng++] = '-';
	  newlexchar;
	}
      else if (lexchar == '+')
	newlexchar;
      if (lexchar >= '0' & lexchar <= '9')
	yytext[yyleng++] = lexchar;
      while (newlexchar >= '0' && lexchar <= '9' || lexchar == '_')
	if (yyleng > SAFEYYTEXTLENGTH)
	  {
	    lerror (20);
	    yyleng = 1;
	  }
	else if (lexchar != '_')
	  yytext[yyleng++] = lexchar;
    }
  unput (lexchar);
  yytext[yyleng++] = '\0';
#if DAD_LIB
  yylval.rval = get_c_double (yytext);
#else
  (void) sscanf ((char *) yytext, "%lf", &yylval.rval);
#endif
  if (lexradix != 10)
    lerror (16);
#if FLOAT_IMPLEMENTED
#else
  lerror (31);
#endif
  return (HREALKONST);

digit:
  {				/* Behandling av tall som starter med et
				 * siffer */
    lexradix = 10;
    yyleng = 0;
    yytext[yyleng++] = lexchar;
    if (newlexchar == 'R' && (yytext[0] == '2' | yytext[0] == '4' 
			      | yytext[0] == '8'))
      {
	lexradix = yytext[0] - '0';
	yyleng = 0;
      }
    else if (yytext[0] == '1' && lexchar == '6')
      {
	yytext[yyleng++] = lexchar;
	if (newlexchar == 'R')
	  {
	    lexradix = 16;
	    yyleng = 0;
	  }
	else
	  unput (lexchar);
      }
    else
      unput (lexchar);
    while ((lexradix == 16 ? isxdigit (newlexchar) : isdigit (newlexchar))
	   || lexchar == '_')
      if (yyleng > SAFEYYTEXTLENGTH)
	{
	  lerror (20);
	  yyleng = 1;
	}
      else if (lexchar == '_');
      else if (isdigit (lexchar))
	yytext[yyleng++] = lexchar;
      else
	yytext[yyleng++] = lexchar + ('9' + 1 - 'A');
    if (lexchar == '.' && lexradix == 10)
      {
	newlexchar;
	goto digitsdot;
      }
    if (lexchar == '&' && lexradix == 10)
      {
	newlexchar;
	goto digitsexp;
      }
    yytext[yyleng++] = '\0';
    unput (lexchar);
    yylval.ival = radix (lexradix, yytext);
    return (HINTEGERKONST);
  }
}

/******************************************************************************
                                                                RADIX        */

/* Rutine for } lage et heltall gitt et tall som er kodet tekstlig. */

long 
radix (r, t)
     int r;
     char *t;
{
  long maxint_div_r = MAX_INT / r;

  long dig = 0,
    siffer;
  while (*t)
    {
      siffer = *t - '0';
      if (siffer >= r)
	{
	  lerror (12);
	  return (0);
	}
      if (dig < maxint_div_r)
	dig = dig * r + siffer;
      else if (dig == maxint_div_r)
	{
	  if (siffer <= MAX_INT % r)
	    dig = dig * r + siffer;
	  else
	    {
	      lerror (9);
	      return (0);
	    }
	}
      else
	{
	  lerror (9);
	  return (0);
	}
      t++;
    }
  return (dig);
}

/******************************************************************************
  						          PUTCHARACTER       */

/* Hjelpe-prosedyre for } bygge opp et konstant-tektsobjekt. */

#define LERROR(t) (leerror?leerror:(leerror=TRUE,lerror(t)))

char *
putcharacter (character)
     unsigned char character;
{
  static char s[10];
  if (external == TRUE)
    (void) sprintf (s, "%c", character);
  else if (isgraph (character) && character != '\\' && character != '"')
    (void) sprintf (s, "%c", character);
  else
    (void) sprintf (s, "\\%03o", character);
  return (s);
}

/******************************************************************************
  							      PUTCHARTEXT    */

/* Prosedyre som fungerer som grensesnitt mot scanner,
 *  for } bygge opp et konstant-tekstobjekt.
 *  Denne rutinen kalles for hvert tegn som skal inn i tekst-objektet. */

putchartext (character)
     unsigned char character;
{
  char *s;
  int l;
  antsimchar++;
  s = putcharacter (character);
#if SHORT_RECORD
  if ((antsimchar & 32) == 0)
    strcat (s, "\\\n");
#endif
  if (antchar + (l = strlen (s)) >= str_length)
    {
      char *s;
      int i;
      s = str;
      str = (char *) salloc (str_length = str_length * 2 + 100);
      for (i = 0; i < antchar; i++)
	str[i] = s[i];
    }
  for (i = 0; i < l; i++)
    str[antchar++] = s[i];
}

/******************************************************************************
                                   				GETTEXT      */

/* Denne rutinen bygger opp et internt konstant-tekstobjekt og returnerer
 * en peker til det. Teksten er bygget opp p} en slik m}te at den 
 * kun inneholder skrivbare tegn eksklusiv '\' ' ' og '"'.
 * Ikke skrivbare tegn og de tre som er nevnt ovenfor er kodet oktalt
 * i teksten (\nnn). Denne teksten kan uten videre skrives ut i C og trenger
 * ingen omkoding. For } sammenligne en tekst mot en annen tekst ved
 * kompilering, m} det brukes spesial-versjoner av strcmp og strlen.
 * Disser er sstrcmp() og sstrlen() (sjekker.c).
 */

char *
gettext ()
{
  char *s;
  if (antsimchar == MAX_TEXT_CHAR)
    lerror (44);
  if (str == NULL)
    str = (char *) salloc (str_length = str_length * 2 + 100);
  str[antchar++] = '\0';
  s = (char *) salloc (antchar);
  (void) strcpy (s, str);
  antchar = 0;
  leerror = FALSE;
  return (s);
}
