/* $Id: rtextutl.c,v 1.8 1994/10/15 10:58:56 cim Exp $ */

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

/* Run time rutiner for tekst utilities  rutiner */

#include "cim.h"
#include "cconst.h"
#include <ctype.h>


/******************************************************************************
                                          CHARACTER PROCEDURE CHAR(i)        */

#if nolib|rcharrut
char 
__rchar (i)
     long i;
{
  if (i < 0 || i > MAXRANK)
    __rerror ("Char: Parameter outside limits");
  return (i);
}
#endif

/******************************************************************************
                                          CHARACTER PROCEDURE ISOCHAR(i)     */

#if nolib|risocharrut
char 
__risochar (i)
     long i;
{
  if (i < 0 || i > 255)
    __rerror ("Isochar: Parameter outside limits");
  return (i);
}
#endif

/******************************************************************************
                                          INTEGER PROCEDURE RANK(c)          */

#if nolib|rrankrut
long 
__rrank (c)
     char c;
{
  return ((unsigned char) c);
}
#endif

/******************************************************************************
                                          INTEGER PROCEDURE ISORANK(c)       */

#if nolib|risorankrut
long 
__risorank (c)
     char c;
{
  return ((unsigned char) c);
}
#endif

/******************************************************************************
                                          BOOLEAN PROCEDURE DIGIT(c)         */

#if nolib|rdigitrut
char 
__rdigit (c)
     char c;
{
  return (c >= '0' && c <= '9');
}
#endif

/******************************************************************************
                                          BOOLEAN PROCEDURE LETTER(c)        */

#if nolib|rletterrut
char 
__rletter (c)
     char c;
{
  return (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}
#endif

/******************************************************************************
                                          CHARACTER PROCEDURE LOWTEN(c)      */

#if nolib|rlowtenrut
char 
__rlowten (c)
     char c;
{
  char s;
  if (isdigit (c) || c == '+' || c == '-' 
      || c == '.' || c == ',' || c == 127 || c < 32
      || __risorank (c) > 127)
    __rerror ("Lowten: Illegal character");

  s = __currentlowten;
  __currentlowten = c;
  return (s);
}
#endif

/******************************************************************************
                                          CHARACTER PROCEDURE DECIMALMARK(c) */

#if nolib|rdecimalmarkrut
char 
__rdecimalmark (c)
     char c;
{
  char s;
  if (c != '.' && c != ',')
    __rerror ("Decimalmark: Illegal character");
  s = __currentdecimalmark;
  __currentdecimalmark = c;
  return (s);
}
#endif

/******************************************************************************
                                          TEXT PROCEDURE UPCASE(t)           */

#if nolib|rupcaserut
__txtvp
__rupcase (t)
     __txtvp t;
{
  register long i;
  unsigned char *s;
  s = (unsigned char *) t->obj->string;

  for (i = 0; i < t->length; i++)
    s[t->start + i - 1] =
      (isalpha (s[t->start + i - 1]) 
       ? (islower (s[t->start + i - 1]) 
	  ? toupper ((int) s[t->start + i - 1]) : s[t->start + i - 1])
       : s[t->start + i - 1]);
  __et.obj = t->obj;
  __et.length = t->length;
  __et.pos = 1;
  __et.start = t->start;
  return(&__et);
}
#endif

/******************************************************************************
                                          TEXT PROCEDURE LOWCASE(t)          */

#if nolib|rlowcaserut
__txtvp
__rlowcase (t)
     __txtvp t;
{
  register long i;
  unsigned char *s;
  s = (unsigned char *) t->obj->string;

  for (i = 0; i < t->length; i++)
    s[t->start + i - 1] =
      (isalpha (s[t->start + i - 1]) 
       ? (isupper (s[t->start + i - 1]) 
	  ? tolower ((int) s[t->start + i - 1]) : s[t->start + i - 1])
       : s[t->start + i - 1]);
  __et.obj = t->obj;
  __et.length = t->length;
  __et.pos = 1;
  __et.start = t->start;
  return(&__et);
}
#endif
