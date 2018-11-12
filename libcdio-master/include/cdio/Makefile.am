#   Copyright (C) 2003, 2004, 2006, 2008, 2011, 2012
#    Rocky Bernstein <rocky@gnu.org>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.

########################################################
# Things to make the install (public) libcdio headers
########################################################
#

cdio_config.h: $(top_builddir)/config.h
	@echo  '#ifndef __CDIO_CONFIG_H__' > cdio_config.h
	@echo  '#define __CDIO_CONFIG_H__' >> cdio_config.h
	@@SED@ @SED_EXTENDED_RE_FLAG@ -e 's/^(#[ \t]*define[ \t]+)/\1CDIO_/' $(top_builddir)/config.h >>cdio_config.h
	@echo '#endif /* #ifndef CDIO_CONFIG_H */' >>cdio_config.h

libcdioincludedir=$(includedir)/cdio
dist_libcdioinclude_HEADERS = \
	audio.h \
	bytesex.h \
	bytesex_asm.h \
	cdio.h \
	cd_types.h \
	cdtext.h \
	device.h \
	disc.h \
	ds.h \
	dvd.h \
	ecma_167.h \
	iso9660.h \
	logging.h \
	memory.h \
	mmc.h \
	mmc_cmds.h \
	mmc_hl_cmds.h \
	mmc_ll_cmds.h \
	mmc_util.h \
	posix.h \
	read.h \
	rock.h \
	sector.h \
        track.h \
        types.h \
	udf.h \
	udf_file.h \
	udf_time.h \
	utf8.h \
	util.h \
	version.h \
	xa.h

nodist_libcdioinclude_HEADERS = cdio_config.h

EXTRA_DIST = version.h.in
BUILT_SOURCES = version.h

DISTCLEANFILES = cdio_config.h
