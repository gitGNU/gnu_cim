/* $Id: dad.h,v 1.3 1994/09/20 16:21:33 cim Exp $ */

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

#define signbit (1L<<31)
#define emask (2047L<<20)
#define POS 1
#define NEG -1
#define NUL 0
#define INF 2

/******************************************************************************
                                               Structure for extended format */

typedef struct
  {
    long exp;
    unsigned long hfrac;
    unsigned long lfrac;
    long sign;
  }
extendedd;

/******************************************************************************
                                                Structure for base 10 format */

typedef struct
  {
    long exp;
    char frac[18];
    long sign;
  }
ascii;


double __rextendedd_to_double ();
extendedd *__rdouble_to_extendedd ();
extendedd *__rascii_to_extendedd ();
ascii *__rextendedd_to_ascii ();
