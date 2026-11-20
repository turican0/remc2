#!/bin/bash
# enttest.sh <level>: the entities at 20FB0 of the first frame (the fields the sprites read), DOSBox vs port
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
L=$1
D=$S/ent$L; rm -rf $D; mkdir -p $D
WD="$W\\ent$L"
cd $S
DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 20FB0 "$WD" stateptr=AE400:4 > /dev/null
base=$(python -c "import struct,sys;print(struct.unpack('<I',open(sys.argv[1],'rb').read())[0])" "$D/mc1_stateptr.bin")
DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 20FB0 "$WD" entities=@$(printf "%X" $((base + 0x7463))):280A0 > /dev/null
cd /c/prenos/remc2-dev2/remc2/x64/Release
MC1_TEMPENT="$WD" REMC2_SKIPINTRO=1 timeout 60 ./remc2.exe -game mc1 -custom -level $L > $D/port.log 2>&1
python - $D <<'PYEOF'
import struct, sys
d = sys.argv[1]
a = open(d + '/mc1_entities.bin', 'rb').read()
b = open(d + '/port_entities.bin', 'rb').read()
fields = [(0x10, '<I', 'flags&21', 0x21), (0x14, '<H', 'mapNext', None), (0x1E, '<h', 'yaw', None), (0x48, '<3h', 'pos', None),
          (0x56, '<h', 'sprite', None), (0x58, '<B', 'frame', None)]
bad = {}
shown = 0
for i in range(1000):
    ea, eb = a[i * 0xA4:(i + 1) * 0xA4], b[i * 0x98:(i + 1) * 0x98]
    if not ea[0x40]:
        continue
    for off, fmt, name, mask in fields:
        va = struct.unpack_from(fmt, ea, off)
        vb = struct.unpack_from(fmt, eb, off - 4)
        if mask:
            va, vb = (va[0] & mask,), (vb[0] & mask,)
        if va != vb:
            bad[name] = bad.get(name, 0) + 1
            if shown < 12:
                print('entity', i, 'class', ea[0x40], 'model', ea[0x41], name, va, vb)
                shown += 1
print('differences by field', bad)
PYEOF
