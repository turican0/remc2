import os
import re
S = os.path.dirname(os.path.abspath(__file__)) + '/'
d = open(S + 'scantab/mc1_tab.bin', 'rb').read()
src = open(r'C:/prenos/remc2-dev2/remc2/remc1/engine/Mc1Input.cpp', encoding='utf-8').read()
body = src[src.index('scanCodeChars_9AD0C_9AD0C[128] = {'):]
body = body[body.index('{') + 1:body.index('};')]
vals = []
for t in re.findall(r"0x[0-9A-Fa-f]+|'\\\\'|'.'", body):
    if t.startswith('0x'):
        vals.append(int(t, 16))
    elif t == "'\\\\'":
        vals.append(ord('\\'))
    else:
        vals.append(ord(t[1]))
print(len(vals))
print([(i, hex(d[i]), hex(vals[i])) for i in range(128) if d[i] != vals[i]])
