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

/******************************************************************************
                                                                  NEWSTRCAT */

char * newstrcat1(s1)
     char *s1;
{
  char *str= (char *) xmalloc (strlen(s1) + 1);
  strcpy (str, s1);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT2 */

char * newstrcat2(s1, s2)
     char *s1, *s2;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT3 */

char * newstrcat3(s1, s2, s3)
     char *s1, *s2, *s3;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT4 */

char * newstrcat4(s1, s2, s3, s4)
     char *s1, *s2, *s3, *s4;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT5 */

char * newstrcat5(s1, s2, s3, s4, s5)
     char *s1, *s2, *s3, *s4, *s5;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT6 */

char * newstrcat6(s1, s2, s3, s4, s5, s6)
     char *s1, *s2, *s3, *s4, *s5, *s6;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT7 */

char * newstrcat7(s1, s2, s3, s4, s5, s6, s7)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT8 */

char * newstrcat8(s1, s2, s3, s4, s5, s6, s7, s8)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  return str;
}

/******************************************************************************
                                                                  NEWSTRCAT9 */

char * newstrcat9(s1, s2, s3, s4, s5, s6, s7, s8, s9)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + strlen(s9) + 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  strcat (str, s9);
  return str;
}

/******************************************************************************
                                                                 NEWSTRCAT10 */

char * newstrcat10(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + strlen(s9) + 
			       strlen(s10)+ 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  strcat (str, s9);
  strcat (str, s10);
  return str;
}

/******************************************************************************
                                                                 NEWSTRCAT11 */

char * newstrcat11(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + strlen(s9) + 
			       strlen(s10)+ strlen(s11)+ 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  strcat (str, s9);
  strcat (str, s10);
  strcat (str, s11);
  return str;
}

/******************************************************************************
                                                                 NEWSTRCAT12 */

char * newstrcat12(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11, *s12;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + strlen(s9) + 
			       strlen(s10)+ strlen(s11)+ strlen(s12)+ 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  strcat (str, s9);
  strcat (str, s10);
  strcat (str, s11);
  strcat (str, s12);
  return str;
}

/******************************************************************************
                                                                 NEWSTRCAT13 */

char * newstrcat13(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13)
     char *s1, *s2, *s3, *s4, *s5, *s6, *s7, *s8, *s9, *s10, *s11, *s12, *s13;
{
  char *str= (char *) xmalloc (strlen(s1) + strlen(s2) + strlen(s3) +
			       strlen(s4) + strlen(s5) + strlen(s6) + 
			       strlen(s7) + strlen(s8) + strlen(s9) + 
			       strlen(s10)+ strlen(s11)+ strlen(s12)+ 
			       strlen(s13)+ 1);
  strcpy (str, s1);
  strcat (str, s2);
  strcat (str, s3);
  strcat (str, s4);
  strcat (str, s5);
  strcat (str, s6);
  strcat (str, s7);
  strcat (str, s8);
  strcat (str, s9);
  strcat (str, s10);
  strcat (str, s11);
  strcat (str, s12);
  strcat (str, s13);
  return str;
}
