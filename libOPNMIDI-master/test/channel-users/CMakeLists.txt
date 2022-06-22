
set(CMAKE_CXX_STANDARD 11)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../common
                     ${CMAKE_SOURCE_DIR}/include
                     ${CMAKE_SOURCE_DIR}/src)

add_executable(ChannelUsersTest
               channel_users.cpp
                ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_midiplay.cpp
                ${libOPNMIDI_SOURCE_DIR}/src/opnmidi_opn2.cpp
                ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked_opn2.cpp
                ${libOPNMIDI_SOURCE_DIR}/src/chips/nuked/ym3438.c
               $<TARGET_OBJECTS:Catch-objects>)

set_target_properties(ChannelUsersTest PROPERTIES COMPILE_DEFINITIONS "GSL_THROW_ON_CONTRACT_VIOLATION")
target_compile_definitions(ChannelUsersTest PRIVATE
  OPNMIDI_DISABLE_MIDI_SEQUENCER
  OPNMIDI_DISABLE_GENS_EMULATOR
  OPNMIDI_DISABLE_MAME_EMULATOR
  OPNMIDI_DISABLE_GX_EMULATOR)
add_test(NAME ChannelUsersTest COMMAND ChannelUsersTest)

