//
// sample C code using LG sound library
//  Pat McElhatton  July 18 '96
//  mainly a ripoff of Golf's sound.c module
//

#include <windows.h>
#include <lg.h>
#include <res.h>
#include <mprintf.h>
#include <stdio.h>
#include <stdlib.h>
#include <lgsound.h>
#include <wappapi.h>
#include <appapi.h>
#include <appagg.h>

// maximum # of simultaneous active sounds
#define MAX_SOUNDS 8
static BOOL donePlaying = FALSE;

////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundInit()
//
// Description: Initialize the sound module.
// 
////////////////////////////////////////////////////////////////////////
ISndMixer *
SoundInit ( void )
{
   ISndMixer *pMixer = NULL;

   if (SndCreateMixer (&pMixer, NULL) == FALSE) {
      printf ("Sound system not started\n");
   } else {
      sSndSetup setup;
      setup.hwnd = NULL;
        
      if (ISndMixer_Init (pMixer, &setup, MAX_SOUNDS, NULL) != kSndOk) {
         printf ("Sound system init failed\n");
      }
   }            

   return pMixer;
}


////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundTerm()
//
// Description: Terminate the sound module.
// 
////////////////////////////////////////////////////////////////////////
void
SoundTerm ( ISndMixer *pMixer )
{                              
   if (pMixer) {
      ISndMixer_StopAllSamples (pMixer);
      ISndMixer_Release (pMixer);
      pMixer = NULL;
   }    
}


////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundRecur()
//
// Description: Recur into the sound module for management purposes.
// 
////////////////////////////////////////////////////////////////////////
void
SoundRecur( ISndMixer *pMixer )
{
   if ( pMixer ) {
      // allow any samples which have finished playing to do
      //   end-of-sample cleanup
      ISndMixer_Update( pMixer );
   }
}

        
////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundCallback()
//
// this will be called when sample finishes playing,
//  it will unlock the sample resource and release the sample
// 
////////////////////////////////////////////////////////////////////////
static void
SoundCallback(ISndSample *pSample,
              void *pCBData)
{
   ISndSample_Release ( pSample );
   donePlaying = TRUE;
}


////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundPlayRez()
//
// Description: Play a sound, given its resource id.
// 
////////////////////////////////////////////////////////////////////////
ISndSample *
SoundPlayRez ( Id          id,
               ISndMixer   *pMixer,
               int32       panVal,
               int32       volumeVal )
{
   ISndSample *pSample = NULL;

   if ( pMixer ) {
      uchar *data = ResLock (id);
      pSample = ISndMixer_CreateSampleFromRez( pMixer, kSndSampleNormal, data, ResSize (id) );
      ResUnlock( id );

      if ( pSample ) {

         ISndSample_SetPan( pSample, panVal );
         ISndSample_SetVolume( pSample, volumeVal );
         // play it
         ISndSample_Play( pSample );

         // setup and end-of-sample callback which will release the
         //  sample and unlock the resource
         ISndSample_RegisterEndCallback (pSample, SoundCallback, (void *) id );
      }    
   }

   return pSample;
}

////////////////////////////////////////////////////////////////////////
// 
// Function name: SoundPlayFile()
//
// Description: Play a sound, given its filename.
// 
////////////////////////////////////////////////////////////////////////
ISndSample *
SoundPlayFile ( char        *fName,
                ISndMixer   *pMixer,
                int32       volumeVal,
                int32       panVal )
{
   ISndSample  *pSample = NULL;
   long        fileLen;
   FILE        *inFile;
   void        *pFileBuff;

   if ( pMixer ) {
      
      inFile = fopen( fName, "rb" );
      if ( inFile == NULL ) {
         return NULL;
      }
      fseek( inFile, 0, SEEK_END );
      fileLen = ftell( inFile );
      fseek( inFile, 0, SEEK_SET );
      pFileBuff = malloc( fileLen );
      if ( fread( pFileBuff, fileLen, 1, inFile ) != 1 ) {
         free( pFileBuff );
         return NULL;
      }
      pSample = ISndMixer_CreateSampleFromRez( pMixer, kSndSampleNormal, pFileBuff, fileLen );
        
      if ( pSample ) {

         ISndSample_SetPan( pSample, panVal );
         ISndSample_SetVolume( pSample, volumeVal );
         // play it
         ISndSample_Play( pSample );

         // setup an end-of-sample callback which will release the
         //  sample and unlock the resource
         ISndSample_RegisterEndCallback (pSample, SoundCallback, NULL );
      } else {
         printf( "Error in Mixer::CreateSampleFromRez!\n" );
         donePlaying = TRUE;
      }
   }
   return pSample;
}


LRESULT FAR PASCAL WndProc (HWND hWnd, UINT iMessage, WPARAM wParam, 
    LPARAM lParam)
{
    switch (iMessage)       
    {
        case WM_COMMAND:    // process menu items 
           break ;
        case WM_DESTROY:
            PostQuitMessage (0) ;
            break ;
        default:            // default windows message processing 
            return DefWindowProc (hWnd, iMessage, wParam, lParam) ;
    }
    return (0L) ;
}


#define APPNAME "testsnd1"

int LGAPI
AppMain( int argc,
         const char **argv )
{
   ISndMixer *pMixer;
   int nTimer;
   HWND        hWnd ;     // a handle to a message 
   MSG         msg ;      // a message 
   WNDCLASS    wndclass ; // the window class 
   IWinApp     *pWinApp;
   HINSTANCE hInstance;

   hInstance = GetModuleHandle( NULL );

   // load data into window class struct. 
   wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
   wndclass.lpfnWndProc   = WndProc ;
   wndclass.cbClsExtra    = 0 ;
   wndclass.cbWndExtra    = 0 ;
   wndclass.hInstance     = hInstance ;
   wndclass.hIcon         = LoadIcon (hInstance, APPNAME) ;
   wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
   wndclass.hbrBackground = GetStockObject (WHITE_BRUSH) ;
   wndclass.lpszMenuName  = APPNAME;
   wndclass.lpszClassName = APPNAME;
          
   // register the window class 
   if (!RegisterClass (&wndclass)) {
      return FALSE ;
   }
    
   hWnd = CreateWindow (       // create the program's window here 
                        APPNAME,                // class name 
                        APPNAME,                // window name 
                        WS_OVERLAPPEDWINDOW,    // window style 
                        CW_USEDEFAULT,          // x position on screen 
                        CW_USEDEFAULT,          // y position on screen 
                        CW_USEDEFAULT,          // width of window 
                        CW_USEDEFAULT,          // height of window 
                        NULL,                   // parent window handle (null = none) 
                        NULL,                   // menu handle (null = class menu) 
                        hInstance,              // instance handle 
                        NULL) ;                 // lpstr (null = not used) 
   
   pWinApp = AppGetObj( IWinApp );
   IWinApp_SetMainWnd( pWinApp, hWnd );
   SafeRelease( pWinApp );

   ShowWindow (hWnd, SW_SHOW) ;   
   //ShowWindow (hWnd, nCmdShow) ;   
   UpdateWindow (hWnd) ;       // send first WM_PAINT message 
    
   pMixer = SoundInit();

   SoundPlayFile( argv[1], pMixer, 127, 0 );

   nTimer = SetTimer( hWnd, 1, 60, NULL );  // send timer events every 60 milliseconds
   while (GetMessage (&msg, NULL, 0, 0))   // the message loop 
   {
        TranslateMessage (&msg) ;
        SoundRecur( pMixer );
        DispatchMessage (&msg) ;
        if ( donePlaying ) {
           KillTimer( hWnd, nTimer );
           break;
        }
    }
    return msg.wParam ;

}

tResult LGAPI AppCreateObjects(int argc, const char * argv[])
{
   GenericApplicationCreate( argc, argv, "testsnd1", NULL );

   return NOERROR;
}

