set(REGRESSION_TEST_TARGET remc2-regression-test)

################################################################################
# Source groups
################################################################################
set(SOURCE_FILES_REGRESSIONTESTS
    "../remc2/engine/Animation.cpp"
    "../remc2/engine/Animation.h"
    "../remc2/engine/Basic.cpp"
    "../remc2/engine/Basic.h"
    "../remc2/engine/CommandLineParser.cpp"
    "../remc2/engine/CommandLineParser.h"
    "../remc2/engine/Events.cpp"
    "../remc2/engine/Events.h"
    "../remc2/engine/Event.h"
    "../remc2/engine/EventDispatcher.cpp"
    "../remc2/engine/EventDispatcher.h"
    "../remc2/engine/GameBitmap.cpp"
    "../remc2/engine/GameBitmap.h"
    "../remc2/engine/GameBitmapIndexes.h"
    "../remc2/engine/GameRenderHD.cpp"
    "../remc2/engine/GameRenderHD.h"
    "../remc2/engine/GameRenderInterface.h"
    "../remc2/engine/GameRenderNG.cpp"
    "../remc2/engine/GameRenderNG.h"
    "../remc2/engine/GameRenderOriginal.cpp"
    "../remc2/engine/GameRenderOriginal.h"
    "../remc2/engine/GameUI.cpp"
    "../remc2/engine/GameUI.h"
    "../remc2/engine/GameUiConstants.h"
    "../remc2/engine/Graphics.cpp"
    "../remc2/engine/Graphics.h"
    "../remc2/engine/Level.cpp"
    "../remc2/engine/Level.h"
    "../remc2/engine/LevelStructs.cpp"
    "../remc2/engine/LevelStructs.h"
    "../remc2/engine/Map.h"
    "../remc2/engine/MenusAndIntros.cpp"
    "../remc2/engine/MenusAndIntros.h"
    "../remc2/engine/Network.cpp"
    "../remc2/engine/Network.h"	
    "../remc2/engine/Player.cpp"
    "../remc2/engine/Player.h"
    "../remc2/engine/PlayerInput.cpp"
    "../remc2/engine/PlayerInput.h"
    "../remc2/engine/ProjectionVertex.cpp"
    "../remc2/engine/ProjectionVertex.h"
    "../remc2/engine/RenderThread.cpp"
    "../remc2/engine/RenderThread.h"
    "../remc2/engine/Sound.cpp"
    "../remc2/engine/Sound.h"
    "../remc2/engine/XmiInfo.cpp"
    "../remc2/engine/XmiInfo.h"
    "../remc2/engine/Terrain.cpp"
    "../remc2/engine/Terrain.h"
    "../remc2/engine/TextureMaps.cpp"
    "../remc2/engine/TextureMaps.h"
    "../remc2/engine/Type_D404C.cpp"
    "../remc2/engine/Type_D404C.h"
    "../remc2/engine/Type_D93C0_Bldgprmbuffer.cpp"
    "../remc2/engine/Type_D93C0_Bldgprmbuffer.h"
    "../remc2/engine/Type_D94F0_Bldgprmbuffer.cpp"
    "../remc2/engine/Type_D94F0_Bldgprmbuffer.h"
    "../remc2/engine/Type_MapScreenPortals_E17CC.cpp"
    "../remc2/engine/Type_MapScreenPortals_E17CC.h"
    "../remc2/engine/Type_SecretMapScreenPortals_E2970.cpp"
    "../remc2/engine/Type_SecretMapScreenPortals_E2970.h"	
    "../remc2/engine/Type_WORD_D951C.cpp"
    "../remc2/engine/Type_WORD_D951C.h"
    "../remc2/engine/Unk_D47E0.cpp"
    "../remc2/engine/Unk_D47E0.h"
    "../remc2/engine/Unk_D4A30.cpp"
    "../remc2/engine/Unk_D4A30.h"
    "../remc2/engine/ViewPort.cpp"
    "../remc2/engine/engine_support.cpp"
    "../remc2/engine/engine_support.h"
    "../remc2/engine/read_config.cpp"
    "../remc2/engine/read_config.h"
    "../remc2/portability/Config.cpp"
    "../remc2/portability/Config.h"
    "../remc2/portability/fcaseopen.cpp"
    "../remc2/portability/fcaseopen.h"
    "../remc2/portability/port_filesystem.cpp"
    "../remc2/portability/port_filesystem.h"
    "../remc2/portability/InputRecorder.cpp"
    "../remc2/portability/InputRecorder.h"
    "../remc2/portability/port_net.cpp"
    "../remc2/portability/port_net.h"
    "../remc2/portability/port_outputs.cpp"
    "../remc2/portability/port_outputs.h"	
    "../remc2/portability/port_sdl_joystick.cpp"
    "../remc2/portability/port_sdl_joystick.h"
    "../remc2/portability/port_sdl_sound.cpp"
    "../remc2/portability/port_sdl_sound.h"
    "../remc2/portability/port_sdl_vga_mouse.cpp"
    "../remc2/portability/port_sdl_vga_mouse.h"
    "../remc2/portability/port_show_perifery.cpp"
    "../remc2/portability/port_show_perifery.h"
    "../remc2/portability/port_time.cpp"
    "../remc2/portability/port_time.h"
    "../remc2/portability/xmi2mid.cpp"
    "../remc2/portability/xmi2mid.h"
    "../remc2/sub_main.cpp"
    "../remc2/sub_main.h"
    "../remc2/utilities/BitmapIO.cpp"
    "../remc2/utilities/BitmapIO.h"
    "../remc2/utilities/DataFileIO.cpp"
    "../remc2/utilities/DataFileIO.h"
    "../remc2/utilities/Maths.cpp"
    "../remc2/utilities/Maths.h"
    "../remc2/utilities/RendererTests.cpp"
    "../remc2/utilities/RendererTests.h"
    "../remc2/utilities/StateMonitor.cpp"
    "../remc2/utilities/StateMonitor.h"
    "regression-tests.cpp"
    "regression-tests.h"
    "tests.cpp"
    "../remc2/build_config.h"
    "../remc2/engine/BasicMini.cpp"
    "../remc2/engine/BasicMini.h"
    "../remc2/engine/BasicTerrain.cpp"
    "../remc2/engine/BasicTerrain.h"
    "../remc2/engine/CompareMaps.cpp"
    "../remc2/engine/CompareMaps.h"
    "../remc2/engine/ConvertMapInfo.cpp"
    "../remc2/engine/ConvertMapInfo.h"
    "../remc2/engine/DatTabIndexes.cpp"
    "../remc2/engine/DatTabIndexes.h"
    "../remc2/engine/EventsFunctions.cpp"
    "../remc2/engine/EventsFunctions.h"
    "../remc2/engine/LevelInit.cpp"
    "../remc2/engine/LevelInit.h"
    "../remc2/engine/MouseOnOff.cpp"
    "../remc2/engine/MouseOnOff.h"
    "../remc2/engine/PrintMessages.cpp"
    "../remc2/engine/PrintMessages.h"
    "../remc2/engine/ReadAndDecompress.cpp"
    "../remc2/engine/ReadAndDecompress.h"
    "../remc2/engine/ScreenBuff.cpp"
    "../remc2/engine/ScreenBuff.h"
    "../remc2/engine/Spells.cpp"
    "../remc2/engine/Spells.h"
    "../remc2/engine/UVTable_D4350.cpp"
    "../remc2/engine/UVTable_D4350.h"
    "../remc2/utilities/DataFileRNC.cpp"
    "../remc2/utilities/DataFileRNC.h"
    "../remc2/lib/stb_impl.cpp"
    "../remc2/lib/stb_image.h"
    "../remc2/lib/stb_image_write.h"
)
source_group("Source Files\\regressiontests" FILES ${SOURCE_FILES_REGRESSIONTESTS})

set(ROOT_NAMESPACE remc2-regression-test)


################################################################################
# Regression tests
################################################################################
add_executable(${REGRESSION_TEST_TARGET} ${SOURCE_FILES_REGRESSIONTESTS})
target_include_directories(${REGRESSION_TEST_TARGET} PUBLIC
    "../remc2/engine"
    "../remc2/portability"
    "../findfirst"
    "../spdlog/include"
    "../nlohmann-json"
)
target_link_libraries(${REGRESSION_TEST_TARGET} PUBLIC 
    gtest
    pthread
	SDL2::SDL2
	SDL2_mixer::SDL2_mixer
	SDL2_ttf::SDL2_ttf
	${PNG_LIBRARY}
	findfirst
	ADLMIDI_static
	fmt::fmt
)
add_compile_definitions(COMPILE_FOR_64BIT)
install(
    TARGETS ${REGRESSION_TEST_TARGET}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(
    DIRECTORY
    ${CMAKE_SOURCE_DIR}/remc2-regression-test/memimages
    DESTINATION bin
)
install(
    FILES
    ${CMAKE_SOURCE_DIR}/remc2-regression-test/regression-config.json
    ${CMAKE_SOURCE_DIR}/remc2-regression-test/renderer-config.json
    ${CMAKE_SOURCE_DIR}/remc2-regression-test/test_renderer.sh
    DESTINATION bin
)


if (USE_SANITIZERS)
    target_link_options(${REGRESSION_TEST_TARGET} PRIVATE ${SANITIZER_OPTIONS})
endif ()
