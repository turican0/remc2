#!/bin/sh
#$Id: check_fuzzyiso.sh,v 1.6 2008/03/20 03:45:43 edsdead Exp $

if test "X$srcdir" = "X" ; then
  srcdir=`pwd`
fi

if test "X$top_srcdir" = "X" ; then
  top_srcdir=`pwd`/..
fi

if test "X$top_builddir" = "X" ; then
  top_builddir=`pwd`/..
fi

check_program="$top_builddir/example/isofuzzy"

if test ! -x $check_program ; then
  exit 77
fi

cd $srcdir; src_dir=`pwd`
for file in $src_dir/data/*.bin $src_dir/data/*.iso $src_dir/data/*.nrg ; do 
  case "$file" in
  $src_dir/data/p1.nrg | $src_dir/data/p1.bin | $src_dir/data/cdda.bin | $src_dir/data/cdda-mcn.nrg | $src_dir/data/svcdgs.nrg | $src_dir/data/cdtext.bin | $src_dir/data/udf102.iso )
    good_rc=1
    ;;
  *) 
    good_rc=0
    ;;
  esac
  $check_program $file
  if test $? -ne $good_rc ; then 
    echo "$0: failed running:"
    echo "	$check_program $file"
    exit 1
  fi
done
exit 0

#;;; Local Variables: ***
#;;; mode:shell-script ***
#;;; eval: (sh-set-shell "bash") ***
#;;; End: ***
