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

#include "cim.h"
#include "config.h"

#if STDC_HEADERS
#include <stdlib.h>
#include <float.h>
#endif

#if HAVE_VALUES_H
#include <values.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if FLOAT_IEEE | FLOAT_VAX | FLOAT_IBM
#define reswd (1L<<31)
#define signbit (1L<<31)
#if FLOAT_IBM
#define emask (127<<24)
#define tmask (15<<20)
#define adexp (1<<20)
#endif
#if FLOAT_IEEE
#define emask (2047L<<20)
#endif
#endif
