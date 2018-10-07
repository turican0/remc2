//############################################################################
//##                                                                        ##
//##  Miles Sound System                                                    ##
//##                                                                        ##
//##  MSSDBG.C: Runtime debugging facilities for MSS API calls              ##
//##                                                                        ##
//##  16-bit protected-mode source compatible with MSC 7.0                  ##
//##  32-bit protected-mode source compatible with MSC 9.0                  ##
//##                                                                        ##
//##  Version 1.00 of 15-Feb-95: Derived from WAILDBG V1.01                 ##
//##          1.01 of 11-Oct-95: Added MEM_alloc/free_lock() debug trace    ##
//##          1.02 of 20-Jan-96: jkr: added all the win32 stuff             ##
//##          1.03 of 20-Mar-98: jkr: merged DOS and windows                ##
//##                                                                        ##
//##  Author: John Miles & Jeff Roberts                                     ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#if defined(_XENON) || (_XBOX_VER == 200) || defined(_XBOX)
#include <xtl.h>
#elif defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(__NT__) || defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define STRICT
#include <windows.h>
#include <mmsystem.h>
#include <ks.h>
#pragma warning( disable : 4214)
#include <ksmedia.h>
#endif

#include "mss.h"
#include "imssapi.h"

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>

#ifdef IS_LINUX
#include <string.h>
#endif

#ifdef IS_PS2
#include <eekernel.h>
#endif

#ifdef IS_WII
#include <revolution/dvd.h>
#endif

//############################################################################
//##                                                                        ##
//## Locked static data                                                     ##
//##                                                                        ##
//############################################################################

U16 AIL_debug=0;

U16 AIL_sys_debug=0;

U16 AIL_indent=0;

U32 AIL_starttime;

#ifndef IS_DOS
AILTRACECB trace_callback;
#endif

#ifdef IS_WIN64

  #define PTRFMT "0x%I64X"
  #define SINTFMT "%I64d"
  #define HPFMT "%ld"

#else

  #define PTRFMT "0x%lX"
  #define SINTFMT "%ld"
  #define HPFMT "%ld"

#endif

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS3)

FILE *AIL_debugfile;
#define NL "\n"

#elif defined(IS_WII)

U32 AIL_debugfile;
#define NL "\n"

#else

U32 AIL_debugfile;
#define NL "\r\n"

#endif

#if defined(IS_WIN32API) || defined(IS_PS3) || defined(IS_MAC)

  #define IN_AIL  InMilesMutex()
  #define OUT_AIL OutMilesMutex()

  #define IN_AIL_NM
  #define OUT_AIL_NM

#else
  #define IN_AIL
  #define OUT_AIL

  #define IN_AIL_NM
  #define OUT_AIL_NM
#endif

#ifdef IS_32

  int AIL_didaninit=NO;

  #define DidAnInit() (AIL_didaninit!=NO)

  #define SetInit(val) AIL_didaninit=(val);

#else

  #define MAXTASKS 32             // Only 32 16-bit apps can be used at once

  static HTASK inittasks[MAXTASKS];

  static int numtasks=0;

  static U8 DidAnInit()          // See if this task has previously called
  {                              //   AIL_startup
    int i;
    HTASK t=GetCurrentTask();
    for(i=0;i<numtasks;i++)
      if (inittasks[i]==t)
        return(1);
    return(0);
  }

  static void SetInit(int val)   // Clear or set the task's init status
  {
    int i;
    HTASK t;

    if (val==NO) {
      t=GetCurrentTask();
      for(i=0;i<numtasks;i++)
        if (inittasks[i]==t) {
          AIL_memcpy((void*)&inittasks[i],(void*)&inittasks[i+1],(numtasks-i-1)*sizeof(inittasks[0]));
          --numtasks;
          break;
        }
    } else {
      if (numtasks>=MAXTASKS)
        MessageBox(0,"Too many MSS applications loaded at once.","Error from MSS",MB_OK);
      else
        inittasks[numtasks++]=GetCurrentTask();
    }
  }

#endif

//############################################################################
//##                                                                        ##
//## Debug trace macros and helper functions                                ##
//##                                                                        ##
//############################################################################

#define START  if (AIL_indent++, (AIL_debug && \
                  ((AIL_indent==1) || AIL_sys_debug) && \
                  (!AIL_low_background() && AIL_time_write())))

#define END    AIL_indent--

#define RESULT if (AIL_debug && ((AIL_indent==1) || AIL_sys_debug) && (!AIL_low_background()))

#define INDENT AIL_fprintf(AIL_debugfile,"%s%s",(char FAR *)indentspace,(char FAR *)indentdot+33-AIL_indent);

static char indentspace[]="               ";
static char indentdot[]="--------------------------------";

#define FTS AIL_ftoa

#ifdef IS_PS2

static void AIL_fprintf (U32 out, char const FAR *fmt, ...)
{
   va_list ap;

   va_start(ap, 
            fmt);

   if (trace_callback == NULL)
      {
      sceVprintf(fmt, 
                 ap);
      }
   else
      {
      C8 text[256];

      sceVsnprintf(text,
                   sizeof(text)-1,
                   fmt,
                   ap);
     text[sizeof(text)-1]=0;

      trace_callback(text,
                     AIL_indent);
      }

   va_end(ap);
}

#elif defined(IS_WII)

static void AIL_fprintf (U32 out, char const FAR *fmt, ...)
{
   C8 text[512];
   va_list ap;

   va_start(ap, 
            fmt);

   vsnprintf(text,
             sizeof(text)-1,
             fmt,
             ap);
   text[sizeof(text)-1]=0;

   if (trace_callback == NULL)
      {
      OSReport( text );
      }
      else
      {
      trace_callback(text,
                     AIL_indent);
      }

   va_end(ap);
}

#else

#ifndef IS_DOS
static void AIL_fprintf (FILE *out, char const FAR *fmt, ...)
{
   va_list ap;

   va_start(ap, 
            fmt);

   if (trace_callback == NULL)
      {
      vfprintf(out,
               fmt, 
               ap);
      }
   else
      {
      C8 text[256];

      #if defined(IS_MAC) || defined(IS_PS3) || defined(IS_WII)
      vsnprintf(text,
      #else
      _vsnprintf(text,
      #endif
                 sizeof(text)-1,
                 fmt,
                 ap);
      text[sizeof(text)-1]=0;

      trace_callback(text,
                     AIL_indent);
      }

   va_end(ap);
}
#endif

#endif

#ifdef IS_DOS

//############################################################################
//##                                                                        ##
//## Locked code                                                            ##
//##                                                                        ##
//############################################################################

#ifdef __WATCOMC__
#pragma aux AIL_debug "_*";
#pragma aux AIL_sys_debug "_*";
#pragma aux AIL_indent "_*";
#pragma aux AIL_starttime "_*";
#pragma aux AIL_debugfile "_*";
#pragma aux AIL_didaninit "_*";
#endif

#define LOCK(x) AIL_vmm_lock((void *) &(x),sizeof(x))

static S32 locked = 0;

void __pascal AILDEBUG_end(void);

void __pascal AILDEBUG_start(void)
{
   if (!locked)
      {
      AIL_vmm_lock_range(AILDEBUG_start, AILDEBUG_end);

      LOCK (AIL_indent);
      LOCK (AIL_debug);
      LOCK (AIL_sys_debug);
      LOCK (AIL_debugfile);

      LOCK (AIL_didaninit);
      LOCK (AIL_starttime);

      locked = 1;

      }
}

#endif
                                        
static void outresfloat(F32 val)
{
  INDENT;
  AIL_fprintf(AIL_debugfile,"Result = %s" NL,FTS(val));
}

static void outresint(U32 val)
{
  INDENT;
  AIL_fprintf(AIL_debugfile,"Result = %d" NL,val);
}

static void outresstr(C8 *val)
{
  INDENT;
  if (val == NULL)
      AIL_fprintf(AIL_debugfile,"Result = NULL (no error)" NL);
   else
      AIL_fprintf(AIL_debugfile,"Result = %s" NL,val);
}

static void outreshex(U32 val)
{
  INDENT;
  AIL_fprintf(AIL_debugfile,"Result = 0x%X" NL,val);
}

static void outresuint(UINTa val)
{
  INDENT;
  AIL_fprintf(AIL_debugfile,"Result = " SINTFMT NL,val); 
}

static void outresuhex(UINTa val)
{
  INDENT;
  AIL_fprintf(AIL_debugfile,"Result = " PTRFMT NL,val); 
}

DXDEF C8 * AILCALL AIL_ftoa(F32 fval)
{
#define N_STRINGS_IN_POOL     16
#define MAX_POOL_STRING_BYTES 32

   //
   // Maintain pool of strings so that AIL_ftoa() can be called multiple
   // times without invalidating previous results
   // (e.g., in a printf() statement)
   //
   // Background and foreground contexts need separate string pools
   // to avoid collisions
   //

   static C8  f_pool[N_STRINGS_IN_POOL][MAX_POOL_STRING_BYTES];
   static S32 f_cur = 0;

   static C8  b_pool[N_STRINGS_IN_POOL][MAX_POOL_STRING_BYTES];
   static S32 b_cur = 0;

#ifdef IS_PS2
   S32 irq = DI();
#endif

   C8 *result;

   if (AIL_API_background())
      {
      result = b_pool[b_cur];
      b_cur = (b_cur + 1) % N_STRINGS_IN_POOL;
      }
   else
      {
      result = f_pool[f_cur];
      f_cur = (f_cur + 1) % N_STRINGS_IN_POOL;
      }

#ifdef IS_PS2
   if (irq) EI();
   
   // 
   // GCC under PS2 cannot handle floats in vararg functions, so
   // we have to implement our own float-to-string handler
   //

#define FRACT_DIGITS 4

   {
   S32 s,i;
   S32 fixed;
   C8 buffer[MAX_POOL_STRING_BYTES];
   C8 *d,*r;

   s = (fval < 0.0F);
   if (s) fval = -fval;

   fixed = (S32) ((AIL_pow(10.0F,FRACT_DIGITS) * fval) + 0.5F);

   d = buffer;

   for (i=0; i < MAX_POOL_STRING_BYTES-3; i++)
      {
      *d++ = (fixed % 10) + '0';
      fixed /= 10;

      if (i == (FRACT_DIGITS-1))
         {
         *d++ = '.';
         }

      if ((!fixed) && (i >= (FRACT_DIGITS)))
         {
         break;
         }
      }

   if (s) 
      {
      *d++ = '-';
      }

   r = result;
   while (d > buffer)
      {
      *r++ = *--d;
      }
   *r = 0;
   }
#else

   sprintf(result,"%f",fval);

#endif

   return result;
}

#if defined(IS_WIN32) || defined(IS_PS2) || defined(IS_MAC) || defined(IS_WII)

//
// Open a matrix- or output-filter provider
//

S32 MC_open_output_filter(C8        *name, //)
                          HDIGDRIVER driver,
                          S32        is_matrix_filter)
{
   HPROVIDER HP;

   if ((is_matrix_filter) && (!(driver->hw_format & DIG_F_16BITS_MASK)))
      {
      AIL_set_error("Matrix filters require 16-bit PCM output format");
      return 0;
      }

   if (!AIL_find_filter(name, 
                       &HP))
      {
      AIL_set_error("Required output filter not found");
      return 0;
      }

   if (!AIL_open_filter(HP,
                        driver))
      {
      return 0;
      }

   return 1;
}

#endif
      
#ifdef IS_WIN32

  static U32 doquery=0;   // jkr: use queryperformance instead of timegettime?

  static LONGLONG qpfreq; // frequency of the high-perf timer

  static LONGLONG qpfirst;// first read of queryperformance

  static U32 lasttime;
  static U32 lastrad;
  static U32 radadj=0;

  static U32 lasttimeu;
  static U32 lastradu;
  static U32 radadju=0;

  // jkr: this function is called when the 32 bit DLL is loaded

  BOOL Win32Init(HINSTANCE hinstDll)
  {
    if (QueryPerformanceFrequency((LARGE_INTEGER*)&qpfreq))
    {
      doquery=1;
      QueryPerformanceCounter((LARGE_INTEGER*)&qpfirst);
      lastrad=0;
      lasttime=GetTickCount();
      lastradu=0;
      lasttimeu=lasttime;
    }

    return( TRUE );
  }

//
// we don't display these calls in the debug file because they are
// called so frequently
//

DXDEF U32 AILEXPORT AIL_ms_count()
{
    LONGLONG count;
    if (doquery)
    {
      U32 ret, time;
      S32 deltatime,deltarad;

      // read the new times...
      QueryPerformanceCounter((LARGE_INTEGER*)&count);
      time = GetTickCount();
      ret = radadj + ( (U32)(((count-qpfirst)*((LONGLONG)1000))/qpfreq) );

      // see how much each has moved
      deltatime = time - lasttime;
      deltarad = ret - lastrad;

      // check the query against GetTickCount to make sure it hasn't
      //   jumped ahead...

      if ( abs( deltatime-deltarad ) > 200 )
      {
        deltatime -= deltarad;
        radadj += deltatime;
        ret += deltatime;
      }

      // now check to see if it went backwards?
      if ( ( ( U32 ) ( ret - lastrad ) ) > 0xc0000000 )
      {
        // yup, just return the old timer value then
        return( lastrad );
      }

      lasttime = time;
      lastrad = ret;
      return(ret);

    } else
      return( timeGetTime() );
}

DXDEF U32 AILEXPORT AIL_us_count()
{
    LONGLONG count;
    if (doquery) {
      U32 ret, time;
      S32 deltatime,deltarad;

      // read the new times...
      QueryPerformanceCounter((LARGE_INTEGER*)&count);
      time = GetTickCount();
      ret = radadju + ( (U32)(((count-qpfirst)*((LONGLONG)1000000L))/qpfreq) );

      // see how much each has moved
      deltatime = (time - lasttimeu)*1000;
      deltarad = ret - lastradu;

      // check the query against GetTickCount to make sure it hasn't
      //   jumped ahead...

      if ( abs( deltatime-deltarad ) > 200000 )
      {
        deltatime -= deltarad;
        radadju += deltatime;
        ret += deltatime;
      }

      // now check to see if it went backwards?
      if ( ( ( U32 ) ( ret - lastradu ) ) > 0xc0000000 )
      {
        // yup, just return the old timer value then
        return( lastradu );
      }

      lasttimeu = time;
      lastradu = ret;
      return(ret);
    } else
      return( timeGetTime()*1000 );
}

#endif

#if defined(IS_LINUX)

#include <sys/time.h>

  //
  // we don't display these calls in the debug file because they are
  // called so frequently
  //

  static long long start = 0;

  DXDEF U32 AILEXPORT AIL_ms_count()
  {
    long long now;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    now = tv.tv_sec;
    now *= 1000000;
    now += tv.tv_usec;
    if ( !start ) {
        start = now;
    }
    return (U32)((now - start) / 1000);
  }

  DXDEF U32 AILEXPORT AIL_us_count()
  {
    long long now;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    now = tv.tv_sec;
    now *= 1000000;
    now += tv.tv_usec;
    if ( !start ) {
        start = now;
    }
    return (U32)(now - start);
  }

#endif

// jkr: these macros expand to normal function calls

#define ailcall0(header,func,type)                                    \
          header##_API_##func()

#define ailcall1(header,func,type,param1)                             \
          header##_API_##func(param1)

#define ailcall2(header,func,type,param1,param2)                      \
          header##_API_##func(param1,param2)

#define ailcall3(header,func,type,param1,param2,param3)               \
          header##_API_##func(param1,param2,param3)

#define ailcall4(header,func,type,param1,param2,param3,param4)        \
          header##_API_##func(param1,param2,param3,param4)

#define ailcall5(header,func,type,param1,param2,param3,param4,param5) \
          header##_API_##func(param1,param2,param3,param4,param5)

#define ailcall6(header,func,type,param1,param2,param3,param4,param5,param6) \
          header##_API_##func(param1,param2,param3,param4,param5,param6)

#define ailcall7(header,func,type,param1,param2,param3,param4,param5,param6,param7) \
          header##_API_##func(param1,param2,param3,param4,param5,param6,param7)

#define ailcall14(header,func,type,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14) \
          header##_API_##func(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14)

#define ailcall15(header,func,type,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15) \
          header##_API_##func(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15)

#define ailcall16(header,func,type,param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15,param16) \
          header##_API_##func(param1,param2,param3,param4,param5,param6,param7,param8,param9,param10,param11,param12,param13,param14,param15,param16)

#define dsailcall0 ailcall0
#define dsailcall1 ailcall1
#define dsailcall2 ailcall2
#define dsailcall3 ailcall3
#define dsailcall4 ailcall4
#define dsailcall5 ailcall5
#define dsailcall6 ailcall6
#define dsailcall7 ailcall7
#define dsailcall14 ailcall14
#define dsailcall15 ailcall15
#define dsailcall16 ailcall16

#if defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

static void AIL_get_debug_file(char* fname)
{
  #if defined(IS_XBOX) || defined(IS_XENON)
    AIL_strcpy( fname, "d:\\mss_debug_log.txt" );
  #else
    #if defined(IS_PS3)
      AIL_strcpy( fname, "/dev_hdd0/mss_debug_log.txt" );
    #else
      #if defined(IS_PS2)
        AIL_strcpy( fname, "host0:\\mss_debug_log.txt" );
      #elif defined(IS_WII)
        AIL_strcpy( fname, "/mss_debug_log.txt" );
     #else
        AIL_strcpy( fname, MSS_Directory);
        AIL_strcat(fname,"mssdebug.log");
     #endif
    #endif
  #endif
}

#endif

static S32    AIL_low_background                (void)
{
  return(AIL_API_background());
}

//############################################################################
//##                                                                        ##
//## Write timestamp to output file or device                               ##
//##                                                                        ##
//############################################################################

S32 AILCALL AIL_time_write(void)
{
   U32 t;
   U16 h,m,s,c;

   t = AIL_ms_count()-AIL_starttime;

   h = (U16)(t/3600000L);
   m = (U16)((t/60000L)%60L);
   s = (U16)((t/1000L)%60L);
   c = (U16)(t%1000L);

   if (AIL_indent == 1)
      {
      AIL_fprintf(AIL_debugfile,"[%.02d:%.02d:%.02d.%.03d] %s",h,m,s,c,(char FAR *)indentdot+33-AIL_indent);
      }
   else
      AIL_fprintf(AIL_debugfile,"%s%s", (char FAR *)indentspace,(char FAR *)indentdot+33-AIL_indent);

   return 1;
}

//############################################################################
//##                                                                        ##
//## AIL_MMX_available()                                                    ##
//##                                                                        ##
//############################################################################

#ifdef IS_X86

DXDEF U32 AILEXPORT AIL_MMX_available(void)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_MMX_available()" NL);
      }

   IN_AIL_NM;

   if (!AIL_get_preference(AIL_ENABLE_MMX_SUPPORT))
      {
      result = 0;
      }
   else
      {
      result = MSS_MMX_available();
      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

#endif

//############################################################################
//##                                                                        ##
//## AIL_debug_printf()                                                     ##
//##                                                                        ##
//############################################################################

#ifndef IS_MAC

DXDEF void
#if !defined(IS_LINUX) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII) 
__cdecl
#endif
AIL_debug_printf(C8 const FAR *fmt, ...)
{
  #ifdef _DEBUG
   char work_string[768];

   va_list ap;

   va_start(ap,
            fmt);

#if defined(IS_XBOX) || defined(IS_DOS)
   vsprintf(work_string,
#else
   vsnprintf(work_string,768,
#endif
             fmt,
             ap);

   va_end  (ap);

#if defined(IS_WINDOWS) || defined(IS_XBOX) || defined(IS_XENON)
   OutputDebugString(work_string);
#else
#if SHOW_DEBUG_TRACE || defined(IS_WII)
   printf(work_string);
#endif
#endif

#if DEBUG_LOGFILE
   static S32 init = 0;

   if (init == 0)
      {
      _unlink("debug.log");
      init = 1;
      }

   FILE FAR*out = fopen("debug.log","a+t");
   fprintf(out,"%s",work_string);
   fclose(out);
#endif
#endif
}

#else

extern void DebugStr( unsigned char const * );

DXDEF void AIL_debug_printf(C8 const FAR *fmt, ...)
{
  #ifdef _DEBUG
   char work_string[256];

   va_list ap;

   va_start(ap,
            fmt);

   work_string[0] = (char) vsnprintf(work_string + 1, 255,fmt,ap);
   work_string[255]=0;
   
   va_end  (ap);
   DebugStr( (U8*) work_string );
  #endif
}
#endif

#ifndef IS_DOS

//############################################################################
DXDEC  AILTRACECB AILCALL AIL_register_trace_callback (AILTRACECB cb, 
                                                       S32        level)
{
   AILTRACECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_trace_callback(" PTRFMT ",0x%ld)" NL,
         cb, level);
      }

   IN_AIL;

   result         = trace_callback;
   trace_callback = cb;

   switch (level)
      {
      case 0: 
         AIL_debug      = FALSE;
         AIL_sys_debug  = FALSE;
         break;

      case 1:
         AIL_debug      = TRUE;
         AIL_sys_debug  = FALSE;
         break;

      case 2:
         AIL_debug      = TRUE;
         AIL_sys_debug  = TRUE;
         break;
      }

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

#endif

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

//############################################################################
DXDEF S32    AILEXPORT AIL_startup                   (void)
{
   static char filename[256];
   static S8 sysdebug[8];
#ifndef IS_PS2
   static time_t elapstime;
   static struct tm *adjtime;
   static char loctime[32];
#endif

   if (DidAnInit())
     return(0);

   SetInit(YES);

#ifdef IS_WIN32API

   //
   // Get handle to application process thread that called AIL_startup(),
   // so that callback functions can suspend it
   //

   DuplicateHandle(GetCurrentProcess(),
                   GetCurrentThread(),
                   GetCurrentProcess(),
                  &hAppThread,
                   0,
                   FALSE,
                   DUPLICATE_SAME_ACCESS);

#endif

   //
   // Get profile string for debug script, and enable debug mode
   // if script filename valid
   //

   AIL_debug     = 0;
   AIL_sys_debug = 0;

#if defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

  AIL_get_debug_file( filename );

  #if defined(IS_WII)
    if (DVDConvertPathToEntrynum(filename)!=-1)
      AIL_debugfile=1;
    else
      AIL_debugfile=0;
  #elif defined(IS_PS2) 
     {
     U32 hand = 0;
     AIL_debugfile = MSS_open(filename, &hand);

     if (AIL_debugfile)
        {
        MSS_close(hand);
        }
     }
  #else
    AIL_debugfile = fopen(filename,"r");
    if(AIL_debugfile)
    {
      fclose(AIL_debugfile);
      AIL_debugfile = fopen(filename,"wt");       // Debug file is used as a journaling target
      AIL_sys_debug = 0;
    }
  #endif

#else

   GetProfileString("MSS", "MSSDebug", "None", filename, sizeof(filename));

   if (!AIL_strnicmp(filename, "None", 4))
      {
      ailcall0(AIL,startup,void);
      return(1);
      }

   GetProfileString("MSS", "MSSSysDebug", "0", sysdebug, sizeof(sysdebug));

   if (AIL_strnicmp(sysdebug, "0", 1))
      {
      AIL_sys_debug = 1;
      }

   //
   // Open script file and set "debug" flag
   //

   AIL_debugfile = fopen(filename,"wt");        // Debug file is used only as a flag

#endif

   //
   // if there is no debug file opened, then just startup normally
   //

   if (AIL_debugfile == 0)
      {
      ailcall0(AIL,startup,void);
      return(1);
      }

   //
   // Write header to script file
   //

#if defined(IS_PS2) || defined(IS_WII)

   AIL_fprintf(AIL_debugfile,
               "-------------------------------------------------------------------------------" NL
               "Miles Sound System usage script generated by MSS V"MSS_VERSION"" NL
               "-------------------------------------------------------------------------------" NL NL);
#else

   time(&elapstime);
   adjtime = localtime(&elapstime);
   AIL_strcpy(loctime,asctime(adjtime));
   loctime[24] = 0;

   AIL_fprintf(AIL_debugfile,
               "-------------------------------------------------------------------------------" NL
               "Miles Sound System usage script generated by MSS V"MSS_VERSION"" NL
               "Start time: %s" NL
               "-------------------------------------------------------------------------------" NL NL
               ,(U8 FAR *) loctime);
#endif

   //
   // Initialize API
   //

   ailcall0(AIL,startup,void);

   AIL_starttime=AIL_ms_count();

   AIL_debug  = 1;

   AIL_indent = 1;
   AIL_time_write();
   AIL_indent = 0;

   AIL_fprintf(AIL_debugfile,"AIL_startup()" NL);

   return(1);
}

#endif

//############################################################################

DXDEF
void    AILEXPORT  AIL_shutdown                  (void)
{
   if (DidAnInit())
     SetInit(NO);

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_shutdown()" NL);
      }

   IN_AIL;

   ailcall0(AIL,shutdown,void);

   OUT_AIL;        

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS3)
   if (AIL_debug && (AIL_debugfile != 0))
      fclose(AIL_debugfile);
#endif

   END;

   #ifdef IS_WIN32API

   if (hAppThread)
   {
     CloseHandle(hAppThread);     //avoid debug kernel message
     hAppThread=0;
   }

   #endif

}

//############################################################################

#ifdef IS_WIN32

DXDEF
HWND AILEXPORT AIL_HWND(void)
{
   HWND result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_HWND()" NL);
      }

   IN_AIL_NM;

   result = AIL_API_HWND();

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

#else

#ifdef IS_PS2

//############################################################################
DXDEF 
MSS_RESOURCE_POLICY AILEXPORT AIL_set_scratchpad_policy (MSS_RESOURCE_POLICY policy)
{
   MSS_RESOURCE_POLICY result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_scratchpad_policy(%d)" NL,policy);
      }

   IN_AIL_NM;

   result = ailcall1(AIL,set_scratchpad_policy,MSS_RESOURCE_POLICY,policy);

   OUT_AIL_NM;

   RESULT
      {
      outresint((S32) result);
      }

   END;

   return result;
}

//############################################################################
DXDEF 
MSS_RESOURCE_POLICY AILEXPORT AIL_scratchpad_policy (void)
{
   MSS_RESOURCE_POLICY result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_scratchpad_policy()" NL);
      }

   IN_AIL_NM;

   result = ailcall0(AIL,scratchpad_policy,MSS_RESOURCE_POLICY);

   OUT_AIL_NM;

   RESULT
      {
      outresint((S32) result);
      }

   END;

   return result;
}

#endif

#endif

//############################################################################

DXDEF
void FAR * AILEXPORT AIL_mem_alloc_lock(UINTa size)
{
   void FAR *result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_mem_alloc_lock(" SINTFMT ")" NL,size);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,mem_alloc_lock,void FAR*,size);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void AILEXPORT AIL_mem_free_lock(void FAR *ptr)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_mem_free_lock(" PTRFMT ")" NL,ptr);
      }

   IN_AIL_NM;

   ailcall1(AIL,mem_free_lock,void,ptr);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
SINTa    AILEXPORT  AIL_set_preference            (U32       number, //)
                                                   SINTa     value)
{
   SINTa result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_preference(%ld," SINTFMT ")" NL,number,value);
      }

   IN_AIL;

   result=ailcall2(AIL,set_preference,SINTa,number,value);

   OUT_AIL;

   RESULT
      {
      outresuint((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
char FAR * AILEXPORT  AIL_last_error               (void)
{
   char FAR *result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_last_error()" NL);
      }

   IN_AIL_NM;

   result=ailcall0(AIL,last_error,char FAR*);

   OUT_AIL_NM;

   RESULT
      {
      outresstr(result);
      }

   END;

   return (char FAR*) result;
}

//############################################################################

DXDEF
void AILEXPORT  AIL_set_error               (char const FAR* error_msg)
{
   START
      {
      if (error_msg == NULL)
         {
         AIL_fprintf(AIL_debugfile,"AIL_set_error(NULL)" NL);
         }
      else
         {
         AIL_fprintf(AIL_debugfile,"AIL_set_error(%s)" NL, error_msg);
         }
      }

   IN_AIL_NM;

   ailcall1(AIL,set_error,void,error_msg);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_delay                     (S32        intervals)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_delay(%ld)" NL,intervals);
      }

   IN_AIL_NM;

#ifdef IS_WIN32API

   Sleep(intervals * 100 / 6);

#else

   AIL_API_delay(intervals);

#endif

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
S32    AILEXPORT  AIL_background                (void)
{
   S32 result;

   IN_AIL_NM;

   result=AIL_low_background();

   OUT_AIL_NM;

   return result;
}

//############################################################################

DXDEF
HTIMER  AILEXPORT  AIL_register_timer            (AILTIMERCB    callback_fn)
{
   HTIMER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_timer(" PTRFMT ")" NL,callback_fn);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,register_timer,HTIMER,callback_fn);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
UINTa AILEXPORT  AIL_set_timer_user            (HTIMER      timer, //)
                                                UINTa       user)
{
   UINTa result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_timer_user(%ld," SINTFMT ")" NL,timer,user);
      }

   IN_AIL;

   result=ailcall2(AIL,set_timer_user,UINTa,timer,user);

   OUT_AIL;

   RESULT
      {
      outresuint((UINTa) result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_set_timer_period          (HTIMER      timer, //)
                                               U32         microseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_timer_period(%ld,%lu)" NL,timer,microseconds);
      }

   IN_AIL;

   ailcall2(AIL,set_timer_period,void,timer,microseconds);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_set_timer_frequency       (HTIMER      timer, //)
                                               U32         hertz)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_timer_frequency(%ld,%lu)" NL,timer,hertz);
      }

   IN_AIL;

   ailcall2(AIL,set_timer_frequency,void,timer,hertz);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_set_timer_divisor         (HTIMER      timer, //)
                                               U32         PIT_divisor)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_timer_divisor(%ld,%lu)" NL,timer,PIT_divisor);
      }

   IN_AIL;

   ailcall2(AIL,set_timer_divisor,void,timer,PIT_divisor);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_start_timer               (HTIMER      timer)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_start_timer(%lu)" NL,timer);
      }

   IN_AIL;

   ailcall1(AIL,start_timer,void,timer);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_start_all_timers          (void)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_start_all_timers()" NL);
      }

   IN_AIL;

   ailcall0(AIL,start_all_timers,void);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_stop_timer                (HTIMER      timer)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stop_timer(%lu)" NL,timer);
      }

   IN_AIL;

   ailcall1(AIL,stop_timer,void,timer);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_stop_all_timers           (void)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stop_all_timers()" NL);
      }

   IN_AIL;

   ailcall0(AIL,stop_all_timers,void);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_release_timer_handle      (HTIMER      timer)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_release_timer_handle(%lu)" NL,timer);
      }

   IN_AIL;

   ailcall1(AIL,release_timer_handle,void,timer);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void    AILEXPORT  AIL_release_all_timers        (void)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_release_all_timers()" NL);
      }

   IN_AIL;

   ailcall0(AIL,release_all_timers,void);

   OUT_AIL;

   END;
}

//############################################################################

#ifdef IS_WIN32

//
// (Not currently exported as a separate API call -- accessible only 
// via AIL_open_digital_driver())
//

static HDIGDRIVER AIL_open_multichannel_driver( U32         frequency,
                                                S32         bits,
                                                MSS_MC_SPEC channel_spec,
                                                U32         flags )
{
   HDIGDRIVER result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_digital_driver(%ld, %ld, %ld, 0x%lX)" NL,
                                                         frequency,
                                                         bits,
                                                         channel_spec,
                                                         flags);
      }

   IN_AIL;

#ifdef IS_WIN32
   {
   WAVEFORMATEXTENSIBLE wf;
   S32                  ds_emulated_ok = 0;
   MSSVECTOR3D          SPKR[MAX_SPEAKERS];

   AIL_memset(&wf,
               0,
               sizeof(wf));

   wf.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;    
   wf.Format.cbSize     = 22;     
   wf.SubFormat         = KSDATAFORMAT_SUBTYPE_PCM;

   if (channel_spec == MSS_MC_USE_SYSTEM_CONFIG)
      {
      if (!get_system_speaker_configuration(&channel_spec))
         {
         goto bail;
         }
      }

   switch (channel_spec)
      {
      //
      // Turn channel_spec into wf.Format.nChannels, wf.dwChannelMask, default speaker positions
      //
      // Low byte = # of physical (DAC) channels
      // High byte = # of logical (source) channels
      //
      // Formats that use voice-output filters (like DirectSound3D) should specify zero logical channels,
      // to indicate that the core output system is not used
      //
      
      case MSS_MC_DIRECTSOUND3D:
      case MSS_MC_EAX2:
      case MSS_MC_EAX3:
      case MSS_MC_EAX4:
         {
         AIL_set_preference(DIG_DS_DSBCAPS_CTRL3D, TRUE);
         AIL_set_preference(DIG_USE_WAVEOUT,       FALSE);
         AIL_set_preference(DIG_DS_USE_PRIMARY,    NO);

         wf.Format.nChannels = 0x0;
         break;
         }

      case MSS_MC_MONO:
         {
         wf.Format.nChannels = 0x101;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT;
         break;
         }

      case MSS_MC_STEREO:
         {
         wf.Format.nChannels = 0x202;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT + 
                            SPEAKER_FRONT_RIGHT;

         SPKR[0].x = -0.7F; SPKR[0].y = 0.0F; SPKR[0].z = 0.7F;
         SPKR[1].x =  0.7F; SPKR[1].y = 0.0F; SPKR[1].z = 0.7F;
         break;
         }

      case MSS_MC_HEADPHONES:   
         {
         wf.Format.nChannels = 0x202;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT + 
                            SPEAKER_FRONT_RIGHT;

         SPKR[0].x = -1.0F; SPKR[0].y = 0.0F; SPKR[0].z = 0.0F;
         SPKR[1].x =  1.0F; SPKR[1].y = 0.0F; SPKR[1].z = 0.0F;
         break;
         }

      case MSS_MC_40_DTS:
         {
         wf.Format.nChannels = 0x402;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT;
         break;
         }

      case MSS_MC_40_DISCRETE:           
         {
         wf.Format.nChannels = 0x404;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT  + 
                            SPEAKER_FRONT_RIGHT + 
                            SPEAKER_BACK_LEFT   +
                            SPEAKER_BACK_RIGHT;
         break;
         }

      case MSS_MC_DOLBY_SURROUND:  
         {
         wf.Format.nChannels = 0x302;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT;
         break;
         }

      case MSS_MC_SRS_CIRCLE_SURROUND:
         {
         wf.Format.nChannels = 0x702;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT;
         break;
         }

      case MSS_MC_51_DTS:
         {
         wf.Format.nChannels = 0x602;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT;
         break;
         }

      case MSS_MC_51_DISCRETE:  
         {
         wf.Format.nChannels = 0x606;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT   + 
                            SPEAKER_FRONT_CENTER  +
                            SPEAKER_LOW_FREQUENCY +
                            SPEAKER_BACK_LEFT     +
                            SPEAKER_BACK_RIGHT;
         break;
         }

      case MSS_MC_61_DISCRETE:  
         {
         wf.Format.nChannels = 0x707;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT   + 
                            SPEAKER_FRONT_CENTER  +
                            SPEAKER_LOW_FREQUENCY +
                            SPEAKER_BACK_LEFT     +
                            SPEAKER_BACK_RIGHT    + 
                            SPEAKER_BACK_CENTER;
         break;
         }

      case MSS_MC_71_DISCRETE:           
         {
         wf.Format.nChannels = 0x808;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT   + 
                            SPEAKER_FRONT_CENTER  +
                            SPEAKER_LOW_FREQUENCY +
                            SPEAKER_BACK_LEFT     +
                            SPEAKER_BACK_RIGHT    +
                            SPEAKER_SIDE_LEFT     +
                            SPEAKER_SIDE_RIGHT;
         break;
         }

      case MSS_MC_81_DISCRETE:
         {
         wf.Format.nChannels = 0x909;

         wf.dwChannelMask = SPEAKER_FRONT_LEFT    + 
                            SPEAKER_FRONT_RIGHT   + 
                            SPEAKER_FRONT_CENTER  +
                            SPEAKER_LOW_FREQUENCY +
                            SPEAKER_BACK_LEFT     +
                            SPEAKER_BACK_RIGHT    +
                            SPEAKER_BACK_CENTER   +
                            SPEAKER_SIDE_LEFT     +
                            SPEAKER_SIDE_RIGHT;
         break;
         }

      default:
         {
         //
         // Invalid enum submitted
         //

         AIL_set_error("Unsupported output format (MSS_MC_SPEC enum required)");
         goto bail;
         }
      }

   wf.Format.nSamplesPerSec       = (frequency) ? frequency : 22050;
   wf.Format.wBitsPerSample       = (bits == 8) ? 8 : 16;
   wf.Samples.wValidBitsPerSample = wf.Format.wBitsPerSample;

   wf.Format.nBlockAlign          = (wf.Format.nChannels == 0) ? 
                                     2 : ((wf.Format.wBitsPerSample / 8) * (wf.Format.nChannels & 0xff));

   wf.Format.nAvgBytesPerSec      = (U32) wf.Format.nSamplesPerSec * (U32) wf.Format.nBlockAlign;

   if ((flags & AIL_OPEN_DIGITAL_FORCE_PREFERENCE) == 0)
      {
      AIL_set_preference(DIG_USE_WAVEOUT,FALSE);
      }

   if (AIL_API_waveOutOpen(&result,0,(LPWAVEFORMAT) &wf) == 0)
      {
      if ((result->emulated_ds) && ((flags & AIL_OPEN_DIGITAL_FORCE_PREFERENCE) == 0))
         {
         OutMilesMutex();
         AIL_API_waveOutClose(result);
         InMilesMutex();

         ds_emulated_ok = 1;
         goto trywaveout;
         }
      }
   else
      {
      if ((flags & AIL_OPEN_DIGITAL_FORCE_PREFERENCE) == 0)
         {
trywaveout:
         AIL_set_preference(DIG_USE_WAVEOUT,TRUE);
tryagain:
         if (AIL_API_waveOutOpen(&result, 0, (LPWAVEFORMAT) &wf))
            {
            // if waveout failed, but emulated suceeded, go back to directsound

            if ( ds_emulated_ok )
               {
               ds_emulated_ok=0;
               AIL_set_preference(DIG_USE_WAVEOUT,FALSE);
               goto tryagain;
               }

            result=0;
            }
         }
      else  
         {
         result=0;
         }
      }

   //
   // Perform format-specific operations (such as filter installation) after 
   // driver has been created
   //

   if (result != 0)
      {
      S32 filter_OK = 1;

      result->channel_spec = channel_spec;

      switch (channel_spec)
         {
         case MSS_MC_STEREO:
         case MSS_MC_HEADPHONES:
            {
            //
            // Use SPKR array to distinguish between speakers and headphones,
            // keeping the default falloff power established when the driver was opened 
            //
            // Otherwise, the default speaker positions are set up at driver-allocation time
            //

            AIL_set_speaker_configuration(result, 
                                          SPKR, 
                                          2, 
                                          result->D3D.falloff_power);
            break;
            }

         case MSS_MC_40_DTS:
         case MSS_MC_51_DTS:
            {
            //
            // DTS requires its own postmix filter
            // (PS2 only for now, but hook installed for future use...)
            //

            filter_OK = MC_open_output_filter("DTS 4.0/5.1 Matrix Filter", result, TRUE);
            break;
            }

         case MSS_MC_SRS_CIRCLE_SURROUND:
            {
            //
            // SRS requires its own postmix filter
            //

            filter_OK = MC_open_output_filter("SRS Circle Surround 6.1 Matrix Filter", result, TRUE);
            break;
            }

         case MSS_MC_DOLBY_SURROUND:
            {
            //
            // Dolby Surround requires its own postmix filter
            //

            filter_OK = MC_open_output_filter("Dolby Surround Matrix Filter", result, TRUE);
            break;
            }

         //
         // Each DirectSound3D provider maintains its own secondary buffer for each voice
         // via a per-voice output filter
         //

         case MSS_MC_DIRECTSOUND3D:
            {
            filter_OK = MC_open_output_filter("DirectSound3D Hardware Support", result, FALSE);
            break;
            }

         case MSS_MC_EAX2:
            {
            filter_OK = MC_open_output_filter("Creative Labs EAX 2 (TM)", result, FALSE);
            break;
            }

         case MSS_MC_EAX3:
            {
            filter_OK = MC_open_output_filter("Creative Labs EAX 3 (TM)", result, FALSE);
            break;
            }

         case MSS_MC_EAX4:
            {
            filter_OK = MC_open_output_filter("Creative Labs EAX 4 (TM)", result, FALSE);
            break;
            }
         }

      if (!filter_OK)
         {
         //
         // A required matrix filter or voice filter could not be associated with the driver
         // Close the driver handle and return failure
         //

         OutMilesMutex();
         AIL_API_waveOutClose(result);
         InMilesMutex();

         result = 0;
         }
      }

bail:
   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;
   }
#endif

   return result;
}

#endif


//############################################################################

#ifdef IS_PS2

//
// (Not currently exported as a separate API call -- accessible only 
// via AIL_open_digital_driver())
//

static HDIGDRIVER AIL_open_multichannel_driver( U32         frequency,
                                                S32         bits,
                                                MSS_MC_SPEC channel_spec,
                                                U32         flags )
{
   HDIGDRIVER result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_digital_driver(%ld, %ld, %ld, 0x%lX)" NL,
                                                         frequency,
                                                         bits,
                                                         channel_spec,
                                                         flags);
      }

   IN_AIL;

   {
   MSSVECTOR3D SPKR[MAX_SPEAKERS];

   if ((channel_spec == MSS_MC_USE_SYSTEM_CONFIG) || (channel_spec == MSS_MC_MONO))
      {
      channel_spec = MSS_MC_STEREO;
      }

   switch (channel_spec)
      {
      case MSS_MC_STEREO:
         {
         SPKR[0].x = -0.7F; SPKR[0].y = 0.0F; SPKR[0].z = 0.7F;
         SPKR[1].x =  0.7F; SPKR[1].y = 0.0F; SPKR[1].z = 0.7F;
         break;
         }

      case MSS_MC_HEADPHONES:   
         {
         SPKR[0].x = -1.0F; SPKR[0].y = 0.0F; SPKR[0].z = 0.0F;
         SPKR[1].x =  1.0F; SPKR[1].y = 0.0F; SPKR[1].z = 0.0F;
         break;
         }

      case MSS_MC_DOLBY_SURROUND:  
         {
         break;
         }

      case MSS_MC_SRS_CIRCLE_SURROUND:
         {
         break;
         }

      case MSS_MC_40_DTS:
      case MSS_MC_51_DTS:
         {
         flags |= AIL_OPEN_DIGITAL_USE_IOP_CORE0;
         break;
         }

      default:
         {
         //
         // Invalid enum submitted
         //

         AIL_set_error("Unsupported output format (MSS_MC_SPEC enum required)");
         goto bail;
         }
      }

   result = AIL_API_open_digital_driver( frequency, bits, channel_spec, flags );

   //
   // Perform format-specific operations (such as filter installation) after 
   // driver has been created
   //

   if (result != 0)
      {
      S32 filter_OK = 1;

      result->channel_spec = channel_spec;

      switch (channel_spec)
         {
         case MSS_MC_STEREO:
         case MSS_MC_HEADPHONES:
            {
            //
            // Use SPKR array to distinguish between speakers and headphones,
            // keeping the default falloff power established when the driver was opened 
            //
            // Otherwise, the default speaker positions are set up at driver-allocation time
            //

            AIL_set_speaker_configuration(result, 
                                          SPKR, 
                                          2, 
                                          result->D3D.falloff_power);
            break;
            }

         case MSS_MC_40_DTS:
         case MSS_MC_51_DTS:
            {
            //
            // DTS requires its own postmix filter
            //

            filter_OK = MC_open_output_filter("DTS 4.0/5.1 Matrix Filter", result, TRUE);
            break;
            }

         case MSS_MC_SRS_CIRCLE_SURROUND:
            {
            //
            // SRS requires its own postmix filter
            //

            filter_OK = MC_open_output_filter("SRS Circle Surround 6.1 Matrix Filter", result, TRUE);
            break;
            }

         case MSS_MC_DOLBY_SURROUND:
            {
            //
            // Dolby Surround requires its own postmix filter
            //

            filter_OK = MC_open_output_filter("Dolby Surround Matrix Filter", result, TRUE);
            break;
            }

         default:
            break;
         }

      if (!filter_OK)
         {
         //
         // A required matrix filter or voice filter could not be associated with the driver
         // Close the driver handle and return failure
         //

         OutMilesMutex();
         AIL_API_close_digital_driver(result);
         InMilesMutex();

         result = 0;
         }
      }

bail:
   OUT_AIL;

   RESULT
      {
      outreshex((U32)result);
      }

   END;
   }

   return result;
}

#endif

//############################################################################

DXDEF HDIGDRIVER AILEXPORT AIL_open_digital_driver( U32 frequency,
                                                    S32 bits,
                                                    S32 channels,
                                                    U32 flags )
{
   HDIGDRIVER result;
   
#if defined(IS_XENON)
   channels = MSS_MC_51_DISCRETE;   // TODO: headphone support on Xenon
#endif

#if defined(IS_WIN32) || defined(IS_PS2)

   //
   // In Win32, channel specs beyond mono/stereo require the creation of a 
   // WAVEFORMATEXTENSIBLE structure rather than a PCMWAVEFORMAT structure.  For maximum
   // compatibility with older/legacy systems, we'll avoid calling the MC-compatible
   // version for channel specs that don't require it
   //
   // PS2 also goes through this path for SRS/Dolby/DTS support
   //

   if (channels > 2)
      {
      result = AIL_open_multichannel_driver(frequency,
                                            bits,
                              (MSS_MC_SPEC) channels,
                                            flags);
      return result;
      }
#endif

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_digital_driver(%ld, %ld, %ld, 0x%lX)" NL,
                                                             frequency,
                                                             bits,
                                                             channels,
                                                             flags);
      }

   IN_AIL;

   #if defined(IS_WINDOWS)

   {
     PCMWAVEFORMAT wf;

     #ifdef IS_WIN32
     S32 ds_emulated_ok = 0;
     #endif

     wf.wf.wFormatTag      = WAVE_FORMAT_PCM;
     wf.wf.nChannels       = (channels==1)?1:2;
     wf.wf.nSamplesPerSec  = (frequency)?frequency:22050;
     wf.wBitsPerSample     = (bits==8)?8:16;

     wf.wf.nBlockAlign     = (wf.wBitsPerSample / 8) * wf.wf.nChannels;
     wf.wf.nAvgBytesPerSec = (U32)wf.wf.nSamplesPerSec * (U32)wf.wf.nBlockAlign;

     if ((flags&AIL_OPEN_DIGITAL_FORCE_PREFERENCE)==0)
       AIL_set_preference(DIG_USE_WAVEOUT,FALSE);

     if (AIL_API_waveOutOpen(&result,0,(LPWAVEFORMAT)&wf)==0)
     {
       #ifdef IS_WIN32
       if ((result->emulated_ds) && ((flags&AIL_OPEN_DIGITAL_FORCE_PREFERENCE)==0))
       {
         OutMilesMutex();
         AIL_API_waveOutClose(result);
         InMilesMutex();
         ds_emulated_ok = 1;
         goto trywaveout;
       }
       #endif
     }
     else
     {
       #ifdef IS_WIN32
       if ((flags&AIL_OPEN_DIGITAL_FORCE_PREFERENCE)==0)
       {
        trywaveout:
         AIL_set_preference(DIG_USE_WAVEOUT,TRUE);
        tryagain:
         if (AIL_API_waveOutOpen(&result,0,(LPWAVEFORMAT)&wf))
         {
           // if waveout failed, but emulated suceeded, go back to directsound
           if ( ds_emulated_ok )
           {
             ds_emulated_ok=0;
             AIL_set_preference(DIG_USE_WAVEOUT,FALSE);
             goto tryagain;
           }
           result=0;
         }
       }
       else
       #endif
         result=0;
     }
   }

   #else

   #ifdef IS_DOS

   if ((flags&AIL_OPEN_DIGITAL_FORCE_PREFERENCE)==0)
   {
     if (frequency)
       AIL_set_preference(DIG_HARDWARE_SAMPLE_RATE, frequency );
     if (bits)
       AIL_set_preference(DIG_USE_16_BITS, (bits>8)?TRUE:FALSE );
     if (channels)
       AIL_set_preference(DIG_USE_STEREO, (channels>1)?TRUE:FALSE );
   }
   if (AIL_install_DIG_INI(&result)!=AIL_INIT_SUCCESS)
     result=0;

   #else

   // Mac, PS3, Xbox, Xenon, Linux, Wii

   result = AIL_API_open_digital_driver( frequency, bits, channels, flags );

#ifdef IS_WII
   if ( result )
   {
     if ( result->logical_channels_per_sample == 3 )
     {
       if ( ! MC_open_output_filter("Dolby Surround Matrix Filter", result, TRUE) )
         goto wii_flt_err;
     } 
     else if ( result->logical_channels_per_sample == 7 )
     {
       if ( ! MC_open_output_filter("SRS Circle Surround 6.1 Matrix Filter", result, TRUE) )
       {
        wii_flt_err:
         AIL_API_close_digital_driver(result);
         result = 0;
       }
     } 
   }      
#endif


   #endif
   #endif

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   if (result)
      {
      result->channel_spec = (MSS_MC_SPEC)channels;
      }

   return result;
}

DXDEF void AILEXPORT AIL_close_digital_driver( HDIGDRIVER dig )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_close_digital_driver(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   #ifdef IS_WINDOWS
   dsailcall1(AIL,waveOutClose,void,dig);
   #else
   #ifdef IS_DOS
   AIL_API_uninstall_DIG_driver(dig)
   #else
   AIL_API_close_digital_driver(dig);
   #endif
   #endif

   OUT_AIL_NM;

   END;
}

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)

//############################################################################

DXDEF
S32         AILEXPORT AIL_digital_handle_release      (HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_handle_release(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=dsailcall1(AIL,digital_handle_release,S32,dig);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_digital_handle_reacquire    (HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_handle_reacquire(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=dsailcall1(AIL,digital_handle_reacquire,S32,dig);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF HDIGINPUT AILEXPORT AIL_open_input          (AIL_INPUT_INFO FAR *info)
{
   HDIGINPUT result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_input(" PTRFMT ")" NL,info);
      }

   IN_AIL;

   result=ailcall1(AIL,open_input,HDIGINPUT,info);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF void      AILEXPORT AIL_close_input         (HDIGINPUT         dig)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_close_input(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   ailcall1(AIL,close_input,void,dig);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF AIL_INPUT_INFO FAR * AILEXPORT AIL_get_input_info (HDIGINPUT         dig)
{
   AIL_INPUT_INFO FAR *result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_get_input_info(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=ailcall1(AIL,get_input_info,AIL_INPUT_INFO FAR *,dig);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32       AILEXPORT AIL_set_input_state     (HDIGINPUT         dig, //)
                                                   S32               enable)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_input_state(" PTRFMT ",%ld)" NL,dig,enable);
      }

   IN_AIL;

   result=ailcall2(AIL,set_input_state,S32,dig,enable);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

#endif

//############################################################################

DXDEF
S32         AILEXPORT AIL_digital_CPU_percent(HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_CPU_percent(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,digital_CPU_percent,S32,dig);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_background_CPU_percent(void)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_background_CPU_percent()" NL);
      }

   IN_AIL_NM;

#ifdef IS_DOS
   result = 0;
#else
   result=ailcall0(AIL,background_CPU_percent,S32);
#endif

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_digital_latency(HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_latency(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,digital_latency,S32,dig);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
HSAMPLE      AILEXPORT AIL_allocate_sample_handle (HDIGDRIVER dig)

{
   HSAMPLE result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_allocate_sample_handle(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=dsailcall1(AIL,allocate_sample_handle,HSAMPLE,dig);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_release_sample_handle (HSAMPLE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_release_sample_handle(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   dsailcall1(AIL,release_sample_handle,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_init_sample        (HSAMPLE S, //)
                                               S32     format,
                                               U32     flags)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_init_sample(" PTRFMT ",%ld,%lu)" NL,S,format,flags);
      }

   IN_AIL;

   dsailcall3(AIL,init_sample,void,S,format,flags);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32        AILEXPORT AIL_set_sample_file        (HSAMPLE   S, //)
                                              void const FAR *file_image,
                                              S32       block)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_file(" PTRFMT "," PTRFMT ",%ld)" NL,S,file_image,block);
      }

   IN_AIL;

   result=ailcall3(AIL,set_sample_file,S32,S,file_image,block);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32        AILEXPORT AIL_set_sample_info     (HSAMPLE   S, //)
                                              AILSOUNDINFO const FAR *info )
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_info(" PTRFMT "," PTRFMT ")" NL,S,info);
      }

   IN_AIL;

   result=ailcall2(AIL,set_sample_info,S32,S,info);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32          AILEXPORT AIL_set_named_sample_file (HSAMPLE   S, //)
                                                  C8   const FAR *file_type_suffix,
                                                  void const FAR *file_image,
                                                  U32       file_size,
                                                  S32       block)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_named_sample_file(" PTRFMT ",%s," PTRFMT ",%ld,%ld)" NL,
         S,file_type_suffix,file_image,file_size,block);
      }

   IN_AIL;

   result=ailcall5(AIL,set_named_sample_file,U32,S,file_type_suffix,file_image,file_size,block);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void        AILEXPORT AIL_save_sample_attributes (HSAMPLE   S, //)
                                                  HSATTRIBS D)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_save_sample_attributes(" PTRFMT "," PTRFMT ")" NL,S,D);
      }

   IN_AIL;

   ailcall2(AIL,save_sample_attributes,void,S,D);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
U32        AILEXPORT AIL_load_sample_attributes (HSAMPLE   S, //)
                                                 HSATTRIBS D)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_load_sample_attributes(" PTRFMT "," PTRFMT ")" NL,S,D);
      }

   IN_AIL;

   result=ailcall2(AIL,load_sample_attributes,S32,S,D);

   OUT_AIL;

   RESULT
      {
      outresint(result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
HPROVIDER    AILEXPORT AIL_set_sample_processor  (HSAMPLE     S, //)
                                                SAMPLESTAGE pipeline_stage,
                                                HPROVIDER   provider)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_processor(" PTRFMT ",%ld," HPFMT ")" NL,
         S,pipeline_stage,provider);
      }

   IN_AIL;

   result=ailcall3(AIL,set_sample_processor,HPROVIDER,S,pipeline_stage,provider);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
HPROVIDER    AILEXPORT AIL_sample_processor  (HSAMPLE     S, //)
                                            SAMPLESTAGE pipeline_stage)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_processor(" PTRFMT ",%ld)" NL,
         S,pipeline_stage);
      }

   IN_AIL;

   result=ailcall2(AIL,sample_processor,HPROVIDER,S,pipeline_stage);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
HPROVIDER    AILEXPORT AIL_set_digital_driver_processor  (HDIGDRIVER  dig, //)
                                                        DIGDRVSTAGE pipeline_stage,
                                                        HPROVIDER   provider)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_digital_driver_processor(" PTRFMT ",%ld," HPFMT ")" NL,
         dig,pipeline_stage,provider);
      }

   IN_AIL;

   result=ailcall3(AIL,set_digital_driver_processor,HPROVIDER,dig,pipeline_stage,provider);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
HPROVIDER  AILEXPORT AIL_digital_driver_processor  (HDIGDRIVER  dig, //)
                                                    DIGDRVSTAGE pipeline_stage)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_driver_processor(" PTRFMT ",%ld)" NL,
         dig,pipeline_stage);
      }

   IN_AIL;

   result=ailcall2(AIL,digital_driver_processor,HPROVIDER,dig,pipeline_stage);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_address    (HSAMPLE S, //)
                                               void const    FAR *start,
                                               U32     len)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_address(" PTRFMT "," PTRFMT ",%lu)" NL,S,start,len);
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_address,void,S,start,len);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_adpcm_block_size(HSAMPLE S, //)
                                                       U32 blocksize)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_adpcm_block_size(" PTRFMT ",%lu)" NL,S,blocksize);
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_adpcm_block_size,void,S,blocksize);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_start_sample          (HSAMPLE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_start_sample(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   dsailcall1(AIL,start_sample,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_stop_sample           (HSAMPLE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stop_sample(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   dsailcall1(AIL,stop_sample,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_resume_sample         (HSAMPLE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_resume_sample(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   dsailcall1(AIL,resume_sample,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_end_sample            (HSAMPLE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_end_sample(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   dsailcall1(AIL,end_sample,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_playback_rate//()
                                             (HSAMPLE S,
                                              S32     playback_rate)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_playback_rate(" PTRFMT ",%ld)" NL,S,playback_rate);
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_playback_rate,void,S,playback_rate);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
void         AILEXPORT AIL_set_sample_channel_levels (HSAMPLE S, //)
                                                      F32 FAR *level_array,
                                                      S32  n_levels)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_channel_levels(" PTRFMT "," PTRFMT ",%d)" NL,S,level_array,n_levels);
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_channel_levels,void,S,level_array,n_levels);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
F32 FAR *    AILEXPORT AIL_sample_channel_levels (HSAMPLE  S, //)
                                                  S32 FAR *n_levels)
{                         
   F32 FAR *result = NULL;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_channel_levels(" PTRFMT "," PTRFMT ")" NL,S,n_levels);
      }

   IN_AIL;

   result = dsailcall2(AIL,sample_channel_levels,F32 FAR *,S,n_levels);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  
void         AILEXPORT AIL_set_speaker_configuration (HDIGDRIVER dig, //)
                                                      MSSVECTOR3D FAR *speaker_positions,
                                                      S32  n_channels,
                                                      F32  falloff_power)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_speaker_configuration(" PTRFMT "," PTRFMT ",%ld,%s)" NL,dig,speaker_positions,n_channels,FTS(falloff_power));
      }

   IN_AIL;        

   dsailcall4(AIL,set_speaker_configuration,void,dig,speaker_positions,n_channels,falloff_power);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
MSSVECTOR3D FAR *    AILEXPORT AIL_speaker_configuration (HDIGDRIVER       dig, //)
                                                          S32         FAR *n_physical_channels,
                                                          S32         FAR *n_logical_channels,
                                                          F32         FAR *falloff_power,
                                                          MSS_MC_SPEC FAR *channel_spec)
{
   MSSVECTOR3D FAR *result = NULL;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_speaker_configuration(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,dig,n_physical_channels,n_logical_channels,falloff_power,channel_spec);
      }

   IN_AIL;

   result = dsailcall5(AIL,speaker_configuration,MSSVECTOR3D FAR *,dig,n_physical_channels,n_logical_channels,falloff_power,channel_spec);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  
void AILEXPORT AIL_set_listener_relative_receiver_array (HDIGDRIVER               dig, //)
                                                         MSS_RECEIVER_LIST FAR *array,
                                                         S32                      n_receivers)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_listener_relative_receiver_array(" PTRFMT "," PTRFMT ",%d)" NL,dig,array,n_receivers);
      }

   IN_AIL;

   dsailcall3(AIL,set_listener_relative_receiver_array,void,dig,array,n_receivers);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
MSS_RECEIVER_LIST FAR *    AILEXPORT AIL_listener_relative_receiver_array (HDIGDRIVER  dig, //)
                                                                           S32 FAR    *n_receivers)
{                                     
   MSS_RECEIVER_LIST FAR *result = NULL;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_listener_relative_receiver_array(" PTRFMT "," PTRFMT ")" NL,dig,n_receivers);
      }

   IN_AIL;

   result = dsailcall2(AIL,listener_relative_receiver_array,MSS_RECEIVER_LIST FAR *,dig,n_receivers);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

#ifndef IS_XBOX

//############################################################################
DXDEF  
void  AILEXPORT AIL_set_speaker_reverb_levels (HDIGDRIVER dig, //)
                                               F32       *wet_array,
                                               F32       *dry_array,
                                               S32        n_levels)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_speaker_reverb_levels(" PTRFMT "," PTRFMT "," PTRFMT ",%d)" NL,dig,wet_array,dry_array,n_levels);
      }

   IN_AIL;

   dsailcall4(AIL,set_speaker_reverb_levels,void,dig,wet_array,dry_array,n_levels);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
void    AILEXPORT AIL_speaker_reverb_levels (HDIGDRIVER     dig, //)
                                             S32 FAR       *n_levels,
                                             F32 FAR * FAR *wet_array,
                                             F32 FAR * FAR *dry_array)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_speaker_reverb_levels(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,dig,n_levels,wet_array,dry_array);
      }

   IN_AIL;

   dsailcall4(AIL,speaker_reverb_levels,void,dig,n_levels,wet_array,dry_array);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  
S32   AILEXPORT AIL_calculate_3D_channel_levels      (HDIGDRIVER       dig, //)
                                                      F32         FAR *channel_levels,
                                                      MSSVECTOR3D FAR *src_pos,
                                                      MSSVECTOR3D FAR *src_face,
                                                      MSSVECTOR3D FAR *src_up,
                                                      F32              src_inner_angle,
                                                      F32              src_outer_angle,
                                                      F32              src_outer_volume,
                                                      F32              src_max_dist,
                                                      F32              src_min_dist,
                                                      MSSVECTOR3D FAR *listen_pos, 
                                                      MSSVECTOR3D FAR *listen_face,
                                                      MSSVECTOR3D FAR *listen_up,  
                                                      F32              rolloff_factor,
                                                      MSSVECTOR3D FAR *doppler_velocity,
                                                      F32             *doppler_shift)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_calculate_3D_channel_levels(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ",%s,%s,%s,%s,%s," PTRFMT "," PTRFMT "," PTRFMT ",%s," PTRFMT ",%s)" NL,
         dig,
         channel_levels,    
         src_pos,           
         src_face,          
         src_up,            
         FTS(src_inner_angle),   
         FTS(src_outer_angle),   
         FTS(src_outer_volume),  
         FTS(src_max_dist),      
         FTS(src_min_dist),      
         listen_pos,        
         listen_face,       
         listen_up,         
         FTS(rolloff_factor),
         doppler_velocity,
         doppler_shift);
      }

   IN_AIL;

   result = dsailcall16(AIL,calculate_3D_channel_levels,S32,
         dig,
         channel_levels,    
         src_pos,           
         src_face,          
         src_up,            
         src_inner_angle,   
         src_outer_angle,   
         src_outer_volume,  
         src_max_dist,      
         src_min_dist,      
         listen_pos,        
         listen_face,       
         listen_up,         
         rolloff_factor,
         doppler_velocity,
         doppler_shift);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

#endif

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_volume_pan  (HSAMPLE S, //)
                                                   F32     volume,
                                                   F32     pan)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_volume_pan(" PTRFMT ",%s,%s)" NL,S,FTS(volume),FTS(pan));
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_volume_pan,void,S,volume,pan);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_51_volume_pan  (HSAMPLE S, //)
                                                      F32     volume,
                                                      F32     pan,
                                                      F32     fb_pan,
                                                      F32     center_level,
                                                      F32     sub_level )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_51_volume_pan(" PTRFMT ",%s,%s,%s,%s,%s)" NL,S,FTS(volume),FTS(pan),FTS(fb_pan),FTS(center_level),FTS(sub_level));
      }

   IN_AIL;

   dsailcall6(AIL,set_sample_51_volume_pan,void,S,volume,pan,fb_pan,center_level,sub_level);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_sample_51_volume_pan  (HSAMPLE   S, //)
                                                  F32 FAR * volume,
                                                  F32 FAR * pan,
                                                  F32 FAR * fb_pan,
                                                  F32 FAR * center_level,
                                                  F32 FAR * sub_level )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_51_volume_pan(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,volume,pan,fb_pan,center_level,sub_level);
      }

   IN_AIL_NM;

   dsailcall6(AIL,sample_51_volume_pan,void,S,volume,pan,fb_pan,center_level,sub_level);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_51_volume_levels (HSAMPLE S, //)
                                                        F32     f_left_level,
                                                        F32     f_right_level,
                                                        F32     b_left_level,
                                                        F32     b_right_level,
                                                        F32     center_level,
                                                        F32     sub_level )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_51_volume_levels(" PTRFMT ",%s,%s,%s,%s,%s)" NL,S,FTS(f_left_level),FTS(f_right_level),FTS(b_left_level),FTS(b_right_level),FTS(center_level),FTS(sub_level));
      }

   IN_AIL;

   dsailcall7(AIL,set_sample_51_volume_levels,void,S,f_left_level,f_right_level,b_left_level,b_right_level,center_level,sub_level);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_sample_51_volume_levels (HSAMPLE   S, //)
                                                    F32 FAR * f_left_level,
                                                    F32 FAR * f_right_level,
                                                    F32 FAR * b_left_level,
                                                    F32 FAR * b_right_level,
                                                    F32 FAR * center_level,
                                                    F32 FAR * sub_level )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_51_volume_levels(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,f_left_level,f_right_level,b_left_level,b_right_level,center_level,sub_level);
      }

   IN_AIL_NM;

   dsailcall7(AIL,sample_51_volume_levels,void,S,f_left_level,f_right_level,b_left_level,b_right_level,center_level,sub_level);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_volume_levels  (HSAMPLE S, //)
                                                      F32     left_level,
                                                      F32     right_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_volume_levels(" PTRFMT ",%s,%s)" NL,S,FTS(left_level),FTS(right_level));
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_volume_levels,void,S,left_level,right_level);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_reverb_levels  (HSAMPLE S, //)
                                                      F32     dry_level,
                                                      F32     wet_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_reverb_levels(" PTRFMT ",%s,%s)" NL,S,FTS(dry_level),FTS(wet_level));
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_reverb_levels,void,S,dry_level,wet_level);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_low_pass_cut_off(HSAMPLE S, //)
                                                       F32     cut_off)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_low_pass_cut_off(" PTRFMT ",%s)" NL,S,FTS(cut_off));
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_low_pass_cut_off,void,S,cut_off);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_loop_count (HSAMPLE S, //)
                                               S32     loop_count)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_loop_count(" PTRFMT ",%ld)" NL,S,loop_count);
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_loop_count,void,S,loop_count);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
void         AILEXPORT AIL_set_sample_loop_block (HSAMPLE S,
                                                     S32     loop_start_offset,
                                                     S32     loop_end_offset)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_loop_block(" PTRFMT ",%ld,%ld)" NL,S,loop_start_offset,loop_end_offset);
      }

   IN_AIL;

   dsailcall3(AIL,set_sample_loop_block,void,S,loop_start_offset,loop_end_offset);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF
S32        AILEXPORT AIL_sample_loop_block   (HSAMPLE S, //)
                                              S32    *loop_start_offset,
                                              S32    *loop_end_offset)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_loop_block(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          S,loop_start_offset,loop_end_offset);
      }

   IN_AIL_NM;

   result = AIL_API_sample_loop_block(S,loop_start_offset,loop_end_offset);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
U32        AILEXPORT AIL_sample_status         (HSAMPLE S)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_status(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_status,U32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_sample_playback_rate  (HSAMPLE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_playback_rate(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_playback_rate,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_sample_volume_pan         (HSAMPLE S,F32 FAR*volume, F32 FAR* pan)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_volume_pan(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,volume,pan);
      }

   IN_AIL_NM;

   dsailcall3(AIL,sample_volume_pan,void,S,volume,pan);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_sample_volume_levels  (HSAMPLE S, //)
                                                  F32 FAR * left_level,
                                                  F32 FAR * right_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_volume_levels(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,left_level,right_level);
      }

   IN_AIL_NM;

   dsailcall3(AIL,sample_volume_levels,void,S,left_level,right_level);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_sample_reverb_levels  (HSAMPLE S, //)
                                                  F32 FAR * dry_level,
                                                  F32 FAR * wet_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_reverb_levels(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,dry_level,wet_level);
      }

   IN_AIL_NM;

   dsailcall3(AIL,sample_reverb_levels,void,S,dry_level,wet_level);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
F32         AILEXPORT AIL_sample_low_pass_cut_off(HSAMPLE S )
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_low_pass_cut_off(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_low_pass_cut_off,F32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat(result);
      }

   END;

   return( result );
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_sample_loop_count     (HSAMPLE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_loop_count(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_loop_count,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_set_digital_master_volume_level
                                             (HDIGDRIVER dig,
                                              F32        master_volume)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_digital_master_volume_level(" PTRFMT ",%s)" NL,dig,FTS(master_volume));
      }

   IN_AIL;

   dsailcall2(AIL,set_digital_master_volume_level,void,dig,master_volume);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
F32     AILEXPORT AIL_digital_master_volume_level   (HDIGDRIVER dig)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_master_volume_level(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=dsailcall1(AIL,digital_master_volume_level,F32,dig);

   OUT_AIL;

   RESULT
      {
      outresfloat((F32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_digital_master_reverb_levels  (HDIGDRIVER dig, //)
                                                              F32     dry_level,
                                                              F32     wet_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_digital_master_reverb_levels(" PTRFMT ",%s,%s)" NL,dig,FTS(dry_level),FTS(wet_level));
      }

   IN_AIL;

   dsailcall3(AIL,set_digital_master_reverb_levels,void,dig,dry_level,wet_level);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_digital_master_reverb_levels  (HDIGDRIVER dig, //)
                                                          F32 FAR * dry_level,
                                                          F32 FAR * wet_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_master_reverb_levels(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,dig,dry_level,wet_level);
      }

   IN_AIL;

   dsailcall3(AIL,digital_master_reverb_levels,void,dig,dry_level,wet_level);

   OUT_AIL;

   END;
}

#ifndef IS_XBOX

//############################################################################

DXDEF
void         AILEXPORT AIL_set_digital_master_reverb         (HDIGDRIVER dig, //)
                                                              F32     reverb_time,
                                                              F32     reverb_predelay,
                                                              F32     reverb_damping)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_digital_master_reverb(" PTRFMT ",%s,%s,%s)" NL,dig,FTS(reverb_time),FTS(reverb_predelay),FTS(reverb_damping));
      }

   IN_AIL_NM;

   dsailcall4(AIL,set_digital_master_reverb,void,dig,reverb_time,reverb_predelay,reverb_damping);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_digital_master_reverb (HDIGDRIVER dig, //)
                                                  F32 FAR * reverb_time,
                                                  F32 FAR * reverb_predelay,
                                                  F32 FAR * reverb_damping)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_master_reverb(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,dig,reverb_time,reverb_predelay,reverb_damping);
      }

   IN_AIL_NM;

   dsailcall4(AIL,digital_master_reverb,void,dig,reverb_time,reverb_predelay,reverb_damping);

   OUT_AIL_NM;

   END;
}

#endif

//############################################################################

DXDEF
S32     AILEXPORT AIL_minimum_sample_buffer_size  (HDIGDRIVER dig, //)
                                                S32        playback_rate,
                                                S32        format)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_minimum_sample_buffer_size(" PTRFMT ",%ld,%ld)" NL,dig,
         playback_rate,format);
      }

   IN_AIL;

   result=dsailcall3(AIL,minimum_sample_buffer_size,S32,dig,playback_rate,format);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_sample_buffer_ready       (HSAMPLE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_buffer_ready(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_buffer_ready,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_load_sample_buffer        (HSAMPLE S, //)
                                               U32     buff_num,
                                               void const FAR *buffer,
                                               U32     len)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_load_sample_buffer(" PTRFMT ",%ld," PTRFMT ",%lu)" NL,
         S,buff_num,buffer,len);
      }

   IN_AIL;

   dsailcall4(AIL,load_sample_buffer,void,S,buff_num,buffer,len);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_sample_buffer_info      (HSAMPLE S, //)
            U32     FAR *pos0,
            U32     FAR *len0,
            U32     FAR *pos1,
            U32     FAR *len1)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_buffer_info(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
                 S,pos0,len0,pos1,len1);
      }

   IN_AIL;

   result=dsailcall5(AIL,sample_buffer_info,S32,S,pos0,len0,pos1,len1);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
U32    AILEXPORT AIL_sample_granularity        (HSAMPLE S)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_granularity(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_granularity,U32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_set_sample_position       (HSAMPLE S, //)
                                               U32     pos)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_position(" PTRFMT ",%lu)" NL,S,pos);
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_position,void,S,pos);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
U32    AILEXPORT AIL_sample_position           (HSAMPLE S)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_position(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=dsailcall1(AIL,sample_position,U32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILSAMPLECB AILEXPORT AIL_register_SOB_callback     (HSAMPLE S, //)
                                                    AILSAMPLECB SOB)
{
   AILSAMPLECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_SOB_callback(" PTRFMT "," PTRFMT ")" NL,S,SOB);
      }

   IN_AIL;

   result=dsailcall2(AIL,register_SOB_callback,AILSAMPLECB,S,SOB);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILSAMPLECB AILEXPORT AIL_register_EOB_callback     (HSAMPLE S, //)
                                                    AILSAMPLECB EOB)
{
   AILSAMPLECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_EOB_callback(" PTRFMT "," PTRFMT ")" NL,S,EOB);
      }

   IN_AIL;

   result=dsailcall2(AIL,register_EOB_callback,AILSAMPLECB,S,EOB);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILSAMPLECB AILEXPORT AIL_register_EOS_callback     (HSAMPLE S, //)
                                                    AILSAMPLECB EOS)
{
   AILSAMPLECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_EOS_callback(" PTRFMT "," PTRFMT ")" NL,S,EOS);
      }

   IN_AIL;

   result=dsailcall2(AIL,register_EOS_callback,AILSAMPLECB,S,EOS);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILFALLOFFCB AILEXPORT AIL_register_falloff_function_callback     (HSAMPLE S, //)
                                                                  AILFALLOFFCB falloff_cb)
{
   AILFALLOFFCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_falloff_function_callback(" PTRFMT "," PTRFMT ")" NL,S,falloff_cb);
      }

   IN_AIL;

   result=dsailcall2(AIL,register_falloff_function_callback,AILFALLOFFCB,S,falloff_cb);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_set_sample_user_data      (HSAMPLE S, //)
                                                  U32     index,
                                                  SINTa   value)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_user_data(" PTRFMT ",%ld," SINTFMT ")" NL,S,index,value);
      }

   IN_AIL_NM;

   dsailcall3(AIL,set_sample_user_data,void,S,index,value);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
SINTa   AILEXPORT AIL_sample_user_data          (HSAMPLE S, //)
                                                 U32     index)
{
   SINTa result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_user_data(" PTRFMT ",%lu)" NL,S,index);
      }

   IN_AIL_NM;

   result=dsailcall2(AIL,sample_user_data,SINTa,S,index);

   OUT_AIL_NM;

   RESULT
      {
      outresuint((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_active_sample_count       (HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_active_sample_count(" PTRFMT ")" NL,dig);
      }

   IN_AIL;

   result=dsailcall1(AIL,active_sample_count,S32,dig);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

void     AILEXPORT AIL_digital_configuration     (HDIGDRIVER dig,
                                               S32   FAR *rate,
                                               S32   FAR *format,
                                               char  FAR *config)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_configuration(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         dig,rate,format,config);
      }

   IN_AIL;

   dsailcall4(AIL,digital_configuration,void,dig,rate,format,config);

   OUT_AIL;

   END;
}


#if defined(IS_WIN32) || defined(IS_XBOX)

//############################################################################

DXDEF
void     AILEXPORT AIL_get_DirectSound_info (HSAMPLE              S,
                                          LPVOID               *lplpDS,
                                          LPVOID               *lplpDSB)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_get_DirectSound_info(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         S,lplpDS,lplpDSB);
      }

   IN_AIL;

#ifdef IS_WINDOWS
   if (AIL_preference[DIG_USE_WAVEOUT]==NO)
#endif
      {
      AIL_API_get_DirectSound_info(S,lplpDS,lplpDSB);
      }

   OUT_AIL;

   END;
}

#endif

#ifdef IS_WINDOWS

//############################################################################

DXDEF
S32      AILEXPORT  AIL_set_DirectSound_HWND(HDIGDRIVER dig, HWND wnd)
{
   S32 result=0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_DirectSound_HWND(" PTRFMT ", " PTRFMT ")" NL,dig,wnd);
      }

   IN_AIL;

   if (AIL_preference[DIG_USE_WAVEOUT]==NO)
      {
      result=AIL_API_set_DirectSound_HWND(dig,wnd);
      }

   OUT_AIL;

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

#endif


//############################################################################

DXDEF HMDIDRIVER AILEXPORT AIL_open_XMIDI_driver( U32 flags )
{
   HMDIDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_XMIDI_driver(0x%lX)" NL,
                                                       flags);
      }

   IN_AIL;

   #ifdef IS_WINDOWS

   if (AIL_midiOutOpen(&result,0,(flags&AIL_OPEN_XMIDI_NULL_DRIVER)?(U32)MIDI_NULL_DRIVER:(U32)MIDI_MAPPER))
     result=0;

   #else

   #ifdef IS_DOS

   if (flags&AIL_OPEN_XMIDI_NULL_DRIVER)
   {
     result=AIL_install_MDI_driver_file("NULL.MDI",0);
   }
   else
   {
     if (AIL_install_MDI_INI(&result)!=AIL_INIT_SUCCESS)
       result=0;
   }

   #else

   result = AIL_API_open_XMIDI_driver( flags );

   #endif

   #endif

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}


DXDEF void AILEXPORT AIL_close_XMIDI_driver( HMDIDRIVER mdi )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_close_XMIDI_driver(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   #ifdef IS_WINDOWS
   dsailcall1(AIL,midiOutClose,void,mdi);
   #else
   #ifdef IS_DOS
   AIL_API_uninstall_MDI_driver(mdi)
   #else
   AIL_API_close_XMIDI_driver( mdi );
   #endif
   #endif

   OUT_AIL;

   END;
}


#ifdef IS_WINDOWS

DXDEF
S32          AILEXPORT AIL_midiOutOpen      (HMDIDRIVER FAR *drvr,   //)
                                             LPHMIDIOUT FAR *lphMidiOut,
                                             S32         dwDeviceID)

{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_midiOutOpen(" PTRFMT ", " PTRFMT ", %d)" NL,drvr,lphMidiOut,dwDeviceID);
      }

   IN_AIL;

   result=ailcall3(AIL,midiOutOpen,S32,drvr,lphMidiOut,(U32)dwDeviceID);

   OUT_AIL;

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_midiOutClose      (HMDIDRIVER mdi)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_midiOutClose(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   ailcall1(AIL,midiOutClose,void,mdi);

   OUT_AIL;

   END;
}

#endif

//############################################################################

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)

DXDEF
S32         AILEXPORT AIL_MIDI_handle_release     (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_MIDI_handle_release(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,MIDI_handle_release,S32,mdi);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_MIDI_handle_reacquire     (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_MIDI_handle_reacquire(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,MIDI_handle_reacquire,S32,mdi);

   OUT_AIL;

   RESULT
      {
      INDENT;

      AIL_fprintf(AIL_debugfile,"Result = %lu" NL,result);
      }

   END;

   return result;
}

#endif

//############################################################################

DXDEF
HSEQUENCE    AILEXPORT AIL_allocate_sequence_handle//()
                                             (HMDIDRIVER mdi)
{
   HSEQUENCE result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_allocate_sequence_handle(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,allocate_sequence_handle,HSEQUENCE,mdi);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_release_sequence_handle//()
                                             (HSEQUENCE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_release_sequence_handle(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   ailcall1(AIL,release_sequence_handle,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_init_sequence         (HSEQUENCE S, //)
                                              void const FAR *start,
                                              S32       sequence_num)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_init_sequence(" PTRFMT "," PTRFMT ",%ld)" NL,S,start,sequence_num);
      }

   IN_AIL;

   result=ailcall3(AIL,init_sequence,S32,S,start,sequence_num);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_start_sequence        (HSEQUENCE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_start_sequence(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   ailcall1(AIL,start_sequence,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_stop_sequence         (HSEQUENCE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stop_sequence(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   ailcall1(AIL,stop_sequence,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_resume_sequence       (HSEQUENCE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_resume_sequence(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   ailcall1(AIL,resume_sequence,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_end_sequence          (HSEQUENCE S)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_end_sequence(" PTRFMT ")" NL,S);
      }

   IN_AIL;

   ailcall1(AIL,end_sequence,void,S);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sequence_tempo    (HSEQUENCE S, //)
                                               S32       tempo,
                                               S32       milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sequence_tempo(" PTRFMT ",%ld,%ld)" NL,S,tempo,
         milliseconds);
      }

   IN_AIL;

   ailcall3(AIL,set_sequence_tempo,void,S,tempo,milliseconds);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sequence_volume   (HSEQUENCE S, //)
                                               S32       volume,
                                               S32       milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sequence_volume(" PTRFMT ",%ld,%ld)" NL,S,volume,
         milliseconds);
      }

   IN_AIL_NM;

   ailcall3(AIL,set_sequence_volume,void,S,volume,milliseconds);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void         AILEXPORT AIL_set_sequence_loop_count (HSEQUENCE S, //)
                                                 S32       loop_count)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sequence_loop_count(" PTRFMT ",%ld)" NL,S,loop_count);
      }

   IN_AIL_NM;

   ailcall2(AIL,set_sequence_loop_count,void,S,loop_count);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
U32        AILEXPORT AIL_sequence_status       (HSEQUENCE S)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_status(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,sequence_status,U32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_sequence_tempo        (HSEQUENCE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_tempo(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,sequence_tempo,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_sequence_volume       (HSEQUENCE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_volume(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,sequence_volume,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32         AILEXPORT AIL_sequence_loop_count   (HSEQUENCE S)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_loop_count(" PTRFMT ")" NL,S);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,sequence_loop_count,S32,S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_set_XMIDI_master_volume
                                             (HMDIDRIVER mdi,
                                              S32        master_volume)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_XMIDI_master_volume(" PTRFMT ",%d)" NL,mdi,master_volume);
      }

   IN_AIL;

   ailcall2(AIL,set_XMIDI_master_volume,void,mdi,master_volume);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_XMIDI_master_volume   (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_XMIDI_master_volume(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,XMIDI_master_volume,S32,mdi);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_active_sequence_count     (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_active_sequence_count(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,active_sequence_count,S32,mdi);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_controller_value          (HSEQUENCE S, //)
                                              S32       channel,
                                              S32       controller_num)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_controller_value(" PTRFMT ",%ld,%ld)" NL,S,channel,
         controller_num);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,controller_value,S32,S,channel,controller_num);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_channel_notes             (HSEQUENCE S, //)
                                              S32       channel)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_channel_notes(" PTRFMT ",%ld)" NL,S,channel);
      }

   IN_AIL;

   result=ailcall2(AIL,channel_notes,S32,S,channel);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_sequence_position         (HSEQUENCE S, //)
                                                  S32       FAR *beat,
                                                  S32       FAR *measure)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_position(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,beat,measure);
      }

   IN_AIL_NM;

   ailcall3(AIL,sequence_position,void,S,beat,measure);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_branch_index              (HSEQUENCE S, //)
                                               U32       marker)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_branch_index(" PTRFMT ",%lu)" NL,S,marker);
      }

   IN_AIL;

   ailcall2(AIL,branch_index,void,S,marker);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
AILPREFIXCB AILEXPORT AIL_register_prefix_callback  (HSEQUENCE S, //)
                                               AILPREFIXCB   callback)
{
   AILPREFIXCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_prefix_callback(" PTRFMT "," PTRFMT ")" NL,
         S,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_prefix_callback,AILPREFIXCB,S,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILTRIGGERCB AILEXPORT AIL_register_trigger_callback (HSEQUENCE S, //)
                                              AILTRIGGERCB   callback)
{
   AILTRIGGERCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_trigger_callback(" PTRFMT "," PTRFMT ")" NL,
         S,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_trigger_callback,AILTRIGGERCB,S,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILSEQUENCECB AILEXPORT AIL_register_sequence_callback(HSEQUENCE S, //)
                                              AILSEQUENCECB   callback)
{
   AILSEQUENCECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_sequence_callback(" PTRFMT "," PTRFMT ")" NL,
         S,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_sequence_callback,AILSEQUENCECB,S,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILBEATCB AILEXPORT AIL_register_beat_callback(HSEQUENCE S, //)
                                              AILBEATCB   callback)
{
   AILBEATCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_beat_callback(" PTRFMT "," PTRFMT ")" NL,
         S,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_beat_callback,AILBEATCB,S,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILEVENTCB AILEXPORT AIL_register_event_callback   (HMDIDRIVER mdi, //)
                                                    AILEVENTCB callback)
{
   AILEVENTCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_event_callback(" PTRFMT "," PTRFMT ")" NL,
         mdi,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_event_callback,AILEVENTCB,mdi,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
AILTIMBRECB AILEXPORT AIL_register_timbre_callback  (HMDIDRIVER mdi, //)
                                                     AILTIMBRECB callback)
{
   AILTIMBRECB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_timbre_callback(" PTRFMT "," PTRFMT ")" NL,
         mdi, callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_timbre_callback,AILTIMBRECB,mdi,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_set_sequence_user_data    (HSEQUENCE S, //)
                                                  U32       index,
                                                  SINTa     value)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sequence_user_data(" PTRFMT ",%ld," SINTFMT ")" NL,S,index,value);
      }

   IN_AIL_NM;

   ailcall3(AIL,set_sequence_user_data,void,S,index,value);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
SINTa   AILEXPORT AIL_sequence_user_data        (HSEQUENCE S, //)
                                                 U32       index)
{
   SINTa result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_user_data(" PTRFMT ",%lu)" NL,S,index);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,sequence_user_data,SINTa,S,index);

   OUT_AIL_NM;

   RESULT
      {
      outresuint((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_register_ICA_array        (HSEQUENCE   S,  //)
                                                  U8     FAR *array)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_ICA_array(" PTRFMT "," PTRFMT ")" NL,S,array);
      }

   IN_AIL;

   ailcall2(AIL,register_ICA_array,void,S,array);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_lock_channel              (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_lock_channel(" PTRFMT ")" NL,mdi);
      }

   IN_AIL;

   result=ailcall1(AIL,lock_channel,S32,mdi);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_release_channel           (HMDIDRIVER mdi, //)
                                               S32        channel)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_release_channel(" PTRFMT ",%ld)" NL,mdi,channel);
      }

   IN_AIL;

   ailcall2(AIL,release_channel,void,mdi,channel);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_map_sequence_channel      (HSEQUENCE S, //)
                                               S32       seq_channel,
                                               S32       new_channel)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_map_sequence_channel(" PTRFMT ",%ld,%ld)" NL,
         S,seq_channel,new_channel);
      }

   IN_AIL;

   ailcall3(AIL,map_sequence_channel,void,S,seq_channel,new_channel);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32     AILEXPORT AIL_true_sequence_channel     (HSEQUENCE S, //)
                                              S32       seq_channel)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_true_sequence_channel(" PTRFMT ",%ld)" NL,
         S,seq_channel);
      }

   IN_AIL;

   result=ailcall2(AIL,true_sequence_channel,S32,S,seq_channel);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_send_channel_voice_message(HMDIDRIVER mdi, //)
                                               HSEQUENCE  S,
                                               S32        status,
                                               S32        data_1,
                                               S32        data_2)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_send_channel_voice_message(" PTRFMT "," PTRFMT ",0x%lX,0x%lX,0x%lX)" NL,
         mdi,S,status,data_1,data_2);
      }

   IN_AIL;

   ailcall5(AIL,send_channel_voice_message,void,mdi,S,status,data_1,data_2);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_send_sysex_message        (HMDIDRIVER mdi, //)
                                               void const FAR *buffer)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_send_sysex_message(" PTRFMT "," PTRFMT ")" NL,mdi,buffer);
      }

   IN_AIL;

   ailcall2(AIL,send_sysex_message,void,mdi,buffer);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
HWAVESYNTH    AILEXPORT AIL_create_wave_synthesizer   (HDIGDRIVER dig,  //)
                                                    HMDIDRIVER mdi,
                                                    void const FAR *wave_lib,
                                                    S32        polyphony)
{
   HWAVESYNTH result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_create_wave_synthesizer(" PTRFMT "," PTRFMT "," PTRFMT ",%ld)" NL,
         dig,mdi,wave_lib,polyphony);
      }

   IN_AIL;

   result=ailcall4(AIL,create_wave_synthesizer,HWAVESYNTH,dig,mdi,wave_lib,polyphony);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
void     AILEXPORT AIL_destroy_wave_synthesizer  (HWAVESYNTH W)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_destroy_wave_synthesizer(" PTRFMT ")" NL,W);
      }

   IN_AIL;

   ailcall1(AIL,destroy_wave_synthesizer,void,W);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32        AILEXPORT AIL_file_error  (void)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_error()" NL);
      }

   IN_AIL_NM;

   result=ailcall0(AIL,file_error,S32);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
S32        AILEXPORT AIL_file_size   (char const FAR   *filename)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_size(%s)" NL,filename);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,file_size,S32,filename);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void FAR * AILEXPORT AIL_file_read   (char const FAR   *filename,
                                          void FAR *dest)
{
   void FAR * result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_read(%s," PTRFMT ")" NL,filename,dest);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,file_read,void FAR*,filename,dest);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
S32        AILEXPORT AIL_file_write  (char const FAR   *filename,
                                          void const FAR *buf,
                                          U32       len)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_write(%s," PTRFMT ",%ld)" NL,filename,buf,len);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,file_write,S32,filename,buf,len);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
S32        AILEXPORT AIL_WAV_file_write  (char const FAR   *filename,
                                          void const FAR *buf,
                                          U32       len,
                                          S32       rate,
                                          S32       format)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_WAV_file_write(%s," PTRFMT ",%ld,%ld,%ld)" NL,filename,buf,len,rate,format);
      }

   IN_AIL_NM;

   result=ailcall5(AIL,WAV_file_write,S32,filename,buf,len,rate,format);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


#ifdef ON_MAC_USE_FSS
//############################################################################

DXDEF
S32        AILEXPORT AIL_file_fss_size   (FSSpec const *spec)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_fss_size(" PTRFMT ")" NL,spec);
      }

   IN_AIL;

   result=ailcall1(AIL,file_fss_size,S32,spec);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void FAR * AILEXPORT AIL_file_fss_read   (FSSpec const *spec,
                                          void FAR *dest)
{
   void FAR * result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_fss_read(" PTRFMT "," PTRFMT ")" NL,spec,dest);
      }

   IN_AIL;

   result=ailcall2(AIL,file_fss_read,void FAR*,spec,dest);

   OUT_AIL;

   RESULT
      {
      outreshex((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
S32        AILEXPORT AIL_file_fss_write  (FSSpec const *spec,
                                          void const FAR *buf,
                                          U32       len)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_fss_write(" PTRFMT "," PTRFMT ",%ld)" NL,spec,buf,len);
      }

   IN_AIL;

   result=ailcall3(AIL,file_fss_write,S32,spec,buf,len);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}

//############################################################################

DXDEF
S32        AILEXPORT AIL_file_fss_attrib_write  (FSSpec const *spec,
                                          void FAR const *buf,
                                          U32       len,
                                          U32 creator,
                                          U32 type )
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_fss_attrib_write(" PTRFMT "," PTRFMT ",%ld,%ld,%ld)" NL,spec,buf,len,creator,type);
      }

   IN_AIL;

   result=ailcall5(AIL,file_fss_attrib_write,S32,spec,buf,len,creator,type);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
S32        AILEXPORT AIL_WAV_file_fss_write  (FSSpec const *spec,
                                          void const FAR *buf,
                                          U32       len,
                                          S32       rate,
                                          S32       format)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_WAV_file_fss_write(" PTRFMT "," PTRFMT ",%ld,%ld,%ld)" NL,spec,buf,len,rate,format);
      }

   IN_AIL;

   result=ailcall5(AIL,WAV_file_fss_write,S32,spec,buf,len,rate,format);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}
#endif

//############################################################################

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

DXDEF
void       AILEXPORT AIL_serve()
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_serve()" NL);
      }

   IN_AIL_NM;

   ailcall0(AIL,serve,void);

   OUT_AIL_NM;

   END;
}

#endif

#if !defined(IS_XBOX) && !defined(IS_XENON) && !defined(IS_PS2) && !defined(IS_PS3) && !defined(IS_WII)

//############################################################################

DXDEF
HREDBOOK AILEXPORT AIL_redbook_open(U32 which)
{
   HREDBOOK result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_open(%ld)" NL,which);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_open,HREDBOOK,which);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
HREDBOOK AILEXPORT
#ifdef IS_MAC
AIL_redbook_open_volume(char const* drive)
#else
AIL_redbook_open_drive(S32 drive)
#endif
{
   HREDBOOK result;

   START
      {
      AIL_fprintf(AIL_debugfile,
#ifdef IS_MAC
      "AIL_redbook_open_volume(%s:)" NL
#else
      "AIL_redbook_open_drive(%c:)" NL
#endif
      ,drive);
      }

   IN_AIL_NM;

#ifdef IS_MAC
   result=ailcall1(AIL,redbook_open_volume,HREDBOOK,drive);
#else
   result=ailcall1(AIL,redbook_open_drive,HREDBOOK,drive);
#endif

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_redbook_close(HREDBOOK hand)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_close(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   ailcall1(AIL,redbook_close,void,hand);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void AILEXPORT AIL_redbook_eject(HREDBOOK hand)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_eject(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   ailcall1(AIL,redbook_eject,void,hand);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void AILEXPORT AIL_redbook_retract(HREDBOOK hand)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_retract(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   ailcall1(AIL,redbook_retract,void,hand);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_status(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_status(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_status,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_tracks(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_tracks(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_tracks,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      INDENT;

      AIL_fprintf(AIL_debugfile,"Result = %ld" NL,result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_track(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_track(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_track,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_redbook_track_info(HREDBOOK hand,U32 tracknum,U32 FAR* startmsec,U32 FAR* endmsec)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_track_info(" PTRFMT ", %ld, " PTRFMT ", " PTRFMT ")" NL,hand,tracknum,startmsec,endmsec);
      }

   IN_AIL_NM;

   ailcall4(AIL,redbook_track_info,void,hand,tracknum,startmsec,endmsec);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_id(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_id(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_id,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_position(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_position(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_position,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_play(HREDBOOK hand,U32 startmsec, U32 endmsec)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_play(" PTRFMT ", %ld, %ld)" NL,hand,startmsec,endmsec);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,redbook_play,U32,hand,startmsec,endmsec);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_stop(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_stop(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_stop,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_pause(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_pause(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_pause,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
U32 AILEXPORT AIL_redbook_resume(HREDBOOK hand)
{
   U32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_resume(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_resume,U32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
F32 AILEXPORT AIL_redbook_volume_level(HREDBOOK hand)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_volume_level(" PTRFMT ")" NL,hand);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,redbook_volume_level,F32,hand);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat(result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
F32 AILEXPORT AIL_redbook_set_volume_level(HREDBOOK hand, F32 volume)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_redbook_set_volume(" PTRFMT ",%s)" NL,hand,FTS(volume));
      }

   IN_AIL_NM;

   result=ailcall2(AIL,redbook_set_volume_level,F32,hand,volume);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat(result);
      }
   END;

   return result;
}

#endif

//############################################################################

#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

DXDEF
S32   AILEXPORT AIL_quick_startup             (S32         use_digital,
                                               S32         use_MIDI,
                                               U32         output_rate,
                                               S32         output_bits,
                                               S32         output_channels)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_startup(%ld, %ld, %ld, %ld, %ld )" NL,use_digital,use_MIDI,output_rate,output_bits,output_channels);
      }

   IN_AIL_NM;

   result=ailcall5(AIL,quick_startup,S32,use_digital,use_MIDI,output_rate,output_bits,output_channels);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}

#endif

//############################################################################

DXDEF
void   AILEXPORT AIL_quick_shutdown            (void)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_shutdown()" NL);
      }

   IN_AIL_NM;

   ailcall0(AIL,quick_shutdown,void);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_quick_handles ( HDIGDRIVER FAR* pdig, HMDIDRIVER FAR* pmdi, HDLSDEVICE FAR* pdls )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_handles( " PTRFMT ", " PTRFMT " , " PTRFMT " )" NL,pdig,pmdi,pdls);
      }

   IN_AIL_NM;

   ailcall3(AIL,quick_handles,void,pdig,pmdi,pdls);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
HAUDIO  AILEXPORT AIL_quick_load                (char const   FAR *filename)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_load(%s)" NL,filename);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_load,HAUDIO,filename);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

#ifdef ON_MAC_USE_FSS

DXDEF
HAUDIO  AILEXPORT AIL_quick_fss_load            (FSSpec const   FAR *filename)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_fss_load(" PTRFMT ")" NL,filename);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_fss_load,HAUDIO,filename);

   OUT_AIL_NM;

   RESULT
      {
      outreshex((U32)result);
      }
   END;

   return result;
}

#endif

//############################################################################

DXDEF
HAUDIO  AILEXPORT AIL_quick_load_mem            (void const   FAR *mem,
                                                 U32    size)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_load_mem(" PTRFMT ",%lu)" NL,mem,size);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,quick_load_mem,HAUDIO,mem,size);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
HAUDIO  AILEXPORT AIL_quick_load_named_mem      (void const   FAR *mem,
                                                 char const   FAR *filename,
                                                 U32    size)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_load_mem(" PTRFMT ",%s%lu)" NL,mem,filename,size);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,quick_load_named_mem,HAUDIO,mem,filename,size);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
HAUDIO  AILEXPORT AIL_quick_copy                (HAUDIO audio)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_copy(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_copy,HAUDIO,audio);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void    AILEXPORT AIL_quick_unload              (HAUDIO      audio)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_unload(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   ailcall1(AIL,quick_unload,void,audio);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
S32    AILEXPORT AIL_quick_play                (HAUDIO      audio,
                                                U32         loop_count)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_play(" PTRFMT ", %ld)" NL,audio,loop_count);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,quick_play,S32,audio,loop_count);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_quick_halt                (HAUDIO      audio)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_halt(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   ailcall1(AIL,quick_halt,void,audio);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
S32     AILEXPORT AIL_quick_status              (HAUDIO      audio)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_status(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_status,S32,audio);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
HAUDIO  AILEXPORT AIL_quick_load_and_play       (char const  FAR *filename,
                                                 U32         loop_count,
                                                 S32         wait_request)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_load_and_play(%s, %ld, %ld)" NL,filename,loop_count,wait_request);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,quick_load_and_play,HAUDIO,filename,loop_count,wait_request);

   OUT_AIL_NM;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}

//############################################################################

#ifdef ON_MAC_USE_FSS

DXDEF
HAUDIO  AILEXPORT AIL_quick_fss_load_and_play   (FSSpec const  FAR *filename,
                                                 U32         loop_count,
                                                 S32         wait_request)
{
   HAUDIO result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_fss_load_and_play(" PTRFMT ", %ld, %ld)" NL,filename,loop_count,wait_request);
      }

   IN_AIL_NM;

   result=ailcall3(AIL,quick_fss_load_and_play,HAUDIO,filename,loop_count,wait_request);

   OUT_AIL_NM;

   RESULT
      {
      outreshex((U32)result);
      }
   END;

   return result;
}

#endif

//############################################################################

DXDEF
void   AILEXPORT AIL_quick_set_speed     (HAUDIO      audio, S32 speed)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_set_speed(" PTRFMT ", %ld)" NL,audio,speed);
      }

   IN_AIL_NM;

   ailcall2(AIL,quick_set_speed,void,audio,speed);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_quick_set_volume (HAUDIO audio, F32 volume, F32 extravol)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_set_volume(" PTRFMT ", %s, %s)" NL,audio,FTS(volume),FTS(extravol));
      }

   IN_AIL_NM;

   ailcall3(AIL,quick_set_volume,void,audio,volume,extravol);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_quick_set_reverb_levels (HAUDIO audio,
                                              F32     dry_level,
                                              F32     wet_level)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_set_reverb_levels(" PTRFMT ", %s, %s)" NL,audio,FTS(dry_level),FTS(wet_level));
      }

   IN_AIL_NM;

   ailcall3(AIL,quick_set_reverb_levels,void,audio,dry_level,wet_level);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_quick_set_low_pass_cut_off(HAUDIO audio,
                                                F32    cut_off)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_set_low_pass_cut_off(" PTRFMT ", %s)" NL,audio,FTS(cut_off));
      }

   IN_AIL_NM;

   ailcall2(AIL,quick_set_low_pass_cut_off,void,audio,cut_off);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
HSTREAM AILEXPORT AIL_open_stream(HDIGDRIVER dig, char const FAR* filename, S32 stream_mem)
{
   HSTREAM result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_stream(" PTRFMT ", %s, %ld)" NL,dig,filename,stream_mem);
      }

   IN_AIL;

   result=ailcall3(AIL,open_stream,HSTREAM,dig,filename,stream_mem);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_close_stream(HSTREAM stream)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_close_stream(" PTRFMT ")" NL,stream);
      }

   IN_AIL;

   ailcall1(AIL,close_stream,void,stream);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
HSAMPLE AILEXPORT AIL_stream_sample_handle(HSTREAM stream)
{
   HSAMPLE result = NULL;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_sample_handle(" PTRFMT ")" NL,stream);
      }

   if (stream != NULL)
      result = stream->samp;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}

//############################################################################

DXDEF
S32 AILEXPORT AIL_service_stream(HSTREAM stream, S32 fillup)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_service_stream(" PTRFMT ", %ld)" NL,stream,fillup);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,service_stream,S32,stream,fillup);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_start_stream(HSTREAM stream)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_start_stream(" PTRFMT ")" NL,stream);
      }

   IN_AIL;

   ailcall1(AIL,start_stream,void,stream);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
void AILEXPORT AIL_pause_stream(HSTREAM stream, S32 onoff)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_pause_stream(" PTRFMT ", %ld)" NL,stream,onoff);
      }

   IN_AIL;

   ailcall2(AIL,pause_stream,void,stream,onoff);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
S32 AILEXPORT AIL_stream_loop_count(HSTREAM stream)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_loop_count(" PTRFMT ")" NL,stream);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,stream_loop_count,S32,stream);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void         AILEXPORT AIL_set_stream_loop_block (HSTREAM S,
                                                     S32     loop_start_offset,
                                                     S32     loop_end_offset)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_stream_loop_block(" PTRFMT ",%ld,%ld)" NL,S,loop_start_offset,loop_end_offset);
      }

   IN_AIL_NM;

   ailcall3(AIL,set_stream_loop_block,void,S,loop_start_offset,loop_end_offset);

   OUT_AIL_NM;

   END;
}



//############################################################################

DXDEF
void AILEXPORT AIL_set_stream_loop_count(HSTREAM stream, S32 count)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_stream_loop_count(" PTRFMT ", %ld)" NL,stream,count);
      }

   IN_AIL_NM;

   ailcall2(AIL,set_stream_loop_count,void,stream,count);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
S32 AILEXPORT AIL_stream_status(HSTREAM stream)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_status(" PTRFMT ")" NL,stream);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,stream_status,S32,stream);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_set_stream_position(HSTREAM stream,S32 offset)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_stream_position(" PTRFMT ", %ld)" NL,stream,offset);
      }

   IN_AIL;

   ailcall2(AIL,set_stream_position,void,stream,offset);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
S32 AILEXPORT AIL_stream_position(HSTREAM stream)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_position(" PTRFMT ")" NL,stream);
      }

   IN_AIL;

   result=ailcall1(AIL,stream_position,S32,stream);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }
   END;

   return result;
}


//############################################################################

DXDEF
void AILEXPORT AIL_stream_info(HSTREAM stream, S32 FAR* datarate, S32 FAR* sndtype, S32 FAR* length, S32 FAR* memory)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_info(" PTRFMT ", " PTRFMT ", " PTRFMT ", " PTRFMT ", " PTRFMT ")" NL,stream,datarate,sndtype,length,memory);
      }

   IN_AIL_NM;

   ailcall5(AIL,stream_info,void,stream,datarate,sndtype,length,memory);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
AILSTREAMCB AILEXPORT AIL_register_stream_callback(HSTREAM stream, AILSTREAMCB callback)
{
   AILSTREAMCB result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_register_stream_callback(" PTRFMT ", " PTRFMT ")" NL,stream,callback);
      }

   IN_AIL;

   result=ailcall2(AIL,register_stream_callback,AILSTREAMCB,stream,callback);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}


#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_XBOX) || defined(IS_XENON) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

//############################################################################

DXDEF
void AILEXPORT AIL_auto_service_stream(HSTREAM stream, S32 onoff)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_auto_service_stream(" PTRFMT ", %ld)" NL,stream,onoff);
      }

   IN_AIL;

   ailcall2(AIL,auto_service_stream,void,stream,onoff);

   OUT_AIL;

   END;
}


#endif

//############################################################################

DXDEF
void     AILEXPORT AIL_set_stream_user_data      (HSTREAM S, //)
                                                  U32     index,
                                                  SINTa   value)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_stream_user_data(" PTRFMT ",%ld," SINTFMT ")" NL,S,index,value);
      }

   IN_AIL_NM;

   ailcall3(AIL,set_stream_user_data,void,S,index,value);

   OUT_AIL_NM;

   END;
}


//############################################################################

DXDEF
SINTa    AILEXPORT AIL_stream_user_data          (HSTREAM S, //)
                                                  U32     index)
{
   SINTa result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_user_data(" PTRFMT ",%lu)" NL,S,index);
      }

   IN_AIL_NM;

   result=ailcall2(AIL,stream_user_data,SINTa,S,index);

   OUT_AIL_NM;

   RESULT
      {
      outresuint((UINTa)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
HDLSDEVICE AILEXPORT AIL_DLS_open(HMDIDRIVER mdi, HDIGDRIVER dig,
#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)
                                  char const FAR* dls,
#else
                                  AILSTATICDLS const FAR* dls,
#endif
                                  U32 flags, U32 rate, S32 bits, S32 channels)
{
   HDLSDEVICE result;

   START
      {
#if defined(IS_WINDOWS) || defined(IS_MAC) || defined(IS_LINUX)
      AIL_fprintf(AIL_debugfile,"AIL_DLS_open(" PTRFMT "," PTRFMT ",%s,%lX,%lu,%lu,%lu)" NL,mdi,dig,dls,flags,rate,bits,channels);
#else
      AIL_fprintf(AIL_debugfile,"AIL_DLS_open(" PTRFMT "," PTRFMT "," PTRFMT ",%lX,%lu,%lu,%lu)" NL,mdi,dig,dls,flags,rate,bits,channels);
#endif
      }

   IN_AIL;

   result=ailcall7(AIL,DLS_open,HDLSDEVICE,mdi,dig,dls,flags,rate,bits,channels);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
void   AILEXPORT  AIL_DLS_close(HDLSDEVICE dls, U32 flags)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_close(" PTRFMT ",%ld)" NL,dls,flags);
      }

   IN_AIL;

   ailcall2(AIL,DLS_close,void,dls,flags);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
HDLSFILEID AILEXPORT AIL_DLS_load_file(HDLSDEVICE dls, char const FAR* filename, U32 flags)
{
   HDLSFILEID result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_load_file(" PTRFMT ",%s,%lu)" NL,dls,filename,flags);
      }

   IN_AIL;

   result=ailcall3(AIL,DLS_load_file,HDLSFILEID,dls,filename,flags);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
HDLSFILEID AILEXPORT AIL_DLS_load_memory(HDLSDEVICE dls, void const FAR* memfile, U32 flags)
{
   HDLSFILEID result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_load_memory(" PTRFMT "," PTRFMT ",%lu)" NL,dls,memfile,flags);
      }

   IN_AIL;

   result=ailcall3(AIL,DLS_load_memory,HDLSFILEID,dls,memfile,flags);

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_DLS_unload(HDLSDEVICE dls, HDLSFILEID dlsid)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_unload(" PTRFMT "," PTRFMT ")" NL,dls,dlsid);
      }

   IN_AIL;

   ailcall2(AIL,DLS_unload,void,dls,dlsid);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
void   AILEXPORT AIL_DLS_compact(HDLSDEVICE dls)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_compact(" PTRFMT ")" NL,dls);
      }

   IN_AIL;

   ailcall1(AIL,DLS_compact,void,dls);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
HSAMPLE AILEXPORT AIL_DLS_sample_handle(HDLSDEVICE dls)
{
   HSAMPLE result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_sample_handle(" PTRFMT ")" NL,dls);
      }

  if (dls) {
    // are we using the soft-synthesizer
#if !defined(IS_MAC) && !defined(IS_LINUX)
#ifdef IS_WINDOWS
    if (dls->lib==0)
#else
    if(dls->DOSname==0)
#endif
#endif
    {
      result = dls->sample;
    }
  }

   RESULT
      {
      outresuhex((UINTa)result);
      }
   END;

   return result;
}

//############################################################################

DXDEF
void   AILEXPORT AIL_DLS_get_info(HDLSDEVICE dls, AILDLSINFO FAR* info, S32 FAR* PercentCPU)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_get_info(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,dls,info,PercentCPU);
      }

   IN_AIL_NM;

   ailcall3(AIL,DLS_get_info,void,dls,info,PercentCPU);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void AILEXPORT    AIL_set_sequence_ms_position  (HSEQUENCE S, //)
                                                 S32       milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sequence_ms_position(" PTRFMT ",%lu)" NL,S,milliseconds);
      }

   IN_AIL;

   ailcall2(AIL,set_sequence_ms_position,void,S,milliseconds);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void AILEXPORT AIL_sequence_ms_position(HSEQUENCE S, //)
                                  S32 FAR    *total_milliseconds,
                                  S32 FAR    *current_milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sequence_ms_position(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,total_milliseconds,current_milliseconds);
      }

   IN_AIL_NM;

   ailcall3(AIL,sequence_ms_position,void,S,total_milliseconds,current_milliseconds);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void AILEXPORT AIL_sample_ms_position(HSAMPLE S, //)
                                  S32 FAR    *total_milliseconds,
                                  S32 FAR    *current_milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_ms_position(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,total_milliseconds,current_milliseconds);
      }

   IN_AIL_NM;

   dsailcall3(AIL,sample_ms_position,void,S,total_milliseconds,current_milliseconds);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void AILEXPORT    AIL_set_sample_ms_position  (HSAMPLE S, //)
                                               S32       milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_ms_position(" PTRFMT ",%lu)" NL,S,milliseconds);
      }

   IN_AIL;

   dsailcall2(AIL,set_sample_ms_position,void,S,milliseconds);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void AILEXPORT AIL_stream_ms_position(HSTREAM S, //)
                                  S32 FAR    *total_milliseconds,
                                  S32 FAR    *current_milliseconds)
{
    START
      {
      AIL_fprintf(AIL_debugfile,"AIL_stream_ms_position(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,S,total_milliseconds,current_milliseconds);
      }

   IN_AIL_NM;

   ailcall3(AIL,stream_ms_position,void,S,total_milliseconds,current_milliseconds);

   OUT_AIL_NM;

   END;
}

//############################################################################

DXDEF
void AILEXPORT    AIL_set_stream_ms_position  (HSTREAM S, //)
                                               S32       milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_stream_ms_position(" PTRFMT ",%lu)" NL,S,milliseconds);
      }

   IN_AIL;

   ailcall2(AIL,set_stream_ms_position,void,S,milliseconds);

   OUT_AIL;

   END;
}

//############################################################################

DXDEF
void AILEXPORT AIL_quick_set_ms_position(HAUDIO audio,S32 milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_set_ms_position(" PTRFMT ",%lu)" NL,audio,milliseconds);
      }

   IN_AIL;

   ailcall2(AIL,quick_set_ms_position,void,audio,milliseconds);

   OUT_AIL;

   END;
}


//############################################################################

DXDEF
S32 AILEXPORT AIL_quick_ms_position(HAUDIO audio)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_ms_position(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_ms_position,S32,audio);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################

DXDEF
S32 AILEXPORT AIL_quick_ms_length(HAUDIO audio)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_ms_length(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_ms_length,S32,audio);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}


//############################################################################

DXDEF
S32 AILEXPORT AIL_quick_type(HAUDIO audio)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_quick_type(" PTRFMT ")" NL,audio);
      }

   IN_AIL_NM;

   result=ailcall1(AIL,quick_type,S32,audio);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

DXDEF
S32  AILEXPORT      AIL_MIDI_to_XMI       (void const FAR* MIDI,
                                           U32       MIDI_size,
                                           void FAR* FAR*XMIDI,
                                           U32  FAR* XMIDI_size,
                                           S32       flags)

{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_MIDI_to_XMI(" PTRFMT ",%lu," PTRFMT "," PTRFMT ",0x%lX)" NL,
         MIDI, MIDI_size,XMIDI,XMIDI_size,flags);
      }

   IN_AIL_NM;

   result=AIL_API_MIDI_to_XMI(MIDI, MIDI_size,XMIDI,XMIDI_size,flags);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}


DXDEF
S32 AILEXPORT AIL_compress_ADPCM(AILSOUNDINFO const FAR* info,
                              void FAR* FAR* outdata, U32 FAR* outsize)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_compress_ADPCM(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         info,outdata,outsize);
      }

   IN_AIL_NM;

   result=AIL_API_compress_ADPCM(info,outdata,outsize);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}

DXDEF
S32 AILEXPORT AIL_decompress_ADPCM(AILSOUNDINFO const FAR* info,
                                void FAR* FAR* outdata, U32 FAR* outsize)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_decompress_ADPCM(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         info,outdata,outsize);
      }

   IN_AIL_NM;

   result=AIL_API_decompress_ADPCM(info,outdata,outsize);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}


DXDEF
S32 AILEXPORT AIL_WAV_info(void const FAR* data, AILSOUNDINFO FAR* info)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_WAV_info(" PTRFMT "," PTRFMT ")" NL,
         data,info);
      }

   IN_AIL_NM;

   result=AIL_API_WAV_info(data,info);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}

DXDEF
S32 AILEXPORT AIL_file_type(void const FAR* data, U32 size)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_type(" PTRFMT ",%lu)" NL,
         data,size);
      }

   IN_AIL_NM;

   result=AIL_API_file_type(data,size);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}

DXDEF
S32 AILEXPORT AIL_file_type_named(void const FAR* data, char const FAR * filename,U32 size)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_file_type(" PTRFMT ",%s,%lu)" NL,
         data,filename,size);
      }

   IN_AIL_NM;

   result=AIL_API_file_type_named(data,filename,size);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}

DXDEF
S32 AILEXPORT AIL_find_DLS    (void const FAR* data, U32 size,
                               void FAR* FAR*xmi, U32 FAR* xmisize,
                               void FAR* FAR*dls, U32 FAR* dlssize)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_find_DLS_in_XMI(" PTRFMT ",%lu," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         data,size,xmi,xmisize,dls,dlssize);
      }

   IN_AIL_NM;

   result=AIL_API_find_DLS(data,size,xmi,xmisize,dls,dlssize);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

  return( result);

}

//############################################################################

DXDEF
S32 AILEXPORT AIL_extract_DLS      (void const FAR       *source_image, //)
                                     U32             source_size,
                                     void FAR * FAR *XMI_output_data,
                                     U32  FAR       *XMI_output_size,
                                     void FAR * FAR *DLS_output_data,
                                     U32  FAR       *DLS_output_size,
                                     AILLENGTHYCB    callback)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_DLS_extract_image(" PTRFMT ",0x%lu," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         source_image,source_size,XMI_output_data,XMI_output_size,DLS_output_data,DLS_output_size,callback);
      }

   IN_AIL_NM;

   result=AIL_API_extract_DLS(source_image,source_size,XMI_output_data,XMI_output_size,DLS_output_data,DLS_output_size,callback);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;

}

#if defined(IS_WIN32API) || defined(IS_MAC) || defined(IS_LINUX) || defined(IS_DOS) || defined(IS_PS2) || defined(IS_PS3) || defined(IS_WII)

extern HDIGDRIVER primary_digital_driver;   // from msswo.cpp

//############################################################################
DXDEF HDIGDRIVER AILEXPORT AIL_primary_digital_driver  (HDIGDRIVER new_primary)
{
   HDIGDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_primary_digital_driver(" PTRFMT ")" NL,
         new_primary);
      }

   IN_AIL;

   result = primary_digital_driver;

   if (new_primary != NULL)
      {
      primary_digital_driver = new_primary;
      }

   OUT_AIL;

   RESULT
      {
      outresuhex((UINTa)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF
void       AILEXPORT AIL_set_sample_obstruction (HSAMPLE S, //)
                                                 F32     obstruction)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_obstruction(" PTRFMT ",%s)" NL,
          S, FTS(obstruction));
      }

   IN_AIL;

   AIL_API_set_sample_obstruction(S, obstruction);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF
void       AILEXPORT AIL_set_sample_exclusion (HSAMPLE S, //)
                                               F32     exclusion)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_exclusion(" PTRFMT ",%s)" NL,
          S, FTS(exclusion));
      }

   IN_AIL;

   AIL_API_set_sample_exclusion(S, exclusion);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF
void       AILEXPORT AIL_set_sample_occlusion   (HSAMPLE S, //)
                                                 F32     occlusion)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_occlusion(" PTRFMT ",%s)" NL,
          S, FTS(occlusion));
      }

   IN_AIL;

   AIL_API_set_sample_occlusion(S, occlusion);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF
void       AILEXPORT AIL_set_sample_3D_cone        (HSAMPLE S, //)
                                                    F32       inner_angle,
                                                    F32       outer_angle,
                                                    F32       outer_volume)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_cone(" PTRFMT ",%s,%s,%s)" NL,
          S, FTS(inner_angle),FTS(outer_angle),FTS(outer_volume));
      }

   IN_AIL;

   AIL_API_set_sample_3D_cone(S, inner_angle,outer_angle,outer_volume);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF
F32        AILEXPORT AIL_sample_obstruction (HSAMPLE S)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_obstruction(" PTRFMT ")" NL,
          S);
      }

   IN_AIL_NM;

   result = AIL_API_sample_obstruction(S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)(S32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF
F32        AILEXPORT AIL_sample_exclusion (HSAMPLE S)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_exclusion(" PTRFMT ")" NL,
          S);
      }

   IN_AIL_NM;

   result = AIL_API_sample_exclusion(S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)(S32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF
F32        AILEXPORT AIL_sample_occlusion   (HSAMPLE S)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_occlusion(" PTRFMT ")" NL,
          S);
      }

   IN_AIL_NM;

   result = AIL_API_sample_occlusion(S);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)(S32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF
void       AILEXPORT AIL_sample_3D_cone        (HSAMPLE S,
                                                F32 FAR* inner_angle,
                                                F32 FAR* outer_angle,
                                                F32 FAR* outer_volume)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_3D_cone(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          S,inner_angle,outer_angle,outer_volume);
      }

   IN_AIL_NM;

   AIL_API_sample_3D_cone(S,inner_angle,outer_angle,outer_volume);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_sample_3D_distances (HSAMPLE samp, //)
                                                      F32     max_dist,
                                                      F32     min_dist,
                                                      S32     auto_3D_wet_atten)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_distances(" PTRFMT ",%s,%s,%ld)" NL,
          samp,FTS(max_dist),FTS(min_dist),auto_3D_wet_atten);
      }

   IN_AIL;

   AIL_API_set_sample_3D_distances(samp,max_dist,min_dist,auto_3D_wet_atten);

   OUT_AIL;

   END;
}


//############################################################################
DXDEF void AILEXPORT     AIL_sample_3D_distances     (HSAMPLE samp, //)
                                                      F32 FAR * max_dist,
                                                      F32 FAR * min_dist,
                                                      S32 FAR * auto_3D_wet_atten)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_3D_distances(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          samp,max_dist,min_dist,auto_3D_wet_atten);
      }

   IN_AIL_NM;

   AIL_API_sample_3D_distances(samp,max_dist,min_dist,auto_3D_wet_atten);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF  S32 AILEXPORT    AIL_room_type   (HDIGDRIVER dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_room_type(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result = AIL_API_room_type(dig);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  void AILEXPORT   AIL_set_room_type   (HDIGDRIVER dig, //)
                                             S32       EAX_room_type)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_room_type(" PTRFMT ",%d)" NL,dig,EAX_room_type);
      }

   IN_AIL;

   AIL_API_set_room_type(dig, EAX_room_type);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  F32 AILEXPORT    AIL_3D_rolloff_factor  (HDIGDRIVER dig)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_3D_rolloff_factor(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result = AIL_API_3D_rolloff_factor(dig);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat((F32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  void AILEXPORT   AIL_set_3D_rolloff_factor (HDIGDRIVER dig, //)
                                                   F32       factor )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_3D_rolloff_factor(" PTRFMT ",%f)" NL,dig,FTS(factor));
      }

   IN_AIL;

   AIL_API_set_3D_rolloff_factor(dig, factor);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  F32 AILEXPORT    AIL_3D_doppler_factor  (HDIGDRIVER dig)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_3D_doppler_factor(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result = AIL_API_3D_doppler_factor(dig);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat((F32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  void AILEXPORT   AIL_set_3D_doppler_factor (HDIGDRIVER dig, //)
                                                   F32       factor )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_3D_doppler_factor(" PTRFMT ",%s)" NL,dig,FTS(factor));
      }

   IN_AIL;

   AIL_API_set_3D_doppler_factor(dig, factor);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF  F32 AILEXPORT    AIL_3D_distance_factor  (HDIGDRIVER dig)
{
   F32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_3D_distance_factor(" PTRFMT ")" NL,dig);
      }

   IN_AIL_NM;

   result = AIL_API_3D_distance_factor(dig);

   OUT_AIL_NM;

   RESULT
      {
      outresfloat((F32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF  void AILEXPORT   AIL_set_3D_distance_factor (HDIGDRIVER dig, //)
                                                    F32       factor )
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_3D_distance_factor(" PTRFMT ",%s)" NL,dig,FTS(factor));
      }

   IN_AIL;

   AIL_API_set_3D_distance_factor(dig, factor);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void  AILEXPORT    AIL_set_sample_3D_position         (HSAMPLE S, //)
                                                      F32     X,
                                                      F32     Y,
                                                      F32     Z)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_position(" PTRFMT ",%s,%s,%s)" NL,
          S,FTS(X),FTS(Y),FTS(Z));
      }

   IN_AIL;

   AIL_API_set_sample_3D_position(S,X,Y,Z);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_sample_3D_velocity         (HSAMPLE S, //)
                                                      F32     dX_per_ms,
                                                      F32     dY_per_ms,
                                                      F32     dZ_per_ms,
                                                      F32     magnitude)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_velocity(" PTRFMT ",%s,%s,%s,%s)" NL,
          S,FTS(dX_per_ms),FTS(dY_per_ms),FTS(dZ_per_ms),FTS(magnitude));
      }

   IN_AIL;

   AIL_API_set_sample_3D_velocity(S,dX_per_ms,dY_per_ms,dZ_per_ms,magnitude);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_sample_3D_velocity_vector  (HSAMPLE S, //)
                                                      F32     dX_per_ms,
                                                      F32     dY_per_ms,
                                                      F32     dZ_per_ms)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_velocity_vector(" PTRFMT ",%s,%s,%s)" NL,
          S,FTS(dX_per_ms),FTS(dY_per_ms),FTS(dZ_per_ms));
      }

   IN_AIL;

   AIL_API_set_sample_3D_velocity_vector(S,dX_per_ms,dY_per_ms,dZ_per_ms);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_sample_3D_orientation      (HSAMPLE S, //)
                                                      F32     X_face,
                                                      F32     Y_face,
                                                      F32     Z_face,
                                                      F32     X_up,
                                                      F32     Y_up,
                                                      F32     Z_up)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_sample_3D_orientation(" PTRFMT ",%s,%s,%s,%s,%s,%s)" NL,
          S,FTS(X_face),FTS(Y_face),FTS(Z_face),FTS(X_up),FTS(Y_up),FTS(Z_up));
      }

   IN_AIL;

   AIL_API_set_sample_3D_orientation(S,X_face,Y_face,Z_face,X_up,Y_up,Z_up);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF S32 AILEXPORT     AIL_sample_3D_position       (HSAMPLE  S, //)
                                                      F32 FAR *X,
                                                      F32 FAR *Y,
                                                      F32 FAR *Z)
{
   S32 result;
   
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_3D_position(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          S,X,Y,Z);
      }

   IN_AIL_NM;

   result = AIL_API_sample_3D_position(S,X,Y,Z);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32) result);
      }

   END;

   return result;
}

//############################################################################
DXDEF void AILEXPORT     AIL_sample_3D_velocity             (HSAMPLE  S, //)
                                                      F32 FAR *dX_per_ms,
                                                      F32 FAR *dY_per_ms,
                                                      F32 FAR *dZ_per_ms)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_3D_velocity(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          S,dX_per_ms,dY_per_ms,dZ_per_ms);
      }

   IN_AIL_NM;

   AIL_API_sample_3D_velocity(S,dX_per_ms,dY_per_ms,dZ_per_ms);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_sample_3D_orientation          (HSAMPLE  S, //)
                                                      F32 FAR *X_face,
                                                      F32 FAR *Y_face,
                                                      F32 FAR *Z_face,
                                                      F32 FAR *X_up,
                                                      F32 FAR *Y_up,
                                                      F32 FAR *Z_up)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_3D_orientation(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          S,X_face,Y_face,Z_face,X_up,Y_up,Z_up);
      }

   IN_AIL_NM;

   AIL_API_sample_3D_orientation(S,X_face,Y_face,Z_face,X_up,Y_up,Z_up);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_update_sample_3D_position      (HSAMPLE S, //)
                                                             F32     dt_milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_update_sample_3D_position(" PTRFMT ",%s)" NL,
          S,FTS(dt_milliseconds));
      }

   IN_AIL;

   AIL_API_update_sample_3D_position(S, dt_milliseconds);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void  AILEXPORT    AIL_set_listener_3D_position         (HDIGDRIVER dig, //)
                                                      F32     X,
                                                      F32     Y,
                                                      F32     Z)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_listener_3D_position(" PTRFMT ",%s,%s,%s)" NL,
          dig,FTS(X),FTS(Y),FTS(Z));
      }

   IN_AIL;

   AIL_API_set_listener_3D_position(dig,X,Y,Z);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_listener_3D_velocity         (HDIGDRIVER dig, //)
                                                      F32     dX_per_ms,
                                                      F32     dY_per_ms,
                                                      F32     dZ_per_ms,
                                                      F32     magnitude)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_listener_3D_velocity(" PTRFMT ",%s,%s,%s,%s)" NL,
          dig,FTS(dX_per_ms),FTS(dY_per_ms),FTS(dZ_per_ms),FTS(magnitude));
      }

   IN_AIL;

   AIL_API_set_listener_3D_velocity(dig,dX_per_ms,dY_per_ms,dZ_per_ms,magnitude);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_listener_3D_velocity_vector  (HDIGDRIVER dig, //)
                                                      F32     dX_per_ms,
                                                      F32     dY_per_ms,
                                                      F32     dZ_per_ms)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_listener_3D_velocity_vector(" PTRFMT ",%s,%s,%s)" NL,
          dig,FTS(dX_per_ms),FTS(dY_per_ms),FTS(dZ_per_ms));
      }

   IN_AIL;

   AIL_API_set_listener_3D_velocity_vector(dig,dX_per_ms,dY_per_ms,dZ_per_ms);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_set_listener_3D_orientation      (HDIGDRIVER dig, //)
                                                      F32     X_face,
                                                      F32     Y_face,
                                                      F32     Z_face,
                                                      F32     X_up,
                                                      F32     Y_up,
                                                      F32     Z_up)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_listener_3D_orientation(" PTRFMT ",%s,%s,%s,%s,%s,%s)" NL,
          dig,FTS(X_face),FTS(Y_face),FTS(Z_face),FTS(X_up),FTS(Y_up),FTS(Z_up));
      }

   IN_AIL;

   AIL_API_set_listener_3D_orientation(dig,X_face,Y_face,Z_face,X_up,Y_up,Z_up);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_listener_3D_position             (HDIGDRIVER  dig, //)
                                                      F32 FAR *X,
                                                      F32 FAR *Y,
                                                      F32 FAR *Z)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_listener_3D_position(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          dig,X,Y,Z);
      }

   IN_AIL_NM;

   AIL_API_listener_3D_position(dig,X,Y,Z);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_listener_3D_velocity             (HDIGDRIVER  dig, //)
                                                      F32 FAR *dX_per_ms,
                                                      F32 FAR *dY_per_ms,
                                                      F32 FAR *dZ_per_ms)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_listener_3D_velocity(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          dig,dX_per_ms,dY_per_ms,dZ_per_ms);
      }

   IN_AIL_NM;

   AIL_API_listener_3D_velocity(dig,dX_per_ms,dY_per_ms,dZ_per_ms);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_listener_3D_orientation          (HDIGDRIVER  dig, //)
                                                      F32 FAR *X_face,
                                                      F32 FAR *Y_face,
                                                      F32 FAR *Z_face,
                                                      F32 FAR *X_up,
                                                      F32 FAR *Y_up,
                                                      F32 FAR *Z_up)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_listener_3D_orientation(" PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
          dig,X_face,Y_face,Z_face,X_up,Y_up,Z_up);
      }

   IN_AIL_NM;

   AIL_API_listener_3D_orientation(dig,X_face,Y_face,Z_face,X_up,Y_up,Z_up);

   OUT_AIL_NM;

   END;
}

//############################################################################
DXDEF void AILEXPORT     AIL_update_listener_3D_position      (HDIGDRIVER dig, //)
                                                      F32     dt_milliseconds)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_update_listener_3D_position(" PTRFMT ",%s)" NL,
          dig,FTS(dt_milliseconds));
      }

   IN_AIL;

   AIL_API_update_listener_3D_position(dig, dt_milliseconds);

   OUT_AIL;

   END;
}

#endif

#ifdef MSS_FLT_SUPPORTED

//############################################################################
DXDEF HPROVIDER AILEXPORT AIL_digital_output_filter (HDIGDRIVER dig)
{
   HPROVIDER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_digital_output_filter(" PTRFMT ")" NL,
         dig);
      }

   IN_AIL;

#ifdef MSS_VFLT_SUPPORTED
   if (dig->voice_filter != NULL)
      {
      result = dig->voice_filter->provider;     // e.g., EAX
      }
   else
#endif 
   if (dig->matrix_filter != NULL)
      {
      result = dig->matrix_filter->provider;    // e.g., Dolby Surround
      }
   else
      {
      result = 0;
      }

   OUT_AIL;

   RESULT
      {
      outreshex((U32) result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_filters (HPROENUM  FAR *next, //)
                                           HPROVIDER FAR *dest,
                                           C8  FAR * FAR *name)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_filters(" PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         next,dest,name);
      }

   IN_AIL_NM;

   result = AIL_API_enumerate_filters(next,dest,name);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF HDRIVERSTATE AILEXPORT AIL_open_filter        (HPROVIDER  provider, //)
                                                     HDIGDRIVER dig)
{
   HDRIVERSTATE result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_open_filter(" HPFMT "," PTRFMT ")" NL,
         provider,dig);
      }

   IN_AIL;

   result = AIL_API_open_filter(provider,dig);

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF void  AILEXPORT    AIL_close_filter       (HDRIVERSTATE filter)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_close_filter(" PTRFMT ")" NL,
         filter);
      }

   IN_AIL;

   AIL_API_close_filter(filter);

   OUT_AIL;

   END;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_filter_properties(HPROVIDER                  lib,   //)
                                                    HINTENUM FAR *             next,
                                                    RIB_INTERFACE_ENTRY FAR *  dest)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_filter_properties(" HPFMT "," PTRFMT "," PTRFMT ")" NL,
         lib,next,dest);
      }

   IN_AIL_NM;

   if (RIB_request_interface(lib,
                            "MSS pipeline filter",
                             0,
                             NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "MSS pipeline filter",
                                       RIB_PROPERTY,
                                       next,
                                       dest);
      }
   else if (RIB_request_interface(lib,                                 
                                 "MSS voice filter",    
                                  0,                                   
                                  NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "MSS voice filter",
                                       RIB_PROPERTY,
                                       next,
                                       dest);

      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_filter_sample_properties(HPROVIDER                  lib,   //)
                                                           HINTENUM FAR *             next,
                                                           RIB_INTERFACE_ENTRY FAR *  dest)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_filter_sample_properties(" HPFMT "," PTRFMT "," PTRFMT ")" NL,
         lib,next,dest);
      }

   IN_AIL_NM;

   if (RIB_request_interface(lib,
                            "Pipeline filter sample services",
                             0,
                             NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Pipeline filter sample services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);
      }
   else if (RIB_request_interface(lib,                                 
                                 "Voice filter sample services",    
                                  0,                                   
                                  NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Voice filter sample services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);

      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_output_filter_driver_properties(HPROVIDER                  lib,   //)
                                                                  HINTENUM FAR *             next,
                                                                  RIB_INTERFACE_ENTRY FAR *  dest)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_output_filter_driver_properties(" HPFMT "," PTRFMT "," PTRFMT ")" NL,
         lib,next,dest);
      }

   IN_AIL_NM;

   if (RIB_request_interface(lib,                                 
                            "Voice filter driver services",    
                             0,                                   
                             NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Voice filter driver services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);

      }
   else if (RIB_request_interface(lib,
                                 "Pipeline filter driver services",
                                  0,
                                  NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Pipeline filter driver services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);
      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_output_filter_sample_properties(HPROVIDER                  lib,   //)
                                                                  HINTENUM FAR *             next,
                                                                  RIB_INTERFACE_ENTRY FAR *  dest)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_output_filter_sample_properties(" HPFMT "," PTRFMT "," PTRFMT ")" NL,
         lib,next,dest);
      }

   IN_AIL_NM;

   if (RIB_request_interface(lib,                                 
                            "Voice filter sample services",    
                             0,                                   
                             NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Voice filter sample services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);

      }
   else if (RIB_request_interface(lib,                                 
                                 "Pipeline filter sample services",    
                                  0,                                   
                                  NULL) == RIB_NOERR)
      {
      result = RIB_enumerate_interface(lib,
                                      "Pipeline filter sample services",
                                       RIB_PROPERTY,
                                       next,
                                       dest);

      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_filter_property(HPROVIDER   lib,  //)
                                         C8 const FAR *    name,
                                         void FAR *       before_value,
                                         void const FAR * new_value,
                                         void FAR *       after_value
                                         )
{
   UINTa token;
   union
   {
     UINTa token;
     PROVIDER_PROPERTY property;
   } un = {0};
     
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_filter_property(" HPFMT ",%s," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         lib,name,before_value,new_value,after_value);
      }

   IN_AIL_NM;

   if (RIB_request_interface_entry(lib,
                                  "MSS pipeline filter",
                                   RIB_FUNCTION,
                                  "PROVIDER_property",
                      (UINTa FAR*)&un.token) == RIB_NOERR)
      {
      if (RIB_request_interface_entry(lib,
                                     "MSS pipeline filter",
                                      RIB_PROPERTY,
                                      name,
                                     &token) == RIB_NOERR)
         {
         return( un.property( token, before_value,new_value,after_value ) );
         }
      }
   else if (RIB_request_interface_entry(lib,
                                       "MSS voice filter",
                                        RIB_FUNCTION,
                                       "PROVIDER_property",
                           (UINTa FAR*)&un.token) == RIB_NOERR)
      {
      if (RIB_request_interface_entry(lib,
                                     "MSS voice filter",
                                      RIB_PROPERTY,
                                      name,
                                     &token) == RIB_NOERR)
         {
         return( un.property( token, before_value,new_value,after_value ) );
         }
      }

   OUT_AIL_NM;

   END;

   return 0;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_enumerate_sample_stage_properties(HSAMPLE                    samp,  //)
                                                          SAMPLESTAGE                stage,
                                                          HINTENUM FAR *             next,
                                                          RIB_INTERFACE_ENTRY FAR *  dest)
{
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_enumerate_sample_stage_properties(" PTRFMT ",%ld," PTRFMT "," PTRFMT ")" NL,
         samp,stage,next,dest);
      }

   IN_AIL_NM;

   if (samp != NULL)
      {
      if ((stage >= SP_FILTER_0) && (stage <= SP_FILTER_7))
         {
         if (samp->pipeline[stage].active)
            {
            result = RIB_enumerate_interface(samp->pipeline[stage].provider,
                                            "Pipeline filter sample services",
                                             RIB_PROPERTY,
                                             next,
                                             dest);
            }
         }
#ifdef MSS_VFLT_SUPPORTED
      else
         {
         //
         // Note that matrix postmix filters can't have any per-sample attributes...
         //
         
         if (samp->driver->voice_filter != NULL)
            {
            result = RIB_enumerate_interface(samp->driver->voice_filter->provider,
                                            "Voice filter sample services",
                                             RIB_PROPERTY,
                                             next,
                                             dest);
            }
         }
#endif
      }

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF S32 AILEXPORT AIL_sample_stage_property  (HSAMPLE        samp, //)
                                                SAMPLESTAGE    stage,  
                                                C8 const FAR * name,
                                                void FAR *       before_value,
                                                void const FAR * new_value,
                                                void FAR *       after_value
                                                )
{
   UINTa token;
   S32 result = 0;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_sample_stage_property(" PTRFMT ",%ld,%s," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         samp,stage,name,before_value,new_value,after_value);
      }

   IN_AIL;

   if (samp != NULL)
      {
      if ((stage >= SP_FILTER_0) && (stage <= SP_FILTER_7))
         {
         if (samp->pipeline[stage].active)
            {
            FLTPROVIDER FAR *provptr = samp->pipeline[stage].TYPE.FLT.provider;

            if (provptr != NULL)
               {
               FLTSMP_SAMPLE_PROPERTY sample_property = provptr->sample_property;

               if (sample_property != NULL)
                  {
                  if (RIB_request_interface_entry(samp->pipeline[stage].provider,
                                                 "Pipeline filter sample services",
                                                  RIB_PROPERTY,
                                                  name,
                                                 &token) == RIB_NOERR)
                     {
                     result = sample_property( samp->pipeline[stage].TYPE.FLT.sample_state,token, before_value,new_value,after_value );
                     }
                  }
               }
            }
         }
#ifdef MSS_VFLT_SUPPORTED
      else
         {
         //
         // Note that matrix postmix filters can't have any per-sample attributes...
         //

         if (samp->driver->voice_filter != NULL)
            {
            if (RIB_request_interface_entry(samp->driver->voice_filter->provider,
                                           "Voice filter sample services",
                                            RIB_PROPERTY,
                                            name,
                                           &token) == RIB_NOERR)
               {
                result = samp->driver->voice_filter->output_sample_property( samp,token, before_value,new_value,after_value );
               }
            }
         }
#endif
      }

   OUT_AIL;

   RESULT
      {
      outresint((U32)result);
      }

   END;
   
   return( result );
}

//############################################################################
DXDEF S32 AILEXPORT AIL_output_filter_driver_property (HDIGDRIVER     dig, //)
                                                       C8 const FAR * name,
                                                       void FAR *       before_value,
                                                       void const FAR * new_value,
                                                       void FAR *       after_value
                                                       )
{
   UINTa token;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_output_filter_driver_property(" PTRFMT ",%s," PTRFMT "," PTRFMT "," PTRFMT ")" NL,
         dig,name,before_value,new_value,after_value);
      }

   IN_AIL_NM;

   if (dig != NULL)
      {
#ifdef MSS_VFLT_SUPPORTED
      if (dig->voice_filter != NULL)
         {
         if (RIB_request_interface_entry(dig->voice_filter->provider,
                                        "Voice filter driver services",
                                         RIB_PROPERTY,
                                         name,
                                        &token) == RIB_NOERR)
            {
            return( dig->voice_filter->driver_property( dig->voice_filter->driver_state,token, before_value,new_value,after_value ) );
            }
         }
      else 
#endif
      if (dig->matrix_filter != NULL)
         {
         if (RIB_request_interface_entry(dig->matrix_filter->provider,
                                        "Pipeline filter driver services",
                                         RIB_PROPERTY,
                                         name,
                                        &token) == RIB_NOERR)
            {
            return( dig->matrix_filter->driver_property( dig->matrix_filter->driver_state,token, before_value,new_value,after_value ) );
            }
         }
      }

   OUT_AIL_NM;

   END;
   
   return 0;
}

#endif

//############################################################################
DXDEF
S32 AILEXPORT AIL_size_processed_digital_audio(
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO const FAR* src)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_size_processed_digital_audio(%li,%li,%li," PTRFMT ")" NL,
          dest_rate,dest_format,num_srcs,src);
      }

   IN_AIL_NM;

   result = AIL_API_size_processed_digital_audio(dest_rate,dest_format,num_srcs,src);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//############################################################################
DXDEF
S32 AILEXPORT AIL_process_digital_audio(
                                 void FAR       *dest_buffer,
                                 S32             dest_buffer_size,
                                 U32             dest_rate,
                                 U32             dest_format,
                                 S32             num_srcs,
                                 AILMIXINFO FAR* src)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_process_digital_audio(" PTRFMT ",%li,%li,%li,%li," PTRFMT ")" NL,
          dest_buffer,dest_buffer_size,dest_rate,dest_format,num_srcs,src);
      }

   IN_AIL_NM;

   result = AIL_API_process_digital_audio(dest_buffer,dest_buffer_size,dest_rate,dest_format,num_srcs,src);

   OUT_AIL_NM;

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}


#ifdef IS_DOS

//#############################################################################
REALFAR AILEXPORT  AIL_get_real_vect             (U32       vectnum)
{
   REALFAR result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_get_real_vect(0x%X)" NL,vectnum);
      }

   result = AIL_API_get_real_vect(vectnum);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}
//#############################################################################
void    AILEXPORT  AIL_set_real_vect             (U32       vectnum,
                                              REALFAR     real_ptr)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_real_vect(0x%X,0x%X)" NL,vectnum,real_ptr);
      }

   AIL_API_set_real_vect(vectnum,real_ptr);

   END;
}

//#############################################################################
void    AILEXPORT  AIL_set_USE16_ISR             (S32        IRQ,
                                              REALFAR     real_base,
                                              U32       ISR_offset)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_USE16_ISR(%d,0x%X,%u)" NL,IRQ,real_base,ISR_offset);
      }

   AIL_API_set_USE16_ISR(IRQ,real_base,ISR_offset);

   END;
}

//#############################################################################
void    AILEXPORT  AIL_restore_USE16_ISR         (S32        IRQ)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_restore_USE16_ISR(%d)" NL,IRQ);
      }

   AIL_API_restore_USE16_ISR(IRQ);

   END;
}

//#############################################################################
U32   AILEXPORT  AIL_disable_interrupts        (void)
{
   U32 result;

   result = AIL_API_disable_interrupts();

   return result;
}

//#############################################################################
void    AILEXPORT  AIL_restore_interrupts        (U32       FD_register)
{
   AIL_API_restore_interrupts(FD_register);
}

//#############################################################################
S32    AILEXPORT  AIL_call_driver               (AIL_DRIVER *drvr,
                                              S32        fn,
                                              VDI_CALL   *in,
                                              VDI_CALL   *out)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_call_driver(" PTRFMT ",0x%X," PTRFMT "," PTRFMT ")" NL,
         drvr,fn,in,out);
      }

   result = AIL_API_call_driver(drvr,fn,in,out);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
S32    AILEXPORT  AIL_read_INI                  (AIL_INI    *INI,
                                              char      *filename)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_API_read_INI(" PTRFMT ",%s)" NL,INI,filename);
      }

   result = AIL_API_read_INI(INI,filename);

   if (result)
      {
      RESULT
         {
         INDENT;
         AIL_fprintf(AIL_debugfile,"Driver = %s" NL,INI->driver_name);
         INDENT;
         AIL_fprintf(AIL_debugfile,"Device = %s" NL,INI->device_name);
         INDENT;
         AIL_fprintf(AIL_debugfile,"IO     = %X" NL,INI->IO.IO);
         INDENT;
         AIL_fprintf(AIL_debugfile,"IRQ    = %d" NL,INI->IO.IRQ);
         INDENT;
         AIL_fprintf(AIL_debugfile,"DMA_8  = %d" NL,INI->IO.DMA_8_bit);
         INDENT;
         AIL_fprintf(AIL_debugfile,"DMA_16 = %d" NL,INI->IO.DMA_16_bit);
         }
      }
   else
      {
      RESULT
         {
         outresint((U32)result);
         }
      }

   END;

   return result;
}

//#############################################################################
IO_PARMS *   AILEXPORT AIL_get_IO_environment    (AIL_DRIVER *drvr)
{
   IO_PARMS *result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_get_IO_environment(" PTRFMT ")" NL,drvr);
      }

   result = AIL_API_get_IO_environment(drvr);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
AIL_DRIVER * AILEXPORT AIL_install_driver        (U8 const     *driver_image,
                                              U32       n_bytes)
{
   AIL_DRIVER *result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_driver(" PTRFMT ",%u)" NL,driver_image,n_bytes);
      }

   result = AIL_API_install_driver(driver_image,n_bytes);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
void         AILEXPORT AIL_uninstall_driver      (AIL_DRIVER *drvr)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_uninstall_driver(" PTRFMT ")" NL,drvr);
      }

   AIL_API_uninstall_driver(drvr);

   END;
}

//#############################################################################
S32 AILEXPORT AIL_install_DIG_INI(HDIGDRIVER *dig)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_DIG_INI(" PTRFMT ")" NL,dig);
      }

   result = AIL_API_install_DIG_INI(dig);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
HDIGDRIVER  AILEXPORT AIL_install_DIG_driver_file
                                             (char const *filename,
                                              IO_PARMS *IO)
{
   HDIGDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_DIG_driver_file(%s," PTRFMT ")" NL,filename,IO);
      }

   result = AIL_API_install_DIG_driver_file(filename,IO);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
void         AILEXPORT AIL_uninstall_DIG_driver  (HDIGDRIVER dig)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_uninstall_DIG_driver(" PTRFMT ")" NL,dig);
      }

   AIL_API_uninstall_DIG_driver(dig);

   END;
}

//#############################################################################
HDIGDRIVER  AILEXPORT AIL_install_DIG_driver_image
                                             (void const *driver_image,
                                              U32     size,
                                              IO_PARMS *IO)
{
   HDIGDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_DIG_driver_image(" PTRFMT ",%u," PTRFMT ")" NL,
         driver_image,size,IO);
      }

   result = AIL_API_install_DIG_driver_image(driver_image,size,IO);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
S32 AILEXPORT AIL_install_MDI_INI(HMDIDRIVER *mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_MDI_INI(" PTRFMT ")" NL,mdi);
      }

   result = AIL_API_install_MDI_INI(mdi);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
HMDIDRIVER  AILEXPORT AIL_install_MDI_driver_file
                                             (char const *filename,
                                              IO_PARMS   *IO)
{
   HMDIDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_MDI_driver_file(%s," PTRFMT ")" NL,filename,IO);
      }

   result = AIL_API_install_MDI_driver_file(filename,IO);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
void         AILEXPORT AIL_uninstall_MDI_driver  (HMDIDRIVER  mdi)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_uninstall_MDI_driver(" PTRFMT ")" NL,mdi);
      }

   AIL_API_uninstall_MDI_driver(mdi);

   END;
}

//#############################################################################
HMDIDRIVER  AILEXPORT AIL_install_MDI_driver_image
                                             (void const *driver_image,
                                              U32       size,
                                              IO_PARMS   *IO)
{
   HMDIDRIVER result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_MDI_driver_image(" PTRFMT ",%u," PTRFMT ")" NL,
         driver_image,size,IO);
      }

   result = AIL_API_install_MDI_driver_image(driver_image,size,IO);

   RESULT
      {
      outreshex((U32)result);
      }

   END;

   return result;
}

//#############################################################################
S32     AILEXPORT AIL_MDI_driver_type       (HMDIDRIVER mdi)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_MDI_driver_type(" PTRFMT ")" NL,mdi);
      }

   result = AIL_API_MDI_driver_type(mdi);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
void     AILEXPORT AIL_set_GTL_filename_prefix   (char const       *prefix)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_set_GTL_filename_prefix(%s)" NL,prefix);
      }

   AIL_API_set_GTL_filename_prefix(prefix);

   END;
}

//#############################################################################
S32     AILEXPORT AIL_timbre_status             (HMDIDRIVER  mdi,
                                              S32        bank,
                                              S32        patch)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_timbre_status(" PTRFMT ",%d,%d)" NL,mdi,bank,patch);
      }

   result = AIL_API_timbre_status(mdi,bank,patch);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
S32     AILEXPORT AIL_install_timbre            (HMDIDRIVER  mdi,
                                              S32        bank,
                                              S32        patch)
{
   S32 result;

   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_install_timbre(" PTRFMT ",%d,%d)" NL,mdi,bank,patch);
      }

   result = AIL_API_install_timbre(mdi,bank,patch);

   RESULT
      {
      outresint((U32)result);
      }

   END;

   return result;
}

//#############################################################################
void     AILEXPORT AIL_protect_timbre            (HMDIDRIVER  mdi,
                                              S32        bank,
                                              S32        patch)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_protect_timbre(" PTRFMT ",%d,%d)" NL,mdi,bank,patch);
      }

   AIL_API_protect_timbre(mdi,bank,patch);

   END;
}

//#############################################################################
void  AILEXPORT AIL_unprotect_timbre          (HMDIDRIVER  mdi,
                                              S32        bank,
                                              S32        patch)
{
   START
      {
      AIL_fprintf(AIL_debugfile,"AIL_unprotect_timbre(" PTRFMT ",%d,%d)" NL,mdi,bank,patch);
      }

   AIL_API_unprotect_timbre(mdi,bank,patch);

   END;
}


//############################################################################
//##                                                                        ##
//## End of locked code                                                     ##
//##                                                                        ##
//############################################################################

void __pascal AILDEBUG_end(void)
{
   if (locked)
      {
      AIL_vmm_unlock_range(AILDEBUG_start, AILDEBUG_end);

      locked = 0;

      }
}

#endif
