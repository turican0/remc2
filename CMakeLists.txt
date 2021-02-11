cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(remc2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x86")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()



################################################################################
# Conan setup
################################################################################

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()



################################################################################
# Dependencies
################################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

if (COMPATIBILITY_32_PATH)
    set(SDL2_DIR "${COMPATIBILITY_32_PATH}/cmake/SDL2")
    set(CMAKE_LIBRARY_PATH "${COMPATIBILITY_32_PATH}")
endif ()

list(INSERT 0 CMAKE_PREFIX_PATH /usr)
if (USE_64_BIT)
    message("Use 64 bit")
    set(FIND_LIBRARY_USE_LIB64_PATHS True)
    set(PNG_LIBRARY "/usr/lib64/libpng.so")
    set(SDL2_LIBRARY "/usr/lib64/libSDL2.so")
    set(SDL2_MIXER_LIBRARY "/usr/lib64/libSDL2_mixer.so")
    set(CMAKE_LIBRARY_ARCHITECTURE x86_64)
else ()
    message("Use 32 bit")
    set(PNG_LIBRARY "/usr/lib/libpng.so")
    set(SDL2_LIBRARY "/usr/lib/libSDL2.so")
    set(SDL2_MIXER_LIBRARY "/usr/lib/libSDL2_mixer.so")
    set(CMAKE_LIBRARY_ARCHITECTURE i686)
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(PNG REQUIRED)


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
    #add_compile_options(-fpermissive)
    add_compile_options(-Wno-narrowing)
    if (ALL_WARNINGS)
        add_compile_options(-Wall)
    else ()
        add_compile_options(-Wunused)
        add_compile_options(-Wunused-function)
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
if (USE_64_BIT)
    add_compile_definitions(COMPILE_FOR_64BIT)
else ()
    set_target_properties(ADLMIDI_static PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif ()
        
# remc2
add_subdirectory(remc2)

