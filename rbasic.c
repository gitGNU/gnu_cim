/* $Id: rbasic.c,v 1.2 1994/07/17 10:41:12 cim Exp $ */

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

/* Run time rutiner for "klassen" BASICIO */

#include "cim.h"

/******************************************************************************
                                               REF(infile) PROCEDURE SYSIN   */

#if nolib|rsysinrut
__dhp 
__rsysin ()
{
  return ((__dhp) & __sysin);
}
#endif

/******************************************************************************
                                            REF(printfile) PROCEDURE SYSOUT  */

#if nolib|rsysoutrut
__dhp 
__rsysout ()
{
  return ((__dhp) & __sysout);
}
#endif

/******************************************************************************
                                            REF(printfile) PROCEDURE SYSERR  */

#if nolib|rsyserrrut
__dhp 
__rsyserr ()
{
  return ((__dhp) & __syserr);
}
#endif

/******************************************************************************
                                            PROCEDURE TERMINATE_PROGRAM      */

/* Er implementert som inline kode (goto stop) */
