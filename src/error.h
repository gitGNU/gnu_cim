/* $Id: cfeil.c,v 1.19 1995/03/07 10:53:16 cim Exp $ */

/* Copyright (C) 1994, 1998 Sverre Hvammen Johansen,
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

/* Inneholder de tekstlige feilmeldingene som kompilatoren kan gi.
 * For noen tilfeller b|r det gis bedre og mer spesifike feilmeldinger.
 * Dette gjelder spesielt for feilmeldinger fra sjekkeren.
 *                                                              
 * Siden det kan inkluderes filer m} det lages et tabellverk som
 * holder greie p} hvilke linjenummere internt i kompilator-programmet
 * som h|rer til de enkelte filene.                              
 * Dette tabellverket brukes s} i forbindelse med utskrift              
 * av feilmeldinger. */

#include "dekl.h"

extern int anterror;
void lerror (int errcode);
void yerror (int errcode, char *txt);
void d1error (int errcode, char *name);
void d2error (int errcode, decl_t *rd1, decl_t *rd2);
char *textnumber (int i);
void serror (int errcode, char *name, int ant);
void gerror (int errcode, char *name);
void merror (int errcode, char *name);
