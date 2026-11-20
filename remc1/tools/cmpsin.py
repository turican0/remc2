import re, struct, sys
# cmpsin.py <dump of 90B4C, 2800h bytes>: the MC1 sin table vs remc2 Maths::sin_DB750
src = open('C:/prenos/remc2-dev2/remc2/remc2/utilities/Maths.cpp', encoding='utf-8').read()
body = src[src.index('Maths::sin_DB750[2560]'):]
body = body[body.index('{') + 1:body.index('};')]
body = re.sub(r'//[^\n]*', '', body)
vals = [int(t, 0) for t in re.findall(r'-?(?:0x[0-9A-Fa-f]+|\d+)', body)]
d = open(sys.argv[1], 'rb').read()
dos = list(struct.unpack('<2560i', d))
print(len(vals), sum(1 for a, b in zip(vals, dos) if (a & 0xFFFFFFFF) != (b & 0xFFFFFFFF)))
