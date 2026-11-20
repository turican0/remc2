import os
import re, sys
# mkgcc.py <out.sh> <file>...: gcc_render.sh with another list of files (relative to remc2/remc2)
S = os.path.dirname(os.path.abspath(__file__)) + '/'
s = open(S + 'gcc_render.sh', encoding='utf-8').read()
files = ' '.join('"%s"' % f for f in sys.argv[2:])
s = re.sub(r'printf "%s\\n" .*? \| xargs', lambda m: 'printf "%s\\n" ' + files + ' | xargs', s)
open(S + sys.argv[1], 'w', encoding='utf-8', newline='\n').write(s)
