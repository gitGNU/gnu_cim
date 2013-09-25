/* $Id: cgen.c,v 1.18 1997/01/08 09:49:17 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen and Terje Mjøs,
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

#include "gen.h"
#include "extspec.h"
#include "mapline.h"

static long last_line;
static long true_last_line;

/* JUMP SOM ING]R I RUNTIME-SYSTEMET */
int lediglabel = STARTLABEL;

/******************************************************************************
                             NEWLABEL & TYPELABEL & EXITLABEL & GOTOLABEL    */

int newlabel (void )
{
  return (lediglabel++);
}

void typelabel (int l)
{
  fprintf (ccode, "__l%d:", l);
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

void gotoswitch (void)
{
  fprintf (ccode, "goto __sw;");
  last_line = 0;
}

void exitlabel (int l)
{
  gotoswitch ();
  typelabel (l);
}

void exitcondlabel (int l)
{
  fprintf (ccode, "goto __sw;__l%d:", l);
  not_reached = FALSE;
  last_line = 0;
  genline ();
}

void gotolabel (int l)
{
  fprintf (ccode, "goto __l%d;", l);
}

/******************************************************************************
                                        NEWLLABEL & TYPELLABEL & GOTOLLABEL  */

/* LOKALE HOPP */
static int ledigllabel;

int newllabel (void)
{
  return (ledigllabel++);
}

void typellabel (int l)
{
  fprintf (ccode, "__ll%d:", l);
  not_reached = FALSE;
  last_line = 0;
}

void gotollabel (int l)
{
  fprintf (ccode, "goto __ll%d;", l);
}


/******************************************************************************
                                                        GENLINE              */

void genline (void)
{
  if (true_last_line != lineno)
    {
      fprintf (ccode, "\n# %d \"%s\"\n",getmapline(lineno), getmapfile(lineno));
    }
  if (!option_line && last_line != lineno)
    fprintf (ccode, "__cline=%ld;", lineno);
  true_last_line= last_line= lineno;
}



