cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(remc2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)


################################################################################
# Dependencies
################################################################################

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

if (GITHUB_CI_COMPATIBILITY_PATH)
    set(SDL2_DIR "${GITHUB_CI_COMPATIBILITY_PATH}/cmake/SDL2")
    set(CMAKE_LIBRARY_PATH "${GITHUB_CI_COMPATIBILITY_PATH}")
endif ()

find_package(SDL2 REQUIRED)

if (GITHUB_CI_COMPATIBILITY_PATH)
    # CI cross-compile mód — přímé importované targety bez find_package
    foreach(_entry
        "SDL2_mixer|SDL_mixer.h|SDL2_mixer"
        "SDL2_ttf|SDL_ttf.h|SDL2_ttf"
        "SDL2_image|SDL_image.h|SDL2_image")

        string(REPLACE "|" ";" _pkg "${_entry}")
        list(GET _pkg 0 _name)
        list(GET _pkg 1 _header)
        list(GET _pkg 2 _lib)

        find_path(_inc ${_header}
            PATH_SUFFIXES SDL2
            PATHS
                "${GITHUB_CI_COMPATIBILITY_PATH}/../include"
                "/usr/include"
            NO_DEFAULT_PATH
        )
        find_library(_libpath
            NAMES ${_lib}
            PATHS "${GITHUB_CI_COMPATIBILITY_PATH}"
            NO_DEFAULT_PATH
        )
        if(NOT _inc OR NOT _libpath)
            message(FATAL_ERROR "Nelze najít ${_name} v ${GITHUB_CI_COMPATIBILITY_PATH}")
        endif()
        add_library(${_name}::${_name} UNKNOWN IMPORTED)
        set_target_properties(${_name}::${_name} PROPERTIES
            IMPORTED_LOCATION "${_libpath}"
            INTERFACE_INCLUDE_DIRECTORIES "${_inc}"
        )
        message(STATUS "Found ${_name} (CI): ${_libpath}")
        unset(_inc CACHE)
        unset(_libpath CACHE)
    endforeach()

else()
    # Lokální / nativní build — standardní find_package s fallbackem
    foreach(_entry
        "SDL2_mixer|SDL_mixer.h|SDL2_mixer"
        "SDL2_ttf|SDL_ttf.h|SDL2_ttf"
        "SDL2_image|SDL_image.h|SDL2_image")

        string(REPLACE "|" ";" _pkg "${_entry}")
        list(GET _pkg 0 _name)
        list(GET _pkg 1 _header)
        list(GET _pkg 2 _lib)

        find_package(${_name} QUIET CONFIG)
        if(NOT ${_name}_FOUND)
            find_path(_inc ${_header} PATH_SUFFIXES SDL2)
            find_library(_libpath NAMES ${_lib})
            if(NOT _inc OR NOT _libpath)
                message(FATAL_ERROR "Nelze najít ${_name}. Nainstaluj lib${_lib}-dev.")
            endif()
            if(NOT TARGET ${_name}::${_name})
                add_library(${_name}::${_name} UNKNOWN IMPORTED)
                set_target_properties(${_name}::${_name} PROPERTIES
                    IMPORTED_LOCATION "${_libpath}"
                    INTERFACE_INCLUDE_DIRECTORIES "${_inc}"
                )
            endif()
            message(STATUS "Found ${_name} (fallback): ${_libpath}")
            unset(_inc CACHE)
            unset(_libpath CACHE)
        endif()
    endforeach()
endif()

find_package(PNG REQUIRED)
find_package(fmt REQUIRED)

# Vulkan renderer (GameRenderHW / VulkanPolygonRenderer)
find_package(Vulkan REQUIRED)

find_program(GLSLC_EXECUTABLE
    NAMES glslc
    HINTS "${Vulkan_GLSLC_EXECUTABLE}" "$ENV{VULKAN_SDK}/bin" "$ENV{VULKAN_SDK}/Bin"
)
if (NOT GLSLC_EXECUTABLE)
    find_program(GLSLANG_VALIDATOR_EXECUTABLE
        NAMES glslangValidator
        HINTS "${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}" "$ENV{VULKAN_SDK}/bin" "$ENV{VULKAN_SDK}/Bin"
    )
    if (NOT GLSLANG_VALIDATOR_EXECUTABLE)
        message(FATAL_ERROR "No GLSL compiler found. Install glslc (shaderc) or glslangValidator.")
    endif ()
endif ()

# Distro packages for these are patchy, so fall back to fetching the same
# versions vcpkg uses. FetchContent needs network access at configure time.
include(FetchContent)

find_package(VulkanMemoryAllocator CONFIG QUIET)
if (NOT TARGET GPUOpen::VulkanMemoryAllocator)
    find_path(VMA_INCLUDE_DIR vk_mem_alloc.h PATH_SUFFIXES vma)
    if (VMA_INCLUDE_DIR)
        add_library(GPUOpen::VulkanMemoryAllocator INTERFACE IMPORTED)
        set_target_properties(GPUOpen::VulkanMemoryAllocator PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${VMA_INCLUDE_DIR}"
        )
        message(STATUS "Found VulkanMemoryAllocator (header): ${VMA_INCLUDE_DIR}")
    else ()
        message(STATUS "VulkanMemoryAllocator not found, fetching v3.1.0")
        FetchContent_Declare(VulkanMemoryAllocator
            GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
            GIT_TAG v3.1.0
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(VulkanMemoryAllocator)
    endif ()
endif ()

find_package(vk-bootstrap CONFIG QUIET)
if (NOT TARGET vk-bootstrap::vk-bootstrap)
    # vk-bootstrap copes with older Vulkan headers but not newer ones, so match
    # its release to the system headers when they are newer than our baseline.
    set(VK_BOOTSTRAP_TAG v1.4.312)
    if (Vulkan_VERSION VERSION_GREATER 1.4.312)
        set(VK_BOOTSTRAP_TAG v${Vulkan_VERSION})
    endif ()
    message(STATUS "vk-bootstrap not found, fetching ${VK_BOOTSTRAP_TAG}")
    FetchContent_Declare(vk-bootstrap
        GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
        GIT_TAG ${VK_BOOTSTRAP_TAG}
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(vk-bootstrap)
endif ()

set(VULKAN_LIBRARIES
    Vulkan::Vulkan
    GPUOpen::VulkanMemoryAllocator
    vk-bootstrap::vk-bootstrap
    ${CMAKE_DL_LIBS}
)

# Compiles the renderer's GLSL shaders to SPIR-V in <target dir>/shaders,
# where VulkanPolygonRenderer loads them from at runtime.
function(add_vulkan_shaders TARGET)
    set(_shader_dir "${CMAKE_SOURCE_DIR}/remc2/portability/shaders")
    set(_out_dir "$<TARGET_FILE_DIR:${TARGET}>/shaders")
    set(_outputs)
    foreach (_shader polygon.vert polygon.frag wireframe.frag overlay.vert overlay.frag hud.frag)
        set(_out "${CMAKE_CURRENT_BINARY_DIR}/shaders/${_shader}.spv")
        if (GLSLC_EXECUTABLE)
            set(_compile "${GLSLC_EXECUTABLE}" "${_shader_dir}/${_shader}" -o "${_out}")
        else ()
            set(_compile "${GLSLANG_VALIDATOR_EXECUTABLE}" -V "${_shader_dir}/${_shader}" -o "${_out}")
        endif ()
        add_custom_command(
            OUTPUT "${_out}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders"
            COMMAND ${_compile}
            DEPENDS "${_shader_dir}/${_shader}"
            COMMENT "Compiling shader ${_shader}"
            VERBATIM
        )
        list(APPEND _outputs "${_out}")
    endforeach ()
    add_custom_target(${TARGET}-shaders DEPENDS ${_outputs})
    add_dependencies(${TARGET} ${TARGET}-shaders)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders" "${_out_dir}"
        VERBATIM
    )
    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders" DESTINATION bin)
endfunction()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_C_FLAGS "")
    set(CMAKE_C_FLAGS_DEBUG "")
    set(CMAKE_C_FLAGS_RELEASE "")
    set(CMAKE_C_FLAGS_XBOX_DEBUG "")
    set(CMAKE_C_FLAGS_XBOX_RELEASE "")
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_XBOX_DEBUG "")
    set(CMAKE_CXX_FLAGS_XBOX_RELEASE "")
else()
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    add_compile_options(-fpermissive)
    add_compile_options(-Wno-narrowing)
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-pedantic)
    add_compile_options(-Wno-unknown-pragmas)
    add_compile_options(${SPDLOG_CFLAGS_OTHER})

    # Linux/ARM
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
        add_compile_options(-fsigned-char)
    endif ()

    if (USE_SANITIZERS)
        add_compile_options(-fsanitize=address)
        add_compile_options(-fsanitize=pointer-compare)
        add_compile_options(-fsanitize=pointer-subtract)
        add_compile_options(-fsanitize=leak)
        add_compile_options(-fsanitize=undefined)
    endif ()
endif()


################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
else()
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
    set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS})
endif()


################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

# dependencies
set(libADLMIDI_STATIC TRUE)
add_subdirectory(libADLMIDI-master)
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

# remc2 and tests
add_subdirectory(remc2)
add_subdirectory(remc2-editor)
add_subdirectory(remc2-configurator)
if (UNIT_TESTS)
    find_package(GTest REQUIRED)
    add_subdirectory(remc2-unit-test)
    add_subdirectory(remc2-regression-test)
endif ()
