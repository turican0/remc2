#   Copyright (C) 2003-2006, 2008-2013 Rocky Bernstein <rocky@gnu.org>
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

####################################################
# Things for regression testing
####################################################
#
#
SUBDIRS = data driver

hack = check_sizeof testassert testgetdevices testischar \
       testisocd testisocd2 testisocd_joliet testiso9660 \
       testisorr test_lib_driver_util testudf \
       testpregap

DATA_DIR       = @abs_top_srcdir@/test/data

AM_CPPFLAGS = $(LIBCDIO_CFLAGS) $(LIBISO9660_CFLAGS)

check_sizeof_LDADD    = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
testassert_LDADD      = $(LIBCDIO_LIBS) $(LTLIBICONV)
testgetdevices_CFLAGS = -DDATA_DIR=\"$(DATA_DIR)\"
testgetdevices_LDADD  = $(LIBCDIO_LIBS) $(LTLIBICONV)
testischar_LDADD      = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
testiso9660_LDADD     = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

testisocd_LDADD       = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
testisocd2_LDADD      = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
testisocd_joliet_LDADD= $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
testisorr_LDADD       = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

testudf_LDADD         = $(LIBUDF_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

test_lib_driver_util_LDADD    = $(LIBCDIO_LIBS) $(LTLIBICONV)
test_lib_driver_util_CFLAGS   = -DDATA_DIR=\"$(DATA_DIR)\"

testpregap_LDADD    = $(LIBCDIO_LIBS) $(LTLIBICONV)
testpregap_CFLAGS   = -DDATA_DIR=\"$(DATA_DIR)\"

check_SCRIPTS = check_nrg.sh  check_cue.sh  check_cd_read.sh check_udf.sh \
                check_iso.sh  check_fuzzyiso.sh check_opts.sh \
		check_iso_read.sh

check_udf.sh: @abs_top_builddir@/example/extract$(EXEEXT)

@abs_top_builddir@/example/extract$(EXEEXT):
	$(MAKE) -C @abs_top_builddir@/example extract$(EXEEXT)

check_PROGRAMS = $(hack)

check_DATA = vcd_demo.right vcd_demo_vcdinfo.right \
	     videocd.right \
	     cdda.right \
	     isofs-m1.right isofs-m1-no-rr.right isofs-m1-test2.right \
	     check_opts0.right check_opts1.right check_opts2.right \
             check_opts3.right check_opts4.right check_opts5.right \
             check_opts6.right check_opts7.right \
	     isofs-m1-read.right cdda-read.right cdtext.right \
	     copying.right copying-rr.right \
	     joliet.right joliet-nojoliet.right \
	     malformed.right malformed2.right \
	     copying.gpl copying-rr.gpl copying-rr-mingw.right

EXTRA_DIST = $(check_SCRIPTS) $(check_DATA) \
	check_common_fn check_cue.sh.in check_nrg.sh.in \
	testpregap.c.in testisorr.c.in check_legal.regex \
        testgetdevices.c.in check_iso.sh.in testudf.c.in \
	check_iso_read.sh.in

TESTS = $(check_PROGRAMS) $(check_SCRIPTS)
XFAIL_TESTS = testassert

MOSTLYCLEANFILES = \
	core core.* *.dump cdda-orig.wav cdda-try.wav *.raw \
	*file?s-read.txt exampleIso*.iso*

#: run regression tests. "test" is the same thing as "check"
test: check-am

#: Run all tests without bloated output
check-short:
	@$(MAKE) check 2>&1  | ruby @abs_top_srcdir@/make-check-filter.rb

#: Run 5GB ISO 9660 read test
# Stripped down greatly from automakes check-TESTS
check-iso-read-large:
	@if ./check_iso_read.sh 5000; then \
	  echo test of 5M iso_read succeeded; \
	elif test $? -ne 77; then \
	  echo iso_read large file test failed; \
	else \
	  echo iso_read large file test skipped; \
	fi

#: Run 5GB ISO 9660 read test without some of the bloated output
check-iso-read-large-terse:
	@$(MAKE) check-iso-read-large 2>&1  | ruby @abs_top_srcdir@/make-check-filter.rb

# This is a really bad hack to make sure check_nrg and check_cue.sh
# are executable. Automake will remake check_nrg.sh and check_cue.sh
# but not run the configure default commands for them to make sure
# they are executable. You know it would be nice one could just set
# permissions and mode when it makes the files. I'm sure there's some
# cleaner a way to do this, but frankly I've wasted far too much of my
# life the crappy automess system that I've really lost interest in
# learning any more of this awful system than I need to.
check-am:	make-executable
make-executable: check_nrg.sh check_cue.sh
	@chmod +x *.sh
	@if test ! -f cdda.bin -a ! -s cdda.bin ; then \
		test -L cdda.bin && $(RM) cdda.bin ; \
		$(LN_S) $(abs_top_srcdir)/test/data/cdda.bin cdda.bin ; \
	fi
	@if test ! -f @abs_top_builddir@/test/data/isofs-m1.bin -a ! -s @abs_top_builddir@/test/data/isofs-m1.bin ; then \
		test -L isofs-m1.bin && $(RM) isofs-m1.bin ; \
		$(LN_S) $(abs_top_srcdir)/test/data/isofs-m1.bin @abs_top_builddir@/test/data/isofs-m1.bin ;  \
	fi
	@if test ! -f @abs_top_builddir@/test/data/isofs-m1.cue -a ! -s isofs-m1.cue -a ! -f @abs_top_builddir@/test/data/isofs-m1.cue; then \
		test -L isofs-m1.bin && $(RM) isofs-m1.cue ; \
		$(LN_S) $(abs_top_srcdir)/test/data/isofs-m1.cue @abs_top_builddir@/test/data/isofs-m1.cue ;  \
	fi

clean-local: clean-local-check
.PHONY: clean-local-check
clean-local-check:
	-rm -rf exampleIso*.iso.prep *.log *.trs *.orig *.rej
