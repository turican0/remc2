//############################################################################
//##                                                                        ##
//##  MilesStu.C                                                            ##
//##                                                                        ##
//##  MSS 4.0 Miles Sound Studio                                            ##
//##                                                                        ##
//##  V1.00 of 18-Mar-96: Initial version                                   ##
//##                                                                        ##
//##  C source compatible with Microsoft C v9.0 or later                    ##
//##                                                                        ##
//##   Author: Jeff Roberts                                                 ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  For technical support, contact RAD Game Tools at 425-893-4300.        ##
//##                                                                        ##
//############################################################################

#include <windows.h>
#include "mss.h"

#include <string.h>

#include <richedit.h>
#include <commctrl.h>
#include <stdio.h>

/* @cdep pre
  $if($ispresent($BuildDir,win64),
    $DefaultsWinEXE64EMT
    $requiresbinary(mss64.lib)
  ,
    $DefaultsWinEXE
    $requiresbinary(mss32.lib)
  )
  $requires(milesstu.rc)
  $requiresbinary(shell32.lib comdlg32.lib)
*/

/* @cdep post
  $if($ispresent($BuildDir,win64),
    $BuildWinEXE64EMT
  ,
    $BuildWinEXE
  )
*/


//
// globals
//

static S32 firstshow=1;
static S32 firstoutershow=1;
static S32 sizeable=0;
static HWND par;
static HWND dlg;
static S32 X=100;
static S32 Y=100;
static S32 W=0;
static S32 H=0;
static char output[4096];
static char dir[256];
static HINSTANCE hInst;
static HPROVIDER ASI;
static char* ASI_name;
static char ASI_types[256];
static char ASI_type[4];

//
// variables used by the show_slow and hide_slow routines
static char title[256]="";
static char* laststr;
static HCURSOR norm,waitcur;


//
// descriptions of the file types
//

static char* stype[]={
"n unknown file type",
"n uncompressed PCM wave file",
"n IMA ADPCM compressed wave file",
" wave file using unknown compression",
" Creative Labs VOC file",
" standard MIDI file",
" Miles XMIDI file with no embedded DLS data",
" Miles XMIDI file with embedded, uncompressed DLS data",
" Miles XMIDI file with embedded, compressed DLS data",
"n uncompressed DLS file",
" compressed DLS file",
"n MPEG Layer 1 audio compressed digital sound file",
"n MPEG Layer 2 audio compressed digital sound file",
"n MPEG Layer 3 audio compressed digital sound file",
"n unknown ASI wave file",
"n Xbox 1 ADPCM file",
"n Ogg Vorbis compressed digital sound file",
" Voxware variable bit 1200 bps voice chat file",
" Voxware fixed rate 2400 bps voice chat file",
" Voxware fixed rate 2900 bps voice chat file"
};


//
// ini file section to use
//

static char SECTION[]="Miles Sound Studio";


//
// default output file filters (used in get_outname)
//

static char* savefilt[]=
 {"XMIDI files (*.mss;*.xmi)\0*.mss;*.xmi\0All files (*.*)\0*.*\0",
  "RAW files (*.raw)\0*.raw\0All files (*.*)\0*.*\0",
  "WAV files (*.wav)\0*.wav\0All files (*.*)\0*.*\0",
  "Compressed DLS files (*.mls)\0*.mls\0All DLS files (*.dls;*.mls)\0*.dls;*.mls\0All files (*.*)\0*.*\0",
  "XMIDI files with embedded DLS data (*.mss)\0*.mss\0All XMIDI files (*.mss;*.xmi)\0*.mss;*.xmi\0All files (*.*)\0*.*\0",
  "Uncompressed DLS files (*.dls)\0*.dls\0All DLS files (*.dls;*.mls)\0*.dls;*.mls\0All files (*.*)\0*.*\0",
  "",
  ""};

//
// default output extensions (used in get_outname)
//

static char* savedef[]={"xmi","raw","wav","mls","mss","dls",""};

//
// help strings
//

static char* help[]={
  "The Compress button:\n\n"
  "This option will compress a digital wave file, a DLS\n"
  "file, or DLS data embedded in an XMIDI file with either\n"
  "IMA ADPCM or MPEG Layer-3 compression (if you have\n"
  "purchased the Miles MP3 compressor).  APDCM provides\n"
  "4 to 1 compression with very little quality loss. MPEG\n"
  "Layer 3 provides 11 to 1 compression with *no* audible\n"
  "quality loss at all.",

  "The Decompress button:\n\n"
  "This option will decompress a digital wave file, an MLS\n"
  "file, or MLS data embedded in an XMIDI file that was\n"
  "was previously compressed with the Compress option.\n",

  "The MIDI List button:\n\n"
  "This option will display the contents of the selected\n"
  "MIDI file.  This list file will display every MIDI\n"
  "event in the file.  It is useful when trouble-\n"
  "shooting problem MIDI files.  You can also choose\n"
  "to view the Roland MT-32 / LAPC-1 Programmer's\n"
  "Reference Guide with the MIDI listing.",

  "The DLS List button:\n\n"
  "This option will display the contents of the selected\n"
  "DLS file.  This list file will display all of the\n"
  "instrument information inside the DLS file. You can\n"
  "also choose whether to view the articulation data\n"
  "for each instrument.",

  "The Convert to XMIDI button:\n\n"
  "This option will convert one or more MIDI sequences into\n"
  "an XMIDI file.  XMIDI is the native format for MIDI data\n"
  "in the Miles Sound System, so converting them saves a step\n"
  "at playback time.  Even cooler, XMIDI files are about 30%\n"
  "smaller than MIDI files, so you get good compression as a bonus.",

  "The Convert to RAW button:\n\n"
  "This option will convert a highlighted digital wave file\n"
  "into a RAW file.  A raw file is a digital sound file that\n"
  "has no header or trailer information (sometimes useful in\n"
  "double-buffering sound playback).",

  "The Unmerge XMI and DLS button:\n\n"
  "This option will unmerge both the XMIDI and DLS pieces\n"
  "of a merged-XMIDI file into two separate files.  This is\n"
  "useful when you need to edit the DLS instruments that\n"
  "were previously embedded into a merged-XMIDI file.\n",

  "The Filter DLS with MIDI button:\n\n"
  "This option will use one or more MIDI or XMIDI files to\n"
  "filter instruments from a DLS or MLS file.  This option\n"
  "allows you to shrink huge DLS files (GM sets, for example)\n"
  "into smaller DLS files that include only the instruments\n"
  "that are used by the specified MIDI files.  You can\n"
  "highlight as many MIDI files as you want to filter with,\n"
  "so you can even create 'application-wide' DLS files.\n"
  "If the data is uncompressed, you can also choose to\n"
  "compress it as it is filtered.\n\n"
  "You must highlight both a DLS and at least one MIDI file\n"
  "for this option.  To highlight more than one file at a time,\n"
  "hold the 'Control' key down as you click on the files.",

  "The Merge XMI with DLS button:\n\n"
  "This option will merge together a DLS or MLS file into\n"
  "a MIDI or XMIDI file. This allows you to distribute your\n"
  "MIDI files with built-in instruments - kind of like a\n"
  "high-rent MOD file!  For incredible space-savings, you\n"
  "can also choose to remove the instruments that aren't\n"
  "used in the MIDI sequence!  You can even compress\n"
  "the instrument data as it is merged into the new file.\n\n"
  "You must highlight both a DLS and a MIDI file for this\n"
  "option.  To highlight more than one file at a time,\n"
  "hold the 'Control' key down as you click on the files.",

  "The File Type Button:\n\n"
  "This option will display the file type of the highlighted\n"
  "file.  It's handy when you can't remember if you already\n"
  "embedded DLS data into a file, or whether you\n"
  "compressed it, etc, etc."

};


//
// function to extract only the filename from a complete pathname
//

static char FAR* get_filename(char FAR* pathname)
{
   char FAR* f=pathname+lstrlen(pathname)-1;
   while ((f>=pathname) && (*f!='\\') && (*f!=':')) --f;
   return(f+1);
}


//
// write out an integer to the ini file
//

static void WriteProfileInt(char FAR* sec,char FAR* key,U32 value)
{
  char buf[16];
  wsprintf(buf,"%i",value);
  WriteProfileString(sec,key,buf);
}


//
// extract the first filename and advance the next file pointer
//

static char FAR* extract_filename(char FAR* dest,char FAR* cmdline)
{
  char FAR* s;
  char FAR* e;

  s=cmdline;

  //check for null
  if (s==0) {
    *dest=0;
    return(0);
  }

  //skip whitespace
  while ((*s<=32) && (*s!=0))
    s++;

  //is empty?
  if (*s==0) {
    *dest=0;
    return(0);
  }

  //is it a quoted name?
  if (*s=='"') {
    //scan until we hit the end or the last quote
    e=(++s)+1;
    while ((*e!='"') && (*e!=0))
      e++;

    //copy the string
    memcpy(dest,s,e-s);
    dest[e-s]=0;

    //skip over the end quote
    if (*e=='"')
      ++e;
    goto finish;
  } else {
    //scan until we hit the end
    e=s+1;
    while (*e!=0)
      e++;

    //copy the string
    memcpy(dest,s,e-s);
    dest[e-s]=0;

   finish:
    //skip over any remaining whitespace
    while ((*e<=32) && (*e!=0))
      e++;

    return(*e?e:0);
  }
}


//
// function to replace the current extension with a different one
//

static void replace_ext(char FAR* dest, char FAR* ext)
{
  char FAR*s;
  char FAR*e;

  s=get_filename(dest);
  e=s+lstrlen(s)-1;

  while ((e!=s) && (*e!='.'))
    --e;

  if (*e!='.') {
    e=s+lstrlen(s);
    *e='.';
  }
  strcpy(e+1,ext);
}


//
// simple error routines
//

#define show_MSS_error() show_error(AIL_last_error())
static void show_error(char* str)
{
  MessageBox(par,str,"Error",MB_OK|MB_ICONERROR);
}


//
// return the current filenames with quotes
//

static void get_current_files(char* addr,U32 length)
{
  CommDlg_OpenSave_GetSpec(par, addr, length);
}


//
// extract only one filename with error reporting
//

static S32 one_filename(char* dest,char*src)
{
  char* e;
  e=extract_filename(dest,src);
  if (e!=0) {
    show_error("Multiple files selected - highlight a single file.");
    return(0);
  } else if (*dest==0) {
    show_error("You haven't highlighted a file.");
    return(0);
  } else
    return(1);
}


//
// change the caption bar status and switch to an hourglass
//

static void show_slow(char* str)
{
  if (*title==0)
    GetWindowText(par,title,256);
  SetWindowText(par,str);
  laststr=str;
  norm=SetCursor(waitcur);
}


//
// hide the caption bar status and hourglass cursor
//

static void hide_slow()
{
  if (*title) {
    SetWindowText(par,title);
    *title=0;
  }
  SetCursor(norm);
}


//
// copy the given data to the clipboard, then launch WordPad, then
//   trigger a clipboard paste, then clear the clipboard - whew!
//

static void show_in_wordpad(void* list,U32 size)
{
  HWND wnd,main;
  HANDLE hand;
  void* ptr;
  HINSTANCE inst;

  show_slow("Loading WordPad to view the data...");

  //move the list onto the clipboard
  hand=GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,size);
  ptr=(char*)GlobalLock(hand);
  memcpy(ptr,list,size);
  GlobalUnlock(hand);
  OpenClipboard(par);
  EmptyClipboard();
  SetClipboardData(CF_TEXT,hand);
  CloseClipboard();

  //start the wordpad to look at it

  inst=ShellExecute(par,0,"wordpad",0,"",SW_SHOW);

  if (((UINTa)inst)>32) {
    U32 time=AIL_ms_count()+3000;
    do {
      // find the window
      main=FindWindow("WordPadClass",0);
      if (main) {
        // find the edit subwindow
        wnd=FindWindowEx(main,0,"RICHEDIT",0);
        if (wnd==0)
          wnd=FindWindowEx(main,0,"RichEdit20A",0);
        if (wnd==0)
          wnd=FindWindowEx(main,0,"RichEdit20W",0);
        if (wnd==0)
          wnd=FindWindowEx(main,0,"RichEdit50W",0);
        if (IsWindowVisible(wnd)) {
          //move to the first line
          SendMessage(wnd,EM_SETSEL,0,0);
          //paste in the data
          SendMessage(wnd,WM_PASTE,0,0);
          //see if the data got pasted
          if (SendMessage(wnd,EM_GETLINECOUNT,0,0)>1)
            //yup, so move back to the first line
            SendMessage(wnd,EM_SETSEL,0,0);
            OpenClipboard(par);
            EmptyClipboard();
            CloseClipboard();
            goto found;
         }
       }
       // wait a little while and then try again
       Sleep(50);
    } while(AIL_ms_count()<time);
    show_error("Could not find WordPad window.  Use Edit-Paste to get the report.");
   found:;
  } else
    show_error("Couldn't load WordPad. Use Edit-Paste to get the report.");

  hide_slow();
}


//
// read in a file with error reporting
//

static S32 file_read(char* name,void FAR* addr,S32 withsize)
{
  show_slow("Reading...");
  (*(void **)addr)=AIL_file_read(name,withsize?FILE_READ_WITH_SIZE:0);
  hide_slow();

  if (*((U32*)addr)==0) {
    show_error("File could not be loaded.");
    return(0);
  } else
    return(1);
}


//
// write out a file with error reporting
//

static S32 file_write(char*name,void const FAR*addr,U32 size)
{
  S32 result;

  show_slow("Writing...");
  result=AIL_file_write(name,addr,size);
  hide_slow();

  if (result==0)
    show_MSS_error();

  return(result);
}


//
// get a filename to use for output
//

static S32 get_outname(char* dest,U32 which,char* title)
{
   char oldname[256];

   OPENFILENAME fn;

   memset(&fn, 0, sizeof(fn));

   strcpy(oldname,dest);

   replace_ext(dest,savedef[which]);

   if (_stricmp(oldname,dest)==0) {
     strcpy(dest,"New_");
     strcat(dest,oldname);
   }

   //
   // setup the save as window
   //

   fn.lStructSize       = sizeof(fn);
   fn.hwndOwner         = par;
   fn.lpstrFilter       = savefilt[which];
   fn.nFilterIndex      = 1;
   fn.lpstrFile         = dest;
   fn.lpstrInitialDir   = dir;
   fn.nMaxFile          = 256;
   fn.lpstrTitle        = title;
   fn.Flags             = OFN_EXPLORER |
                          OFN_LONGNAMES |
                          OFN_NOCHANGEDIR |
                          OFN_OVERWRITEPROMPT |
                          OFN_PATHMUSTEXIST |
                          OFN_HIDEREADONLY;
   fn.lpstrDefExt       = savedef[which];

  return( GetSaveFileName(&fn)?1:0 );

}


//############################################################################
//##                                                                        ##
//## Play a highlighted file                                                ##
//##                                                                        ##
//############################################################################

static void play_file()
{
  char FAR* o;

  GetModuleFileName(0,output,255);
  strcpy(get_filename(output),"MilesPly.EXE  ");

  o=output+strlen(output);

  get_current_files(o,4096-255);

  // add quotes if the file isn't quoted already

  if (*o!='"') {
    o[-1]='"';
    strcat(o,"\"");
  }

  //
  // we post a message on error, because if we pop up a messagebox inside the
  // play button, then the common dialog forgets that we don't want to close it
  //

  if ((UINT)WinExec(output,SW_SHOW)<=32)
    PostMessage(dlg,WM_USER+0x88,0,(UINTa)"Error loading the Miles Sound Player.");
}

//############################################################################
//##                                                                        ##
//## Show the about window                                                  ##
//##                                                                        ##
//############################################################################

static void show_about()
{
   char text[1024];
   char version[8];
   AIL_MSS_version(version,8);
   lstrcpy(text,"Version ");
   lstrcat(text,version);
   lstrcat(text," " MSS_COPYRIGHT "\n\n"
                "The Miles Sound Studio from the Miles Sound System."
               "\n\nFor questions or comments, please contact RAD Game Tools at:\n\n"
               "\tRAD Game Tools\n"
               "\t401 Park Place - Suite 103\n"
               "\tKirkland, WA  98033\n"
               "\t425-893-4300\n"
               "\tFAX: 425-609-2463\n\n"
               "\tWeb: http://www.radgametools.com\n"
               "\tE-mail: sales3@radgametools.com");
   MessageBox(par,text,"About the Miles Sound Studio...", MB_OK);
}


//############################################################################
//##                                                                        ##
//## Convert to xmi                                                         ##
//##                                                                        ##
//############################################################################

static void to_xmidi()
{
  char each[256];
  U32* file;
  void* xmidi;
  U32 size;
  S32 result;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      if (AIL_file_type(file+1,file[0])!=AILFILETYPE_MIDI) {
        show_error("File is not a standard MIDI file.");
        goto cont;
      }

      show_slow("Converting...");
      result=AIL_MIDI_to_XMI(file+1,file[0],&xmidi,&size,0);
      hide_slow();

      if (result==0) {

        char error[512];
        wsprintf(error,"There was an error converting the MIDI sequence:\n\n%s\n\nDo you want to try converting again in 'error-tolerant' mode?",AIL_last_error());

        if (MessageBox(par,error,"Converting...",MB_YESNO|MB_ICONQUESTION)==IDYES) {
          show_slow("Converting...");
          result=AIL_MIDI_to_XMI(file+1,file[0],&xmidi,&size,AILMIDITOXMI_TOLERANT);
          hide_slow();
        } else
          goto cont;
      }

      if (result==0) {
        show_MSS_error();
      } else {

        if (get_outname(each,0,"Enter the XMIDI filename to convert the MIDI file into:")) {

          file_write(each,xmidi,size);

        }

        AIL_mem_free_lock(xmidi);
      }

     cont:

      AIL_mem_free_lock(file);
    }
  }

}


//############################################################################
//##                                                                        ##
//## Convert to raw                                                         ##
//##                                                                        ##
//############################################################################

static void to_raw()
{
  char each[256];
  U32* file;
  AILSOUNDINFO si;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {
      S32 t;

      t = AIL_file_type(file+1,file[0]);
      
      if ( (t!=AILFILETYPE_PCM_WAV) && (t!=AILFILETYPE_ADPCM_WAV) && (t!=AILFILETYPE_OTHER_WAV) && (t!=AILFILETYPE_OTHER_ASI_WAV) ) {
        show_error("File is not a wave file - you can try decompressing it first.");
      }
      else if (AIL_WAV_info(file+1,&si)==0) {
        show_MSS_error();
      } else {

        if (get_outname(each,1,"Enter the RAW filename to convert the wave file into:")) {

          file_write(each,si.data_ptr,si.data_len);

        }

      }

      AIL_mem_free_lock(file);
    }
  }

}


//############################################################################
//##                                                                        ##
//## Show the DLS list                                                      ##
//##                                                                        ##
//############################################################################

static void show_dls()
{
  char each[256];
  char wav_dir[256];
  char offer[256];
  char* list;
  U32* file;
  U32 size;
  U32 flags=0;
  S32 result;
  void* dls;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      result=AIL_file_type(file+1,file[0]);

      if ((result!=AILFILETYPE_DLS) && (result!=AILFILETYPE_MLS) && (result!=AILFILETYPE_XMIDI_DLS) && (result!=AILFILETYPE_XMIDI_MLS)) {
       err:
        show_error("This isn't a DLS or MLS file (or an XMIDI file with embedded DLS data).");
        goto done;
      }

      result=AIL_find_DLS(file+1,file[0],0,0,&dls,0);
      if (result==0)
        goto err;

      switch (MessageBox(par,"Do you also want to see the articulation information?","DLS List",MB_YESNOCANCEL|MB_ICONQUESTION)) {
        case IDYES:
          flags=AILDLSLIST_ARTICULATION;
          break;
        case IDNO:
          flags=0;
          break;
        case IDCANCEL:
          goto done;
      }

      //
      // Derive directory name from DLS filename
      //

      strcpy(wav_dir,each);
      replace_ext(wav_dir,"");
      strcpy(wav_dir+strlen(wav_dir)-1," Samples");

      wsprintf(offer,"Would you like to extract the digital samples into the directory '%s'?",
         wav_dir);

      switch (MessageBox(par,offer,"DLS List",MB_YESNOCANCEL|MB_ICONQUESTION)) {
        case IDYES:
          flags |= AILDLSLIST_DUMP_WAVS;
          break;
        case IDNO:
          break;
        case IDCANCEL:
          goto done;
      }

      show_slow("Building list...");
      result=AIL_list_DLS(dls,&list,&size,flags,each);
      hide_slow();

      if (result==0)
        show_MSS_error();
      else
        show_in_wordpad(list,size);

     done:
      AIL_mem_free_lock(file);
    }
  }

}

//############################################################################
//##                                                                        ##
//## Show the MIDI list                                                     ##
//##                                                                        ##
//############################################################################

static void show_midi()
{
  char each[256];
  char* list;
  U32* file;
  U32 size;
  U32 flags = 0;
  S32 result;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      if (AIL_file_type(file+1,file[0])!=AILFILETYPE_MIDI) {
        show_error("This isn't a standard MIDI file.");
        goto done;
      }

      switch (MessageBox(par,"Do you want to include the Roland MIDI reference?","DLS List",MB_YESNOCANCEL|MB_ICONQUESTION)) {
        case IDYES:
          switch (MessageBox(par,"Do you want the Roland MIDI reference in the unabridged format (really big)?","DLS List",MB_YESNOCANCEL|MB_ICONQUESTION)) {
            case IDYES:
              flags=AILMIDILIST_ROLANDUN;
              break;
            case IDNO:
              flags=AILMIDILIST_ROLANDAB;
              break;
            case IDCANCEL:
              goto done;
          }
          break;
        case IDNO:
          flags=0;
          break;
        case IDCANCEL:
          goto done;
      }

      show_slow("Building list...");
      result=AIL_list_MIDI(file+1,file[0],&list,&size,flags);
      hide_slow();

      if (result==0)
        show_MSS_error();
      else
        show_in_wordpad(list,size);

     done:
      AIL_mem_free_lock(file);
    }

  }

}

//############################################################################
//#                                                                          #
//# Macros to acquire bitfield data of length n from header/side or audio    #
//# buffers, n <= 24                                                         #
//#                                                                          #
//# Bit position 0 is MSB of byte 0                                          #
//#                                                                          #
//# Request for 0 bits is considered valid, returning 0                      #
//#                                                                          #
//############################################################################

#define H(n) (n ? read_bits(header_buffer, &hpos, (n)) : 0)

#ifdef IS_LE

U32 read_bits(U8 FAR *data, S32 *bitpos, S32 n)
{
   U32 val;

#if defined(IS_WIN32) && !defined(IS_WIN64)

   _asm
      {
      mov edx,bitpos
      mov ebx,data

      mov ecx,[edx]
      mov eax,ecx

      and ecx,7
      shr eax,3

      mov eax,[ebx][eax]
      bswap eax

      shl eax,cl

      mov ecx,n
      add [edx],ecx

      mov ecx,32
      sub ecx,n
      shr eax,cl
      mov val,eax
      }

#else

   S32     b = *bitpos;
   U8 FAR *p = &data[b >> 3];

   val = ((U32)(p[3]))        +
        (((U32)(p[2])) << 8)  +
        (((U32)(p[1])) << 16) +
        (((U32)(p[0])) << 24);

   val <<= b & 7;
   val >>= 32 - n;

   *bitpos = b + n;

#endif

   return val;
}

U32 view_bits8(U8 FAR *data, S32 bitpos)
{
   U32 val;

#if defined(IS_WIN32) && !defined(IS_WIN64)

   _asm
      {
      mov ecx,[bitpos]
      mov ebx,[data]
      mov eax,ecx

      and ecx,7
      shr eax,3

      mov ax,[ebx][eax]

      rol ax,cl

      movzx eax,al
      mov val,eax
      }

#else

   U8 FAR *p = &data[bitpos >> 3];

   val = ((U32)(p[1])) + (((U32)(p[0])) << 8);

   val <<= (bitpos & 7) + 16;
   val >>= 24;

#endif

   return val;
}

#else

U32 read_bits(U8 FAR *data, S32 *bitpos, S32 n)
{
   U32 val;
   S32     b = *bitpos;

   val = *(U32*)&data[b>>3];

   val <<= b & 7;
   val >>= 32 - n;

   *bitpos = b + n;

   return val;
}

U32 view_bits8(U8 FAR *data, S32 bitpos)
{
   U32 val;

   U8 FAR *p = &data[bitpos >> 3];

   val = *(U32*)&data[bitpos>>3];

   val <<= bitpos & 7;
   val >>= 32 - 8;

   return val;
}

#endif

//
// Bit rate in bits/second for [MPEG version][value]
//

const S32 MPEG_bit_rate[2][15] =
{
   { 0L,8000L,16000L,24000L,32000L,40000L,48000L,56000L,64000L,80000L,96000L,112000L,128000L,144000L,160000L         }
   ,
   { 0L,32000L,40000L,48000L,56000L,64000L,80000L,96000L,112000L,128000L,160000L,192000L,224000L,256000L,320000L     }
};

//
// Sample rate in samples/second for [MPEG25][MPEG version][value]
//

const S32 MPEG_sample_rate[2][2][4] =
{{
   { 22050L,24000L,16000L,22050L },
   { 44100L,48000L,32000L,44100L }
},
{
   { 11025L,12000L, 8000L,11025L },
   { 44100L,48000L,32000L,44100L }
}};

//############################################################################
//#                                                                          #
//# Show MP3 frame data in Wordpad                                           #
//#                                                                          #
//############################################################################

static void show_MP3_data(C8 *filename, //)
                          U8 *file_image, 
                          S32 file_size)
{
   S32   i;
   FILE *tempfile;

   //
   // Copy of certain fields from first header encountered in stream,
   // used for validation of subsequent headers
   //

   S32 check_valid = 0;
   S32 check_MPEG1 = 0;
   S32 check_MPEG25 = 0;
   S32 check_layer = 0;
   S32 check_protection_bit = 0;
   S32 check_sampling_frequency = 0;
   S32 check_mode = 0;
   S32 check_copyright = 0;
   S32 check_original = 0;

   S32 frame_cnt = 0;

   U8 *MP3_file_begin = NULL;

   //
   // Generate name for temporary file
   //

   C8 tempfile_name[MAX_PATH] = "c:\\msstemp.txt";
   C8 path_buffer  [MAX_PATH] = "";

   GetTempPath(sizeof(path_buffer)-1,
               path_buffer);

   GetTempFileName(path_buffer,
                  "MSS",
                   0,
                   tempfile_name);

   strcat(tempfile_name,".htm");

   //
   // Open tempfile and write header
   //

   tempfile = fopen(tempfile_name,"w+t");

   if (tempfile == NULL)
      {
      C8 buff[MAX_PATH*2];
      sprintf(buff,"Could not open temporary file %s",tempfile_name);
      show_error(buff);
      return;
      }

   fprintf(tempfile,"File \"%s\": %d bytes\n\
______________________________________________________________________________________________________________________________________\
\n\n",
      filename,
      file_size);

   //
   // Skip initial ID3v2 tag for compliance with rest of library
   //

   if ( ( ((U8 const FAR* )file_image)[ 0 ] == 0x49 ) && ( ((U8 const FAR* )file_image)[ 1 ] == 0x44 ) && ( ((U8 const FAR* )file_image)[ 2 ] == 0x33 ) &&
        ( ((U8 const FAR* )file_image)[ 3 ] < 0xff )  && ( ((U8 const FAR* )file_image)[ 4 ] < 0xff )  &&
        ( ((U8 const FAR* )file_image)[ 6 ] < 0x80 )  && ( ((U8 const FAR* )file_image)[ 7 ] < 0x80 )  && ( ((U8 const FAR* )file_image)[ 8 ] < 0x80 ) && ( ((U8 const FAR* )file_image)[ 9 ] < 0x80 ) )
   {
     U32 skip = 10 + ( (U32) ((U8 const FAR* )file_image)[ 9 ] ) | ( ( (U32) ((U8 const FAR* )file_image)[ 8 ] ) << 7 ) |
                   ( ( (U32) ((U8 const FAR* )file_image)[ 7 ] ) << 14 ) | ( ( (U32) ((U8 const FAR* )file_image)[ 6 ] ) << 21 );

     file_image = ( (U8 FAR*) file_image ) + skip;
     file_size -= skip;

     fprintf(tempfile,"Skipping ID3v2 tag at start of file, total tag length = %d bytes\n",
        skip);
     fprintf(tempfile,"This amount must be added to all subsequent frame offsets to make them relative to the beginning of the physical file\n\n");

   }

   MP3_file_begin = file_image;

   //
   // Account for ID3V1 tag at end of file, if any
   //

   if (file_size >= 128)
      {
      C8 *ptr = (C8*)&MP3_file_begin[file_size-128];

      if ((ptr[0]=='T') && (ptr[1]=='A') && (ptr[2]=='G'))
         {
         file_size -= 128;

         fprintf(tempfile,"ID3v1 tag is present at end of file (offset %d, total tag length = 128 bytes)\n\n",
            file_size-128);
         }
      }

   //
   // Dump frames, using same discrimination logic as ASI implementation
   //

   for(;;)
      {
      UINTa seek_pos;
      U32 seek_data;  
      U32 dest_data;
      U32 valid_frame;
      U32 bytes_left;

read_frame_header:

      //
      // ID3v2 tags can appear at any frame boundary, not just the first
      //

      while (file_size >= 10)   
         {
         if (!( ( file_image[ 0 ] == 0x49 ) && ( file_image[ 1 ] == 0x44 ) && ( file_image[ 2 ] == 0x33 ) &&
                ( file_image[ 3 ] < 0xff )  && ( file_image[ 4 ] < 0xff )  &&
                ( file_image[ 6 ] < 0x80 )  && ( file_image[ 7 ] < 0x80 )  && ( file_image[ 8 ] < 0x80 ) && ( file_image[ 9 ] < 0x80 ) ) )
            {
            //
            // No ID3v2 tag present
            //
          
            break;
            }
         else
            {
            S32 size = ( (U32) file_image[ 9 ] ) | ( ( (U32) file_image[ 8 ] ) << 7 ) | ( ( (U32) file_image[ 7 ] ) << 14 ) | ( ( (U32) file_image[ 6 ] ) << 21 );
            S32 total_len;
   
            if (file_image[5] & 0x10)
               {
               total_len = 20 + size;

               fprintf(tempfile,"ID3v2 tag with footer found at offset %d, total tag length = %d bytes\n",
                  file_image - MP3_file_begin,
                  total_len);
               }
            else
               {
               total_len = 10 + size;

               fprintf(tempfile,"ID3v2 tag found at offset %d, total tag length = %d bytes\n",
                  file_image - MP3_file_begin,
                  total_len);
               }

            //
            // Skip ID3v2 tag
            //

            if (file_size < total_len)
               {
               fprintf(tempfile,"EOF: ID3v2 tag truncated (%d bytes expected, %d available)\n",
                  total_len,
                  file_size);
               break;
               }

            file_size  -= total_len;
            file_image += total_len;
            }
         }

      //
      // Evaluate frame header for validity
      //

      seek_pos    = file_image - MP3_file_begin;
      seek_data   = 0;          
      dest_data   = 0;          
      valid_frame = 0;          
      bytes_left  = file_size; 

      for(;;)
         {
         if (!file_size)
            {
            break;
            }

         seek_data >>= 8;

         *(((U8 FAR *) &dest_data) + 3) = *file_image++;
         --file_size;

         seek_data |= dest_data;

         if ((seek_data & 0x0000e0ff ) == 0x0000e0ff)
            {
            //
            // Potentially-valid 32-bit frame header
            //

            valid_frame = 1;
            break;
            }
         }

      if (!valid_frame)
         {
         if (!bytes_left)
            {
            fprintf(tempfile,"EOF: End of MPEG audio data coincides with end of file\n");
            }
         else
            {
            fprintf(tempfile,"EOF: No (more) 11-bit frame sync markers found in last %d byte(s) of file\n",
               bytes_left);
            }

         break;
         }
      else
         {
         U8  header_buffer[64];           // Raw header and side info for current frame
         S32 hpos;                        // Current bit position in header/side buffer

         S32 header_size;                 // 4 or 6 bytes, depending on CRC
         S32 side_info_size;              // Valid for layer 3 side info only
         S32 ngr;
         S32 nch;
         S32 main_data_begin;

         S32 MPEG1;                       // 1=MPEG1, 0=MPEG2 or MPEG2.5
         S32 MPEG25;                      // 1=MPEG2.5, 0=MPEG1 or MPEG2
         S32 layer;
         S32 protection_bit;
         S32 bitrate_index;
         S32 sampling_frequency;
         S32 padding_bit;
         S32 private_bit;
         S32 mode;
         S32 mode_extension;
         S32 copyright;
         S32 original;
         S32 emphasis;

         S32 bit_rate;                    // Characteristics of current frame
         S32 sample_rate;
         S32 average_frame_size;
         S32 data_size;

         //
         // Log offset of frame in bytes from beginning of file
         //

         SINTa frame_offset = (file_image - MP3_file_begin) - 4;

         //
         // Note any cases where data between valid headers was skipped
         //

         if ((frame_offset != (SINTa)seek_pos) && (seek_pos != 0))
            {
            fprintf(tempfile,"Note: Found %d unexpected byte(s) beginning at offset %d\n",
               frame_offset - seek_pos,
               seek_pos);
            }

         //
         // Parse first 32 bits of header, first skipping 11-bit sync field
         //

         *((U32 FAR *) header_buffer) = seek_data;

         hpos = 11;

         MPEG25             = !H(1);
         MPEG1              =  H(1);
         layer              =  H(2);
         protection_bit     =  H(1);
         bitrate_index      =  H(4);
         sampling_frequency =  H(2);
         padding_bit        =  H(1);
         private_bit        =  H(1);
         mode               =  H(2);
         mode_extension     =  H(2);
         copyright          =  H(1);
         original           =  H(1);
         emphasis           =  H(2);

         //
         // Perform sanity check on header, since most encoders seem to be written
         // with complete disregard to the rule against >= 12 consecutive 1 bits
         // in stream....
         //
         // We assume the first header found in the stream is valid, and use its
         // contents to check the fields of all subsequent headers.  The fields
         // tested are those which should not ordinarily change in the course of
         // a single stream.
         //

         if ((bitrate_index      == 0x0f) ||
             (sampling_frequency == 0x03) )
            {
            //
            // Header contains one or more invalid bitfields, so skip it
            //
            // (Note that this will skip a valid frame if it begins within
            // the first 4 bytes of a false header)
            //

            fprintf(tempfile, "Header at offset %d contains one or more invalid bitfields (bitrate_index=0x%X, sampling_frequency=0x%X)\n",
               frame_offset,
               bitrate_index,
               sampling_frequency);

            goto read_frame_header;
            }

         // keep searching if we find a non layer 3 block
         if (layer != 1)
            {
            fprintf(tempfile, "Header at offset %d does not specify MPEG layer 3 (%d)\n",
               frame_offset,
               layer);

            goto read_frame_header;
            }

         if (!check_valid)
            {
            check_valid = 1;

            check_MPEG25             = MPEG25;
            check_MPEG1              = MPEG1;
            check_layer              = layer;
            check_protection_bit     = protection_bit;
            check_sampling_frequency = sampling_frequency;
            check_mode               = mode;
            check_copyright          = copyright;
            check_original           = original;
            }
         else
            {
            if ((MPEG1              != check_MPEG1)              ||
                (MPEG25             != check_MPEG25)             ||
                (layer              != check_layer)              ||
//              (protection_bit     != check_protection_bit)     ||
//              (mode               != check_mode)               ||
//              (copyright          != check_copyright)          ||
//              (original           != check_original)           ||
                (sampling_frequency != check_sampling_frequency) )
               {
               //
               // Header does not match characteristics of most-recent one encountered
               // in stream -- keep looking
               //
               // (Note that this will skip a valid frame if it begins within
               // the first 4 bytes of a false header)
               //

               fprintf(tempfile, "Header at offset %d is inconsistent with current frame type (MPEG1:%d vs %d, MPEG25:%d vs %d, layer:%d vs %d, sampling frequency:%d vs %d\n",
                  frame_offset,
                  MPEG1, check_MPEG1,
                  MPEG25, check_MPEG25,
                  layer, check_layer,
                  sampling_frequency, check_sampling_frequency);

               check_MPEG25             = MPEG25;
               check_MPEG1              = MPEG1;
               check_layer              = layer;
               check_protection_bit     = protection_bit;
               check_sampling_frequency = sampling_frequency;
               check_mode               = mode;
               check_copyright          = copyright;
               check_original           = original;

               goto read_frame_header;
               }
            }

         //
         // Skip CRC word if present
         //

         header_size = 4;

         if (protection_bit == 0)
            {
            if (file_size < 2)
               {
               fprintf(tempfile,"EOF: Header at offset %d truncated at CRC word\n",
                  frame_offset);
               break;
               }

            header_buffer[4] = *file_image++;
            header_buffer[5] = *file_image++;
            file_size -= 2;

            hpos += 16;
            header_size = 6;
            }

         //
         // Read MPEG side info into header buffer
         //
         
         if (MPEG1)
            {
            //
            // MPEG 1: Mono=17, stereo=32 bytes
            //

            side_info_size = (mode == 3) ? 17 : 32;
            }
         else
            {
            //
            // MPEG2: Mono=9, stereo=17 bytes
            //

            side_info_size = (mode == 3) ? 9 : 17;
            }

         if (file_size < side_info_size)
            {
            fprintf(tempfile,"EOF: Truncated side info block (%d byte(s) left, %d byte(s) needed)\n",
               file_size,
               side_info_size);               
            break;
            }

         file_size -= side_info_size;

         for (i=0; i < side_info_size; i++)
            {
            header_buffer[header_size+i] = *file_image++;
            }

         //
         // Get main data offset word and skip private bits
         // This leaves hpos at the MPEG 1 scalefactor selection info block, which
         // we don't care about reading...
         //

         nch = (mode == 3) ? 1 : 2;
         ngr = (MPEG1) ? 2 : 1;

         if (MPEG1)
            {
            main_data_begin = H(9);
            (nch == 1) ? H(5) : H(3);
            }
         else
            {
            main_data_begin = H(8);
            (nch == 1) ? H(1) : H(2);
            }

         //
         // Calculate average and actual frame size
         //
         // In Layer 3, MPEG1 has two granules per frame
         //

         bit_rate = MPEG_bit_rate[MPEG1][bitrate_index];

         sample_rate = MPEG_sample_rate[MPEG25][MPEG1][sampling_frequency];

         if (MPEG1) 
            {
            average_frame_size = (144 * bit_rate) / sample_rate;
            }
         else
            {
            average_frame_size = (72 * bit_rate) / sample_rate;
            }

         data_size = average_frame_size + padding_bit - (header_size + side_info_size);

         if (data_size < 0)
            {
            data_size = 0;
            }

         //
         // Skip over actual audio data
         //

         if (file_size < data_size)
            {
            fprintf(tempfile,"EOF: Truncated audio data block (%d byte(s) left, %d byte(s) needed)\n",
               file_size,
               data_size);               
            
            break;
            }

         file_size  -= data_size;
         file_image += data_size;

         //
         // Write frame descriptor to file
         //

         fprintf(tempfile,"Frame #%d \t Offset:%d \t Header/side bytes:%d \t Data bytes:%d \t Padding=%d byte(s) \t Channel(s):%d \t BPS: %d \t Sample rate: %d\n",
            ++frame_cnt,
            frame_offset,
            header_size + side_info_size,
            data_size - padding_bit,
            padding_bit,
            nch,
            bit_rate,
            sample_rate);
         }
      }

   fclose(tempfile);

   //
   // Display file in Wordpad or IE
   // Delete the file after IE has had a chance to load it
   //

#if 0
   ShellExecute(par,0,tempfile_name,0,"",SW_SHOW);
   Sleep(2000);
#else
   {
   U32 *file;

   if (file_read(tempfile_name, &file, 1))
      {
      show_in_wordpad(&file[1], file[0]);
      AIL_mem_free_lock(file);
      }
   }
#endif

   DeleteFile(tempfile_name);
}

//############################################################################
//##                                                                        ##
//## Show the file type                                                     ##
//##                                                                        ##
//############################################################################

static void show_type()
{
   char mess[512];
   char each[256];
   U32* file;

   get_current_files(output,4096);

   if (one_filename(each,output)) 
      {
      if (file_read(each,&file,1)) 
         {
         S32 type = AIL_file_type(file+1,file[0]);

         wsprintf(mess,"'%s' is a%s, and is %lu bytes in size.",each,stype[type],file[0]);

         if (type != AILFILETYPE_MPEG_L3_AUDIO)
            {
            AIL_mem_free_lock(file);
            MessageBox(par,mess,"File Type...",MB_OK|MB_ICONINFORMATION);
            }
         else
            {
            strcat(mess,"\n\nWould you like to disassemble the MP3 file and display its frame offsets?");

            switch (MessageBox(par,mess,"File Type...",MB_YESNO|MB_ICONQUESTION)) 
               {
               case IDYES:
                  {
                  show_MP3_data(each, 
                       (U8 *) (&file[1]), 
                                file[0]);
                  break;
                  }
               }

            AIL_mem_free_lock(file);
            }
         }
      }
}

//############################################################################
//##                                                                        ##
//## find out if the user wants to use the ASI encoder                      ##
//##                                                                        ##
//############################################################################

static char* ask_to_use_asi()
{
  char buf[512];
  if (ASI) {

    wsprintf(buf,"The %s is installed, would you like to use it to compress the file?",ASI_name);
    switch (MessageBox(par,buf,"Use compression...",MB_YESNOCANCEL|MB_ICONQUESTION)) {
      case IDYES:
        return(ASI_type);
      case IDNO:
        return(0);
      case IDCANCEL:
        return((char*)-1);
    }
  }
  return(0);
}


//############################################################################
//##                                                                        ##
//## Compression callback                                                   ##
//##                                                                        ##
//############################################################################

static U32 lastper;

S32 AILCALLBACK comp_cb(U32 state,UINTa user)
{
  char buf[512];
  U32 newper;
  RECT r;

  switch (state) {
    case AIL_LENGTHY_INIT:
      //reset pressed state
      GetAsyncKeyState(VK_ESCAPE);
      lastper=101;
      break;
    case AIL_LENGTHY_UPDATE:
      //update the title bar
      newper=(U32)*((F32*)&user);
      if (lastper!=newper) {
        lastper=newper;
        wsprintf(buf,"%s (%i%%)",laststr,lastper);
        SetWindowText(par,buf);
      }
      //see if the user pressed escape
      if (GetAsyncKeyState(VK_ESCAPE)&1)
        if (MessageBox(par,"Cancel the conversion?","Cancel...",MB_YESNO|MB_ICONQUESTION)==IDYES)
          return(0);
      break;
  }

  // repaints the window, if necessary
  if (GetUpdateRect(par,&r,0))
    UpdateWindow(par);

  // moves window to top, if necessary
  if (GetForegroundWindow()==par)
    BringWindowToTop(par);

  return(1);
}


//############################################################################
//##                                                                        ##
//## Compress file                                                          ##
//##                                                                        ##
//############################################################################

static void compress()
{
  char mess[512];
  char each[256];
  U32* file;
  S32 type;
  S32 which=0;
  char* t=0;
  void* data=0;
  U32 size=0;
  S32 result;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      type=AIL_file_type(file+1,file[0]);

      if ((type==AILFILETYPE_ADPCM_WAV) || (type==AILFILETYPE_XMIDI_MLS) || (type==AILFILETYPE_MLS)) {
        wsprintf(mess,"You can't compress '%s' because it is already compressed.\n\nIt is a%s - try decompressing it first.",each,stype[type]);
        show_error(mess);
        goto done;
      }

      if ((type==AILFILETYPE_PCM_WAV) || (type==AILFILETYPE_ADPCM_WAV)) {
        AILSOUNDINFO si;

        char* ourext=ask_to_use_asi();
        if (((SINTa)ourext)==-1)
          goto done;

        result=AIL_WAV_info(file+1,&si);
        if (result) {
          t="Enter the filename to compress into:";
          show_slow("Compressing wave file...");

          if (ourext==0) {
            switch (MessageBox(par,"Would you like to use the IMA ADPCM compressor to compress the file?","Use compression...",MB_YESNOCANCEL|MB_ICONQUESTION)) {
              case IDYES:
                which=2;
                result=AIL_compress_ADPCM(&si,&data,&size);
                break;
              case IDNO:
              case IDCANCEL:
                goto done;
            }
          } else {
            which=6;
            result=AIL_compress_ASI(&si,ASI_type,&data,&size,comp_cb);
          }
        }
        goto comp;
      }

      if (type==AILFILETYPE_DLS) {

        char* ourext=ask_to_use_asi();
        if (((SINTa)ourext)==-1)
          goto done;

        which=3;
        t="Enter the DLS filename to compress into:";
        show_slow("Compressing DLS file...");

        result=AIL_compress_DLS(file+1,ourext,&data,&size,comp_cb);

        goto comp;
      }

      if (type==AILFILETYPE_XMIDI_DLS) {
        void* dls;

        which=4;
        t="Enter the XMIDI filename to compress into:";
        show_slow("Extracting DLS data from the XMIDI file...");

        result=AIL_extract_DLS(file+1,file[0],0,0,&dls,0,comp_cb);

        if (result) {
          void* dlsc;

          char* ourext=ask_to_use_asi();
          if (((SINTa)ourext)==-1)
            goto done;

          show_slow("Compressing DLS data in the XMIDI file...");
          result=AIL_compress_DLS(dls,ourext,&dlsc,0,comp_cb);
          AIL_mem_free_lock(dls);

          if (result) {

            show_slow("Merging DLS data back into the XMIDI file...");
            result=AIL_merge_DLS_with_XMI(file+1,dlsc,&data,&size);
            AIL_mem_free_lock(dlsc);

          }
        }

       comp:

        hide_slow();

        if (result==0) {
          show_MSS_error();
        } else {
          if (get_outname(each,which,t))
            file_write(each,data,size);

          AIL_mem_free_lock(data);
        }
        goto done;
      }

      wsprintf(mess,"You can't compress '%s' because it is a%s.\n\nYou can try decompressing it first (this will work if we have a decoder for it).",each,stype[type]);
      show_error(mess);

     done:

      AIL_mem_free_lock(file);

    }

  }

}


//############################################################################
//##                                                                        ##
//## Decompress file                                                        ##
//##                                                                        ##
//############################################################################

static void decompress()
{
  char mess[512];
  char each[256];
  U32* file;
  S32 type;
  S32 which;
  char* t;
  void* data=0;
  U32 size=0;
  S32 result;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      type=AIL_file_type(file+1,file[0]);

      if ((type==AILFILETYPE_PCM_WAV) || (type==AILFILETYPE_XMIDI_DLS) || (type==AILFILETYPE_DLS)) {
        wsprintf(mess,"You can't decompress '%s' because it isn't compressed.  (It is a%s).",each,stype[type]);
        show_error(mess);
        goto done;
      }

      if (type==AILFILETYPE_ADPCM_WAV) {
        AILSOUNDINFO si;

        which=2;
        t="Enter the WAV filename to uncompress into:";
        show_slow("Uncompressing ADPCM wave file...");

        result=AIL_WAV_info(file+1,&si);

        if (result)
          result=AIL_decompress_ADPCM(&si,&data,&size);

        goto comp;
      }

      if ( (type==AILFILETYPE_MPEG_L3_AUDIO) || 
           (type==AILFILETYPE_OGG_VORBIS   ) || 
           (type==AILFILETYPE_OGG_SPEEX    ) || 
           (type==AILFILETYPE_S8_VOICE     ) || 
           (type==AILFILETYPE_S16_VOICE    ) || 
           (type==AILFILETYPE_S32_VOICE    ) || 
           (type==AILFILETYPE_V12_VOICE    ) || 
           (type==AILFILETYPE_V24_VOICE    ) ||  
           (type==AILFILETYPE_V29_VOICE    ) ) {
        which=2;
        t="Enter the WAV filename to uncompress into:";
        show_slow("Uncompressing ASI file...");

        result=AIL_decompress_ASI(file+1,file[0],each,&data,&size,comp_cb);

        goto comp;
      }

      if (type==AILFILETYPE_MLS) {
        which=5;
        t="Enter the DLS filename to decompress into:";
        show_slow("Decompressing DLS file...");

        result=AIL_extract_DLS(file+1,file[0],0,0,&data,&size,comp_cb);

        goto comp;
      }

      if (type==AILFILETYPE_XMIDI_MLS) {
        void* dls;

        which=4;
        t="Enter the XMIDI filename to decompress into:";
        show_slow("Extracting DLS data from the XMIDI file...");

        result=AIL_extract_DLS(file+1,file[0],0,0,&dls,0,comp_cb);

        if (result) {

          show_slow("Merging DLS data back into the XMIDI file...");
          result=AIL_merge_DLS_with_XMI(file+1,dls,&data,&size);
          AIL_mem_free_lock(dls);

        }

       comp:

        hide_slow();

        if (result==0) {
          show_MSS_error();
        } else {
          if (get_outname(each,which,t))
            file_write(each,data,size);

          AIL_mem_free_lock(data);
        }
        goto done;
      }

      wsprintf(mess,"You can't compress '%s' because it is a%s.",each,stype[type]);
      show_error(mess);

     done:

      AIL_mem_free_lock(file);

    }

  }

}


//############################################################################
//##                                                                        ##
//## Unmerge XMI and DLS                                                    ##
//##                                                                        ##
//############################################################################

static void unmerge()
{
  char each[256];
  char copy[256];
  U32* file;
  U32 type;
  void* xmi;
  U32 xmisize;
  void* dls;
  U32 dlssize;
  S32 result;

  get_current_files(output,4096);

  if (one_filename(each,output)) {

    if (file_read(each,&file,1)) {

      type=AIL_file_type(file+1,file[0]);

      if ((type!=AILFILETYPE_XMIDI_DLS) && (type!=AILFILETYPE_XMIDI_MLS))
        show_error("This isn't a XMIDI file with embedded DLS data, so you can't unmerge it.");
      else {

        result=AIL_find_DLS(file+1,file[0],&xmi,&xmisize,&dls,&dlssize);

        if (result==0)
          show_MSS_error();
        else {

          strcpy(copy,each);
          if (get_outname(copy,0,"Enter the XMIDI filename to unmerge the MIDI data to:"))
            file_write(copy,xmi,xmisize);

          if (get_outname(each,(type==AILFILETYPE_XMIDI_DLS)?5:3,"Enter the DLS filename to unmerge the DLS data to:"))
            file_write(each,dls,dlssize);
        }
      }

      AIL_mem_free_lock(file);
    }
  }

}


//############################################################################
//##                                                                        ##
//## Merge XMI and DLS                                                      ##
//##                                                                        ##
//############################################################################

static void merge()
{
  char file1[256];
  char file2[256];
  U32* data1;
  U32* data2;
  U32 size1;
  U32 size2;
  S32 type1;
  S32 type2;
  S32 result;
  char* e;
  void* final;
  U32 finalsize;

  get_current_files(output,4096);

  e=extract_filename(file1,output);
  if (e)
    e=extract_filename(file2,e);
  else
    *file2=0;

  if ((*file1==0) || (*file2==0)) {
    show_error("To merge, you must highlight a MIDI/XMIDI file, AND a DLS/MLS file.\n\n"
               "To highlight more than one file at a time, hold down the 'Control' key as you click.");
    return;
  }

  if (e) {
    show_error("You can only merge two files at a time (a MIDI/XMIDI file and a DLS/MLS file).");
    return;
  }

  if (file_read(file1,&data1,0)) {

    size1=AIL_file_size(file1);
    type1=AIL_file_type(data1,size1);

    if (file_read(file2,&data2,0)) {

      size2=AIL_file_size(file2);
      type2=AIL_file_type(data2,size2);

      // xmidi,midi,xmididls,xmidimls with dls or mls
      if (((type1==AILFILETYPE_MIDI) || (type1==AILFILETYPE_XMIDI) || (type1==AILFILETYPE_XMIDI_DLS) || (type1==AILFILETYPE_XMIDI_MLS)) &&
         ((type2==AILFILETYPE_DLS) || (type2==AILFILETYPE_MLS)))
        goto good;

      // xmidi,midi with xmididls, xmidimls, dls or mls
      if (((type1==AILFILETYPE_MIDI) || (type1==AILFILETYPE_XMIDI)) &&
         ((type2==AILFILETYPE_XMIDI_DLS) || (type2==AILFILETYPE_XMIDI_MLS) || (type2==AILFILETYPE_DLS) || (type2==AILFILETYPE_MLS)))
        goto good;

      // xmidi,midi,xmididls,xmidimls with dls or mls
      if (((type2==AILFILETYPE_MIDI) || (type2==AILFILETYPE_XMIDI) || (type2==AILFILETYPE_XMIDI_DLS) || (type2==AILFILETYPE_XMIDI_MLS)) &&
         ((type1==AILFILETYPE_DLS) || (type1==AILFILETYPE_MLS)))
        goto swap;

      // xmidi,midi with xmididls, xmidimls, dls or mls
      if (((type2==AILFILETYPE_MIDI) || (type2==AILFILETYPE_XMIDI)) &&
         ((type1==AILFILETYPE_XMIDI_DLS) || (type1==AILFILETYPE_XMIDI_MLS) || (type1==AILFILETYPE_DLS) || (type1==AILFILETYPE_MLS))) {
        U32* t;
        U32 tmp;
       swap:
        t=data1;
        data1=data2;
        data2=t;

        tmp=type1;
        type1=type2;
        type2=tmp;

        tmp=size1;
        size1=size2;
        size2=tmp;

        strcpy(file1,file2);

       good:
        if (type1==AILFILETYPE_MIDI) {
          void*xmidi;

          show_slow("Converting...");
          result=AIL_MIDI_to_XMI(data1,size1,&xmidi,&size1,AILMIDITOXMI_TOLERANT);
          hide_slow();

          if (result) {
            AIL_mem_free_lock(data1);
            data1=xmidi;
          } else {
            show_MSS_error();
            goto cont;
          }
        }

        if ((type2==AILFILETYPE_XMIDI_DLS) || (type2==AILFILETYPE_XMIDI_MLS)) {
          // extract the DLS data
          U32 dsize;

          if (AIL_find_DLS(data2,size2,0,0,0,&dsize)) {
            void* dls;
            void* tmp;
            dls=AIL_mem_alloc_lock(dsize);
            if (dls==0) {
              show_error("Out of memory.");
              goto cont;
            }
            AIL_find_DLS(data2,size2,0,0,&tmp,0);
            memcpy(dls,tmp,dsize);
            AIL_mem_free_lock(data2);
            data2=dls;
            size2=dsize;
          } else {
            show_MSS_error();
            goto cont;
          }

        }

        switch (MessageBox(par,"Do you want to remove instruments that aren't use in the MIDI sequence (makes the final file much smaller)?","Remove unused instruments?",MB_YESNOCANCEL|MB_ICONQUESTION)) {
          case IDYES:
            {
              void*filt;

              show_slow("Removing the unused instruments...");
              result=AIL_filter_DLS_with_XMI(data1,data2,&filt,&size2,0,comp_cb);
              hide_slow();

              if (result) {
                AIL_mem_free_lock(data2);
                data2=filt;
              } else {
                show_MSS_error();
                goto cont;
              }

            }
            break;
          case IDCANCEL:
            goto cont;
        }

        if ((type2==AILFILETYPE_DLS) || (type2==AILFILETYPE_XMIDI_DLS)) {
          switch (MessageBox(par,"Do you want to compress the DLS data before merging it with the MIDI sequence?","Compress?",MB_YESNOCANCEL|MB_ICONQUESTION)) {
            case IDYES:
              {
                void*comp;

                char* ourext=ask_to_use_asi();
                if (((SINTa)ourext)==-1)
                  goto cont;

                show_slow("Compressing the DLS data...");
                result=AIL_compress_DLS(data2,ourext,&comp,&size2,comp_cb);
                hide_slow();

                if (result) {
                  AIL_mem_free_lock(data2);
                  data2=comp;
                } else {
                  show_MSS_error();
                  goto cont;
                }

              }
              break;
            case IDCANCEL:
              goto cont;
          }
        }

        show_slow("Merging...");
        result=AIL_merge_DLS_with_XMI(data1,data2,&final,&finalsize);
        hide_slow();

        if (result) {

          if (get_outname(file1,4,"Enter the XMIDI filename to merge the MIDI and DLS data into:"))
            file_write(file1,final,finalsize);

          AIL_mem_free_lock(final);
        }

        goto cont;
      }

      show_error("You have to highlight both an MIDI/XMIDI file AND a DLS/MLS file.");

     cont:
      AIL_mem_free_lock(data2);
    }

    AIL_mem_free_lock(data1);
  }

}


//############################################################################
//##                                                                        ##
//## Filter DLS file with one or more MIDI sequence                         ##
//##                                                                        ##
//############################################################################

static void filter()
{
  char each[256];
  char dlsf[256];
  UINTa midis[256];
  void* dls;
  char* e;
  void* tmp;
  U32 size;
  S32 result;
  S32 which=0;

  midis[0]=0;
  dls=0;

  get_current_files(output,4096);

  e=extract_filename(each,output);

  while (*each) {
    U32 type;

    if (!file_read(each,&tmp,0))
      goto cleanup;

    size=AIL_file_size(each);

    type=AIL_file_type(tmp,size);

    if (type==AILFILETYPE_MIDI) {

      //convert to xmidi on-the-fly
      void* xmidi;
      show_slow("Converting...");
      result=AIL_MIDI_to_XMI(tmp,size,&xmidi,0,AILMIDITOXMI_TOLERANT);
      hide_slow();

      if (result==0) {
        show_MSS_error();
        goto err;
      }

      AIL_mem_free_lock(tmp);
      tmp=xmidi;
      goto xmidiadd;

    } else {

      if ((type==AILFILETYPE_XMIDI) || (type==AILFILETYPE_XMIDI_DLS) ||(type==AILFILETYPE_XMIDI_MLS)) {

        // save the xmidi pointer
       xmidiadd:
        midis[++midis[0]]=(UINTa)tmp;

      } else {

        if ((type==AILFILETYPE_DLS) || (type==AILFILETYPE_MLS)) {

          // check to see if we've already had a DLS file
          if (dls) {

            show_error("You can only select one DLS/MLS file to filter against at a time.");
           err:
            AIL_mem_free_lock(tmp);
            goto cleanup;

          }

          // first one, so save the name and pointer
          strcpy(dlsf,each);
          dls=tmp;
          which=(type==AILFILETYPE_DLS)?5:3;

        } else {

          // some other file of file
          show_error("You can only choose MIDI, XMIDI, and DLS/MLS files for filtering.");
          goto err;

        }
      }
    }

    e=extract_filename(each,e);

  };

  if ((midis[0]==0) || (*dlsf==0)) {

    show_error("To filter, you must select one DLS/MLS file, AND at least one MIDI or XMIDI file.\n\n"
               "To highlight more than one file at a time, hold down the 'Control' key as you click.");

  } else {

    show_slow("Filtering the unused instruments...");
    result=AIL_filter_DLS_with_XMI(midis,dls,&tmp,&size,AILFILTERDLS_USINGLIST,comp_cb);
    hide_slow();

    AIL_mem_free_lock(dls);
    dls=tmp;

    if (result) {

      if (which==5) {
        switch (MessageBox(par,"Do you want to compress the DLS data now that you've filtered it?","Compress?",MB_YESNOCANCEL|MB_ICONQUESTION)) {
          case IDYES:
            {
              void*comp;

              char* ourext=ask_to_use_asi();
              if (((SINTa)ourext)==-1)
                goto cleanup;

              show_slow("Compressing the DLS data...");
              result=AIL_compress_DLS(dls,ourext,&comp,&size,comp_cb);
              hide_slow();

              if (result) {
                AIL_mem_free_lock(dls);
                dls=comp;
                which=3;
              } else {
                show_MSS_error();
                goto cleanup;
              }

            }
            break;
          case IDCANCEL:
            goto cleanup;
        }
      }

      if (get_outname(dlsf,which,"Enter the DLS filename to save the filtered DLS data to:"))
        file_write(dlsf,dls,size);

    }

  }

 cleanup:

  // free all the saved midis
  while(midis[0]) {
    AIL_mem_free_lock((void FAR*)midis[midis[0]]);
    --midis[0];
  }

  if (dls)
    AIL_mem_free_lock(dls);

}


static U32 minw,minwextra,firstsize=1;

static void checksize()
{
  RECT rc1,rc2;
  if (firstsize)
  {
    firstsize=0;
    GetWindowRect(GetDlgItem(dlg,200),&rc1);
    GetWindowRect(GetDlgItem(dlg,149),&rc2);
    minw=(rc1.right-rc1.left);
    minwextra=((rc2.left-rc1.left)*4);
  }
}

WNDPROC OldParWndProc;

LONG_PTR FAR PASCAL ParentProc(HWND hWnd, unsigned msg, UINT wparam, LONG_PTR lparam)
{
  WINDOWPLACEMENT r;
  SINTa i;

  switch (msg) {
    case WM_GETMINMAXINFO:
      checksize();
      i=CallWindowProc(OldParWndProc, hWnd, (UINT)msg, wparam,lparam);
      ((LPMINMAXINFO)lparam)->ptMinTrackSize.x=minw+minwextra;
      return(i);

    case WM_SHOWWINDOW:
      if ((!firstshow) && (firstoutershow)) {
        firstoutershow=0;
        r.length=sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(par,&r);
        if ((sizeable) && (W) && (H)) {
          r.rcNormalPosition.right=W+X;
          r.rcNormalPosition.bottom=H+Y;
        } else {
          r.rcNormalPosition.right=r.rcNormalPosition.right-r.rcNormalPosition.left+X;
          r.rcNormalPosition.bottom=r.rcNormalPosition.bottom-r.rcNormalPosition.top+Y;
        }
        r.rcNormalPosition.left=X;
        r.rcNormalPosition.top=Y;
        checksize();
        if (sizeable==0) {
          SetWindowLong(par,GWL_STYLE,GetWindowLong(par,GWL_STYLE)|WS_SIZEBOX);
          InsertMenu(GetSystemMenu(par,0),2,MF_BYPOSITION,SC_SIZE,"&Size");
          minwextra/=2;
          SetWindowPlacement(par,&r);
          SetWindowLong(par,GWL_STYLE,GetWindowLong(par,GWL_STYLE)&~WS_SIZEBOX);
          RemoveMenu(GetSystemMenu(par,0),2,MF_BYPOSITION);
        } else
          SetWindowPlacement(par,&r);
      }
      break;

    case WM_SIZE:
      if (wparam==SIZE_MINIMIZED)
        return(0);  // this hack fixes the common dialog handler from
                    // ever seeing a minimized window
      break;

    case WM_DESTROY:
      //
      // save the position before the window is history
      //

      r.length=sizeof(WINDOWPLACEMENT);
      GetWindowPlacement(par,&r);
      X=r.rcNormalPosition.left;
      Y=r.rcNormalPosition.top;
      W=r.rcNormalPosition.right-r.rcNormalPosition.left;
      H=r.rcNormalPosition.bottom-r.rcNormalPosition.top;
      break;

  }
  return(CallWindowProc(OldParWndProc, hWnd, (UINT)msg, wparam,lparam));
}

//
// message handler for the standard dialog box
//

UINT_PTR CALLBACK OFNHookProc(
  HWND hdlg,      // handle to child dialog window
  UINT uiMsg,     // message identifier
  WPARAM wParam,  // message parameter
  LPARAM lParam   // message parameter
)
{
  HMENU menu;

  switch(uiMsg) {
    case WM_SHOWWINDOW:
      if (firstshow) {
        firstshow=0;

        //
        // get the dialog window
        //

        dlg=hdlg;
        par=GetParent(hdlg);

        //
        // set the window position, drop in an icon, and change the
        //   labels on the common buttons
        //
        // This hack gets rid of spurious warning due to broken MS header...
        //

#ifdef IS_WIN64
        SetClassLongPtr(par, GCLP_HICON, (LONG_PTR) LoadIcon(hInst,"MILESSTU"));
#else
        SetClassLong(par, GCL_HICON, (U32) (UINTa) LoadIcon(hInst,"MILESSTU"));
#endif
        
        menu=GetSystemMenu(par,0);
        if (GetMenuItemCount(menu)>2) {
          InsertMenu(menu,0,MF_BYPOSITION,SC_RESTORE,"&Restore");
          InsertMenu(menu,3,MF_BYPOSITION,SC_MINIMIZE,"Mi&nimize");
          InsertMenu(menu,4,MF_BYPOSITION,SC_MAXIMIZE,"Ma&ximize");
          SetWindowLong(par,GWL_STYLE,GetWindowLong(par,GWL_STYLE)|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
          sizeable=1;
        } else {
          InsertMenu(menu,0,MF_BYPOSITION,SC_RESTORE,"&Restore");
          InsertMenu(menu,2,MF_BYPOSITION,SC_MINIMIZE,"Mi&nimize");
          SetWindowLong(par,GWL_STYLE,(GetWindowLong(par,GWL_STYLE)|WS_MINIMIZEBOX)&~WS_MAXIMIZEBOX);
          sizeable=0;
        }

        SetWindowPos(par,0,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

        CommDlg_OpenSave_SetControlText(par, 1, "&Play");
        CommDlg_OpenSave_SetControlText(par, 2, "Close");

        // This hack gets rid of spurious warning due to broken MS header...
        //

#ifdef IS_WIN64
        OldParWndProc = (WNDPROC)SetWindowLongPtr(par,GWLP_WNDPROC,(LONG_PTR)ParentProc);
#else
        OldParWndProc = (WNDPROC)(UINTa)SetWindowLong(par,GWL_WNDPROC,(DWORD)(UINTa)ParentProc);
#endif

      }
      break;

    case WM_NOTIFY:
      //
      //
      switch (((LPOFNOTIFY)lParam)->hdr.code) {
        case CDN_FILEOK:
          //
          // tell the common dialog that we're handling the play button
          //
          SetWindowLong(hdlg,DWLP_MSGRESULT,1);
          play_file();
          return 1;
        case CDN_FOLDERCHANGE:
          //
          // keep track of the current dir
          //
          CommDlg_OpenSave_GetFolderPath(par, dir, 256);
          break;
      }
      break;

    case WM_HELP:
      //
      // display help on the buttons
      //

      if (((((LPHELPINFO)lParam)->iCtrlId)>=149) && ((((LPHELPINFO)lParam)->iCtrlId)<=158))
        MessageBox(par,help[(((LPHELPINFO)lParam)->iCtrlId)-149],"Help...",MB_OK|MB_ICONINFORMATION);
      else if ((((LPHELPINFO)lParam)->iCtrlId)<=101) // help on about is about
        show_about();
      return(1);

    case WM_COMMAND:
      //
      // these are the extra buttons only
      //

      switch (wParam) {
        case 149:
          compress();
          break;

        case 150:
          decompress();
          break;

        case 151:
          show_midi();
          break;

        case 152:
          show_dls();
          break;

        case 153:
          to_xmidi();
          break;

        case 154:
          to_raw();
          break;

        case 155:
          unmerge();
          break;

        case 156:
          filter();
          break;

        case 157:
          merge();
          break;

        case 158:
          show_type();
          break;

        case 100:
        case 101:
          show_about();
          break;

      }
      break;

    case WM_USER+0x88:
      show_error((char*)lParam);
      break;
  }
  return(0);
}

//############################################################################
//##                                                                        ##
//## Length double 0 terminated string                                      ##
//##                                                                        ##
//############################################################################

static S32 len_dbl_str(char* dbl)
{
  S32 i=0;
  S32 len;

  while((len=(S32)strlen(dbl))!=0) {
    i+=(len+1);
    dbl+=(len+1);
  }
  return( i+1 );
}

//############################################################################
//##                                                                        ##
//## Load any ASI encoders                                                  ##
//##                                                                        ##
//############################################################################

static void load_any_ASI_encoders()
{
    char* types;
    S32 len;

    static HPROPERTY NAME;
    static HPROPERTY TYPES;
    static PROVIDER_PROPERTY PROVIDER_property;

    RIB_INTERFACE_ENTRY ASICODEC[] =
      {
      PR("Name",NAME),
      PR("Output file types",TYPES),
      FN(PROVIDER_property)
      };

    ASI = RIB_find_file_provider("ASI codec",
                                 "Output file types",
                                 "mp3");

    if (ASI) {
      RIB_request(ASI,"ASI codec",ASICODEC);

      ASI_name = 0;
      PROVIDER_property(NAME, &ASI_name, 0, 0 );

      types = 0;
      PROVIDER_property(TYPES, &types, 0, 0 );

      len=len_dbl_str(types)-1;
      memcpy(ASI_types,types,len);
      types="All files (*.*)\0*.*\0";
      
      memcpy(ASI_types+len,types,len_dbl_str(types));
      memcpy(ASI_type,ASI_types+strlen(ASI_types)+3,3);
      ASI_type[3]=0;

      savefilt[6]=ASI_types;
      savedef[6]=ASI_type;
    }
}


//############################################################################
//##                                                                        ##
//## WinMain()                                                              ##
//##                                                                        ##
//############################################################################

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   OPENFILENAME fn;

   hInst=hInstance;

   waitcur=LoadCursor(0,IDC_WAIT);

   AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );
   AIL_startup();

   memset(&fn, 0, sizeof(fn));
   output[0]=0;

   //
   // get the saved info
   //

   GetProfileString(SECTION,"Path","",dir,256);
   X=GetProfileInt(SECTION,"X",(UINT)X);
   Y=GetProfileInt(SECTION,"Y",(UINT)Y);
   W=GetProfileInt(SECTION,"W",(UINT)W);
   H=GetProfileInt(SECTION,"H",(UINT)H);

   //
   // setup the main window
   //

   fn.lStructSize       = sizeof(fn);
   fn.hInstance         = hInstance;
   fn.hwndOwner         = 0;
   fn.lpstrFilter       = "Sound files (*.mss;*.xmi;*.mid;*.rmi;*.dls;*.mls;*.wav;*.raw;*.mp?;*.ogg;*.v29;*.v24;*.v12)\0*.mid;*.mss;*.rmi;*.xmi;*.wav;*.mls;*.dls;*.raw;*.mp?;*.ogg;*.v29;*.v24;*.v12;\0MIDI files (*.mss;*.xmi;*.mid;*.rmi)\0*.mss;*.xmi;*.mid;*.rmi\0DLS files (*.dls;*.mls)\0*.dls;*.mls\0Digital sound files (*.wav;*.raw;*.mp?;*.ogg;*.v29;*.v24;*.v12)\0*.wav;*.raw;*.mp?;*.ogg;*.v29;*.v24;*.v12\0All files (*.*)\0*.*\0";
   fn.nFilterIndex      = 1;
   fn.lpfnHook          = OFNHookProc;
   fn.lpstrFile         = output;
   fn.lpstrInitialDir   = dir;
   fn.nMaxFile          = 4096;
   fn.lpstrTitle        = "Miles Sound Studio";
   fn.Flags             = OFN_FILEMUSTEXIST |
                          OFN_EXPLORER |
                          OFN_LONGNAMES |
                          OFN_ENABLETEMPLATE |
                          OFN_PATHMUSTEXIST |
                          OFN_ENABLEHOOK |
                          OFN_ALLOWMULTISELECT |
                          OFN_HIDEREADONLY |
                          OFN_ENABLESIZING;

   fn.lpstrDefExt       = "xmi";
   fn.lpTemplateName    = "MILESSTU";


   //
   // Snag a provider for optional ASI compression
   //

   load_any_ASI_encoders();

   //
   // open the main application
   //

   GetOpenFileName(&fn);

   WriteProfileString(SECTION,"Path",dir);
   WriteProfileInt(SECTION,"X",X);
   WriteProfileInt(SECTION,"Y",Y);
   WriteProfileInt(SECTION,"W",W);
   WriteProfileInt(SECTION,"H",H);

   AIL_shutdown();

   return(0);
}

