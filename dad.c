/* $Id: dad.c,v 1.3 1994/09/20 16:21:31 cim Exp $ */

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

/* DOUBLE->ASCII->DOUBLE coersion
 *
 * Library for converting between three floating point formats,             
 * IEEE-double - Own extended format and base 10 format.                    
 * The extended format is only used for calculating, and are                
 * independent of other formats. The extended format are limited up to      
 * 64 bit fraction. This package is slow, there is room for optimalizations
 * and should only be used when the standard library does not do coersion   
 * correct. */

#include "dad.h"
/******************************************************************************
                                                      GENERAL RIGHT SHIFT    */
#if 0
extendedd *
__rgrshift (e, n)
     extendedd *e;
     int n;
{
  if (n < 32)
    {
      e->lfrac = (e->lfrac >> n) | (e->hfrac << (32 - n));
      e->hfrac = e->hfrac >> n;
    }
  else if (n > 32)
    {
      e->lfrac = e->hfrac >> (n - 32);
      e->hfrac = 0L;
    }
  else
    {
      e->lfrac = e->hfrac;
      e->hfrac = 0L;
    }
  return (e);
}
#endif
/******************************************************************************
                                                              RIGHT SHIFT    */

extendedd *
__rrshift (e)
     extendedd *e;
{
  e->lfrac = ((e->lfrac >> 1) & ((1L << 31) - 1)) | ((e->hfrac & 1) << 31);
  e->hfrac = ((e->hfrac >> 1) & ((1L << 31) - 1));
  return (e);
}

/******************************************************************************
                                                       GENERAL LEFT SHIFT    */
#if 0
extendedd *
__rglshift (e, n)
     extendedd *e;
     int n;
{
  if (n < 32)
    {
      e->hfrac = (e->hfrac << n) | (e->lfrac >> (32 - n));
      e->lfrac = e->lfrac << n;
    }
  else if (n > 32)
    {
      e->hfrac = e->lfrac << (n - 32);
      e->lfrac = 0L;
    }
  else
    {
      e->hfrac = e->lfrac;
      e->lfrac = 0L;
    }
  return (e);
}
#endif
/******************************************************************************
                                                               LEFT SHIFT    */

extendedd *
__rlshift (e)
     extendedd *e;
{
  e->hfrac = (e->hfrac << 1) | ((e->lfrac >> 31) & 1);
  e->lfrac = (e->lfrac << 1) | (e->lfrac & 1);
  return (e);
}

/******************************************************************************
                                                            GENERAL SHIFT    */
#if 0
extendedd *
__rgshift (e, n)
     extendedd *e;
     int n;
{
  if (n > 0)
    return (__rgrshift (e, n));
  if (n < 0)
    return (__rglshift (e, n));
  return (e);
}
#endif
/******************************************************************************
                                                            NORMALIZATION    */

extendedd *
__rnormalize (e)
     extendedd *e;
{
  if (e->hfrac || e->lfrac)
    while ((e->hfrac & (1L << 31)) == 0)
      {
	__rlshift (e);
	e->exp--;
      }
  return (e);
}

/******************************************************************************
                                                   EXTENDED TO IEEE-DOUBLE   */

double 
__rextendedd_to_double (e, ill)
     extendedd *e;
     char *ill;
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
  if (e->sign > 0)
    value.dasii.i2 = 0;
  else
    value.dasii.i2 = signbit;
  if (e->sign == NUL)
    {
      value.dasii.i2 = 0;
      value.dasii.i1 = 0;
      return (value.d);
    }
  if (e->exp >= 1024 || e->sign == INF || e->sign == -INF)
    {
      *ill = INF;
      value.dasii.i2 |= emask;
      value.dasii.i1 = 0;
      return (value.d);
    }
  if (e->exp <= -1023)
    {
      while (e->exp < -1023)
	{
	  __rrshift (e);
	  e->exp++;
	}
      __rrshift (e);
    }
  value.dasii.i2 |= (e->exp + 1023) << 20;
  value.dasii.i2 |= (e->hfrac >> 11) & ((1L << 20) - 1);
  value.dasii.i1 = e->hfrac << 21;
  value.dasii.i1 |= (e->lfrac >> 11) & ((1L << 21) - 1);
  if (e->lfrac & (1 << 10))
    if (e->sign == POS)
      return (__raddepsilon (value.d));
    else
      return (__rsubepsilon (value.d));
  else
    return (value.d);
}

/******************************************************************************
                                                   IEEE-DOUBLE TO EXTENDED   */

extendedd *
__rdouble_to_extendedd (d)
     double d;
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
  static extendedd e;
  value.d = d;
  if (value.dasii.i2 & signbit)
    e.sign = NEG;
  else
    e.sign = POS;
  if ((value.dasii.i2 & emask) == emask)
    e.sign *= 2;
  else
    {
      e.exp = ((value.dasii.i2 >> 20) & ((1L << 11) - 1)) - 1023;
      e.hfrac = (value.dasii.i2 & ((1L << 20) - 1)) << 11;
      e.hfrac |= (value.dasii.i1 >> 21) & ((1 << 11) - 1);
      e.lfrac = value.dasii.i1 << 11;
      if (value.dasii.i2 & emask)
	{
	  e.exp = ((value.dasii.i2 >> 20) & ((1 << 11) - 1)) - 1023;
	  e.hfrac |= 1L << 31;
	}
      else
	{
	  e.exp = -1022;
	  if (value.dasii.i1 == 0 && (value.dasii.i2 & (~(1L << 31))) == 0)
	    e.sign = NUL;
	  __rnormalize (&e);
	}
    }
  return (&e);
}

/******************************************************************************
                                                            MULTIPLY BY 10   */

extendedd *
__rmul_10 (e)
     extendedd *e;
{
  unsigned long carry, h8, l8, h2, l2, h10, l10;
  h8 = e->hfrac;
  l8 = e->lfrac;
  __rrshift (__rrshift (e));
  h2 = e->hfrac;
  l2 = e->lfrac;
  l10 = l2 + l8;
  if (l10 < l8)
    carry = 1;
  else
    carry = 0;
  h10 = h2 + h8 + carry;
  e->hfrac = h10;
  e->lfrac = l10;
  e->exp += 3;
  if (h10 < h8)
    {
      __rrshift (e);
      e->hfrac |= (1L << 31);
      e->exp++;
    }
  if (l8 & 2)
    {
      l10 = e->lfrac;
      e->lfrac++;
      if (e->lfrac < l10)
	{
	  h10 = e->hfrac;
	  e->hfrac++;
	  if (e->hfrac < h10)
	    {
	      __rrshift (e);
	      e->hfrac |= (1L << 31);
	      e->exp++;
	    }
	}
    }
  return (e);
}

/******************************************************************************
                                                         DIVIDE BY 10        */

extendedd *
__rdiv_10 (e)
     extendedd *e;
{
  unsigned long h = 0, l = 0, carry;
  int i;
  if (e->hfrac >= (unsigned long) (((unsigned long) 10) << 28))
    {
      e->hfrac -= (unsigned long) (((unsigned long) 10) << 28);
      h |= 1L << 31;
    }
  for (i = 30; i >= 0; i--)
    {
      if (e->hfrac >= (unsigned long) (((unsigned long) 10) << 27))
	{
	  e->hfrac -= (unsigned long) (((unsigned long) 10) << 27);
	  h |= 1L << i;
	}
      __rlshift (e);
    }
  for (i = 31; i >= 0; i--)
    {
      if (e->hfrac >= (unsigned long) (((unsigned long) 10) << 27))
	{
	  e->hfrac -= (unsigned long) (((unsigned long) 10) << 27);
	  l |= 1L << i;
	}
      __rlshift (e);
    }
  if (e->hfrac >= (unsigned long) (((unsigned long) 10) << 27))
    {
      e->lfrac = l + 1;
      if (e->lfrac < l)
	e->hfrac = h + 1;
      else
	e->hfrac = h;
    }
  else
    {
      e->lfrac = l;
      e->hfrac = h;
    }
  if (e->hfrac & (1L << 31))
    carry = 0;
  else
    carry = 1;
  e->exp -= 3 + carry;
  if (carry)
    __rlshift (e);
  return (e);
}

/******************************************************************************
                                                     BASE-10 TO EXTENDED     */

extendedd *
__rascii_to_extendedd (a)
     ascii *a;
{
  static extendedd e;
  int i, u;
  e.exp = 0;
  e.hfrac = 0;
  e.lfrac = 0;
  e.sign = a->sign;
  if (e.sign == NUL || e.sign == INF || e.sign == -INF)
    return (&e);
  for (i = 0; i < 18 && a->frac[i] != '\0'; i++);
  for (i--; a->frac[i] == '0'; i--);
#if 0
  if (a->exp >= i)
    {
      u = i;
      a->exp -= i;
      for (i = 0; i <= u; i++)
	{
	  unsigned long ohfrac;
	  ohfrac = e.hfrac;
	  e.hfrac += ((long) (a->frac[i] - '0')) << 31 - e.exp;
	  if (e.hfrac < ohfrac)
	    {
	      __rrshift (&e);
	      e.exp++;
	    }
	  if (i < u)
	    __rmul_10 (&e);
	}
    }
  else
#endif
    for (; i >= 0; i--)
      {
	switch (a->frac[i])
	  {
	  case '9':
	  case '8':
	    while (e.exp < 3)
	      {
		__rrshift (&e);
		e.exp++;
	      }
	    e.hfrac |= ((long) (a->frac[i] - '0')) << 28;
	    break;
	  case '7':
	  case '6':
	  case '5':
	  case '4':
	    while (e.exp < 2)
	      {
		__rrshift (&e);
		e.exp++;
	      }
	    e.hfrac |= ((long) (a->frac[i] - '0')) << 29;
	    break;
	  case '3':
	  case '2':
	    while (e.exp < 1)
	      {
		__rrshift (&e);
		e.exp++;
	      }
	    e.hfrac |= ((long) (a->frac[i] - '0')) << 30;
	    break;
	  case '1':
	    while (e.exp < 0)
	      {
		__rrshift (&e);
		e.exp++;
	      }
	    e.hfrac |= ((long) (a->frac[i] - '0')) << 31;
	    break;
	  case '0':
	    break;
	  }
	if (i > 0)
	  __rdiv_10 (&e);
      }
  while (a->exp > 0)
    {
      __rmul_10 (&e);
      a->exp--;
    }
  while (a->exp < 0)
    {
      __rdiv_10 (&e);
      a->exp++;
    }
  if (e.hfrac == 0 && e.lfrac == 0)
    e.sign = NUL;
  return (&e);
}

/******************************************************************************
                                                     EXTENDED TO BASE-10     */

ascii *
__rextendedd_to_ascii (e, prec, frac)
     extendedd *e;
     int prec;
     char frac;
{
  static ascii a;
  int carry;
  int i;
  a.exp = 0;
  a.sign = e->sign;
  if (a.sign == NUL || a.sign == INF || a.sign == -INF)
    {
      return (&a);
    }
  if (e->exp > 0)
    {
      while (e->exp > 3)
	{
	  a.exp++;
	  __rdiv_10 (e);
	}
      if (e->exp == 3 && e->hfrac >= (10L << 28))
	{
	  a.exp++;
	  __rdiv_10 (e);
	}
    }
  else
    {
      while (e->exp < 0)
	{
	  a.exp--;
	  __rmul_10 (e);
	}
    }
  if (frac)
    prec += a.exp + 1;
  if (prec > 17)
    prec = 17;
  else if (prec < 0)
    prec = 0;
  for (i = 0; i <= prec; i++)
    {
      switch (e->exp)
	{
	case 3:
	  a.frac[i] = '0' + (e->hfrac >> 28);
	  e->hfrac = e->hfrac & (~(15L << 28));
	  break;
	case 2:
	  a.frac[i] = '0' + (e->hfrac >> 29);
	  e->hfrac = e->hfrac & (~(7L << 29));
	  break;
	case 1:
	  a.frac[i] = '0' + (e->hfrac >> 30);
	  e->hfrac = e->hfrac & (~(3L << 30));
	  break;
	case 0:
	  a.frac[i] = '0' + (e->hfrac >> 31);
	  e->hfrac = e->hfrac & (~(1L << 31));
	  break;
	default:
	  a.frac[i] = '0';
	  break;
	}
      __rnormalize (e);
      __rmul_10 (e);
    }
  if (a.frac[prec] >= '5')
    carry = 1;
  else
    carry = 0;
  for (i = prec - 1; i >= 0; i--)
    {
      if (carry)
	{
#if 0
#else
	  if (a.frac[i] == ':')
	    a.frac[i] = '1';
	  else
#endif
	  if (a.frac[i] == '9')
	    a.frac[i] = '0';
	  else
	    {
	      a.frac[i]++;
	      carry = 0;
	    }
	}
      else
#if 0
	break;
#else
      if (a.frac[i] == ':')
	{
	  a.frac[i] = '0';
	  carry = 1;
	}
#endif
    }
  if (carry)
    {
      for (i = prec - 1; i > 0; i--)
	a.frac[i] = a.frac[i - 1];
      a.frac[0] = '1';
      a.exp++;
    }
  a.frac[prec] = '\0';
  return (&a);
}

#if DAD_DEB
print_extendedd (e)
     extendedd *e;
{
  printf ("Extendedd S: %ld E: %ld f: %lx,%lx\n", e->sign
	  ,e->exp, e->hfrac, e->lfrac);
}

print_ascii (a)
     ascii *a;
{
  printf ("ascii: ");
  switch (a->sign)
    {
    case INF:
      printf ("+***********************\n");
      break;
    case POS:
      printf ("+%se%3ld\n", a->frac, a->exp);
      break;
    case NUL:
      printf ("0\n");
      break;
    case NEG:
      printf ("-%se%3ld\n", a->frac, a->exp);
      break;
    case -INF:
      printf ("-***********************\n");
      break;
    }
}

print_double (d)
     double d;
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
  value.d = d;
  printf ("double: %.17le %.15le (%x,%x)\n", value.d, value.d, value.dasii.i1, value.dasii.i2);
}
#endif
