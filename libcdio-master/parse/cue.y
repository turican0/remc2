%{
/*  $Id: cue.y,v 1.2 2008/03/23 17:21:34 karl Exp $

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

#include <stdio.h>
#include <stdlib.h>
extern FILE *cuein;
int cuelex(void);
int cueerror(char *s);

#ifdef STANDALONE
#include <getopt.h>
#define YYDEBUG 1
#endif
  
%}

/* BISON Declarations */
%token TRACK_TOKEN
%token AUDIO_TOKEN
%token MODE1_2048_TOKEN
%token MODE1_2352_TOKEN
%token MODE2_2336_TOKEN
%token MODE2_2352_TOKEN
%token INDEX_TOKEN
%token FILE_TOKEN
%token PREGAP_TOKEN
%token POSTGAP_TOKEN
%token BINARY_TOKEN
%token MOTOROLA_TOKEN
%token FLAGS_TOKEN
%token CATALOG_TOKEN
%token ISRC_TOKEN
%token FOURCH_TOKEN
%token DCP_TOKEN
%token PRE_TOKEN
%token COLON_TOKEN
%token INTEGER_TOKEN
%token SPACES_TOKEN
%token STRING_TOKEN
%token FILENAME_TOKEN

 /* Grammar follows */
%%

 /* We optionally allow spaces at the end of the file.
 */
cue: cue_stmts opt_spaces ;

/* one or more cuesheet statements. We allow spaces before the beginning
   of the cuesheet statement */
cue_stmts: cue_stmts SPACES_TOKEN cue_stmt | opt_spaces cue_stmt ;

cue_stmt: catalog_stmt
        | file_stmt
        | track_stmt
        | flags_stmt
        | pregap_stmt
        | postgap_stmt
        | index_stmt
 ;


/* FILE *filename* {BINARY|MOTOROLA|AUDIO}  */
file_stmt: FILE_TOKEN SPACES_TOKEN file_id SPACES_TOKEN file_flags
;

file_id: FILENAME_TOKEN | STRING_TOKEN 
;


file_flags: BINARY_TOKEN | MOTOROLA_TOKEN | AUDIO_TOKEN
;

/* CATALOG *n*  */
catalog_stmt: CATALOG_TOKEN INTEGER_TOKEN
;

/* TRACK *n* {MODE1/2048 | MODE1/2352 | MODE2/2336 | MODE2/2352}  */
track_stmt: TRACK_TOKEN SPACES_TOKEN INTEGER_TOKEN SPACES_TOKEN track_mode_exp
;

track_mode_exp: MODE1_2048_TOKEN
              | MODE1_2352_TOKEN
              | MODE2_2336_TOKEN
              | MODE2_2352_TOKEN
;

/* FLAGS {DCP | 4CH | PRE}  */
flags_stmt: FLAGS_TOKEN SPACES_TOKEN flags_exp
;

flags_exp: DCP_TOKEN | FOURCH_TOKEN | PRE_TOKEN
;

/* PREGAP dd:dd:ddx  */
pregap_stmt: PREGAP_TOKEN SPACES_TOKEN msf_exp
;

/* PROSTGAP dd:dd:dd  */
postgap_stmt: POSTGAP_TOKEN SPACES_TOKEN msf_exp
;

/* INDEX *n* dd:dd:dd  */
index_stmt: INDEX_TOKEN SPACES_TOKEN INTEGER_TOKEN SPACES_TOKEN msf_exp
;

/* a MSF e.g. 00:00:00 or 02:00 */
msf_exp: INTEGER_TOKEN COLON_TOKEN INTEGER_TOKEN COLON_TOKEN INTEGER_TOKEN 
       | INTEGER_TOKEN COLON_TOKEN INTEGER_TOKEN 
;


opt_spaces: SPACES_TOKEN | ;

%%

#ifdef STANDALONE
/* The controlling function */

int 
cueerror(char *s)	/* called by cueparse on error */
{
	printf("%s\n",s);
	return(0);
}

int
main( int argc, char **argv )
{
  int c;

  cuedebug = 0;
  
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"debug", 0, 0, 'd'},
      {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "d", long_options, &option_index);
    if (c == -1)
      break;
    
    switch (c) {
    case 'd':
      cuedebug = 1;
      break;
    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      exit(1);
    }
  }
  
  if ( optind < argc )
    cuein = fopen( argv[optind], "r" );
  else
    cuein = stdin;
  
  if (cueparse()==0) {
    printf("Is a CUE file\n");
  } else {
    printf("Isn't a CUE file\n");
  }
  exit(0);
}
#endif /* STANDALONE*/
