#pragma once

#include <array>
#include <map>
#include <string>

// renderer regressions tests data for comparing HD and original renderer
enum class RendererTestsHitCheckpoint {
	RendTest_HD_Draw_Rasterline_SingleColor, RendTest_Orig_Draw_Rasterline_SingleColor,
	HD_BYTE_E126D_case_1_v176, Original_BYTE_E126D_case_1_v176,
	HD_BYTE_E126D_case_2_v228, Original_BYTE_E126D_case_2_v228,
	HD_BYTE_E126D_case_3_v383, Original_BYTE_E126D_case_3_v383,
	HD_BYTE_E126D_case_4_v339, Original_BYTE_E126D_case_4_v339,
	RendTest_HD_Draw_Rasterline_Standard, RendTest_Orig_Draw_Rasterline_Standard,
	HD_BYTE_E126D_case_6_v392, Original_BYTE_E126D_case_6_v392,
	RendTest_HD_Draw_Rasterline_Flatshading, RendTest_Orig_Draw_Rasterline_Flatshading,
	RendTest_HD_Draw_Rasterline_Reflections, RendTest_Orig_Draw_Rasterline_Reflections,
};

extern int renderer_tests_frame_count;
extern bool renderer_tests_success;
extern bool renderer_tests_quit;
constexpr int renderer_tests_frame_count_max = 100;
typedef std::pair<RendererTestsHitCheckpoint, bool> RendererTestsHitCheckpointPair;
typedef std::map<RendererTestsHitCheckpoint, bool> RendererTestsHitCheckpointMap;
typedef struct {
	int max_frames;
	int set_mouse_x;
	int set_mouse_y;
	bool set_flatshader;
	int differences;
	std::map<RendererTestsHitCheckpoint, bool> must_hit_checkpoints;
} RendererTestsForLevel;
extern std::array<RendererTestsForLevel,25> renderer_tests;
bool stop_renderer_tests();
void renderer_tests_eval_findings();
void renderer_tests_register_hit(const RendererTestsHitCheckpoint& checkpoint);
