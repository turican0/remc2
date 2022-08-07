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

    private:
        void InterpretParams();

        std::vector<std::string> m_params;

        // modes: these control behaviour and set multiple other parameters
        bool m_mode_release_game {false};
        bool m_mode_playing_game {false};
        bool m_mode_test_regressions_game {false};
        bool m_mode_debug_afterload {false};
        bool m_mode_debug_onstart {false};
        bool m_mode_test_network {false};

        // parameters
        bool m_alternative_gamespeed_control {true};
        bool m_analyze_entity {true};
        bool m_auto_change_res {false};
        bool m_copy_skip_config {false};
        bool m_debug_sequences {false};
        bool m_debug_sequences2 {false};
        bool m_detect_dword_a {false};
        bool m_disable_graphics_enhance {false};
        bool m_fix_flyasistant {false};
        bool m_fix_mouse {false};
        bool m_interval_save {false};
        bool m_load_edited_level {false};
        bool m_mouse_off {false};
        bool m_mouse_off2 {false};
        bool m_move_player {false};
        bool m_no_show_new_procedures {false};
        bool m_off_pause_5 {false};
        bool m_right_button {false};
        bool m_test_regression {false};
        bool m_test_renderers {false};
        bool m_debugafterload {false};
        bool m_graphics_debug {false};
        bool m_hide_graphics {false};
        bool m_rotate_player {false};
        bool m_set_objective {false};
        bool m_set_level {false};
        bool m_test_network_chng1 {false};
};

extern CommandLineParser CommandLineParams;
