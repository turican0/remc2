
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include <commctrl.h>
#include <commdlg.h>
#include <memory.h>
#include <cderr.h>

#include <inttype.h>
#include <resource.h>

#include <lgsound.h>

#define VOL_MAX 127
#define VOL_MIN 0
#define VOL_PAGESIZE 16

typedef struct {
	uint32 len;
	uint8 *data;
} snd_file;

typedef struct {
	int			file;
	uint32		len;
	BOOL			done;
	uint8			*buffer;
	ISndSample	*pSample;
} snd_db;

snd_db db_sound = {-1,0,0,NULL};
#define DB_SIZE 8192

#define PRINT_INT(i) { char tmp[32];sprintf(tmp, "%d", i);l_printf(tmp); }
#define PRINT_DB_LOAD() PRINT_INT(db_sound.len)

BOOL alloc_double_buffer(snd_db *db, char *fname);
uint8 *read_next_chunk(snd_db *db);
void close_double_buffer(snd_db *db);
void check_db(snd_db *db);
void HandleVolScroll( int nCode, int nPos );

char szAppClass[] = "SoundWndClass";
char szAppName[]  = "SoundTest";
char szAppCaption[] = "Sound Test";
char szTemp[256];
 
char szFileBuffer[MAX_PATH];
char szFileTitle[MAX_PATH];
char szOpenFilter[] = "Voc Files (*.pcm)\0*.PCM";

HWND hWndMain, hWndPlay, hWndList, hWndStop, hWndSlider;
HINSTANCE hInst;
UINT gTimer;
int gVolume;

snd_file fileDat = {0, NULL};

ISndMixer *gMixer = NULL;
int gListItems;

static BOOL InitApp( HINSTANCE hInstance );
static BOOL InitInstance( HINSTANCE hInstance, int nCmdShow );

LRESULT CALLBACK MainWindowProc( HWND hWnd, unsigned uMsg,
									WPARAM wParam, LPARAM lParam );

void CALLBACK SoundTimerCallback(HWND, UINT, UINT,DWORD); 
HWND MakeButton(char *title, int x, int y, int w, int h, int id);
void GetFile();

uint8 *read_wave_file(char *fname, snd_file *ldat);
void l_printf(char *str);
ISndSample * sample_finish(ISndMixer *mixer, ISndSample *);

//-----------------------------------------------------------------------------
// WinMain()
//
// Entry point for all Windows programs - performs initialization, message loop
//-----------------------------------------------------------------------------

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR lpCmdLine, int nCmdShow )
{
    MSG     msg;

    hInst = hInstance;

    /* Make sure the common controls are loaded for our use */
    //InitCommonControls();

    if( !hPrevInstance )
        if( !InitApp( hInstance ))
            {
            MessageBox(NULL, "Ugh", "Init App Failed", MB_ICONSTOP );
            return( FALSE );
            }

    if( !InitInstance( hInstance, nCmdShow ))
    {
        MessageBox( NULL, "Ugh", " Init Instance Failed", MB_ICONSTOP );
        return( FALSE );
    }

    while( GetMessage((LPMSG)&msg, NULL, 0, 0 ))
        {
        if(db_sound.pSample)
			check_db(&db_sound);
		  TranslateMessage( &msg );
        DispatchMessage( &msg );
        }

	KillTimer(hWndMain, gTimer);

	
//	if(fileDat.data != NULL)
//		free(fileDat.data);

    UnregisterClass( szAppClass, hInstance );

    return( msg.wParam );
} /* End of WinMain() */


//-----------------------------------------------------------------------------
// InitApp()                                                                 */
//                                                                           */
//   Inits things that only need to be created once for the this application */
// (like creating the window class).                                         */
//-----------------------------------------------------------------------------

static BOOL InitApp( HINSTANCE hInstance )
{
    WNDCLASS    wc;

    /* Set up and register a window class */
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName    = szAppClass;
    wc.lpfnWndProc      = (WNDPROC)MainWindowProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = sizeof( DWORD );
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL; //LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ));
    wc.hCursor          = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground    = (HBRUSH)(COLOR_WINDOW);
    wc.lpszMenuName     = MAKEINTRESOURCE( IDR_MENU1 );

    if( !RegisterClass( &wc ))
        {
        MessageBox(NULL, "Ugh", "Register Class Failed", MB_ICONSTOP );
        return( FALSE );
        }

    return( TRUE );
 } /* End of InitApp() */


//-----------------------------------------------------------------------------
//
// InitInstance()                                                            
//                                                                           
// Performs initialization that must be done for each application instance.  
//                                                                           
//-----------------------------------------------------------------------------

static BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    HWND        hWnd;
    RECT        crect;

    hWnd = CreateWindow( 	szAppClass,            	// class name
                        	szAppCaption,           // caption for window
                        	WS_OVERLAPPEDWINDOW,    // style 
                        	CW_USEDEFAULT,          // x position
                        	CW_USEDEFAULT,          // y position 
                        	300,                    // width 
                        	300,                    // height 
                        	NULL,                   // parent window 
                        	NULL,                   // menu 
                        	hInstance,              // instance 
                        	NULL );                 // parms 

    if( !hWnd ) 
    {
        MessageBox(NULL, "Init Window Failed", "Ugh", MB_ICONSTOP );
        return FALSE;
    }

    gTimer = SetTimer(hWnd, 1, 50, (TIMERPROC)SoundTimerCallback);

    hWndMain = hWnd;
    GetClientRect( hWndMain, &crect );

	hWndPlay = MakeButton("Play", 10,10,60,30, ID_SOUND_PLAY);
	hWndStop = MakeButton("Stop", 10,42,60,30, ID_SOUND_STOP);

	hWndList = CreateWindow( "listbox", NULL, WS_CHILD | WS_VISIBLE
                                | LBS_NOINTEGRALHEIGHT | WS_VSCROLL,
                                80, 10, 200, 200,
                                hWnd, NULL, hInstance, NULL );

	RECT lRect;
	ListBox_GetItemRect(hWndList,0, &lRect);
	gListItems = 200/(lRect.bottom - lRect.top);

    // Make the volume trackbars.
    // Create main volume bar.
	hWndSlider = CreateWindow(
        TRACKBAR_CLASS, 
        "", 
        WS_CHILD | WS_VISIBLE | TBS_VERT | TBS_BOTH, 
        10,
        78,
        40, 
        100,              
        hWnd, 
        (HMENU)(0),
        hInstance, 
        NULL);

   SendMessage(hWndSlider, TBM_SETRANGE, FALSE, MAKELONG(VOL_MIN, VOL_MAX)); 
   SendMessage(hWndSlider, TBM_SETPOS, TRUE, VOL_MIN);
   SendMessage(hWndSlider, TBM_SETPAGESIZE, FALSE, VOL_PAGESIZE);
	gVolume = VOL_MAX;


    // Create some controls for things like volume, panning, etc.
    //if( CreateChildren( crect ))
    //    return( FALSE );
	
//	if(read_wave_file("test.wave", &fileDat) == NULL)
//		MessageBox(hWndMain, "Open File Failed", "Uh oh", MB_ICONSTOP);		
//	else
//		l_printf("test.wave");

	SndCreateMixer(&gMixer, NULL);

	if(gMixer)
	{
		sSndSetup setup;
		setup.hwnd = hWndMain;
		if(gMixer->Init(&setup, 8) != kSndOk)
			MessageBox(hWndMain, "Sound System Not Started", "Drat!", MB_ICONSTOP);
		else
			gMixer->RegisterEosCallback(sample_finish);
	}

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

	return( TRUE );
}


//----------------------------------------------------------------------------
// MainWindowProc()                                                         
//                                                                          
//    The callback that Windows calls for your window                         
//                                                                          
//----------------------------------------------------------------------------

LRESULT CALLBACK MainWindowProc( HWND hWnd, unsigned uMsg,
                                                WPARAM wParam, LPARAM lParam )
{
float   fPercent;
UINT    uPercent;
BOOL    bResult = FALSE;
int     nChkErr;
HRESULT dsrval;
static ISndSample *sample = NULL;

    switch( uMsg )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ))
            {
				case ID_SOUND_PLAY:
				{
					if(sample == NULL)
						sample = gMixer->CreateSample(kSndSampleDoubleBuffered, 
																	NULL,0, kSndPcmFile);
					if(sample)
					{
						db_sound.pSample = sample;
						db_sound.pSample->AddRef();

						read_next_chunk(&db_sound);
						if(sample->BufferReady(DB_SIZE) != SND_BUFFER_NOT_READY)
						{
							sample->LoadBuffer(db_sound.buffer, db_sound.len);
							sample->Loop();
							l_printf("playing sound...");

							PRINT_DB_LOAD();

							read_next_chunk(&db_sound);
						}
					}
					break;
				}
				case ID_SOUND_STOP:
					if(sample)
					{	
                                                sample->Stop();
						db_sound.done = TRUE;
						sample->Release();
						sample = NULL;
					}
					break;
				case ID_FILE_OPENWAVE:
					if(sample)
					{
						sample->Stop();
						sample->Release();
						sample = NULL;
					}
					GetFile();
					break;
				case ID_VOLUME:
					l_printf("vol");
					break;
            }
            break;
/*****
        case WM_HSCROLL:
            if(((HWND)lParam == hWndPan) && lpDSBStreamBuffer )
                {
                HandlePanScroll( (int)LOWORD(wParam), (int)HIWORD(wParam));
                }
            else if(((HWND)lParam == hWndVol) && lpDSBStreamBuffer )
                {
                HandleVolScroll( (int)LOWORD(wParam), (int)HIWORD(wParam));
                }
            else if(((HWND)lParam == hWndFreq) && lpDSBStreamBuffer )
                {
                HandleFreqScroll( (int)LOWORD(wParam), (int)HIWORD(wParam));
                }
            break;

        case WM_INITMENU:
            if((HWND)wParam != GetMenu( hWnd ))
                break;
            CheckMenuItem((HWND)wParam, IDM_OPTIONS_ENUMDRIVERS,
                                bEnumDrivers ? MF_CHECKED : MF_UNCHECKED );
            break;
*************/
        case WM_VSCROLL:
            if(((HWND)lParam == hWndSlider))
                {
                HandleVolScroll( (int)LOWORD(wParam), (int)HIWORD(wParam));
                }
				break;
      case WM_DESTROY:
		case ID_FILE_EXITME:

            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, uMsg, wParam, lParam );
        }
    return 0L;
} /* WindowProc */

uint8 *read_wave_file(char *fname, snd_file *ldat)
{
   int fh;

   if ((fh=open(fname,O_RDONLY|O_BINARY))!=-1)
   {
      long len;
      uint8 *ptr;

      len=lseek(fh,0,SEEK_END);
      lseek(fh,0,SEEK_SET);
      if ((ptr=(uint8 *)malloc(len))!=NULL)
      {
	      read(fh,ptr,len);
          close(fh);
	      if (ldat!=NULL)
	       { ldat->len = len; ldat->data=ptr; }
	      return ptr;
      }
      close(fh);
   }
   if (ldat!=NULL)
    { ldat->len=0; ldat->data=NULL; }
   return NULL;
}

BOOL alloc_double_buffer(snd_db *db, char *fname)
{
	db->file = open(fname, O_RDONLY|O_BINARY);
	db->buffer = (uint8 *)malloc(DB_SIZE);

	return (db->file != -1 && db->buffer != NULL);
}

uint8 *read_next_chunk(snd_db *db)
{
	if(db->file == -1) return NULL;
	if(db->buffer == NULL) return NULL;

	db->len = read(db->file, db->buffer, DB_SIZE);
	return db->buffer;
}

void close_double_buffer(snd_db *db)
{
	if(db->file != -1)
		close(db->file);
	if(db->buffer)
		free(db->buffer);
	if(db->pSample)
		db->pSample->Release();
	db->file = -1;
	db->buffer = NULL;
	db->pSample = NULL;
	db->done = FALSE;
}

void check_db(snd_db *db)
{
	if(db->pSample && db->done == FALSE)
	{
		if(db->pSample->BufferReady(db->len) != SND_BUFFER_NOT_READY)
		{
			db->pSample->LoadBuffer(db->buffer, db->len);
			if(db->len == 0)
				db->done = TRUE;
			PRINT_DB_LOAD();
			read_next_chunk(db);
		}
	}							
}

HWND MakeButton(char *title, int x, int y, int w, int h, int id)
{
	return CreateWindow( "button", title,
                                    WS_CHILD | WS_VISIBLE ,
                                    x, y, w, h, hWndMain,
                                    (HMENU)id, hInst, NULL );
}

// want to write my own sprintf thingy...
void l_printf(char *str)
{
	int num_items;
	ListBox_AddString(hWndList, str);
	num_items = ListBox_GetCount(hWndList);
	if(num_items > gListItems)
		ListBox_SetTopIndex(hWndList, num_items - gListItems + 1);

}	                                 

void GetFile()
{
   DWORD          dwCDErr = 0;
   BOOL           bResult;
   OPENFILENAME   ofn;

 	memset( &ofn, 0, sizeof(OPENFILENAME));
   ofn.lStructSize     = sizeof(OPENFILENAME);
   ofn.hwndOwner       = hWndMain;
   ofn.hInstance       = hInst;
   ofn.lpstrFilter     = szOpenFilter;
   ofn.nFilterIndex    = 1;
   szFileBuffer[0]     = '\0';
   ofn.lpstrFile       = szFileBuffer;
   ofn.nMaxFile        = sizeof(szFileBuffer);
   ofn.lpstrFileTitle  = szFileTitle;
   ofn.nMaxFileTitle   = sizeof(szFileTitle);
   ofn.lpstrDefExt     = "PCM";
   ofn.lpstrTitle      = "Open A File";
   ofn.Flags           = OFN_FILEMUSTEXIST;

   bResult = GetOpenFileName( &ofn ); /* Do the dialog box */

    //
    //  A return of TRUE indicates that the user did not select a filename.
    // The possible reasons are: Cancel was clicked, or an error occured.
    // If Cancel was clicked, the CommDlgExtendedError() function will not
    // return a valid code.  For anything else, an error code will come back.
    //
   if( bResult == FALSE )
   {
      dwCDErr = CommDlgExtendedError();
      if( dwCDErr )
      {
	  	;
         // Handle a common dialog box error 
         //HandleCommDlgError( dwCDErr );
      }
      else    // Clicked Cancel, so finish msg processing
         return;
   }                
   else
   {
      if( fileDat.len != 0 )
      {
      	free(fileDat.data);
			fileDat.data = NULL;
			fileDat.len = 0;
      }

     if(alloc_double_buffer(&db_sound, ofn.lpstrFile) == FALSE)
//	  if(read_wave_file(ofn.lpstrFile, &fileDat) == NULL)
		  MessageBox(hWndMain, "Open File Failed", "Uh oh", MB_ICONSTOP);		
	  else
	  {
		  char *fname = ofn.lpstrFile;

		  if((fname = strrchr(fname, '\\')) == NULL)
			  fname = ofn.lpstrFile;
		  else
			  fname++;

		  l_printf(fname);
	  }
   }
}

/*
void fin_call(snd_handle id, void *data)
{
	char buf[64] = "";

	sprintf(buf, "finish call for %d\l", id);

	l_printf(buf);

}
*/

void CALLBACK SoundTimerCallback(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if(gMixer)
		gMixer->Update();
}

ISndSample * sample_finish(ISndMixer *mixer, ISndSample *sample)
{
	l_printf("callback for sound!");
	return NULL;
}

void HandleVolScroll( int nCode, int nPos )
{
    long  lVol, lDelta;

    lVol = (LONG)SendMessage( hWndSlider, TBM_GETPOS, (WPARAM)0, (LPARAM)0 );

    switch( nCode )
        {
        case TB_LINEDOWN:
            if( lVol <= VOL_MAX - 1 )
                lDelta = 1;
            break;
        case TB_LINEUP:
            if( lVol >= VOL_MIN + 1 )
                lDelta = -1;
            break;
        case TB_PAGEDOWN:
            if( lVol <= VOL_MAX - VOL_PAGESIZE )
                lDelta = 10;
            break;
        case TB_PAGEUP:
            if( lVol >= VOL_MIN + VOL_PAGESIZE )
                lDelta = -10;
            break;
        case TB_ENDTRACK:
            return;
        default:
            lDelta = 0;
        }

	gVolume = 127 - lVol;//+= lDelta;
	
	if(db_sound.pSample)
	{
		//SendMessage( hWndSlider, TBM_SETPOS, TRUE, (lVol + lDelta));
		db_sound.pSample->SetVolume(gVolume);
		PRINT_INT(gVolume);
	}
   else
		SendMessage( hWndSlider, TBM_SETPOS, TRUE, (long)nPos );
}


