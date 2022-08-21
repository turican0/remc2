//############################################################################
//##                                                                        ##
//##  FLT.CPP: Wrapper for pipeline filter providers                        ##
//##                                                                        ##
//##  32-bit protected-mode source compatible with MSC 11.0/Watcom 10.6     ##
//##                                                                        ##
//##  Version 1.00 of 7-Feb-99: Initial                                     ##
//##                                                                        ##
//##  Author: John Miles                                                    ##
//##                                                                        ##
//############################################################################
//##                                                                        ##
//##  Copyright (C) RAD Game Tools, Inc.                                    ##
//##                                                                        ##
//##  Contact RAD Game Tools at 425-893-4300 for technical support.         ##
//##                                                                        ##
//############################################################################

#include "rib.h"
#include "mss.h"
#include "imssapi.h"

static FLTPROVIDER FAR *first = NULL;

//############################################################################
//#                                                                          #
//# FLT_init_list()                                                          #
//#                                                                          #
//############################################################################

void FLT_init_list(void)
{
   first = NULL;
}

//############################################################################
//#                                                                          #
//# FLT_find_provider_instance()                                             #
//#                                                                          #
//############################################################################

FLTPROVIDER FAR *FLT_find_provider_instance(HPROVIDER  provider, //)
                                            HDIGDRIVER dig)
{
   FLTPROVIDER FAR *F = first;

   while (F != NULL)
      {
      if ((F->dig      == dig) &&
          (F->provider == provider))
         {
         return F;
         }

      F = F->next;
      }

   return 0;
}

//############################################################################
//#                                                                          #
//# FLT_disconnect_driver()                                                  #
//#                                                                          #
//############################################################################

void FLT_disconnect_driver(HDIGDRIVER dig)
{
   //
   // Kill all filter descriptors associated with this driver
   //

   FLTPROVIDER FAR *F = first;

   while (F != NULL)
      {
      if (F->dig == dig)
         {
         AIL_close_filter(HDRIVERSTATE(F));
         F = first;
         continue;
         }

      F = F->next;
      }
}

//############################################################################
//#                                                                          #
//# FLT_call_premix_processors()                                             #
//#                                                                          #
//############################################################################

void FLT_call_premix_processors(HDIGDRIVER dig)
{
   //
   // For each filter associated with this driver...
   //

   FLTPROVIDER FAR *F = first;

   while (F != NULL)
      {
      if (F->dig == dig)
         {
         //
         // Call this filter's premix processor
         //

         F->premix_process(F->driver_state);
         }

      F = F->next;
      }
}

//############################################################################
//#                                                                          #
//# FLT_call_postmix_processors()                                            #
//#                                                                          #
//############################################################################

S32 FLT_call_postmix_processors(HDIGDRIVER dig, //)
                                void FAR  *output_buffer)
{
   S32 inhibit_copy = FALSE;

   //
   // For each filter associated with this driver...
   //

   FLTPROVIDER FAR *F = first;

   while (F != NULL)
      {
      if (F->dig == dig)
         {
         //
         // Call this filter's postmix processor
         //

         if (F->postmix_process(F->driver_state, output_buffer))
            {
            inhibit_copy = TRUE;
            }
         }

      F = F->next;
      }

   //
   // Return TRUE if any of the postmix processors handled the task of copying data from the build 
   // buffer(s) to the output buffer(s)
   //

   return inhibit_copy;
}

//############################################################################
//#                                                                          #
//# AIL_API_enumerate_filters()                                              #
//#                                                                          #
//############################################################################

S32        AILCALL AIL_API_enumerate_filters  (HPROENUM  FAR *next, //)
                                               HPROVIDER FAR *dest,
                                               C8  FAR * FAR *name)
{
   HPROPERTY                PROVFLAGS;
   PROVIDER_PROPERTY PROVIDER_property = NULL;

   RIB_INTERFACE_ENTRY FLT[] = 
      { 
      PR("Flags",PROVFLAGS),
      FN(PROVIDER_property)
      };

   //
   // Only sample-pipeline filters are returned here
   // We want to skip matrix postmix filters, and any others that aren't explicitly marked FPROV_ENUMERABLE
   //

   for(;;)
      {
      if (!RIB_enumerate_providers("MSS pipeline filter",
                                    next,
                                    dest))
         {
         return 0;
         }

      RIB_request(*dest,"MSS pipeline filter",FLT);

      S32 flags = 0; PROVIDER_property(PROVFLAGS,&flags,0,0);

      if (!(flags & FPROV_ON_SAMPLES))
         {
         continue;
         }

      return( PROVIDER_property( PROVIDER_NAME, name, 0, 0 ) );
      }
}

//############################################################################
//#                                                                          #
//# AIL_API_open_filter()                                                    #
//#                                                                          #
//############################################################################

HDRIVERSTATE AILCALL AIL_API_open_filter (HPROVIDER  provider, //)
                                          HDIGDRIVER dig)
{
   //
   // If filter already associated with driver, return -1
   //

   FLTPROVIDER FAR *F = first;

   while (F != NULL)
      {
      if ((F->dig      == dig) &&
          (F->provider == provider))
         {
         return -1;
         }

      F = F->next;
      }

   //
   // Allocate our provider descriptor
   //

   FLTPROVIDER FAR *FLT = (FLTPROVIDER FAR *) AIL_mem_alloc_lock(sizeof(FLTPROVIDER));

   if (FLT == NULL)
      {
      AIL_set_error("Out of memory");
      return 0;
      }

   AIL_memset(FLT, 0, sizeof(FLTPROVIDER));

   //
   // Fill in provider members
   //

   HPROPERTY flags_attr = 0;

   RIB_INTERFACE_ENTRY FLT_request1[] =
      {
      { RIB_FUNCTION,  "PROVIDER_property",              (UINTa) &FLT->PROVIDER_property          ,RIB_NONE },
      { RIB_PROPERTY,  "Flags",                          (UINTa) &flags_attr                      ,RIB_NONE },
      { RIB_FUNCTION,  "FLT_startup",                    (UINTa) &FLT->startup                    ,RIB_NONE },
      { RIB_FUNCTION,  "FLT_error",                      (UINTa) &FLT->error                      ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY FLT_request2[] =
      {
      { RIB_FUNCTION,  "FLT_shutdown",                   (UINTa) &FLT->shutdown                   ,RIB_NONE },
      { RIB_FUNCTION,  "FLT_open_driver",                (UINTa) &FLT->open_driver                ,RIB_NONE },
      { RIB_FUNCTION,  "FLT_close_driver",               (UINTa) &FLT->close_driver               ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY FLT_request3[] =
      {
      { RIB_FUNCTION,  "FLT_premix_process",             (UINTa) &FLT->premix_process             ,RIB_NONE },
      { RIB_FUNCTION,  "FLT_postmix_process",            (UINTa) &FLT->postmix_process            ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY SMP_request1[] =
      {
      { RIB_FUNCTION,  "FLTSMP_open_sample",             (UINTa) &FLT->open_sample                ,RIB_NONE },
      { RIB_FUNCTION,  "FLTSMP_close_sample",            (UINTa) &FLT->close_sample               ,RIB_NONE },
      { RIB_FUNCTION,  "FLTSMP_sample_process",          (UINTa) &FLT->sample_process             ,RIB_NONE },
      { RIB_FUNCTION,  "FLTSMP_sample_property",         (UINTa) &FLT->sample_property            ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY VFLT_request1[] =
      {
      { RIB_FUNCTION,  "PROVIDER_property",              (UINTa) &FLT->PROVIDER_property          ,RIB_NONE },
      { RIB_PROPERTY,  "Flags",                          (UINTa) &flags_attr                      ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_startup",                   (UINTa) &FLT->startup                    ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_error",                     (UINTa) &FLT->error                      ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY VFLT_request2[] =
      {
      { RIB_FUNCTION,  "VFLT_shutdown",                  (UINTa) &FLT->shutdown                   ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_open_driver",               (UINTa) &FLT->open_driver                ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_close_driver",              (UINTa) &FLT->close_driver               ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY VFLT_request3[] =
      {
      { RIB_FUNCTION,  "VFLT_assign_sample_voice",       (UINTa) &FLT->assign_sample_voice        ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_release_sample_voice",      (UINTa) &FLT->release_sample_voice       ,RIB_NONE },
      { RIB_FUNCTION,  "VFLT_start_sample_voice",        (UINTa) &FLT->start_sample_voice         ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY VDRV_request[] =
      {
      { RIB_FUNCTION,  "VDRV_driver_property",           (UINTa) &FLT->driver_property            ,RIB_NONE },
      { RIB_FUNCTION,  "VDRV_force_update",              (UINTa) &FLT->force_update               ,RIB_NONE },
      };

   RIB_INTERFACE_ENTRY VSMP_request[] =
      {
      { RIB_FUNCTION,  "VSMP_sample_property",           (UINTa) &FLT->output_sample_property     ,RIB_NONE },
      };

   RIBRESULT result1 = RIB_request(provider,"MSS pipeline filter",FLT_request1);
   RIBRESULT result2 = RIB_request(provider,"MSS pipeline filter",FLT_request2);
   RIBRESULT result3 = RIB_request(provider,"MSS pipeline filter",FLT_request3);

   if ((result1 == RIB_NOERR) && (result2 == RIB_NOERR) && (result3 == RIB_NOERR))
      {
      RIB_request(provider,"Pipeline filter sample services",SMP_request1);
      }
   else
      {
#ifdef MSS_VFLT_SUPPORTED
      //
      // If "MSS voice filter" is provided, we're installing a per-voice output filter
      // There can be only one
      //

      RIBRESULT result1 = RIB_request(provider,"MSS voice filter",VFLT_request1);
      RIBRESULT result2 = RIB_request(provider,"MSS voice filter",VFLT_request2);
      RIBRESULT result3 = RIB_request(provider,"MSS voice filter",VFLT_request3);

      if ((result1 == RIB_NOERR) && (result2 == RIB_NOERR) && (result3 == RIB_NOERR))
         {
         if (dig->voice_filter != NULL)
            {
            AIL_set_error("Voice filter already associated with driver");
            AIL_mem_free_lock(FLT);
            return 0;
            }

         dig->voice_filter    = FLT;
         dig->stream_callback = SS_stream_to_buffer;

         RIB_request(provider,"Voice filter driver services",VDRV_request);
         RIB_request(provider,"Voice filter sample services",VSMP_request);
         }
      else
#endif
         {
         //
         // This provider doesn't offer any filter services at all
         //

         AIL_set_error("Not a filter provider");
         AIL_mem_free_lock( FLT );
         return 0;
         }
      }

   //
   // Store FPROV flags with provider descriptor
   //

   FLT->provider_flags = -1; FLT->PROVIDER_property(flags_attr,&FLT->provider_flags,0,0);

   //
   // If this is a matrix filter, there can be only one per driver...
   //

   if (FLT->provider_flags & FPROV_MATRIX)
      {
      if (dig->matrix_filter != NULL)
         {
         AIL_set_error("Matrix filter already associated with driver");
         AIL_mem_free_lock(FLT);
         return 0;
         }

      dig->matrix_filter = FLT;
      }

   //
   // Activate provider, storing HPROVIDER, DRIVERSTATE and original
   // HDIGDRIVER in our wrapper descriptor
   //
   // The activation/shutdown process is the same for both pipeline and voice-output filters...
   //

   OutMilesMutex();
   FLT->driver_state = FLT->open_driver(dig);
   InMilesMutex();

   if (FLT->driver_state==0)
      {
      C8 *txt = FLT->error();

      if (txt == NULL)
         {
         AIL_set_error("Error loading filter");
         }
      else
         {
         AIL_set_error(txt);
         }

#ifdef MSS_VFLT_SUPPORTED
      if (dig->voice_filter == FLT) 
         {
         dig->voice_filter = NULL;
         }
#endif

      AIL_mem_free_lock(FLT);
      return 0;
      }

   FLT->dig = dig;

   FLT->provider = provider;

   //
   // Link filter descriptor into list
   //

   FLT->next = first;
   first     = FLT;

   //
   // Return our wrapper descriptor's address as the HDRIVERSTATE from the
   // app's point of view
   //

   return (HDRIVERSTATE) FLT;
}

//############################################################################
//#                                                                          #
//# AIL_API_close_filter()                                                   #
//#                                                                          #
//############################################################################

void       AILCALL AIL_API_close_filter       (HDRIVERSTATE filter)
{
   FLTPROVIDER FAR *FLT = (FLTPROVIDER FAR *) filter;

   if (FLT == NULL)
      {
      return;
      }

   //
   // Deactivate provider
   //

   FLT->close_driver(FLT->driver_state);

   //
   // Unlink descriptor from list
   //

   if (first == FLT)
      {
      first = FLT->next;
      }
   else
      {
      FLTPROVIDER FAR *link = first;
      FLTPROVIDER FAR *prev = NULL;

      while (link != FLT)
         {
         prev = link;
         link = link->next;
         }

      prev->next = FLT->next;
      }

   //
   // Uninstall output filter from driver, if appropriate
   //

#ifdef MSS_VFLT_SUPPORTED
   if (FLT->dig->voice_filter == FLT)
      {
      FLT->dig->voice_filter = NULL;
      }
#endif

   if (FLT->dig->matrix_filter == FLT)
      {
      FLT->dig->matrix_filter = NULL;
      }

   //
   // Free descriptor structure
   //

   AIL_mem_free_lock(FLT);
}
