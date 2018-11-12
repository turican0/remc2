#!/usr/bin/env ruby
# Use this to cut out the crud from make check.
# Use like this:
#   make check 2>&1  | ruby ../make-check-filter.rb
# See Makefile.am
pats = '(' + 
  ["^  CC",
   "^  CXX",
   '^(re|g)?make\[',
   "^(re|g)?make ",
   "Making check in",
   '^[+]{2} WARN: ',
   '^m4/',              # doesn't work always
   '^configure.ac',     # doesn't work always
   '^ cd \.\.',         # doesn't work always
   '^config.status',    # doesn't work always
   "^	  vcd_demo.right",
   '^-- ',
   '^-+$',
   '^##<<+$',
   '^##>>+$',
   '`.+\' is up to date.$',
   '^\s*$',
  ].join('|') + ')'
# puts pats
skip_re = /#{pats}/

while gets()
  next if $_ =~ skip_re
  puts $_
end
