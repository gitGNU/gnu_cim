/* $Id: cextspec.c,v 1.17 1997/01/26 14:30:17 cim Exp $ */

/* Copyright (C) 1994 Sverre Hvammen Johansen,
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


#include "config.h"
#include "const.h"
#include "filelist.h"

typedef struct _filelist filelist_t;
typedef struct _fileelem fileelem_t;

struct _fileelem
{
  fileelem_t *next;
  char *name;
};

struct _filelist
{
  fileelem_t *first;
  fileelem_t *last;
  int max_strlen;
  int total_strlen; /* Total strlen including one ws between each emement */
};

static filelist_t dirlist, archlist, linklist;

/******************************************************************************
                                                                  CLEAR_LIST */

clear_list (listp) filelist_t *listp;
{
  listp->first=listp->last=0;
  listp->max_strlen=0;
  listp->total_strlen=0;
}
/******************************************************************************
                                                                     IS_NAME */

char is_name (listp, name) filelist_t *listp; char *name;
{
  fileelem_t *elem;
  for (elem= listp->first; elem!=NULL; elem= elem->next)
    if (!strcmp (name, elem->name)) return (1);
  return (0);
}

/******************************************************************************
                                                                   GET_NAMES */

char *get_names (listp) filelist_t *listp;
{
  fileelem_t *elem;
  char *str;
  str = (char *) xmalloc (listp->total_strlen + 1);
  strcpy (str, "\0");
  for (elem= listp->first; elem!=NULL; elem= elem->next)
    {
      strcat (str, elem->name);
      if (elem->next != NULL) strcat (str, " ");
    }
  return str;
}

/******************************************************************************
                                                       GET_NAMES_IN_LINKLIST */

char * get_names_in_linklist ()
{
  return get_names(&linklist);
}

/******************************************************************************
                                                                 INSERT_NAME */

char insert_name (listp, name, first) filelist_t *listp; char *name, first;
{
  fileelem_t *new;
  new= (fileelem_t *) xmalloc(sizeof (fileelem_t));
  new->next= NULL;
  new->name= name;

  if (is_name(listp, name)) return FALSE;

  if (listp->first == NULL)
    {
      listp->first= listp->last= new;
      listp->max_strlen= listp->total_strlen= strlen(name);
    }
  else
    {
      int this_strlen;


      this_strlen= strlen(name);
      listp->total_strlen+= this_strlen+1;
      if (this_strlen > listp->max_strlen) listp->max_strlen= this_strlen;

      if (first)
	{
	  new->next= listp->first;
	  listp->first= new;
	} 
      else
	{
	  listp->last= listp->last->next= new;
	}
    }
  return TRUE;
}

/******************************************************************************
                                                      INSERT_NAME_IN_DIRLIST */

char insert_name_in_dirlist (name) char *name;
{
  char *str;
  if (name==NULL)
    {
      clear_list (&dirlist);
    }
  else
    {
      str= (char *) xmalloc (strlen (name) + 3);
      strcpy (str, "-L");
      strcat (str, name);
      insert_name (&linklist, str, FALSE);
      return insert_name (&dirlist, name, FALSE);
    }
}

/******************************************************************************
                                                     INSERT_NAME_IN_ARCHLIST */

char insert_name_in_archlist (name) char *name;
{
  return insert_name (&archlist, name, FALSE);
}

/******************************************************************************
                                                     INSERT_NAME_IN_LINKLIST */

char insert_name_in_linklist (name, first) char *name, first;
{
  return insert_name (&linklist, name, first);
}

/******************************************************************************
                                                              TRANSFORM_NAME */

char * transform_name(name, fromsuffix, tosuffix) 
  char *name, *fromsuffix, *tosuffix;
{
  char *str;
  int name_len= strlen(name);
  int fromsuffix_len= strlen(fromsuffix);
  int tosuffix_len= strlen(tosuffix);
  str = (char *) xmalloc(name_len-fromsuffix_len+tosuffix_len+1);
  strncpy (str, name, name_len-fromsuffix_len);
  str[name_len-fromsuffix_len]= '\0';
  strcat (str, tosuffix);
  return (str);
}

/******************************************************************************
                                                                   OPEN_NAME */

FILE *open_name (dirlist, linklist, name, link) 
  filelist_t *dirlist, *linklist; char *name; char link;
{
  FILE *f;
  fileelem_t *elem;
  char *str;
  str = (char *) xmalloc (dirlist->max_strlen + strlen(name) + 2);
  for (elem= dirlist->first; elem!=NULL; elem= elem->next)
    {
      strcpy (str, elem->name);
      strcat (str, "/");
      strcat (str, name);
#if OPEN_FILE_IN_BINARY_MODE
      if ((f = fopen (str, "rb")) != NULL)
#else
      if ((f = fopen (str, "r")) != NULL)
#endif
	{
	  if (link) 
	    insert_name (linklist, transform_name (str, ".atr", ".o"), TRUE);
	  return (f);
	}
    }
  return (NULL);
}

/******************************************************************************
                                                             SHORT_FILE_NAME */

char * short_file_name (f)
     FILE *f;
{
  int i,
    c;
  static char s[17];
  for (i = 0; i < 16; i++)
    s[i] = getc (f);
  for (i = 15; i >= 0; i--)
    if (s[i] != ' ')
      break;
  if (i >= 0 && s[i] != '/')
    i++;
  s[i] = '\0';
  if ((c = getc (f)) != 0)
    ungetc (c, f);
  if (c == EOF)
    return (NULL);
  else
    return (s);
}

/******************************************************************************
                                                 OPEN_AND_POSITION_ARCH_NAME */

FILE *open_and_position_arch_name (archname, name) char *archname, *name;
{
  FILE *f;
  static char *string_table;
  static int string_table_length;
  int i,
    sl;
  long l;
  char s1[17], *s2, c;

#if OPEN_FILE_IN_BINARY_MODE
  f = fopen (archname, "rb");
#else
  f = fopen (archname, "r");
#endif
  if (f == NULL)
    merror (6, archname);
  (void) fscanf (f, "%7s", s1);
  getc (f);
  if (strcmp (s1, "!<arch>"))
    merror (7, archname);
  while ((s2 = short_file_name (f)) != NULL)
    {
      (void) fscanf (f, "%12ld", &l);
      (void) fscanf (f, "%6ld", &l);
      (void) fscanf (f, "%6ld", &l);
      (void) fscanf (f, "%8ld", &l);
      (void) fscanf (f, "%10ld", &l);
      while ((c = getc (f)) != '`' && c != EOF);
      if (c != '`' || getc (f) != '\n')
	merror (8, archname);
      if (!strcmp (s2, name))
	return (f);
      if (s2[0] == '/' && isdigit(s2[1]))
	{
	  long pos;
	  sscanf(&s2[1],"%ld",&pos);
	  if (!strcmp(&string_table[pos], name))
	    return (f);
	}
      if (l & 1)
	l++;
      if(!strcmp(s2,"/"))
	{
	  int pos;
	  if (string_table_length<l)
	    string_table = (char *) xmalloc (l);
	  for (pos=0; pos<l; pos++)
	    {
	      char c;
	      c = getc (f);
	      if (c == '/' || c == '\n')
		string_table[pos] = '\0';
	      else string_table[pos] = c;
	    }
	} else
	  {
#if NO_SEEK_IN_AR
	    while (l-- > 0)
	      getc (f);
#else
	    (void) fseek (f, l, 1);
#endif
	  }
    }
  (void) fclose (f);
  return (NULL);
}

/******************************************************************************
                                             SEARC_AND_OPEN_NAME_IN_ARCHLIST */

FILE *searc_and_open_name_in_archlist (name, link)
char *name; char link;
{
  FILE *f;
  fileelem_t *elem;
#if OPEN_FILE_IN_BINARY_MODE
  if ((f = fopen (name, "rb")) != NULL)
#else
  if ((f = fopen (name, "r")) != NULL)
#endif
    {
      if (link) insert_name (&linklist, transform_name(name,".atr",".o"), TRUE);
      return (f);
    }
  
  f=open_name (&dirlist, &linklist, name, link);

  for (elem= archlist.first; elem!=NULL; elem= elem->next)
    if ((f= open_and_position_arch_name (elem->name, name)) != NULL) 
      return(f);

  return (NULL);
}

/******************************************************************************
                                                       SEARC_AND_INSERT_NAME */

char searc_and_insert_name (dirlistp, listp, name) 
  filelist_t *dirlistp, *listp; char *name;
{
  FILE *f;
  fileelem_t *elem;
  char *str;
  str = (char *) xmalloc (dirlistp->max_strlen + strlen(name) + 2);
  for (elem= dirlistp->first; elem!=NULL; elem= elem->next)
    {
      strcpy (str, elem->name);
      strcat (str, "/");
      strcat (str, name);
      if ((f = fopen (str, "r")) != NULL)
	{
	  insert_name (listp, str, FALSE);
	  fclose(f);
	  return TRUE;
	}
    }
  return FALSE;
}

/******************************************************************************
                                                                     NEW_LIB */

#define LIBPREFIX "lib"
#define LIBSUFFIX ".a"
#define LIBARCHSUFFIX "-atr.a"

void new_lib (name) 
  char *name;
{
  char *str;
  str= (char *) 
    xmalloc (strlen(name) + strlen(LIBPREFIX) + strlen (LIBSUFFIX) + 1);
  strcpy (str, LIBPREFIX);
  strcat (str, name);
  strcat (str, LIBSUFFIX);

  searc_and_insert_name (&dirlist, &archlist, transform_name (str, LIBSUFFIX, LIBARCHSUFFIX));
#if 0
  searc_and_insert_name (&dirlist, &linklist, str);
#else
  strcpy (str, "-l");
  strcat (str, name);
  insert_name (&linklist, str, FALSE);
#endif
}




