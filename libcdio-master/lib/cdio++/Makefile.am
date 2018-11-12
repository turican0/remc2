#   $Id: Makefile.am,v 1.11 2008/10/29 09:53:00 rocky Exp $
#
#   Copyright (C) 2005, 2006, 2007, 2008 Rocky Bernstein <rocky@gnu.org>
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
# Things to make the libcdio++ library
########################################################
#
# From libtool documentation amended with guidance from N. Boullis:
#
#  1. Start with version information of `0:0:0' for each libtool library.
#
#  2. It is probably not a good idea to update the version information
#     several times between public releases, but rather once per public
#     release. (This seems to be more an aesthetic consideration than
#     a hard technical one.)
#
#  3. If the library source code has changed at all since the last
#     update, then increment REVISION (`C:R:A' becomes `C:R+1:A').
#
#  4. If any interfaces have been added, removed, or changed since the
#     last update, increment CURRENT, and set REVISION to 0.
#
#  5. If any interfaces have been added since the last public release,
#     then increment AGE.
#
#  6. If any interfaces have been removed or changed since the last
#     public release, then set AGE to 0. A changed interface means an
#     incompatibility with previous versions.

lib_LTLIBRARIES    = libiso9660++.la libcdio++.la

libcdiopp_la_CURRENT = 0
libcdiopp_la_REVISION = 2
libcdiopp_la_AGE = 0

libcdiopp_sources = cdio.cpp devices.cpp

libcdio___la_SOURCES = $(libcdiopp_sources)
libcdio___la_LDFLAGS = -version-info $(libcdiopp_la_CURRENT):$(libcdiopp_la_REVISION):$(libcdiopp_la_AGE) @LT_NO_UNDEFINED@
libcdio___la_LIBADD = $(top_builddir)/lib/driver/libcdio.la

libiso9660pp_la_CURRENT = 0
libiso9660pp_la_REVISION = 0
libiso9660pp_la_AGE = 0

libiso9660pp_sources = iso9660.cpp

libiso9660___la_SOURCES = $(libiso9660pp_sources)
libiso9660___la_LIBADD  = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS)
libiso9660___la_LDFLAGS = -version-info $(libiso9660pp_la_CURRENT):$(libiso9660pp_la_REVISION):$(libiso9660pp_la_AGE) @LT_NO_UNDEFINED@

AM_CPPFLAGS = -I$(top_srcdir)/include/ -I$(top_builddir)/include
