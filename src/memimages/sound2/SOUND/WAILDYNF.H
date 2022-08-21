/*
 * $Source: x:/prj/tech/libsrc/sound/RCS/waildynf.h $
 * $Log: waildynf.h $
 * Revision 1.4  1996/11/07  16:03:34  PATMAC
 * Added DAIL_sequence_volume
 * 
 * Revision 1.3  1996/07/30  19:05:38  PATMAC
 * Add wail MEM calls
 * 
 * Revision 1.2  1996/07/30  17:50:59  PATMAC
 * anarkmd mc5 header comment style
 * 
 * Revision 1.1  1996/07/30  17:36:23  PATMAC
 * Initial revision
 *
 */

// Dynamic function loading

#ifndef __WAILDYNF_H
#define __WAILDYNF_H
#include <wail.h>
#include <dynfunc.h>

DeclDynFunc_( S32, WINAPI, AIL_startup, (void) );
#define DAIL_startup (DynFunc(AIL_startup).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_shutdown, (void) );
#define DAIL_shutdown (DynFunc(AIL_shutdown).GetProcAddress())

DeclDynFunc_( S32, WINAPI, AIL_set_preference, (U32 number, S32 value) );
#define DAIL_set_preference (DynFunc(AIL_set_preference).GetProcAddress())
                                                     
DeclDynFunc_( S32, WINAPI, AIL_midiOutOpen,
              (HMDIDRIVER FAR *drvr, LPHMIDIOUT FAR *lphMOut, U32 dwDevID) );
#define DAIL_midiOutOpen (DynFunc(AIL_midiOutOpen).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_midiOutClose, (HMDIDRIVER mdi) );
#define DAIL_midiOutClose (DynFunc(AIL_midiOutClose).GetProcAddress())

DeclDynFunc_( HSEQUENCE, WINAPI, AIL_allocate_sequence_handle, (HMDIDRIVER mdi) );
#define DAIL_allocate_sequence_handle (DynFunc(AIL_allocate_sequence_handle).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_release_sequence_handle, (HSEQUENCE S) );
#define DAIL_release_sequence_handle (DynFunc(AIL_release_sequence_handle).GetProcAddress())

DeclDynFunc_( S32, WINAPI, AIL_init_sequence,
              (HSEQUENCE S, void FAR *start, S32 sequence_num) );
#define DAIL_init_sequence (DynFunc(AIL_init_sequence).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_start_sequence, (HSEQUENCE S) );
#define DAIL_start_sequence (DynFunc(AIL_start_sequence).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_stop_sequence, (HSEQUENCE S) );
#define DAIL_stop_sequence (DynFunc(AIL_stop_sequence).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_resume_sequence, (HSEQUENCE S) );
#define DAIL_resume_sequence (DynFunc(AIL_resume_sequence).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_end_sequence, (HSEQUENCE S) );
#define DAIL_end_sequence (DynFunc(AIL_end_sequence).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_set_sequence_tempo,
              (HSEQUENCE S, S32 tempo, S32 milliseconds) );
#define DAIL_set_sequence_tempo (DynFunc(AIL_set_sequence_tempo).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_set_sequence_volume,
              (HSEQUENCE S, S32 volume, S32 milliseconds) );
#define DAIL_set_sequence_volume (DynFunc(AIL_set_sequence_volume).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_set_sequence_loop_count,
              (HSEQUENCE S, S32 loop_count) );
#define DAIL_set_sequence_loop_count (DynFunc(AIL_set_sequence_loop_count).GetProcAddress())

DeclDynFunc_( U32, WINAPI, AIL_sequence_status, (HSEQUENCE S) );
#define DAIL_sequence_status (DynFunc(AIL_sequence_status).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_set_XMIDI_master_volume,
              (HMDIDRIVER mdi, S32 master_volume) );
#define DAIL_set_XMIDI_master_volume (DynFunc(AIL_set_XMIDI_master_volume).GetProcAddress())

                                                     
DeclDynFunc_( void, WINAPI, AIL_sequence_position,
              (HSEQUENCE S, S32 FAR *beat, S32 FAR *measure) );
#define DAIL_sequence_position (DynFunc(AIL_sequence_position).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_branch_index,
              (HSEQUENCE S, U32 marker) );
#define DAIL_branch_index (DynFunc(AIL_branch_index).GetProcAddress())

DeclDynFunc_( AILPREFIXCB, WINAPI, AIL_register_prefix_callback,
              (HSEQUENCE S, AILPREFIXCB callback) );
#define DAIL_register_prefix_callback (DynFunc(AIL_register_prefix_callback).GetProcAddress())

                                                     
DeclDynFunc_( AILTRIGGERCB, WINAPI, AIL_register_trigger_callback,
              (HSEQUENCE S, AILTRIGGERCB callback) );
#define DAIL_register_trigger_callback (DynFunc(AIL_register_trigger_callback).GetProcAddress())

DeclDynFunc_( AILSEQUENCECB, WINAPI, AIL_register_sequence_callback ,
              (HSEQUENCE S, AILSEQUENCECB callback) );
#define DAIL_register_sequence_callback (DynFunc(AIL_register_sequence_callback).GetProcAddress())

DeclDynFunc_( AILBEATCB, WINAPI, AIL_register_beat_callback,
              (HSEQUENCE S, AILBEATCB callback) );
#define DAIL_register_beat_callback (DynFunc(AIL_register_beat_callback).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_set_sequence_user_data,
              (HSEQUENCE S, U32 index, S32 value) );
#define DAIL_set_sequence_user_data (DynFunc(AIL_set_sequence_user_data).GetProcAddress())

DeclDynFunc_( S32, WINAPI, AIL_sequence_user_data,
              (HSEQUENCE S, U32  index) );
#define DAIL_sequence_user_data (DynFunc(AIL_sequence_user_data).GetProcAddress())

DeclDynFunc_( void, WINAPI, AIL_register_ICA_array,
              (HSEQUENCE S, S8 FAR *array) );
#define DAIL_register_ICA_array (DynFunc(AIL_register_ICA_array).GetProcAddress())

DeclDynFunc_( void*, WINAPI, MEM_alloc_lock, (U32 size) );
#define DMEM_alloc_lock (DynFunc(MEM_alloc_lock).GetProcAddress())

DeclDynFunc_( void, WINAPI, MEM_free_lock, (void FAR *ptr) );
#define DMEM_free_lock (DynFunc(MEM_free_lock).GetProcAddress())

DeclDynFunc_( int32, WINAPI, AIL_sequence_volume, (HSEQUENCE S) );
#define DAIL_sequence_volume (DynFunc(AIL_sequence_volume).GetProcAddress())

BOOL LoadWail();

#endif /* !__WAILDYNF_H */
