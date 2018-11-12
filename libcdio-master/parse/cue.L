/*  $Id: cue.L,v 1.2 2008/03/23 17:21:34 karl Exp $ -*- C -*- */
/*  CUE-sheet scanner
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

#include "cue.tab.h"

  static int debug_lex=0;
%}
%x filename

TrackDef         "TRACK"
Audio            "AUDIO"
Mode1_2048       "MODE1/2048"
Mode1_2352       "MODE1/2352"
Mode2_2336       "MODE2/2336"
Mode2_2352       "MODE2/2352"
Index            "INDEX"
File             "FILE"
Pregap           "PREGAP"
Postgap          "POSTGAP"
Binary           "BINARY"
Motorola         "MOTOROLA"
Flags            "FLAGS"
Catalog          "CATALOG"
Isrc             "ISRC"
Four_Channel     "4CH"
Dont_Copy        "DCP"
Pre_Emphasis     "PRE"
Colon            ":"
Integer          [[:digit:]]+
Spaces           [[:blank:]\n\r]+ 
String           \".+\" 
Filename         [^[:blank:]\n\r]+
%%
{TrackDef} {
  /*"*/
  return TRACK_TOKEN;
}
{Audio}  {
  return AUDIO_TOKEN;
}

{Mode1_2048} {
  return MODE1_2048_TOKEN;
}

{Mode1_2352} {
  return MODE1_2352_TOKEN;
}

{Mode2_2336} {
  return MODE2_2336_TOKEN;
}

{Mode2_2352} {
  return MODE2_2352_TOKEN;
}

{Index} {
  return INDEX_TOKEN;
}

{File} {
  BEGIN(filename);
  return FILE_TOKEN;
}

{Pregap} {
  return PREGAP_TOKEN;
}

{Postgap} {
  return POSTGAP_TOKEN;
}

{Binary} {
  return BINARY_TOKEN;
}

{Motorola} {
  return MOTOROLA_TOKEN;
}

{Flags} {
  return FLAGS_TOKEN;
}

{Catalog} {
  return CATALOG_TOKEN;
}

{Isrc} {
  if (debug_lex) printf("Isrc token\n");
}

{Four_Channel} {
  return FOURCH_TOKEN;
}

{Dont_Copy} {
  return DCP_TOKEN;
}

{Pre_Emphasis} {
  if (debug_lex) printf("Pre_Emphasis token\n");
}

{Colon} {
  return COLON_TOKEN;
}

{Integer} {
  return INTEGER_TOKEN;
}

<filename>{Filename} {
  BEGIN(INITIAL);
  return FILENAME_TOKEN;
}

<INITIAL,filename>{String} {
  return STRING_TOKEN;
}

<INITIAL,filename>{Spaces} {
  return SPACES_TOKEN;
}

<INITIAL,filename><<EOF>> {
  return EOF;
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
    case TRACK_TOKEN:
      printf("TRACK\n");
      break;
    case AUDIO_TOKEN:
      printf("AUDIO token\n");
      break;
    case MODE1_2048_TOKEN: 
      printf("MODE1/2048\n");
      break;
    case MODE1_2352_TOKEN:
      printf("MODE1/2352\n");
      break;
    case MODE2_2336_TOKEN:
      printf("MODE1/2336\n");
      break;
    case MODE2_2352_TOKEN:
      printf("MODE2/2352\n");
      break;
    case INDEX_TOKEN:
      printf("INDEX\n");
      break;
    case FILE_TOKEN:
      printf("FILE\n");
      break;
    case PREGAP_TOKEN:
      printf("PREGAP\n");
      break;
    case POSTGAP_TOKEN:
      printf("POSTGAP\n");
      break;
    case BINARY_TOKEN:
      printf("BINARY\n");
      break;
    case SPACES_TOKEN:
      printf("spaces\n");
      break;
    case STRING_TOKEN:
      printf("string: %s\n", yytext);
      break;
    case INTEGER_TOKEN:
      printf("integer: %d (%s)\n", atoi(yytext), yytext);
      break;
    case COLON_TOKEN:
      printf(":\n");
      break;
    case FLAGS_TOKEN:
      printf("FLAGS\n");
      break;
    case CATALOG_TOKEN:
      printf("CATALOG\n");
      break;
    case DCP_TOKEN:
      printf("DCP\n");
      break;
    case MOTOROLA_TOKEN:
      printf("MOTOROLA\n");
      break;
    case FOURCH_TOKEN:
      printf("4CH\n");
      break;
    case FILENAME_TOKEN:
      printf("filename %s\n", yytext);
      break;
    default: ;
    }
  }
  return 0;
}
#endif
