#!/bin/sh
#   Copyright (C) 2003, 2005, 2008, 2010 Rocky Bernstein <rocky@gnu.org>
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
#
# Tests to see that CD reading is correct (via cd-read). 

if test -z $srcdir ; then
  srcdir=`pwd`
fi

if test "X$top_builddir" = "X" ; then
  top_builddir=`pwd`/..
fi

. ${top_builddir}/test/check_common_fn

if test ! -x ../src/cd-read ; then
  exit 77
fi

BASE=`basename $0 .sh`

fname=cdda
testnum=CD-DA
opts="-c ${srcdir}/data/${fname}.cue --mode=red --just-hex --start=0"
test_cd_read  "$opts" ${fname}-read.dump ${srcdir}/${fname}-read.right
RC=$?
check_result $RC "cd-read CUE test $testnum" "cd-read $opts"

fname=isofs-m1
testnum=MODE1
opts="-i ${srcdir}/data/${fname}.cue --mode m1f1 -s 26 -n 2"
test_cd_read "$opts" ${fname}-read.dump ${srcdir}/${fname}-read.right
RC=$?
check_result $RC "cd-read CUE test $testnum" "$CD_READ $opts"

exit $RC

#;;; Local Variables: ***
#;;; mode:shell-script ***
#;;; eval: (sh-set-shell "bash") ***
#;;; End: ***
