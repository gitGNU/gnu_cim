/* $Id: cextspec.c,v 1.17 1997/01/26 14:30:17 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen,
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

/* Inn og utlesing av externe spesifikasjoner */

#include "const.h"
#include "dekl.h"
#include "filelist.h"
#include "newstr.h"
#include "cimcomp.h"
#include "extspec.h"
#include "navn.h"

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h>
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#include <strings.h>
#endif /* not STDC_HEADERS and not HAVE_STRING_H */

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

#ifdef STDC_HEADERS
#include <stdlib.h>
#else
double strtod ();
#endif

/* HUSK AT REKKEF\LGEN SKAL V[RE categ,type,kind 
 *
 * Filen starter alltid med <tidsmerke><LF>
 *
 * Deklarasjon av en klasse:
 *
 * <categ><type><kind><navn><blank><'!'<prefiks navn><blank> | '&'>
 * <fornest><blank><connest><blank><blno><blank><ent>
 * <param.spec><virt. spec><hidden og protected spec><deklarasjoner><LF>
 *
 * Deklarasjon og spesifisering av prosedyre:
 *
 * <categ><type><kind><navn><blank><(if (type==TREF)<'!'qualnavn><blank>) |
 *                                 (if (categ==CCPROC)'^'<cnavn><blank>) | '&'>
 * <blno><blank><ent><param.spec><LF>
 *
 *
 * Spesifisering av vanlige parametere og deklarasjon av variable:
 *
 * <categ><type><kind><navn><blank><(if (type==TREF)<qualnavn><blank>)
 * else if(type==TLABEL)<ent>) else tom>
 * <(if (categ==CCONST)<value><blank>) | tom>
 *
 * Spesifisering av et array:
 *
 * <categ><type><kind><navn><blank><(if (type==TREF)<qualnavn><blank>) | tom>
 * <dim>
 *
 * Spesifisering av EXTERNAL klasse eller prosedyre
 *
 * <&><type><kind><navn><blank><tidsmerke><blank><filnavn><LF>
 *
 *
 * Alle filer slutter med en ekstra <LF> */

#define ENDOF_CLASS_PROC_FILE '\n'
#define START_NEW_EXTERNAL_SPEC '&'
#define PREFIKS_MARKER '!'
#define CPROC_MARKER '^'
#define NO_MARKER '$'
#define inchar(f) getc(f)
#define getval(f, i)  { int tmp;(void)fscanf(f,"%d",&tmp);i=tmp;}
#define getconst(f, i)  { (void)fscanf(f,"%d",&i);}
#define getname(f, x) { (void)fscanf(f,"%s",x);}

/* fscanf leter frem til neste \n eller blank (eller til slutten) men lar 
 * \n eller blank bli igjen.                                              
 * Hvis \n er forste tegn n}r fscanf kalles s} kastes dette tegnet.Men    
 * getc kalles etter fscanf s} vil denne returnere med \n.Derfor m} dette 
 * tegnet leses av etter at hvert navn er lest inn
 * For å overføre filnavn id til deklarasjonslageret */

char timestamp[TIMESTAMPLENGTH + 1];	/* Det globale tidsmerket */
char directive_timestamp[TIMESTAMPLENGTH + 1];
static char hhtimestamp[TIMESTAMPLENGTH + 1];	/* Lokalt tidsmerke i denne modulen */
static char *external_spec_id;		/* Hash verdien til konkateneringen av
				 * tidsmerke, * en blank og filnavnet */
struct stamp *first_stamp;

static char timestampchars[63] =
{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_'};

/******************************************************************************
                                                          GETTIMESTAMP       */

char *
gettimestamp ()
{
#if GET_TIME_OF_DAY
  static struct timeval tp;
  static struct timezone tzp;
#endif

  FILE *f;
  int t,
    th;
  int tpos = 0;;
  if (strcmp (directive_timestamp, ""))
    {
      strcpy (timestamp, directive_timestamp);
    }
  else if (option_reuse_timestamp && (f = searc_and_open_name_in_archlist 
				      (extcodename, TRUE)) != NULL)
    {
      if (option_verbose)
	fprintf (stderr, "Reading atr-file %s\n", extcodename);
      /* Leser identifikasjon , som alltid ligger f|rst p} filen */
      {
	char r_buff[12];
	r_buff[0] = '\0';
	(void) fscanf (f, "%11s\n", r_buff);
	if (strcmp (r_buff, "/*Cim_atr*/"))
	  merror (5, extcodename);
      }

      getname (f, timestamp);
      (void) fclose (f);
    }
  else
    {
#if GET_TIME_OF_DAY
      (void) gettimeofday (&tp, &tzp);
      t = tp.tv_sec;
      th = tp.tv_usec;
#else
      t = time (NULL);
      th = 0;
#endif
      th /= 252;
      timestamp[tpos++] = timestampchars[th - th / 63 * 63];
      th /= 63;
      timestamp[tpos++] = timestampchars[th - th / 63 * 63];
      while (t != 0)
	{
	  timestamp[tpos++] = timestampchars[t - t / 63 * 63];
	  t /= 63;
	}
      timestamp[tpos] = '\0';
    }
}

/******************************************************************************
                                                    MAKETIMESTAMPANDFILENAME */

static char *
maketimestampandfilename (timestamp, filename)
     char *timestamp,
      *filename;
{
  char s[YYTEXTLENGTH + TIMESTAMPLENGTH];
  (void) strcpy (s, timestamp);
  (void) strcat (s, " ");
  (void) strcat (s, filename);
  return (tag (s, strlen (s)));
}

/******************************************************************************
                                                          GETIDENTID         */

static char *
getidentid (f)
     FILE *f;
{
  char s[YYTEXTLENGTH];

  getname (f, s);
  (void) getc (f);		/* Leser av <LF> eller <blank> */
  return (tag (s, strlen (s)));
}

/******************************************************************************
                                                 GENATRFILENAMEFROMID        */

static char *
genatrfilenamefromid (ident)
     char *ident;
{
  int i;
  char s[YYTEXTLENGTH];
  (void) sprintf (s, "%s.atr", ident);
  for (i = strlen (s) - 5; i >= 0; i--)
    if (s[i] >= 'A' && s[i] <= 'Z')
      s[i] += 32;		/* LOWERCASE */
  return (tag (s, strlen (s)));
}

/******************************************************************************
                                              GENATRFILENAMEFROMFILENAME     */

static char *
genatrfilenamefromfilename (filename)
     char *filename;
{
  char s[YYTEXTLENGTH];
  int l = strlen (filename);
  (void) strcpy (s, filename);
  if (l > SAFEYYTEXTLENGTH)	/* 4 er lik lengden av ".atr" */
    merror (1, filename);

  if (l >= 4 & !strcmp (&s[l - 4], ".atr")) /* OK */ ;
  else if (l >= 4 & !strcmp (&s[l - 4], ".sim"))
    {
      s[l - 3] = 'a';
      s[l - 2] = 't';
      s[l - 1] = 'r';
    }
  else
    (void) strcat (s, ".atr");
  return (tag (s, strlen (s)));
}

/******************************************************************************
                                                        EXTRACT_TIMESTAMP    */
char *
extract_timestamp (s)
     char *s;
{
  /* Ekstraherer ut tidsmerket */
  static char htimestamp[TIMESTAMPLENGTH + 1];	/* Må ha plass til \0 til
						 * slutt */

  sscanf (s, "%s", htimestamp);
  return (htimestamp);
}

/******************************************************************************
                                                            EXTRACT_FILENAME */

static char *
extract_filename (s)
     char *s;
{
  /* Ekstraherer ut filnavn */
  for (; *s != ' '; s++);
  return (++s);
}

/******************************************************************************
                                                              EXTERNAL_IS_IN */

static char 
external_is_in (ident, kind)
     char *ident;
     char kind;
{
  struct DECL *rd;
  struct BLOCK *rb;

  rb = display[cblev];

  for (rd = rb->parloc; rd != NULL; rd = rd->next)
    if (rd->ident == ident && rd->kind == kind)
      return (TRUE);

  return (FALSE);
}

/******************************************************************************
                                                                    NEXTDECL */

static lesinn ();

static nextdecl (f)
     FILE *f;
{
  char hcateg,
    hkind;
  char tegn,
    filename[YYTEXTLENGTH];
  char *ident;

  extern char insert_with_codeclass;
  char with_codeclass = FALSE;
  char codeclass;
  char *rtname;
  char *hexternid;
  categ = getc (f);

  if (categ == (char) EOF)
    merror (5, extract_filename (external_spec_id));
  if (categ == ENDOF_CLASS_PROC_FILE)
    return (FALSE);
  else if (categ == START_NEW_EXTERNAL_SPEC)
    {
      char *this_external_spec_id;

      /* Skal n} lese fra en annen fil. M} ta vare p} id'en til denne filen */
      /* Etter at den nye filen er lest, skal lesingen fra denne filen      */
      /* fortsettes. M} derfor sette tilbake fil id'en etter kallet p{      */
      /* lesinn()                                                           */

      hexternid = external_spec_id;
      type = getc (f);
      kind = getc (f);

      ident = getidentid (f);	/* Leser navnet */
      getname (f, hhtimestamp);
      (void) getc (f);		/* tidsmerket   */
      getname (f, filename);
      (void) getc (f);		/* filnavnet    */

      external_spec_id = this_external_spec_id =
	maketimestampandfilename (hhtimestamp, filename);
      if (!external_is_in (ident, kind))
	lesinn (tag (filename, strlen (filename)));
      if (external_spec_id != this_external_spec_id)
	merror (4, extract_filename (hexternid));

      external_spec_id = hexternid;
      return (TRUE);
    }

  type = getc (f);
  kind = getc (f);
  ident = getidentid (f);

  switch (kind)
    {
    case KPROC:
    case KCLASS:
      tegn = getc (f);
      if (tegn == PREFIKS_MARKER)
	{
	  prefquantident = getidentid (f);
	  tegn = getc (f);
	  if (tegn == CPROC_MARKER)
	    {
	      rtname = getidentid (f);
	      if (categ != CCPROC)
		{
		  codeclass = getc (f) - '0';
		  with_codeclass = TRUE;
		}
	    }
	  else
	    ungetc (tegn, f);
	}
      else
	{
	  prefquantident = 0;
	  if (tegn == CPROC_MARKER)
	    {
	      rtname = getidentid (f);
	      if (categ != CCPROC)
		{
		  codeclass = getc (f) - '0';
		  with_codeclass = TRUE;
		}
	    }
	}
      newdekl (ident);
      newblck (kind);
      cblock->externid = external_spec_id;

      if (kind == KCLASS)
	{
	  getval (f, cblock->fornest);
	  getval (f, cblock->connest);
	}
      getval (f, cblock->ptypno);
      if (getc (f) == '%')
	cblock->blno = cblock->ptypno;
      getval (f, cblock->ent);
      /* Les inn parametere, virtuelle, hidden, protected og deklarasjoner */
      while (nextdecl (f));
      insert_with_codeclass = with_codeclass;
      endblock (rtname, codeclass);
      insert_with_codeclass = FALSE;
      break;
    default:
      if (type == TREF)
	prefquantident = getidentid (f);
      switch (categ)
	{
	case CCONST:
	  newdekl (ident);
	  if (type == TTEXT)
	    {
	      int i;
	      getval (f, i);
	      (void) getc (f);
	      clastdecl->value.tval.txt = (char *) xmalloc (i + 1);
	      getname (f, clastdecl->value.tval.txt);
	      (void) getc (f);
	    }
	  else if (type == TREAL)
	    {
	      char s[100];
	      getname (f, s);
	      clastdecl->value.rval= strtod (s, NULL);
	      (void) getc (f);
	    }
	  else
	    {
	      getconst (f, clastdecl->value.ival);
	      (void) getc (f);
	    }
	  break;
	default:
	  newdekl (ident);
	  if (type == TLABEL)
	    getconst (f, cprevdecl->idplev.plev);
	  break;
	}
      if (kind == KARRAY)
	getval (f, clastdecl->dim);
    }
  return (TRUE);
}

/******************************************************************************
                                                                      LESINN */

static lesinn (filename)
     char *filename;
{
  char *sp;
  struct stamp *st;
  FILE *f;

  f = searc_and_open_name_in_archlist (filename, TRUE);
  if (f == NULL)return(TRUE);

  if (option_verbose)
    fprintf (stderr, "Reading atr-file %s\n", filename);
  /* Leser identifikasjon , som alltid ligger f|rst p} filen */
  {
    char r_buff[12];
    r_buff[0] = '\0';
    (void) fscanf (f, "%11s\n", r_buff);	
    if (strcmp (r_buff, "/*Cim_atr*/"))
      merror (5, filename);
  }

  /* Leser tidsmerke */

  getname (f, hhtimestamp);
  (void) getc (f);
  /* Konkatenerer tidsmerke og filnavn med en blank mellom   */
  external_spec_id = maketimestampandfilename (hhtimestamp, filename);
  sp = tag (hhtimestamp, strlen (hhtimestamp));
  for (st = first_stamp; st != NULL; st = st->next)
    if (st->timestamp == sp)
      goto found;
  st = (struct stamp *) xmalloc (sizeof (struct stamp));
  st->timestamp = sp;
  st->next = first_stamp;
  st->lest_inn = FALSE;
  st->filename = filename;
  first_stamp = st;
found:
  if (st->filename != filename)
    merror (11, filename);
  st->lest_inn = TRUE;
  /* Leser inn liste med tidsmerker */
  while (getc (f) == ' ')
    {
      char filename[YYTEXTLENGTH],
       *filenamep;
      getname (f, hhtimestamp);
      (void) getc (f);
      getname (f, filename);
      (void) getc (f);
      sp = tag (hhtimestamp, strlen (hhtimestamp));
      filenamep = tag (filename, strlen (filename));

      for (st = first_stamp; st != NULL; st = st->next)
	if (st->timestamp == sp)
	  goto find_next;
      st = (struct stamp *) xmalloc (sizeof (struct stamp));
      st->timestamp = sp;
      st->next = first_stamp;
      st->lest_inn = FALSE;
      st->filename = filenamep;
      first_stamp = st;
    find_next:;
      if (st->filename != filenamep)
	merror (11, filenamep);
    }

  while (nextdecl (f));

  (void) fclose (f);
  return (FALSE);
}

/******************************************************************************
                                                        LESINN_EXTERNAL_SPEC */

lesinn_external_spec (ident, filename)
     char *ident;
     char *filename;
{
  char hcateg;
  char *hprefquantident;
  struct BLOCK *hcblock;
  struct DECL *hclastdecl,
   *rd;
  hcateg = categ;
  hprefquantident = prefquantident;
  hcblock = cblock;
  hclastdecl = clastdecl;
  if (filename == NULL)
    filename = genatrfilenamefromid (ident);
  else
    filename = genatrfilenamefromfilename (filename);
  if (!external_is_in (ident, kind))
    {
      if (lesinn (filename))
	{
	  merror (3, filename);
	  return;
	}
    }

  /* Denne modulen skal ha categ=CEXTRMAIN */
  if (hclastdecl == NULL)
    rd = hcblock->parloc;
  else
    rd = hclastdecl;
  for (; rd != NULL && rd->ident != ident; rd = rd->next);

  if (rd != NULL)
    rd->categ = CEXTRMAIN;

  categ = hcateg;		/* M} settes tilbake. */
  prefquantident = hprefquantident;
}

static char link;

/******************************************************************************
                                                           WRITE_INDENTATION */
write_indentation (f, level) FILE *f; int level;
{
  int i;
  fprintf (f, "\n");
  for (i=1; i<=level; i++) fprintf (f, "   ");
}

/******************************************************************************
                                                              WRITE_CHAR_MIF */
static write_char_mif (f, c) FILE *f; unsigned char c;
{
  if ((isprint (c)
#if ISO_LATIN
       || c >= 160
#endif
       ) && c != '!' && c != '"')
    fprintf (f, "= '%c'", c);
  else
    fprintf (f, "= '!%d!'", c);
}

/******************************************************************************
                                                              WRITE_TEXT_MIF */
static write_text_mif (f, s) FILE *f; unsigned char *s;
{
  int i;
  fprintf (f, "= \"");
  for (i = 0; s[i]; i++)
    if (s[i] == '\\')
      {
	char j = 0;
	j = j * 8 + s[++i] - '0';
	j = j * 8 + s[++i] - '0';
	j = j * 8 + s[++i] - '0';
	if (j == ' ')
	  putc (' ', f);
	else
	  fprintf (f, "!%d!", j);
      }
  else
    putc (s[i], f);
  putc ('"', f);
}

/******************************************************************************
                                                              WRITE_DECL_MIF */

static write_decl_mif (f, rd, level)
       FILE *f; struct DECL *rd; int level;
{
  if (level == 0)
    {
      int i;
      fprintf (f, "\n\n%% **************************************"
	       "************************************* %%\n%% ");
      for (i= 75-strlen(rd->ident); i>0; i--)fprintf (f, " ");
      fprintf (f, "%s %%", rd->ident);
    }
  write_indentation (f, level);
  switch(rd->categ)
    {
    case CVALUE:
      fprintf (f, "value: ");
      break;
    case CNAME:
      fprintf (f, "name: ");
      break;
    case CVAR:
      fprintf (f, "var: ");
      break;
    case CEXTR:
      break;
    case CEXTRMAIN:
      break;
    case CEXTROUT:
      break;
    case CHIDEN:
      fprintf (f, "hidden ");
      break;
    case CPROT:
      fprintf (f, "protected ");
      break;
    case CHIPRO:
      fprintf (f, "hidden proteced ");
      break;
    case CCPROC:
      (void) fprintf (f, "external C procedure %s is "
		      ,((exinfop) rd->descr->hiprot)->rtname);
      break;

    }
  switch(rd->type)
    {
    case TINTG:
      fprintf (f, "integer ");
      break;
    case TREAL:
      fprintf (f, "real ");
      break;
      break;
    case TBOOL:
      fprintf (f, "boolean ");
      break;
    case TCHAR:
      fprintf (f, "character ");
      break;
    case TLABEL:
      if (rd->kind == KARRAY) fprintf (f, "switch ");
      else if (rd->categ != CLOCAL) fprintf (f, "label ");
      break;
    case TTEXT:
      fprintf (f, "text ");
      break;
    case TREF:
      fprintf (f, "ref (%s) ", rd->prefqual->ident);
      break;
    case TERROR:
      break;
    case TVARARGS:
      fprintf (f, "... ");
      break;
    }
  switch(rd->kind)
    {
    case KARRAY:
      if (rd->type != TLABEL) fprintf (f, "array ");
      break;
    case KPROC:
      fprintf (f, "procedure ");
      break;
    case KCLASS:
      if (rd->prefqual != NULL && rd->prefqual != commonprefiks)
	/* Prefiks klassens navn eller kvalifikasjon */
	(void) fprintf (f, "%s ", rd->prefqual->ident);
      fprintf (f, "class ");
      break;
    }

  (void) fprintf (f, "%s", rd->ident);

  if (rd->categ == CEXTR || rd->categ == CEXTRMAIN)
    {
    (void) fprintf (f, "= \"%s\"", rd->descr->externid);
    }
  else if (rd->kind == KPROC || rd->kind == KCLASS)
    {
      struct DECL *rdv;
      struct BLOCK *rb;
      struct DECL *rdx;
      rb = rd->descr;
      if (rd->categ == CEXTROUT)
	rd->categ = CEXTR;
	  
      /* evt. parametere */
      fprintf (f, " (");
      for (rdx = rb->parloc; rdx != NULL && (rdx->categ == CDEFLT || rdx->categ == CNAME ||
				   rdx->categ == CVAR || rdx->categ == CVALUE)
	   ; rdx = rdx->next)
	write_decl_mif (f, rdx, level+1);

      fprintf (f, ")");

      switch (rd->categ)
	{
	case CDEFLT:
	case CNAME:
	case CVAR:
	case CVALUE:
	case CVIRT:
	  break;
	default:
	  fprintf (f, ";");
	}

#if 0
      /* Fornest,Connest,blno,ent */
      if (rb->quant.kind == KCLASS)
	(void) fprintf (f, "\n%% f_c_b_e %d %d %d %d",
			rb->fornest, rb->connest, rb->blno, rb->ent);
#endif

      if (rb->quant.kind == KCLASS)
	{
	  int i;
	  write_indentation (f, level);
	  fprintf (f, "virtual:");
	  /* evt. virtuelle spesifiksajoner , men bare de som er spesifisert */
	  /* i denne klassen. Ikke de akkumulerte. De akkumuleres opp av     */
	  /* sjekkdekl senere                                                */
	  i = (rb->quant.idplev.plev > 0) ? rb->quant.prefqual->descr->navirt
	    + rb->quant.prefqual->descr->navirtlab : 0;
	  for (rdv = rb->virt; i-- > 0; rdv = rdv->next);
	  for (; rdv != NULL; rdv = rdv->next)
	    write_decl_mif (f, rdv, level+1);

	  /* evt. spesifikasjoner av hidden og protected. */
	  for (rdv = rb->hiprot; rdv != NULL; rdv = rdv->next)
	    write_decl_mif (f, rdv, level+1);

          write_indentation (f, level);
          fprintf (f, "begin");
	  /* Lokale deklarasjoner NB Forutsetter at rdx peker p} forste
	   * deklarasjon */
	  for (; rdx != NULL; rdx = rdx->next)
	    write_decl_mif (f, rdx, level+1);
	  write_indentation (f, level);
	  fprintf (f, "end");
	}
    }
  else
    {
#if 0
      if (rd->type == TLABEL)
	(void) fprintf (f, "\n%% ENT %d", rd->idplev.plev);
#endif
      if (rd->categ == CCONST)
	if (rd->type == TTEXT)
	  write_text_mif (f, rd->value.tval.txt);
	else if (rd->type == TREAL)
	  {
	    char s[100];
	    int i;
	    sprintf (s, "= %.16le", rd->value.rval);
	    for (i=0; s[i]; i++) 
	      if (s[i]=='e') 
		{
		  s[i]='&';
		  break;
		}
	    fprintf (f, s);
	  }
	else if (rd->type == TCHAR)
	  write_char_mif (f, rd->value.ival);
	else
	  (void) fprintf (f, "= %d", rd->value.ival);
      if (rd->kind == KARRAY && rd->type != TLABEL)
	{
	  int i;
	  fprintf (f, "( ");
	  for (i=2; i<= rd->dim; i++) fprintf (f, ", ");
	  fprintf (f, ")");
	}
    }

  switch (rd->categ)
    {
    case CDEFLT:
    case CNAME:
    case CVAR:
    case CVALUE:
      if (rd->next!=NULL && (rd->next->categ == CDEFLT || 
			     rd->next->categ == CNAME ||
			     rd->next->categ == CVAR || 
			     rd->next->categ == CVALUE))
	fprintf(f, ", ");
      break;
    case CLOCAL:
      if (rd->type == TLABEL && rd->kind == KSIMPLE) 
	{
	  fprintf (f, ":");
	  break;
	}
    default:
      fprintf (f, ";");
    }
}

/******************************************************************************
                                                               WRITE_ALL_MIF */

write_all_mif ()
{
  /* Trenger ikke skrive ut lokale deklarasjoner i procedyrer
   * Merker i tillegg de klasser/prosedyrer som er kallbare
   * utenifra modulen ved } sette feltet ptypno!=0 i BLOCK-
   * objektene. */
  struct DECL *rd;
  struct stamp *st;
  FILE *f;

  char hcateg;

  if ((f = fopen (mifcodename, "w")) == NULL)
    merror (9, mifcodename);

  /* Skriver f|rst ut identifikasjon til find */
  (void) fprintf (f, "%% Cim_mif");

#if 0
  /* Skriver ut tidsmerke */
  (void) fprintf (f, "\n%%timestamp %s", timestamp);

  /* Skriver ut tidsmerke til alle moduler */
  for (st = first_stamp; st != NULL; st = st->next)
    (void) fprintf (f, "\n%% timestamp_other_module %s %s"
		    ,st->timestamp, st->filename);
#endif

  for (rd = display[EXTERNALGLOBALBLEV]->parloc; rd != NULL; rd = rd->next)
    if (rd->categ == CEXTR) /* OK */ ;
    else if (rd->categ == CEXTRMAIN)
      {
	rd->categ = CEXTR;
	write_decl_mif (f, rd, 0);
      }
    else
      {
	hcateg = rd->categ;
	if (rd->categ != CCPROC) rd->categ = CEXTROUT;
	if (rd->kind == KCLASS || rd->kind == KPROC)
	  write_decl_mif (f, rd, 0);
	rd->categ = hcateg;
      }
  (void) fprintf (f, "\n\n%%eof\n");
  (void) fclose (f);
}

/******************************************************************************
                                                              WRITE_DECL_EXT */

static write_decl_ext (f, rd)
       FILE *f; struct DECL *rd;
{
  if (rd->categ == CEXTR || rd->categ == CEXTRMAIN)
    (void) fprintf (f, "&%c%c%s %s\n", rd->type, rd->kind
		    ,rd->ident, rd->descr->externid);
  else if (rd->kind == KPROC || rd->kind == KCLASS)
    {
      struct DECL *rdv;
      struct BLOCK *rb;
      rb = rd->descr;
      if (rd->categ == CEXTROUT)
	rd->categ = CEXTR;
      /* Skriver ut <categ><type><kind><navn><blank> */
      (void) fprintf (f, "%c%c%c%s ", rd->categ, rd->type, rd->kind,
		      rd->ident);

      if (rd->prefqual != NULL && rd->prefqual != commonprefiks)
	/* Prefiks klassens navn eller kvalifikasjon */
	(void) fprintf (f, "%c%s ", PREFIKS_MARKER, rd->prefqual->ident);
      else if (rd->categ == CCPROC)	/* C-navnet */
	(void) fprintf (f, "%c%s ", CPROC_MARKER
			,((exinfop) rd->descr->hiprot)->rtname);
      else
	(void) fprintf (f, "%c", NO_MARKER);

      /* Fornest,Connest,blno,ent */
      if (rb->quant.kind == KCLASS)
	(void) fprintf (f, "%d %d ", rb->fornest, rb->connest);
      (void) fprintf (f, "%d %d", rb->blno, rb->ent);

      /* evt. parametere */
      for (rd = rb->parloc; rd != NULL && (rd->categ == CDEFLT || rd->categ == CNAME ||
				   rd->categ == CVAR || rd->categ == CVALUE)
	   ; rd = rd->next)
	write_decl_ext (f, rd);

      if (rb->quant.kind == KCLASS)
	{
	  int i;
	  /* evt. virtuelle spesifiksajoner , men bare de som er spesifisert */
	  /* i denne klassen. Ikke de akkumulerte. De akkumuleres opp av     */
	  /* sjekkdekl senere                                                */
	  i = (rb->quant.idplev.plev > 0) ? rb->quant.prefqual->descr->navirt
	    + rb->quant.prefqual->descr->navirtlab : 0;
	  for (rdv = rb->virt; i-- > 0; rdv = rdv->next);
	  for (; rdv != NULL; rdv = rdv->next)
	    write_decl_ext (f, rdv);

	  /* evt. spesifikasjoner av hidden og protected. */
	  for (rdv = rb->hiprot; rdv != NULL; rdv = rdv->next)
	    write_decl_ext (f, rdv);

	  /* Lokale deklarasjoner.  NB Forutsetter at rd peker p} forste
	   * deklarasjon */
	  for (; rd != NULL; rd = rd->next)
	    write_decl_ext (f, rd);
	}
      (void) fprintf (f, "%c", ENDOF_CLASS_PROC_FILE);
    }
  else
    {
      (void) fprintf (f, "%c%c%c%s ", rd->categ, rd->type,
		      rd->kind, rd->ident);

      if (rd->type == TREF)
	(void) fprintf (f, "%s ", rd->prefqual->ident);
      else if (rd->type == TLABEL)
	(void) fprintf (f, "%d", rd->idplev.plev);

      if (rd->categ == CCONST)
	if (rd->type == TTEXT)
	  (void) fprintf (f, "%d %s "
			  ,strlen (rd->value.tval.txt)
			  ,rd->value.tval.txt);
	else if (rd->type == TREAL)
	  (void) fprintf (f, "%.16e ", rd->value.rval);
	else
	  (void) fprintf (f, "%d ", rd->value.ival);
      if (rd->kind == KARRAY)
	(void) fprintf (f, "%c", (rd->dim + ((short) '0')));
    }
}

/******************************************************************************
                                                               WRITE_ALL_EXT */

write_all_ext ()
{
  /* Trenger ikke skrive ut lokale deklarasjoner i procedyrer
   * Merker i tillegg de klasser/prosedyrer som er kallbare
   * utenifra modulen ved } sette feltet ptypno!=0 i BLOCK-
   * objektene. */
  struct DECL *rd;
  struct stamp *st;
  FILE *f;

  char hcateg;

#if OPEN_FILE_IN_BINARY_MODE
  if ((f = fopen (extcodename, "wb")) == NULL)
#else
  if ((f = fopen (extcodename, "w")) == NULL)
#endif
    merror (9, extcodename);

  /* Skriver f|rst ut identifikasjon til find */
  (void) fprintf (f, "/*Cim_atr*/\n");

  /* Skriver ut tidsmerke */
  (void) fprintf (f, "%s\n", timestamp);

  /* Skriver ut tidsmerke til alle moduler */
  for (st = first_stamp; st != NULL; st = st->next)
    (void) fprintf (f, " %s %s\n"
		    ,st->timestamp, st->filename);
  fprintf (f, "\n");

  for (rd = display[EXTERNALGLOBALBLEV]->parloc; rd != NULL; rd = rd->next)
    if (rd->categ == CEXTR) /* OK */ ;
    else if (rd->categ == CEXTRMAIN)
      {
	rd->categ = CEXTR;
	write_decl_ext (f, rd);
      }
    else
      {
	hcateg = rd->categ;
	if (rd->categ != CCPROC) rd->categ = CEXTROUT;
	if (rd->kind == KCLASS || rd->kind == KPROC)
	  write_decl_ext (f, rd);
	rd->categ = hcateg;
      }
  (void) fprintf (f, "%c", ENDOF_CLASS_PROC_FILE);
  (void) fclose (f);

  write_all_mif();
}

/******************************************************************************
                                                                MORE_MODULES */

more_modules ()
{
  FILE *f;
  struct stamp *st;
  for (st = first_stamp; st != NULL; st = st->next)
    if (st->lest_inn == FALSE && (f = fopen (st->filename,
#if OPEN_FILE_IN_BINARY_MODE
					     "rb"
#else
					     "r"
#endif
					     )) != NULL)
      {
	char *newlink_moduler;
	char r_buff[12];
	
	/* Leser identifikasjon , som alltid ligger f|rst p} filen */
	r_buff[0] = '\0';
	(void) fscanf (f, "%11s\n", r_buff);
	if (strcmp (r_buff, "/*Cim_atr*/"))
	  merror (5, st->filename);
	
	/* Leser tidsmerke */

	getname (f, hhtimestamp);
	if (tag (hhtimestamp, strlen (hhtimestamp)) == st->timestamp)
	  {
	    if (option_verbose)
	      fprintf (stderr, "Reading atr-file %s\n", st->filename);
	    insert_name_in_linklist 
	      (transform_name (st->filename, ".atr", ".o"), TRUE);
	      
	  }
      }
}
