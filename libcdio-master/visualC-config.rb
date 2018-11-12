#!/usr/bin/env ruby
# Alternative configuration for Visual C written in Ruby.


ABS_TOP_SRCDIR = File.dirname(__FILE__)

# Text substitutions performed on files
SUBST = {

  # Put in what you want for the build string.
  :build => ARGV[0] || 'pc-windows-visualstudio',

  :abs_top_srcdir => ABS_TOP_SRCDIR.dup,
  :abs_top_builddir => ABS_TOP_SRCDIR.dup,
  :native_abs_top_srcdir => File.expand_path(ABS_TOP_SRCDIR),
  :LIBCDIO_SOURCE_PATH => File.expand_path(ABS_TOP_SRCDIR)
}

# Get substitution information from configure.ac
def extract_from_configure_ac
  config_file = File.dirname(__FILE__) + '/configure.ac'
  lines = File.readlines(config_file)
  relnum_regexp = Regexp.new('^define\(RELEASE_NUM, (\d+)')
  relnum_line = lines.grep(relnum_regexp)
  if relnum_line.size != 1
    if relnum.size > 1
      STDERR.puts("Multiple define(RELEASE_NUM, ...) in #{config_file}")
    else
      STDERR.puts("Can't find define(RELEASE_NUM, ...) in #{config_file}")
    end
    exit 1
  end
  if relnum_line[0] !~ relnum_regexp
    STDERR.puts("Something went wrong in matching release number in #{config_file}")
    exit 2
  end
  SUBST[:LIBCDIO_VERSION_NUM] = $1
  SUBST[:VERSION] = "0.#{$1}"

  ac_init_regexp = Regexp.new('^AC_INIT\(\[(.+?)\]')
  ac_init_line = lines.grep(ac_init_regexp)
  if ac_init_line.size != 1
    if ac_init_line.size > 1
      STDERR.puts("Multiple AC_INIT in #{config_file}")
    else
      STDERR.puts("Can't find AC_INIT([...],) in #{config_file}")
    end
    exit 2
  end
  if ac_init_line[0] !~ ac_init_regexp
    STDERR.puts("Something went wrong in getting package name #{config_file}")
    exit 3
  end
  SUBST[:PACKAGE] = $1
end

# Write #{filename} from #{filename}.in and SUBST
def perform_substitutions(filename)

  builddir = SUBST[:abs_top_builddir]
  version_file = builddir + '/' + filename
  version_file_template = version_file + '.in'

  text = File.read(version_file_template)
  SUBST.each do |key, val|
    from_str = "@#{key.to_s}@"
    text.gsub!(from_str, val)
  end
  File.open(version_file, 'w') { |f| f.write(text) }
end

extract_from_configure_ac
%w(include/cdio/version.h
   test/testgetdevices.c test/testisocd2.c   		test/testisocd_joliet.c
   test/driver/bincue.c  test/driver/track.c		test/testisocd_joliet.c
   test/driver/cdrdao.c  test/testgetdevices.c	test/testpregap.c
   test/driver/nrg.c	 test/testisocd2.c
   test/testpregap.c     doc/doxygen/Doxyfile
).each do |file|
  perform_substitutions(file)
end
