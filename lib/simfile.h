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

/******************************************************************************
                                     Outermost block and the file classes    */

/* Outermost block */
typedef struct
  {
    __dh h;
    __dhp c1,
      c2;
  }
__bs0FILE, __bs0;
extern __bs0FILE __blokk0FILE;
extern __bs0FILE *__bp0FILE;
extern short __rl0FILE[];
extern __pty __pl0FILE[];
extern __ptyp __p0FILE;

/* Class file */
typedef struct
  {
    __dh h;
    __txt filename;
    void *file;
    char open;
    char shared,
      append,
      create,
      readwrite,
      re_wind,
      purge;
  }
__bs1FILE, __bs1;
extern __bs1FILE *__bp1FILE;
extern short __rl1FILE[];
extern __pty __pl1FILE[];
extern __ptyp __p1FILE;

/* File class imagefile */
typedef struct
  {
    __bs1FILE s;
    __txt IMAGE;
  }
__bs2FILE, __bs2;
extern __bs2FILE *__bp2FILE;
extern short __rl2FILE[];
extern __pty __pl2FILE[];
extern __ptyp __p2FILE;

/* Imagefile class outfile */
typedef struct
  {
    __bs2FILE s;
  }
__bs3FILE, __bs3;
extern __pty __pl3FILE[];
extern __ptyp __p3FILE;

/* Imagefile class infile */
typedef struct
  {
    __bs2FILE s;
    char endfile;
  }
__bs4FILE, __bs4;
extern __pty __pl4FILE[];
extern __ptyp __p4FILE;
extern __bs4FILE __sysin;

/* Imagefile class directfile */
typedef struct
  {
    __bs2FILE s;
    long loc,
      maxloc,
      minwriteloc,
      imagelength;
    char endfile,
      locked,
      lastop,
      writeonly;
  }
__bs5FILE, __bs5;
extern __pty __pl5FILE[];
extern __ptyp __p5FILE;

/* Outfile class printfile */
typedef struct
  {
    __bs3FILE s;
    long line,
      lines_per_page,
      spacing,
      page;
  }
__bs6FILE, __bs6;
extern __pty __pl6FILE[];
extern __ptyp __p6FILE;
extern __bs6FILE __sysout;
extern __bs6FILE __syserr;

/* File class bytefile  */
typedef struct
  {
    __bs1FILE s;
    char endfile,
      bytesize;
  }
__bs7FILE, __bs7;
extern __pty __pl7FILE[];
extern __ptyp __p7FILE;

/* Bytefile class inbytefile */
typedef struct
  {
    __bs7FILE s;
  }
__bs8FILE, __bs8;
extern __pty __pl8FILE[];
extern __ptyp __p8FILE;

/* Bytefile class outbytefile */
typedef struct
  {
    __bs7FILE s;
  }
__bs9FILE, __bs9;
extern __pty __pl9FILE[];
extern __ptyp __p9FILE;

/* Bytefile class directbytefile */
typedef struct
  {
    __bs7FILE s;
    long loc,
      maxloc,
      minwriteloc;
    char locked,
      lastop,
      writeonly;
  }
__bs10FILE, __bs10;
extern __pty __pl10FILE[];
extern __ptyp __p10FILE;

