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

#include "limit.h"
#include "cim.h"

#if SIZEOF_LONG == 8
#define PSURANNUM 7450580596923828125L	/* Se nedenfor (p=13) */
#else
#define PSURANNUM 1220703125L	/* I f|lge standarden lik 5**(2*p+1) (p=6) */
#endif
#define CALCU (((unsigned long)(*U= (*U*PSURANNUM)|1))>>1)
#define MAXU (MAX_INT)
#define BASICU ((((double)CALCU)+0.5)/((double)MAXU+1.0))

#define ARRELEM(A,i) (*(double *)(((char *)A)+sizeof(__ah)+\
                                 sizeof(__arrlimit)+sizeof(double)*(i)))
