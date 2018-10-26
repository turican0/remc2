/*
 * libADLMIDI is a free MIDI to WAV conversion library with OPL3 emulation
 *
 * Original ADLMIDI code: Copyright (c) 2010-2014 Joel Yliluoma <bisqwit@iki.fi>
 * ADLMIDI Library API:   Copyright (c) 2015-2018 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * Library is based on the ADLMIDI, a MIDI player for Linux and Windows with OPL3 emulation:
 * http://iki.fi/bisqwit/source/adlmidi.html
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPNMIDI_DISABLE_MIDI_SEQUENCER

// Rename class to avoid ABI collisions
#define BW_MidiSequencer OpnMidiSequencer
// Inlucde MIDI sequencer class implementation
#include "midi_sequencer_impl.hpp"

#include "opnmidi_midiplay.hpp"
#include "opnmidi_opn2.hpp"
#include "opnmidi_private.hpp"

/****************************************************
 *           Real-Time MIDI calls proxies           *
 ****************************************************/

static void rtNoteOn(void *userdata, uint8_t channel, uint8_t note, uint8_t velocity)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_NoteOn(channel, note, velocity);
}

static void rtNoteOff(void *userdata, uint8_t channel, uint8_t note)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_NoteOff(channel, note);
}

static void rtNoteAfterTouch(void *userdata, uint8_t channel, uint8_t note, uint8_t atVal)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_NoteAfterTouch(channel, note, atVal);
}

static void rtChannelAfterTouch(void *userdata, uint8_t channel, uint8_t atVal)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_ChannelAfterTouch(channel, atVal);
}

static void rtControllerChange(void *userdata, uint8_t channel, uint8_t type, uint8_t value)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_Controller(channel, type, value);
}

static void rtPatchChange(void *userdata, uint8_t channel, uint8_t patch)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_PatchChange(channel, patch);
}

static void rtPitchBend(void *userdata, uint8_t channel, uint8_t msb, uint8_t lsb)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_PitchBend(channel, msb, lsb);
}

static void rtSysEx(void *userdata, const uint8_t *msg, size_t size)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_SysEx(msg, size);
}


/* NonStandard calls */
static void rtDeviceSwitch(void *userdata, size_t track, const char *data, size_t length)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    context->realTime_deviceSwitch(track, data, length);
}

static size_t rtCurrentDevice(void *userdata, size_t track)
{
    OPNMIDIplay *context = reinterpret_cast<OPNMIDIplay *>(userdata);
    return context->realTime_currentDevice(track);
}
/* NonStandard calls End */


void OPNMIDIplay::initSequencerInterface()
{
    BW_MidiRtInterface *seq = new BW_MidiRtInterface;
    m_sequencerInterface.reset(seq);

    std::memset(seq, 0, sizeof(BW_MidiRtInterface));

    seq->onDebugMessage             = hooks.onDebugMessage;
    seq->onDebugMessage_userData    = hooks.onDebugMessage_userData;

    /* MIDI Real-Time calls */
    seq->rtUserData = this;
    seq->rt_noteOn  = rtNoteOn;
    seq->rt_noteOff = rtNoteOff;
    seq->rt_noteAfterTouch = rtNoteAfterTouch;
    seq->rt_channelAfterTouch = rtChannelAfterTouch;
    seq->rt_controllerChange = rtControllerChange;
    seq->rt_patchChange = rtPatchChange;
    seq->rt_pitchBend = rtPitchBend;
    seq->rt_systemExclusive = rtSysEx;

    /* NonStandard calls */
    seq->rt_deviceSwitch = rtDeviceSwitch;
    seq->rt_currentDevice = rtCurrentDevice;
    /* NonStandard calls End */

    m_sequencer->setInterface(seq);
}

double OPNMIDIplay::Tick(double s, double granularity)
{
    MidiSequencer &seqr = *m_sequencer;
    double ret = seqr.Tick(s, granularity);

    Synth &synth = *m_synth;
    s *= seqr.getTempoMultiplier();
    for(uint16_t c = 0; c < synth.m_numChannels; ++c)
        m_chipChannels[c].addAge(static_cast<int64_t>(s * 1e6));

    updateVibrato(s);
    updateArpeggio(s);
#if !defined(OPNMIDI_AUDIO_TICK_HANDLER)
    updateGlide(s);
#endif

    return ret;
}

#endif /* OPNMIDI_DISABLE_MIDI_SEQUENCER */
