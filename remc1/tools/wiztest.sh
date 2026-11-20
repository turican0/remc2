#!/bin/bash
# wiztest.sh <level> [frame] [key]: player 0 and its wizard after the world tick of the frame (DOSBox at 55100, the
# n-th time), the key (scan code hex) held from the first frame; DOSBox vs port
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
L=$1; F=${2:-1}; K=${3:-0}
D=$S/wiz$L-$F-$K; rm -rf $D; mkdir -p $D
WD="$W\\wiz$L-$F-$K"
cd $S
DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 55100 "$WD" stateptr=AE400:4 > /dev/null
base=$(python -c "import struct,sys;print(struct.unpack('<I',open(sys.argv[1],'rb').read())[0])" "$D/mc1_stateptr.bin")
POKE=""
[ $K != 0 ] && POKE=$(printf "%X,%X,01" $((0x17C20 + 0x1D1000)) $((0x12EEF0 + 0x$K + 0x1BE000)))
REMC1_POKE=$POKE REMC1_DUMP_HIT=$F DBXCONF="$W\\c$L.conf" python dbxdump.py mc1 55100 "$WD" player=@$(printf "%X" $((base + 0x340B))):801 entities=@$(printf "%X" $((base + 0x7463))):280A0 > /dev/null
cd /c/prenos/remc2-dev2/remc2/x64/Release
KD=""
[ $K != 0 ] && KD=$K
MC1_TEMPKEYDOWN=$KD MC1_TEMPWIZ="$WD" MC1_TEMPWIZFRAME=$F REMC2_SKIPINTRO=1 timeout 60 ./remc2.exe -game mc1 -custom -level $L > $D/port.log 2>&1
python - $D <<'PYEOF'
import struct, sys
d = sys.argv[1]
pa, pb = open(d + '/mc1_player.bin', 'rb').read(), open(d + '/port_player.bin', 'rb').read()
diff = [i for i in range(0x801) if pa[i] != pb[i]]
print('player', len(diff), ' '.join('%X:%02X/%02X' % (i, pa[i], pb[i]) for i in diff[:40]))
idx = struct.unpack_from('<H', pa, 0x0A)[0]
ents = open(d + '/mc1_entities.bin', 'rb').read()
wa = ents[idx * 0xA4 + 4: idx * 0xA4 + 0x9C]
wb = open(d + '/port_wizard.bin', 'rb').read()
diff = [i for i in range(len(wa)) if wa[i] != wb[i]]
print('wizard', idx, len(diff), ' '.join('%X:%02X/%02X' % (i + 4, wa[i], wb[i]) for i in diff[:40]))
print('dos pos', struct.unpack_from('<3h', wa, 0x48 - 4), 'speed', struct.unpack_from('<h', wa, 0x7E - 4)[0],
      'port pos', struct.unpack_from('<3h', wb, 0x48 - 4), 'speed', struct.unpack_from('<h', wb, 0x7E - 4)[0])
PYEOF
