cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(remc2 LANGUAGES C CXX)

include(GNUInstallDirs)
include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (USE_CLANG_TIDY)
    #set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
    #set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters;-fix")
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=misc-unused-parameters")
endif ()

################################################################################
# Source groups
################################################################################
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DATADIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES ${SRCDIR}/remc2/*.cpp ${SRCDIR}/remc2/*.h)
source_group("Source Files" FILES ${SOURCE_FILES})

file(GLOB_RECURSE EDITOR_FILES ${SRCDIR}/remc2/*.cpp ${SRCDIR}/remc2/*.h)
source_group("Editor Files" FILES ${SOURCE_FILES})

file(GLOB_RECURSE DATA_FILES FILES ${DATADIR}/*)
source_group("Data Assets" FILES ${DATA_FILES}/*)

set(SOURCE_FILES__libs__inih
	"${SRCDIR}/libs/inih/ini.c"
	"${SRCDIR}/libs/inih/ini.h"
	"${SRCDIR}/libs/inih/cpp/INIReader.cpp"
	"${SRCDIR}/libs/inih/cpp/INIReader.h"
)
source_group("Source Files\\libs\\inih" FILES ${SOURCE_FILES__libs__inih})

set(SOURCE_FILES__libs__findfirst
	"${SRCDIR}/libs/findfirst/findfirst.c"
	"${SRCDIR}/libs/findfirst/findfirst.h"
	"${SRCDIR}/libs/findfirst/spec.c"
	"${SRCDIR}/libs/findfirst/spec.h"
)
source_group("Source Files\\libs\\findfirst" FILES ${SOURCE_FILES__libs__findfirst})

set(SOURCE_FILES_UNITTESTS
	"${SRCDIR}/remc2/CommandLineParser.cpp"
	"${SRCDIR}/remc2/CommandLineParser.h"
    	"${SRCDIR}/remc2/CommandLineParserUT.cpp"
    	"${SRCDIR}/remc2/fcaseopen.cpp"
    	"${SRCDIR}/remc2/fcaseopen.h"
    	"${SRCDIR}/remc2/fcaseopenUT.cpp"
    	"${SRCDIR}/remc2/UnitTests.cpp"
)
source_group("Source Files\\unittests" FILES ${SOURCE_FILES_UNITTESTS})

if (WIN32)
	set(WIN32_FILES
	    "${SRCDIR}/win32/stdafx.h"
	    "${SRCDIR}/win32/targetver.h"
	    "${SRCDIR}/win32/stdafx.cpp"
    )
else ()
	set(WIN32_FILES
    )
endif ()
source_group("Win32 Files" FILES ${HEADER_FILES})

set(ALL_FILES
	${WIN32_FILES}
	${SOURCE_FILES}
	${EDITOR_FILES}
	${SOURCE_FILES__libs__inih}
	${SOURCE_FILES__libs__findfirst}
)
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

pkg_search_module(ADLMIDI REQUIRED libADLMIDI)
pkg_search_module(SDL2_MIXER_EXT REQUIRED SDL2_mixer_ext)
#pkg_search_module(INIH REQUIRED inih)
#pkg_search_module(INIREADER REQUIRED INIReader)
#pkg_search_module(FINDFIRST REQUIRED findfirst)
#pkg_search_module(DIRENT REQUIRED dirent)

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
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    add_compile_options(-fpermissive)
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


# preprocessor flags
if (FLATPAK)
    add_compile_definitions(FLATPAK)
endif ()




################################################################################
# targets
################################################################################
add_library(findfirst ${SOURCE_FILES__libs__findfirst})
add_executable(${PROJECT_NAME} ${ALL_FILES})

install(
    TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug;Release;RelWithDebInfo
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
)
if (WIN32)
    add_precompiled_header(${PROJECT_NAME} "stdafx.h" "stdafx.cpp")
endif (WIN32)

set(ROOT_NAMESPACE remc2)


################################################################################
# Linux settings
################################################################################
if (UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC
	    "${SRCDIR}/libs/inih"
	    "${SRCDIR}/libs/inih/cpp"
	    "${SRCDIR}/libs//findfirst"
	    "${ADLMIDI_INCLUDES}"
    )
    #target_compile_definitions(${PROJECT_NAME} PRIVATE
    #)
    #target_compile_options(${PROJECT_NAME} PRIVATE
    #)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        SDL2::Main
        SDL2::Mixer
        ${PNG_LIBRARY}
        findfirst
	${ADLMIDI_LIBRARIES}
        Boost::system
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
    )
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:DEBUG>:_DEBUG>"
    )
endif ()


################################################################################
# Unit tests
################################################################################
if (UNIT_TESTS)
    include(GoogleTest)

    add_executable(remc2unittests ${SOURCE_FILES_UNITTESTS})
    target_include_directories(remc2unittests PUBLIC
    )
    target_link_libraries(remc2unittests PUBLIC 
        gtest
        pthread
    )
    add_compile_definitions(COMPILE_FOR_64BIT)
    gtest_add_tests(
        TARGET      remc2unittests
        TEST_SUFFIX .noArgs
        TEST_LIST   noArgsTests
    )
    set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 10)
    install(
        TARGETS remc2unittests
        CONFIGURATIONS Debug;Release;RelWithDebInfo
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
    )
endif ()

if (UNIX)
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
        target_link_options(remc2 PRIVATE ${SANITIZER_OPTIONS})
        if (UNIT_TESTS)
            target_link_options(remc2unittests PRIVATE ${SANITIZER_OPTIONS})
        endif () 
        if (EDITOR)
            target_link_options(remc2editor PRIVATE ${SANITIZER_OPTIONS})
        endif ()
    endif ()
endif ()

target_compile_options(${PROJECT_NAME} PUBLIC
	-DDATADIR=\"${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/\"
	)

message("-- Data files will be installed to: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/")

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Config )
install(DIRECTORY ${DATADIR}  TYPE DATA COMPONENT assets)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

