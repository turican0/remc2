#   Copyright (C) 2003-2006, 2008-2009, 2012-2013, 2017
#   Rocky Bernstein <rocky@gnu.org>
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
# Sample programs
####################################################
#
if ENABLE_CPP
SUBDIRS =  C++
endif

noinst_PROGRAMS = \
	audio \
	cdchange \
	cdio-eject \
	cdtext \
	cdtext-raw \
	device \
	discid \
	drives \
	eject \
	extract \
	isofile \
	isorr \
	isofile2 \
	isofuzzy \
	isolist \
	isolsn \
	logging \
	mmc1 \
	mmc2 \
	mmc2a \
	mmc3 \
	sample3 \
	sample4 \
	tracks \
	udf1 \
	udffile

check_PROGRAMS  = device drives isofile2 \
                   mmc1 mmc2 mmc2a mmc3 sample4

#: run regression tests. "test" is the same thing as "check"
test: check-am

#: Run all tests without bloated output
check-short:
	$(MAKE) check 2>&1  | ruby @abs_top_srcdir@/make-check-filter.rb

#: run valgrind on C programs
check-leaks: $(check_PROGRAMS)
	for p in $(check_PROGRAMS); do \
	   valgrind ./$$p; \
        done

AM_CPPFLAGS = $(LIBCDIO_CFLAGS)

audio_DEPENDENCIES    = $(LIBCDIO_DEPS)
audio_LDADD           = $(LIBCDIO_LIBS) $(LTLIBICONV)

cdchange_DEPENDENCIES = $(LIBCDIO_DEPS)
cdchange_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

cdio_eject_DEPENDENCIES = $(LIBCDIO_DEPS)
cdio_eject_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

cdtext_DEPENDENCIES   = $(LIBCDIO_DEPS)
cdtext_LDADD          = $(LIBCDIO_LIBS) $(LTLIBICONV)

cdtext_raw_DEPENDENCIES = $(LIBCDIO_DEPS)
cdtext_raw_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

device_DEPENDENCIES   = $(LIBCDIO_DEPS)
device_LDADD          = $(LIBCDIO_LIBS) $(LTLIBICONV)

discid_DEPENDENCIES   = $(LIBCDIO_DEPS)
discid_LDADD          = $(LIBCDIO_LIBS) $(LTLIBICONV)

drives_DEPENDENCIES   = $(LIBCDIO_DEPS)
drives_LDADD          = $(LIBCDIO_LIBS) $(LTLIBICONV)

eject_DEPENDENCIES    = $(LIBCDIO_DEPS)
eject_LDADD           = $(LIBCDIO_LIBS) $(LTLIBICONV)

extract_DEPENDENCIES  = $(LIBISO9660_LIBS) $(LIBUDF_LIBS) $(LIBCDIO_DEPS)
extract_LDADD         = $(LIBISO9660_LIBS) $(LIBUDF_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

isorr_LDADD    = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

isofile_LDADD    = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
isofile2_LDADD   = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
isofuzzy_LDADD   = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
isolist_LDADD    = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)
isolsn_LDADD     = $(LIBISO9660_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

logging_DEPENDENCIES = $(LIBCDIO_DEPS)
logging_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

mmc1_DEPENDENCIES = $(LIBCDIO_DEPS)
mmc1_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)
mmc2_DEPENDENCIES = $(LIBCDIO_DEPS)
mmc2_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

mmc2a_DEPENDENCIES = $(LIBCDIO_DEPS)
mmc2a_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

mmc3_DEPENDENCIES = $(LIBCDIO_DEPS)
mmc3_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

sample3_DEPENDENCIES = $(LIBCDIO_DEPS)
sample3_LDADD = $(LIBCDIO_LIBS) $(LTLIBICONV)

sample4_DEPENDENCIES = $(LIBCDIO_DEPS)
sample4_LDADD = $(LIBCDIO_LIBS) $(LTLIBICONV)

tracks_DEPENDENCIES = $(LIBCDIO_DEPS)
tracks_LDADD        = $(LIBCDIO_LIBS) $(LTLIBICONV)

udf1_DEPENDENCIES = $(LIBUDF_LIBS) $(LIBCDIO_DEPS)
udf1_LDADD        = $(LIBUDF_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

udffile_DEPENDENCIES = $(LIBUDF_LIBS) $(LIBCDIO_DEPS)
udffile_LDADD        = $(LIBUDF_LIBS) $(LIBCDIO_LIBS) $(LTLIBICONV)

TESTS               = $(check_PROGRAMS)

MOSTLYCLEANFILES = copying *.wav *.log *.trs *.orig *.rej *.dSYM

mostlyclean-generic:
	-test -z "$(MOSTLYCLEANFILES)" || rm -fr $(MOSTLYCLEANFILES)
	-test -z "$(TEST_LOGS)" || rm -f $(TEST_LOGS)
	-test -z "$(TEST_LOGS:.log=.trs)" || rm -f $(TEST_LOGS:.log=.trs)
	-test -z "$(TEST_SUITE_LOG)" || rm -f $(TEST_SUITE_LOG)
