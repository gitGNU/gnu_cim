/* $Id: $ */

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

#define UP re->up
#define UPUP re->up->up
#define LEFT re->left
#define RIGHT re->right
#define RIGHTLEFT re->right->left
#define RIGHTRIGHT re->right->right
#define RIGHTRIGHTLEFT re->right->right->left

#define UPTOKEN re->up->token
#define UPUPTOKEN re->up->up->token
#define LEFTTOKEN re->left->token
#define LEFTRIGHTTOKEN re->left->right->token
#define RIGHTTOKEN re->right->token
#define TOKEN re->token

#define UPVALUE re->up->value
#define LEFTVALUE re->left->value
#define LEFTRIGHTVALUE re->left->right->value
#define RIGHTVALUE re->right->value
#define VALUE re->value

#define UPTYPE re->up->type
#define LEFTTYPE re->left->type
#define RIGHTTYPE re->right->type
#define RIGHTLEFTTYPE re->right->left->type
#define RIGHTRIGHTLEFTTYPE re->right->right->left->type
#define TYPE re->type

#define UPRD re->up->rd
#define UPUPRD re->up->up->rd
#define LEFTRD re->left->rd
#define LEFTRIGHTRD re->left->right->rd
#define RIGHTRD re->right->rd
#define RD re->rd

#define UPQUAL re->up->qual
#define UPUPQUAL re->up->up->qual
#define LEFTQUAL re->left->qual
#define LEFTLEFTQUAL re->left->left->qual
#define LEFTRIGHTQUAL re->left->right->qual
#define RIGHTQUAL re->right->qual
#define QUAL re->qual

#define SEENTHROUGH re->seenthrough
#define LEFTSEENTHROUGH re->left->seenthrough
#define LEFTLEFTSEENTHROUGH re->left->left->seenthrough

#define ISLEFT re->up->left==re
#define UPISLEFT re->up->up->left==re->up
#define ISRIGHT re->up->right==re

#undef DANGER
#define DANGER re->danger
#define KONST re->konst
