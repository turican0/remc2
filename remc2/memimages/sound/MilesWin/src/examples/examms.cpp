//############################################################################
//##                                                                        ##
//##  EXAMMS.C                                                              ##
//##                                                                        ##
//##  Multispeaker 3D test bed                                              ##
//##                                                                        ##
//##  V1.00 of 23-Sep-98: Initial V3.0 release                              ##
//##                                                                        ##
//##  Written by Jeff Roberts, John Miles                                   ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  For technical support, contact RAD Game Tools at 425-893-4300.        ##
//##                                                                        ##
//############################################################################

#include <windows.h>
#include <windowsx.h>
#include "mss.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <math.h>
#include <commctrl.h>

/* @cdep pre
  $if($ispresent($BuildDir,win64),
    $DefaultsWinEXE64EMT
    $requiresbinary(mss64.lib)
  ,
    $DefaultsWinEXE
    $requiresbinary(mss32.lib)
  )
  $requires(examms.rc)
  $requiresbinary(comctl32.lib comdlg32.lib)
*/

/* @cdep post
  $if($ispresent($BuildDir,win64),
    $BuildWinEXE64EMT
  ,
    $BuildWinEXE
  )
*/

//
// Configuration equates
//

#define APPNAME "EXAMMS"

#define OUTPUT_RATE     44100
#define OUTPUT_BITS     16

#define MAX_3D_SOUNDS   4
#define TIMER_UPDATE_MS 50

#define DEFAULT_3D_SAMPLE_FILENAME "..\\media\\shot.wav"

#define EPSILON       0.0001F
#define PI            3.14159265358979F
#define FASTSPEED    (400.0F / 3600.0F)  // (400 km/hour)
#define SLOWSPEED    (40.0F / 3600.0F)   // (40 km/hour)
#define CIRCLERADIUS  100.0F
#define STRAIGHTDIST  200.0F
#define CIRCLE       (2.0F * PI)

#define SOUNDSIZE     4     
#define LISTENERSIZE  12
#define CRATESIZE     12
#define WALLHEIGHT    48
#define WALLWIDTH     8

#define CLIENT_LISTENER_X       447
#define CLIENT_LISTENER_Y       162
#define CLIENT_LISTENER_RANGE_X 125
#define CLIENT_LISTENER_RANGE_Y 157

//
// Combo box entries
//

struct COMBO_ENTRY
{
   C8         *name;   
   MSS_MC_SPEC channel_spec;
};

COMBO_ENTRY combo_entries[] = 
{
   { "Use Windows speaker configuration", MSS_MC_USE_SYSTEM_CONFIG   },
   { "Mono",                              MSS_MC_MONO                },
   { "Stereo headphones",                 MSS_MC_HEADPHONES          },
   { "Stereo speakers",                   MSS_MC_STEREO              },
   { "Dolby Surround",                    MSS_MC_DOLBY_SURROUND      },
   { "SRS Circle Surround",               MSS_MC_SRS_CIRCLE_SURROUND },
   { "4.0 discrete",                      MSS_MC_40_DISCRETE         },
   { "5.1 discrete",                      MSS_MC_51_DISCRETE         },
   { "6.1 discrete",                      MSS_MC_61_DISCRETE         },
   { "7.1 discrete",                      MSS_MC_71_DISCRETE         },
   { "8.1 discrete",                      MSS_MC_81_DISCRETE         },
   { "DirectSound 3D hardware support",   MSS_MC_DIRECTSOUND3D       },
   { "Creative Labs EAX 2 (TM)",          MSS_MC_EAX2                },
   { "Creative Labs EAX 3 (TM)",          MSS_MC_EAX3                },
   { "Creative Labs EAX 4 (TM)",          MSS_MC_EAX4                },
};

//
// Display environment
//

typedef enum SNDMOVE
{
   AUTO_ROTATE,
   AUTO_BACK_AND_FORTH,
   MANUAL_SOURCE_PLACEMENT,
   MANUAL_LISTENER_ORIENTATION,
   INVALID,
};

struct ENV
{
   HWND    env;
   RECT    envrect;

   S32     envwidth;
   S32     soundadjx;
   S32     soundadjy;
   HBRUSH  old;
   HBRUSH  listbrush;
   HBRUSH  cratebrush;
   HBRUSH  wallbrush;
   HBRUSH  soundbrush[MAX_3D_SOUNDS];
   HBITMAP cbm;
   HDC     envdc;

   BOOL    obstruction_crates;
   BOOL    occlusion_walls;
   BOOL    looped;

   BOOL    use_phaser;
   BOOL    auto_3D_atten;
   BOOL    use_reverb_for_2D;
   BOOL    use_cone;

   SNDMOVE movement_type;
   F32     requested_falloff;
   S32     requested_room_type;

   F32     listener_bearing_x;
   F32     listener_bearing_y;
   F32     listener_bearing_z;

   S32     pitch_slider;
   F32     speed_slider;
   F32     wet_slider;
   F32     dry_slider;
};

//
// Driver-related configuration state
//

struct CFG
{
   HDIGDRIVER DIG3D;
   HPROVIDER  phaser_filter;
   
   BOOL reverb_supported;
};

//
// Control state for a given 3D sound
//

struct SND3D
{
   //
   // File image copied into sample slot when slot is played
   //

   void *loaded_sample_address;
   S32   loaded_sample_file_size;
   C8    loaded_sample_filename[MAX_PATH];

   //
   // HSAMPLE assigned to slot, if any
   //

   HSAMPLE S;

   //
   // Maintain the sound state separately from the HSAMPLE so the HSAMPLE can be
   // reallocated on the fly
   //

   BOOL data_registered;
   BOOL triggered;
   BOOL playing;

   S32  base_sample_rate;

   S32  loop_count;
   S32  play_cursor;

   F32 x;
   F32 y;
   F32 z;
   F32 face_x;
   F32 face_y;
   F32 face_z;

   F32 inner_cone_angle;
   F32 outer_cone_angle;
   F32 outer_cone_vol;

   F32     offset;
   F32     cone_axis;
   SNDMOVE movement_type;

   S32     playback_rate;

   F32     occlusion;
   F32     obstruction;
   F32     wet;
   F32     dry;
   BOOL    auto_3D_wet_atten;

   BOOL    use_phaser_filter;
   BOOL    use_cone;
   BOOL    looped;

   F32     dx_dt;
   F32     dy_dt;
   F32     dz_dt;

   //
   // Housekeeping
   //

   S32 last_time_ms;
};

//
// Control equates and miscellaneous definitions
//

#define cboxConfig   100
#define btnPlay3D    101
#define btnPlay2D    116
#define sldSpeed     102
#define sldPitch     129
#define txtPitch     203
#define radAroundx   103
#define radBackForth 104
#define radManualRot 105
#define radManualLst 131
#define radNone      106
#define radBath      107
#define radMountains 108
#define radHallway   109
#define btnStop      110
#define btnAbout     111
#define btnClose     112
#define btnLoad      113
#define btnProviderA 114
#define btnSoundA    115
#define radFalloff   119
#define radF1        120
#define radF2        121
#define radF3        122
#define radF4        123
#define chkPhaser    124
#define chkCone      125
#define chkOcclude   126
#define chkObstruct  127
#define chkLoop      128

#define txtDry       212
#define txtWet       213
#define sldDry       214
#define sldWet       215
#define chk3DAtten   216
#define chk2DReverb  217

#define txtSpeed     202
#define txtRevEnv    206
#define txtCurrent   130
#define txtRevMix    207

//
// Colors for sound brushes
//

U32 sound_colors[MAX_3D_SOUNDS] = { RGB(0x6a,0x52,0x00),
                                    RGB(0xf0,0x00,0x00), 
                                    RGB(0xfe,0xad,0x0c),
                                    RGB(0xff,0xf7,0x21) };

//
// Sound/environment globals
// 

SND3D sounds[MAX_3D_SOUNDS];

ENV *E = NULL;
CFG *current_configuration = NULL;

//
// Hard-wired 2D provider
//

HDIGDRIVER DIG2D = NULL;
HSAMPLE    opened_2D_sample = 0;

//
// Currently-loaded sound file data
//

void       *loaded_sample_address = NULL;
S32         loaded_sample_file_size = 0;
C8          loaded_sample_filename[MAX_PATH];
void       *sample_2D_address = NULL;
S32         sample_2D_file_size = 0;
C8          sample_2D_filename[MAX_PATH] = "..\\media\\ogres.mp3"; 

//
// Miscellaneous globals
//

static char szAppName[] = APPNAME;

HWND        hwnd;
HWND        combo;

static C8   txtbuf[65536];

//############################################################################
//#                                                                          #
//# Load new sample file                                                     #
//#                                                                          #
//############################################################################

S32 FILE_load (C8    *filename, //)
               void **sample_address,
               S32   *sample_file_size)
{
   void *ptr = AIL_file_read(filename,0);

   if (ptr==0)
      {
      MessageBox(0,"Couldn't load the digital file.","Error",MB_OK);
      return 0;
      }

   if (*sample_address != NULL)
      {
      AIL_mem_free_lock(*sample_address);
      }

   *sample_address   = ptr;
   *sample_file_size = AIL_file_size(filename);

   return 1;
}

//############################################################################
//#                                                                          #
//#  Enable or disable a dialog window control                               #
//#                                                                          #
//############################################################################

static void UI_enable_control(WORD control, BOOL state)
{
   EnableWindow(GetDlgItem(hwnd,control), state);
}

//############################################################################
//#                                                                          #
//#  Check or uncheck a dialog window control                                #
//#                                                                          #
//############################################################################

static void UI_check_control(WORD control, BOOL state)
{
   Button_SetCheck(GetDlgItem(hwnd,control), state );
}

//############################################################################
//#                                                                          #
//#  Enable controls common to all configurations                            #
//#                                                                          #
//############################################################################

static void UI_enable_base_controls(void)
{
   UI_enable_control(btnPlay3D,    TRUE);
   UI_enable_control(btnPlay2D,    TRUE);
   UI_enable_control(sldSpeed,     TRUE);
   UI_enable_control(sldPitch,     TRUE);
   UI_enable_control(txtPitch,     TRUE);
   UI_enable_control(radAroundx,   TRUE);
   UI_enable_control(radBackForth, TRUE);
   UI_enable_control(radManualRot, TRUE);
   UI_enable_control(radManualLst, TRUE);
   UI_enable_control(btnStop,      TRUE);
   UI_enable_control(btnAbout,     TRUE);
   UI_enable_control(btnClose,     TRUE);
   UI_enable_control(btnLoad,      TRUE);
   UI_enable_control(btnProviderA, TRUE);
   UI_enable_control(btnSoundA,    TRUE);
   UI_enable_control(radFalloff,   TRUE);
   UI_enable_control(radF1,        TRUE);
   UI_enable_control(radF2,        TRUE);
   UI_enable_control(radF3,        TRUE);
   UI_enable_control(radF4,        TRUE);
   UI_enable_control(chkPhaser,    TRUE);
   UI_enable_control(chkCone,      TRUE);
   UI_enable_control(chkOcclude,   TRUE);
   UI_enable_control(chkObstruct,  TRUE);
   UI_enable_control(chkLoop,      TRUE);
   UI_enable_control(txtSpeed,     TRUE);
   UI_enable_control(txtCurrent,   TRUE);
}

//############################################################################
//#                                                                          #
//#  Disable all controls except configuration selector                      #
//#                                                                          #
//############################################################################

static void UI_disable_all_controls(void)
{
   UI_enable_control(btnPlay3D,    FALSE);
   UI_enable_control(btnPlay2D,    FALSE);
   UI_enable_control(sldSpeed,     FALSE);
   UI_enable_control(sldPitch,     FALSE);
   UI_enable_control(txtPitch,     FALSE);
   UI_enable_control(radAroundx,   FALSE);
   UI_enable_control(radBackForth, FALSE);
   UI_enable_control(radManualRot, FALSE);
   UI_enable_control(radManualLst, FALSE);
   UI_enable_control(radNone,      FALSE);
   UI_enable_control(radBath,      FALSE);
   UI_enable_control(radMountains, FALSE);
   UI_enable_control(radHallway,   FALSE);
   UI_enable_control(btnStop,      FALSE);
   UI_enable_control(btnAbout,     FALSE);
   UI_enable_control(btnClose,     FALSE);
   UI_enable_control(btnLoad,      FALSE);
   UI_enable_control(btnProviderA, FALSE);
   UI_enable_control(btnSoundA,    FALSE);
   UI_enable_control(radFalloff,   FALSE);
   UI_enable_control(radF1,        FALSE);
   UI_enable_control(radF2,        FALSE);
   UI_enable_control(radF3,        FALSE);
   UI_enable_control(radF4,        FALSE);
   UI_enable_control(chkPhaser,    FALSE);
   UI_enable_control(chkCone,      FALSE);
   UI_enable_control(chkOcclude,   FALSE);
   UI_enable_control(chkObstruct,  FALSE);
   UI_enable_control(chkLoop,      FALSE);
   UI_enable_control(txtDry,       FALSE);
   UI_enable_control(txtWet,       FALSE);
   UI_enable_control(sldDry,       FALSE);
   UI_enable_control(sldWet,       FALSE);
   UI_enable_control(chk3DAtten,   FALSE);
   UI_enable_control(chk2DReverb,  FALSE);
   UI_enable_control(txtSpeed,     FALSE);
   UI_enable_control(txtRevEnv,    FALSE);
   UI_enable_control(txtCurrent,   FALSE);
   UI_enable_control(txtRevMix,    FALSE);
}

//############################################################################
//#                                                                          #
//# Set control startup defaults                                             #
//#                                                                          #
//############################################################################

static void UI_set_control_defaults()
{
   HWND slider = GetDlgItem(hwnd,sldSpeed);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   slider = GetDlgItem(hwnd,sldSpeed);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   slider = GetDlgItem(hwnd,sldWet);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 128 );

   slider = GetDlgItem(hwnd,sldDry);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 128 );

   slider = GetDlgItem(hwnd,sldPitch);
   SendMessage(slider, TBM_SETRANGE, TRUE, MAKELONG( 0, 128 ) );
   SendMessage(slider, TBM_SETPOS, TRUE, 64 );

   UI_check_control(radF1,0);
   UI_check_control(radF2,0);
   UI_check_control(radF3,1);
   UI_check_control(radF4,0);

   UI_check_control(radNone,1);
   UI_check_control(radMountains,0);
   UI_check_control(radBath,0);
   UI_check_control(radHallway,0);

   UI_check_control(radAroundx,1);
   UI_check_control(radNone,1);

   UI_check_control(chkLoop, 1);
   UI_check_control(chk3DAtten, 1);
   UI_check_control(chk2DReverb, 1);
}

//############################################################################
//#                                                                          #
//#  Read a slider's position                                                #
//#                                                                          #
//############################################################################

static S32 UI_get_slider_value(S32 slider_ID)
{
   HWND slider = GetDlgItem(hwnd,slider_ID);

   return (S32) SendMessage(slider,TBM_GETPOS,0,0);
}

//############################################################################
//#                                                                          #
//# Create environment for display/control of demo objects                   #
//#                                                                          #
//############################################################################

ENV *ENV_create(void)
{
   ENV *E = (ENV *) AIL_mem_alloc_lock(sizeof(ENV));

   if (E == NULL)
      {
      return NULL;
      }

   memset(E,
          0,
          sizeof(ENV));

   E->env = GetDlgItem(hwnd, txtCurrent);
   GetWindowRect(E->env, &E->envrect);

   E->envrect.right  -= (E->envrect.left + 12);
   E->envrect.bottom -= (E->envrect.top  + 10);
   E->envrect.left    = 12;
   E->envrect.top     = 20;

   E->envwidth  = E->envrect.right - E->envrect.left - (SOUNDSIZE * 7);
   E->soundadjx = 12 + (E->envwidth / 2) + (SOUNDSIZE * 3);
   E->soundadjy = 20 + ((E->envrect.bottom - E->envrect.top - (SOUNDSIZE * 4)) / 2) + ((SOUNDSIZE * 3) / 2);

   HDC dc = GetDC(E->env);

   E->envdc = CreateCompatibleDC(dc);

   E->cbm = CreateCompatibleBitmap(dc,               
                                   E->envrect.right, 
                                   E->envrect.bottom);

   SelectObject(E->envdc, E->cbm);

   ReleaseDC(E->env, dc);

   E->listbrush  = CreateSolidBrush(RGB(255,128,128));
   E->cratebrush = CreateSolidBrush(RGB(208,208,0));
   E->wallbrush  = CreateSolidBrush(RGB(228,0,64));

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      E->soundbrush[i] = CreateSolidBrush(sound_colors[i]);
      }

   E->movement_type = AUTO_ROTATE;

   E->listener_bearing_x = 0.0F;
   E->listener_bearing_y = 0.0F;
   E->listener_bearing_z = 1.0F;

   return E;
}

//############################################################################
//#                                                                          #
//# Destroy renderable environment                                           #
//#                                                                          #
//############################################################################

void ENV_destroy(ENV *E)
{
   if (E->envdc != NULL)
      {
      DeleteDC(E->envdc);
      E->envdc = NULL;   
      }

   if (E->cbm != NULL)
      {
      DeleteObject(E->cbm);
      E->cbm = NULL;   
      }

   if (E->listbrush != NULL)
      {
      DeleteObject(E->listbrush);
      E->listbrush = NULL;
      }

   if (E->cratebrush != NULL)
      {
      DeleteObject(E->cratebrush);
      E->cratebrush = NULL;
      }

   if (E->wallbrush != NULL)
      {
      DeleteObject(E->wallbrush);
      E->wallbrush = NULL;
      }

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      if (E->soundbrush[i] != NULL)
         {
         DeleteObject(E->soundbrush[i]);
         E->soundbrush[i] = NULL;
         }
      }
}

//############################################################################
//#                                                                          #
//# Begin rendering an environment                                           #
//#                                                                          #
//############################################################################

void ENV_begin_frame(ENV *E)
{
   //
   // Read UI controls for this frame
   //

   E->dry_slider   = ((F32) UI_get_slider_value(sldDry))   / 128.0F;  
   E->wet_slider   = ((F32) UI_get_slider_value(sldWet))   / 128.0F;
   E->speed_slider = ((F32) UI_get_slider_value(sldSpeed)) / 128.0F;  
   E->pitch_slider =        UI_get_slider_value(sldPitch);

   E->obstruction_crates = (SendMessage(GetDlgItem(hwnd,chkObstruct), BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;
   E->occlusion_walls    = (SendMessage(GetDlgItem(hwnd,chkOcclude),  BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;
   E->looped             = (SendMessage(GetDlgItem(hwnd,chkLoop),     BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;   
   E->use_phaser         = (SendMessage(GetDlgItem(hwnd,chkPhaser),   BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;  
   E->use_cone           = (SendMessage(GetDlgItem(hwnd,chkCone),     BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;   
   E->auto_3D_atten      = (SendMessage(GetDlgItem(hwnd,chk3DAtten),  BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;  
   E->use_reverb_for_2D  = (SendMessage(GetDlgItem(hwnd,chk2DReverb), BM_GETCHECK, 0, 0) == 0) ? FALSE : TRUE;  

   if (SendMessage(GetDlgItem(hwnd,radAroundx), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = AUTO_ROTATE;
      }
   else if (SendMessage(GetDlgItem(hwnd,radBackForth), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = AUTO_BACK_AND_FORTH;
      }
   else if (SendMessage(GetDlgItem(hwnd,radManualRot), BM_GETCHECK,0,0) != 0)
      {
      E->movement_type = MANUAL_SOURCE_PLACEMENT;
      }
   else
      {
      E->movement_type = MANUAL_LISTENER_ORIENTATION;
      }

   if (SendMessage(GetDlgItem(hwnd,radF1), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 1.0F;
      }
   else if (SendMessage(GetDlgItem(hwnd,radF2), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 2.0F;
      }
   else if (SendMessage(GetDlgItem(hwnd,radF3), BM_GETCHECK,0,0) != 0)
      {
      E->requested_falloff = 3.0F;
      }
   else
      {
      E->requested_falloff = 4.0F;
      }

   if (SendMessage(GetDlgItem(hwnd,radBath), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_BATHROOM;
      }
   else if (SendMessage(GetDlgItem(hwnd,radMountains), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_MOUNTAINS;
      }
   else if (SendMessage(GetDlgItem(hwnd,radHallway), BM_GETCHECK,0,0) != 0)
      {
      E->requested_room_type = ENVIRONMENT_HALLWAY;
      }
   else
      {
      E->requested_room_type = ENVIRONMENT_GENERIC;
      }

   //
   // Maintain 2D sample state directly, since it isn't associated with any specific driver configuration
   //

   if (opened_2D_sample != NULL)
      {
      if (E->looped)
         {
         AIL_set_sample_loop_count(opened_2D_sample, 0);
         }
      else
         {
         AIL_set_sample_loop_count(opened_2D_sample, 1);
         }
      }

   //
   // Allow user to change the listener's facing direction
   //

   if (E->movement_type == MANUAL_LISTENER_ORIENTATION)
      {
      //
      // Check for left-click in environment window, and get its bearing to the
      // listener
      //

      if ((GetForegroundWindow() == hwnd) && 
          (GetKeyState(VK_LBUTTON) & 0x8000))
         {
         POINT screen_ul;
         RECT rect;
         GetClientRect(hwnd, &rect);
         screen_ul.x = rect.left; 
         screen_ul.y = rect.top; 
         ClientToScreen(hwnd, &screen_ul); 

         POINT screen_cursor;
         GetCursorPos(&screen_cursor);  

         F32 x = F32((screen_cursor.x - screen_ul.x) - CLIENT_LISTENER_X);
         F32 y = F32((screen_cursor.y - screen_ul.y) - CLIENT_LISTENER_Y);

         if ((x >= -CLIENT_LISTENER_RANGE_X) && (x < CLIENT_LISTENER_RANGE_X) &&
             (y >= -CLIENT_LISTENER_RANGE_Y) && (y < CLIENT_LISTENER_RANGE_Y))
            {
            //
            // We have a valid bearing -- use it to generate a facing vector
            // (which does not have to be normalized before passing to MSS)
            //
            // Screen X/Y in left-handed coords = +X/-Z
            //

            E->listener_bearing_x =  x;
            E->listener_bearing_y =  0.0F;
            E->listener_bearing_z = -y;
            }
         }
      }

   //
   // Set the listener's facing direction
   //
   // The listener's up-vector in this example is always = +Y
   //

   if (current_configuration != NULL)
      {
      AIL_set_listener_3D_orientation(current_configuration->DIG3D,
                                      E->listener_bearing_x,
                                      E->listener_bearing_y,
                                      E->listener_bearing_z,
                                      0.0F,
                                      1.0F,
                                      0.0F);
      }

   //
   // Set up to begin rendering
   // 
   
   E->old = (HBRUSH) SelectObject(E->envdc, 
                                  E->listbrush);

   FillRect(E->envdc,
           &E->envrect,
           (HBRUSH) (COLOR_WINDOW+1));
}

//############################################################################
//#                                                                          #
//# End environment render                                                   #
//#                                                                          #
//############################################################################

void ENV_end_frame(ENV *E)
{
   SelectObject(E->envdc, 
                E->old);

   HDC dc = GetDC(E->env);

   if (dc)
      {
      //
      // Report total 2D/3D CPU usage
      //

      S32 p = 0;

      if (DIG2D != NULL)
         {
         p += AIL_digital_CPU_percent(DIG2D);
         }

      CFG *C = current_configuration;

      if ((C != NULL) && (C->DIG3D != NULL))
         {
         p += AIL_digital_CPU_percent(C->DIG3D);
         }

      static C8 profile[32];

      wsprintf(profile, "CPU: %d%%", p);

      SelectObject(dc, GetStockObject(ANSI_VAR_FONT));

      TextOut(E->envdc, 
              E->envrect.left   + 4, 
              E->envrect.bottom - 16,
              profile, 
        (S32) strlen(profile));

      //
      // Copy environment to window
      //

      BitBlt(dc,
             E->envrect.left,
             E->envrect.top,
             E->envrect.right  - E->envrect.left,
             E->envrect.bottom - E->envrect.top,
             E->envdc,
             E->envrect.left,
             E->envrect.top,
             SRCCOPY);

      ReleaseDC(E->env, dc);
      }
}

//############################################################################
//#                                                                          #
//# Render current environment                                               #
//#                                                                          #
//############################################################################

void ENV_render_environment(ENV *E)
{
   //
   // Draw listener's "nose" at its current orientation
   //

   F32 t = atan2(E->listener_bearing_x,
                 E->listener_bearing_z);

   while (t > CIRCLE) t -= CIRCLE;
   while (t < 0.0F)   t += CIRCLE;

   POINT n[3],nose[3];

   S32 L = LISTENERSIZE-1;

   n[0].x = - L / 3;
   n[0].y = - L;
   n[1].x = 0;
   n[1].y = - ((L * 5) / 3);
   n[2].x = + L / 3;
   n[2].y = - L;

   for (S32 i=0; i < 3; i++)
      {
      nose[i].x = S32(n[i].x * cos(t)) - S32(n[i].y * sin(t)) + E->soundadjx;
      nose[i].y = S32(n[i].y * cos(t)) + S32(n[i].x * sin(t)) + E->soundadjy; 
      }

   Polygon(E->envdc,
           nose,
           3);

   Ellipse(E->envdc,
           E->soundadjx - LISTENERSIZE,
           E->soundadjy - LISTENERSIZE,
           E->soundadjx + LISTENERSIZE,
           E->soundadjy + LISTENERSIZE);

   //
   // Draw obstacles
   //

   RECT r;

   if (E->obstruction_crates)
      {
      r.left   = E->soundadjx -  CRATESIZE;
      r.right  = E->soundadjx +  CRATESIZE;
      r.top    = E->soundadjy - (CRATESIZE * 7);
      r.bottom = E->soundadjy - (CRATESIZE * 5);

      FillRect(E->envdc,
              &r,
               E->cratebrush);

      r.top    = E->soundadjy + (CRATESIZE * 4);
      r.bottom = E->soundadjy + (CRATESIZE * 6);

      FillRect(E->envdc,
              &r,
               E->cratebrush);
      }

   if (E->occlusion_walls)
      {
      if (E->movement_type == AUTO_BACK_AND_FORTH)
         {
         r.left   = E->soundadjx - WALLHEIGHT;
         r.right  = E->soundadjx + WALLHEIGHT;
         r.top    = E->soundadjy - (CRATESIZE * 9);
         r.bottom = E->soundadjy - (CRATESIZE * 9) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);

         r.top    = E->soundadjy + (CRATESIZE * 7);
         r.bottom = E->soundadjy + (CRATESIZE * 7) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         }
      else
         {
         r.left   = E->soundadjx - (CRATESIZE * 6) - WALLWIDTH;
         r.right  = E->soundadjx - (CRATESIZE * 6);
         r.top    = E->soundadjy - WALLHEIGHT;
         r.bottom = E->soundadjy + WALLHEIGHT;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         
         r.left   = E->soundadjx + (CRATESIZE * 6);
         r.right  = E->soundadjx + (CRATESIZE * 6) + WALLWIDTH;

         FillRect(E->envdc,
                 &r,
                  E->wallbrush);
         }
      }
}

//############################################################################
//#                                                                          #
//# Reset SND3D position and offset                                          #
//#                                                                          #
//############################################################################

void SND3D_reset_movement(SND3D *S)
{
   if (S->movement_type == AUTO_ROTATE)
      {
      S->x             =  25.0F;
      S->y             =   0.0F;
      S->z             =   0.0F;
      S->offset        =   0.0F;
      S->cone_axis     =   0.0F;
      }
   else if (S->movement_type == AUTO_BACK_AND_FORTH)
      {
      S->x             =   0.0F;
      S->y             =   0.0F;
      S->z             =  75.0F;
      S->offset        =  -1.0F;
      S->cone_axis     =   0.0F;
      }
}

//############################################################################
//#                                                                          #
//# Apply timestep to SND3D object                                           #
//#                                                                          #
//############################################################################
 
void SND3D_update(SND3D *S)
{
   //
   // Get elapsed time since last update
   //

   S32 cur_time_ms = AIL_ms_count();

   S32 dT_ms = cur_time_ms - S->last_time_ms;

   S->last_time_ms = cur_time_ms;

   //
   // Get relative velocity from slider and use it to scale the timestep
   //

   F32 i = (E->speed_slider * (FASTSPEED - SLOWSPEED)) + SLOWSPEED;
   i = i * (F32) dT_ms;

   //
   // Read other UI controls that affect the sample
   //

   S->use_phaser_filter = E->use_phaser;   
   S->auto_3D_wet_atten = E->auto_3D_atten;

   //
   // Handle change in movement type
   //

   if (S->movement_type != E->movement_type)
      {
      S->movement_type = E->movement_type;

      SND3D_reset_movement(S);
      }

   //
   // Update position, orientation, occlusion, and obstruction
   //
   // To calculate obstruction and occlusion in a 3d game, you would
   //   usually ray cast a spread of rays out from the sound source
   //   and see how many hit the player's current sector.
   //
   // Here, we simply do it closed form, since the crates and walls
   //   aren't moving.
   //

   S->obstruction = 0.0F;
   S->occlusion   = 0.0F;

   F32 prev_x = S->x;
   F32 prev_y = S->y;
   F32 prev_z = S->z;

   if (E->movement_type == AUTO_BACK_AND_FORTH)
      {
      S->z += (S->offset * i);

      if ( S->z > STRAIGHTDIST )
         S->offset = -1.0F;
      else if ( S->z < -(STRAIGHTDIST))
         S->offset = 1.0F;

      if (E->obstruction_crates)
         {
         if ( ( S->z >= 156.5F ) || ( S->z <= -136.5F ) )
            {
            S->obstruction = 0.50F;
            }
         else
            {
            if ( ( S->z >= 115.5F ) && ( S->z <= 156.5F ) )
               S->obstruction = 0.50F * ( ( S->z - 115.5F ) / 41.0F );
            else if ( ( S->z >= -135.5F ) && ( S->z <= -95.5F ) )
               S->obstruction = 0.50F * ( - ( S->z + 95.5F ) / 41.0F );
            }
         }

      if ( E->occlusion_walls )
         {
         if ( ( S->z >= 198.5F ) || ( S->z <= -175.5F ) )
            {
            S->occlusion = 1.0F;
            }
         else
            {
            if ( ( S->z>= 173.0F ) && ( S->z <= 198.5F ) )
               S->occlusion = ( S->z - 173.0F ) / 25.0F;
            else if ( ( S->z >= -175.0F ) && ( S->z <= -150.0F ) )
               S->occlusion = ( -( S->z + 150.0F ) / 25.0F );
            }
         }
      }
   else 
      {
      if (E->movement_type == AUTO_ROTATE)
         {
         S->offset += (i / CIRCLERADIUS);
         }
      else if (E->movement_type == MANUAL_SOURCE_PLACEMENT)
         {
         //
         // Check for left-click in environment window, and get its bearing to the
         // listener
         //

         if ((GetForegroundWindow() == hwnd) && 
             (GetKeyState(VK_LBUTTON) & 0x8000))
            {
            POINT screen_ul;
            RECT rect;
            GetClientRect(hwnd, &rect);
            screen_ul.x = rect.left; 
            screen_ul.y = rect.top; 
            ClientToScreen(hwnd, &screen_ul); 

            POINT screen_cursor;
            GetCursorPos(&screen_cursor);  

            F32 x = F32((screen_cursor.x - screen_ul.x) - CLIENT_LISTENER_X);
            F32 y = F32((screen_cursor.y - screen_ul.y) - CLIENT_LISTENER_Y);

            if ((x >= -CLIENT_LISTENER_RANGE_X) && (x < CLIENT_LISTENER_RANGE_X) &&
                (y >= -CLIENT_LISTENER_RANGE_Y) && (y < CLIENT_LISTENER_RANGE_Y))
               {
               S->offset = atan2(-y,x);
               }
            }
         }

      while (S->offset > CIRCLE) S->offset -= CIRCLE;
      while (S->offset < 0.0F)   S->offset += CIRCLE;

      S->x = (F32) (CIRCLERADIUS * cos( S->offset ));
      S->z = (F32) (CIRCLERADIUS * sin( S->offset ));

      if (E->obstruction_crates)
         {
         if ( ( ( S->offset >= 1.43F ) && ( S->offset <= 1.72F ) ) || 
              ( ( S->offset >= 4.51F ) && ( S->offset <= 4.90F ) ) )
            {
            S->obstruction = 0.90F;
            }
         else
            {
            if ( ( S->offset >= 1.38F ) && ( S->offset <= 1.43F ) )
               S->obstruction = 0.90F * ( ( S->offset - 1.38F ) / 0.05F );
            else if ( ( S->offset >= 1.72F ) && ( S->offset <= 1.77F ) )
               S->obstruction = 0.90F * ( 1.0F - ( ( S->offset - 1.72F ) / 0.05F ) );
            else if ( ( S->offset >= 4.46F ) && ( S->offset <= 4.51F ) )
               S->obstruction = 0.90F * ( ( S->offset - 4.46F ) / 0.05F );
            else if ( ( S->offset >= 4.90F ) && ( S->offset <= 4.95F ) )
               S->obstruction = 0.90F * ( 1.0F - ( ( S->offset - 4.90F ) / 0.05F ) );
            }
         }

      if (E->occlusion_walls)
         {
         if ( ( ( S->offset >= 2.76F ) && ( S->offset <= 3.51F ) ) || 
                ( S->offset <= 0.40F ) || ( S->offset >= 5.89F ) )
            {
            S->occlusion = 1.0F;
            }
         else
            {
            if ( ( S->offset >= 0.40F ) && ( S->offset <= 0.81F ) )
               S->occlusion = 1.0F - ( ( S->offset - 0.40F ) / 0.42F );
            else if ( ( S->offset >= 2.34F ) && ( S->offset <= 2.76F ) )
               S->occlusion = ( S->offset - 2.34F ) / 0.42F;
            else if ( ( S->offset >= 3.51F ) && ( S->offset <= 3.93F ) )
               S->occlusion = 1.0F - ( ( S->offset - 3.51F ) / 0.42F );
            else if ( ( S->offset >= 5.47F ) && ( S->offset <= 5.89F ) )
               S->occlusion = ( S->offset - 5.47F ) / 0.42F;
            }
         }
      } 

   //
   // Calculate velocity (for Doppler shift)
   //
   // We'll set this to zero if the source is moving in a circle -- when you are moving in a
   // circle, there is angular velocity, but no velocity away or towards the listener
   //

   if (E->movement_type == AUTO_ROTATE)
      {
      S->dx_dt = 0.0F;
      S->dy_dt = 0.0F;
      S->dz_dt = 0.0F;
      }
   else
      {
      S->dx_dt = (S->x - prev_x) / dT_ms;
      S->dy_dt = (S->y - prev_y) / dT_ms;
      S->dz_dt = (S->z - prev_z) / dT_ms;
      }

   //
   // Update global wet/dry levels and playback rate scale
   //

   S->wet = E->wet_slider;
   S->dry = E->dry_slider;

   S->playback_rate = E->pitch_slider;

   //
   // Update orientation
   //
   // If a directional cone is in use, rotate it
   // If not, the sound source faces the listener at the origin
   //

   S->face_y = -S->y;

   S->use_cone = E->use_cone;

   if (S->use_cone)
      {
      S->cone_axis += (CIRCLE / 45.0F);

      if (S->cone_axis > CIRCLE)
         {
         S->cone_axis -= CIRCLE;
         }

      S->face_x = (F32) (CIRCLERADIUS * cos(S->cone_axis));
      S->face_z = (F32) (CIRCLERADIUS * sin(S->cone_axis));

      S->inner_cone_angle = 90.0F;
      S->outer_cone_angle = 120.0F;
      S->outer_cone_vol   = 0.05F;
      }
   else
      {
      S->face_x = -S->x;
      S->face_z = -S->z;

      S->inner_cone_angle = 360.0F;
      S->outer_cone_angle = 360.0F;
      S->outer_cone_vol   = 1.0F;
      }

   //
   // Maintain looping status
   //

   if (E->looped)
      {
      S->loop_count = 0;
      }
   else
      {
      S->loop_count = 1;
      }
}

//############################################################################
//#                                                                          #
//# Synchronize MSS sample with SND3D state                                  #
//#                                                                          #
//############################################################################

BOOL SND3D_update_HSAMPLE(SND3D *S)
{
   CFG *C = current_configuration;

   if (C == NULL)
      {
      //
      // No driver loaded
      //

      return FALSE;
      }

   //
   // If slot has no HSAMPLE yet, allocate it
   //

   if (S->S == NULL)
      {
      S->S = AIL_allocate_sample_handle(current_configuration->DIG3D);

      if (S->S == NULL)
         {
         MessageBox(hwnd,"Could not allocate HSAMPLE","Error",MB_OK);
         return FALSE;
         }
      }

   //
   // Set HSAMPLE's address, type, and rate based on file image 
   // associated with SND3D, if any
   //

   if ((S->loaded_sample_address != NULL) && (!S->data_registered))
      {
      S->data_registered = TRUE;

      if (!AIL_set_named_sample_file(S->S,
                                     S->loaded_sample_filename,
                                     S->loaded_sample_address,
                                     S->loaded_sample_file_size,
                                     0))
         {
         MessageBox(hwnd,AIL_last_error(),"Error loading sample file",MB_OK);
         return FALSE;
         }

      //
      // Record sample's original playback rate, so it can be scaled with the pitch control
      //

      S->base_sample_rate = AIL_sample_playback_rate(S->S);
      }

   //
   // Setting 3D position on the HSAMPLE causes MSS to apply distance/cone falloff to the
   // sample.  None of the other 3D parameters will have any effect if the sample's 3D position 
   // is not set!
   //

   AIL_set_sample_3D_position(S->S,
                              S->x,
                              S->y,
                              S->z);

   AIL_set_sample_3D_orientation(S->S,
                                 S->face_x,
                                 S->face_y,
                                 S->face_z,
                                 0.0F, 
                                 1.0F, 
                                 0.0F);

   AIL_set_sample_3D_velocity_vector(S->S,
                                     S->dx_dt,
                                     S->dy_dt,
                                     S->dz_dt);

   AIL_set_sample_3D_cone(S->S,
                          S->inner_cone_angle,
                          S->outer_cone_angle,
                          S->outer_cone_vol);

   AIL_set_sample_3D_distances(S->S,
                               STRAIGHTDIST + 10,
                               40,
                               S->auto_3D_wet_atten);          

   //
   // Set non-3D-specific parameters
   //

   AIL_set_sample_loop_count   (S->S, S->loop_count);
   AIL_set_sample_occlusion    (S->S, S->occlusion);
   AIL_set_sample_obstruction  (S->S, S->obstruction);
   AIL_set_sample_reverb_levels(S->S, S->dry, S->wet);
   
   //
   // Adjust playback rate up or down with slider
   // 

   if (S->data_registered)
      {
      S32 half_rate = S->base_sample_rate / 2;

      S32 min_rate = S->base_sample_rate - half_rate;
      S32 max_rate = S->base_sample_rate + half_rate;

      AIL_set_sample_playback_rate(S->S,
                                   min_rate + (((max_rate - min_rate) * S->playback_rate) / 128));
      }

   //
   // Install or uninstall example pipeline filter as requested via UI
   //
   // The phaser filter can cause clipping when used with full-scale sounds
   // (such as samples passed to output filters).  So we'll back its wet level 
   // down to 50% for better quality across all providers
   //
 
   if (S->use_phaser_filter)
      {
      if (AIL_sample_processor(S->S, 
                               SP_FILTER_0) == NULL)
         {
         AIL_set_sample_processor(S->S, 
                                  SP_FILTER_0, 
                                  C->phaser_filter);
         F32 mix = 0.5F; 
         
         AIL_sample_stage_property( S->S, 
                                    SP_FILTER_0,
                                    "Phaser Mix",
                                    0, &mix, 0 );
         }
      }
   else
      {
      if (AIL_sample_processor(S->S, 
                               SP_FILTER_0) != NULL)
         {
         AIL_set_sample_processor(S->S, 
                                  SP_FILTER_0, NULL);
         }
      }

   //
   // Trigger sample, and maintain our view of its playback state
   //

   if (S->triggered)
      {
      S->triggered = FALSE;

      AIL_set_sample_position(S->S, S->play_cursor);

      AIL_resume_sample(S->S);
      }

   U32 status = AIL_sample_status(S->S);

   S->playing = (status == SMP_PLAYING);

   if (S->playing)
      {
      S->play_cursor = AIL_sample_position(S->S);
      }

   return TRUE;
}

//############################################################################
//#                                                                          #
//# Render SND3D representation to environment display                       #
//#                                                                          #
//############################################################################

void SND3D_render(SND3D *S, //)
                  ENV   *E)
{
   //
   // Render playing samples only
   //

   if (!S->playing)
      {
      return;
      }

   //
   // Calculate screenspace position and length of cone-direction pointer
   //

   F32 length = (F32) sqrt((S->face_x * S->face_x) + (S->face_z * S->face_z));

   if (length <= 0.0001F)
      {
      length = 1.0F;
      }

   F32 f,r;

   if (E->movement_type == AUTO_BACK_AND_FORTH)
      {
      r = STRAIGHTDIST * 2.0F;
      f = 20.0F;
      }                                                                                          
   else
      {
      r = CIRCLERADIUS * 2.0F;
      f = 10.0F;
      }

   S32 x  = (S32) ((  S->x * (F32) E->envwidth)                                / r);
   S32 y  = (S32) (( -S->z * (F32) E->envwidth)                                / r);
   S32 dx = (S32) ((( S->x + (( S->face_x / length) * f)) * (F32) E->envwidth) / r);
   S32 dy = (S32) (((-S->z + ((-S->face_z / length) * f)) * (F32) E->envwidth) / r);

   //
   // Draw sound representation
   //

   MoveToEx(E->envdc,
            E->soundadjx + x,
            E->soundadjy + y,
            0);

   LineTo(E->envdc,
          E->soundadjx + dx,  
          E->soundadjy + dy);

   Ellipse(E->envdc,
           E->soundadjx + x - SOUNDSIZE,
           E->soundadjy + y - SOUNDSIZE,
           E->soundadjx + x + SOUNDSIZE,
           E->soundadjy + y + SOUNDSIZE);
}

//############################################################################
//#                                                                          #
//# Destroy existing configuration                                           #
//#                                                                          #
//############################################################################

void CFG_destroy(CFG *C)
{
   //
   // Disable UI
   //

   UI_disable_all_controls();

   //
   // Free all SND3D HSAMPLEs
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      if (S->S != NULL)
         {
         AIL_release_sample_handle(S->S);
         S->S = NULL;
         }
      }

   //
   // Unload driver
   //

   if (C->DIG3D != NULL)
      {
      AIL_close_digital_driver(C->DIG3D);
      C->DIG3D = NULL;
      }

   AIL_mem_free_lock(C);
}

//############################################################################
//#                                                                          #
//# Create new configuration based on selected combo-box entry               #
//#                                                                          #
//############################################################################

CFG *CFG_create(S32 index)
{
   //
   // Allocate and zero a new CFG object
   //

   CFG *C = (CFG *) AIL_mem_alloc_lock(sizeof(CFG));

   if (C == NULL)
      {
      return NULL;
      }

   memset(C,
          0,
          sizeof(CFG));

   //
   // Load a digital driver
   //

   C->DIG3D = AIL_open_digital_driver(OUTPUT_RATE,
                                      OUTPUT_BITS,
                                      combo_entries[index].channel_spec,
                                      0);

   if (C->DIG3D == NULL)
      {
      MessageBox(0,AIL_last_error(),"Error",MB_OK);

      CFG_destroy(C);
      return NULL;
      }

   //
   // Load a phaser filter instance
   //
   
   if (!AIL_find_filter("Phaser Filter",&C->phaser_filter))
      {    
      MessageBox(0,"Couldn't find phaser filter provider.","Error",MB_OK);

      CFG_destroy(C);
      return NULL;
      }

   //
   // See if we're running under an EAX-compatible provider
   // (i.e., not DS3D hardware or another output filter that doesn't support reverb)
   //
   // The default room type will be -1 on these providers
   //

   C->reverb_supported = (AIL_room_type(C->DIG3D) != -1);

   //
   // Enable/disable the reverb-property controls accordingly
   //

   UI_enable_base_controls();

   UI_enable_control(txtRevEnv,    C->reverb_supported);
   UI_enable_control(radNone,      C->reverb_supported);
   UI_enable_control(radBath,      C->reverb_supported);
   UI_enable_control(radMountains, C->reverb_supported);
   UI_enable_control(radHallway,   C->reverb_supported);
   
   UI_enable_control(txtRevMix,    C->reverb_supported);
   UI_enable_control(txtDry,       C->reverb_supported);
   UI_enable_control(txtWet,       C->reverb_supported);
   UI_enable_control(sldWet,       C->reverb_supported);
   UI_enable_control(sldDry,       C->reverb_supported);
   UI_enable_control(chk3DAtten,   C->reverb_supported);
   UI_enable_control(chk2DReverb,  C->reverb_supported);

   S32 using_output_filter = (AIL_digital_output_filter(C->DIG3D) != NULL); 

   UI_enable_control(radFalloff, !using_output_filter);
   UI_enable_control(radF1, !using_output_filter);
   UI_enable_control(radF2, !using_output_filter);
   UI_enable_control(radF3, !using_output_filter);
   UI_enable_control(radF4, !using_output_filter);

   //
   // Invalidate all samples, and retrigger any that were playing
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      S->data_registered = FALSE;

      if (S->playing)
         {
         S->triggered = TRUE;
         }
      }

   return C;
}

//############################################################################
//#                                                                          #
//#  Stop all playing sounds                                                 #
//#                                                                          #
//############################################################################

void CFG_stop_all_sounds(CFG *C)
{
   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      if (S->S != NULL)
         {
         AIL_end_sample(S->S);
         }
      }
}

//############################################################################
//#                                                                          #
//# Trigger 3D-positioned sound                                              #
//#                                                                          #
//############################################################################

void CFG_play_current_sample(CFG *C)
{
   //
   // Find a stopped sample
   //

   S32 i;

   for (i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      if (!S->playing)
         {
         break;
         }
      }

   if (i == MAX_3D_SOUNDS)
      {
      return;
      }

   SND3D *S = &sounds[i];

   //
   // Make a copy of the currently-loaded sound file image, freeing any old one first
   //

   if (S->loaded_sample_address != NULL)
      {
      AIL_mem_free_lock(S->loaded_sample_address);
      S->loaded_sample_address = NULL;
      }

   S->loaded_sample_address   = AIL_mem_alloc_lock(loaded_sample_file_size);
   S->loaded_sample_file_size = loaded_sample_file_size;

   memcpy(S->loaded_sample_address,
          loaded_sample_address,
          loaded_sample_file_size);

   strcpy(S->loaded_sample_filename,
          loaded_sample_filename);

   //
   // Reposition sample's physical location and play cursor
   //

   SND3D_reset_movement(S);

   S->play_cursor = 0;

   //
   // Arrange for sample's data to be passed to MSS and played
   //

   S->data_registered = FALSE;
   S->triggered       = TRUE;
}

//############################################################################
//#                                                                          #
//# Update current configuration                                             #
//#                                                                          #
//############################################################################

void CFG_update(CFG *C)
{
   //
   // This is called from WM_TIMER.  We don't want it re-entered from within an error dialog's 
   // internal message pump
   //

   static S32 reentry_flag = 0;

   if (reentry_flag)
      {
      return;
      }

   ++reentry_flag;

   //
   // Set new falloff parameter only if it's changed
   // (These functions cause some recalculation overhead inside MSS)
   //

   F32 current_falloff = 0.0F;

   AIL_speaker_configuration(C->DIG3D,  
                             NULL,
                             NULL,
                            &current_falloff,
                             NULL);
   
   if (fabs(current_falloff - E->requested_falloff) > EPSILON)
      {
      AIL_set_speaker_configuration(C->DIG3D,
                                    NULL,
                                    0,
                                    E->requested_falloff);
      }

   //
   // See if a new room type has been requested
   //
   
   if (AIL_room_type(C->DIG3D) != E->requested_room_type)
      {
      AIL_set_room_type(C->DIG3D,
                        E->requested_room_type);
      }

   //
   // Select ENVIRONMENT_GENERIC for 2D provider if no 2D reverb requested
   //

   S32 requested_2D_room = ENVIRONMENT_GENERIC;

   if (E->use_reverb_for_2D)
      {
      requested_2D_room = E->requested_room_type;
      }

   if (AIL_room_type(DIG2D) != requested_2D_room)
      {
      AIL_set_room_type(DIG2D,
                        requested_2D_room);
      }

   AIL_set_sample_reverb_levels(opened_2D_sample, E->dry_slider, E->wet_slider);

   //
   // Update environment based on UI controls
   //

   ENV_begin_frame(E);

   //
   // Render environment (listener, walls, etc.)
   //

   ENV_render_environment(E);

   //
   // Service individual sound instances
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      //
      // Update SND3D based on UI controls
      //

      SND3D_update(S);

      //
      // Bring HSAMPLE up to date with SND3D state
      //

      if (!SND3D_update_HSAMPLE(S))
         {
         CFG_destroy(C);

         --reentry_flag;
         return;
         }

      //
      // Render SND3D state into environment
      //

      SelectObject(E->envdc, 
                   E->soundbrush[i]);

      SND3D_render(S, E);
      }

   //
   // Render result and exit
   //

   ENV_end_frame(E);

   --reentry_flag;
}

//############################################################################
//#                                                                          #
//#  Show the about window                                                   #
//#                                                                          #
//############################################################################

static void DLG_show_about()
{
   char version[8];
   AIL_MSS_version(version,8);
   lstrcpy(txtbuf,"Version ");
   lstrcat(txtbuf,version);
   lstrcat(txtbuf," " MSS_COPYRIGHT "\n\n"
               APPNAME ": Miles Sound System Multispeaker Example program\n\n"
               "For questions or comments, please contact RAD Game Tools at:\n\n"
               "\tRAD Game Tools\n"
               "\t401 Park Place - Suite 103\n"
               "\tKirkland, WA  98033\n"
               "\t425-893-4300\n"
               "\tFAX: 425-609-2463\n\n"
               "\tWeb: www.radgametools.com\n"
               "\tE-mail: sales3@radgametools.com");
   MessageBox(hwnd,txtbuf,"About " APPNAME, MB_OK);
}

static void DLG_show_provider_props(void)
{
   S32 value;
   S32 len=0;

   if (current_configuration == NULL)
      {
      return;
      }

   HDIGDRIVER DIG3D = current_configuration->DIG3D;

   HPROVIDER output_filter = AIL_digital_output_filter(DIG3D);

   HINTENUM next = HINTENUM_FIRST;

   RIB_INTERFACE_ENTRY prop;

   len += wsprintf(txtbuf+len,"Provider-level properties:\n");

   while (AIL_enumerate_filter_properties( output_filter,
                                          &next,
                                          &prop ))
      {
      AIL_filter_property( output_filter,
                           prop.entry_name,
                           &value, 0, 0 );

      len+=wsprintf(txtbuf+len,"   %s = %s\n",
                    prop.entry_name,
                    RIB_type_string(&value, prop.subtype));
      }

   len += wsprintf(txtbuf+len,"Driver-level properties:\n");

   while (AIL_enumerate_output_filter_driver_properties( output_filter,
                                                        &next,
                                                        &prop ))
      {
      AIL_output_filter_driver_property( DIG3D,
                                         prop.entry_name,
                                        &value, 0, 0 );

      len+=wsprintf(txtbuf+len,"   %s = %s\n",
                    prop.entry_name,
                    RIB_type_string(&value, prop.subtype));
      }


   MessageBox(hwnd,txtbuf,"Digital Output Filter Provider Properties",MB_OK|MB_ICONINFORMATION);
}

static void DLG_show_sound_props(void)
{
   S32 len=0;

   if (current_configuration == NULL)
      {
      return;
      }

   HDIGDRIVER DIG3D = current_configuration->DIG3D;

   HSAMPLE opened_3D_sample = sounds[0].S;

   if (opened_3D_sample == NULL)
      {
      return;
      }

   HINTENUM next = HINTENUM_FIRST;

   RIB_INTERFACE_ENTRY prop;

   lstrcpy(txtbuf,"None.");

   //
   // The following two enumeration calls are similar -- an output filter's sample 
   // properties may be enumerated (but not accessed) with only an HPROVIDER handle
   //

   while (AIL_enumerate_output_filter_sample_properties( AIL_digital_output_filter(DIG3D),
                                                        &next,
                                                        &prop ))
   {
      S32 value;

      AIL_sample_stage_property( opened_3D_sample,
                                 SP_OUTPUT,
                                 prop.entry_name,
                                &value, 0, 0 );

      len += wsprintf(txtbuf+len,"   %s = %s\n",
                      prop.entry_name,
                      RIB_type_string(&value, prop.subtype));
   }

   MessageBox(hwnd,txtbuf,"3D Sample Properties",MB_OK|MB_ICONINFORMATION);
}

//############################################################################
//#                                                                          #
//#  File-load dialog                                                        #
//#                                                                          #
//############################################################################

static BOOL DLG_show_load()
{
   OPENFILENAME fn;

   memset(&fn, 0, sizeof(fn));

   fn.lStructSize       = sizeof(fn);
   fn.hwndOwner         = hwnd;
   fn.lpstrFilter       = "Digital sound files (*.wav;*.mp?)\0*.wav;*.mp?\0All files\0*.*\0";
   fn.nFilterIndex      = 1;
   fn.lpstrFile         = loaded_sample_filename;
   fn.nMaxFile          = 126;
   fn.lpstrTitle        = "Select digital sound file...";
   fn.Flags             = OFN_FILEMUSTEXIST |
#ifdef IS_WIN32
                          OFN_EXPLORER | OFN_LONGNAMES |
#endif
                          OFN_NOCHANGEDIR |
                          OFN_PATHMUSTEXIST |
                          OFN_HIDEREADONLY;
   fn.lpstrDefExt       = "wav";

   if (GetOpenFileName(&fn))
      {
      if (FILE_load(loaded_sample_filename, 
                   &loaded_sample_address, 
                   &loaded_sample_file_size))
         {
         return TRUE;
         }
      }

   return FALSE;
}

//############################################################################
//#                                                                          #
//#  Main window procedure                                                   #
//#                                                                          #
//############################################################################

BOOL app_active   = FALSE;
BOOL dig_released = FALSE;

LRESULT AILEXPORT WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
      {
      case WM_SETFOCUS:    // deal with the focus in this weird dialog-window
         {
         HWND h = GetWindow(hwnd, GW_CHILD);

         while (h) 
           {
           if ((GetWindowLong(h,GWL_STYLE) & 0x2f) == BS_DEFPUSHBUTTON) 
              {
              SetFocus(h);
              goto found;
              }

           h = GetNextWindow(h, GW_HWNDNEXT);
           }

         SetFocus(GetWindow(hwnd, GW_CHILD));
found:
         break;
         }

      case WM_HSCROLL:
         return 0;

      case WM_COMMAND:

         switch (LOWORD(wParam))
            {
            case cboxConfig:
               {
               if (HIWORD(wParam) == CBN_SELENDOK)
                  {
                  S32 selected = ComboBox_GetCurSel(combo) - 1;

                  if (selected >= 0)
                     {
                     if (current_configuration != NULL)
                        {
                        CFG_destroy(current_configuration);
                        current_configuration = NULL;
                        }

                     current_configuration = CFG_create(selected);
                     }
                  }

                break;
                }

            case btnPlay3D:
               {
               if (current_configuration != NULL)
                  {
                  CFG_play_current_sample(current_configuration);
                  }
               break;
               }

            case btnPlay2D:
               {
               if (opened_2D_sample)
                  {
                  AIL_start_sample(opened_2D_sample);
                  }
               break;
               }

            case btnStop:
               {
               if (current_configuration != NULL)
                  {
                  CFG_stop_all_sounds(current_configuration);
                  }

               if (opened_2D_sample)
                  {
                  AIL_end_sample(opened_2D_sample);
                  }

               break;
               }

            case btnProviderA:
               DLG_show_provider_props();
               break;

            case btnSoundA:
               DLG_show_sound_props();
               break;

            case btnLoad:
               DLG_show_load();
               break;

            case btnAbout:
               DLG_show_about();
               break;

            case IDCANCEL:
            case btnClose:
               {
               DestroyWindow(hwnd);
               break;
               }
            }
         return 0;

      case WM_TIMER:
         {
         if (current_configuration != NULL)
            {
            CFG_update(current_configuration);
            }
         break;
         }

      case WM_DESTROY:
         {
         PostQuitMessage(0);
         return 0;
         }
      }

   return DefWindowProc(hwnd,message,wParam,lParam);
}

//############################################################################
//#                                                                          #
//#  WinMain()                                                               #
//#                                                                          #
//############################################################################

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
   //
   // Create dialog box
   //

   MSG      msg;
   WNDCLASS wndclass;

   if (!hPrevInstance)
      {
      wndclass.lpszClassName = szAppName;
      wndclass.lpfnWndProc   = (WNDPROC) WndProc;
      wndclass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
      wndclass.hInstance     = hInstance;
      wndclass.hIcon         = LoadIcon(hInstance,"Demo");
      wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW);
      wndclass.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
      wndclass.cbClsExtra    = 0;
      wndclass.cbWndExtra    = DLGWINDOWEXTRA;
      wndclass.lpszMenuName  = NULL;

      RegisterClass(&wndclass);
      }

   InitCommonControls();

   hwnd = CreateDialog(hInstance,szAppName,0,NULL);

   if (hwnd==0) 
      {
      C8 error_text[256] = "";

      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    GetLastError(),
                    0,
                    error_text,
                    255,
                    NULL);

      MessageBox(0,error_text,"Couldn't create dialog box",MB_OK);
      return(0);
      }

   //
   // Copy configuration choices to the dropdown list
   //
   
   combo = GetDlgItem(hwnd, cboxConfig);

   ComboBox_AddString(combo,"Choose a configuration...");

   S32 n_configs = sizeof(combo_entries) / sizeof(combo_entries[0]);

   for (S32 i=0; i < n_configs; i++)
      {
      ComboBox_AddString(combo,
                         combo_entries[i].name);
      }

   ComboBox_SetCurSel(combo,0);

   //
   // Disable all controls except the configuration selector, and set defaults
   //

   UI_disable_all_controls();

   UI_set_control_defaults();

   //
   // Create bitmap environment
   //

   E = ENV_create();

   //
   // Initialize the Miles Sound System
   //

   AIL_set_redist_directory( MSS_DIR_UP_TWO MSS_REDIST_DIR_NAME );
   AIL_startup();

   //
   // Demonstrate creation of a separate driver instance for 2D (stereo only) sound output
   //
   // Menu/utility sounds can use a separate 2D driver to avoid consuming potentially-scarce
   // 3D hardware voices
   //

   DIG2D = AIL_open_digital_driver(OUTPUT_RATE, OUTPUT_BITS, 2, 0);

   opened_2D_sample    = NULL;
   sample_2D_address   = NULL;
   sample_2D_file_size = 0;

   //
   // Allocate an HSAMPLE for our default 2D sample, and load the 2D sample file
   //

   if (DIG2D != NULL)
      {
      opened_2D_sample = AIL_allocate_sample_handle(DIG2D);

      if (!FILE_load(sample_2D_filename, 
                    &sample_2D_address, 
                    &sample_2D_file_size))
         {
         MessageBox(hwnd,AIL_last_error(),"Error loading...",MB_OK);
         exit(1);
         }

      if (!AIL_set_named_sample_file(opened_2D_sample, 
                                     sample_2D_filename,
                                     sample_2D_address,
                                     sample_2D_file_size,
                                     0))
         {
         MessageBox(hwnd,AIL_last_error(),"Error loading...",MB_OK);
         exit(1);
         }
      }

   //
   // Load default 3D sample file
   //

   strcpy(loaded_sample_filename, DEFAULT_3D_SAMPLE_FILENAME);

   if (!FILE_load(loaded_sample_filename, 
                 &loaded_sample_address, 
                 &loaded_sample_file_size))
      {
      exit(1);
      }

   //
   // Initialize 3D sample slots
   //

   for (S32 i=0; i < MAX_3D_SOUNDS; i++)
      {
      SND3D *S = &sounds[i];

      memset(S,
             0,
             sizeof(*S));

      S->movement_type = INVALID;

      S->last_time_ms = AIL_ms_count();
      }

   //
   // Start timer to maintain sample status
   //

   SetTimer(hwnd, 
            1, 
            TIMER_UPDATE_MS, 
            0);
   //
   // Main message loop
   //

   ShowWindow(hwnd,nCmdShow);

   while (GetMessage(&msg, 0, 0, 0)) 
      {
      if (!IsDialogMessage(hwnd,&msg)) 
         {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
         }
      }

   //
   // Shutdown
   //

   if (current_configuration != NULL)
      {
      CFG_destroy(current_configuration);
      current_configuration = NULL;
      }

   if (E != NULL)
      {
      ENV_destroy(E);
      E = NULL;
      }

   return (S32) msg.wParam;
}

