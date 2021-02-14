project(remc2editor C CXX)

set(SOURCE_FILES_EDITOR
    "editor.cpp"
    "editor_main.cpp"
    "editor.h"
    "../../kiss_sdl-master/kiss_sdl.h"
    "../../kiss_sdl-master/kiss_widgets.cpp"
    "../../kiss_sdl-master/kiss_general.cpp"
    "../../kiss_sdl-master/kiss_draw.cpp"
)
foreach(FILE_ENTRY ${SOURCE_FILES_ENGINE} ${SOURCE_FILES__portability} ${SOURCE_FILES__libs__inih})
  list(APPEND SOURCE_FILES_EDITOR "../${FILE_ENTRY}")
endforeach()
source_group("Source Files Editor" FILES ${SOURCE_FILES_EDITOR} )

add_executable(${PROJECT_NAME} ${SOURCE_FILES_EDITOR})

target_link_libraries(${PROJECT_NAME} PUBLIC 
    SDL2::Main
    SDL2::TTF
    SDL2::Image
    SDL2::Mixer
    ${PNG_LIBRARY}
    findfirst
    ADLMIDI_static
)
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/kiss_sdl-master"
)
if (USE_64_BIT)
    add_compile_definitions(COMPILE_FOR_64BIT)
else ()
    set_target_properties(remc2editor PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif ()

install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)

if (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_SOURCE_DIR}/inih"
        "${CMAKE_SOURCE_DIR}/findfirst"
    )
endif ()