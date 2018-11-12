#!/usr/bin/env ruby
comment_buffer = []
KEEP_LIST = %w(HAVE_CDDB HAVE_JOLIET HAVE_DARWIN_CDROM HAVE_BSDI_CDROM)

def handle_define_undefine(keyword, line, comment_buffer) 
  if KEEP_LIST.member?(keyword)
    puts 
    comment_buffer.each {|line| print line}
    print line
  end
  comment_buffer = []
end
  
in_comment = false
File.open("/tmp/config.h").readlines.each do |line|
  if match = line.match('^#define ([A-Z_]+)')
    handle_define_undefine(match[1], line, comment_buffer)
    in_comment = false
  elsif match = line.match('^/[*] #undef ([A-Z_]+)')
    handle_define_undefine(match[1], line, comment_buffer)
    in_comment = false
  elsif keyword = line.match('^/[*]')
    comment_buffer.push(line)
    in_comment = not(line.match('\*/$'))
  elsif in_comment
    comment_buffer.push(line)
    in_comment = not(line.match('\*/$'))
  else
    comment_buffer = []
  end
end
