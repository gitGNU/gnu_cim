/* $Id: rfile.c,v 1.15 1995/02/22 08:32:13 cim Exp $ */

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

/* Run time rutiner for klassen FILE */

#include <ctype.h>
#include <stdio.h>
#include <math.h>
#define __FILES FILE
#include "cim.h"
#include "config.h"

#if 0
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#else
#if HAVE_STAT_H
#include <stat.h>
#endif
#endif
#endif

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

/******************************************************************************
                                                TEXT PROCEDURE FILENAME      */

#if nolib|rfilenamerut
__txtvp 
__rfilename (as, p)
     __bs1FILE *p;
     long as;
{
  return (__rcopy (as, &p->filename));
}
#endif

/******************************************************************************
                                                INTEGER PROCEDURE ISOPEN     */

#if nolib|risopenrut
char 
__risopen (p)
     __bs1FILE *p;
{
  return (p->open);
}
#endif

/******************************************************************************
                                                BOOLEAN PROCEDURE SETACCESS  */

#if nolib|rsetaccessrut

/* Hjelpe rutine som sammenligner to stringer.Sm} og store bokstaver */
/* omhverandre                                                       */

char 
__rcompstr (s1, s2, n)
     char *s1,
      *s2;
     int n;
{				/* String s2 best}r av store bokstaver */
  int i;
  char c;
  for (i = 0; i < n; i++)
    {
      c = s1[i];
      if ((islower (c) ? toupper ((int) c) : c) != s2[i])
	return (__FALSE);
    }
  return (__TRUE);
}

char 
__rsetaccess (p, t)
     __bs1FILE *p;
     __txtvp t;
{
  long length;
  char *s;
  __pty ppx;
  if (t->obj == NULL)
    return (__FALSE);
  length = t->length;
  s = &t->obj->string[t->start - 1];
  ppx = p->h.pp;
  if (length == 6 && __rcompstr (s, "SHARED", 6))
    {
      p->shared = __SHARED;
      return (__TRUE);
    }
  if (length == 8 && __rcompstr (s, "NOSHARED", 8))
    {
      p->shared = __NOSHARED;
      return (__TRUE);
    }
#if HAVE_UNLINK
  if (length == 5 && __rcompstr (s, "PURGE", 5))
    {
      p->purge = __PURGE;
      return (__TRUE);
    }
#endif
  if (length == 7 && __rcompstr (s, "NOPURGE", 7))
    {
      p->purge = __NOPURGE;
      return (__TRUE);
    }
  if (ppx->pref[2] == &__p5FILE || ppx->pref[2] == &__p10FILE) 
                                              /* DIRECT FILES */
    {
      if (length == 8 && __rcompstr (s, "READONLY", 8))
	{
	  p->readwrite = __READONLY;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "WRITEONLY", 9))
	{
	  p->readwrite = __WRITEONLY;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "READWRITE", 9))
	{
	  p->readwrite = __READWRITE;
	  return (__TRUE);
	}
    }
  else
    /* IN AND OUT FILES */
    {
      if (length == 6 && __rcompstr (s, "REWIND", 6))
	{
	  p->re_wind = __REWIND;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOREWIND", 8))
	{
	  p->re_wind = __NOREWIND;
	  return (__TRUE);
	}
    }
  if (ppx->pref[1] == &__p7FILE)	/* BYTE FILES */
    {
      if (length == 10 && __rcompstr (s, "BYTESIZE:8", 10))
	return (__TRUE);
      if (length == 10 && __rcompstr (s, "BYTESIZE:0", 10))
	return (__TRUE);
    }
  if (!(ppx->pref[2] == &__p4FILE || ppx->pref[2] == &__p8FILE))	
                                        /* NOT IN FILES */
    {
      if (length == 6 && __rcompstr (s, "APPEND", 6))
	{
	  p->append = __APPEND;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOAPPEND", 8))
	{
	  p->append = __NOAPPEND;
	  return (__TRUE);
	}
      if (length == 6 && __rcompstr (s, "CREATE", 6))
	{
	  p->create = __CREATE;
	  return (__TRUE);
	}
      if (length == 8 && __rcompstr (s, "NOCREATE", 8))
	{
	  p->create = __NOCREATE;
	  return (__TRUE);
	}
      if (length == 9 && __rcompstr (s, "ANYCREATE", 9))
	{
	  p->create = __ANYCREATE;
	  return (__TRUE);
	}
    }
  return (__FALSE);
}
#endif

/******************************************************************************
                               Hjelperutine som shifter en tekst til venstre */

#if nolib|rleftshiftrut
__rleftshift (t, j)
     __txtvp t;
     long j;
{
  long i,
    l;
  char *to;
  to = &(t->obj->string[t->start - 1]);
  l = t->length;
  if (j > 0)
    {
      for (i = j; i < l; i++)
	to[i - j] = to[i];
      for (i = l - j; i < l; i++)
	to[i] = ' ';
    }
}
#endif

/******************************************************************************
                                          TEXT PROCEDURE FIELD(w)            */

/* ikke en attributt prosedyre */

#if nolib|rfieldrut
__rfield (p, w)
     __bs2FILE *p;
     long w;
{
  if (w > p->IMAGE.length)
    __rerror ("Outint, outfix etc: Width on out field is greater than IMAGE.length");
  if (p->IMAGE.pos + w - 1 > p->IMAGE.length)
    if (((__bs1FILE *) p)->h.pp == &__p6FILE)
      __rpoutimage (((__bs6FILE *) p));
    else if (((__bs1FILE *) p)->h.pp == &__p5FILE)
      __rdoutimage ((__bs5FILE *) p);
    else
      __rooutimage (p);
  __et.obj = p->IMAGE.obj;
  __et.start = p->IMAGE.start + p->IMAGE.pos - 1;
  __et.length = w;
  __et.pos = 1;
}
#endif

/* Run time rutiner for klassen IMAGEFILE */

/******************************************************************************
                                                            SETPOS           */

#if nolib|rsetposrut
__dhp 
__rsetpos (p, i)
     __bs2FILE *p;
     long i;
{
  p->IMAGE.pos = ((i < 1 || i > p->IMAGE.length + 1) ? p->IMAGE.length + 1 : i);
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                                              POS            */

#if nolib|rposrut
long 
__rpos (p)
     __bs2FILE *p;
{
  if (p->IMAGE.obj == __NULL)
    return (1);
  else
    return ((long) p->IMAGE.pos);
}
#endif

/******************************************************************************
                                                              MORE           */

#if nolib|rmorerut
char 
__rmore (p)
     __bs2FILE *p;
{
  if (p->IMAGE.obj == __NULL)
    return (__FALSE);
  else
    return (((long) p->IMAGE.pos <= p->IMAGE.length));
}
#endif

/******************************************************************************
                                                               LENGTH        */

#if nolib|rlengthrut
long 
__rlength (p)
     __bs2FILE *p;
{
  return ((long) p->IMAGE.length);
}
#endif

/* RUNTIME rutiner for klassen directfile  skrevet i c. */

/******************************************************************************
                                         INTEGER PROCEDURE LOCATION          */

/* PS !!! HVIS DET IKKE SKAL V]RE NOEN TESTER INNE I LOCATE S] KAN ALLE
 * KALL rdlocate(((__bs5FILE *)p)->loc) SLETTES. */

#if nolib|rdlocationrut
long 
__rdlocation (p)
     __bs5FILE *p;
{
  return (p->loc);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE ENDFILE           */

#if nolib|rdendfilerut
char 
__rdendfile (p)
     __bs5FILE *p;
{
  return (p->endfile);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE LOCKED            */

#if nolib|rdlockedrut
char 
__rdlocked (p)
     __bs5FILE *p;
{
  return (p->locked);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CHECKPOINT        */

#if nolib|rdcheckpointrut
char 
__rdcheckpoint (p)
     __bs5FILE *p;
{
  if (fflush (((__bs1FILE *) p)->file) == __EOF)
    return (__FALSE);
  else
    return (__TRUE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE UNLOCK            */

#if nolib|rdunlockrut
char 
__rdunlock (p)
     __bs5FILE *p;
{
  char c;
  c = __rdcheckpoint (p);
  if (p->locked)
    {
      /* Release file */
      p->locked = __FALSE;
    }
  return (c);
}
#endif

/******************************************************************************
                                         PROCEDURE LOCK                      */

#if nolib|rdlockrut
#if FLOAT_IMPLEMENTED
/*ARGSUSED */
long 
__rdlock (p, t, i, j)
     __bs5FILE *p;
     double t;
     long i,
       j;
{
  long lock = -1;
  if (t > 0.0)
    {
      if (p->locked)
	(void) __rdunlock (p);
    }
  return (lock);
}
#endif
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|rdopenrut
char 
__rdopen (p, t)
     __bs5FILE *p;
     __txtvp t;
{
  long loc;
  if (!((__bs1FILE *) p)->open)
    {
      ((__bs1FILE *) p)->file 
	= fopen (((__bs1FILE *) p)->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file != __NULL)
	(void) fclose (((__bs1FILE *) p)->file);
      if (((__bs1FILE *) p)->create == __CREATE 
	  && ((__bs1FILE *) p)->file != __NULL)
	return (__FALSE);
      if (((__bs1FILE *) p)->create == __NOCREATE 
	  && ((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
#if VMS_DIRECTFILE
      if (((__bs1FILE *) p)->file == __NULL)
	{
	  if ((((__bs1FILE *) p)->file 
	       = fopen (((__bs1FILE *) p)->filename.obj->string
			,"w")) == __NULL)
	    return (__FALSE);
	  (void) fclose (((__bs1FILE *) p)->file);
	}
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
					    ,"r+")) == __NULL)
	return (__FALSE);
#else
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
				 ,((__bs1FILE *) p)->file == __NULL ? "w+" :
	((__bs1FILE *) p)->readwrite == __READONLY ? "r" : "r+")) == __NULL)
	return (__FALSE);
#endif
      if (((__bs1FILE *) p)->append == __APPEND)
	{
	  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
	    {
	      (void) fclose (((__bs1FILE *) p)->file);
	      return (__FALSE);
	    }
	  loc = ftell (((__bs1FILE *) p)->file);
	}
      else
	loc = 0;
      p->loc = loc / (t->length + 1) + 1;
      if (loc % (t->length + 1))
	__rerror ("Open: Illegal size on direct-file");
      p->minwriteloc = p->loc;
      p->lastop = __SEEK;
      p->maxloc = (MAX_INT - 1) / (t->length + 1);
      if (((__bs1FILE *) p)->readwrite == __READONLY)
	p->minwriteloc = MAX_INT;
      if (((__bs1FILE *) p)->readwrite == __WRITEONLY)
	p->writeonly = __TRUE;
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      ((__bs5FILE *) p)->endfile = __FALSE;
      ((__bs5FILE *) p)->imagelength = t->length;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|rdcloserut
char 
__rdclose (p)
     __bs5FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      if (p->locked)
	(void) __rdunlock (p);
      p->loc = p->maxloc = 0;
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs1FILE *) p)->open = __FALSE;
      return (p->endfile = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         PROCEDURE LOCATE                    */

#if nolib|rdlocaterut
__dhp 
__rdlocate (p, i)
     __bs5FILE *p;
     long i;
{
  if ((i < 1) | (i > p->maxloc))
    __rerror ("Locate: Parameter out of range");
  if (p->loc != i)
    {
      p->loc = i;
      if (fseek (((__bs1FILE *) p)->file, 
		 (i - 1) * (((__bs5FILE *) p)->imagelength + 1), 0) == __EOF)
	__rerror ("Locate: Not possible to seek");
      p->lastop = __SEEK;
    }
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                        INTEGER PROCEDURE LASTLOC            */

#if nolib|rdlastlocrut
long 
__rdlastloc (p)
     __bs5FILE *p;
{
  long pos,
    len;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Lastloc: File not open");
  pos = ftell (((__bs1FILE *) p)->file);
  fseek (((__bs1FILE *) p)->file, 0, 2);
  len = ftell (((__bs1FILE *) p)->file);
  fseek (((__bs1FILE *) p)->file, pos, 0);
  return (len / (((__bs5FILE *) p)->imagelength + 1));
}
#endif

/******************************************************************************
                                         INTEGER PROCEDURE MAXLOC            */

#if nolib|rdmaxlocrut
long 
__rdmaxloc (p)
     __bs5FILE *p;
{
  if (!((__bs1FILE *) p)->open)
    __rerror ("Maxloc: File not open");
  return (p->maxloc);
}
#endif

/******************************************************************************
                                         PROCEDURE INIMAGE                   */

#if nolib|rdinimagerut
__dhp 
__rdinimage (p)
     __bs5FILE *p;
{
  char *c,
    c1,
    d;
  long i,
    l;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Inimage: IMAGE equals notext");
  if (((__bs2FILE *) p)->IMAGE.obj->h.konstant)
    __rerror ("Inimage: IMAGE is a constant text object");
  if (p->writeonly)
    __rerror ("Inimage: Writeonly file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Inimage: Illegal length of IMAGE");
  ((__bs2FILE *) p)->IMAGE.pos = 1;

  c = &((__bs2FILE *) p)->IMAGE.obj->string[((__bs2FILE *) p)
					    ->IMAGE.start - 1];
  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;

  if (((__bs5FILE *) p)->endfile = (((__bs5FILE *) p)->loc > __rdlastloc (p)))
    {
      *(c++) = 25;
      for (i = 2; i <= l; i++)
	*(c++) = ' ';
    }
  else
    {
      if (p->lastop == __WRITE 
	  && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
	__rerror ("Inimage: Not possible to seek");
      p->lastop = __READ;
      d = '\0';
      for (i = 1; i <= l; i++)
	{
	  if ((c1 = getc (f)) == __EOF)
	    {
	      __rerror ("Inimage: Read error");
	      break;
	    }

	  if ((*(c++) = c1) != '\0')
	    d = '\n';
	}
      if ((c1 = getc (f)) != '\n' && c1 != d)
	__rerror ("Inimage: Read error");
      p->loc++;
    }
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTIMAGE                  */

#if nolib|rdoutimagerut
__dhp 
__rdoutimage (p)
     __bs5FILE *p;
{
  char *c;
  long i,
    l;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Outimage: IMAGE equals notext");
  if (p->loc > ((__bs5FILE *) p)->maxloc)
    __rerror ("Outimage: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Outimage: Append underflow or read-only file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Outimage: Illegal length of image");
  if (p->lastop == __READ && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __WRITE;

  c = &((__bs2FILE *) p)->IMAGE.obj->string[((__bs2FILE *) p)
					    ->IMAGE.start - 1];
  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;
  for (i = 1; i <= l; i++)
    {
      if (putc (*c, f) == __EOF)
	{
	  __rerror ("Outimage: Write error");
	  break;
	}
      *(c++) = ' ';
    }
  if (putc ('\n', f) == __EOF)
    __rerror ("Outimage: Write error");

  p->loc++;
  ((__bs2FILE *) p)->IMAGE.pos = 1;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE DELETEIMAGE       */

#if nolib|rddeleteimagerut
char 
__rddeleteimage (p)
     __bs5FILE *p;
{
  long c1,
    i,
    l,
    last;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Deleteimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Deleteimage: IMAGE equals notext");
  if (p->loc > ((__bs5FILE *) p)->maxloc)
    __rerror ("Deleteimage: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Deleteimage: Append underflow or read-only file");
  if (((__bs5FILE *) p)->imagelength != ((__bs2FILE *) p)->IMAGE.length)
    __rerror ("Deleteimage: Illegal length of image");
  if (p->loc > (last = __rdlastloc (p)))
    return (__FALSE);
  if (p->lastop == __READ && fseek (((__bs1FILE *) p)->file, 0L, 1) == __EOF)
    __rerror ("Deleteimage: Not possible to seek");
  p->lastop = __WRITE;

  l = ((__bs5FILE *) p)->imagelength;
  f = ((__bs1FILE *) p)->file;
  for (i = 1; i <= l; i++)
    {
      if (putc ('\0', f) == __EOF)
	{
	  __rerror ("Deleteimage: Write error");
	  break;
	}
    }
  if (putc ('\0', f) == __EOF)
    __rerror ("Deleteimage: Write error");

  ((__bs2FILE *) p)->IMAGE.pos = 1;
  if (p->loc == last)
    {
      for (i = p->loc - 1; i > 0; i--)
	{
	  if (fseek (((__bs1FILE *) p)->file, 
		     (((__bs5FILE *) p)->imagelength + 1) * i - 1, 0) == __EOF)
	    __rerror ("Deleteimage: Not possible to seek");
	  if ((c1 = getc (f)) == __EOF)
	    __rerror ("Deleteimage: Read error");
	  if (c1 != '\0')
	    break;
	}
      (void) fflush (f);
#if F_TRUNCATE
#ifdef mips			/* Just to satisfy lint */
      (void) ftruncate ((int) fileno (f), (int) ((i) * (l + 1)));
#else
      (void) ftruncate ((int) fileno (f), ((i) * (l + 1)));
#endif
#endif
      if (i != p->loc - 1)
	{
	  if (fseek (((__bs1FILE *) p)->file, 
		     (((__bs5FILE *) p)->imagelength + 1) * p->loc,
		     0) == __EOF)
	    __rerror ("Deleteimage: Not possible to seek");
	  p->lastop = __SEEK;
	}
    }
  p->loc++;
  return (__TRUE);
}
#endif

/* LASTITEM, INCHAR, INTEXT, ININT, INREAL og INFRAC er lik de i INFILE
 * OUTCHAR, OUTTEXT, OUTINT, OUTREAL, OUTFRAC og OUTFIX er lik de i OUTFILE */

/* Run time rutiner for klassen INFILE */

FILE *fopen ();

/******************************************************************************
                                         BOOLEAN PROCEDURE ENDFILE           */

#if nolib|riendfilerut
char 
__riendfile (p)
     __bs4FILE *p;
{
  return (p->endfile);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|riopenrut
char 
__riopen (p, t)
     __bs1FILE *p;
     __txtvp t;
{
  long i;
  char *s;

  if (!p->open)
    {
      /* Trenger ikke ha med p->filename.start fordi start=1 siden */
      /* filename er overf|rt by value                             */

      ((__bs1FILE *) p)->file = fopen (p->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs4FILE *) p)->endfile = __FALSE;
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = t->length + 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      s = ((__bs2FILE *) p)->IMAGE.obj->string;
      for (i = 0; i < t->length; i++)
	s[t->start + i - 1] = ' ';
      return ((p->open = __TRUE));
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|ricloserut
char 
__riclose (p)
     __bs1FILE *p;
{
  if (p->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND 
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      fclose (((__bs1FILE *) p)->file);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (p->filename.obj->string);
#endif
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      p->open = __FALSE;
      return ((((__bs4FILE *) p)->endfile = __TRUE));
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                                 PROCEDURE INIMAGE           */

#if nolib|riinimagerut
__dhp 
__riinimage (p)
     __bs2FILE *p;
{
  long i,
    j;
  FILE *f;
  char *s;
  long c;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inimage: File not open");
  if (((__bs4FILE *) p)->endfile)
    __rerror ("Inimage: End of file");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Inimage: IMAGE equals notext");
  if (((__bs2FILE *) p)->IMAGE.obj->h.konstant)
    __rerror ("Inimage: Constant text object");

  f = ((__bs1FILE *) p)->file;
  s = p->IMAGE.obj->string;
  i = p->IMAGE.start - 1;
  j = i + p->IMAGE.length;

  while ((c = getc (f)) != __EOF)
    {
      if (c == '\n')
	goto utinimage;
      if (i == j)
	goto inimageerror;
      s[i++] = c;
    }

  if (i == p->IMAGE.start - 1)
    {
      ((__bs4FILE *) p)->endfile = __TRUE;
      s[i++] = 25;
      for (; i < j; i++)
	s[i] = ' ';
      p->IMAGE.pos = 1;
      return ((__dhp) p);
    }
  else
    (void) ungetc ((int) c, f);

utinimage:
  while (i != j)
    s[i++] = ' ';
  p->IMAGE.pos = 1;
  return ((__dhp) p);
inimageerror:
  __rerror ("Inimage: IMAGE to short");
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE INRECORD          */

#if nolib|riinrecordrut
char 
__riinrecord (p)
     __bs2FILE *p;
{
  long i,
    j;
  FILE *f;
  char *s;
  long c;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inrecord: File not open");
  if (((__bs4FILE *) p)->endfile)
    __rerror ("Inrecord: End of file");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Inrecord: IMAGE equals notext");

  f = ((__bs1FILE *) p)->file;
  s = p->IMAGE.obj->string;
  i = p->IMAGE.start - 1;
  j = i + p->IMAGE.length;

  while ((c = getc (f)) != __EOF)
    {
      if (c == '\n')
	goto utinrecord;
      if (i == j)
	{
	  (void) ungetc ((int) c, f);
	  goto inrecordfull;
	}
      s[i++] = c;
    }

  if (i == p->IMAGE.start - 1)
    {
      ((__bs4FILE *) p)->endfile = __TRUE;
      s[i] = 25;
      p->IMAGE.pos = 2;
      return (__FALSE);
    }
  else
    (void) ungetc ((int) c, f);

utinrecord:
  p->IMAGE.pos = i - p->IMAGE.start + 2;
  return (__FALSE);
inrecordfull:
  p->IMAGE.pos = p->IMAGE.length + 1;
  return (__TRUE);
}
#endif

/******************************************************************************
                                         CHARACTER PROCEDURE INCHAR          */

#if nolib|riincharrut
char 
__riinchar (p)
     __bs2FILE *p;
{
  if (!__rtmore (&(p->IMAGE)))
    {				/* Sorger for at riktige inimage kalles */
      if (((__bs1FILE *) p)->h.pp == &__p5FILE)
	__rdinimage ((__bs5FILE *) p);
      else
	__riinimage (p);
    }
  return (p->IMAGE.obj->string[p->IMAGE.start + (p->IMAGE.pos++) - 2]);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE LASTITEM          */

#if nolib|rilastitemrut
char 
__rilastitem (p)
     __bs2FILE *p;
{
  char c;
  if (p->IMAGE.obj == __NULL)
    return (__TRUE);
  c = ' ';
  while (!(((__bs1FILE *) p)->h.pp == &__p5FILE
	   ? ((__bs5FILE *) p)->endfile : ((__bs4FILE *) p)->endfile)
	 && (c == ' ' || c == 9))
    c = __riinchar (p);

  if (c != ' ')
    p->IMAGE.pos--;

  return (((__bs1FILE *) p)->h.pp == &__p5FILE
	  ? ((__bs5FILE *) p)->endfile : ((__bs4FILE *) p)->endfile);
}
#endif

/******************************************************************************
                                          TEXT PROCEDURE INTEXT              */

#if nolib|riintextrut
__txtvp 
__riintext (as, p, w)
     __bs2FILE *p;
     long as,
       w;
{
  long i;
  __er = (__dhp) p;
  (void) __rblanks (as, w);
  for (i = 0; i < w; i++)
    __et.obj->string[i] = __riinchar (__er);
  return (&__et);
}
#endif

/******************************************************************************
                                           INTEGER PROCEDURE ININT           */

#if nolib|riinintrut
long 
__riinint (p)
     __bs2FILE *p;
{
  __txt t;
  long inint;
  if (__rilastitem (p))
    __rerror ("Inint: End of file");
  t.obj = p->IMAGE.obj;
  t.start = p->IMAGE.start + p->IMAGE.pos - 1;
  t.length = p->IMAGE.length - p->IMAGE.pos + 1;
  t.pos = 1;
  inint = __rtgetint (&t);
  p->IMAGE.pos += t.pos - 1;
  return (inint);
}
#endif

/******************************************************************************
                                           REAL PROCEDURE INREAL             */

#if nolib|riinrealrut
#if FLOAT_IMPLEMENTED
double 
__riinreal (p)
     __bs2FILE *p;
{
  __txt t;
  double inreal;
  if (__rilastitem (p))
    __rerror ("Inreal: End of file");
  t.obj = p->IMAGE.obj;
  t.start = p->IMAGE.start + p->IMAGE.pos - 1;
  t.length = p->IMAGE.length - p->IMAGE.pos + 1;
  t.pos = 1;
  inreal = __rtgetreal (&t);
  p->IMAGE.pos += t.pos - 1;
  return (inreal);
}
#endif
#endif

/******************************************************************************
                                           INTEGER PROCEDURE INFRAC          */

#if nolib|riinfracrut
long 
__riinfrac (p)
     __bs2FILE *p;
{
  __txt t;
  long infrac;
  if (__rilastitem (p))
    __rerror ("Infrac: End of file");
  t.obj = p->IMAGE.obj;
  t.start = p->IMAGE.start + p->IMAGE.pos - 1;
  t.length = p->IMAGE.length - p->IMAGE.pos + 1;
  t.pos = 1;
  infrac = __rtgetfrac (&t);
  p->IMAGE.pos += t.pos - 1;
  return (infrac);
}
#endif

/* RUNTIME rutiner for klassen outfile  skrevet i c. */

FILE *fopen ();

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|roopenrut
char 
__roopen (p, t)
     __bs1FILE *p;
     __txtvp t;
{
  if (!p->open)
    {
      /* Trenger ikke ha med p->filename.start fordi start=1 siden */
      /* filename er overf|rt by value                             */

      if (((__bs1FILE *) p)->create != __ANYCREATE)
	{
	  ((__bs1FILE *) p)->file = fopen (p->filename.obj->string, "r");
	  if (((__bs1FILE *) p)->file != __NULL)
	    (void) fclose (((__bs1FILE *) p)->file);
	  if ((((__bs1FILE *) p)->file == __NULL 
	       && ((__bs1FILE *) p)->create == __NOCREATE) ||
	      (((__bs1FILE *) p)->file != __NULL 
	       && ((__bs1FILE *) p)->create == __CREATE))
	    return (__FALSE);
	}
      if (((__bs1FILE *) p)->append == __APPEND)
	((__bs1FILE *) p)->file = fopen (p->filename.obj->string, "a");
      else
	((__bs1FILE *) p)->file = fopen (p->filename.obj->string, "w");

      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);

      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      return ((long) (p->open = __TRUE));
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|rocloserut
char 
__roclose (p)
     __bs1FILE *p;
{
  if (p->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND 
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      if (((__bs2FILE *) p)->IMAGE.pos > 1)
	__rooutimage ((__bs2FILE *) p);
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (p->filename.obj->string);
#endif
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      p->open = __FALSE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTIMAGE                  */

#if nolib|rooutimagerut
__dhp 
__rooutimage (p)
     __bs2FILE *p;		/* Skriver ikke ut etterfolgende blanke */
{
  char *s;
  long i;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outimage: File not open");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Outimage: IMAGE equals notext");
  (void) __rtstrip (&((__bs2FILE *) p)->IMAGE);		/* Lengden av teksten 
							 * uten etterfolgende 
							 * blanke  */
  /* ligger i den globale variabelen et.length og  */
  /* start som ikke er forandret) ligger i et.start */
  s = p->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  for (i = 0; i < __et.length; i++)
    {
      (void) putc (s[__et.start + i - 1], f);
      s[__et.start + i - 1] = ' ';
    }
  (void) putc ('\n', f);
  (void) fflush (f);
  p->IMAGE.pos = 1;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                        PROCEDURE OUTRECORD                  */

#if nolib|rooutrecordrut
__dhp 
__rooutrecord (p)
     __bs2FILE *p;
{
  char *s;
  register long i,
    j,
    k;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outrecord: File not open");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Outrecord: IMAGE equals notext");
  s = p->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  j = p->IMAGE.pos - 1;
  k = p->IMAGE.start - 1;
  for (i = 0; i < j; i++)
    (void) putc (s[k + i], f);
  (void) putc ('\n', f);
  p->IMAGE.pos = 1;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE BREAKOUTIMAGE             */

#if nolib|robreakoutimagerut
__dhp 
__robreakoutimage (p)
     __bs2FILE *p;
{
  char *s;
  register long i,
    j,
    k;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Breakoutimage: File not open");
  if (p->IMAGE.obj == __NULL)
    __rerror ("Breakoutimage: IMAGE equals notext");
  s = p->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  j = p->IMAGE.pos - 1;
  k = p->IMAGE.start - 1;
  for (i = 0; i < j; i++)
    (void) putc (s[k + i], f);
  (void) fflush (f);
  j = p->IMAGE.length;
  for (i = 0; i < j; i++)
    s[k + i] = ' ';

  p->IMAGE.pos = 1;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTCHAR(c)                */

#if nolib|rooutcharrut
__dhp 
__rooutchar (p, c)
     __bs2FILE *p;
     char c;
{
  if (!__rtmore (&(p->IMAGE)))
    {				/* Sorger for at riktige outimage kalles */
      if (((__bs1FILE *) p)->h.pp == &__p6FILE)
	__rpoutimage (((__bs6FILE *) p));
      else if (((__bs1FILE *) p)->h.pp == &__p5FILE)
	__rdoutimage ((__bs5FILE *) p);
      else
	__rooutimage (p);
    }
  __rtputchar (&(p->IMAGE), c);
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTTEXT(t)                */

#if nolib|roouttextrut
__dhp 
__roouttext (p, t)
     __bs2FILE *p;
     __txtvp t;
{
  register long i;
  char *s;
  if (p->IMAGE.pos > 1 && t->length > (p->IMAGE.length - p->IMAGE.pos + 1))
    {				/* Sorger for at riktige outimage kalles */
      if (((__bs1FILE *) p)->h.pp == &__p6FILE)
	__rpoutimage (((__bs6FILE *) p));
      else if (((__bs1FILE *) p)->h.pp == &__p5FILE)
	__rdoutimage ((__bs5FILE *) p);
      else
	__rooutimage (p);
    }
  s = t->obj->string;
  for (i = 0; i < t->length; i++)
    __rooutchar (p, s[t->start + i - 1]);
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTINT(i,w)              */

#if nolib|rooutintrut
__dhp 
__rooutint (p, i, w)
     __bs2FILE *p;
     long i,
       w;
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputint */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputint (&tv, i);
      __rfield (((__bs2FILE *) p), __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k] 
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (((__bs2FILE *) p), -w);		/* Returverdier i et */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputint (&ti, i);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (((__bs2FILE *) p), w);	/* Returverdier i et */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputint (&ti, i);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                        PROCEDURE OUTFIX(r,i,w)              */

#if nolib|rooutfixrut
#if FLOAT_IMPLEMENTED
__dhp 
__rooutfix (p, r, i, w)
     __bs2FILE *p;
     double r;
     long i,
       w;
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputfix */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputfix (&tv, r, i);
      __rfield (p, __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k] 
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (p, -w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputfix (&ti, r, i);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (p, w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputfix (&ti, r, i);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
#endif
#endif

/******************************************************************************
                                       PROCEDURE OUTREAL(r,i,w)              */

#if nolib|rooutrealrut
#if FLOAT_IMPLEMENTED
__dhp 
__rooutreal (p, r, i, w)
     __bs2FILE *p;
     double r;
     long i,
       w;
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputreal */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputreal (&tv, r, i);
      __rfield (p, __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k] 
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (p, -w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputreal (&ti, r, i);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (p, w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputreal (&ti, r, i);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
#endif
#endif

/******************************************************************************
                                       PROCEDURE OUTFRAC(i,n,w)              */

#if nolib|rooutfracrut
__dhp 
__rooutfrac (p, i, n, w)
     __bs2FILE *p;
     long i;
     long n,
       w;
{
  __txt ti;
  long k;
  if (w == 0)
    {
      /* Bruker ikke IMAGE, men oppretter et midlertidig tekstobjekt  og
       * sender den som parameter til __rtputfrac */
      static char th[sizeof (__th) + __RTPUTTEXTLENGTH + 1];
      __tobj *tob = (__tobj *) th;
      __txt tv;
      tob->h.konstant = 0;
      tv.obj = tob;
      tv.length = __RTPUTTEXTLENGTH;
      tv.pos = 1;
      tv.start = 1;
      __rtputfrac (&tv, i, n);
      __rfield (p, __rputlen);
      p->IMAGE.pos += __rputlen;
      for (k = 0; k < __rputlen; k++)
	__et.obj->string[__et.start - 1 + k]
	  = tob->string[__RTPUTTEXTLENGTH - __rputlen + k];
    }
  else if (w < 0)
    {
      __rfield (p, -w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      __rtputfrac (&ti, i, n);
      __rleftshift (&ti, -w - __rputlen);
      p->IMAGE.pos -= w;
    }
  else
    {
      __rfield (p, w);		/* Returverdier i __et. */
      ti.obj = __et.obj;
      ti.length = __et.length;
      ti.pos = __et.pos;
      ti.start = __et.start;
      (void) __rtputfrac (&ti, i, n);
      p->IMAGE.pos += w;
    }
  return ((__dhp) p);
}
#endif

/* CHECKPOINT er lik den i DIRECTFILE */

/* RUNTIME rutiner for klassen printfile  skrevet i c. */

/******************************************************************************
                                         INTEGER PROCEDURE LINE              */

#if nolib|rplinerut
long 
__rpline (p)
     __bs6FILE *p;
{
  return (p->line);
}
#endif
/******************************************************************************
                                         INTEGER PROCEDURE PAGE              */

#if nolib|rppagerut
long 
__rppage (p)
     __bs6FILE *p;
{
  return (p->page);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|rpopenrut
char 
__rpopen (p, t)
     __bs6FILE *p;
     __txtvp t;
{
  if (!((__bs1FILE *) p)->open)
    {
      /* Trenger ikke ha med p->filename.start fordi start=1 siden */
      /* filename er overf|rt by value                             */

      if (((__bs1FILE *) p)->create != __ANYCREATE)
	{
	  ((__bs1FILE *) p)->file 
	    = fopen (((__bs1FILE *) p)->filename.obj->string, "r");
	  if (((__bs1FILE *) p)->file != __NULL)
	    (void) fclose (((__bs1FILE *) p)->file);
	  if ((((__bs1FILE *) p)->file == __NULL 
	       && ((__bs1FILE *) p)->create == __NOCREATE) ||
	      (((__bs1FILE *) p)->file != __NULL 
	       && ((__bs1FILE *) p)->create == __CREATE))
	    return (__FALSE);
	}
      if (((__bs1FILE *) p)->append == __APPEND)
	((__bs1FILE *) p)->file 
	  = fopen (((__bs1FILE *) p)->filename.obj->string, "a");
      else
	((__bs1FILE *) p)->file 
	  = fopen (((__bs1FILE *) p)->filename.obj->string, "w");

      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs2FILE *) p)->IMAGE.obj = t->obj;
      ((__bs2FILE *) p)->IMAGE.length = t->length;
      ((__bs2FILE *) p)->IMAGE.pos = 1;
      ((__bs2FILE *) p)->IMAGE.start = t->start;
      p->page = 1;
      p->line = 1;
      ((__bs1FILE *) p)->open = __TRUE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|rpcloserut
char 
__rpclose (p)
     __bs6FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      if (((__bs2FILE *) p)->IMAGE.pos > 1)
	__rpoutimage (p);
      if (p->lines_per_page != MAX_INT)
	__rpeject (((__bs6FILE *) p), p->lines_per_page);
      p->line = 0;
      p->spacing = 1;
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs2FILE *) p)->IMAGE.obj = __NULL;
      ((__bs2FILE *) p)->IMAGE.length = 0;
      ((__bs2FILE *) p)->IMAGE.pos = 0;
      ((__bs2FILE *) p)->IMAGE.start = 0;
      ((__bs1FILE *) p)->open = __FALSE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         INTEGER PROCEDURE LINESPERPAGE      */

#if nolib|rplinesperpagerut
long 
__rplinesperpage (p, n)
     __bs6FILE *p;
     long n;
{
  long i;
  i = p->lines_per_page;
  p->lines_per_page = (n > 0) ? n : (n < 0) ? MAX_INT : LINES_PER_PAGE;
  return (i);
}
#endif

/******************************************************************************
                                         PROCEDURE SPACING                   */

#if nolib|rpspacingrut
__dhp 
__rpspacing (p, n)
     __bs6FILE *p;
     long n;
{
  if (0 <= n && n <= p->lines_per_page)
    p->spacing = n;
  else
    __rerror ("Spacing: Parameter out of range");
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                           PROCEDURE EJECT                   */

/* N}r det skal lages et bibliotek kompileres denne rutinen alltid sammen
 * med den neste da de avhenger av hverandre */

#if nolib|rpejectrut
__dhp 
__rpeject (p, n)
     __bs6FILE *p;
     long n;
{
  long i;

  if (!((__bs1FILE *) p)->open)
    __rerror ("Eject: File not open");
  if (n <= 0)
    __rerror ("Eject: Parameter out of range");
  if (n > p->lines_per_page)
    n = 1;

  if (n <= p->line)
    {
      (void) putc ('\f', ((__bs1FILE *) p)->file);
      p->line = 1;
      p->page += 1;
    }
  for (i = p->line; i < n; i++)
    (void) putc ('\n', ((__bs1FILE *) p)->file);
  p->line = n;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTIMAGE                  */

#if nolib|rpoutimagerut
__dhp 
__rpoutimage (p)
     __bs6FILE *p;		/* Skriver ikke ut etterfolgende blanke */
{
  char *s;
  long i;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outimage: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Outimage: IMAGE equals notext");
  if (p->line > p->lines_per_page)
    __rpeject (p, 1L);

  (void) __rtstrip (&((__bs2FILE *) p)->IMAGE);		/* Lengden av teksten 
							 * uten etterfolgende 
							 * blanke ligger i
							 * den globale
							 * variabelen
							 * __et.length og
							 * start(som ikke er
							 * forandret) ligger
							 * i __et.start */
  s = ((__bs2FILE *) p)->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  for (i = 0; i < __et.length; i++)
    {
      (void) putc (s[__et.start + i - 1], f);
      s[__et.start + i - 1] = ' ';
    }
  for (i = 1; i <= p->spacing; i++)
    {
      (void) putc ('\n', f);
      p->line++;
      if (p->line > p->lines_per_page)
	break;
    }
  if (p->spacing == 0)
    (void) putc ('\r', f);

  ((__bs2FILE *) p)->IMAGE.pos = 1;
#if STDOUT_FLUSH
  if (f == stdout)
    fflush (stdout);
#endif
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                        PROCEDURE OUTRECORD                  */

#if nolib|rpoutrecordrut
__dhp 
__rpoutrecord (p)
     __bs6FILE *p;
{
  char *s;
  register long i,
    j,
    k;
  FILE *f;
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outrecord: File not open");
  if (((__bs2FILE *) p)->IMAGE.obj == __NULL)
    __rerror ("Outrecord: IMAGE equals notext");
  if (p->line > p->lines_per_page)
    __rpeject (p, 1L);
  s = ((__bs2FILE *) p)->IMAGE.obj->string;
  f = ((__bs1FILE *) p)->file;
  j = ((__bs2FILE *) p)->IMAGE.pos - 1;
  k = ((__bs2FILE *) p)->IMAGE.start - 1;
  for (i = 0; i < j; i++)
    (void) putc (s[k + i], f);
  for (i = 1; i <= p->spacing; i++)
    {
      (void) putc ('\n', f);
      p->line++;
      if (p->line > p->lines_per_page)
	break;
    }
  if (p->spacing == 0)
    (void) putc ('\r', f);
  p->line += p->spacing;
  ((__bs2FILE *) p)->IMAGE.pos = 1;
  return ((__dhp) p);
}
#endif

/* Bytefile */

/******************************************************************************
                                         INTEGER PROCEDURE BYTESIZE          */

#if nolib|rbytesizerut
long 
__rbytesize (p)
     __bs7FILE *p;
{
  return (p->bytesize);
}
#endif

/* Inbytefile */

/******************************************************************************
                                         BOOLEAN PROCEDURE ENDFILE           */

#if nolib|ribendfilerut
char 
__ribendfile (p)
     __bs8FILE *p;
{
  return (((__bs7FILE *) p)->endfile);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|ribopenrut
char 
__ribopen (p)
     __bs8FILE *p;
{
  if (!((__bs1FILE *) p)->open)
    {
      ((__bs1FILE *) p)->file 
	= fopen (((__bs1FILE *) p)->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      ((__bs7FILE *) p)->endfile = __FALSE;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|ribcloserut
char 
__ribclose (p)
     __bs8FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND 
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      fclose (((__bs1FILE *) p)->file);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs1FILE *) p)->open = __FALSE;
      return (((__bs7FILE *) p)->endfile = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         (SHORT) INTEGER PROCEDURE INBYTE    */

#if nolib|ribinbyterut
long 
__ribinbyte (p)
     __bs8FILE *p;
{
  long c;
  if (((__bs7FILE *) p)->endfile == __TRUE)
    __rerror ("Inbyte: End of file");
  if ((c = getc (((__bs1FILE *) p)->file)) == __EOF)
    {
      ((__bs7FILE *) p)->endfile = __TRUE;
      return (0);
    }
  else
    return (c);
}
#endif

/******************************************************************************
                                         TEXT PROCEDURE INTEXT(t)            */

#if nolib|ribintextrut
__txtvp
__ribintext (p, t)
     __bs8FILE *p;
     __txtvp t;
{
  __rtsetpos (t, 1L);
  while (__rtmore (t) & !((__bs7FILE *) p)->endfile)
    __rtputchar (t, (char) __ribinbyte (p));
  if (((__bs7FILE *) p)->endfile)
    __rtsetpos (t, __rtpos (t) - 1);
  return (__rtsub (t, 1L, __rtpos (t) - 1));
}
#endif

/* Outbytefile */

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|robopenrut
char 
__robopen (p)
     __bs9FILE *p;
{
  if (!((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->create != __ANYCREATE)
	{
	  ((__bs1FILE *) p)->file =
	    fopen (((__bs1FILE *) p)->filename.obj->string, "r");
	  if (((__bs1FILE *) p)->file != NULL)
	    (void) fclose (((__bs1FILE *) p)->file);
	  if (((__bs1FILE *) p)->file == __NULL
	      && ((__bs1FILE *) p)->create == __NOCREATE ||
	      ((__bs1FILE *) p)->file != __NULL
	      && ((__bs1FILE *) p)->create == __CREATE)
	    return (__FALSE);
	}
      ((__bs1FILE *) p)->file =
	fopen (((__bs1FILE *) p)->filename.obj->string, 
	       ((__bs1FILE *) p)->append == __APPEND ? "a" : "w");
      if (((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|robcloserut
char 
__robclose (p)
     __bs9FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      if (((__bs1FILE *) p)->re_wind == __REWIND
	  && fseek (((__bs1FILE *) p)->file, 0L, 0) == __EOF)
	__rerror ("Close: Not possible to rewind");
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs1FILE *) p)->open = __FALSE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
#endif
/******************************************************************************
                                         PROCEDURE OUTBYTE(x)                */

#if nolib|roboutbyterut
__dhp 
__roboutbyte (p, x)
     __bs9FILE *p;
     long x;
{
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outbyte: File closed");
  if ((x < 0) | (x >= 256))
    __rerror ("Outbyte: Illegal byte value");
  (void) putc ((int) x, ((__bs1FILE *) p)->file);
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTTEXT(t)                */

#if nolib|robouttextrut
__dhp 
__robouttext (p, t)
     __bs9FILE *p;
     __txtvp t;
{
  __rtsetpos (t, 1L);
  while (__rtmore (t))
    __roboutbyte (p, (long) __rtgetchar (t));
  return ((__dhp) p);
}
#endif

/* CHECKPOINT er lik den i DIRECTFILE */

/* Directbytefile */

/******************************************************************************
                                         BOOLEAN PROCEDURE ENDFILE           */

#if nolib|rdbendfilerut
char 
__rdbendfile (p)
     __bs10FILE *p;
{
  return (((__bs1FILE *) p)->open && p->loc > __rdblastloc (p));
}
#endif

/******************************************************************************
                                         INTEGER PROCEDURE LOCATION          */

#if nolib|rdblocationrut
long 
__rdblocation (p)
     __bs10FILE *p;
{
  return (p->loc);
}
#endif

/******************************************************************************
                                           INTEGER PROCEDURE MAXLOC          */

#if nolib|rdbmaxlocrut
long 
__rdbmaxloc (p)
     __bs10FILE *p;
{
  return (p->maxloc);
}
#endif

/******************************************************************************
                                           BOOLEAN PROCEDURE LOCKED          */

#if nolib|rdblockedrut
char 
__rdblocked (p)
     __bs10FILE *p;
{
  return (p->locked);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE UNLOCK            */

#if nolib|rdbunlockrut
char 
__rdbunlock (p)
     __bs10FILE *p;
{
  char c;
  c = __rdcheckpoint (((__bs5FILE *) p));
  if (p->locked)
    {
      /* Release file */
      p->locked = __FALSE;
    }
  return (c);
}
#endif

/******************************************************************************
                                         PROCEDURE LOCK(t,i,j)               */

#if nolib|rdblockrut
/*ARGSUSED */
#if FLOAT_IMPLEMENTED
long 
__rdblock (p, t, i, j)
     __bs10FILE *p;
     double t;
     long i,
       j;
{
  long lock = -1;
  if (t > 0.0)
    {
      if (p->locked)
	(void) __rdbunlock (p);
    }
  return (lock);
}
#endif
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE OPEN              */

#if nolib|rdbopenrut
char 
__rdbopen (p)
     __bs10FILE *p;
{
  if (!((__bs1FILE *) p)->open)
    {
      ((__bs1FILE *) p)->file 
	= fopen (((__bs1FILE *) p)->filename.obj->string, "r");
      if (((__bs1FILE *) p)->file != NULL)
	(void) fclose (((__bs1FILE *) p)->file);
      if (((__bs1FILE *) p)->create == __CREATE 
	  && ((__bs1FILE *) p)->file != __NULL)
	return (__FALSE);
      if (((__bs1FILE *) p)->create == __NOCREATE 
	  && ((__bs1FILE *) p)->file == __NULL)
	return (__FALSE);
#if VMS_DIRECTFILE
      if (((__bs1FILE *) p)->file == __NULL)
	{
	  if ((((__bs1FILE *) p)->file 
	       = fopen (((__bs1FILE *) p)->filename.obj->string
			,"w")) == __NULL)
	    return (__FALSE);
	  (void) fclose (((__bs1FILE *) p)->file);
	}
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1 *) p)->filename.obj->string
		    ,"r+")) == __NULL)
	return (__FALSE);
#else
      if ((((__bs1FILE *) p)->file 
	   = fopen (((__bs1FILE *) p)->filename.obj->string
		    ,((__bs1FILE *) p)->file == __NULL ? "w+" :
		    ((__bs1FILE *) p)->readwrite == __READONLY 
		    ? "r" : "r+")) == __NULL)
	return (__FALSE);
#endif
      if (((__bs1FILE *) p)->append == __APPEND)
	{
	  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
	    {
	      (void) fclose (((__bs1FILE *) p)->file);
	      return (__FALSE);
	    }
	  p->loc = p->minwriteloc = ftell (((__bs1FILE *) p)->file);
	}
      else
	p->loc = p->minwriteloc = 1;
      p->lastop = __SEEK;
      p->maxloc = MAX_INT - 1;
      if (((__bs1FILE *) p)->readwrite == __READONLY)
	p->minwriteloc = MAX_INT;
      if (((__bs1FILE *) p)->readwrite == __WRITEONLY)
	p->writeonly = __TRUE;
      return (((__bs1FILE *) p)->open = __TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         BOOLEAN PROCEDURE CLOSE             */

#if nolib|rdbcloserut
char 
__rdbclose (p)
     __bs10FILE *p;
{
  if (((__bs1FILE *) p)->open)
    {
      if (p->locked)
	(void) __rdbunlock (p);
      if (fclose (((__bs1FILE *) p)->file) == __EOF)
	return (__FALSE);
#if HAVE_UNLINK
      if (((__bs1FILE *) p)->purge == __PURGE)
	(void) unlink (((__bs1FILE *) p)->filename.obj->string);
#endif
      ((__bs1FILE *) p)->open = __FALSE;
      return (__TRUE);
    }
  else
    return (__FALSE);
}
#endif

/******************************************************************************
                                         INTEGER PROCEDURE LASTLOC           */

#if nolib|rdblastlocrut
long 
__rdblastloc (p)
     __bs10FILE *p;
{
  long i;
  if (fseek (((__bs1FILE *) p)->file, 0L, 2) == __EOF)
    __rerror ("Lastloc: Not possible to seek");
  i = ftell (((__bs1FILE *) p)->file);
  if (fseek (((__bs1FILE *) p)->file, p->loc-1, 0) == __EOF)
    __rerror ("Lastloc: Not possible to seek");
  p->lastop = __SEEK;
  return (i);
}
#endif

/******************************************************************************
                                         PROCEDURE LOCATE(i)                 */

#if nolib|rdblocaterut
__dhp 
__rdblocate (p, i)
     __bs10FILE *p;
     long i;
{
  if ((i < 1) | (i > p->maxloc))
    __rerror ("Locate: Parameter out of range");
  if (p->loc != i)
    {
      p->loc = i;
      if (fseek (((__bs1FILE *) p)->file, i - 1, 0) == __EOF)
	__rerror ("Locate: Not possible to seek");
      p->lastop = __SEEK;
    }
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         (SHORT) INTEGER PROCEDURE INBYTE    */

#if nolib|rdbinbyterut
long 
__rdbinbyte (p)
     __bs10FILE *p;
{
  long c;
  if (p->writeonly)
    __rerror ("Inbyte: Writeonly file");
  if (!((__bs1FILE *) p)->open)
    __rerror ("Inbyte: File closed");
  if (p->lastop == __WRITE 
      && fseek (((__bs1FILE *) p)->file, p->loc - 1, 0) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __READ;
  p->loc++;
  if ((c = getc (((__bs1FILE *) p)->file)) == __EOF)
    return (0);
  else
    return (c);
}
#endif

/******************************************************************************
                                         PROCEDURE OUTBYTE(x)                */

#if nolib|rdboutbyterut
__dhp 
__rdboutbyte (p, x)
     __bs10FILE *p;
     long x;
{
  if (!((__bs1FILE *) p)->open)
    __rerror ("Outbyte: File closed");
  if ((x < 0) | (x >= 256))
    __rerror ("Outbyte: Illegal byte value");
  if (p->loc > p->maxloc)
    __rerror ("Outbyte: File overflow");
  if (p->loc < p->minwriteloc)
    __rerror ("Outbyte: Append underflow or read-only file");
  if (p->lastop == __READ 
      && fseek (((__bs1FILE *) p)->file, p->loc - 1, 0) == __EOF)
    __rerror ("Outbyte: Not possible to seek");
  p->lastop = __WRITE;
  (void) putc ((char) x, ((__bs1FILE *) p)->file);
  p->loc++;
  return ((__dhp) p);
}
#endif

/******************************************************************************
                                         TEXT PROCEDURE INTEXT(t)            */

#if nolib|rdbintextrut
__txtvp
__rdbintext (p, t)
     __bs10FILE *p;
     __txtvp t;
{
  __rtsetpos (t, 1L);
  while (__rtmore (t) & !((__bs7FILE *) p)->endfile)
    __rtputchar (t, (char) __rdbinbyte (p));
  if (((__bs7FILE *) p)->endfile)
    __rtsetpos (t, __rtpos (t) - 1);
  return(__rtsub (t, 1L, __rtpos (t) - 1));
}
#endif

/******************************************************************************
                                         PROCEDURE OUTTEXT(t)                */

#if nolib|rdbouttextrut
__dhp 
__rdbouttext (p, t)
     __bs10FILE *p;
     __txtvp t;
{
  __rtsetpos (t, 1L);
  while (__rtmore (t))
    __rdboutbyte (p, (long) __rtgetchar (t));
  return ((__dhp) p);
}
#endif

/* CHECKPOINT er lik den i DIRECTFILE */
