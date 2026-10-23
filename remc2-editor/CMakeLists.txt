set(EDITOR_TARGET remc2-editor)

if (WIN32)
	set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/icons/editor.rc)
endif()

set(SOURCE_FILES_EDITOR
    "editor.cpp"
    "editor_main.cpp"
    "editor.h"
    "../remc2/engine/Animation.cpp"
    "../remc2/engine/Animation.h"
    "../remc2/engine/Basic.cpp"
    "../remc2/engine/Basic.h"
    "../remc2/engine/CommandLineParser.cpp"
    "../remc2/engine/CommandLineParser.h"
    "../remc2/engine/GameBitmap.cpp"
    "../remc2/engine/GameBitmap.h"
    "../remc2/engine/GameBitmapIndexes.h"
    "../remc2/engine/GameRenderHD.cpp"
    "../remc2/engine/GameRenderHD.h"
    "../remc2/engine/GameRenderNG.cpp"
    "../remc2/engine/GameRenderNG.h"
    "../remc2/engine/GameRenderOriginal.cpp"
    "../remc2/engine/GameRenderOriginal.h"
    "../remc2/engine/Graphics.cpp"
    "../remc2/engine/Graphics.h"
    "../remc2/engine/GameRenderInterface.h"
    "../remc2/engine/GameUI.cpp"
    "../remc2/engine/GameUI.h"
    "../remc2/engine/Events.cpp"
    "../remc2/engine/Events.h"
    "../remc2/engine/Event.h"
    "../remc2/engine/EventDispatcher.cpp"
    "../remc2/engine/EventDispatcher.h"
    "../remc2/engine/Level.cpp"
    "../remc2/engine/Level.h"
    "../remc2/engine/LevelStructs.cpp"
    "../remc2/engine/LevelStructs.h"
    "../remc2/engine/GameUiConstants.h"
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
    "../remc2/engine/Type_SpriteDef_D951C.cpp"
    "../remc2/engine/Type_SpriteDef_D951C.h"
    "../remc2/engine/Unk_D47E0.cpp"
    "../remc2/engine/Unk_D47E0.h"
    "../remc2/engine/Unk_D4A30.cpp"
    "../remc2/engine/Unk_D4A30.h"
    "../remc2/engine/ViewPort.cpp"
    "../remc2/engine/engine_support.cpp"
    "../remc2/engine/engine_support.h"
    "../remc2/engine/read_config.cpp"
    "../remc2/engine/read_config.h"
    "../remc2/engine/RecordedEvent.h"
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
    "../remc2/portability/awe32/Awe32Mdi.cpp"
    "../remc2/portability/awe32/Awe32Rom.cpp"
    "../remc2/portability/awe32/Emu8000.cpp"
    "../remc2/portability/awe32/Emu8000Box.cpp"
    "../remc2/portability/awe32/I18n.cpp"
    "../remc2/portability/awe32/SoundFont.cpp"
    "../remc2/portability/awe32/Synth.cpp"
    "../remc2/portability/awe32/86box/snd_emu8k.c"
    "../remc2/portability/awe32/Awe32Curves.h"
    "../remc2/portability/awe32/Awe32Driver.h"
    "../remc2/portability/awe32/Awe32InitArrays.h"
    "../remc2/portability/awe32/Awe32Mdi.h"
    "../remc2/portability/awe32/Awe32Rom.h"
    "../remc2/portability/awe32/Emu8000.h"
    "../remc2/portability/awe32/Emu8000Box.h"
    "../remc2/portability/awe32/Emu8000Effects.h"
    "../remc2/portability/awe32/Emu8000Regs.h"
    "../remc2/portability/awe32/I18n.h"
    "../remc2/portability/awe32/MidiTypes.h"
    "../remc2/portability/awe32/SoundFont.h"
    "../remc2/portability/awe32/Synth.h"
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
    "kiss_widgets.cpp"
    "kiss_general.cpp"
    "kiss_draw.cpp"
)
#foreach(FILE_ENTRY ${SOURCE_FILES_ENGINE} ${SOURCE_FILES__portability})
#  list(APPEND SOURCE_FILES_EDITOR "../remc2/${FILE_ENTRY}")
#endforeach()
source_group("Source Files Editor" FILES ${SOURCE_FILES_EDITOR} )

set(ROOT_NAMESPACE ${EDITOR_TARGET})

add_executable(${EDITOR_TARGET} ${SOURCE_FILES_EDITOR})
target_include_directories(${EDITOR_TARGET} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../remc2/portability/awe32/86box/include")
target_compile_definitions(${EDITOR_TARGET} PRIVATE AWE32EMU_WITH_86BOX)

target_link_libraries(${EDITOR_TARGET} PUBLIC 
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
    ${PNG_LIBRARY}
    findfirst
    ADLMIDI_static
    fmt::fmt
)
target_include_directories(${EDITOR_TARGET} PUBLIC
    "../kiss_sdl-master"
    "../findfirst"
    "../spdlog/include"
)
if (APPLE)
    execute_process(COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    target_include_directories(${EDITOR_TARGET} PUBLIC
        "${HOMEBREW_PREFIX}/include"
    )
    target_include_directories(${EDITOR_TARGET} PUBLIC ${PNG_INCLUDE_DIRS})
    # See remc2/CMakeLists.txt — pack(1) structs require classic relocations.
    target_link_options(${EDITOR_TARGET} PRIVATE "-Wl,-no_fixup_chains")
endif ()
add_compile_definitions(COMPILE_FOR_64BIT)

install(
    TARGETS ${EDITOR_TARGET}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
install(
    FILES 
    ${CMAKE_SOURCE_DIR}/remc2-editor/editor-config.json
    DESTINATION bin
)
install(
    DIRECTORY
    ${CMAKE_SOURCE_DIR}/remc2-editor/kiss
    DESTINATION bin
)

if (USE_SANITIZERS)
    target_link_options(${EDITOR_TARGET} PRIVATE ${SANITIZER_OPTIONS})
endif ()
