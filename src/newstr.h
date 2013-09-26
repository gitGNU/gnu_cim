/* $Id: cim.c,v 1.18 1997/01/26 14:30:21 cim Exp $ */

/* Copyright (C) 1987-1997 Sverre Hvammen Johansen,
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


void init_newstr (void);
void newstr_grown(char *s1, int n);
void newstr_grow1(char *s1);
void newstr_grow2(char *s1, char *s2);
char *newstr_finish(void);
char *newstrcat1(char *s1);
char *newstrcat2(char *s1, char *s2);
char *newstrcat3(char *s1, char *s2, char *s3);
char *newstrcat4(char *s1, char *s2, char *s3, char *s4);
char *newstrcat5(char *s1, char *s2, char *s3, char *s4, char *s5);
char *newstrcat6(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6);
