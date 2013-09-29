/* $Id: clex.c,v 1.15 1997/02/14 16:33:00 cim Exp $ */

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

/* Scanner for Simula */

#include <stdio.h>

#include <ctype.h>
#include "gen.h"
#include "name.h"
#include "filelist.h"
#include "error.h"
#include "extspec.h"
#include "mapline.h"

#ifdef STDC_HEADERS
#include <stdlib.h>
#else
double strtod ();
#endif

#include <obstack.h>
char *xmalloc();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_lex, os_ifdef;
static char *first_object_allocated_ptr_lex;
static char *first_object_allocated_ptr_ifdef;

#define unput(c) {ungetc (c, include_file ());}
static int notintext = TRUE;

#define  newlexchar (lexchar=input())
static int newsymbole;
long lineno;
static int antnewline;
static int lexradix;
static int i;
static int lexchar;
static unsigned char firstchar;
static unsigned char secondchar;
static unsigned char thirdchar;
static char *yytext;

static int pardeep = 0;
static int antsimchar = 0;
static int antchar = 0;
static char leerror = FALSE;

static char end_of_file;

char external = FALSE;	/* Har man sett "EXTERNAL PROC/CLASS =" angir 
				 * external at man venter et filnavn 
				 * som ikke skal behandles som
				 * en text-konstant. */

typedef struct _ifdefstack ifdefstack_t;
struct _ifdefstack
{
  char ifdef;
  ifdefstack_t *prev;
}  *ifdefp;

char *flag, mellflag, bl_in_dir_line;

/* Brytere som kan slåes AV/P} ved hjelp av kompilator-direktiver */
char nameasvar = OFF;
char sensitive = OFF;
static char flaggpass2;
char staticblock = OFF;


/******************************************************************************
                                                                       INPUT */

/* Egen definisjon  på input for å  gjøre små  bokstaver om  til store
 * Samt fjerne ^@ fra input
 * Dette skal dog ikke gjøres innenfor text eller karakter -konstanter */

static int input (void)
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
	yytchar = getc (include_file ());
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

void print_lexsymbol (int lextok, YYSTYPE *yylvalp)
{
  int yyleng;
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
    case HVAR:
      printf ("VAR ");
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
      printf ("%ld ", yylvalp->ival);
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
	printf ("'!%ld!'", yylvalp->ival);
      else
	printf ("'%c' ", (char) yylvalp->ival);
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
                                                                RADIX        */

/* Rutine for } lage et heltall gitt et tall som er kodet tekstlig. */

static long radix (int r, char *t)
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
				                    SCANNAME & SCANNOWS      */

static void scan_nows (void)
{
  obstack_free (&os_lex, yytext);
  while (lexchar != '\n' && lexchar != EOF 
	 && lexchar != ' ' && lexchar != '\t')
    {
      obstack_1grow (&os_lex, lexchar);
      newlexchar;
    }
  obstack_1grow (&os_lex, 0);
  yytext= obstack_finish (&os_lex);
}

static scan_name (void)
{
  obstack_free (&os_lex, yytext);
  while ((isalnum (lexchar) || lexchar == '_'))
    {
      obstack_1grow (&os_lex, lexchar);
      newlexchar;
    }
  obstack_1grow (&os_lex, 0);
  yytext= obstack_finish (&os_lex);
}


/******************************************************************************
				                              SCANIFDEF      */

static void scan_ifdef (void)
{
  char scan;
  char elsedef;
  char notdef;

  while (lexchar != EOF)
    {
      if (!strcmp (yytext, "ENDIF"))
	{
	  if (ifdefp == include_ifdefp ()) lerror (23);
	  else 
	    {
	      ifdefstack_t *prev= ifdefp->prev;
	      obstack_free (&os_ifdef, ifdefp);
	      ifdefp= prev;
	    }
	}
      else
	{
	  if (!strcmp (yytext, "ELSE"))
	    {
	      if (!(ifdefp->ifdef & IFGREN))
		lerror (22);
	      notdef = FALSE;
	      ifdefp->ifdef &= TRUE;
	    }
	  else
	    {
	      if (!strcmp (yytext, "ELSEDEF"))
		{
		  elsedef = TRUE;
		  notdef = FALSE;
		}
	      else if (!strcmp (yytext, "ELSENOTDEF"))
		{
		  elsedef = TRUE;
		  notdef = TRUE;
		}
	      else if (!strcmp (yytext, "IFDEF"))
		{
		  elsedef = FALSE;
		  notdef = FALSE;
		}
	      else if (!strcmp (yytext, "IFNOTDEF"))
		{
		  elsedef = FALSE;
		  notdef = TRUE;
		} 
	      else goto proceed;

	      while (lexchar == ' ' | lexchar == '\t')
		newlexchar;
	      if (isalpha (lexchar) || lexchar == '_')
		{
		  scan_name ();
	      
		  if (elsedef == TRUE)
		    {
		      if (!(ifdefp->ifdef & IFGREN))
			lerror (21);
		      if (ifdefp->ifdef & (TRUE | SCAN))
			scan = SCAN;
		      else
			scan = FALSE;
		    }
		  else
		    {
		      ifdefstack_t *prev= ifdefp;
		      ifdefp= (ifdefstack_t *)
			obstack_alloc (&os_ifdef, sizeof (ifdefstack_t));
		      ifdefp->prev= prev;
		      ifdefp->ifdef= 0;
		      scan = FALSE;
		    }
		  ifdefp->ifdef = ifdef_name (tag (yytext)) | IFGREN | scan;
		}
	      else 
		{
		  if (!bl_in_dir_line) lerror (8);
		  goto proceed;
		}
	    }
	  if (notdef)
	    {
	      if ((ifdefp->ifdef & TRUE) == TRUE)
		ifdefp->ifdef--;
	      else
		ifdefp->ifdef++;
	    }
	}
      if ((ifdefp->prev != include_ifdefp ()) && (ifdefp != include_ifdefp ()))
	{
	  if (!((ifdefp->prev->ifdef == (IFGREN | TRUE)) ||
		(ifdefp->prev->ifdef == (ELSEGREN | FALSE))))
	    ifdefp->ifdef |= SCAN;
	}
      if ((ifdefp->ifdef == (IFGREN | TRUE)) ||
	  (ifdefp->ifdef == (ELSEGREN | FALSE)))
	break;
      
    proceed:
      while (lexchar != EOF)
	{
	  while (lexchar != '\n' && lexchar != EOF) newlexchar;
	  lineno++;
	  if (!option_write_tokens)
	    mout (MNEWLINE);
	  if (newlexchar == '%' && 
	      ((newlexchar == ' ' && option_bl_in_dir_line) ?
	       (newlexchar, bl_in_dir_line = TRUE) :
	       ((bl_in_dir_line = FALSE), TRUE)) && isalpha (lexchar))
	    {
	      scan_name ();
	      break;
	    }
	}
    }
}

/******************************************************************************
				                              SCANDIRFLAGS   */

static void scan_dirflags (void)
{
  while (lexchar == ' ' | lexchar == '\t')
    newlexchar;
  if (isalpha (lexchar))
    {
      scan_name ();

      if (lexchar == '\n' | lexchar == EOF
	  | lexchar == ' ' | lexchar == '\t')
	{
	  if (!strcmp (yytext, "ON"))
	    {
	      if (flag != &flaggpass2)
		*flag = ON;
	      else
		{
		  if (!option_write_tokens)
		    {
		      mout (MFLAG);
		      mout (mellflag + 1);
		    }
		}
	    }
	  else if (!strcmp (yytext, "OFF"))
	    {
	      if (flag != &flaggpass2)
		*flag = OFF;
	      else
		{
		  if (!option_write_tokens)
		    {
		      mout (MFLAG);
		      mout (mellflag);
		    }
		}
	    }
	  else if (!bl_in_dir_line) lerror (8);
	}
      else if (!bl_in_dir_line) lerror (8);
    }
  else if (!bl_in_dir_line) lerror (8);
}

/******************************************************************************
                                                                   DIRLINE   */

static void scan_dirline (void)
{
  FILE *file;
  lineno += antnewline;
  for (; antnewline; antnewline--)
    if (!option_write_tokens)
      mout(MNEWLINE);
  if (newlexchar == ' ' && option_bl_in_dir_line)
    {
      bl_in_dir_line = TRUE;
      newlexchar;
    }
  else
    bl_in_dir_line = FALSE;
  if (isalpha (lexchar))
    {
      scan_name ();
      if (lexchar == '\n' | lexchar == EOF
	  | lexchar == ' ' | lexchar == '\t')
	{
	  switch (yytext[0])
	    {
	    case 'C':
	      if (!strcmp (yytext, "CASESENSITIVE"))
		{
		  flag = &sensitive;
		  scan_dirflags ();
		}
	      else if (!strcmp (yytext, "COMMENT"))
		{
		  int comlev = 1;
		  while (comlev && lexchar !=EOF)
		    {
		      while (lexchar != '\n' && lexchar != EOF)
			newlexchar;
		      if (lexchar == EOF) lerror (19);
		      
		      lineno++;
		      if (!option_write_tokens) mout (MNEWLINE);
		      if (newlexchar == '%' && 
			  ((newlexchar == ' ' &&
			    option_bl_in_dir_line) ? newlexchar : 0, TRUE) 
			  && isalpha (lexchar))
			{
			  scan_name ();
			  
			  if (!strcmp (yytext, "COMMENT"))
			    comlev++;
			  if (!strcmp (yytext, "ENDCOMMENT"))
			    comlev--;
			}
		    }
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'D':
	      if (!strcmp (yytext, "DEFINE"))
		{
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (isalpha (lexchar) | lexchar == '_')
		    {
		      scan_name ();
		      
		      define_name (tag (yytext), TRUE);
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'E':
	      if (!strcmp (yytext, "EOF"))
		{
		  end_of_file = 2;
		  return;
		}
	      else if (!strcmp (yytext, "ELSE")) scan_ifdef ();
	      else if (!strcmp (yytext, "ELSEDEF")) scan_ifdef ();
	      else if (!strcmp (yytext, "ELSENOTDEF")) scan_ifdef ();
	      else if (!strcmp (yytext, "ENDIF")) scan_ifdef ();
	      else if (!strcmp (yytext, "ENDCOMMENT")) lerror (15);
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'I':
	      if (!strcmp (yytext, "IFDEF")) scan_ifdef ();
	      else if (!strcmp (yytext, "IFNOTDEF")) scan_ifdef ();
	      else if (!strcmp (yytext, "INCLUDE") ||
		       !strcmp (yytext, "INSERT"))
		{
		  notintext = FALSE;
		  define_name (tag ("INCLUDED"), TRUE);
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (lexchar != '\n' | lexchar != EOF)
		    {
		      scan_nows ();
		      
		      notintext = TRUE;

		      pushfilmap (tag (yytext), ifdefp);
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!strcmp (yytext, "INDEXTEST"))
		{
		  flag = &flaggpass2;
		  mellflag = MINDEXTEST;
		  scan_dirflags ();
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'L':
	      if (!strcmp (yytext, "LIST")) ;
	      else if (!strcmp (yytext, "LINE"))
		{
		  long nylinje;
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (isdigit (lexchar))
		    {
		      obstack_free (&os_lex, yytext);
		      while (isalnum (lexchar) || lexchar == '_')
			{
			  if (lexchar != '_') obstack_1grow (&os_lex, lexchar);
			  newlexchar;
			}
		      obstack_1grow (&os_lex, 0);
		      yytext= obstack_finish (&os_lex);
		  
		      nylinje = radix (10, yytext);
		      notintext = FALSE;
		      while (lexchar == ' ' | lexchar == '\t')
			newlexchar;
		      if (lexchar != '\n' & lexchar != EOF)
			{
			  scan_nows ();
		      
			  setfilmap (tag (yytext), nylinje);
			}
		      else
			setfilmap ((char *) NULL, nylinje);
		      notintext = TRUE;
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'M':
	      if (!strcmp (yytext, "MAIN"))
		{
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (isalpha (lexchar) | lexchar == '_')
		    {
		      scan_name ();

		      mainroutine= yytext;
		      yytext= obstack_finish (&os_lex);
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'N':
	      if (!strcmp (yytext, "NOCOMMENT"))
		return;
	      else if (!strcmp (yytext, "NAMEASVAR"))
		{
		  flag = &nameasvar;
		  scan_dirflags ();
		}
	      else if (!strcmp (yytext, "NONETEST"))
		{
		  flag = &flaggpass2;
		  mellflag = MNONETEST;
		  scan_dirflags ();
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'P':
	      if (!strcmp (yytext, "PAGE")) ;
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'S':
	      if (!strcmp (yytext, "STRIPSIDEEFFECTS"))
		{
		  flag = &flaggpass2;
		  mellflag = MSTRIPSIDEEFFECTS;
		  scan_dirflags ();
		}
	      else if (!strcmp (yytext, "STATICBLOCK"))
		{
		  flag = &staticblock;
		  scan_dirflags ();
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'T':
	      if (!strcmp (yytext, "TIMESTAMP"))
		{
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (isalpha (lexchar) | lexchar == '_')
		    {
		      scan_name ();
		  
		      directive_timestamp= yytext;
		      yytext= obstack_finish (&os_lex);
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!strcmp (yytext, "TITLE")) ;
	      else if (!bl_in_dir_line) lerror (8);
	      break;
	    case 'U':
	      if (!strcmp (yytext, "UNDEF"))
		{
		  while (lexchar == ' ' | lexchar == '\t')
		    newlexchar;
		  if (isalpha (lexchar) | lexchar == '_')
		    {
		      scan_name ();

		      define_name (tag (yytext), FALSE);
		    }
		  else if (!bl_in_dir_line) lerror (8);
		}
	      else if (!bl_in_dir_line) lerror (8);
	      break;

	    }
	}
      else if (!bl_in_dir_line) lerror (8);
    }
  else
    /* Hvis det er en blank eller tom linje er hele linja en kommentar */
    if (lexchar != ' ' & lexchar != '\t' & lexchar != '\n'
	& lexchar != '\r' & lexchar != '!' & lexchar != EOF)
      if (!bl_in_dir_line) lerror (8);

  while (lexchar != '\n' && lexchar != EOF) newlexchar;
  lineno++;
  if (!option_write_tokens)
    mout (MNEWLINE);
}


/******************************************************************************
                                                                   INITLEX   */

void lex_init (void)
{
  obstack_init (&os_lex);
  first_object_allocated_ptr_lex= obstack_alloc (&os_lex, 0);

  obstack_init (&os_ifdef);
  first_object_allocated_ptr_ifdef= obstack_alloc (&os_ifdef, 0);
}

int lex_init_pass1 (char *sourcename)
{
  yytext= obstack_finish (&os_lex);
  ifdefp= obstack_alloc (&os_ifdef, sizeof (ifdefstack_t));
  ifdefp->prev= NULL;
  ifdefp->ifdef= 0;
  if (mapline_init (sourcename, ifdefp)) return (TRUE);
  lineno = 1L;
  while (newlexchar == '%')
    scan_dirline ();
  unput (lexchar);
  return FALSE;
}

void lex_reinit (void)
{
  obstack_free (&os_lex, first_object_allocated_ptr_lex);
  obstack_free (&os_ifdef, first_object_allocated_ptr_ifdef);
}
/******************************************************************************
  						          PUTCHARACTER       */

/* Hjelpe-prosedyre for } bygge opp et konstant-tektsobjekt. */

static char *putcharacter (unsigned char character)
{
  static char s[10];
  if (external == TRUE)
    sprintf (s, "%c", character);
  else if (isgraph (character) && character != '\\' && character != '"')
    sprintf (s, "%c", character);
  else
    sprintf (s, "\\%03o", character);
  return (s);
}

/******************************************************************************
  							      PUTCHARTEXT    */

/* Prosedyre som fungerer som grensesnitt mot scanner,
 *  for } bygge opp et konstant-tekstobjekt.
 *  Denne rutinen kalles for hvert tegn som skal inn i tekst-objektet. */

static putchartext ( unsigned char character)
{
  char *s;
  s = putcharacter (character);
  obstack_grow (&os_lex, s, strlen (s));
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

static char *gettext (void)
{
  char *s;

  if (obstack_object_size (&os_lex) >= MAX_TEXT_CHAR)
    lerror (44);
  obstack_1grow (&os_lex, 0);
  s= obstack_finish (&os_lex);
  yytext= obstack_finish (&os_lex);
  leerror = FALSE;
  return (s);
}

/******************************************************************************
                                                                   YYLEX     */

/* Returnerer et token til parser. */

int yylex (void)
{
  char first_lexchar;
  int reported;

  while (TRUE)
  {
    lineno += antnewline;
    for (; antnewline; antnewline--)
      if (!option_write_tokens)
	mout (MNEWLINE);
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
	scan_name ();
	
	unput (lexchar);
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
	      {
	      end:
		reported=0;
		newlexchar;
		while (TRUE)
		  {
		    while (!isalpha (lexchar))
		      if (lexchar == '\n')
			{
			  antnewline++;
			  while (newlexchar == '%')
			    scan_dirline ();
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
			    if (newlexchar == 'D' && !isalnum (newlexchar) 
				&& lexchar != '_')
			      {	/* END is found and comment is terminated */
				unput (lexchar);
				newsymbole = HEND;
				return (HEND);
			      } else 
				if (antnewline && !reported) 
				  {lerror (32); reported = 1;}
			  }
			else if (lexchar == 'L' && newlexchar == 'S' 
				 && newlexchar == 'E'
				 && !isalnum (newlexchar) && lexchar != '_')
			  { /* ELSE is found and comment is terminated */
			    unput (lexchar);
			    newsymbole = HELSE;
			    return (HEND);
			  } else if (antnewline && !reported) 
			    {lerror (32); reported = 1;}
		      }
		    else if (lexchar == 'W')
		      {
			if (newlexchar == 'H' && newlexchar == 'E' 
			    && newlexchar == 'N'
			    && !isalnum (newlexchar) && lexchar != '_')
			  { /* WHEN is found and comment is terminated */
			    unput (lexchar);
			    newsymbole = HWHEN;
			    return (HEND);
			  } else if (antnewline && !reported) 
			    {lerror (32); reported = 1;}
		      }
		    else if (lexchar == 'O' && newlexchar == 'T' 
			     && newlexchar == 'H' && newlexchar == 'E' 
			     && newlexchar == 'R' && newlexchar == 'W'
			     && newlexchar == 'I' && newlexchar == 'S' 
			     && newlexchar == 'E'
			     && !isalnum (newlexchar) && lexchar != '_')
		      {	/* OTHERWISE is found and comment is terminated */
			unput (lexchar);
			newsymbole = HOTHERWISE;
			return (HEND);
		      } else if (antnewline && !reported) 
			{lerror (32); reported = 1;}
		    while (isalpha (lexchar) || lexchar == '_')
		      newlexchar;
		  }
	      }
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
	    if (!strcmp (yytext, "VAR"))
	      return (HVAR);
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
	yylval.ident = tag (yytext);
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
	return (HFACTOROPERATOR);
      case '.':
	if (isdigit (newlexchar))
	  {
	  dotdigit:
	    /* Behandling av tall som starter med tegnet '.' */
	    obstack_free (&os_lex, yytext);
	    lexradix = 10;
	  digitsdot:
	    obstack_1grow (&os_lex, '.');
	    if (lexchar >= '0' & lexchar <= '9')
	      obstack_1grow (&os_lex, lexchar);
	    while (newlexchar >= '0' && lexchar <= '9' || lexchar == '_')
	      if (lexchar != '_')
		obstack_1grow (&os_lex, lexchar);
	    if (lexchar == '&')
	      {
		newlexchar;
	      digitsexp:
		if (lexchar == '&')
		  newlexchar;
		obstack_1grow (&os_lex, 'e');
		if (lexchar == '-')
		  {
		    obstack_1grow (&os_lex, '-');
		    newlexchar;
		  }
		else if (lexchar == '+')
		  newlexchar;
		if (lexchar >= '0' & lexchar <= '9')
		  obstack_1grow (&os_lex, lexchar);
		while (newlexchar >= '0' && lexchar <= '9' || lexchar == '_')
		  obstack_1grow (&os_lex, lexchar);
	      }
	    obstack_1grow (&os_lex, 0);
	    yytext= obstack_finish (&os_lex);

	    unput (lexchar);

	    yylval.rval = strtod (yytext, NULL);

	    if (lexradix != 10)
	      lerror (16);
	    return (HREALKONST);

	  }
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
	    lexradix = 10;

	    obstack_free (&os_lex, yytext);
	    obstack_1grow (&os_lex, '1');

	    goto digitsexp;
	  }
	unput (lexchar);
	return (HCONC);
      case '!':
      comment:
        while ((newlexchar != ';') && lexchar != EOF)
	  if (lexchar == '\n')
	  {
	    antnewline++;
	    while (newlexchar == '%')
	      scan_dirline ();
	    unput (lexchar);
	  }
	if (lexchar != ';')
	  {
	    lerror (1);
	    unput (lexchar);
	  }
	break;
      case '\'':
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
				|| (firstchar == '2' && 
				    (secondchar < '5'
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
      case '\"':
	obstack_free (&os_lex, yytext);
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
				    || (firstchar == '2' 
					&& (secondchar < '5'
					    || (secondchar == '5'
						&& thirdchar < '6'))))
				  {
				    putchartext 
				      ((unsigned char) 
				       (((firstchar - '0') * 10
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
			    putchartext ((unsigned char) 
					 ((firstchar - '0') * 10
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
	break;
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
	lexradix = 10;
	first_lexchar= lexchar;

	obstack_free (&os_lex, yytext);
	obstack_1grow (&os_lex, lexchar);
	if (newlexchar == 'R' && (first_lexchar == '2' | first_lexchar == '4' 
				  | first_lexchar == '8'))
	  {
	    lexradix = first_lexchar - '0';
	    yytext= obstack_finish (&os_lex);
	    obstack_free (&os_lex, yytext);
	  }
	else if (first_lexchar == '1' && lexchar == '6')
	  {
	    obstack_1grow (&os_lex, lexchar);
	    if (newlexchar == 'R')
	      {
		lexradix = 16;
		yytext= obstack_finish (&os_lex);
		obstack_free (&os_lex, yytext);
	      }
	    else
	      unput (lexchar);
	  }
	else
	  unput (lexchar);
	while ((lexradix == 16 ? isxdigit (newlexchar) : isdigit (newlexchar))
	       || lexchar == '_')
	  if (lexchar == '_');
	  else if (isdigit (lexchar))
	    obstack_1grow (&os_lex, lexchar);
	  else
	    obstack_1grow (&os_lex, lexchar + ('9' + 1 - 'A'));
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
	obstack_1grow (&os_lex, 0);
	yytext= obstack_finish (&os_lex);

	unput (lexchar);
	yylval.ival = radix (lexradix, yytext);
	return (HINTEGERKONST);

      case EOF:
	if (ifdefp != include_ifdefp ())
	  {
	    lerror (24);
	    ifdefp = (ifdefstack_t *) include_ifdefp ();
	  }
	fclose (include_file ());	
	popfilmap ();
	if (no_filemap ())
	  return (NOSYMBOL);
	define_name (tag ("INCLUDED"), FALSE);
	/* Ingen break her */
      case '\n':			/* NL (LF) */
	lineno++;
	if (!option_write_tokens)
	  mout (MNEWLINE);
	while (newlexchar == '%')
	  scan_dirline ();
	unput (lexchar);
	break;
      case ' ':
      case '\b':			/* BS */
      case '\t':			/* HT */
      case '\v':			/* VT */
      case '\f':			/* FF */
      case '\r':			/* CR */
	break;
      default:
	lerror (7);
	break;
      }
  }
}

/******************************************************************************
                                                       SCAN_AND_WRITE_TOKENS */

void scan_and_write_tokens (void)
{
  int token;
  long line = 1L;
  /* printf("% Cim_pp\n%line 1 %s\n",sourcename); */
  while (token = yylex ())
    {
      while (line < lineno)
	{
	  putchar ('\n');
	  line++;
	}
      print_lexsymbol (token, &yylval);
    }
}

