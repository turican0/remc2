#pragma once

#include <string>
#include <vector>

class CommandLineParser {
    public:
        CommandLineParser () {};
        CommandLineParser (int &argc, char **argv);

        void Init(int &argc, char **argv);


        bool DoAutoChangeRes() const {return m_auto_change_res;};
        bool DoCopySkipConfig() const {return m_copy_skip_config;};
        bool DoDebugAfterload() const {return m_debug_afterload;};
        bool DoDebugOnstart() const {return m_debug_onstart;};
        bool DoDebugSequences() const {return m_debug_sequences;};
        bool DoDebugSequences2() const {return m_debug_sequences2;};
        bool DoDetectDwordA() const {return m_detect_dword_a;};
        bool DoDisableGraphicsEnhance() const {return m_disable_graphics_enhance;};
        bool DoFixFlyasistant() const {return m_fix_flyasistant;};
        bool DoFixMouse() const {return m_fix_mouse;};
        bool DoIntervalSave() const {return m_interval_save;};
        bool DoLoadEditedLevel() const {return m_load_edited_level;};
        bool DoMouseOff2() const {return m_mouse_off2;};
        bool DoMovePlayer() const {return m_move_player;};
        bool DoNoShowNewProcedures() const {return m_no_show_new_procedures;};
        bool DoOffPause5() const {return m_off_pause_5;};
        bool DoPlayingGame() const {return m_playing_game;};
        bool DoReleaseGame() const {return m_release_game;};
        bool DoRightButton() const {return m_right_button;};
        bool DoTestNetwork() const {return m_test_network;};
        bool DoTestRegression() const {return m_test_regression;};
        bool DoTestRegressionsGame() const {return m_test_regressions_game;};
        bool DoDebugafterload() const {return m_debugafterload;};
        bool DoFirstEnter() const {return m_first_enter;};
        bool DoGraphicsDebug() const {return m_graphics_debug;};
        bool DoHideGraphics() const {return m_hide_graphics;};

    private:
        void InterpretParams();

        std::vector<std::string> m_params;

        bool m_auto_change_res {false};
        bool m_copy_skip_config {false};
        bool m_debug_afterload {false};
        bool m_debug_onstart {false};
        bool m_debug_sequences {false};
        bool m_debug_sequences2 {false};
        bool m_detect_dword_a {false};
        bool m_disable_graphics_enhance {false};
        bool m_fix_flyasistant {false};
        bool m_fix_mouse {false};
        bool m_interval_save {false};
        bool m_load_edited_level {false};
        bool m_mouse_off2 {false};
        bool m_move_player {false};
        bool m_no_show_new_procedures {false};
        bool m_off_pause_5 {false};
        bool m_playing_game {false};
        bool m_release_game {false};
        bool m_right_button {false};
        bool m_test_network {false};
        bool m_test_regression {false};
        bool m_test_regressions_game {false};
        bool m_debugafterload {false};
        bool m_first_enter {false};
        bool m_graphics_debug {false};
        bool m_hide_graphics {false};
};

extern CommandLineParser CommandLineParams;
