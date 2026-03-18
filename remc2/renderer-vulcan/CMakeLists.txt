cmake_minimum_required(VERSION 3.20)
project(VulkanRenderer CXX)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------
# Locate Vulkan SDK
# ---------------------------------------------------------------------------
find_package(Vulkan REQUIRED)

# ---------------------------------------------------------------------------
# SPIR-V compilation
# Converts each .vert / .frag file to a .spv binary, then wraps it as a
# C header (uint32_t array) so the renderer can embed shaders at compile time.
# ---------------------------------------------------------------------------
find_program(GLSLC glslc HINTS "${Vulkan_GLSLC_EXECUTABLE}" ENV VULKAN_SDK
             PATH_SUFFIXES bin)

if(NOT GLSLC)
    message(FATAL_ERROR "glslc not found. Install the Vulkan SDK.")
endif()

set(SHADER_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_OUT   "${CMAKE_CURRENT_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY "${SHADER_OUT}")

# Helper: compile one shader and generate a C header with the SPIR-V bytes
function(compile_shader SOURCE_FILE STAGE)
    get_filename_component(BASE "${SOURCE_FILE}" NAME_WE)
    get_filename_component(EXT  "${SOURCE_FILE}" EXT)

    # Remove leading dot from extension for naming
    string(REGEX REPLACE "^\\." "" EXT_CLEAN "${EXT}")

    set(SPV_FILE    "${SHADER_OUT}/${BASE}.${EXT_CLEAN}.spv")
    set(HEADER_FILE "${SHADER_OUT}/${BASE}.${EXT_CLEAN}.spv.h")
    set(ARRAY_NAME  "${BASE}_${EXT_CLEAN}_spv")

    # Compile GLSL -> SPIR-V
    add_custom_command(
        OUTPUT  "${SPV_FILE}"
        COMMAND "${GLSLC}" -fshader-stage=${STAGE}
                           "${SHADER_DIR}/${SOURCE_FILE}"
                           -o "${SPV_FILE}"
        DEPENDS "${SHADER_DIR}/${SOURCE_FILE}"
        COMMENT "Compiling shader ${SOURCE_FILE} -> ${BASE}.${EXT_CLEAN}.spv"
    )

    # Wrap SPIR-V as a C uint32_t array header
    # We use a small CMake script to do the conversion portably.
    add_custom_command(
        OUTPUT  "${HEADER_FILE}"
        COMMAND ${CMAKE_COMMAND}
                    -DINPUT="${SPV_FILE}"
                    -DOUTPUT="${HEADER_FILE}"
                    -DARRAY_NAME="${ARRAY_NAME}"
                    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/spv_to_header.cmake"
        DEPENDS "${SPV_FILE}"
        COMMENT "Generating header ${BASE}.${EXT_CLEAN}.spv.h"
    )

    # Propagate the header path to the parent scope
    set(SHADER_HEADERS ${SHADER_HEADERS} "${HEADER_FILE}" PARENT_SCOPE)
endfunction()

set(SHADER_HEADERS "")
compile_shader(SolidFill.vert vertex)
compile_shader(SolidFill.frag fragment)
compile_shader(Textured.vert  vertex)
compile_shader(Textured.frag  fragment)

add_custom_target(ShaderHeaders ALL DEPENDS ${SHADER_HEADERS})

# ---------------------------------------------------------------------------
# VulkanRenderer static library
# ---------------------------------------------------------------------------
add_library(VulkanRenderer STATIC
    VulkanRenderer.cpp
    VulkanRenderer.h
    GameRenderVulkan.cpp
    GameRenderVulkan.h
)

add_dependencies(VulkanRenderer ShaderHeaders)

target_include_directories(VulkanRenderer
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${SHADER_OUT}"          # for the generated .spv.h headers
    PRIVATE
        "${Vulkan_INCLUDE_DIRS}"
)

target_link_libraries(VulkanRenderer PUBLIC Vulkan::Vulkan)

target_compile_options(VulkanRenderer PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# ---------------------------------------------------------------------------
# Quick smoke-test executable (optional — remove for production builds)
# ---------------------------------------------------------------------------
option(VR_BUILD_TEST "Build the headless smoke test" OFF)
if(VR_BUILD_TEST)
    add_executable(VulkanRendererTest test/smoke_test.cpp)
    target_link_libraries(VulkanRendererTest PRIVATE VulkanRenderer)
endif()
