/*
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
/* Yacc grammer for cdrdao TOC file */
%{
#include "toclexer.h"
#include "errno.h"
FILE *toc_in;
int tocerror (char const *s);

#ifdef STANDALONE
#include <getopt.h>
#define YYDEBUG 1
#endif

%}
     
/* BISON Declarations */

%token ARRANGER
%token AUDIO
%token AUDIOFILE
%token CATALOG
%token CD_DA
%token CD_I
%token CD_ROM
%token CD_ROM_XA
%token CD_TEXT
%token COMPOSER
%token COPY
%token DATAFILE
%token DISC_ID
%token EN
%token END
%token FIFO
%token FILE_TOKEN
%token FOUR_CHANNEL_AUDIO
%token GENRE
%token INDEX
%token ISRC
%token LANGUAGE
%token LANGUAGE_MAP
%token MESSAGE
%token MODE0
%token MODE1
%token MODE1_RAW
%token MODE2
%token MODE2_FORM1
%token MODE2_FORM2
%token MODE2_FORM_MIX
%token MODE2_RAW
%token NO
%token PERFORMER
%token PREGAP
%token PRE_EMPHASIS
%token RESERVED1
%token RESERVED2
%token RESERVED3
%token RESERVED4
%token RW
%token RW_RAW
%token SILENCE
%token SIZE_INFO
%token SONGWRITER
%token START
%token SWAP
%token TITLE
%token TOC_INFO1
%token TOC_INFO2
%token TRACK
%token TWO_CHANNEL_AUDIO
%token UPC_EAN
%token ZERO

%token LeftBrace  /* "{" */
%token RightBrace  /* "}" */
%token Colon  /* ":" */
%token Error  /* Error token return */
%token Id     /* Id but not one of the above keywords */
%token Integer
%token String

%union {
  long unsigned int val;   /* For returning numbers.  */
  symrec  *tptr;   /* For returning symbol-table pointers.  */
}

/* Grammar follows */
%%

/* We optionally allow spaces at the end of the TOC file.
 */
toc: catalog_or_tocType cdTextGlobal tracks ;

catalog_or_tocType: catalog_or_tocType CATALOG String 
                    | catalog_or_tocType tocType
                    | /* empty */ ;

tracks: tracks track | track ;

track: TRACK trackMode opt_subChannelMode opt_track_flags
    cdTextTrack opt_pregap_msf subTracks_or_starts_or_ends opt_index_msfs
    ;

opt_track_flags: opt_track_flags track_flag 
       | /* empty */;

track_flag: ISRC String 
          | opt_no COPY 
          | opt_no PRE_EMPHASIS
          | TWO_CHANNEL_AUDIO  
          | FOUR_CHANNEL_AUDIO ;

opt_no: NO 
          | /* empty */;

opt_pregap_msf: PREGAP msf 
         | /* empty  */;

opt_index_msfs: opt_index_msfs INDEX msf 
         | /* empty */ ;

subTrack_or_start_or_end: subTrack 
         | START opt_msf 
         | END opt_msf ;

subTracks_or_starts_or_ends: subTracks_or_starts_or_ends 
                             subTrack_or_start_or_end 
                           | subTrack_or_start_or_end ;

subTrack: 
    AudioFile String opt_swap opt_start_offset samples
     | DATAFILE String opt_start_length
     | FIFO String dataLength 
     | SILENCE samples 
     | ZERO opt_dataMode opt_subChannelMode dataLength 
 ;

AudioFile: AUDIOFILE | FILE_TOKEN ;

opt_swap: SWAP 
     | /* empty */;

opt_start_offset: "#" sLong 
     | /* empty */;

opt_start_length: "#" sLong 
     | '#' sLong dataLength 
     | /* empty */;

opt_dataMode: dataMode 
     | /* empty */ ;

opt_string: String 
     | /* empty */  ;

uLong: Integer ;

sLong: Integer ;

msf: Integer Colon Integer Colon Integer ;

opt_msf: msf | /* empty */ ;

samples: msf | uLong ;

dataLength:  msf | uLong  ;

dataMode: AUDIO | MODE0 | MODE1 | MODE1_RAW | MODE2
     | MODE2_RAW | MODE2_FORM1 | MODE2_FORM2 | MODE2_FORM_MIX
     ;


trackMode:  AUDIO | MODE1 | MODE1_RAW | MODE2
     | MODE2_RAW  | MODE2_FORM1 | MODE2_FORM2 | MODE2_FORM_MIX
     ;


opt_subChannelMode: RW | RW_RAW 
     | /* empty  */;

tocType: CD_DA | CD_ROM | CD_ROM_XA | CD_I ;

packType:  TITLE | PERFORMER | SONGWRITER | COMPOSER | ARRANGER  
     | MESSAGE | DISC_ID | GENRE | TOC_INFO1 | TOC_INFO2  
     | RESERVED1 | RESERVED2 | RESERVED3 | RESERVED4 | UPC_EAN
     | ISRC | SIZE_INFO ;


binaryData:  LeftBrace Integers RightBrace  ;

Integers:  Integers "," Integer | Integer ;

         
cdTextItem: packType opt_string_or_binaryData | ;

opt_string_or_binaryData: opt_string | binaryData ;
 
cdTextBlock:  LANGUAGE Integer LeftBrace cdTextItem RightBrace ;

opt_cdTextBlock:  cdTextBlock 
         | /* empty */;

opt_cdTextBlocks:  opt_cdTextBlocks cdTextBlock 
         | /* empty */ ;

opt_cdTextLanguageMap : LANGUAGE_MAP LeftBrace Language_mappings RightBrace 
         | /* empty */;

Language_mappings: Language_mappings Language_mapping | Language_mapping ;

Language_mapping: Integer ":" Language_id ;

Language_id: Integer | EN;

cdTextTrack:  CD_TEXT LeftBrace opt_cdTextBlocks  RightBrace | ;

cdTextGlobal: CD_TEXT LeftBrace opt_cdTextLanguageMap opt_cdTextBlock  
                      RightBrace 
            | /* empty */;


%%

#ifdef STANDALONE
/* The controlling function */

int 
tocerror(char const *s)	/* called by tocparse on error */
{
	printf("%s\n",s);
	return(0);
}

int
main( int argc, char **argv )
{
  int c;

  tocdebug = 0;
  
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
      tocdebug = 1;
      break;
    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      exit(1);
    }
  }
  
  if ( optind < argc ) {
    toc_in = fopen( argv[optind], "r" );
    if (!toc_in) {
      printf("unable to open %s for reading: %s\n", argv[optind], 
	     strerror(errno));
      exit(1);
    }
  } else
    toc_in = stdin;


  if (tocparse()==0) {
    printf("Is a TOC file\n");
  } else {
    printf("Isn't a TOC file\n");
  }
  exit(0);
}
#endif /* STANDALONE*/
