#pragma once

#include <string>
#include <vector>
#include <cstdint>

class CommandLineParser {
    public:
        CommandLineParser () = default;
        CommandLineParser (int argc, char **argv);

        void Init(int argc, char **argv);

        // modes: these control behaviour and set multiple other parameters
        bool ModeReleaseGame() const {return m_mode_release_game;}; //this is standard setting
        bool ModePlayingGame() const {return m_mode_playing_game;}; //this is setting for autosavegame
        int ModeRegressionsSaveIndex() const { return m_test_save_index; }; //this is setting for regressions testing
		int ModeRegressionsTestType() const { return m_mode_regression_type; }; //this is setting for regressions testing
		bool ModeDebugOnstart() const {return m_mode_debug_onstart;}; //this is setting is for compare data with dosbox(can fix mouse move, and etc.)
        bool ModeNetwork() const {return m_mode_network;};

        // parameters
		bool DoNetworkDebug() const { return m_network_debug; };

		// Automated network testing.  AutoTest() drives the menus (skip intros, enter the
		// network game, host starts the level) so a test can run unattended.
		//
		// The disturbances below only ever delay game data, never discard or reorder it:
		// the link is TCP, so a message that was sent always arrives, and always in order.
		// What a lossy line really costs is time - the segment is retransmitted and
		// everything behind it waits - which is what NetStallMs models.
		bool AutoTest() const { return m_auto_test; };
		int  NetDelayMs() const { return m_net_delay_ms; };         // constant extra latency
		int  NetJitterMs() const { return m_net_jitter_ms; };       // 0..n random extra latency
		int  NetStallMs() const { return m_net_stall_ms; };         // occasional long pause
		int  NetStallEvery() const { return m_net_stall_every; };   // ...once per n messages
		int  NetKillAfterS() const { return m_net_kill_after_s; };  // drop the link
		int  QuitAfterS() const { return m_quit_after_s; };         // leave the game
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
        bool DoTestNetworkChng1() const {return m_test_network_chng1;};
        bool DoShowDebugMessages1() const { return m_show_debug_messages1;};
        bool DoShowDebugPerifery() const { return m_show_debug_perifery; };
        bool DoTextOutputToConsole() const { return m_text_output_to_console;};
        bool DoStateMonitor() const { return m_state_monitor;};
        bool DoShowInGameDebug() const { return m_enable_in_game_debug; };
        bool DoKillMoveAndRotation() const { return m_kill_move_and_rotation; };

        // settings
        std::string GetMemimagesPath() const {return m_memimages_path;};
        std::string GetConfigFilePath() const { return m_config_file_path; };
		int16_t GetSetLevel() const { return m_set_level; };
		int16_t GetMaxRegressionsSteps() const { return m_max_regressions_steps;};
		std::string GetCustomLevelPath() const { return m_custom_level_path; };
		std::string GetLogLevelStr() const { return m_log_level_str; };
		std::string GetRecordingPath() const { return m_record_file; };
		std::string GetPlaybackPath() const { return m_play_file; };

    private:
        void InterpretParams();

        std::vector<std::string> m_params;

        // modes: these control behaviour and set multiple other parameters
        bool m_mode_release_game;
        bool m_mode_playing_game;
        bool m_mode_debug_onstart;
        bool m_mode_network;
		int m_mode_regression_type;

        // parameters
		bool m_network_debug;
		bool m_auto_test;
		int  m_net_delay_ms;
		int  m_net_jitter_ms;
		int  m_net_stall_ms;
		int  m_net_stall_every;
		int  m_net_kill_after_s;
		int  m_quit_after_s;
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
        bool m_test_network_chng1;
        bool m_show_debug_messages1;
        bool m_show_debug_perifery;
        bool m_text_output_to_console;
        bool m_state_monitor;
        bool m_enable_in_game_debug;
        bool m_kill_move_and_rotation;
		int m_test_save_index;

        std::string m_memimages_path;
        std::string m_config_file_path;
        uint16_t m_set_level;
		uint32_t m_max_regressions_steps;
        std::string m_custom_level_path;
		std::string m_log_level_str;
		std::string m_record_file;
		std::string m_play_file;
};

extern CommandLineParser CommandLineParams;
