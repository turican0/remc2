import os
import re, sys, json
# strings of 20FB0 (help pushes, PAUSED, formats) and the credits table off_9AB44 for CARPET and HIDDEN
def load(path):
    lines = open(path, encoding='utf-8', errors='replace').read().split('\n')
    labels = {}
    for i, l in enumerate(lines):
        m = re.match(r'^(\w+)\s+(db|dd|dw)\s', l)
        if m:
            labels[m.group(1)] = i
    return lines, labels

def parse_db_items(s):
    # items of a db line: 'text', numbers (0Dh, 0, 20h)
    s = s.split(';')[0] if "'" not in s.split(';')[0] or s.count("'") % 2 == 0 else s
    items = []
    i = 0
    while i < len(s):
        c = s[i]
        if c == "'":
            j = i + 1
            buf = ''
            while True:
                if s[j] == "'" and j + 1 < len(s) and s[j + 1] == "'":
                    buf += "'"; j += 2; continue
                if s[j] == "'":
                    break
                buf += s[j]; j += 1
            items.append(buf.encode('latin-1', 'replace'))
            i = j + 1
        elif c == ';':
            break
        elif c.isalnum():
            j = i
            while j < len(s) and (s[j].isalnum()):
                j += 1
            tok = s[i:j]
            if tok.lower() == 'dup':
                raise ValueError('dup')
            v = int(tok[:-1], 16) if tok.endswith('h') else int(tok)
            items.append(bytes([v]))
            i = j
        else:
            i += 1
    return items

def string_at(lines, labels, label, off):
    i = labels[label]
    data = b''
    first = True
    while True:
        l = lines[i]
        m = re.match(r'^(\w+)?\s+db\s+(.*)$', l)
        if not m or (m.group(1) and not first):
            break
        first = False
        for it in parse_db_items(m.group(2)):
            data += it
        if b'\0' in data[off:]:
            break
        i += 1
    data = data[off:]
    return data[:data.index(b'\0')]

def resolve(lines, labels, ref):
    m = re.match(r'(\w+)(?:\+(\w+))?', ref)
    off = 0
    if m.group(2):
        t = m.group(2)
        off = int(t[:-1], 16) if t.endswith('h') else int(t)
    return string_at(lines, labels, m.group(1), off)

out = {}
for name in ['CARPET', 'HIDDEN']:
    lines, labels = load(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'asm', '%s.EXE.asm' % name))
    # pushes inside sub_20FB0
    start = next(i for i, l in enumerate(lines) if l.startswith('sub_20FB0 ') and 'proc' in l)
    end = next(i for i in range(start, len(lines)) if lines[i].startswith('sub_20FB0 ') and 'endp' in lines[i])
    pushes = []
    for l in lines[start:end]:
        m = re.search(r'push\s+\(?offset (\w+(?:\+\w+)?)\)?', l)
        if m:
            pushes.append(resolve(lines, labels, m.group(1)).decode('latin-1'))
    # credits
    i = labels['off_9AB44']
    credits = []
    while True:
        m = re.match(r'^(?:off_9AB44)?\s+dd offset (\w+(?:\+\w+)?)', lines[i])
        if not m:
            if lines[i].strip().startswith(';') or lines[i].strip() == '':
                i += 1; continue
            break
        s = resolve(lines, labels, m.group(1)).decode('latin-1')
        credits.append(s)
        i += 1
        if s.startswith('#'):
            break
    out[name] = {'pushes': pushes, 'credits': credits}
json.dump(out, open(sys.argv[1], 'w', encoding='utf-8'), indent=1, ensure_ascii=False)
for k in out:
    print(k, len(out[k]['pushes']), len(out[k]['credits']))
print('same pushes', out['CARPET']['pushes'] == out['HIDDEN']['pushes'], 'same credits', out['CARPET']['credits'] == out['HIDDEN']['credits'])
