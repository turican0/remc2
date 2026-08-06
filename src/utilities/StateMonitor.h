#pragma once
#include "build_config.h"
#ifdef REMC2_CODE
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif //REMC2_CODE
#include <string>
#include <map>

#include "engine/engine_support.h"
#include "engine/MenusAndIntros.h"

using namespace std;

constexpr int MAX_OBSERVED_CHANGE_TIME_TO_LIVE = 300;
constexpr int MAX_CHANGES_DISPLAYED = 40;
constexpr int STATE_MONITOR_WIDTH = 1600;
constexpr int STATE_MONITOR_HEIGHT = 800;

typedef struct {
    string old_value;
    string new_value;
    int time_to_live;
} ObservedChange;

typedef struct {
    type_D41A0_BYTESTR_0 D41A0_0;
    type_x_DWORD_17DB70str x_DWORD_17DB70str;
    std::array<type_D4C52ar, 17> str_D4C48ar;
} LastStates;

class StateMonitor {
    public:
        StateMonitor();
        ~StateMonitor();

        void Init();
        void Update();

    private:
        void Redraw(const vector<pair<string, ObservedChange> >& changes);

        void RegisterChange(const string& key, const string& old_value, const string& new_value);

        void MonitorD41A0();
        void Monitor17DB70str();
        void Monitor_str_D4C48ar();

        bool m_initialized = false;
#ifdef REMC2_CODE
        TTF_Font *m_font;
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Surface *m_surface;
        SDL_Texture *m_texture;
#endif //REMC2_CODE
        map<string, ObservedChange> m_observed_changes;

        LastStates m_last_states;
};

extern StateMonitor g_state_monitor;
