cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(remc2 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


################################################################################
# Dependencies
################################################################################

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

if (GITHUB_CI_COMPATIBILITY_PATH)
    set(SDL2_DIR "${GITHUB_CI_COMPATIBILITY_PATH}/cmake/SDL2")
    set(CMAKE_LIBRARY_PATH "${GITHUB_CI_COMPATIBILITY_PATH}")
endif ()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(PNG REQUIRED)
find_package(Boost REQUIRED system)


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
    add_compile_options(-Wall)
    add_compile_options(-Wno-unknown-pragmas)
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
add_compile_definitions(COMPILE_FOR_64BIT)

# preprocessor flags
if (FLATPAK)
    add_compile_definitions(FLATPAK)
endif ()
        
# remc2 and tests
add_subdirectory(remc2)
if (UNIT_TESTS)
    add_subdirectory(remc2-unit-test)
    add_subdirectory(remc2-regression-test)
endif ()