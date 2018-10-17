///////////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/waildynf.cpp $
// $Author: PATMAC $
// $Date: 1996/11/07 16:03:19 $
// $Revision: 1.4 $
// $Log: waildynf.cpp $
//Revision 1.4  1996/11/07  16:03:19  PATMAC
//Added DAIL_sequence_volume
//
//Revision 1.3  1996/10/10  15:38:12  TOML
//msvc
//
//Revision 1.2  1996/07/30  19:05:10  PATMAC
//Add wail MEM calls
//
//Revision 1.1  1996/07/30  17:36:06  PATMAC
//Initial revision
//
//
#include <windows.h>
#include <lg.h>
#include <dsound.h>

#include "waildynf.h"

//
// All the WAIL entry points we use
//
ImplDynFunc(AIL_startup,                    "wail32.dll", "_AIL_startup@0", 0);
ImplDynFunc(AIL_shutdown,                   "wail32.dll", "_AIL_shutdown@0", 0);
ImplDynFunc(AIL_set_preference,             "wail32.dll", "_AIL_set_preference@8", 0);
ImplDynFunc(AIL_midiOutOpen,                "wail32.dll", "_AIL_midiOutOpen@12", 0);
ImplDynFunc(AIL_midiOutClose,               "wail32.dll", "_AIL_midiOutClose@4", 0);
ImplDynFunc(AIL_allocate_sequence_handle,   "wail32.dll", "_AIL_allocate_sequence_handle@4", 0);
ImplDynFunc(AIL_release_sequence_handle,    "wail32.dll", "_AIL_release_sequence_handle@4", 0);
ImplDynFunc(AIL_init_sequence,              "wail32.dll", "_AIL_init_sequence@12", 0);
ImplDynFunc(AIL_start_sequence,             "wail32.dll", "_AIL_start_sequence@4", 0);
ImplDynFunc(AIL_stop_sequence,              "wail32.dll", "_AIL_stop_sequence@4", 0);
ImplDynFunc(AIL_resume_sequence,            "wail32.dll", "_AIL_resume_sequence@4", 0);
ImplDynFunc(AIL_end_sequence,               "wail32.dll", "_AIL_end_sequence@4", 0);
ImplDynFunc(AIL_set_sequence_tempo,         "wail32.dll", "_AIL_set_sequence_tempo@12", 0);
ImplDynFunc(AIL_set_sequence_volume,        "wail32.dll", "_AIL_set_sequence_volume@12", 0);
ImplDynFunc(AIL_set_sequence_loop_count,    "wail32.dll", "_AIL_set_sequence_loop_count@8", 0);
ImplDynFunc(AIL_sequence_status,            "wail32.dll", "_AIL_sequence_status@4", 0);
ImplDynFunc(AIL_set_XMIDI_master_volume,    "wail32.dll", "_AIL_set_XMIDI_master_volume@8", 0);
ImplDynFunc(AIL_sequence_position,          "wail32.dll", "_AIL_sequence_position@12", 0);
ImplDynFunc(AIL_branch_index,               "wail32.dll", "_AIL_branch_index@8", 0);
ImplDynFunc(AIL_register_prefix_callback,   "wail32.dll", "_AIL_register_prefix_callback@8", 0);
ImplDynFunc(AIL_register_trigger_callback,  "wail32.dll", "_AIL_register_trigger_callback@8", 0);
ImplDynFunc(AIL_register_sequence_callback, "wail32.dll", "_AIL_register_sequence_callback@8", 0);
ImplDynFunc(AIL_register_beat_callback,     "wail32.dll", "_AIL_register_beat_callback@8", 0);
ImplDynFunc(AIL_set_sequence_user_data,     "wail32.dll", "_AIL_set_sequence_user_data@12", 0);
ImplDynFunc(AIL_sequence_user_data,         "wail32.dll", "_AIL_sequence_user_data@8", 0);
ImplDynFunc(AIL_register_ICA_array,         "wail32.dll", "_AIL_register_ICA_array@8", 0);
ImplDynFunc(MEM_free_lock,                  "wail32.dll", "_MEM_free_lock@4", 0);
ImplDynFunc(MEM_alloc_lock,                 "wail32.dll", "_MEM_alloc_lock@4", 0);
ImplDynFunc(AIL_sequence_volume,            "wail32.dll", "_AIL_sequence_volume@4", 0);

BOOL LoadWail()
{
    return DynFunc(AIL_startup).Load();
}

