project(remc2-test C CXX)

if (USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters")
endif ()

################################################################################
# Source groups
################################################################################
set(SOURCE_FILES_UNITTESTS
    "pch.cpp"
    "pch.h"
    "../remc2/engine/CommandLineParser.cpp"
    "../remc2/engine/CommandLineParser.h"
    "engine/CommandLineParserUT.cpp"
    "../remc2/portability/fcaseopen.cpp"
    "../remc2/portability/fcaseopen.h"
    "portability/fcaseopenUT.cpp"
    "../remc2/engine/ViewPort.cpp"
    "engine/SetRenderViewPortSize_40C50UT.cpp"
	"../remc2/sub_main.cpp"
    "../remc2/sub_main.h"
	"../remc2/utilities/Maths.cpp"
    "../remc2/utilities/Maths.h"
	"../remc2/utilities/DataFileIO.cpp"
    "../remc2/utilities/DataFileIO.h"
	"../remc2/utilities/BitmapIO.cpp"
    "../remc2/utilities/BitmapIO.h"
	"../remc2/portability/port_time.cpp"
    "../remc2/portability/port_time.h"
	"../remc2/portability/port_filesystem.cpp"
    "../remc2/portability/port_filesystem.h"
	"../remc2/portability/port_sdl_vga_mouse.cpp"
    "../remc2/portability/port_sdl_vga_mouse.h"
	"../remc2/portability/port_net.cpp"
    "../remc2/portability/port_net.h"
	"../remc2/portability/port_outputs.cpp"
    "../remc2/portability/port_outputs.h"	
	"../remc2/portability/port_sdl_sound.cpp"
    "../remc2/portability/port_sdl_sound.h"
	"../remc2/engine/Type_WORD_D951C.cpp"
    "../remc2/engine/Type_WORD_D951C.h"
	"../remc2/engine/Type_D404C.cpp"
    "../remc2/engine/Type_D404C.h"
	"../remc2/engine/Type_D94F0_Bldgprmbuffer.cpp"
    "../remc2/engine/Type_D94F0_Bldgprmbuffer.h"
	"../remc2/engine/Type_D93C0_Bldgprmbuffer.cpp"
    "../remc2/engine/Type_D93C0_Bldgprmbuffer.h"
	"../remc2/engine/Type_SecretMapScreenPortals_E2970.cpp"
    "../remc2/engine/Type_SecretMapScreenPortals_E2970.h"	
	"../remc2/engine/Type_MapScreenPortals_E17CC.cpp"
    "../remc2/engine/Type_MapScreenPortals_E17CC.h"
	"../remc2/engine/MenusAndIntros.cpp"
    "../remc2/engine/MenusAndIntros.h"
	"../remc2/engine/ProjectionPolygon.cpp"
    "../remc2/engine/ProjectionPolygon.h"
	"../remc2/engine/Network.cpp"
    "../remc2/engine/Network.h"	
	"../remc2/engine/RenderThread.cpp"
    "../remc2/engine/RenderThread.h"
	"../remc2/engine/GameRenderOriginal.cpp"
    "../remc2/engine/GameRenderOriginal.h"
	"../remc2/engine/GameRenderHD.cpp"
    "../remc2/engine/GameRenderHD.h"
	"../remc2/engine/TextureMaps.cpp"
    "../remc2/engine/TextureMaps.h"
	"../remc2/engine/Animation.cpp"
    "../remc2/engine/Animation.h"
	"../remc2/engine/read_config.cpp"
    "../remc2/engine/read_config.h"
	"../remc2/engine/XUnk_D4350.cpp"
    "../remc2/engine/XUnk_D4350.h"
    "../remc2/engine/Unk_D47E0.cpp"
    "../remc2/engine/Unk_D47E0.h"
    "../remc2/engine/Unk_D4A30.cpp"
    "../remc2/engine/Unk_D4A30.h"
	"../remc2/engine/Basic.cpp"
    "../remc2/engine/Basic.h"
	"../remc2/engine/Sound.cpp"
    "../remc2/engine/Sound.h"
    "../remc2/engine/engine_support.cpp"
    "../remc2/engine/engine_support.h"
    "../remc2/engine/Terrain.cpp"
    "../remc2/engine/Terrain.h"
    "utilities/Math.cpp"
    "utilities/Math.h"
    "comparison/engine/OriginalTerrain.cpp"
    "comparison/engine/OriginalTerrain.h"
    "engine/TerrainUT.cpp"
    "UnitTests.cpp"
)
source_group("Source Files\\unittests" FILES ${SOURCE_FILES_UNITTESTS})

set(ROOT_NAMESPACE remc2-test)


################################################################################
# Unit tests
################################################################################
include(GoogleTest)
add_executable(${PROJECT_NAME} ${SOURCE_FILES_UNITTESTS})
target_include_directories(${PROJECT_NAME} PUBLIC
    "../remc2/engine"
    "../remc2/portability"
    "../findfirst"
    "../inih"
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    gtest
    pthread
	SDL2::Main
	SDL2::Mixer
	${PNG_LIBRARY}
	findfirst
	ADLMIDI_static
	Boost::system
)
add_compile_definitions(COMPILE_FOR_64BIT)
gtest_add_tests(
    TARGET      ${PROJECT_NAME}
    TEST_SUFFIX .noArgs
    TEST_LIST   noArgsTests
)
set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)


if (USE_SANITIZERS)
    set(SANITIZER_OPTIONS
        -fsanitize=address
        -fsanitize=pointer-compare
        -fsanitize=pointer-subtract
        -fsanitize=leak
        -fsanitize=undefined
        -static-libasan
        -static-liblsan
        -static-libubsan
    )
    target_link_options(${PROJECT_NAME} PRIVATE ${SANITIZER_OPTIONS})
endif ()