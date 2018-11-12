#   Copyright (C) 2003, 2004, 2008, 2012 Rocky Bernstein <rocky@gnu.org>
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

EXTRA_DIST = doxygen/Doxyfile.in doxygen/run_doxygen
info_TEXINFOS = libcdio.texi 
libcdio_TEXINFOS = fdl.texi glossary.texi

reference:
	-( cd ${top_srcdir} && $(MAKE) doxygen )

#: Create documentation in PDF format
pdf: libcdio.pdf cd-text-foramt.pdf

#: Create documentation as a text file
txt: libcdio.txt

#: Create documentation in PostScript format
ps: libcdio.ps cd-text-format.ps

#: Create documentation in HTML format
html: libcdio.html cd-text-format.html

%.ps.gz: %.ps
	gzip -9c $< > $@

.texi.pdf: stamp-vti
	texi2pdf $<

.texi.html: stamp-vti
	texi2html $<

.texi.txt: 
	makeinfo --no-headers $< > $@

# Create documentation in all formats, e.g. PDF, DVI, plain text and HTML
all-formats: pdf dvi txt ps html

MOSTLYCLEANFILES = \
	libcdio.html libcdio.pdf libcdio.ps.gz \
	cd-text-foramt.html cd-text-format.pdf cd-text-foramt.ps.gz
