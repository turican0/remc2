#!/usr/bin/env perl
# Use this to cut out the crud from make check.
# Use like this:
#   make check 2>&1  | ruby ../make-check-filter.pl
# See Makefile.am
my @pats =
  ("^  CC",
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
   '^\s*$');
my $pats = "(" . join("|", @pats) . ')';
# print "$pats\n";

my $skip_re = /${pats}/;

# open(STDIN, '<', '/etc/hosts')
#  or die "cannot open > /etc/hosts: $!";
while (<STDIN>) {
  next if $_ =~ /$pats/;
  print $_;
}
