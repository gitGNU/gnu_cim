/* $Id: $ */

/* Copyright (C) 1999 Petr Novak, Charles University, Prague
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* this fncs are used when dividing to test zero operand */

#include "cim.h"
#include "config.h"

#if DIV0

/*****************************************************************************
                                                                       DIV0 */
double __rrdiv0(double i)
{
  if (i != 0) return i;
  __rerror ("Div0: Division by zero");
  return 1.0;
}

#endif


