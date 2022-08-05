#include "CommandLineParser.h"

#include <algorithm>

CommandLineParser CommandLineParams;

CommandLineParser::CommandLineParser (int &argc, char **argv) {
    this->Init(argc, argv);
}

void CommandLineParser::Init(int &argc, char **argv) {

    m_auto_change_res = false;
    m_copy_skip_config = false;
    m_debug_afterload = false;
    m_debug_onstart = false;
    m_debug_sequences = false;
    m_debug_sequences2 = false;
    m_detect_dword_a = false;
    m_disable_graphics_enhance = false;
    m_fix_flyasistant = false;
    m_fix_mouse = false;
    m_interval_save = false;
    m_load_edited_level = false;
    m_mouse_off2 = false;
    m_move_player = false;
    m_no_show_new_procedures = false;
    m_off_pause_5 = false;
    m_playing_game = false;
    m_release_game = false;
    m_right_button = false;
    m_test_network = false;
    m_test_regression = false;
    m_test_regressions_game = false;
    m_debugafterload = false;
    m_first_enter = false;
    m_graphics_debug = false;
    m_hide_graphics = false;

    this->m_params.clear();
    for (int i=1; i < argc; ++i) {
        this->m_params.push_back(argv[i]);
    }
    this->InterpretParams();
}

void CommandLineParser::InterpretParams() {
    const auto &p = this->m_params;

    for (auto param: p) {
        if (param == "--release_game") {
            m_release_game = true;
            m_no_show_new_procedures = true;
            m_auto_change_res = true;
            m_fix_flyasistant = true;
            m_load_edited_level = true;
            m_debugafterload = false;
            //m_disable_graphics_enhance = true;
            m_hide_graphics = false;
        }
        else if (param == "--playing_game") {
            m_playing_game = true;
            m_detect_dword_a = true;
            m_auto_change_res = true;
            m_interval_save = true;
            m_fix_flyasistant = true;
            m_load_edited_level = true;
            m_debugafterload = true;
            m_hide_graphics = false;
        }
        else if (param == "--test_regressions_game") {
            m_test_regressions_game = true;
            m_detect_dword_a = true;
            m_copy_skip_config = true;
            m_fix_mouse = true;
            m_mouse_off2 = true;
            m_off_pause_5 = true;
            m_test_regression = true;
            m_hide_graphics = true;
            //m_debug_sequences = true;
            //m_debug_sequences2 = true;
            m_debugafterload = true;
            //m_disable_graphics_enhance = true;
        }
        else if (param == "--debug_afterload") {
            m_debug_afterload = true;
            m_detect_dword_a = true;
            m_copy_skip_config = true;
            m_debug_sequences2 = true;
            m_fix_mouse = true;
            m_mouse_off2 = true;
            m_off_pause_5 = true;
            m_debugafterload =true;
            m_disable_graphics_enhance = true;
            m_hide_graphics = false;
        }
        else if (param == "--debug_onstart") {
            m_debug_onstart = true;
            m_detect_dword_a = true;
            m_copy_skip_config = true;
            //m_debug_sequences = true;
            m_test_regression = true;
            m_fix_mouse = true;
            m_mouse_off2 = true;
            m_off_pause_5 = true;
            //m_load_edited_level = true;
            //m_right_button = true;
            m_debugafterload = true;
            //m_disable_graphics_enhance = true;
            //m_move_player = true;
            m_hide_graphics = false;
        }
        else if (param == "--test_network") {
            m_test_network = true;
            m_copy_skip_config = true;
            m_hide_graphics = false;
            m_debugafterload = true;
            m_first_enter = true;
        }
        else if (param == "--auto_change_res")           m_auto_change_res = true;
        else if (param == "--copy_skip_config")          m_copy_skip_config = true;
        else if (param == "--debug_sequences")           m_debug_sequences = true;
        else if (param == "--debug_sequences2")          m_debug_sequences2 = true;
        else if (param == "--detect_dword_a")            m_detect_dword_a = true;
        else if (param == "--disable_graphics_enhance")  m_disable_graphics_enhance = true;
        else if (param == "--fix_flyasistant")           m_fix_flyasistant = true;
        else if (param == "--fix_mouse")                 m_fix_mouse = true;
        else if (param == "--interval_save")             m_interval_save = true;
        else if (param == "--load_edited_level")         m_load_edited_level = true;
        else if (param == "--mouse_off2")                m_mouse_off2 = true;
        else if (param == "--move_player")               m_move_player = true;
        else if (param == "--no_show_new_procedures")    m_no_show_new_procedures = true;
        else if (param == "--off_pause_5")               m_off_pause_5 = true;
        else if (param == "--right_button")              m_right_button = true;
        else if (param == "--test_regression")           m_test_regression = true;
        else if (param == "--test_regressions_game")     m_test_regressions_game = true;
        else if (param == "--debugafterload")            m_debugafterload = true;
        else if (param == "--first_enter")               m_first_enter = true;
        else if (param == "--graphics_debug")            m_graphics_debug = true;
        else if (param == "--hide_graphics")             m_hide_graphics = true;
    }
}
