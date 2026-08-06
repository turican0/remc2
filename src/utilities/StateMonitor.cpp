#include "StateMonitor.h"

#include <vector>
#include <algorithm>
#ifdef REMC2_CODE
#include <fmt/core.h>
#endif //REMC2_CODE

#include "portability/port_filesystem.h"

StateMonitor g_state_monitor;

StateMonitor::StateMonitor() {
}

StateMonitor::~StateMonitor() {
#ifdef REMC2_CODE
    if (!m_initialized) {
        return;
    }
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_renderer = nullptr;
    m_window = nullptr;
#endif //REMC2_CODE
}

// Comparator function to sort pairs 
bool cmp(pair<string, ObservedChange>& a, pair<string, ObservedChange>& b) 
{ 
    return a.second.time_to_live > b.second.time_to_live; 
} 
 
void StateMonitor::Init() {
#ifdef REMC2_CODE
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    uint32_t redMask = 0xff000000;
    uint32_t greenMask = 0x00ff0000;
    uint32_t blueMask = 0x0000ff00;
    uint32_t alphaMask = 0x000000ff;
#else
    uint32_t redMask = 0x000000ff;
    uint32_t greenMask = 0x0000ff00;
    uint32_t blueMask = 0x00ff0000;
    uint32_t alphaMask = 0xff000000;
#endif

    m_window = SDL_CreateWindow("StateMonitor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, STATE_MONITOR_WIDTH, STATE_MONITOR_HEIGHT, 0);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawColor(m_renderer, 255,255,255,250);

    m_surface = SDL_CreateRGBSurface(
        SDL_SWSURFACE, STATE_MONITOR_WIDTH, STATE_MONITOR_HEIGHT, 24,
        redMask, greenMask, blueMask, alphaMask);
    m_surface = SDL_ConvertSurfaceFormat(
        m_surface, SDL_PIXELFORMAT_RGB888, 0);
    m_texture = SDL_CreateTexture(m_renderer,
        SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, m_surface->w, m_surface->h);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    TTF_Init();
    m_font = TTF_OpenFont(GetSubDirectoryPath("font/UbuntuMono-B.ttf").c_str(), 18);

    m_initialized = true;
#endif //REMC2_CODE
}

void StateMonitor::Update() {
    if (!m_initialized) {
        return;
    }

    // reduce time to live for all observed changes
    for (auto& change : m_observed_changes) {
        change.second.time_to_live--;
        if (change.second.time_to_live <= 0) {
            m_observed_changes.erase(change.first);
        }
    }

    // check for changes
    MonitorD41A0();
    Monitor17DB70str();

    // create vector of all changes sorted by time to live
    vector<pair<string, ObservedChange> > sorter; 
    for (auto& change : m_observed_changes) {
        sorter.push_back(change);
    } 
    sort(sorter.begin(), sorter.end(), cmp); 

    // redraw
    Redraw(sorter);
}

void StateMonitor::Redraw(const vector<pair<string, ObservedChange> >& changes) {
#ifdef REMC2_CODE
    SDL_RenderClear(m_renderer);

    SDL_Rect rect = {0, 0, STATE_MONITOR_WIDTH, STATE_MONITOR_HEIGHT};
    SDL_FillRect(m_surface, &rect, 0);

    rect = {0, 0, 0, 0};
    for (int i = 0; i < min(MAX_CHANGES_DISPLAYED, (int)changes.size()); i++) {
        const auto& change = changes[i];
        string text = change.first + ": " + change.second.old_value + " -> " + change.second.new_value + " TTL" + to_string(change.second.time_to_live);

        SDL_Color color = {255, 255, 255};
        if (change.second.time_to_live > 9*MAX_OBSERVED_CHANGE_TIME_TO_LIVE / 10) {
            color = {255, 0, 0};
        }
        SDL_Surface *surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
        if (!surface) { return; }

        rect.x = 0;
        rect.y = rect.y + rect.h;
        rect.w = surface->w;
        rect.h = surface->h;

        SDL_BlitSurface(surface, nullptr, m_surface, &rect);
        SDL_FreeSurface(surface);
    } 

	rect = {0, 0, STATE_MONITOR_WIDTH, STATE_MONITOR_HEIGHT};
	SDL_UpdateTexture(m_texture, nullptr, m_surface->pixels, m_surface->pitch);
    SDL_RenderCopy(m_renderer, m_texture, &rect, &rect);
    SDL_RenderPresent(m_renderer);
#endif //REMC2_CODE
}

void StateMonitor::RegisterChange(const string& key, const string& old_value, const string& new_value) {
    m_observed_changes[key] = {old_value, new_value, MAX_OBSERVED_CHANGE_TIME_TO_LIVE};
}

// macros to compare and register changes
#define MONITOR_STRUCT_VARIABLE_HEX(struct, key) \
    if (m_last_states.struct.key != struct.key) { \
        RegisterChange( \
            #struct ": " #key, \
            fmt::format(fmt::runtime("{:x}"), m_last_states.struct.key), \
            fmt::format(fmt::runtime("{:x}"), struct.key) \
        ); \
    }
#define MONITOR_STRUCT_ARRAY_HEX(struct, key) \
    if (m_last_states.struct.key != struct.key) { \
        std::string old_value; \
        std::string new_value; \
        for (auto& value : m_last_states.struct.key) { \
            old_value += fmt::format(fmt::runtime("{:x} "), value); \
        } \
        for (auto& value : struct.key) { \
            new_value += fmt::format(fmt::runtime("{:x} "), value); \
        } \
        RegisterChange(#struct ": " #key, old_value, new_value); \
    }
#define MONITOR_STRUCT_ARRAY_POINTERS(struct, key) \
    if (m_last_states.struct.key != struct.key) { \
        std::string old_value; \
        std::string new_value; \
        for (auto& value : m_last_states.struct.key) { \
            old_value += fmt::format(fmt::runtime("{:x} "), reinterpret_cast<std::uintptr_t>(value)); \
        } \
        for (auto& value : struct.key) { \
            new_value += fmt::format(fmt::runtime("{:x} "), reinterpret_cast<std::uintptr_t>(value)); \
        } \
        RegisterChange(#struct ": " #key, old_value, new_value); \
    }


void StateMonitor::MonitorD41A0() {
#ifdef REMC2_CODE
    // compare D41A0_0 with with last state if different, add to observed changes
	// uint8_t stub0[4];
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x4);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, LevelIndex_0xc);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, NumberOfPlayers_0xe);
	MONITOR_STRUCT_ARRAY_HEX(D41A0_0, array_0x10);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x2d);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x31);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x33);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x35); // entity counter(max 1000 entity changes per step)
    MONITOR_STRUCT_ARRAY_HEX(D41A0_0, array_0x39);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, maptypeMusic_0x235);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x239);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x23a);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x23e);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x242);
    //MONITOR_STRUCT_ARRAY_POINTERS(D41A0_0, pointers_0x246);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x11e6); // second entity counter
	// type_entity_0x6E8E* dword_0x11EA[1000];//??4586
	// GameSettingsStruct_t m_GameSettings;
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x219A);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x219E);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x21A2);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x21A6);
	// type_str_0x21AA str_0x21AA;
	// type_str_0x21AE str_0x21AE;
	// type_str_0x21B2 str_0x21B2;
	// type_str_0x21B6 str_0x21B6;
	// uint8_t stub3b[424];
	// axis_3d array_0x2362[8];
	// uint8_t stub3c[333];
	// uint8_t stub3d[1791];
	// type_str_0x2BDE array_0x2BDE[8];
	// Type_PlayerInput_0x6E3E playerInputs_0x6E3E[8];//28222	lenght 0xa size 0x8// game events
	// type_entity_0x6E8E struct_0x6E8E[1000];//28302 a8*3e8
	// Type_Level_2FECE terrain_2FECE;// a1 = &x_D41A0_BYTEARRAY_0[0x2FECE/*196302*/];//fix - size 0x6604u//compress level
	// type_str_0x364D2 str_0x364D2;//lenght 108
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x3653E);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36540);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36542);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36544);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36546);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36548);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x3654A);
	// type_str_3654C stages_0x3654C[8];//size 10 count 8
	// type_str_3659C struct_0x3659C[8];//size 11 count 8 C-0 D-1 E-2 F-3
	// type_str_0x3647Ac StageVars2_0x365F4[11];//8x11	//set 0x58 // end 0x3664c
	// type_str_0x3664C str_0x3664C[50];// lenght 0x79e//39x50 (end-0x36dea)
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x36DEA_fly_asistant);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x36DEB_xx);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DEC_mousex);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DEE_mousey);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DF0_mousexx);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, dword_0x36DF2);
	// type_str_160* dword_0x36DF6;//pointer
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DFA);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DFC);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, word_0x36DFE);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, countStageVars_0x36E00);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, stageIndex_0x36E01);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x36E02);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x36E03);
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_counter_current_objective_box_0x36E04);
	// uint8_t stub3k[6];
    MONITOR_STRUCT_VARIABLE_HEX(D41A0_0, byte_0x36E0B);
	// uint8_t stubend[11];

    // shallow copy of D41A0_0
    m_last_states.D41A0_0 = D41A0_0;
#endif //REMC2_CODE
}

extern type_x_DWORD_17DB70str x_DWORD_17DB70str;
void StateMonitor::Monitor17DB70str() {
#ifdef REMC2_CODE
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, time_17DB70);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB74);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB76_posx);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB78_posy);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, fill1);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, fill2);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB7E);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB80);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB82_shift_step);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB84);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB86);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB88);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB8A);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_WORD_17DB8C);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_BYTE_17DB8E);
    MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, x_BYTE_17DB8F);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.diffX_0);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.diffY_1);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.absDiffX2_2);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.absDiffY2_3);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.maxX_4);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.maxY_5);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.beginX_6);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.beginY_7);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.secondAdd_8);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.field_9);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.stateX_10);
	MONITOR_STRUCT_VARIABLE_HEX(x_DWORD_17DB70str, unk_17DB90.stateY_11);

    // shallow copy of x_DWORD_17DB70str
    m_last_states.x_DWORD_17DB70str = x_DWORD_17DB70str;
#endif //REMC2_CODE
}

extern std::array<type_D4C52ar, 17> str_D4C48ar;
void StateMonitor::Monitor_str_D4C48ar() {
    for (int i = 0; i < str_D4C48ar.size(); i++) {
        bool different = 
            m_last_states.str_D4C48ar[i].dword_0 != str_D4C48ar[i].dword_0 ||
            m_last_states.str_D4C48ar[i].dword_4 != str_D4C48ar[i].dword_4 ||
            m_last_states.str_D4C48ar[i].word_8 != str_D4C48ar[i].word_8 ||
            m_last_states.str_D4C48ar[i].dword_10 != str_D4C48ar[i].dword_10 ||
            m_last_states.str_D4C48ar[i].dword_14 != str_D4C48ar[i].dword_14;
        if (different) {
#ifdef REMC2_CODE
            RegisterChange(
                fmt::format(fmt::runtime("str_D4C48ar[{}]"), i),
                fmt::format(fmt::runtime("{:x} {:x} {:x} {:x} {:x}"), 
                    m_last_states.str_D4C48ar[i].dword_0, 
                    m_last_states.str_D4C48ar[i].dword_4, 
                    m_last_states.str_D4C48ar[i].word_8, 
                    reinterpret_cast<std::uintptr_t>(m_last_states.str_D4C48ar[i].dword_10), 
                    reinterpret_cast<std::uintptr_t>(m_last_states.str_D4C48ar[i].dword_14)),
                fmt::format(fmt::runtime("{:x} {:x} {:x} {:x} {:x}"), 
                    str_D4C48ar[i].dword_0, 
                    str_D4C48ar[i].dword_4, 
                    str_D4C48ar[i].word_8, 
                    reinterpret_cast<std::uintptr_t>(str_D4C48ar[i].dword_10), 
                    reinterpret_cast<std::uintptr_t>(str_D4C48ar[i].dword_14))
            );
#endif //REMC2_CODE
        }
    }

    // shallow copy of str_D4C48ar
    m_last_states.str_D4C48ar = str_D4C48ar;
}

// TODO: check these 
// - unk_17DBA8str
// - x_DWORD_17DE28str
// - all the type_D4C52ar2 in sub_main.cpp
