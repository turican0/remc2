#include "CommandLineParser.h"

#include <algorithm>

CommandLineParser CommandLineParams;

CommandLineParser::CommandLineParser (int argc, char **argv) {
    this->Init(argc, argv);
}

void CommandLineParser::Init(int argc, char **argv) {

    m_mode_release_game = false;
    m_mode_playing_game = false;
    m_mode_test_regressions_game = false;
    m_mode_debug_afterload = false;
    m_mode_debug_onstart = false;
    m_mode_test_network = false;

    m_analyze_entity = true;
    m_alternative_gamespeed_control = true;
    m_auto_change_res = false;
    m_copy_skip_config = false;
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
    m_right_button = false;
    m_test_regression = false;
    m_test_renderers = false;
    m_debugafterload = false;
    m_graphics_debug = false;
    m_hide_graphics = false;
    m_rotate_player = false;
    m_set_objective = false;
    m_set_level = false;
    m_test_network_chng1 = false;
    m_show_debug_messages1 = false;
    m_text_output_to_console = false;

    m_memimages_path = "../remc2/memimages/";
    m_config_file_path = "";

    this->m_params.clear();
    for (int i=1; i < argc; ++i) {
        this->m_params.emplace_back(argv[i]);
    }
    this->InterpretParams();
}

void CommandLineParser::InterpretParams() {
    const auto &params = this->m_params;

    // FIXME: check if multiple modes are selected and warn/fix

    // if none of the modes is set then set --release_game as default
    std::vector<std::string> all_modes { 
        "--mode_release_game",
        "--mode_playing_game",
        "--mode_test_regressions_game",
        "--mode_debug_afterload",
        "--mode_debug_onstart",
        "--mode_test_network",
    };
    auto is_in_all_params = [&all_modes](const std::string &s) {
        return end(all_modes) != std::find(begin(all_modes), end(all_modes), s);
    };
    if (!std::any_of(begin(params), end(params), is_in_all_params)) {
        this->m_params.emplace_back("--mode_release_game");
    }

    // check for modes (that define a set of params) and single params
    for (auto p = params.cbegin(); p != params.cend(); ++p) {
        const auto param = *p;
        if (param == "--mode_release_game") { //this is standard setting
            m_mode_release_game = true;
            m_no_show_new_procedures = true;
            m_auto_change_res = true;
            m_fix_flyasistant = true;
            m_load_edited_level = true;
            m_debugafterload = false;
            //m_disable_graphics_enhance = true;
            m_hide_graphics = false;
            m_text_output_to_console = true;
        }
        else if (param == "--mode_playing_game") { //this is setting for autosavegame
            m_mode_playing_game = true;
            m_detect_dword_a = true;
            m_auto_change_res = true;
            m_interval_save = true;
            m_fix_flyasistant = true;
            m_load_edited_level = true;
            m_debugafterload = true;
            m_hide_graphics = false;
            m_text_output_to_console = true;
            m_show_debug_messages1 = true;
        }
        else if (param == "--mode_test_regressions_game") { //this is setting for regressions testing
            m_mode_test_regressions_game = true;
            m_no_show_new_procedures = true;
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
        else if (param == "--mode_debug_afterload") { //this is setting is for compare data with dosbox afterload(can fix mouse move, and etc.)
            m_mode_debug_afterload = true;
            m_detect_dword_a = true;
            m_copy_skip_config = true;
            m_debug_sequences2 = true;
            m_fix_mouse = true;
            m_mouse_off2 = true;
            m_off_pause_5 = true;
            m_debugafterload = false;
            m_disable_graphics_enhance = true;
            m_hide_graphics = false;
        }
        else if (param == "--mode_debug_onstart") { //this is setting is for compare data with dosbox(can fix mouse move, and etc.)
            m_mode_debug_onstart = true;
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
        else if (param == "--mode_test_network") {
            m_mode_test_network = true;
            m_copy_skip_config = true;
            m_hide_graphics = false;
            m_debugafterload = true;
            m_auto_change_res = true;
            m_load_edited_level = true;
            m_disable_graphics_enhance = true;
            m_text_output_to_console = true;
            m_show_debug_messages1 = true;
        }
        else if (param == "--no_alternative_gamespeed_control") m_alternative_gamespeed_control = false;
        else if (param == "--no_analyze_entity")                m_analyze_entity = false;
        else if (param == "--auto_change_res")                  m_auto_change_res = true;
        else if (param == "--copy_skip_config")                 m_copy_skip_config = true;
        else if (param == "--debug_sequences")                  m_debug_sequences = true;
        else if (param == "--debug_sequences2")                 m_debug_sequences2 = true;
        else if (param == "--detect_dword_a")                   m_detect_dword_a = true;
        else if (param == "--disable_graphics_enhance")         m_disable_graphics_enhance = true;
        else if (param == "--fix_flyasistant")                  m_fix_flyasistant = true;
        else if (param == "--fix_mouse")                        m_fix_mouse = true;
        else if (param == "--interval_save")                    m_interval_save = true;
        else if (param == "--load_edited_level")                m_load_edited_level = true;
        else if (param == "--mouse_off")                        m_mouse_off = true;
        else if (param == "--mouse_off2")                       m_mouse_off2 = true;
        else if (param == "--move_player")                      m_move_player = true;
        else if (param == "--no_show_new_procedures")           m_no_show_new_procedures = true;
        else if (param == "--off_pause_5")                      m_off_pause_5 = true;
        else if (param == "--right_button")                     m_right_button = true;
        else if (param == "--test_regression")                  m_test_regression = true;
        else if (param == "--test_renderers")                   m_test_renderers = true;
        else if (param == "--debugafterload")                   m_debugafterload = true;
        else if (param == "--graphics_debug")                   m_graphics_debug = true;
        else if (param == "--hide_graphics")                    m_hide_graphics = true;
        else if (param == "--rotate_player")                    m_rotate_player = true;
        else if (param == "--set_objective")                    m_set_objective = true;
        else if (param == "--set_level")                        m_set_level = true;
        else if (param == "--test_network_chng1")               m_test_network_chng1 = true;
        else if (param == "--show_debug_messages1")             m_show_debug_messages1 = true;
        else if (param == "--text_output_to_console")           m_text_output_to_console = true;
        else if (param == "--memimages_path") {
            m_memimages_path = *(++p);
        }
        else if (param == "--config_file_path") {
            m_config_file_path = *(++p);
        }
    }
}
