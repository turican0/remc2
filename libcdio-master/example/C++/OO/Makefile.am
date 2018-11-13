#   Copyright (C) 2005-2006, 2008-2009, 2017 Rocky Bernstein <rocky@gnu.org>
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

############################################################
# Sample C++ programs using libcdio++ (with C++ OO wrapper)
############################################################
#
noinst_PROGRAMS = cdtext device drives eject \
	          isofile isofile2 isolist iso4 mmc1 mmc2 tracks

AM_CPPFLAGS = -I$(top_srcdir)/include $(LIBCDIO_CFLAGS)

cdtext_SOURCES      = cdtext.cpp
cdtext_DEPENDENCIES = $(LIBCDIO_DEPS)
cdtext_LDADD        = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

device_SOURCES      = device.cpp
device_DEPENDENCIES = $(LIBCDIO_DEPS)
device_LDADD        = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

drives_SOURCES      = drives.cpp
drives_DEPENDENCIES = $(LIBCDIO_DEPS)
drives_LDADD        = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

eject_SOURCES       = eject.cpp
eject_DEPENDENCIES  = $(LIBCDIO_DEPS)
eject_LDADD         = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

isofile_SOURCES     = isofile.cpp
isofile_LDADD       = $(LIBISO9660PP_LIBS) $(LIBISO9660_LIBS) \
	              $(LIBCDIOPP_LIBS) $(LTLIBICONV)

isofile2_SOURCES    = isofile2.cpp
isofile2_LDADD      = $(LIBISO9660PP_LIBS) $(LIBISO9660_LIBS) \
	              $(LIBCDIOPP_LIBS) $(LTLIBICONV)

isolist_SOURCES     = isolist.cpp
isolist_LDADD       = $(LIBISO9660PP_LIBS) $(LIBISO9660_LIBS) \
	              $(LIBCDIOPP_LIBS) $(LTLIBICONV)

iso4_SOURCES        = iso4.cpp
iso4_LDADD          = $(LIBISO9660PP_LIBS) $(LIBISO9660_LIBS) \
	              $(LIBCDIOPP_LIBS) $(LTLIBICONV)

mmc1_SOURCES        = mmc1.cpp
mmc1_DEPENDENCIES   = $(LIBCDIO_DEPS)
mmc1_LDADD          = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

mmc2_SOURCES        = mmc2.cpp
mmc2_DEPENDENCIES   = $(LIBCDIO_DEPS)
mmc2_LDADD          = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

tracks_SOURCES      = tracks.cpp
tracks_LDADD        = $(LIBCDIOPP_LIBS) $(LIBCDIO_LIBS)

check_PROGRAMS      = $(noinst_PROGRAMS)

TESTS               = $(check_PROGRAMS)

#: run valgrind on C++ programs
check-leaks: $(check_PROGRAMS)
	for p in $(check_PROGRAMS); do \
	   valgrind ./$$p; \
        done
