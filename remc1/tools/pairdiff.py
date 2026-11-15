import os
"""pairdiff.py <mc1 fn> <mc2 fn> [show]: MC1 (CARPET) vs MC2 (NETHERW) asm, globals / labels / frames normalized."""
import difflib
import re
import sys

C = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'asm', 'CARPET.EXE.asm')
M = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'asm', 'NETHERW.asm')
cache = {}


def lines(path):
    if path not in cache:
        cache[path] = open(path, encoding='utf-8', errors='replace').read().split('\n')
    return cache[path]


def ext(path, fn):
    ls = lines(path)
    start = next(i for i, l in enumerate(ls) if re.match(r'^%s\s+proc' % fn, l))
    end = next(i for i in range(start, len(ls)) if re.match(r'^%s\s+endp' % fn, ls[i]))
    out = []
    for l in ls[start + 1:end]:
        l = l.split(';')[0].rstrip()
        if not l.strip() or re.match(r'^\s*(var|arg)_\w+\s*=', l):
            continue
        if re.match(r'^\w+:', l.strip()):
            out.append('L:')
            continue
        l = re.sub(r'\b(dword|word|byte|unk|off|stru|asc)_[0-9A-F]+\b', 'G', l)
        l = re.sub(r'\bloc_[0-9A-F]+\b', 'L', l)
        l = re.sub(r'\b(sub|def|jpt|locret)_[0-9A-F]+\b', 'S', l)
        l = re.sub(r'\[(esp|ebp)[^\]]*\]', '[F]', l)
        l = re.sub(r'\s+', ' ', l).strip()
        if l in ('push ebp', 'mov ebp, esp', 'pop ebp', 'mov esp, ebp') or l.startswith('lea eax, [eax+0]') or l.startswith('lea edx, [edx+0]') or l == 'mov eax, eax' or l == 'nop':
            continue
        out.append(l)
    return out


a, b = ext(C, sys.argv[1]), ext(M, sys.argv[2])
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
changed = sum(max(i2 - i1, j2 - j1) for op, i1, i2, j1, j2 in sm.get_opcodes() if op != 'equal')
print('%s %s: %d / %d lines, %d differ' % (sys.argv[1], sys.argv[2], len(a), len(b), changed))
if len(sys.argv) > 3:
    for l in difflib.unified_diff(a, b, lineterm='', n=1):
        print(l)
