cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(remc2
	VERSION 10.0
	LANGUAGES C CXX
	HOMEPAGE_URL https://github.com/turican0/remc2
	DESCRIPTION "Magic Carpet 2: Netherworld by Bullfrog remake in C++"
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)

include_directories(
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/findfirst>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/stb>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/inih>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/inih/cpp>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/libADLMIDI/include>
)
link_directories(
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/findfirst>
)

if(NOT CMAKE_GENERATOR MATCHES "^VisualStudio")
	include(GNUInstallDirs)
endif()

################################################################################
# Dependencies
################################################################################

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(PNG REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
	"Debug"
	"Release"
	"ReleaseStrip"
	CACHE STRING "" FORCE
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

if(NOT BUILD_SETUP)
	set(BUILD_SETUP TRUE CACHE STRING "Choose wether to build the configurator." FORCE)
endif()
if(NOT BUILD_EDITOR)
	set(BUILD_EDITOR TRUE CACHE STRING "Choose wether to build the editor." FORCE)
endif()
if(NOT UNIT_TESTS)
	set(UNIT_TESTS FALSE CACHE STRING "Wether to build unit tests." FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_PROCESSOR} ${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_FRONTEND_VARIANT} => ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR} ${CMAKE_CXX_COMPILER_ID}/${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}")

################################################################################
# Global compiler options
################################################################################
if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "GNU|AppleClang")
	if(NOT CMAKE_CROSSCOMPILING)
		add_compile_options(-march=native)
	endif()
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "x64|x86_64|AMD64")
		add_compile_definitions(COMPILE_FOR_64BIT)
		add_compile_options(-mfpmath=sse)
	endif()
	# Linux/ARM
	if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
		add_compile_options(-fsigned-char)
	endif ()
	if (${SYSTEM_PROCESSOR_UPPER} MATCHES "PPC|POWERPC|PPC64|PPC64LE")
		include(CheckCSourceCompiles)

		set(CMAKE_REQUIRED_FLAGS "-maltivec")
		check_cxx_source_compiles("#include <altivec.h> int main() { vector int v = (vector int){0}; return 0; }" HAVE_ALTIVEC)

		if(HAVE_ALTIVEC)
			add_compile_options(-maltivec)
		endif()
	endif()
	add_compile_options(-ftree-vectorize)
	add_compile_options(-fopenmp)
	if(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "AppleClang")
		add_compile_options(-fopenmp-simd)
	endif()
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_compile_options(-g)
	else()
		add_compile_options(-O3)
	endif()
	add_compile_options(-fpermissive)
	add_compile_options(-Wno-narrowing)
	add_compile_options(-Wall)
	add_compile_options(-Wextra)
	add_compile_options(-pedantic)
	add_compile_options(-Wno-unknown-pragmas)
	add_compile_options(-Wno-write-strings)
	add_compile_options(-Wno-unused-result)
	add_compile_options(-Wno-unused-function)
	add_compile_options(-Wno-unused-variable)
	add_compile_options(-Wno-unused-but-set-variable)
	add_compile_options(-Wno-unused-parameter)
	add_compile_options(-Wno-missing-field-initializers)
	add_compile_options(-Wunreachable-code)		# Warn about unreachable code paths
	add_compile_options(-fno-asynchronous-unwind-tables)
	add_compile_options(-ffunction-sections)	# Put each function in its own section
	add_compile_options(-fdata-sections)		# Put each variable in its own section
	if (CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
		add_link_options(-Wl,--gc-sections)
		add_link_options(-Wl,-s)
	endif()
	link_libraries(m)
	if (WIN32 AND MINGW)
		link_libraries(mingw32)
	endif()

	if (USE_SANITIZERS)
		add_compile_options(-fsanitize=address)
		add_compile_options(-fsanitize=pointer-compare)
		add_compile_options(-fsanitize=pointer-subtract)
		add_compile_options(-fsanitize=leak)
		add_compile_options(-fsanitize=undefined)
	endif ()
elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(/openmp:experimental)
	add_compile_options(/Zc:__cplusplus)
	add_compile_options(/std:c++latest)
	add_compile_options(/Zc:preprocessor)
	add_compile_options(/Z7)
	add_compile_options(/Gy)					# Function-level linking (puts each function in its own section)
	add_compile_options(/Gw)					# Optimize global data (puts each variable in its own section)
	if (NOT CMAKE_CROSSCOMPILING)
		add_compile_definitions(_CRT_SECURE_NO_WARNINGS _WIN32)
		if (CMAKE_VS_PLATFORM_NAME MATCHES "x64|Win32")
			#add_compile_options(/Gv)
		endif()
	endif()
	if (CMAKE_BUILD_TYPE MATCHES ReleaseStrip)
		add_link_options(/DEBUG:NONE)
		add_link_options(/OPT:REF)				# Eliminate unreferenced functions and data
		add_link_options(/VERBOSE:REF)			# Print out everything the linker throws away
	endif()
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		add_compile_options(/Zi)
		add_link_options(/DEBUG:FULL)
	else()
		add_compile_options(/O2)
	endif()
	add_link_options(/INCREMENTAL:NO)
else()
	message(FATAL_ERROR "Compiler frontend variant not supported!")
endif()


cmake_policy(SET CMP0072 NEW)   #OpenGL_GL_PREFERENCE to GLVND

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

# dependencies
set(libADLMIDI_STATIC TRUE)
add_subdirectory(lib/libADLMIDI)
add_compile_definitions(COMPILE_FOR_64BIT)
#add_definitions(-w)

# preprocessor flags
if (FLATPAK)
	add_compile_definitions(FLATPAK)
endif ()

# sanitizer flags
if (USE_SANITIZERS)
	set(SANITIZER_OPTIONS
		-fsanitize=address
		-fsanitize=pointer-compare
		-fsanitize=pointer-subtract
		-fsanitize=leak
		-fsanitize=undefined
	)
endif ()



set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/assets/icons/app.rc)
source_group("Source Files" FILES ${PROJECT_NAME})
source_group("Header Files" FILES ${HEADER_FILES})

set(SOURCE_FILES_ENGINE
	"src/engine/MenuState.h"
	"src/engine/Animation.cpp"
	"src/engine/Animation.h"
	"src/engine/Basic.cpp"
	"src/engine/Basic.h"
	"src/engine/CommandLineParser.cpp"
	"src/engine/CommandLineParser.h"
	"src/engine/Compare.cpp"
	"src/engine/Compare.h"
	"src/engine/Events.cpp"
	"src/engine/Events.h"
	"src/engine/Event.h"
	"src/engine/EventDispatcher.cpp"
	"src/engine/EventDispatcher.h"
	"src/engine/GameBitmap.cpp"
	"src/engine/GameBitmap.h"
	"src/engine/GameBitmapIndexes.h"
	"src/engine/GameRenderGL.cpp"
	"src/engine/GameRenderGL.h"
	"src/engine/GameRenderHD.cpp"
	"src/engine/GameRenderHD.h"
	"src/engine/GameRenderInterface.h"
	"src/engine/GameRenderNG.cpp"
	"src/engine/GameRenderNG.h"
	"src/engine/GameRenderOriginal.cpp"
	"src/engine/GameRenderOriginal.h"
	"src/engine/GameUI.cpp"
	"src/engine/GameUI.h"
	"src/engine/GameUiConstants.h"
	"src/engine/Graphics.cpp"
	"src/engine/Graphics.h"
	"src/engine/LangTextIndexes.h"
	"src/engine/Level.cpp"
	"src/engine/Level.h"
	"src/engine/LevelStructs.cpp"
	"src/engine/LevelStructs.h"
	"src/engine/Map.h"
	"src/engine/MapColourIndexs.h"
	"src/engine/MenusAndIntros.cpp"
	"src/engine/MenusAndIntros.h"
	"src/engine/Network.cpp"
	"src/engine/Network.h"
	"src/engine/Player.cpp"
	"src/engine/Player.h"
	"src/engine/PlayerInput.cpp"
	"src/engine/PlayerInput.h"
	"src/engine/ProjectionPolygon.cpp"
	"src/engine/ProjectionPolygon.h"
	"src/engine/RenderThread.cpp"
	"src/engine/RenderThread.h"
	"src/engine/Sound.cpp"
	"src/engine/Sound.h"
	"src/engine/XmiInfo.cpp"
	"src/engine/XmiInfo.h"
	"src/engine/WavIndexes.h"
	"src/engine/Terrain.cpp"
	"src/engine/Terrain.h"
	"src/engine/TextureMaps.cpp"
	"src/engine/TextureMaps.h"
	"src/engine/Type_D404C.cpp"
	"src/engine/Type_D404C.h"
	"src/engine/Type_D93C0_Bldgprmbuffer.cpp"
	"src/engine/Type_D93C0_Bldgprmbuffer.h"
	"src/engine/Type_D94F0_Bldgprmbuffer.cpp"
	"src/engine/Type_D94F0_Bldgprmbuffer.h"
	"src/engine/Type_MapScreenPortals_E17CC.cpp"
	"src/engine/Type_MapScreenPortals_E17CC.h"
	"src/engine/Type_SecretMapScreenPortals_E2970.cpp"
	"src/engine/Type_SecretMapScreenPortals_E2970.h"
	"src/engine/Type_WORD_D951C.cpp"
	"src/engine/Type_WORD_D951C.h"
	"src/engine/Unk_D47E0.cpp"
	"src/engine/Unk_D47E0.h"
	"src/engine/Unk_D4A30.cpp"
	"src/engine/Unk_D4A30.h"
	"src/engine/ViewPort.cpp"
	"src/engine/ViewPort.h"
	"src/engine/ail_sound.h"
	"src/engine/defs.h"
	"src/engine/engine_support.cpp"
	"src/engine/engine_support.h"
	"src/engine/engine_support_converts.cpp"
	"src/engine/engine_support_converts.h"
	"src/engine/read_config.cpp"
	"src/engine/read_config.h"
	"src/engine/sub_main_mouse.h"
	"src/engine/RecordedEvent.h"
	"src/sub_main.cpp"
	"src/sub_main.h"
	"src/utilities/BitmapIO.cpp"
	"src/utilities/BitmapIO.h"
	"src/utilities/DataFileIO.cpp"
	"src/utilities/DataFileIO.h"
	"src/utilities/Maths.cpp"
	"src/utilities/Maths.h"
	"src/utilities/RendererTests.cpp"
	"src/utilities/RendererTests.h"
	"src/utilities/StateMonitor.cpp"
	"src/utilities/StateMonitor.h"
	"src/build_config.h"
	"src/engine/BasicMini.cpp"
	"src/engine/BasicMini.h"
	"src/engine/BasicTerrain.cpp"
	"src/engine/BasicTerrain.h"
	"src/engine/CompareMaps.cpp"
	"src/engine/CompareMaps.h"
	"src/engine/ConvertMapInfo.cpp"
	"src/engine/ConvertMapInfo.h"
	"src/engine/DatTabIndexes.cpp"
	"src/engine/DatTabIndexes.h"
	"src/engine/EventsFunctions.cpp"
	"src/engine/EventsFunctions.h"
	"src/engine/LevelInit.cpp"
	"src/engine/LevelInit.h"
	"src/engine/MouseOnOff.cpp"
	"src/engine/MouseOnOff.h"
	"src/engine/PrintMessages.cpp"
	"src/engine/PrintMessages.h"
	"src/engine/ReadAndDecompress.cpp"
	"src/engine/ReadAndDecompress.h"
	"src/engine/ScreenBuff.cpp"
	"src/engine/ScreenBuff.h"
	"src/engine/Spells.cpp"
	"src/engine/Spells.h"
	"src/engine/UVTable_D4350.cpp"
	"src/engine/UVTable_D4350.h"
	"src/utilities/DataFileRNC.cpp"
	"src/utilities/DataFileRNC.h"
	"src/utilities/stb_impl.cpp"
	"lib/stb/stb_image.h"
	"lib/stb/stb_image_write.h"
)
source_group("Source Files\\engine" FILES ${SOURCE_FILES_ENGINE})

set(SOURCE_FILES__libs__findfirst
	"lib/findfirst/findfirst.c"
	"lib/findfirst/findfirst.h"
	"lib/findfirst/spec.c"
	"lib/findfirst/spec.h"
)
source_group("Source Files\\libs\\findfirst" FILES ${SOURCE_FILES__libs__findfirst})
add_library(findfirst ${SOURCE_FILES__libs__findfirst})

set(SOURCE_FILES__portability
	"src/portability/bitmap_pos_struct.h"
	"src/portability/Config.cpp"
	"src/portability/Config.h"
	"src/portability/fcaseopen.cpp"
	"src/portability/fcaseopen.h"
	"src/portability/GameKey.h"
	"src/portability/KeyboardInputMapping.h"
	"src/portability/MouseInputMapping.h"
	"src/portability/port_net.cpp"
	"src/portability/port_net.h"
	"src/portability/port_filesystem.cpp"
	"src/portability/port_filesystem.h"
	"src/portability/InputRecorder.cpp"
	"src/portability/InputRecorder.h"
	"src/portability/port_outputs.cpp"
	"src/portability/port_outputs.h"
	"src/portability/port_sdl_sound.cpp"
	"src/portability/port_sdl_sound.h"
	"src/portability/port_sdl_vga_mouse.cpp"
	"src/portability/port_sdl_vga_mouse.h"
	"src/portability/port_sdl_joystick.cpp"
	"src/portability/port_sdl_joystick.h"
	"src/portability/port_show_perifery.cpp"
	"src/portability/port_show_perifery.h"
	"src/portability/port_time.cpp"
	"src/portability/port_time.h"
	"src/portability/xmi2mid.cpp"
	"src/portability/xmi2mid.h"
)
source_group("Source Files\\portability" FILES ${SOURCE_FILES__portability})
set(ALL_FILES
	${HEADER_FILES}
	${SOURCE_FILES}
	${SOURCE_FILES_ENGINE}
	${SOURCE_FILES__portability}
	${SOURCE_FILES__networklib}
)

if(BUILD_EDITOR)
	set(EDITOR_TARGET editor)

	if (WIN32)
		set(APP_ICON_RESOURCE_EDITOR ${CMAKE_SOURCE_DIR}/assets/icons/editor.rc)
	endif()

	set(SOURCE_FILES_EDITOR
		${APP_ICON_RESOURCE_EDITOR}
		"tools/editor/editor.cpp"
		"tools/editor/editor_main.cpp"
		"tools/editor/editor.h"
		"src/engine/Animation.cpp"
		"src/engine/Animation.h"
		"src/engine/Basic.cpp"
		"src/engine/Basic.h"
		"src/engine/CommandLineParser.cpp"
		"src/engine/CommandLineParser.h"
		"src/engine/GameBitmap.cpp"
		"src/engine/GameBitmap.h"
		"src/engine/GameBitmapIndexes.h"
		"src/engine/GameRenderHD.cpp"
		"src/engine/GameRenderHD.h"
		"src/engine/GameRenderNG.cpp"
		"src/engine/GameRenderNG.h"
		"src/engine/GameRenderOriginal.cpp"
		"src/engine/GameRenderOriginal.h"
		"src/engine/Graphics.cpp"
		"src/engine/Graphics.h"
		"src/engine/GameRenderInterface.h"
		"src/engine/GameUI.cpp"
		"src/engine/GameUI.h"
		"src/engine/Events.cpp"
		"src/engine/Events.h"
		"src/engine/Event.h"
		"src/engine/EventDispatcher.cpp"
		"src/engine/EventDispatcher.h"
		"src/engine/Level.cpp"
		"src/engine/Level.h"
		"src/engine/LevelStructs.cpp"
		"src/engine/LevelStructs.h"
		"src/engine/GameUiConstants.h"
		"src/engine/MenusAndIntros.cpp"
		"src/engine/MenusAndIntros.h"
		"src/engine/Network.cpp"
		"src/engine/Network.h"	
		"src/engine/Player.cpp"
		"src/engine/Player.h"
		"src/engine/PlayerInput.cpp"
		"src/engine/PlayerInput.h"
		"src/engine/ProjectionPolygon.cpp"
		"src/engine/ProjectionPolygon.h"
		"src/engine/RenderThread.cpp"
		"src/engine/RenderThread.h"
		"src/engine/Sound.cpp"
		"src/engine/Sound.h"
		"src/engine/XmiInfo.cpp"
		"src/engine/XmiInfo.h"
		"src/engine/Terrain.cpp"
		"src/engine/Terrain.h"
		"src/engine/TextureMaps.cpp"
		"src/engine/TextureMaps.h"
		"src/engine/Type_D404C.cpp"
		"src/engine/Type_D404C.h"
		"src/engine/Type_D93C0_Bldgprmbuffer.cpp"
		"src/engine/Type_D93C0_Bldgprmbuffer.h"
		"src/engine/Type_D94F0_Bldgprmbuffer.cpp"
		"src/engine/Type_D94F0_Bldgprmbuffer.h"
		"src/engine/Type_MapScreenPortals_E17CC.cpp"
		"src/engine/Type_MapScreenPortals_E17CC.h"
		"src/engine/Type_SecretMapScreenPortals_E2970.cpp"
		"src/engine/Type_SecretMapScreenPortals_E2970.h"	
		"src/engine/Type_WORD_D951C.cpp"
		"src/engine/Type_WORD_D951C.h"
		"src/engine/Unk_D47E0.cpp"
		"src/engine/Unk_D47E0.h"
		"src/engine/Unk_D4A30.cpp"
		"src/engine/Unk_D4A30.h"
		"src/engine/ViewPort.cpp"
		"src/engine/engine_support.cpp"
		"src/engine/engine_support.h"
		"src/engine/read_config.cpp"
		"src/engine/read_config.h"
		"src/engine/RecordedEvent.h"
		"src/portability/Config.cpp"
		"src/portability/Config.h"
		"src/portability/fcaseopen.cpp"
		"src/portability/fcaseopen.h"
		"src/portability/port_filesystem.cpp"
		"src/portability/port_filesystem.h"
		"src/portability/InputRecorder.cpp"
		"src/portability/InputRecorder.h"
		"src/portability/port_net.cpp"
		"src/portability/port_net.h"
		"src/portability/port_outputs.cpp"
		"src/portability/port_outputs.h"
		"src/portability/port_sdl_joystick.cpp"
		"src/portability/port_sdl_joystick.h"
		"src/portability/port_sdl_sound.cpp"
		"src/portability/port_sdl_sound.h"
		"src/portability/port_sdl_vga_mouse.cpp"
		"src/portability/port_sdl_vga_mouse.h"
		"src/portability/port_show_perifery.cpp"
		"src/portability/port_show_perifery.h"
		"src/portability/port_time.cpp"
		"src/portability/port_time.h"
		"src/portability/xmi2mid.cpp"
		"src/portability/xmi2mid.h"
		"src/sub_main.cpp"
		"src/sub_main.h"
		"src/utilities/BitmapIO.cpp"
		"src/utilities/BitmapIO.h"
		"src/utilities/DataFileIO.cpp"
		"src/utilities/DataFileIO.h"
		"src/utilities/Maths.cpp"
		"src/utilities/Maths.h"
		"src/utilities/RendererTests.cpp"
		"src/utilities/RendererTests.h"
		"src/utilities/StateMonitor.cpp"
		"src/utilities/StateMonitor.h"
		"src/build_config.h"
		"src/engine/BasicMini.cpp"
		"src/engine/BasicMini.h"
		"src/engine/BasicTerrain.cpp"
		"src/engine/BasicTerrain.h"
		"src/engine/CompareMaps.cpp"
		"src/engine/CompareMaps.h"
		"src/engine/ConvertMapInfo.cpp"
		"src/engine/ConvertMapInfo.h"
		"src/engine/DatTabIndexes.cpp"
		"src/engine/DatTabIndexes.h"
		"src/engine/EventsFunctions.cpp"
		"src/engine/EventsFunctions.h"
		"src/engine/LevelInit.cpp"
		"src/engine/LevelInit.h"
		"src/engine/MouseOnOff.cpp"
		"src/engine/MouseOnOff.h"
		"src/engine/PrintMessages.cpp"
		"src/engine/PrintMessages.h"
		"src/engine/ReadAndDecompress.cpp"
		"src/engine/ReadAndDecompress.h"
		"src/engine/ScreenBuff.cpp"
		"src/engine/ScreenBuff.h"
		"src/engine/Spells.cpp"
		"src/engine/Spells.h"
		"src/engine/UVTable_D4350.cpp"
		"src/engine/UVTable_D4350.h"
		"src/utilities/DataFileRNC.cpp"
		"src/utilities/DataFileRNC.h"
		"src/utilities/stb_impl.cpp"
		"lib/stb/stb_image.h"
		"lib/stb/stb_image_write.h"
		"tools/editor/kiss_widgets.cpp"
		"tools/editor/kiss_general.cpp"
		"tools/editor/kiss_draw.cpp"
	)
	source_group("Source Files Editor" FILES ${SOURCE_FILES_EDITOR} )

	set(ROOT_NAMESPACE ${EDITOR_TARGET})

	add_executable(${EDITOR_TARGET} ${SOURCE_FILES_EDITOR})

	install(
		TARGETS ${EDITOR_TARGET}
		CONFIGURATIONS Debug Release RelWithDebInfo ReleaseStrip
	)
endif()

if(BUILD_SETUP)
	set(SETUP_TARGET setup)

	if (WIN32)
		set(APP_ICON_RESOURCE_SETUP assets/icons/config.rc)
	endif()

	set(SOURCE_FILES_SETUP
		${APP_ICON_RESOURCE_SETUP}
		"tools/configurator/Remc2ConfiguratorApp.cpp"
		"src/portability/Config.cpp"
		"src/portability/Config.h"
		"tools/configurator/SettingsDialog.cpp"
		"tools/configurator/SettingsDialog.h"
		"tools/configurator/LaunchOptionsDialog.cpp"
		"tools/configurator/LaunchOptionsDialog.h"
		"tools/configurator/MultiplayerDialog.cpp"
		"tools/configurator/MultiplayerDialog.h"
		"tools/configurator/ControlsDialog.cpp"
		"tools/configurator/ControlsDialog.h"
		"tools/configurator/DisplayDialog.cpp"
		"tools/configurator/DisplayDialog.h"
		"tools/configurator/GameDialog.cpp"
		"tools/configurator/GameDialog.h"
		"tools/configurator/GameFilesDialog.cpp"
		"tools/configurator/GameFilesDialog.h"
		"tools/configurator/GraphicsDialog.cpp"
		"tools/configurator/GraphicsDialog.h"
		"tools/configurator/GraphicsFoldersDialog.cpp"
		"tools/configurator/GraphicsFoldersDialog.h"
		"tools/configurator/MainFrame.cpp"
		"tools/configurator/MainFrame.h"
		"tools/configurator/PathHelpers.cpp"
		"tools/configurator/PathHelpers.h"
		"tools/configurator/SoundDialog.cpp"
		"tools/configurator/SoundDialog.h"
		"tools/configurator/ThreadingDialog.cpp"
		"tools/configurator/ThreadingDialog.h"
		"tools/configurator/ZoneEditorCtrl.cpp"
		"tools/configurator/ZoneEditorCtrl.h"
		"tools/configurator/ZoneEditorDialog.cpp"
		"tools/configurator/ZoneEditorDialog.h"
	)

	source_group("Source Files Configurator" FILES ${SOURCE_FILES_SETUP})

	set(ROOT_NAMESPACE ${SETUP_TARGET})

	# Find wxWidgets
	find_package(wxWidgets REQUIRED)

	# Create executable BEFORE adding libraries
	add_executable(${SETUP_TARGET} ${SOURCE_FILES_SETUP})

	# Now add link libraries
	target_link_libraries(${SETUP_TARGET} PUBLIC
		wxWidgets::wxWidgets
	)

	if (APPLE)
		execute_process(COMMAND brew --prefix
			OUTPUT_VARIABLE HOMEBREW_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE)

		target_include_directories(${SETUP_TARGET} PUBLIC
			"${HOMEBREW_PREFIX}/include"
			${PNG_INCLUDE_DIRS}
		)

		# See remc2/CMakeLists.txt — pack(1) structs require classic relocations.
		target_link_options(${SETUP_TARGET} PRIVATE "-Wl,-no_fixup_chains")
	endif()


	install(
		TARGETS ${SETUP_TARGET}
		CONFIGURATIONS Debug Release RelWithDebInfo ReleaseStrip
	)

	if (USE_SANITIZERS)
		target_link_options(${SETUP_TARGET} PRIVATE ${SANITIZER_OPTIONS})
	endif()
endif()
################################################################################
# targets
################################################################################
add_executable(${PROJECT_NAME} src/remc2.cpp ${ALL_FILES})

install(
	TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Debug;Release;RelWithDebInfo;ReleaseStrip
)

install(
	FILES 
	${CMAKE_SOURCE_DIR}/assets/font/16x16-font.bmp
	${CMAKE_SOURCE_DIR}/assets/font/8x8-font.bmp
	${CMAKE_SOURCE_DIR}/assets/font/UbuntuMono-B.ttf
	DESTINATION font
)
install(
	FILES 
	${CMAKE_SOURCE_DIR}/config.json
	DESTINATION config.json
)
install(
	DIRECTORY
	${CMAKE_SOURCE_DIR}/assets/enhancedassets/music-ogg
	${CMAKE_SOURCE_DIR}/assets/enhancedassets/graphics
	DESTINATION enhancedassets
)

set(ROOT_NAMESPACE remc2)


################################################################################
# Linux settings
################################################################################
if (UNIX)
	if (APPLE)
		execute_process(COMMAND brew --prefix
			OUTPUT_VARIABLE HOMEBREW_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE)
		target_include_directories(${PROJECT_NAME} PUBLIC
			"${HOMEBREW_PREFIX}/include"
		)
		target_include_directories(${PROJECT_NAME} PUBLIC ${PNG_INCLUDE_DIRS})
	endif ()
	#target_compile_definitions(${PROJECT_NAME} PRIVATE
	#)
	#target_compile_options(${PROJECT_NAME} PRIVATE
	#)
	target_link_libraries(${PROJECT_NAME} PUBLIC 
		SDL2::SDL2
		SDL2::SDL2main
		SDL2_mixer::SDL2_mixer
		SDL2_ttf::SDL2_ttf
		${PNG_LIBRARY}
		findfirst
		ADLMIDI_static
		fmt::fmt
		nlohmann_json::nlohmann_json
		spdlog::spdlog
	)
	target_link_directories(${PROJECT_NAME} PUBLIC
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC
		"$<$<CONFIG:DEBUG>:_DEBUG>"
	)
	if (APPLE)
		# Reverse-engineered structs use #pragma pack(1), which places pointers
		# at non-8-aligned offsets. Apple Silicon's default fixup-chain
		# relocations reject these. Fall back to classic relocations.
		target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-no_fixup_chains")
	endif ()
endif ()

if (UNIX)
	if (USE_SANITIZERS)
		target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZER_OPTIONS})
	endif ()
endif ()

target_link_libraries(${EDITOR_TARGET} PUBLIC 
	SDL2::SDL2
	SDL2::SDL2main
	SDL2_mixer::SDL2_mixer
	SDL2_ttf::SDL2_ttf
	SDL2_image::SDL2_image
	${PNG_LIBRARY}
	findfirst
	ADLMIDI_static
	fmt::fmt
	nlohmann_json::nlohmann_json
	spdlog::spdlog
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

install(
	TARGETS ${EDITOR_TARGET}
	CONFIGURATIONS Debug;Release;RelWithDebInfo
)
install(
	FILES 
	editor-config.json
	DESTINATION bin
)
install(
	DIRECTORY
	tools/editor/kiss
	DESTINATION bin
)

if (USE_SANITIZERS)
	target_link_options(${EDITOR_TARGET} PRIVATE ${SANITIZER_OPTIONS})
endif ()

if (UNIT_TESTS)
	set(REGRESSION_TEST_TARGET remc2-regression-test)

	################################################################################
	# Source groups
	################################################################################
	set(SOURCE_FILES_REGRESSIONTESTS
		"src/engine/Animation.cpp"
		"src/engine/Animation.h"
		"src/engine/Basic.cpp"
		"src/engine/Basic.h"
		"src/engine/CommandLineParser.cpp"
		"src/engine/CommandLineParser.h"
		"src/engine/Events.cpp"
		"src/engine/Events.h"
		"src/engine/Event.h"
		"src/engine/EventDispatcher.cpp"
		"src/engine/EventDispatcher.h"
		"src/engine/GameBitmap.cpp"
		"src/engine/GameBitmap.h"
		"src/engine/GameBitmapIndexes.h"
		"src/engine/GameRenderHD.cpp"
		"src/engine/GameRenderHD.h"
		"src/engine/GameRenderInterface.h"
		"src/engine/GameRenderNG.cpp"
		"src/engine/GameRenderNG.h"
		"src/engine/GameRenderOriginal.cpp"
		"src/engine/GameRenderOriginal.h"
		"src/engine/GameUI.cpp"
		"src/engine/GameUI.h"
		"src/engine/GameUiConstants.h"
		"src/engine/Graphics.cpp"
		"src/engine/Graphics.h"
		"src/engine/Level.cpp"
		"src/engine/Level.h"
		"src/engine/LevelStructs.cpp"
		"src/engine/LevelStructs.h"
		"src/engine/Map.h"
		"src/engine/MenusAndIntros.cpp"
		"src/engine/MenusAndIntros.h"
		"src/engine/Network.cpp"
		"src/engine/Network.h"	
		"src/engine/Player.cpp"
		"src/engine/Player.h"
		"src/engine/PlayerInput.cpp"
		"src/engine/PlayerInput.h"
		"src/engine/ProjectionPolygon.cpp"
		"src/engine/ProjectionPolygon.h"
		"src/engine/RenderThread.cpp"
		"src/engine/RenderThread.h"
		"src/engine/Sound.cpp"
		"src/engine/Sound.h"
		"src/engine/XmiInfo.cpp"
		"src/engine/XmiInfo.h"
		"src/engine/Terrain.cpp"
		"src/engine/Terrain.h"
		"src/engine/TextureMaps.cpp"
		"src/engine/TextureMaps.h"
		"src/engine/Type_D404C.cpp"
		"src/engine/Type_D404C.h"
		"src/engine/Type_D93C0_Bldgprmbuffer.cpp"
		"src/engine/Type_D93C0_Bldgprmbuffer.h"
		"src/engine/Type_D94F0_Bldgprmbuffer.cpp"
		"src/engine/Type_D94F0_Bldgprmbuffer.h"
		"src/engine/Type_MapScreenPortals_E17CC.cpp"
		"src/engine/Type_MapScreenPortals_E17CC.h"
		"src/engine/Type_SecretMapScreenPortals_E2970.cpp"
		"src/engine/Type_SecretMapScreenPortals_E2970.h"	
		"src/engine/Type_WORD_D951C.cpp"
		"src/engine/Type_WORD_D951C.h"
		"src/engine/Unk_D47E0.cpp"
		"src/engine/Unk_D47E0.h"
		"src/engine/Unk_D4A30.cpp"
		"src/engine/Unk_D4A30.h"
		"src/engine/ViewPort.cpp"
		"src/engine/engine_support.cpp"
		"src/engine/engine_support.h"
		"src/engine/read_config.cpp"
		"src/engine/read_config.h"
		"src/portability/Config.cpp"
		"src/portability/Config.h"
		"src/portability/fcaseopen.cpp"
		"src/portability/fcaseopen.h"
		"src/portability/port_filesystem.cpp"
		"src/portability/port_filesystem.h"
		"src/portability/InputRecorder.cpp"
		"src/portability/InputRecorder.h"
		"src/portability/port_net.cpp"
		"src/portability/port_net.h"
		"src/portability/port_outputs.cpp"
		"src/portability/port_outputs.h"	
		"src/portability/port_sdl_joystick.cpp"
		"src/portability/port_sdl_joystick.h"
		"src/portability/port_sdl_sound.cpp"
		"src/portability/port_sdl_sound.h"
		"src/portability/port_sdl_vga_mouse.cpp"
		"src/portability/port_sdl_vga_mouse.h"
		"src/portability/port_show_perifery.cpp"
		"src/portability/port_show_perifery.h"
		"src/portability/port_time.cpp"
		"src/portability/port_time.h"
		"src/portability/xmi2mid.cpp"
		"src/portability/xmi2mid.h"
		"src/sub_main.cpp"
		"src/sub_main.h"
		"src/utilities/BitmapIO.cpp"
		"src/utilities/BitmapIO.h"
		"src/utilities/DataFileIO.cpp"
		"src/utilities/DataFileIO.h"
		"src/utilities/Maths.cpp"
		"src/utilities/Maths.h"
		"src/utilities/RendererTests.cpp"
		"src/utilities/RendererTests.h"
		"src/utilities/StateMonitor.cpp"
		"src/utilities/StateMonitor.h"
		"tests/remc2-regression-test/regression-tests.cpp"
		"tests/remc2-regression-test/regression-tests.h"
		"tests/remc2-regression-test/tests.cpp"
		"src/build_config.h"
		"src/engine/BasicMini.cpp"
		"src/engine/BasicMini.h"
		"src/engine/BasicTerrain.cpp"
		"src/engine/BasicTerrain.h"
		"src/engine/CompareMaps.cpp"
		"src/engine/CompareMaps.h"
		"src/engine/ConvertMapInfo.cpp"
		"src/engine/ConvertMapInfo.h"
		"src/engine/DatTabIndexes.cpp"
		"src/engine/DatTabIndexes.h"
		"src/engine/EventsFunctions.cpp"
		"src/engine/EventsFunctions.h"
		"src/engine/LevelInit.cpp"
		"src/engine/LevelInit.h"
		"src/engine/MouseOnOff.cpp"
		"src/engine/MouseOnOff.h"
		"src/engine/PrintMessages.cpp"
		"src/engine/PrintMessages.h"
		"src/engine/ReadAndDecompress.cpp"
		"src/engine/ReadAndDecompress.h"
		"src/engine/ScreenBuff.cpp"
		"src/engine/ScreenBuff.h"
		"src/engine/Spells.cpp"
		"src/engine/Spells.h"
		"src/engine/UVTable_D4350.cpp"
		"src/engine/UVTable_D4350.h"
		"src/utilities/DataFileRNC.cpp"
		"src/utilities/DataFileRNC.h"
		"src/utilities/stb_impl.cpp"
		"lib/stb/stb_image.h"
		"lib/stb/stb_image_write.h"
	)
	source_group("Source Files\\regressiontests" FILES ${SOURCE_FILES_REGRESSIONTESTS})

	set(ROOT_NAMESPACE remc2-regression-test)


	################################################################################
	# Regression tests
	################################################################################
	find_package(GTest REQUIRED)
	add_executable(${REGRESSION_TEST_TARGET} ${SOURCE_FILES_REGRESSIONTESTS})
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
	install(
		TARGETS ${REGRESSION_TEST_TARGET}
		CONFIGURATIONS Debug;Release;RelWithDebInfoi;ReleaseStrip
	)
	install(
		DIRECTORY
		tests/remc2-regression-test/memimages
		DESTINATION bin
	)
	install(
		FILES
		tests/remc2-regression-test/regression-config.json
		tests/remc2-regression-test/renderer-config.json
		tests/remc2-regression-test/test_renderer.sh
		DESTINATION bin
	)


	if (USE_SANITIZERS)
		target_link_options(${REGRESSION_TEST_TARGET} PRIVATE ${SANITIZER_OPTIONS})
	endif ()

	set(UNIT_TEST_TARGET remc2-unit-test)

	################################################################################
	# Source groups
	################################################################################
	set(SOURCE_FILES_UNITTESTS
		"tests/remc2-unit-test/engine/CommandLineParserUT.cpp"
		"tests/remc2-unit-test/portability/fcaseopenUT.cpp"
		"tests/remc2-unit-test/engine/TerrainUT.cpp"
		"tests/remc2-unit-test/engine/SetRenderViewPortSize_40C50UT.cpp"
		"tests/remc2-unit-test/utilities/Math.cpp"
		"tests/remc2-unit-test/utilities/Math.h"
		"tests/remc2-unit-test/comparison/engine/OriginalTerrain.cpp"
		"tests/remc2-unit-test/comparison/engine/OriginalTerrain.h"
		"tests/remc2-unit-test/UnitTests.cpp"
	)
	set(SOURCE_FILES_UNITTESTS_DEPENDENCIES
		"tests/remc2-unit-test/pch.cpp"
		"tests/remc2-unit-test/pch.h"
		"src/engine/Animation.cpp"
		"src/engine/Animation.h"
		"src/engine/Basic.cpp"
		"src/engine/Basic.h"
		"src/engine/CommandLineParser.cpp"
		"src/engine/CommandLineParser.h"
		"src/engine/Events.cpp"
		"src/engine/Events.h"
		"src/engine/Event.h"
		"src/engine/EventDispatcher.cpp"
		"src/engine/EventDispatcher.h"
		"src/engine/GameBitmap.cpp"
		"src/engine/GameBitmap.h"
		"src/engine/GameBitmapIndexes.h"
		"src/engine/GameRenderHD.cpp"
		"src/engine/GameRenderHD.h"
		"src/engine/GameRenderInterface.h"
		"src/engine/GameRenderNG.cpp"
		"src/engine/GameRenderNG.h"
		"src/engine/GameRenderOriginal.cpp"
		"src/engine/GameRenderOriginal.h"
		"src/engine/GameUI.cpp"
		"src/engine/GameUI.h"
		"src/engine/GameUiConstants.h"
		"src/engine/Graphics.cpp"
		"src/engine/Graphics.h"
		"src/engine/Level.cpp"
		"src/engine/Level.h"
		"src/engine/LevelStructs.cpp"
		"src/engine/LevelStructs.h"
		"src/engine/Map.h"
		"src/engine/MenusAndIntros.cpp"
		"src/engine/MenusAndIntros.h"
		"src/engine/Network.cpp"
		"src/engine/Network.h"	
		"src/engine/Player.cpp"
		"src/engine/Player.h"
		"src/engine/PlayerInput.cpp"
		"src/engine/PlayerInput.h"
		"src/engine/ProjectionPolygon.cpp"
		"src/engine/ProjectionPolygon.h"
		"src/engine/RenderThread.cpp"
		"src/engine/RenderThread.h"
		"src/engine/Sound.cpp"
		"src/engine/Sound.h"
		"src/engine/XmiInfo.cpp"
		"src/engine/XmiInfo.h"
		"src/engine/Terrain.cpp"
		"src/engine/Terrain.h"
		"src/engine/TextureMaps.cpp"
		"src/engine/TextureMaps.h"
		"src/engine/Type_D404C.cpp"
		"src/engine/Type_D404C.h"
		"src/engine/Type_D93C0_Bldgprmbuffer.cpp"
		"src/engine/Type_D93C0_Bldgprmbuffer.h"
		"src/engine/Type_D94F0_Bldgprmbuffer.cpp"
		"src/engine/Type_D94F0_Bldgprmbuffer.h"
		"src/engine/Type_MapScreenPortals_E17CC.cpp"
		"src/engine/Type_MapScreenPortals_E17CC.h"
		"src/engine/Type_SecretMapScreenPortals_E2970.cpp"
		"src/engine/Type_SecretMapScreenPortals_E2970.h"	
		"src/engine/Type_WORD_D951C.cpp"
		"src/engine/Type_WORD_D951C.h"
		"src/engine/Unk_D47E0.cpp"
		"src/engine/Unk_D47E0.h"
		"src/engine/Unk_D4A30.cpp"
		"src/engine/Unk_D4A30.h"
		"src/engine/ViewPort.cpp"
		"src/engine/engine_support.cpp"
		"src/engine/engine_support.h"
		"src/engine/read_config.cpp"
		"src/engine/read_config.h"
		"src/portability/Config.cpp"
		"src/portability/Config.h"
		"src/portability/fcaseopen.cpp"
		"src/portability/fcaseopen.h"
		"src/portability/port_filesystem.cpp"
		"src/portability/port_filesystem.h"
		"src/portability/InputRecorder.cpp"
		"src/portability/InputRecorder.h"
		"src/portability/port_net.cpp"
		"src/portability/port_net.h"
		"src/portability/port_outputs.cpp"
		"src/portability/port_outputs.h"	
		"src/portability/port_sdl_joystick.cpp"
		"src/portability/port_sdl_joystick.h"
		"src/portability/port_sdl_sound.cpp"
		"src/portability/port_sdl_sound.h"
		"src/portability/port_sdl_vga_mouse.cpp"
		"src/portability/port_sdl_vga_mouse.h"
		"src/portability/port_show_perifery.cpp"
		"src/portability/port_show_perifery.h"
		"src/portability/port_time.cpp"
		"src/portability/port_time.h"
		"src/portability/xmi2mid.cpp"
		"src/portability/xmi2mid.h"
		"src/sub_main.cpp"
		"src/sub_main.h"
		"src/utilities/BitmapIO.cpp"
		"src/utilities/BitmapIO.h"
		"src/utilities/DataFileIO.cpp"
		"src/utilities/DataFileIO.h"
		"src/utilities/Maths.cpp"
		"src/utilities/Maths.h"
		"src/utilities/RendererTests.cpp"
		"src/utilities/RendererTests.h"
		"src/utilities/StateMonitor.cpp"
		"src/utilities/StateMonitor.h"
		"src/build_config.h"
		"src/engine/BasicMini.cpp"
		"src/engine/BasicMini.h"
		"src/engine/BasicTerrain.cpp"
		"src/engine/BasicTerrain.h"
		"src/engine/CompareMaps.cpp"
		"src/engine/CompareMaps.h"
		"src/engine/ConvertMapInfo.cpp"
		"src/engine/ConvertMapInfo.h"
		"src/engine/DatTabIndexes.cpp"
		"src/engine/DatTabIndexes.h"
		"src/engine/EventsFunctions.cpp"
		"src/engine/EventsFunctions.h"
		"src/engine/LevelInit.cpp"
		"src/engine/LevelInit.h"
		"src/engine/MouseOnOff.cpp"
		"src/engine/MouseOnOff.h"
		"src/engine/PrintMessages.cpp"
		"src/engine/PrintMessages.h"
		"src/engine/ReadAndDecompress.cpp"
		"src/engine/ReadAndDecompress.h"
		"src/engine/ScreenBuff.cpp"
		"src/engine/ScreenBuff.h"
		"src/engine/Spells.cpp"
		"src/engine/Spells.h"
		"src/engine/UVTable_D4350.cpp"
		"src/engine/UVTable_D4350.h"
		"src/utilities/DataFileRNC.cpp"
		"src/utilities/DataFileRNC.h"
		"src/utilities/stb_impl.cpp"
		"lib/stb/stb_image.h"
		"lib/stb/stb_image_write.h"
	)
	source_group("Source Files\\unittests" FILES ${SOURCE_FILES_UNITTESTS} ${SOURCE_FILES_UNITTESTS_DEPENDENCIES})

	set(ROOT_NAMESPACE remc2-unit-test)


	################################################################################
	# Unit tests
	################################################################################
	include(GoogleTest)
	add_executable(${UNIT_TEST_TARGET} ${SOURCE_FILES_UNITTESTS} ${SOURCE_FILES_UNITTESTS_DEPENDENCIES})
	target_include_directories(${UNIT_TEST_TARGET} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tests/${UNIT_TEST_TARGET}>)
	target_link_libraries(${UNIT_TEST_TARGET} PUBLIC 
		pthread
		SDL2::SDL2
		SDL2_mixer::SDL2_mixer
		SDL2_ttf::SDL2_ttf
		${PNG_LIBRARY}
		findfirst
		ADLMIDI_static
		fmt::fmt
		nlohmann_json::nlohmann_json
		spdlog::spdlog
		${GTEST_LIBRARIES}
		${GTEST_MAIN_LIBRARIES}
	)
	gtest_add_tests(
		TARGET      ${UNIT_TEST_TARGET}
		SOURCES     ${SOURCE_FILES_UNITTESTS}
		TEST_SUFFIX .noArgs
		TEST_LIST   noArgsTests
	)
	set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
	install(
		TARGETS ${UNIT_TEST_TARGET}
		CONFIGURATIONS Debug;Release;RelWithDebInfo;ReleaseStrip
	)


	if (USE_SANITIZERS)
		target_link_options(${UNIT_TEST_TARGET} PRIVATE ${SANITIZER_OPTIONS})
	endif ()
endif ()
