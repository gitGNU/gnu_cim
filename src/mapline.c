/* $Id: cfeil.c,v 1.19 1995/03/07 10:53:16 cim Exp $ */

/* Copyright (C) 1998 Sverre Hvammen Johansen,
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

#include "mapline.h"
#include "const.h"
#include "cimcomp.h"
#include "lex.h"
#include "extspec.h"
#include "newstr.h"
#include "filelist.h"
#include <obstack.h>

char *xmalloc();
void free();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack osMap, osMapstack;


struct map
{
  char *filename;
  long line,
    fromline;
  struct map *neste;
}
mapinit;

struct mapstack
{
  long line;
  char *filename;
  void *ifdefp;
  FILE *file;
  struct mapstack *prev;
} *mapstackp;

static struct map *mappos = &mapinit;
static struct map *firstmappos = &mapinit;
static struct map *lastmappos = &mapinit;
static struct map *mapindeks = &mapinit;
static int antmap = 1;

/******************************************************************************
                                                                MAPLINEINIT  */
int maplineInit (sourcename, ifdefp) char *sourcename; void *ifdefp;
{
  obstack_init (&osMap);
  obstack_init (&osMapstack);
  mapinit.filename="";
  return pushfilmap (tag (sourcename), ifdefp);
}


/******************************************************************************
                                                                   NOFILMAP  */
int
noFilemap ()
{
  return mapstackp==NULL;
}


/******************************************************************************
                                                                 PUSHFILMAP  */
int
pushfilmap (filename, ifdefp)
     char *filename;
     void *ifdefp;
{
  FILE *file;
  struct mapstack *prev= mapstackp;
  if (mapstackp == NULL)
    {
      file= fopen (filename, "r");
      if (file == NULL)
	{
	  perror (newstrcat3 (progname, ": ", filename));
	  return TRUE;
	}
    } 
  else
    {
      struct mapstack *ms;
      
      for (ms= mapstackp; ms != NULL; ms= ms->prev)
	{
	  if (!strcmp (filename, ms->filename))
	    {
	      lerror (17);
	      return TRUE;
	    }
	}
      file = searc_and_open_name_in_archlist (filename, FALSE);
      if (file == NULL)
	{
	  lerror (18);
	  return TRUE;
	}
      if (option_verbose)
	fprintf (stderr, "Reading include file %s\n", filename);

    }
  mapstackp= (struct mapstack *) 
    obstack_alloc (&osMapstack, sizeof (struct mapstack));
  mapstackp->line= lineno + 1 + lastmappos->line;
  mapstackp->filename= lastmappos->filename;
  mapstackp->ifdefp= ifdefp;
  mapstackp->file= file;
  mapstackp->prev= prev;
  setfilmap (filename, 1);
  return FALSE;
}

/******************************************************************************
                                                                 INCLUDEFILE */
FILE *
includeFile ()
{
  return mapstackp->file;
}

/******************************************************************************
                                                             INCLUDEIFDEFNIV */
void *
includeIfdefp ()
{
  return mapstackp->ifdefp;
}

/******************************************************************************
                                                                  POPFILMAP  */

void
popfilmap ()
{
  struct mapstack *prev= mapstackp->prev;
  setfilmap (mapstackp->filename, mapstackp->line);
  obstack_free (&osMapstack, mapstackp);
  mapstackp= prev;
}

/******************************************************************************
                                                                   SETFILMAP */
void
setfilmap (filename, line)
     char *filename;
     long line;
{
  antmap++;
  mappos->filename = filename ? filename : lastmappos->filename;
  mappos->line = line - lineno - 1;
  mappos->fromline = lineno + 1;
  mappos = (lastmappos = mappos)->neste 
    = (struct map *) obstack_alloc (&osMap, sizeof (struct map));
  mappos->fromline = MAX_INT;
}

/******************************************************************************
  						                  GETMAPLINE */

long 
getmapline (line)
     long line;
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (line + mapindeks->line);
}

/******************************************************************************
  						                  GETMAPFILE */

char *
getmapfile (line)
     long line;
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (mapindeks->filename);
}

/******************************************************************************
                                                                      GENMAP */

void 
genmap ()
{
  int i;
  struct map *m = firstmappos;
  fprintf (ccode, "__map __map%s[%d]={"
	   ,separat_comp ? timestamp : "main", antmap);
  for (i = 1; i < antmap; i++)
    {
      fprintf (ccode, "\"%s\",%ldL,%ldL,\n", m->filename, 
	       m->line, m->fromline);
      m = m->neste;
    }
  fprintf (ccode, "\"\",0L,%ldL};\n", MAX_INT);
}


