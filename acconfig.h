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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */
@TOP@
/* Additional compiler flags for building shared library objects */
#define PIC_FLAG ""

/* Compiler flag to prevent dynamic linking */
#define LINK_STATIC_FLAG ""

/* How to pass a linker flag through the compiler.*/
#define WL_FLAG ""

/* Define cpu type derived from system type.  */
#define CPU_TYPE "???"

/* Define if we have signal sigbus.  */
#undef HAVE_SIGBUS

/* Define if we have signal sigfpe.  */
#undef HAVE_SIGFPE

/* Define if we have signal sigill.  */
#undef HAVE_SIGILL

/* Define if we have signal sigsegv.  */
#undef HAVE_SIGSEGV

/* Define if we have signal sigsys.  */
#undef HAVE_SIGSYS

/* Define if we have signal sigtrap.  */
#undef HAVE_SIGTRAP

/* Define the system type we are running.  */
#define SYSTEM_TYPE "???"

/* Define manufacturer derived from system type.  */
#define MANUFACTURER "???"

/* Define os type derived from system type.  */
#define OS_TYPE "???"

/* Define os type including version derived from system type.  */
#define OS_TYPE_VERSION "???"

/* Define if temporary file have to be opened in binary mode.  */
#undef OPEN_FILE_IN_BINARY_MODE

/* Define version string */
#undef PACKAGE_VERSION

/* Define version string */
#undef PACKAGE

/* Define if ISO_LATIN is implemented.  */
#define ISO_LATIN 1

/* Define input_line_length.  */
#define INPUT_LINE_LENGTH 	80

/* Define output line length.  */
#define OUTPUT_LINE_LENGTH 	80

/* Define lines per page.  */
#define LINES_PER_PAGE 		60

/* Define the size of the heap */
#define DYNMEMSIZEKB 		512

/* Define if dump is implemented.  */
#undef DUMP

/* Define if the implementation conforms to IEEE-754.  */
#define FLOAT_IEEE 1

/* Define if the implementation conforms to the vax architecture.  */
#undef FLOAT_VAX

/* Define MIN_DOUBLE and MAX_DOUBLE  */
#define MAX_DOUBLE DBL_MAX
#define MIN_DOUBLE DBL_MIN

/* Define alignment */
#define ALIGNMENT 8

@BOTTOM@

/* Define the adress of the first data location.  */
#define FIRST_DATA_LOCATION ((int)(&__start_data_segment))

/* Define MAX_INT */
#if SIZEOF_LONG == 8
#define	MAX_INT (~(1L<<63))
#else
#define	MAX_INT (~(1L<<31))
#endif

/* Define TYPE_32_INT */
#if SIZEOF_LONG == 8
#define TYPE_32_INT int
#else
#define TYPE_32_INT long
#endif

