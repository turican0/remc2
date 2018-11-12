/*
    $Id: toclexer.h,v 1.2 2008/03/23 17:21:34 karl Exp $

    Copyright (C) 2005, 2008 Rocky Bernstein <rocky@gnu.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Common header between TOC lexer and parser. */
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef int token_t;

extern FILE *toc_in;

typedef union {
  long unsigned int val;   /* For returning numbers.  */
  char  const *psz_str;    /* For strings.  */
} tocval_t;

#define YYSTYPE tocval_t
  
YYSTYPE toclval;

/* Call to the TOC scanner */
token_t toclex (void);



     
