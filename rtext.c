/* $Id: rtext.c,v 1.9 1994/10/25 16:32:40 cim Exp $ */

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

/* RUNTIME rutiner for text skrevet i c. */

#include <stdio.h>
#include <math.h>

#include "cim.h"
#include "limit.h"

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#define digit (s[i]>='0' && s[i]<='9')
#define more (i<j)
#define fortegn ((s[i]=='-')?-1:(s[i]=='+')?1:0)
#define skipblanke for(;i<j && (s[i]==' ' || s[i]=='\t');i++);

#if DAD_LIB
#if nolib|rdadrut
#include "dad.c"
#else
#include "dad.h"
#endif
#endif

/******************************************************************************
                                 BOOLEAN PROCEDURE CONSTANT (text attributt) */

#if nolib|rtconstantrut
char 
__rtconstant (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    return (__TRUE);
  else
    return (t->obj->h.konstant);
}
#endif

/******************************************************************************
                                 INTEGER PROCEDURE START (text attributt)    */

#if nolib|rtstartrut
long 
__rtstart (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    return (1);
  else
    return (t->start);
}
#endif

/******************************************************************************
                                 INTEGER PROCEDURE LENGTH (text attributt)   */

#if nolib|rtlengthrut
long 
__rtlength (t)
     __txtvp t;
{
  return (t->length);
}
#endif

/******************************************************************************
                                 TEXT PROCEDURE MAIN (text attributt)        */

#if nolib|rtmainrut
__txtvp 
__rtmain (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = 1;
      __et.length = t->obj->h.size;
      __et.start = 1;
    }
  return (&__et);
}
#endif

/******************************************************************************
                                  INTEGER PROCEDURE POS (text attributt)     */

#if nolib|rtposrut
long 
__rtpos (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    return (1);
  else
    return (t->pos);
}
#endif

/******************************************************************************
                                  BOOLEAN PROCEDURE MORE (text attributt)    */

#if nolib|rtmorerut
char 
__rtmore (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    return (__FALSE);
  else
    return ((t->pos <= t->length));
}
#endif
/******************************************************************************
                                 TEXT PROCEDURE SUB (text attributt)         */

#if nolib|rtsubrut
__txtvp 
__rtsub (t, i, n)
     __txtvp t;
     long i,
       n;
{
  if (i <= 0 || n < 0 || i + n > t->length + 1)
    __rerror ("Sub: Outside text frame");
  if (n == 0)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = t->start + i - 1;
      __et.length = n;
      __et.pos = 1;
    }
  return (&__et);
}
#endif

/******************************************************************************
                                  TEXT PROCEDURE STRIP (text attributt)      */

#if nolib|rtstriprut
__txtvp 
__rtstrip (t)
     __txtvp t;
{
  long pos;

  for (pos = (long) t->start + (long) t->length - 1;
       (long) t->start <= pos && t->obj->string[pos - 1] == ' '; pos--);

  if (pos < (long) t->start)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __et.obj = t->obj;
      __et.start = t->start;
      __et.pos = 1;
      __et.length = pos - (long) t->start + 1;
    }
  return (&__et);
}
#endif

/******************************************************************************
                                  PROCEDURE SETPOS (text attributt)          */

#if nolib|rtsetposrut
__txtvp 
__rtsetpos (t, i)
     __txtvp t;
     long i;
{
  t->pos = ((i < 1 || i > t->length + 1) ? t->length + 1 : i);
  return (t);
}
#endif

/******************************************************************************
                                CHARACTER PROCEDURE GETCHAR (text attributt) */

#if nolib|rtgetcharrut
char 
__rtgetchar (t)
     __txtvp t;
{
  if (t->obj == __NULL)
    __rerror ("Getchar: Notext");
  else if (t->pos > t->length)
    __rerror ("Getchar: Pos outside limits");
  return (t->obj->string[(long) t->start + (long) (t->pos++) - 2]);
}
#endif

/******************************************************************************
                                   INTEGER PROCEDURE GETINT (text attributt) */

#if nolib|rtgetintrut
long 
__rtgetint (t)
     __txtvp t;
{
  char *s;
  int sign;
  long i,
    j,
    n = 0;

  s = t->obj->string;
  i = t->start - 1;
  j = i + t->length;

  skipblanke;
  if (!more)
    __rerror ("Getint: Can't find any integer item");
  sign = fortegn;
  if (sign)
    i++;
  else
    sign = 1;
  skipblanke;
  if (!more)
    __rerror ("Getint: Can't find any integer item");
  if (!digit)
    __rerror ("Getint: Can't find any integer item");
  for (; more && digit; i++)
    {
      if (n > MAX_INT / 10)
	goto forstort;
      if (n == MAX_INT / 10)
	if ((s[i] - '0') > MAX_INT % 10)
	  goto forstort;
      n = 10 * n + s[i] - '0';
    }
  t->pos = i - t->start + 2;
  return (sign * n);
forstort:
  __rwarning ("Getint: To big integer item");
  return (sign * n);
}
#endif

/******************************************************************************
                                     REAL PROCEDURE GETREAL (text attributt) */

#if nolib|rtgetrealrut
#if FLOAT_IMPLEMENTED
double 
__rtgetreal (t)
     __txtvp t;
{
  char *s;
  int sign;
  static char cs[__RTPUTTEXTLENGTH + 1];
  long csi = 0,
    i,
    j,
    p;
  double r;
#if DAD_LIB
  ascii a;
  char ill = 0;
#else
#if FLOAT_IEEE | FLOAT_VAX
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
#endif
#endif
  s = t->obj->string;
  i = t->start - 1;
  j = i + t->length;

  skipblanke;
  if (!more)
    __rerror ("Getreal: Can't find any real item");
  if (sign = fortegn)
    i++;
#if DAD_LIB
  a.exp = 0;
  if (sign == -1)
    a.sign = -1;
  else
    a.sign = 1;
  skipblanke;
  if (!more)
    __rerror ("Getreal: Can't find any integer item");

  if (!digit)
    if (s[i] == __currentlowten)
      {
	a.frac[csi++] = '1';
	a.exp++;
      }
    else if (s[i] != __currentdecimalmark)
      __rerror ("Getreal: Illegal real item");
  if (a.exp == 0)
    for (; more && s[i] == '0'; i++);
  for (; more && digit; i++)
    {
      if (csi < 17)
	a.frac[csi++] = s[i];
      a.exp++;
    }
  if (s[i] == __currentdecimalmark)
    {
      i++;
      if (!digit)
	__rerror ("Getreal: Can't find any real item");
      if (a.exp == 0)
	for (; more && s[i] == '0'; i++)
	  a.exp--;
      for (; more && digit; i++)
	{
	  if (csi < 17)
	    a.frac[csi++] = s[i];
	}
    }
  a.exp--;
  if (s[i] == __currentlowten)
    {
      i++;
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      if (sign = fortegn)
	i++;
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      p = 0;
      for (; more && digit; i++)
	{
	  p = p * 10 + (s[i] - '0');
	}
      if (sign == -1)
	p = -p;
      a.exp += p;
    }
  a.frac[csi++] = '\0';
  r = __rextendedd_to_double (__rascii_to_extendedd (&a), &ill);
  if (ill == INF)
    goto texttolong;
#else
  if (sign == -1)
    cs[csi++] = '-';
  skipblanke;
  if (!more)
    __rerror ("Getreal: Can't find any integer item");
  if (!digit)
    if (s[i] == __currentlowten)
      {
	cs[csi++] = '1';
      }
    else if (s[i] != __currentdecimalmark)
      __rerror ("Getreal: Illegal real item");
  for (; more && digit; i++)
    {
      if (csi >= __RTPUTTEXTLENGTH)
	goto texttolong;
      cs[csi++] = s[i];
    }
  if (s[i] == __currentdecimalmark)
    {
      i++;
      cs[csi++] = '.';
      if (!digit)
	__rerror ("Getreal: Can't find any real item");
      for (; more && digit; i++)
	{
	  if (csi >= __RTPUTTEXTLENGTH)
	    goto texttolong;
	  cs[csi++] = s[i];
	}
    }
  if (s[i] == __currentlowten)
    {
      i++;
      cs[csi++] = 'e';
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      if (sign = fortegn)
	i++;
      if (csi >= __RTPUTTEXTLENGTH)
	goto texttolong;
      if (sign == -1)
	cs[csi++] = '-';
      skipblanke;
      if (!more)
	__rerror ("Getreal: Can't find any real item");
      for (; more && digit; i++)
	{
	  if (csi >= __RTPUTTEXTLENGTH)
	    goto texttolong;
	  cs[csi++] = s[i];
	}
    }
  if (csi > __RTPUTTEXTLENGTH)
    goto texttolong;
  cs[csi++] = '\0';
  (void) sscanf (cs, "%lf", &r);
#if FLOAT_VAX
#define reswd (1<<31)
  if (value.dasii.i2 == reswd && value.dasii.i1 == 0)
    goto texttolong;
#endif
#if FLOAT_IEEE
#define emask (2047L<<20)
  value.d = r;
  if ((value.dasii.i2 & emask) == emask)
    goto texttolong;
#endif
#endif
  t->pos = i - t->start + 2;
  return (r);
texttolong:
  __rerror ("Getreal: To big real item");
  /* NOTREACHED */
}
#endif
#endif

/******************************************************************************
                                  INTEGER PROCEDURE GETFRAC (text attributt) */

#if nolib|rtgetfracrut
long 
__rtgetfrac (t)
     __txtvp t;
{
  char *s;
  int sign;
  long i,
    j;
  long n = 0;

  s = t->obj->string;
  i = t->start - 1;
  j = i + t->length;

  skipblanke;
  if (!more)
    __rerror ("Getfrac: Can't find any grouped item");
  sign = fortegn;
  if (sign)
    i++;
  else
    sign = 1;
  skipblanke;
  if (!more)
    __rerror ("Getfrac: Can't find any grouped item");
  if (!digit)
    {
      if (s[i] == __currentdecimalmark)
	{
	  i++;
	  if (!digit)
	    __rerror ("Getfrac: Illegal grouped item");
	}
      else
	__rerror ("Getfrac: Can't find any grouped item");
    }

  while (1)
    {
      for (; more && digit; i++)
	{
	  if (n > MAX_INT / 10)
	    goto forstort;
	  if (n == MAX_INT / 10)
	    if ((s[i] - '0') > MAX_INT % 10)
	      goto forstort;
	  n = 10 * n + s[i] - '0';
	}
      if (!more)
	goto ut;
      skipblanke;
      if (!more)
	goto ut;
      if (!digit)
	{
	  if (s[i] == __currentdecimalmark)
	    {
	      i++;
	      if (!digit)
		{
		  --i;
		  goto ut;
		}
	    }
	  else
	    goto ut;
	}
    }
ut:
  /* G}r tilbake til siste siffer */
  while (!digit)
    i--;
  t->pos = i - t->start + 3;
  return (sign * n);
forstort:
  __rwarning ("Getfrac: To big grouped item");
  return (sign * n);
}
#endif

/******************************************************************************
                                 PROCEDURE PUTCHAR (text attributt)          */

#if nolib|rtputcharrut
__txtvp 
__rtputchar (t, c)
     __txtvp t;
     char c;
{
  if (t->obj == __NULL)
    __rerror ("Putchar: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putchar: Constant text object");
  if (t->pos > t->length)
    __rerror ("Putchar: Illegal pos value");
  t->obj->string[t->start + (t->pos++) - 2] = c;
  return (t);
}
#endif

/******************************************************************************
                                 PROCEDURE PUTINT (text attributt)          */

#if 0				/* Denne rutinen virker ikke for minint */
__txtvp 
rtputint (t, i)
     __txtvp t;
     long i;
{
  long j;
  char *s,
    minus;
  if (minus = i < 0)
    i *= -1;
  if (t->obj == __NULL)
    __rerror ("Putint: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putint: Constant text object");
  s = t->obj->string;
  j = t->start + t->length - 1;

  if (i == 0)
    s[--j] = '0';
  else
    {
      while (i > 0 && j >= (long) t->start)
	{
	  s[--j] = '0' + (i % 10);
	  i /= 10;
	}
    }
  if ((i == 0 && minus && j < (long) t->start) || i != 0)
    {
      __rwarning ("Putint: Text object to short");
    }
  {
    if (minus)
      s[--j] = '-';
    for (i = t->start - 1; i < j; i++)
      s[i] = ' ';
  }
  t->pos = t->length + 1;
  __rputlen = t->start + t->length - 1 - j;
  return (t);
}
#endif

#if nolib|rtputintrut
__txtvp 
__rtputint (t, ii)
     __txtvp t;
     long ii;
{
  static char cs[__RTPUTTEXTLENGTH + 1];
  long i,
    j,
    l;
  char *s;
  if (t->obj == __NULL)
    __rerror ("Putint: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putint: Constant text object");
  s = t->obj->string;
  (void) sprintf (cs, "%ld", ii);
  if ((l = strlen (cs)) <= (long) t->length)
    {
      i = t->length - l;
      for (j = 0; j < i; j++)
	s[t->start + j - 1] = ' ';
      for (i = 0; i < l; i++)
	s[t->start + (j++) - 1] = cs[i];
    }
  else
    {
      __rwarning ("Putint: Text object to short");
      for (j = 0; j < (long) t->length; j++)
	s[t->start + j - 1] = '*';
    }
  t->pos = t->length + 1;
  __rputlen = l;
  return (t);
}
#endif

/******************************************************************************
                                 PROCEDURE PUTFIX (text attributt)           */

#if nolib|rtputfixrut
#if FLOAT_IMPLEMENTED
__txtvp 
__rtputfix (t, r, n)
     __txtvp t;
     double r;
     long n;
{
  static char cs[__RTPUTTEXTLENGTH + 1],
    fcs[__RTPUTTEXTFORMATLENGTH + 1];
  long i,
    j,
    l;
  char *s;
#if DAD_LIB
  ascii *a;
#endif
  if (n < 0)
    __rerror ("Putfix: Second parameter is lesser than zero");
  if (n > __RTPUTMAXN)
    __rerror ("Putfix: Last parameter to big");
  if (t->obj == __NULL)
    __rerror ("Putfix: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putfix: Constant text object");
  s = t->obj->string;
#if DAD_LIB
  a = __rextendedd_to_ascii (__rdouble_to_extendedd (r), (int) n, __TRUE);
  switch (a->sign)
    {
    case -INF:
    case INF:
      __rerror ("Illegal real number");
    case POS:
    case NUL:
      i = 0;
      break;
    case NEG:
      cs[0] = '-';
      i = 1;
      break;
    }
  j = 0;
  if (a->exp >= 0)
    for (j = 0; a->exp >= 0; a->exp--)
      if (a->frac[j] == '\0' | a->sign == 0)
	cs[i++] = '0';
      else
	cs[i++] = a->frac[j++];
  else
    cs[i++] = '0';
  if (n > 0)
    {
      cs[i++] = __currentdecimalmark;
      for (l = 1; a->exp < -1 && l <= n; (l++, a->exp++))
	cs[i++] = '0';
      for (l = l; l <= n; l++)
	if (a->frac[j] == '\0' | a->sign == 0)
	  cs[i++] = '0';
	else
	  cs[i++] = a->frac[j++];
    }
  cs[i++] = '\0';
#else
#ifdef REALASDOUBLE
  (void) sprintf (fcs, "%%.%d%s", n, "lf");
#else
  (void) sprintf (fcs, "%%.%df", n);
#endif
  (void) sprintf (cs, fcs, r);
  if ((cs[0] == 'I') | (cs[1] == 'I'))	/* Test p} om det er lik uendelig */
    __rerror ("Illegal real number");
  if (cs[0] == '-')
    for (i = 1; __TRUE; i++)
      {
	if (cs[i] == '0' || cs[i] == '.')
	  continue;
	if (cs[i] == '\0')
	  {
	    for (i = 0; cs[i]; i++)
	      cs[i] = cs[i + 1];
	    break;
	  }
	break;
      }
  if (__currentdecimalmark != '.')
    {
      for (i = 0; cs[i] != '.'; i++);
      cs[i] = __currentdecimalmark;
    }
#endif
  if ((l = strlen (cs)) <= t->length)
    {
      i = t->length - l;
      for (j = 0; j < i; j++)
	s[t->start + j - 1] = ' ';
      for (i = 0; i < l; i++)
	s[t->start + (j++) - 1] = cs[i];
    }
  else
    {
      __rwarning ("Putfix: Text object to short");
      for (j = 0; j < (long) t->length; j++)
	s[t->start + j - 1] = '*';
    }
  t->pos = t->length + 1;
  __rputlen = l;
  return (t);
}
#endif
#endif

/******************************************************************************
                                 PROCEDURE PUTREAL (text attributt)          */

#if nolib|rtputrealrut
#if FLOAT_IMPLEMENTED
__txtvp 
__rtputreal (t, r, n)
     __txtvp t;
     double r;
     long n;
{
  static char cs[__RTPUTTEXTLENGTH + 1],
    fcs[__RTPUTTEXTFORMATLENGTH + 1];
  long i,
    j,
    l;
  char *s;
#if DAD_LIB
  ascii *a;
#endif
  if (n < 0)
    __rerror ("Putreal: Second parameter is lesser than zero");
  if (n > __RTPUTMAXN)
    __rerror ("Putreal: Last parameter to big");
  if (t->obj == __NULL)
    __rerror ("Putreal: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putreal: Constant text object");
  s = t->obj->string;
#if DAD_LIB
  a = __rextendedd_to_ascii (__rdouble_to_extendedd (r), (int) n, __FALSE);
  switch (a->sign)
    {
    case -INF:
    case INF:
      __rerror ("Illegal real number");
    case POS:
    case NUL:
      i = 0;
      break;
    case NEG:
      cs[0] = '-';
      i = 1;
      break;
    }
  for (j = 0; j < n; j++)
    {
      if (j == 1)
	cs[i++] = __currentdecimalmark;
      if (j >= 17 | a->sign == 0)
	cs[i++] = '0';
      else
	cs[i++] = a->frac[j];
    }
  cs[i++] = __currentlowten;
  if (a->exp >= 0)
    cs[i++] = '+';
  else
    {
      a->exp = -a->exp;
      cs[i++] = '-';
    }
  for (j = 1; j <= 3; j++)
    {
      l = a->exp - a->exp % 100;
      a->exp -= l;
      a->exp *= 10;
      cs[i++] = '0' + (l / 100);
    }
  cs[i++] = '\0';
#else
#ifdef REALASDOUBLE
  (void) sprintf (fcs, "%%.%d%s", (n > 0) ? n - 1 : 0, "le");
#else
  (void) sprintf (fcs, "%%.%de", (n > 0) ? n - 1 : 0);
#endif
  (void) sprintf		/* ARGSUSED */
    (cs, fcs, r);
  if ((cs[0] == 'I') | (cs[1] == 'I'))	/* Test p} om det er lik uendelig */
    __rerror ("Illegal real number");
  if (n == 0)
    for (i = (cs[0] == '-') ? 1 : 0; cs[i]; i++)
      cs[i] = cs[i + 1];
#if REALPOINT_E_BUG
  if (n <= 1)
    for (i = 0; cs[i]; i++)
      if (cs[i] == '.')
	{
	  for (; cs[i]; i++)
	    cs[i] = cs[i + 1];
	  break;
	}
#endif
  if (cs[0] == '-')
    for (i = 1; __TRUE; i++)
      {
	if (cs[i] == '0' || cs[i] == '.')
	  continue;
	if (cs[i] == 'e')
	  {
	    for (i = 0; cs[i]; i++)
	      cs[i] = cs[i + 1];
	    break;
	  }
	break;
      }
  if (__currentdecimalmark != '.')
    {
      for (i = 0; cs[i] != '.'; i++);
      cs[i] = __currentdecimalmark;
    }
  for (i = n; cs[i] != 'e'; i++);
  cs[i] = __currentlowten;
  if (cs[i + 3] == '\0')
    {
      cs[i + 5] = '\0';
      cs[i + 4] = cs[i + 2];
      cs[i + 3] = '0';
      cs[i + 2] = '0';
    }
  else if (cs[i + 4] == '\0')
    {
      cs[i + 5] = '\0';
      cs[i + 4] = cs[i + 3];
      cs[i + 3] = cs[i + 2];
      cs[i + 2] = '0';
    }
#endif
  if ((l = strlen (cs)) <= t->length)
    {
      i = t->length - l;
      for (j = 0; j < i; j++)
	s[t->start + j - 1] = ' ';
      for (i = 0; i < l; i++)
	s[t->start + (j++) - 1] = cs[i];
    }
  else
    {
      __rwarning ("Putreal: Text object to short");
      for (j = 0; j < (long) t->length; j++)
	s[t->start + j - 1] = '*';
    }
  t->pos = t->length + 1;
  __rputlen = l;
  return (t);
}
#endif
#endif

/******************************************************************************
                                 PROCEDURE PUTFRAC (text attributt)          */

#if nolib|rtputfracrut
__txtvp 
__rtputfrac (t, i, n)
     __txtvp t;
     long i;
     long n;
{
  char *s,
    minus;
  long j,
    antg = 0,
    k;

  if (minus = i < 0)
    i *= -1;
  if (t->obj == __NULL)
    __rerror ("Putfrac: Notext");
  if (t->obj->h.konstant)
    __rerror ("Putfrac: Constant text object");
  s = t->obj->string;
  j = t->start + t->length - 1;
  if (n > 0)
    {
      if ((n % 3) == 1)
	antg = 2;
      else if ((n % 3) == 2)
	antg = 1;

      for (k = 1; k <= n; k++)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = '0' + (i % 10);
	  i /= 10;
	  if (++antg == 3 && k < n)
	    {
	      if (j < t->start)
		goto forkort;
	      s[--j] = ' ';
	      antg = 0;
	    }
	}
      if (j < t->start)
	goto forkort;
      s[--j] = __currentdecimalmark;
      antg = 0;
    }
  else
    {
      n *= -1;
      for (k = 0; k < n; k++)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = '0';
	  if (++antg == 3)
	    {
	      if (j < t->start)
		goto forkort;
	      s[--j] = ' ';
	      antg = 0;
	    }
	}
    }
  while (i > 0 && j >= t->start)
    {
      if (j < t->start)
	goto forkort;
      s[--j] = '0' + (i % 10);
      i /= 10;
      if (++antg == 3 && i != 0)
	{
	  if (j < t->start)
	    goto forkort;
	  s[--j] = ' ';
	  antg = 0;
	}
    }
  if ((i == 0 && minus && j < t->start) || i != 0)
    goto forkort;
  else
    {
      if (minus)
	s[--j] = '-';
      for (i = t->start - 1; i < j; i++)
	s[i] = ' ';
      t->pos = t->length + 1;
    }
  __rputlen = t->start + t->length - 1 - j;
  return (t);
forkort:
  __rwarning ("Putfrac: Text object to short");
  for (j = 0; (long) j < t->length; j++)
    s[t->start + j - 1] = '*';
  t->pos = t->length + 1;
  __rputlen = j;
  return (t);
}
#endif

/******************************************************************************
                                   TEXT PROCEDURE COPY                      */

#if nolib|rcopyrut
__txtvp 
__rcopy (as, t)
     __txtvp t;
     long as;
{
  long i,
    j = 0;

  if (t->obj == __NULL)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __t1 = *t;
      __as = as;
      __et.obj = (__textref) __ralloc (((long) sizeof (__th) + __t1.length + 1));
      /* En ekstra byte |nsker */
      __et.obj->h.size = __et.length = __t1.length;	/* plass 
								 * til 
								 * '\0' */
      __et.obj->h.konstant = __FALSE;
      __et.obj->h.pp = (__pty) __TEXT;
      __as = 0;
      __et.pos = __et.start = 1;

      for (i = __t1.start; i < (long) __t1.start + __t1.length; i++)
	__et.obj->string[j++] = __t1.obj->string[i - 1];

      __et.obj->string[j++] = '\0';	/* Avslutter med \0 */
      __t1.obj = __NULL;
    }
  return (&__et);
}
#endif

/******************************************************************************
                                   TEXT CONCAT                               */

#if nolib|rconcrut
__txtvp 
__rconc (as, t1x, t2x)
     __txtvp t1x,
       t2x;
     long as;
{
  long i,
    j = 0,
    length;
  if (t1x->obj == __NULL && t2x->obj == __NULL)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __t1 = *t1x;
      __t2 = *t2x;
      __as = as;
      length = (long) __t1.length + (long) __t2.length;
      if (length >= MAX_TEXT_CHAR)
	__rerror ("Conc: Resulting text too long");
      __et.obj = (__textref) __ralloc (((long) sizeof (__th) + length + 1));
      /* En ekstra byte |nsker */
      __et.obj->h.size = __et.length = length;	/* plass til
								 * '\0' */
      __et.obj->h.konstant = __FALSE;
      __et.obj->h.pp = (__pty) __TEXT;
      __as = 0;
      __et.pos = __et.start = 1;
      for (i = __t1.start; i < (long) (__t1.start + __t1.length); i++)
	__et.obj->string[j++] = __t1.obj->string[i - 1];
      for (i = __t2.start; i < (long) (__t2.start + __t2.length); i++)
	__et.obj->string[j++] = __t2.obj->string[i - 1];

      __et.obj->string[j++] = '\0';	/* Avslutter med \0 */
      __t1.obj = __t2.obj = __NULL;
    }
  return (&__et);
}
#endif

/******************************************************************************
                                   TEXT PROCEDURE BLANKS                     */

#if nolib|rblanksrut
__txtvp 
__rblanks (as, n)
     long n;
     long as;
{
  long pos;
  if (n < 0)
    __rerror ("Blanks: Parameter lesser than zero");
  if (n >= MAX_TEXT_CHAR)
    __rerror ("Blanks: Parameter too high");
  if (n == 0)
    {
      __et.obj = __NULL;
      __et.pos = __et.start = __et.length = 0;
    }
  else
    {
      __as = as;
      __et.obj = (__textref) __ralloc (((long) sizeof (__th) + n + 1));
      /* En ekstra -> |nsker plass  */
      ((__textref) __et.obj)->h.size = __et.length = n;		/* '\0' p}
								 * slutten av 
								 * og til  */
      ((__textref) __et.obj)->h.konstant = __FALSE;
      ((__textref) __et.obj)->h.pp = (__pty) __TEXT;
      __as = 0;
      __et.start = __et.pos = 1;
      for (pos = 0; pos < n; pos++)
	((__textref) __et.obj)->string[pos] = ' ';
      ((__textref) __et.obj)->string[pos] = '\0';	/* Avslutter med \0 */
    }
  return (&__et);
}
#endif

/******************************************************************************
                                                 rtextvalassign              */

#if nolib|rtextvalassignrut
__txtvp 
__rtextvalassign (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  long i = 0;
  register char *p1x,
   *p2x;

  if ((t1x->obj == __NULL && t2x->obj != __NULL) || (t1x->length < t2x->length))
    __rerror ("Textvalassign: Text area too small");

  if (t1x->obj == __NULL)
    return (t1x);

  if (t1x->obj->h.konstant)
    __rerror ("Textvalassign: Attempt to alter constant text");

  p1x = (char *) &(t1x->obj->string[t1x->start - 1]);

  if (t2x->obj != __NULL)
    for (p2x = (char *) &(t2x->obj->string[t2x->start - 1]); i < (long) t2x->length; i++)
      *p1x++ = *p2x++;

  for (i = 0; i < (long) t1x->length - (long) t2x->length; i++)
    *p1x++ = ' ';

  return (t1x);
}
#endif

/******************************************************************************
                                                 rtextassign                 */

#if nolib|rtextassignrut
__txtvp 
__rtextassign (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  *t1x = *t2x;
  return (t1x);
}
#endif

/******************************************************************************
                                               Runtine for == og =/= testen  */

#if nolib|reqrtextrut
char 
__reqrtext (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  if (t1x->obj == t2x->obj && t1x->length == t2x->length && t1x->start == t2x->start)
    return (__TRUE);
  return (__FALSE);
}
#endif
/******************************************************************************
                                                 Runtine for = og <> testen  */

#if nolib|reqtextrut
char 
__reqtext (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  long i = 0;
  register char *p1x,
   *p2x;
  if (t1x->obj == __NULL && t2x->obj == __NULL)
    return (__TRUE);
  if (t1x->obj == __NULL || t2x->obj == __NULL)
    return (__FALSE);
  if (t1x->length != t2x->length)
    return (__FALSE);

  p1x = (char *) &(t1x->obj->string[t1x->start - 1]);
  for (p2x = (char *) &(t2x->obj->string[t2x->start - 1]); i < (long) t2x->length; i++)
    if (!(*p1x++ == *p2x++))
      return (__FALSE);

  return (__TRUE);
}
#endif

/******************************************************************************
                                                         Rutine for t1 < t2  */

#if nolib|rlttextrut
char 
__rlttext (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  long i = 0,
    j = 0;
  register unsigned char *p1x,
   *p2x;

  if (t1x->obj == __NULL && t2x->obj == __NULL)
    return (__FALSE);
  if (t1x->obj == __NULL)
    return (__TRUE);

  p1x = (unsigned char *) &(t1x->obj->string[t1x->start - 1]);
  p2x = (unsigned char *) &(t2x->obj->string[t2x->start - 1]);
  while (i < t1x->length && j < t2x->length)
    {
      i++;
      j++;
      if (*p1x < *p2x)
	return (__TRUE);
      if (*p1x++ > *p2x++)
	return (__FALSE);
    }
  if (i == t1x->length && j == t2x->length)
    return (__FALSE);		/* Like */
  if (i == t1x->length)
    return (__TRUE);		/* t1 < t2 */
  return (__FALSE);		/* t1 > t2 */
}
#endif

/******************************************************************************
                                                        Rutine for t1 <= t2  */

#if nolib|rletextrut
char 
__rletext (t1x, t2x)
     __txtvp t1x,
       t2x;
{
  long i = 0,
    j = 0;
  register unsigned char *p1x,
   *p2x;
  if (t1x->obj == __NULL && t2x->obj == __NULL)
    return (__TRUE);
  if (t2x->obj == __NULL)
    return (__FALSE);

  p1x = (unsigned char *) &(t1x->obj->string[t1x->start - 1]);
  p2x = (unsigned char *) &(t2x->obj->string[t2x->start - 1]);

  while (i < t1x->length && j < t2x->length)
    {
      i++;
      j++;
      if (*p1x > *p2x)
	return (__FALSE);
      if (*p1x++ < *p2x++)
	return (__TRUE);
    }
  if (i == t1x->length && j == t2x->length)
    return (__TRUE);		/* Like */
  if (j == t2x->length)
    return (__FALSE);		/* t2 < t1 */
  return (__TRUE);
}
#endif
