#pragma once

#include <string>
#include <vector>

class CommandLineParser {
    public:
        CommandLineParser () = default;
        CommandLineParser (int argc, char **argv);

        void Init(int argc, char **argv);

        // modes: these control behaviour and set multiple other parameters
        bool ModeReleaseGame() const {return m_mode_release_game;}; //this is standard setting
        bool ModePlayingGame() const {return m_mode_playing_game;}; //this is setting for autosavegame
        bool ModeTestRegressionsGame() const {return m_mode_test_regressions_game;}; //this is setting for regressions testing
        bool ModeDebugAfterload() const {return m_mode_debug_afterload;}; //this is setting is for compare data with dosbox afterload(can fix mouse move, and etc.)
        bool ModeDebugOnstart() const {return m_mode_debug_onstart;}; //this is setting is for compare data with dosbox(can fix mouse move, and etc.)
        bool ModeTestNetwork() const {return m_mode_test_network;};

        // parameters
        bool DoAlternativeGamespeedControl() const {return m_alternative_gamespeed_control ;};
        bool DoAnalyzeEntity() const {return m_analyze_entity ;};
        bool DoAutoChangeRes() const {return m_auto_change_res;};
        bool DoCopySkipConfig() const {return m_copy_skip_config;};
        bool DoDebugSequences() const {return m_debug_sequences;};
        bool DoDebugSequences2() const {return m_debug_sequences2;};
        bool DoDetectDwordA() const {return m_detect_dword_a;};
        bool DoDisableGraphicsEnhance() const {return m_disable_graphics_enhance;};
        bool DoFixFlyasistant() const {return m_fix_flyasistant;};
        bool DoFixMouse() const {return m_fix_mouse;};
        bool DoIntervalSave() const {return m_interval_save;};
        bool DoLoadEditedLevel() const {return m_load_edited_level;};
        bool DoMouseOff() const {return m_mouse_off;};
        bool DoMouseOff2() const {return m_mouse_off2;};
        bool DoMovePlayer() const {return m_move_player;};
        bool DoNoShowNewProcedures() const {return m_no_show_new_procedures;};
        bool DoShowNewProcedures() const {return !m_no_show_new_procedures;};
        bool DoOffPause5() const {return m_off_pause_5;};
        bool DoRightButton() const {return m_right_button;};
        bool DoTestRegression() const {return m_test_regression;};
        bool DoTestRenderers() const {return m_test_renderers;}; //only for debugging!!!!
        bool DoDebugafterload() const {return m_debugafterload;};
        bool DoGraphicsDebug() const {return m_graphics_debug;};
        bool DoHideGraphics() const {return m_hide_graphics;};
        bool DoRotatePlayer() const {return m_rotate_player;};
        bool DoSetObjective() const {return m_set_objective;};
        bool DoSetLevel() const {return m_set_level;};
        bool DoTestNetworkChng1() const {return m_test_network_chng1;};
        bool DoShowDebugMessages1() { return false; };

        // settings
        std::string GetMemimagesPath() const {return m_memimages_path;};

    private:
        void InterpretParams();

        std::vector<std::string> m_params;

        // modes: these control behaviour and set multiple other parameters
        bool m_mode_release_game;
        bool m_mode_playing_game;
        bool m_mode_test_regressions_game;
        bool m_mode_debug_afterload;
        bool m_mode_debug_onstart;
        bool m_mode_test_network;

        // parameters
        bool m_alternative_gamespeed_control;
        bool m_analyze_entity;
        bool m_auto_change_res;
        bool m_copy_skip_config;
        bool m_debug_sequences;
        bool m_debug_sequences2;
        bool m_detect_dword_a;
        bool m_disable_graphics_enhance;
        bool m_fix_flyasistant;
        bool m_fix_mouse;
        bool m_interval_save;
        bool m_load_edited_level;
        bool m_mouse_off;
        bool m_mouse_off2;
        bool m_move_player;
        bool m_no_show_new_procedures;
        bool m_off_pause_5;
        bool m_right_button;
        bool m_test_regression;
        bool m_test_renderers;
        bool m_debugafterload;
        bool m_graphics_debug;
        bool m_hide_graphics;
        bool m_rotate_player;
        bool m_set_objective;
        bool m_set_level;
        bool m_test_network_chng1;
        std::string m_memimages_path;
};

extern CommandLineParser CommandLineParams;
