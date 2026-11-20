#!/bin/bash
# dumpf.sh <asm> <func...>: function bodies without comments, alignment and the no-op fillers (lea r,[r+0] / mov r,r of the same register)
A=$1; shift
for f in "$@"; do
  n=$(grep -n "^sub_$f *proc" $A | cut -d: -f1); e=$(grep -n "^sub_$f *endp" $A | cut -d: -f1)
  [ -z "$n" ] && { echo "?? $f"; continue; }
  echo "; ==== $f ($((e-n)))"
  sed -n "${n},${e}p" $A | grep -v "^ *;\|^$\|align" | sed 's/ *;.*XREF.*//' | grep -Pv "^\s+lea\s+(e[a-z]{2}), \[\1\+0\]\s*$|^\s+mov\s+(e[a-z]{2}), \2\s*$" | cut -c1-90
done
