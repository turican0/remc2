"""unported.py <calls.txt> <root>: unported asm lines under root (ported = defined in remc1 or mapped in Mc1Shared.h)."""
import glob
import re
import sys

ported = set()
for p in glob.glob(r'C:/prenos/remc2-dev2/remc2/remc1/engine/*.cpp') + glob.glob(r'C:/prenos/remc2-dev2/remc2/remc1/*.cpp'):
    if 'NotPorted' in p:
        continue
    for m in re.finditer(r'^[A-Za-z][^\n;]*?_([0-9A-F]{5})_[0-9A-F]{5}\(', open(p, encoding='utf-8', errors='replace').read(), re.M):
        ported.add('sub_' + m.group(1))
for m in re.finditer(r'_([0-9A-F]{5})_[0-9A-F]{5} = ', open(r'C:/prenos/remc2-dev2/remc2/remc1/engine/Mc1Shared.h', encoding='utf-8').read()):
    ported.add('sub_' + m.group(1))

info = {}
cur = None
for l in open(sys.argv[1], encoding='utf-8'):
    m = re.match(r'^(\S+)\s+(\d+) lines', l)
    if m:
        cur = m.group(1)
        info[cur] = [int(m.group(2)), []]
    elif cur and l.strip().startswith('calls:'):
        info[cur][1] = l.split(':', 1)[1].split()

seen = set()
todo = []


def walk(f):
    if f in seen or f not in info:
        return
    seen.add(f)
    if f in ported and f != sys.argv[2]:
        return
    todo.append((f, info[f][0]))
    for c in info[f][1]:
        walk(c)


walk(sys.argv[2])
print(sys.argv[2], 'unported functions', len(todo), 'lines', sum(n for _, n in todo))
for f, n in sorted(todo, key=lambda x: -x[1])[:int(sys.argv[3]) if len(sys.argv) > 3 else 20]:
    print('  ', f, n)
