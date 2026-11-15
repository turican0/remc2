"""asmload.py: functions of an IDA asm listing as lists of normalized instructions.

Addresses, labels, jump tables and data names are replaced; references into byte data declared with db
(strings, merged strings with an offset) are replaced by the bytes from that offset up to the terminating 0,
so equal strings compare equal whatever label IDA gave them.
"""
import re

_CACHE = {}
_LABEL = r'(?:a[A-Z0-9]\w*|(?:byte|unk|asc|stru)_[0-9A-F]+)'


def _parse_db(text):
    out = bytearray()
    for item in re.findall(r"'[^']*'|\"[^\"]*\"|[0-9A-Fa-f]+h?|\d+", text):
        if item[0] in '\'"':
            out += item[1:-1].encode('latin-1', errors='replace')
        else:
            try:
                out.append(int(item[:-1], 16) & 0xFF if item.endswith('h') else int(item) & 0xFF)
            except ValueError:
                pass
    return bytes(out)


def _bytes(lines):
    out = {}
    cur = None
    for l in lines:
        m = re.match(r'^(' + _LABEL + r')\s+db\s+(.*)$', l)
        if m:
            cur = m.group(1)
            out[cur] = _parse_db(m.group(2).split(';')[0])
            continue
        m = re.match(r'^\s+db\s+(.*)$', l)
        if m and cur:
            out[cur] += _parse_db(m.group(1).split(';')[0])
            continue
        cur = None
    return out


def load(path):
    if path in _CACHE:
        return _CACHE[path]
    lines = open(path, encoding='utf-8', errors='replace').read().split('\n')
    data = _bytes(lines)
    funcs = []
    cur = None

    def string_text(m):
        name, off = m.group(1), int(m.group(2) or 0)
        if name not in data:
            return 'X' + (m.group(2) and '+' + m.group(2) or '')
        b = data[name][off:]
        b = b[:b.index(0)] if 0 in b else b[:32]
        return 'S[%s]' % b.decode('latin-1')

    for l in lines:
        m = re.match(r'^(\S+)\s+proc\s', l)
        if m:
            cur = [m.group(1), []]
            continue
        if cur and re.match(r'^' + re.escape(cur[0]) + r'\s+endp', l):
            funcs.append(cur)
            cur = None
            continue
        if not cur:
            continue
        code = l.split(';')[0].strip()
        if not code or code.endswith(':') or re.match(r'^\w+\s*=\s', code) or code.startswith(('align', 'assume')):
            continue
        if re.match(r'^(loc|locret|def)_[0-9A-F]+:', code):
            continue
        code = re.sub(r'\b(' + _LABEL + r')(?:\+(\d+))?\b', string_text, code)
        code = re.sub(r'\b(sub|loc|locret|unk|byte|word|dword|off|asc|stru|def|qword|jpt|flt|dbl|tbyte|j_\w+)_[0-9A-F]+\b',
                      'X', code)
        code = re.sub(r'^(jpt|def)\w* ', 'X ', code)
        code = re.sub(r'\b[0-9A-F]{5,}h\b', 'N', code)
        code = re.sub(r'\s+', ' ', code)
        cur[1].append(code)
    _CACHE[path] = funcs
    return funcs
