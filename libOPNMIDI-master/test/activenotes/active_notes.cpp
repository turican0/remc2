#include <catch.hpp>
#include <string>
#include <vector>

#include "opnmidi_midiplay.hpp"
#include "opnmidi_opn2.hpp"
#include "opnmidi_private.hpp"

TEST_CASE( "MIDI Channel manipulating", "[OPNMIDIplay::MIDIchannel]" )
{
    OPNMIDIplay::MIDIchannel midi_ch;
    //OPNMIDIplay::OpnChannel  opn_ch; //Must be tested separately, but togerther with MIDI Channel as there are in symbiosis

    SECTION("Turning notes On!")
    {
        for(uint8_t noteq = 0; noteq < 128; noteq++)
        {
            uint8_t note = rand() % 128;
            {//Call note off if note is already in work
                OPNMIDIplay::MIDIchannel::activenoteiterator
                i = midi_ch.activenotes_find(note);
                if(i)
                    midi_ch.activenotes_erase(i);
            }

            const opnInstMeta2 &ains = Synth::m_emptyInstrument;

            OPNMIDIplay::MIDIchannel::NoteInfo::Phys voices[OPNMIDIplay::MIDIchannel::NoteInfo::MaxNumPhysChans] = {
                {0, ains.opn[0], /*false*/},
                {0, ains.opn[1], /*pseudo_4op*/},
            };

            std::pair<OPNMIDIplay::MIDIchannel::activenoteiterator, bool>
            ir = midi_ch.activenotes_insert(note);
            ir.first->vol     = rand() % 127;
            ir.first->noteTone    = rand() % 127;
            ir.first->midiins = rand() % 127;
            ir.first->currentTone = (double)(rand() % 127);
            ir.first->glideRate = (double)(rand() % 127);
            ir.first->ains = &ains;
            ir.first->chip_channels_count = 0;

            for(unsigned ccount = 0; ccount < OPNMIDIplay::MIDIchannel::NoteInfo::MaxNumPhysChans; ++ccount)
            {
                int32_t c = rand() % 2;
                if(c < 0)
                    continue;
                uint16_t chipChan = static_cast<uint16_t>(rand() % 256);
                OPNMIDIplay::MIDIchannel::NoteInfo::Phys * p = ir.first->phys_ensure_find_or_create(chipChan);
                REQUIRE( p != nullptr );
                p->assign(voices[ccount]);
            }
        }
    }

    SECTION("Turning another random notes Off!")
    {
        for(uint8_t noteq = 0; noteq < 128; noteq++)
        {
            uint8_t note = rand() % 128;
            OPNMIDIplay::MIDIchannel::activenoteiterator
            i = midi_ch.activenotes_find(note);
            if(i)
                midi_ch.activenotes_erase(i);
        }
    }

    SECTION("Iterating notes are left!")
    {
        for(OPNMIDIplay::MIDIchannel::activenoteiterator i = midi_ch.activenotes_begin(); i;)
        {
            OPNMIDIplay::MIDIchannel::activenoteiterator j(i++);
            REQUIRE( j );
        }
    }
}

