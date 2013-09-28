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

#include "config.h"

#if DUMP
#ifdef FIRST_DATA_LOCATION

#include "cim.h"
#include <stdio.h>

extern __start_data_segment;
#if HAVE_FCNTL_H
#include <fcntl.h>
#else
#include <sys/file.h>
#endif

/******************************************************************************
                                                       DUMP & UNDUMP         */

/*  Disse prosedyrene kan n}es fra simula ved kall p} Dump og Undump.	
 *  Disse gir mulighet for } lagre en tilstand til en simulakj|ring til fil,
 *  for senere } laste tilstanden inn igjen med undump. For at dette skal g}
 *  bra m} man passe p} at alle filer untatt sysin og sysout er lukket.	
 *  ]pne filer er ikke noe problem for dump, men etter undump s}
 *  starter systemet i en tilstand hvor den tror at et antall filer	
 *  er }pne. Det kan kanskje g} bra hvis de samme filene ogs} var	
 *  }pne f|r kall p} undump, men man m} da v{re sv{rt n|ye med rekkef|lgen
 *  disse filene er }pnet, slik at riktig fildeskriptor blir		
 *  assosiert til riktig fil etter undump.				
 *				
 *  Syntaksen p} dump-filen er som f|lger:    				
 *  Filen starter med to linjer der det p} f|rste linje st}r "/\*Cim_dump*\/".
 *  P} andre linje er det skrevet adressen til |vre kanten p} data-segmentet.
 *  Nedre del av datasegmentet finnes					
 *  ved } ta adressen til variabelen __start_data_segment,		
 *  som er en initialisert variabel som er deklarert f|rst i main-modul	
 *  Deretter kommer innholdet av selve data-segmentet byte for byte,	
 *  men slik at null byte er pakket: For hver ^@ p} filen angir neste	
 *  byte antallet null byte pluss en.			
 *  Deretter f|lger sekvensen "\377\376\375" ... "^c^b^a".		
 *  Til slutt er det fylt opp med bytes opp til en full blokkst|rrelse.	
 *  Disse bytene er en n|yaktig kopi av de i forrige blokk.		
 *  Blokkst|rrelsen er satt til 4096 bytes */

#define DUMP_STR "/*Cim_dump*/\n%12d\n"
#define DUMP_STR_LEN 26
#define DUMP_BUFFER_SIZE 4096

void __rdump (__txtvp t;int ret;void (*mret) ())
{
  char s[DUMP_BUFFER_SIZE];
  long sp,
    p,
    anull = 0;
  char c;
  long f,
    first_loc = FIRST_DATA_LOCATION,
    last_loc = (long) ((char *) sbrk (0) + 4);
  __goto.ent = ret;
  __goto.ment = mret;
  __rgbc ();
  if ((f = open (__rcopytexttoc (t), O_WRONLY | O_CREAT | O_TRUNC, 0700)) 
      == EOF)
    {
      __rwarning ("Dump: Not possible to open dump-file");
      return;
    }
  sprintf (s, DUMP_STR, last_loc);
  sp = DUMP_STR_LEN;
  for (p = first_loc; p < last_loc;)
    {
      c = *(char *) p;
      if (anull)
	if (c != '\0' || anull == 256)
	  {
	    c = (char) (anull - 1);
	    anull = 0;
	  }
	else
	  {
	    anull++;
	    p++;
	    continue;
	  }
      else
	{
	  if (c == '\0')
	    anull++;
	  p++;
	}
      s[sp++] = c;
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
	    {
	      __rwarning ("Dump: Error on writing to dump-file");
	      close (f);
	      return;
	    }
	  sp = 0;
	}
    }
  if (anull)
    s[sp++] = (char) (anull - 1);
  for (c = '\377'; c != '\0'; c--)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
	    {
	      __rwarning ("Dump: Error on writing to dump-file");
	      close (f);
	      return;
	    }
	  sp = 0;
	}
      s[sp++] = c;
    }
  if (write (f, s, DUMP_BUFFER_SIZE) == EOF)
    __rwarning ("Dump: Error on writing to dump-file");
  close (f);
}

void __rundump (__txtvp t;int ret;void (*mret) ())
{

  char s[DUMP_BUFFER_SIZE];
  long sp,
    p,
    skriv_nuller = __FALSE;
  char c;
  long f,
    first_loc = FIRST_DATA_LOCATION,
    last_loc;
  if ((f = open (__rcopytexttoc (t), O_RDONLY, 0)) == EOF)
    __rerror ("Rundump: Not possible to open dump-file");
  if (read (f, s, DUMP_BUFFER_SIZE) < 0)
    __rerror ("Rundump: Error while reading from dump-file");
  if (sscanf (s, DUMP_STR, &last_loc) < 0)
    __rerror ("Rundump: Illegal format on dump-file");
  brk (last_loc);
  sp = DUMP_STR_LEN;
  for (p = first_loc; p < last_loc;)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (read (f, s, DUMP_BUFFER_SIZE) == EOF)
	    __rerror ("Rundump: Error while reading from dump-file");
	  sp = 0;
	}
      c = s[sp++];

      if (skriv_nuller)
	{
	  char i;
	  for (i = '\0'; i != c; i++)
	    {
	      *(char *) p = '\0';
	      p++;
	      if (p == last_loc)
		__rerror ("Rundump: Dump-file don't match data-region");
	    }
	  *(char *) p = '\0';
	  p++;
	  skriv_nuller = __FALSE;
	}
      else if (c == '\0')
	skriv_nuller = __TRUE;
      else
	{
	  *(char *) p = c;
	  p++;
	}
    }
  for (c = '\377'; c != '\0'; c--)
    {
      if (sp == DUMP_BUFFER_SIZE)
	{
	  if (read (f, s, DUMP_BUFFER_SIZE) == EOF)
	    __rerror ("Rundump: Error while reading from dump-file");
	  sp = 0;
	}
      if (s[sp++] != c)
	__rerror ("Rundump: Dump-file don't match data-region");
    }

  close (f);
}

#endif
#endif
