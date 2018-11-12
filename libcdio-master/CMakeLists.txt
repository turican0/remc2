cmake_minimum_required(VERSION 3.2)

project(libcdio VERSION 0.9.3 LANGUAGES C)

if(WIN32)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

set(LIBCDIO_VERSION_NUM 93)
configure_file(
  include/cdio/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/cdio/version.h
  @ONLY
)

add_library(libcdio
  ${CMAKE_CURRENT_BINARY_DIR}/cdio/version.h
  lib/driver/cdtext.c
  lib/driver/cdtext_private.h
  lib/driver/device.c
  lib/driver/disc.c
  lib/driver/ds.c
  lib/driver/filemode.h
  lib/driver/generic.h
  lib/driver/image.h
  lib/driver/image_common.c
  lib/driver/image_common.h
  lib/driver/logging.c
  lib/driver/memory.c
  lib/driver/portable.h
  lib/driver/read.c
  lib/driver/realpath.c
  lib/driver/sector.c
  lib/driver/track.c
  lib/driver/utf8.c
  lib/driver/util.c
  lib/driver/_cdio_generic.c
  lib/driver/_cdio_stdio.c
  lib/driver/_cdio_stdio.h
  lib/driver/_cdio_stream.c
  lib/driver/_cdio_stream.h
  lib/driver/abs_path.c
  lib/driver/audio.c
  lib/driver/cd_types.c
  lib/driver/cdio.c
  lib/driver/cdio_assert.h
  lib/driver/cdio_private.h
  lib/driver/image/bincue.c
  lib/driver/image/cdrdao.c
  lib/driver/image/nrg.h
  lib/driver/image/nrg.c
  lib/driver/MSWindows/win32.h
  lib/driver/MSWindows/win32.c
  lib/driver/MSWindows/win32_ioctl.c
  lib/driver/mmc/mmc.c
  lib/driver/mmc/mmc_cmd_helper.h
  lib/driver/mmc/mmc_hl_cmds.c
  lib/driver/mmc/mmc_ll_cmds.c
  lib/driver/mmc/mmc_private.h
  lib/driver/mmc/mmc_util.c
  lib/iso9660/iso9660.c
  lib/iso9660/iso9660_fs.c
  lib/iso9660/iso9660_private.h
  lib/iso9660/rock.c
  lib/iso9660/xa.c
  lib/udf/filemode.c
  lib/udf/udf.c
  lib/udf/udf_file.c
  lib/udf/udf_fs.c
  lib/udf/udf_fs.h
  lib/udf/udf_private.h
  lib/udf/udf_time.c
  include/cdio/posix.h
  include/cdio/read.h
  include/cdio/rock.h
  include/cdio/scsi_mmc.h
  include/cdio/sector.h
  include/cdio/track.h
  include/cdio/types.h
  include/cdio/udf.h
  include/cdio/udf_file.h
  include/cdio/udf_time.h
  include/cdio/utf8.h
  include/cdio/util.h
  include/cdio/xa.h
  include/cdio/audio.h
  include/cdio/bytesex.h
  include/cdio/bytesex_asm.h
  include/cdio/cd_types.h
  include/cdio/cdio.h
  include/cdio/cdtext.h
  include/cdio/device.h
  include/cdio/disc.h
  include/cdio/ds.h
  include/cdio/dvd.h
  include/cdio/ecma_167.h
  include/cdio/iso9660.h
  include/cdio/logging.h
  include/cdio/memory.h
)
target_include_directories(
  libcdio PRIVATE
  $<BUILD_INTERFACE:include;lib;lib/udf;lib/iso9660;lib/driver>
  $<BUILD_INTERFACE:lib/driver/mmc;lib/driver/image;lib/driver/MSWindows>
  $<BUILD_INTERFACE:MSVC;${CMAKE_CURRENT_BINARY_DIR}>
  INTERFACE
  $<INSTALL_INTERFACE:include/cdio>
)

find_package(iconv NO_MODULE)
target_link_libraries(libcdio PRIVATE winmm.lib iconv::iconv)

target_compile_definitions(libcdio
  PRIVATE
  BUILDING_CDIO
  HAVE_CONFIG_H
  _CRT_SECURE_NO_WARNINGS
  _CRT_NONSTDC_NO_DEPRECATE
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/libcdio-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(TARGETS libcdio EXPORT libcdio
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)


install(DIRECTORY
  include/
  DESTINATION include
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/cdio/version.h
  DESTINATION include/cdio)

install(EXPORT libcdio
  FILE
    libcdio.cmake
  NAMESPACE
    libcdio::
  DESTINATION
    lib/cmake/libcdio
)
install(
  FILES
    cmake/libcdio-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libcdio-config-version.cmake
  DESTINATION
    lib/cmake/libcdio
)

if(MSVC AND BUILD_SHARED_LIBS)
  install(FILES 
    $<TARGET_PDB_FILE:libcdio>
    DESTINATION lib
  )
endif()