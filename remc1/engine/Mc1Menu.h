#pragma once

#include <cstdint>

#include "Mc1Files.h"

// MC1 menu state and the sound setup it edits (C:\carpet.cd\sndsetup.dat, sndsetup.inf)

// sound setup: the text shown in the menu and the value written to sndsetup.inf
extern char soundDriver_12C9A0_12C990[32]; // "NONE", SOUNDFX driver
extern char soundCardText_12C9C0_12C9B0[32];
extern char musicCardText_12C9E0_12C9D0[32];
extern char musicDriver_12CA00_12C9F0[32]; // "NONE", MUSIC driver
extern char saveSetupText_12CA20_12CA10[64];
extern char soundPortText_12CA80_12CA70[10];
extern char soundIrqText_12CA8A_12CA7A[10];
extern char musicPortText_12CA94_12CA84[10];
extern char soundPort_12CA9E_12CA8E[10];
extern char musicPort_12CAA8_12CA98[10];
extern char soundIrq_12CAB2_12CAA2[10];
extern char saveSetup_12CABC_12CAAC[10]; // "YES" / "NO"
extern char soundDma_12CAC6_12CAB6[10];
extern char soundDmaText_12CAD0_12CAC0[12];

// menu sprites, palette and screen backups (group at 49B80 / 49EC0 of the code segment)
extern uint8_t* menuSpritesTab_12CAEC_12CADC;
extern uint8_t* menuSpritesTabEnd_12CAF0_12CAE0;
extern uint8_t* menuSpritesData_12CAF4_12CAE4;
extern uint8_t* menuPalette_12CB9C_12CB8C;
extern uint8_t* screenBackup_12CBA4_12CB94;
extern uint8_t* screenBackup2_12CBA8_12CB98;
extern bitmap_pos_struct_t* menuSpritesIndex; // port: the tab with real pointers
extern Type_Mc1FileEntry menuFiles_49B80_49EC0[6];
extern uint32_t loadMenuFiles_12CA7C_12CA6C;

// language screen sprites (group at 4A050 / 4A390 of the code segment)
extern uint8_t* languageSpritesTab_12CB58_12CB48;
extern uint8_t* languageSpritesTabEnd_12CB5C_12CB4C;
extern uint8_t* languageSpritesData_12CB60_12CB50;
extern bitmap_pos_struct_t* languageSpritesIndex; // port: the tab with real pointers
extern Type_Mc1FileEntry languageFiles_4A050_4A390[3];

// menu input: 3 bits per button / key (pressed, released, held), see UpdateMenuInput_50770_50AB0
extern int16_t mouseX_12CADC_12CACC;
extern int16_t mouseY_12CADE_12CACE;
extern int16_t clickX_12CAE0_12CAD0;
extern int16_t clickY_12CAE2_12CAD2;
extern uint8_t inputFlags_12CAE4_12CAD4[5];

// menu state
extern uint32_t dword_12CBB0_12CBA0;
extern int16_t word_12CBBC_12CBAC;
extern int16_t word_12CBBE_12CBAE;
extern int16_t word_12CBC0_12CBB0;
extern int16_t word_12CBC2_12CBB2;
extern uint8_t byte_12CBC6_12CBB6;
extern uint8_t menuPage_12CBC9_12CBB9; // 0 sound setup, 7 with sndsetup.dat
extern uint8_t byte_12CBCA_12CBBA;
extern uint8_t byte_12CBCC_12CBBC;
extern uint8_t byte_12CBCD_12CBBD;
extern uint8_t mainMenuItem_12CBCE_12CBBE;
extern uint8_t inputMode_12CBCF_12CBBF; // 0 none, 1 keyboard, 2 mouse
extern uint8_t byte_12CBD0_12CBC0;
extern uint8_t byte_12CBD1_12CBC1;
extern uint8_t byte_12CBD2_12CBC2;
extern uint8_t byte_12CBD3_12CBC3;
extern uint8_t byte_12CBD4_12CBC4;
extern uint8_t menuFlags_12CBD5_12CBC5; // 2 intro.pld existed
extern uint8_t byte_12CBD6_12CBC6;

void InitMenu_4A980_4ACC0();
void RunMenu_4AB20_4AE60();
void UpdateMenuInput_50770_50AB0();
void SelectLanguage_4F3F0_4F730();
int32_t LoadSoundSetup_505A0_508E0();
