/* $Id: $ */

/* Copyright (C) 1997 Sverre Hvammen Johansen,
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
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

