/* $Id: cextspec.c,v 1.17 1997/01/26 14:30:17 cim Exp $ */

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

/* Inn og utlesing av externe spesifikasjoner */

#include "cextern.h"
#include "cdekl.h"
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
#define inchar getc(EXTERNALSPECFILE)
#define inval(i)  { int tmp;(void)fscanf(EXTERNALSPECFILE,"%d",&tmp);i=tmp;}
#define inconst(i)  { (void)fscanf(EXTERNALSPECFILE,"%d",&i);}
#define inname(x) { (void)fscanf(EXTERNALSPECFILE,"%s",x);}

/* fscanf leter frem til neste \n eller blank (eller til slutten) men lar 
 * \n eller blank bli igjen.                                              
 * Hvis \n er forste tegn n}r fscanf kalles s} kastes dette tegnet.Men    
 * getc kalles etter fscanf s} vil denne returnere med \n.Derfor m} dette 
 * tegnet leses av etter at hvert navn er lest inn
 * For å overføre filnavn id til deklarasjonslageret */

FILE *EXTERNUT;
char timestamp[TIMESTAMPLENGTH + 1];	/* Det globale tidsmerket */
char directive_timestamp[TIMESTAMPLENGTH + 1];
char hhtimestamp[TIMESTAMPLENGTH + 1];	/* Lokalt tidsmerke i denne modulen */
char *external_spec_id;		/* Hash verdien til konkateneringen av
				 * tidsmerke, * en blank og filnavnet */
struct stamp *first_stamp;

char timestampchars[63] =
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

  FILE *EXTERNALSPECFILE;
  int t,
    th;
  int tpos = 0;;
  if (strcmp (directive_timestamp, ""))
    {
      strcpy (timestamp, directive_timestamp);
    }
  else if (option_reuse_timestamp 
	   && (EXTERNALSPECFILE = positionarch (extkodnavn)) != NULL)
    {
      if (option_verbose)
	fprintf (stderr, "Reading atr-file %s\n", extkodnavn);
      /* Leser identifikasjon , som alltid ligger f|rst p} filen */
      {
	char r_buff[12];
	r_buff[0] = '\0';
	(void) fscanf (EXTERNALSPECFILE, "%11s\n", r_buff);
	if (strcmp (r_buff, "/*Cim_atr*/"))
	  merror (5, extkodnavn);
      }

      inname (timestamp);
      (void) fclose (EXTERNALSPECFILE);
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

char *
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

char *
getidentid (EXTERNALSPECFILE)
     FILE *EXTERNALSPECFILE;
{
  char s[YYTEXTLENGTH];

  inname (s);
  (void) inchar;		/* Leser av <LF> eller <blank> */
  return (tag (s, strlen (s)));
}

/******************************************************************************
                                                 GENATRFILENAMEFROMID        */

char *
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

char *
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

char *
extract_filename (s)
     char *s;
{
  /* Ekstraherer ut filnavn */
  for (; *s != ' '; s++);
  return (++s);
}

/******************************************************************************
                                                              EXTERNAL_IS_IN */

char 
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

/******************************************************************************
                                                             SHORT_FILE_NAME */

char *
short_file_name (f)
     FILE *f;
{
  int i,
    c;
  static char s[17];
  for (i = 0; i < 16; i++)
    s[i] = getc (f);
  for (i = 15; i >= 0; i--)
    if (s[i] != ' ')
      break;
  if (i >= 0 && s[i] != '/')
    i++;
  s[i] = '\0';
  if ((c = getc (f)) != 0)
    ungetc (c, f);
  if (c == EOF)
    return (NULL);
  else
    return (s);
}

static char link;

/******************************************************************************
                                                                POSITIONARCH */

FILE *
positionarch (filename)
     char *filename;
{
  FILE *EXTERNALSPECFILE;
  static char *string_table;
  static int string_table_length;
  int i,
    sl;
  long l;
  char s1[17],
   *s2,
    c;
#if OPEN_FILE_IN_BINARY_MODE
  if ((EXTERNALSPECFILE = fopen (filename, "rb")) != NULL)
#else
  if ((EXTERNALSPECFILE = fopen (filename, "r")) != NULL)
#endif
    {
      link = TRUE;
      return (EXTERNALSPECFILE);
    }
  else
    link = FALSE;
  for (i = 0; i < antsimlib; i++)
    {
#if OPEN_FILE_IN_BINARY_MODE
      EXTERNALSPECFILE = fopen (simlib[i], "rb");
#else
      EXTERNALSPECFILE = fopen (simlib[i], "r");
#endif
      if (EXTERNALSPECFILE == NULL)
	merror (6, simlib[i]);
      (void) fscanf (EXTERNALSPECFILE, "%7s", s1);
      getc (EXTERNALSPECFILE);
      if (strcmp (s1, "!<arch>"))
	merror (7, simlib[i]);
      while ((s2 = short_file_name (EXTERNALSPECFILE)) != NULL)
	{
	  (void) fscanf (EXTERNALSPECFILE, "%12ld", &l);
	  (void) fscanf (EXTERNALSPECFILE, "%6ld", &l);
	  (void) fscanf (EXTERNALSPECFILE, "%6ld", &l);
	  (void) fscanf (EXTERNALSPECFILE, "%8ld", &l);
	  (void) fscanf (EXTERNALSPECFILE, "%10ld", &l);
	  while ((c = getc (EXTERNALSPECFILE)) != '`' && c != EOF);
	  if (c != '`' || getc (EXTERNALSPECFILE) != '\n')
	    merror (8, simlib[i]);
	  if (!strcmp (s2, filename))
	    return (EXTERNALSPECFILE);
	  if (s2[0] == '/' && isdigit(s2[1]))
	    {
	      long pos;
	      sscanf(&s2[1],"%ld",&pos);
	      if (!strcmp(&string_table[pos], filename))
		return (EXTERNALSPECFILE);
	    }
	  if (l & 1)
	    l++;
	  if(!strcmp(s2,"/"))
	    {
	      int pos;
	      if (string_table_length<l)
		string_table = (char *) salloc (l);
	      for (pos=0; pos<l; pos++)
		{
		  char c;
		  c = getc (EXTERNALSPECFILE);
		  if (c == '/' || c == '\n')
		    string_table[pos] = '\0';
		  else string_table[pos] = c;
		}
	    } else
	      {
#if NO_SEEK_IN_AR
	        while (l-- > 0)
	          getc (EXTERNALSPECFILE);
#else
	        (void) fseek (EXTERNALSPECFILE, l, 1);
#endif
	      }
	}
      (void) fclose (EXTERNALSPECFILE);
    }
  return (NULL);
}

/******************************************************************************
                                                                      LESINN */

lesinn (filename)
     char *filename;
{
  char *sp;
  struct stamp *st;
  FILE *EXTERNALSPECFILE;

  EXTERNALSPECFILE = positionarch (filename);
  if (EXTERNALSPECFILE == NULL)return(TRUE);

  if (option_verbose)
    fprintf (stderr, "Reading atr-file %s\n", filename);
  /* Leser identifikasjon , som alltid ligger f|rst p} filen */
  {
    char r_buff[12];
    r_buff[0] = '\0';
    (void) fscanf (EXTERNALSPECFILE, "%11s\n", r_buff);	
    if (strcmp (r_buff, "/*Cim_atr*/"))
      merror (5, filename);
  }

  /* Leser tidsmerke */

  inname (hhtimestamp);
  (void) inchar;
  /* Konkatenerer tidsmerke og filnavn med en blank mellom   */
  external_spec_id = maketimestampandfilename (hhtimestamp, filename);
  sp = tag (hhtimestamp, strlen (hhtimestamp));
  for (st = first_stamp; st != NULL; st = st->next)
    if (st->timestamp == sp)
      goto found;
  st = (struct stamp *) salloc (sizeof (struct stamp));
  st->timestamp = sp;
  st->next = first_stamp;
  st->lest_inn = FALSE;
  st->filename = filename;
  first_stamp = st;
found:
  if (st->filename != filename)
    merror (11, filename);
  if (st->lest_inn == FALSE && link)
    {
      char *newlink_moduler;
      newlink_moduler =
	(char *) salloc (1 + strlen (link_moduler) + strlen (filename));
      (void) strcpy (newlink_moduler, link_moduler);
      (void) strcat (newlink_moduler, filename);
      newlink_moduler[strlen (newlink_moduler) - 4] = '\0';
      (void) strcat (newlink_moduler, OBJECT_SUFFIX);
      (void) strcat (newlink_moduler, LINK_FILE_SEP_MARKER);
      link_moduler = newlink_moduler;
    }
  st->lest_inn = TRUE;
  /* Leser inn liste med tidsmerker */
  while (inchar == ' ')
    {
      char filename[YYTEXTLENGTH],
       *filenamep;
      inname (hhtimestamp);
      (void) inchar;
      inname (filename);
      (void) inchar;
      sp = tag (hhtimestamp, strlen (hhtimestamp));
      filenamep = tag (filename, strlen (filename));

      for (st = first_stamp; st != NULL; st = st->next)
	if (st->timestamp == sp)
	  goto find_next;
      st = (struct stamp *) salloc (sizeof (struct stamp));
      st->timestamp = sp;
      st->next = first_stamp;
      st->lest_inn = FALSE;
      st->filename = filenamep;
      first_stamp = st;
    find_next:;
      if (st->filename != filenamep)
	merror (11, filenamep);
    }

  while (nextdecl (EXTERNALSPECFILE));

  (void) fclose (EXTERNALSPECFILE);
  return (FALSE);
}

/******************************************************************************
                                                                    NEXTDECL */

nextdecl (EXTERNALSPECFILE)
     FILE *EXTERNALSPECFILE;
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
  categ = inchar;

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
      type = inchar;
      kind = inchar;

      ident = getidentid (EXTERNALSPECFILE);	/* Leser navnet */
      inname (hhtimestamp);
      (void) inchar;		/* tidsmerket   */
      inname (filename);
      (void) inchar;		/* filnavnet    */

      external_spec_id = this_external_spec_id =
	maketimestampandfilename (hhtimestamp, filename);
      if (!external_is_in (ident, kind))
	lesinn (tag (filename, strlen (filename)));
      if (external_spec_id != this_external_spec_id)
	merror (4, extract_filename (hexternid));

      external_spec_id = hexternid;
      return (TRUE);
    }

  type = inchar;
  kind = inchar;
  ident = getidentid (EXTERNALSPECFILE);

  switch (kind)
    {
    case KPROC:
    case KCLASS:
      tegn = inchar;
      if (tegn == PREFIKS_MARKER)
	{
	  prefquantident = getidentid (EXTERNALSPECFILE);
	  tegn = inchar;
	  if (tegn == CPROC_MARKER)
	    {
	      rtname = getidentid (EXTERNALSPECFILE);
	      if (categ != CCPROC)
		{
		  codeclass = inchar - '0';
		  with_codeclass = TRUE;
		}
	    }
	  else
	    ungetc (tegn, EXTERNALSPECFILE);
	}
      else
	{
	  prefquantident = 0;
	  if (tegn == CPROC_MARKER)
	    {
	      rtname = getidentid (EXTERNALSPECFILE);
	      if (categ != CCPROC)
		{
		  codeclass = inchar - '0';
		  with_codeclass = TRUE;
		}
	    }
	}
      newdekl (ident);
      newblck (kind);
      cblock->externid = external_spec_id;

      if (kind == KCLASS)
	{
	  inval (cblock->fornest);
	  inval (cblock->connest);
	}
      inval (cblock->ptypno);
      if (inchar == '%')
	cblock->blno = cblock->ptypno;
      inval (cblock->ent);
      /* Les inn parametere, virtuelle, hidden, protected og deklarasjoner */
      while (nextdecl (EXTERNALSPECFILE));
      insert_with_codeclass = with_codeclass;
      endblock (rtname, codeclass);
      insert_with_codeclass = FALSE;
      break;
    default:
      if (type == TREF)
	prefquantident = getidentid (EXTERNALSPECFILE);
      switch (categ)
	{
	case CCONST:
	  newdekl (ident);
	  if (type == TTEXT)
	    {
	      int i;
	      inval (i);
	      (void) inchar;
	      clastdecl->value.tval.txt = (char *) salloc (i + 1);
	      inname (clastdecl->value.tval.txt);
	      (void) inchar;
	    }
	  else if (type == TREAL)
	    {
	      inconst (clastdecl->value.rasiival.i1);
	      (void) inchar;
	      inconst (clastdecl->value.rasiival.i2);
	      (void) inchar;
	    }
	  else
	    {
	      inconst (clastdecl->value.ival);
	      (void) inchar;
	    }
	  break;
	case CLOCAL:
	case CVIRT:
	case CHIDEN:
	case CPROT:
	case CHIPRO:
	  newdekl (ident);
	  if (type == TLABEL)
	    inconst (cprevdecl->idplev.plev);
	  break;
	default:
	  hcateg = categ;
	  hkind = kind;
	  categ = CDEFLT;
	  kind = KNOKD;
	  newdekl (ident);
	  if (hcateg != CDEFLT)
	    {
	      categ = hcateg;
	      newdekl (ident);
	    }
	  categ = CSPEC;
	  kind = hkind;
	  newdekl (ident);
	  if (type == TLABEL)
	    inconst (cprevdecl->idplev.plev);
	}
      if (kind == KARRAY)
	inval (clastdecl->dim);
    }
  return (TRUE);
}

/******************************************************************************
                                                       SKRIVUT_EXTERNAL_SPEC */

skrivut_external_spec ()
{
  /* Trenger ikke skrive ut lokale deklarasjoner i procedyrer
   * Merker i tillegg de klasser/prosedyrer som er kallbare
   * utenifra modulen ved } sette feltet ptypno!=0 i BLOCK-
   * objektene. */
  struct DECL *rd;
  struct stamp *st;

  char hcateg;

#if OPEN_FILE_IN_BINARY_MODE
  if ((EXTERNUT = fopen (extkodnavn, "wb")) == NULL)
#else
  if ((EXTERNUT = fopen (extkodnavn, "w")) == NULL)
#endif
    merror (9, extkodnavn);

  /* Skriver f|rst ut identifikasjon til find */
  (void) fprintf (EXTERNUT, "/*Cim_atr*/\n");

  /* Skriver ut tidsmerke */
  (void) fprintf (EXTERNUT, "%s\n", timestamp);

  /* Skriver ut tidsmerke til alle moduler */
  for (st = first_stamp; st != NULL; st = st->next)
    (void) fprintf (EXTERNUT, " %s %s\n"
		    ,st->timestamp, st->filename);
  fprintf (EXTERNUT, "\n");

  for (rd = display[EXTERNALGLOBALBLEV]->parloc; rd != NULL; rd = rd->next)
    if (rd->categ == CEXTR) /* OK */ ;
    else if (rd->categ == CEXTRMAIN)
      {
	rd->categ = CEXTR;
	skrivut (rd);
      }
    else
      {
	hcateg = rd->categ;
	rd->categ = CEXTROUT;
	if (rd->kind == KCLASS || rd->kind == KPROC)
	  skrivut (rd);
	rd->categ = hcateg;
      }
  (void) fprintf (EXTERNUT, "%c", ENDOF_CLASS_PROC_FILE);
  (void) fclose (EXTERNUT);
}

/******************************************************************************
                                                                     SKRIVUT */

skrivut (rd)
     struct DECL *rd;
{
  if (rd->categ == CEXTR || rd->categ == CEXTRMAIN)
    (void) fprintf (EXTERNUT, "&%c%c%s %s\n", rd->type, rd->kind
		    ,rd->ident, rd->descr->externid);
  else if (rd->kind == KPROC || rd->kind == KCLASS)
    {
      struct DECL *rdv;
      struct BLOCK *rb;
      rb = rd->descr;
      if (rd->categ == CEXTROUT)
	rd->categ = CEXTR;
      /* Skriver ut <categ><type><kind><navn><blank> */
      (void) fprintf (EXTERNUT, "%c%c%c%s ", rd->categ, rd->type, rd->kind,
		      rd->ident);

      if (rd->prefqual != NULL && rd->prefqual != commonprefiks)
	/* Prefiks klassens navn eller kvalifikasjon */
	(void) fprintf (EXTERNUT, "%c%s ", PREFIKS_MARKER, rd->prefqual->ident);
      else if (rd->categ == CCPROC)	/* C-navnet */
	(void) fprintf (EXTERNUT, "%c%s ", CPROC_MARKER
			,((exinfop) rd->descr->hiprot)->rtname);
      else
	(void) fprintf (EXTERNUT, "%c", NO_MARKER);

      /* Fornest,Connest,blno,ent */
      if (rb->quant.kind == KCLASS)
	(void) fprintf (EXTERNUT, "%d %d ", rb->fornest, rb->connest);
      (void) fprintf (EXTERNUT, "%d %d", rb->blno, rb->ent);

      /* evt. parametere */
      for (rd = rb->parloc; rd != NULL && (rd->categ == CDEFLT || rd->categ == CNAME ||
				   rd->categ == CVAR || rd->categ == CVALUE)
	   ; rd = rd->next)
	skrivut (rd);

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
	    skrivut (rdv);

	  /* evt. spesifikasjoner av hidden og protected. */
	  for (rdv = rb->hiprot; rdv != NULL; rdv = rdv->next)
	    skrivut (rdv);

	  /* Lokale deklarasjoner.  NB Forutsetter at rd peker p} forste
	   * deklarasjon */
	  for (; rd != NULL; rd = rd->next)
	    skrivut (rd);
	}
      (void) fprintf (EXTERNUT, "%c", ENDOF_CLASS_PROC_FILE);
    }
  else
    {
      (void) fprintf (EXTERNUT, "%c%c%c%s ", rd->categ, rd->type,
		      rd->kind, rd->ident);

      if (rd->type == TREF)
	(void) fprintf (EXTERNUT, "%s ", rd->prefqual->ident);
      else if (rd->type == TLABEL)
	(void) fprintf (EXTERNUT, "%d", rd->idplev.plev);

      if (rd->categ == CCONST)
	if (rd->type == TTEXT)
	  (void) fprintf (EXTERNUT, "%d %s "
			  ,strlen (rd->value.tval.txt)
			  ,rd->value.tval.txt);
	else if (rd->type == TREAL)
	  (void) fprintf (EXTERNUT, "%d %d "
			  ,rd->value.rasiival.i1
			  ,rd->value.rasiival.i2);
	else
	  (void) fprintf (EXTERNUT, "%d ", rd->value.ival);
      if (rd->kind == KARRAY)
	(void) fprintf (EXTERNUT, "%c", (rd->dim + ((short) '0')));
    }
}

/******************************************************************************
                                                                MORE_MODULES */

more_modules ()
{
  FILE *EXTERNALSPECFILE;
  struct stamp *st;
  for (st = first_stamp; st != NULL; st = st->next)
    if (st->lest_inn == FALSE && (EXTERNALSPECFILE = 
#if OPEN_FILE_IN_BINARY_MODE
        fopen (st->filename, "rb")) != NULL)
#else
        fopen (st->filename, "r")) != NULL)
#endif
      {
	char *newlink_moduler;
	char r_buff[12];

	/* Leser identifikasjon , som alltid ligger f|rst p} filen */
	r_buff[0] = '\0';
	(void) fscanf (EXTERNALSPECFILE, "%11s\n", r_buff);
	if (strcmp (r_buff, "/*Cim_atr*/"))
	  merror (5, st->filename);

	/* Leser tidsmerke */

	inname (hhtimestamp);
	if (tag (hhtimestamp, strlen (hhtimestamp)) == st->timestamp)
	  {
	    if (option_verbose)
	      fprintf (stderr, "Reading atr-file %s\n", st->filename);
	    newlink_moduler =
	      (char *) salloc (1 + strlen (link_moduler) + strlen (st->filename));
	    (void) strcpy (newlink_moduler, link_moduler);
	    (void) strcat (newlink_moduler, st->filename);
	    newlink_moduler[strlen (newlink_moduler) - 3] = '\0';
	    (void) strcat (newlink_moduler, "o ");
	    link_moduler = newlink_moduler;
	    (void) fclose (EXTERNALSPECFILE);
	  }
      }

}





