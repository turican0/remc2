/*
    $Id: toclexer.c,v 1.2 2008/03/23 17:21:34 karl Exp $

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

/* Lexical scanner for cdrdao's TOC. */
#include "toclexer.h"
#include "toc.tab.h"
#include <ctype.h>

#define YYEOF 0

/* A structure for associating a word with a token. */
typedef struct keyword_s
{
  char const *psz_keyword;
  token_t i_token;
} keyword_t;

/* These are all of the words that might appear in a TOC file and
   the token association that the parser will use.
*/
const keyword_t keywords[] =
  {
    {"ARRANGER",           ARRANGER},
    {"AUDIO",              AUDIO},
    {"AUDIOFILE",          AUDIOFILE},
    {"CATALOG",            CATALOG},
    {"CD_DA",              CD_DA},
    {"CD_I",               CD_I},
    {"CD_ROM",             CD_ROM},
    {"CD_ROM_XA",          CD_ROM_XA},
    {"CD_TEXT",            CD_TEXT},
    {"COMPOSER",           COMPOSER},
    {"COPY",               COPY},
    {"DATAFILE",           DATAFILE},
    {"DISC_ID",            DISC_ID},
    {"EN",                 EN},
    {"END",                END},
    {"FIFO",               FIFO},
    {"FILE",               FILE_TOKEN},
    {"FOUR_CHANNEL_AUDIO", FOUR_CHANNEL_AUDIO},
    {"GENRE",              GENRE},
    {"INDEX",              INDEX},
    {"ISRC",               ISRC},
    {"LANGUAGE",           LANGUAGE},
    {"LANGUAGE_MAP",       LANGUAGE_MAP},
    {"MESSAGE",            MESSAGE},
    {"MODE0",              MODE0},
    {"MODE1",              MODE1},
    {"MODE1_RAW",          MODE1_RAW},
    {"MODE2",              MODE2},
    {"MODE2_FORM1",        MODE2_FORM1},
    {"MODE2_FORM2",        MODE2_FORM2},
    {"MODE2_FORM_MIX",     MODE2_FORM_MIX},
    {"MODE2_RAW",          MODE2_RAW},
    {"NO",                 NO},
    {"PERFORMER",          PERFORMER},
    {"PREGAP",             PREGAP},
    {"PRE_EMPHASIS",       PRE_EMPHASIS},
    {"RESERVED1",          RESERVED1},
    {"RESERVED2",          RESERVED2},
    {"RESERVED3",          RESERVED3},
    {"RESERVED4",          RESERVED4},
    {"RW",                 RW},
    {"RW_RAW",             RW_RAW},
    {"SILENCE",            SILENCE},
    {"SIZE_INFO",          SIZE_INFO},
    {"SONGWRITER",         SONGWRITER},
    {"START",              START},
    {"SWAP",               SWAP},
    {"TITLE",              TITLE},
    {"TOC_INFO1",          TOC_INFO1},
    {"TOC_INFO2",          TOC_INFO2},
    {"TRACK",              TRACK},
    {"TWO_CHANNEL_AUDIO",  TWO_CHANNEL_AUDIO},
    {"UPC_EAN",            UPC_EAN},
    {"ZERO",               ZERO},
    {0, 0}
  };

static int 
compare_keyword(const void *p_id, const void *p_keyword) {
  char *psz_id = (char *) p_id;
  char const *psz_keyword = ((keyword_t *) p_keyword)->psz_keyword;
  return strcmp(psz_id, psz_keyword);
}

token_t
toclex (void)
{
  int c;

 start:  
  /* Skip white space.  */
  while  ( isspace(c = fgetc (toc_in)) )
    ;

  /* Process a number.  */
  if (isdigit (c))
    {
      ungetc (c, toc_in);
      fscanf (toc_in, "%lu", &(toclval.val));
      return Integer;
    }

  /* Process a comment.  */
  if ( '/' == c ) {
    if ('/' == (c = fgetc (toc_in)) ) {
      while ((c = fgetc (toc_in)) != EOF && c != '\n')
	;

      /* Return end-of-input.  */
      if (EOF == c) return YYEOF;
      goto start;
    }
    /* Not a comment. So put back the character after the '/' and
       return '/'  */
    ungetc (c, toc_in);
    return '/';
  }
  
  /* Char starts an identifier => read the name.       */
  if (isalpha (c))
    {
      static char symbol[50] = "";
      unsigned int i;
      
      i = 0;
      do
	{
	  /* Add this character to the buffer.         */
	  symbol[i++] = c;
	  /* Get another character.                    */
	  c = fgetc (toc_in);
	}
      while (isgraph (c));
      
      ungetc (c, toc_in);
      symbol[i] = '\0';

      toclval.psz_str = symbol;
      
      {
	keyword_t *p_keyword;
	p_keyword = bsearch(symbol, keywords, 
			    (sizeof(keywords) / sizeof(keyword_t)) - 1,
			    sizeof(keyword_t), compare_keyword);
	if (!p_keyword) return Id;
	return p_keyword->i_token;
      }
      
    }

  /* Process a string.  
     To do: save the value of the string and process octal numbers.
   */
  if ( '"' == c ) {
    int b_backslash = 0;
    while ( EOF != (c = fgetc (toc_in)) 
	    && (b_backslash || '"' != c ) ) {
      b_backslash = ('\\' == c );
    }

    /* Return end-of-input.  */
    if (EOF == c) return YYEOF;

    return String;
  }
  
  /* Return end-of-input.  */
  if (EOF == c) return YYEOF;

  switch (c) {
  case ':': return Colon;
  case '{': return LeftBrace;
  case '}': return RightBrace;
  default:
    /* Return a single char.  */
    return c;
  }
  
}
