set(CONFIGURATOR_TARGET remc2-configurator)

if (WIN32)
	set(APP_ICON_RESOURCE ${CMAKE_SOURCE_DIR}/icons/config.rc)
endif()

set(SOURCE_FILES_CONFIGURATOR
    "Remc2ConfiguratorApp.cpp"
    "../remc2/portability/Config.cpp"
    "../remc2/portability/Config.h"
	"SettingsDialog.cpp"
	"SettingsDialog.h"
	"LaunchOptionsDialog.cpp"
	"LaunchOptionsDialog.h"
    "MultiplayerDialog.cpp"
	"MultiplayerDialog.h"
	"ControlsDialog.cpp"
    "ControlsDialog.h"
    "DisplayDialog.cpp"
    "DisplayDialog.h"
    "GameDialog.cpp"
    "GameDialog.h"
    "GameFilesDialog.cpp"
    "GameFilesDialog.h"
    "GraphicsDialog.cpp"
    "GraphicsDialog.h"
    "GraphicsFoldersDialog.cpp"
    "GraphicsFoldersDialog.h"
    "MainFrame.cpp"
    "MainFrame.h"
    "PathHelpers.cpp"
    "PathHelpers.h"
    "SoundDialog.cpp"
    "SoundDialog.h"
    "ThreadingDialog.cpp"
    "ThreadingDialog.h"
	"ZoneEditorCtrl.cpp"
    "ZoneEditorCtrl.h"
    "ZoneEditorDialog.cpp"
    "ZoneEditorDialog.h"
)

source_group("Source Files Configurator" FILES ${SOURCE_FILES_CONFIGURATOR})

set(ROOT_NAMESPACE ${CONFIGURATOR_TARGET})

# Find wxWidgets
find_package(wxWidgets REQUIRED)

# Create executable BEFORE adding libraries
add_executable(${CONFIGURATOR_TARGET} ${SOURCE_FILES_CONFIGURATOR})

# Now add link libraries
target_link_libraries(${CONFIGURATOR_TARGET} PUBLIC
    wxWidgets::wxWidgets
)

target_include_directories(${CONFIGURATOR_TARGET} PUBLIC
    "../spdlog/include"
)

if (APPLE)
    execute_process(COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    target_include_directories(${CONFIGURATOR_TARGET} PUBLIC
        "${HOMEBREW_PREFIX}/include"
        ${PNG_INCLUDE_DIRS}
    )

    # See remc2/CMakeLists.txt — pack(1) structs require classic relocations.
    target_link_options(${CONFIGURATOR_TARGET} PRIVATE "-Wl,-no_fixup_chains")
endif()

add_compile_definitions(COMPILE_FOR_64BIT)

install(
    TARGETS ${CONFIGURATOR_TARGET}
    CONFIGURATIONS Debug Release RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

install(
    FILES
    DESTINATION bin
)

install(
    DIRECTORY
    DESTINATION bin
)

if (USE_SANITIZERS)
    target_link_options(${CONFIGURATOR_TARGET} PRIVATE ${SANITIZER_OPTIONS})
endif()