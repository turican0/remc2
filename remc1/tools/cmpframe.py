import struct, sys, os
# cmpframe.py <dir> <mc1|hw>: the port dumps of a frame (port_*.bin) vs DOSBox (<variant>_*.bin)
d, v = sys.argv[1], sys.argv[2]
def rd(n):
    return open(os.path.join(d, n), 'rb').read()
for m in ['terrain', 'height', 'shading', 'angle', 'index']:
    a, b = rd('port_%s.bin' % m), rd('%s_%s.bin' % (v, m))
    diff = [i for i in range(len(a)) if a[i] != b[i]]
    print('%-8s %d differences' % (m, len(diff)), ['%04X:%02X/%02X' % (i, a[i], b[i]) for i in diff[:8]])
stateLin = struct.unpack('<I', rd('%s_stateptr.bin' % v))[0]
base = stateLin + 0x7463
types = 0x98F38 + 0x1BE000
dummy = (0xB7330 if v == 'mc1' else 0xB7320) + 0x1BE000
pe, de = rd('port_entities.bin'), rd('%s_entities.bin' % v)
fields = {0x00: 'next', 0x04: 'rand', 0x08: 'maxLife', 0x0C: 'life', 0x10: 'flags', 0x14: 'mapNext', 0x16: 'mapPrev', 0x18: 'id',
          0x1A: 'w1A', 0x1C: 'mailMask', 0x1E: 'w1E', 0x20: 'w20', 0x2A: 'w2A', 0x2C: 'w2C', 0x2E: 'w2E', 0x30: 'w30', 0x32: 'w32',
          0x3A: 'b3A', 0x3F: 'index', 0x40: 'class', 0x41: 'model', 0x46: 'action', 0x47: 'b47', 0x48: 'pos', 0x4E: 'box',
          0x56: 'sprite', 0x58: 'frame', 0x5A: 'mails', 0x7E: 'w7E', 0x80: 'w80', 0x84: 'd84', 0x88: 'd88', 0x8C: 'mana',
          0x90: 'owner', 0x96: 'target', 0x9C: 'typeParams', 0xA0: 'playerData'}
def fname(o):
    k = max(x for x in fields if x <= o)
    return '%s+%X' % (fields[k], o - k) if o != k else fields[k]
used = bad = shown = 0
for i in range(1000):
    p = bytearray(pe[i * 0xA4:(i + 1) * 0xA4])
    q = bytearray(de[i * 0xA4:(i + 1) * 0xA4])
    nxt, tp, pd = struct.unpack_from('<I', q, 0)[0], struct.unpack_from('<I', q, 0x9C)[0], struct.unpack_from('<I', q, 0xA0)[0]
    struct.pack_into('<I', q, 0, 0xFFFFFFFF if nxt == 0 else (nxt - base) // 0xA4)
    struct.pack_into('<I', q, 0x9C, 0xFFFFFFFF if tp == 0 else tp - types)
    if pd == 0:
        pdv = 0xFFFFFFFF
    elif dummy <= pd < dummy + 0x3C0:
        pdv = pd - dummy
    else:
        pdv = 0x80000000 | (pd - stateLin)
    struct.pack_into('<I', q, 0xA0, pdv)
    if q[0x40]:
        used += 1
    if p != q:
        bad += 1
        if shown < 15:
            shown += 1
            offs = [o for o in range(0xA4) if p[o] != q[o]]
            print('entity %d class %d/%d model %d/%d:' % (i, p[0x40], q[0x40], p[0x41], q[0x41]),
                  ', '.join('%s %02X/%02X' % (fname(o), p[o], q[o]) for o in offs[:12]))
print('entities used (DOS) %d, different %d' % (used, bad))
# the players (2409h..7463h: the network player, the backup, 8 players, the inputs)
a, b = rd('port_players.bin'), rd('%s_players.bin' % v)
diff = [i for i in range(len(a)) if a[i] != b[i]]
def where(i):
    o = i + 0x2409
    if o >= 0x7413:
        return 'input%d+%X' % ((o - 0x7413) // 10, (o - 0x7413) % 10)
    n = (o - 0x2409) // 0x801
    name = ['net', 'backup'] + ['player%d' % j for j in range(8)]
    return '%s+%X' % (name[n], (o - 0x2409) % 0x801)
print('players  %d differences' % len(diff), ['%s:%02X/%02X' % (where(i), a[i], b[i]) for i in diff[:16]])
# the settings: flags, frames, colorIndex 5Dh, speedIndex 96h; reuseTop
ps = rd('port_settings.bin')
ds = rd('%s_settings.bin' % v)
dosSet = ds[0:4] + ds[4:8] + ds[0x5D:0x6D] + bytes([ds[0x96]]) + b'\0\0\0' + rd('%s_top2.bin' % v)
names = ['flags'] * 4 + ['frames'] * 4 + ['colorIndex'] * 16 + ['speed', '', '', ''] + ['reuseTop'] * 4
diff = [i for i in range(0x20) if ps[i] != dosSet[i]]
print('settings %d differences' % len(diff), ['%s+%d:%02X/%02X' % (names[i], i, ps[i], dosSet[i]) for i in diff])
