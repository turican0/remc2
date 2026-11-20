#!/bin/bash
# keytest.sh <level> <hex scan code>...: the key in lastKey (12EF70) at the first 17C20, state after 16B00 (poke at 16B00, dump at 3FCA0: the first frame with an empty slot), DOSBox vs port
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
L=$1; shift
base=$((0x1F690)); sbase=$((0x583A0))   # MC1 level 3, player 0
for K in "$@"; do
	D=$S/kt$L-$K; rm -rf $D; mkdir -p $D
	WD="$W\\kt$L-$K"
	cd $S
	REMC1_POKE=$(printf "%X,%X,%02X" $((0x16B00 + 0x1D1000)) $((0x12EF70 + 0x1BE000)) $((0x$K))) DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 3FCA0 "$WD" \
		toggles=@$(printf "%X" $((base + 0x2195))):B message=@$(printf "%X" $((base + 0x340B + 0x1C))):44 input=@$(printf "%X" $((base + 0x7413))):A \
		flags=@$(printf "%X" $sbase):4 speed=@$(printf "%X" $((sbase + 0x96))):1 key=12EF70:1 > /dev/null
	cd /c/prenos/remc2-dev2/remc2/x64/Release
	MC1_TEMPKEY=$K MC1_TEMPKEYOUT="$WD" REMC2_SKIPINTRO=1 timeout 60 ./remc2.exe -game mc1 -custom -level $L > $D/port.log 2>&1
	python - $D $K <<'PYEOF'
import sys
d, k = sys.argv[1], sys.argv[2]
names = ['toggles', 'message', 'input', 'flags', 'speed', 'key']
out = []
for n in names:
    a = open(d + '/mc1_' + n + '.bin', 'rb').read()
    b = open(d + '/port_' + n + '.bin', 'rb').read()
    if n == 'input':  # the move of the mouse differs (the mouse is elsewhere)
        a = a[:3] + a[5:]; b = b[:3] + b[5:]
    if n == 'message':
        a = a[:a.index(b'\0') + 1] + a[0x40:]; b = b[:b.index(b'\0') + 1] + b[0x40:]
    out.append('%s %s' % (n, 'ok' if a == b else 'DIFF dos %s port %s' % (a.hex(), b.hex())))
print('key', k, '|', ', '.join(out))
PYEOF
done
