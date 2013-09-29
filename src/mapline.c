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
#include "config.h"

#if STDC_HEADERS
#include <stdlib.h>
#endif

#include <obstack.h>
#include "mapline.h"

char *xmalloc();

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static struct obstack os_map, os_mapstack;


typedef struct _map map_t;
struct _map
{
  char *filename;
  long line,
    fromline;
  map_t *neste;
}
mapinit;

typedef struct _mapstack mapstack_t;
struct _mapstack
{
  long line;
  char *filename;
  void *ifdefp;
  FILE *file;
  mapstack_t *prev;
} *mapstackp;

static map_t *mappos = &mapinit;
static map_t *firstmappos = &mapinit;
static map_t *lastmappos = &mapinit;
static map_t *mapindeks = &mapinit;
static int antmap = 1;

/******************************************************************************
                                                                MAPLINEINIT  */
int mapline_init (char *sourcename, void *ifdefp)
{
  obstack_init (&os_map);
  obstack_init (&os_mapstack);
  mapinit.filename="";
  return pushfilmap (tag (sourcename), ifdefp);
}


/******************************************************************************
                                                                   NOFILMAP  */
int no_filemap (void)
{
  return mapstackp==NULL;
}


/******************************************************************************
                                                                 PUSHFILMAP  */
int pushfilmap (char *filename, void *ifdefp)
{
  FILE *file;
  mapstack_t *prev= mapstackp;
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
      mapstack_t *ms;
      
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
  mapstackp= (mapstack_t *) 
    obstack_alloc (&os_mapstack, sizeof (mapstack_t));
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
FILE *include_file (void)
{
  return mapstackp->file;
}

/******************************************************************************
                                                             INCLUDEIFDEFNIV */
void *include_ifdefp (void)
{
  return mapstackp->ifdefp;
}

/******************************************************************************
                                                                  POPFILMAP  */

void popfilmap (void)
{
  mapstack_t *prev= mapstackp->prev;
  setfilmap (mapstackp->filename, mapstackp->line);
  obstack_free (&os_mapstack, mapstackp);
  mapstackp= prev;
}

/******************************************************************************
                                                                   SETFILMAP */
void setfilmap (char *filename, long line)
{
  antmap++;
  mappos->filename = filename ? filename : lastmappos->filename;
  mappos->line = line - lineno - 1;
  mappos->fromline = lineno + 1;
  mappos = (lastmappos = mappos)->neste 
    = (map_t *) obstack_alloc (&os_map, sizeof (map_t));
  mappos->fromline = MAX_INT;
}

/******************************************************************************
  						                  GETMAPLINE */

long getmapline (long line)
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (line + mapindeks->line);
}

/******************************************************************************
  						                  GETMAPFILE */

char *getmapfile (long line)
{
  if (mapindeks->fromline > line)
    mapindeks = firstmappos;
  while (mapindeks->neste->fromline <= line)
    mapindeks = mapindeks->neste;
  return (mapindeks->filename);
}

/******************************************************************************
                                                                      GENMAP */

void genmap (void)
{
  int i;
  map_t *m = firstmappos;
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


