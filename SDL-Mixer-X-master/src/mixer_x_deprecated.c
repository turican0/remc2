/*
  SDL_mixer:  An audio mixer library based on the SDL library
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "SDL_mixer_ext.h"

/*
    Deprecated SDL Mixer X's functions.
    There are kept to don't break ABI with existing apps.
*/

void SDLCALLCC MIX_Timidity_addToPathList(const char *path)
{
    Mix_Timidity_addToPathList(path);
}

const char *const *SDLCALLCC  MIX_ADLMIDI_getBankNames()
{
    return Mix_ADLMIDI_getBankNames();
}

int SDLCALLCC MIX_ADLMIDI_getBankID()
{
    return Mix_ADLMIDI_getBankID();
}

void SDLCALLCC MIX_ADLMIDI_setBankID(int bnk)
{
    Mix_ADLMIDI_setBankID(bnk);
}

int SDLCALLCC MIX_ADLMIDI_getTremolo()
{
    return Mix_ADLMIDI_getTremolo();
}

void SDLCALLCC MIX_ADLMIDI_setTremolo(int tr)
{
    Mix_ADLMIDI_setTremolo(tr);
}

int SDLCALLCC MIX_ADLMIDI_getVibrato()
{
    return Mix_ADLMIDI_getVibrato();
}

void SDLCALLCC MIX_ADLMIDI_setVibrato(int vib)
{
    Mix_ADLMIDI_setVibrato(vib);
}

int SDLCALLCC MIX_ADLMIDI_getScaleMod()
{
    return Mix_ADLMIDI_getScaleMod();
}

void SDLCALLCC MIX_ADLMIDI_setScaleMod(int sc)
{
    Mix_ADLMIDI_setScaleMod(sc);
}

int SDLCALLCC MIX_ADLMIDI_getAdLibMode()
{
    return Mix_ADLMIDI_getAdLibMode();
}

void SDLCALLCC MIX_ADLMIDI_setAdLibMode(int sc)
{
    Mix_ADLMIDI_setAdLibMode(sc);
}

void SDLCALLCC MIX_ADLMIDI_setSetDefaults()
{
    Mix_ADLMIDI_setSetDefaults();
}

int SDLCALLCC MIX_ADLMIDI_getLogarithmicVolumes()
{
    return Mix_ADLMIDI_getLogarithmicVolumes();
}

void SDLCALLCC MIX_ADLMIDI_setLogarithmicVolumes(int lv)
{
    Mix_ADLMIDI_setLogarithmicVolumes(lv);
}

int SDLCALLCC MIX_ADLMIDI_getVolumeModel()
{
    return Mix_ADLMIDI_getVolumeModel();
}

void SDLCALLCC MIX_ADLMIDI_setVolumeModel(int vm)
{
    Mix_ADLMIDI_setVolumeModel(vm);
}


void SDLCALLCC MIX_OPNMIDI_setCustomBankFile(const char *bank_wonp_path)
{
    Mix_OPNMIDI_setCustomBankFile(bank_wonp_path);
}


int SDLCALLCC MIX_SetMidiDevice(int device)
{
    return Mix_SetMidiPlayer(device);
}

void SDLCALLCC MIX_SetLockMIDIArgs(int lock_midiargs)
{
    Mix_SetLockMIDIArgs(lock_midiargs);
}

int SDLCALLCC Mix_GetMidiDevice()
{
    return Mix_GetMidiPlayer();
}

int SDLCALLCC Mix_GetNextMidiDevice()
{
    return Mix_GetNextMidiPlayer();
}

int SDLCALLCC Mix_SetMidiDevice(int player)
{
    return Mix_SetMidiPlayer(player);
}
