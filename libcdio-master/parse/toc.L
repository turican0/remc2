/* $Id: toc.L,v 1.2 2008/03/23 17:21:34 karl Exp $ -*- C -*- */
/* cdrdao TOC scanner
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

%{
#undef yywrap
#ifdef STANDALONE
#include <stdio.h>
#endif

#include "toc.tab.h"

  static int debug_lex=0;
%}
%union {
  unsigned long int val;   /* For returning numbers.  */
  char const *      str;   /* For returning stringss.  */
}


integer          [[:digit:]]+

/* The below isn't octal. I'm just going by the name and pattern 
   in the cdrdao pacct grammar.  */
stringoctal      [[:digit:]]{3}

spaces           [[:blank:]\n\r]+ 

# Need to expand this to include "octal" \000 and embedded quotes.
string           \".+\" 
%%
{integer} {
  /*"*/
  return Integer;
}

{stringoctal} {
  /*"*/
  return StringOctal;
}

{spaces}  {
  return Spaces;
}

%%

#if STANDALONE
int
main( int argc, const char **argv )
{
  int token;
  
  ++argv, --argc;  /* skip over program name */
  debug_lex = 1;
  if ( argc > 0 )
    yyin = fopen( argv[0], "r" );
  else
    yyin = stdin;
  
  while ((token=yylex()) != EOF) {
    switch (token) {
    case StringOctal:
      printf("StringOctal\n");
      break;
    case Spaces:
      printf("Spaces\n");
      break;
    case Integer: 
      printf("Integer\n");
      break;
    default: ;
    }
  }
  return 0;
}
#endif
