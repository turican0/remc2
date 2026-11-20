#!/bin/bash
# sprtest.sh <level> <mode>: the first frame with the entities and the entity index map of DOSBox (at 20FB0) injected into the
# port before 20FB0: the screens after 20FB0 (the sprites), DOSBox vs port
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
L=$1; M=$2
D=$S/spr$L-$M; rm -rf $D; mkdir -p $D
WD="$W\\spr$L-$M"
cd $S
DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 20FB0 "$WD" stateptr=AE400:4 screenptr=12EFF4:4 palptr=AE428:4 index=10C1E0:20000 > /dev/null
rd() { python -c "import struct,sys;print(struct.unpack('<I',open(sys.argv[1],'rb').read())[0])" "$D/mc1_$1.bin"; }
base=$(rd stateptr); scr=$(rd screenptr); pal=$(rd palptr)
DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 20FB0 "$WD" entities=@$(printf "%X" $((base + 0x7463))):280A0 > /dev/null
REMC1_POKE=$(printf "%X,%X,%02X" $((0x20FB0 + 0x1D1000)) $((base + 0x3855)) $M) DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 5A560 "$WD" screen=@$(printf "%X" $scr):FA00 palette=@$(printf "%X" $pal):300 > /dev/null
cd /c/prenos/remc2-dev2/remc2/x64/Release
MC1_TEMPINJECT="$WD" MC1_TEMPMODE=$M MC1_TEMPFRAME="$WD" REMC2_SKIPINTRO=1 timeout 120 ./remc2.exe -game mc1 -custom -level $L > $D/port.log 2>&1
python $S/rawpng.py $D/mc1_screen.bin $D/mc1_palette.bin 320 200 $D/dosbox.png
python $S/rawpng.py $D/port_screen.bin $D/port_palette.bin 320 200 $D/port.png
python - $D <<'PYEOF'
import sys
d = sys.argv[1]
a = open(d + '/mc1_screen.bin', 'rb').read(); b = open(d + '/port_screen.bin', 'rb').read()[:64000]
diff = [i for i in range(64000) if a[i] != b[i]]
print('differ', len(diff))
if diff:
    ys = [i // 320 for i in diff]; xs = [i % 320 for i in diff]
    print('box x %d-%d y %d-%d' % (min(xs), max(xs), min(ys), max(ys)))
PYEOF
