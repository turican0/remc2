#!/bin/bash
# verent.sh <c|h> <level>: port vs DOSBox after the features of the level (maps + entities)
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
k=$1; L=$2
if [ $k = c ]; then v=mc1; code=37220; t=CC1E0; h=DC1E0; s=EC1E0; a=FC1E0; i=10C1E0; p=AE400; exe="CARPET.EXE"; game=mc1
else v=hw; code=375E0; t=CC1D0; h=DC1D0; s=EC1D0; a=FC1D0; i=10C1D0; p=AE3F0; exe="HIDDEN.EXE"; game=mc1hw; fi
D=$S/ent_$k$L; rm -rf $D; mkdir -p $D
[ -f $S/$k$L.conf ] || python $S/mkconf.py "$exe -custom -level $L" $S/$k$L.conf
cd $S
DBXCONF="$W\\$k$L.conf" python dbxdump.py $v $code "$W\\ent_$k$L" terrain=$t:10000 height=$h:10000 shading=$s:10000 angle=$a:10000 index=$i:20000 stateptr=$p:4 > /dev/null
ptr=$(python -c "import struct;print('%X'%(struct.unpack('<I',open(r'$W\\ent_$k$L\\${v}_stateptr.bin','rb').read())[0]+0x7463))")
DBXCONF="$W\\$k$L.conf" python dbxdump.py $v $code "$W\\ent_$k$L" entities=@$ptr:280A0 > /dev/null
cd /c/prenos/remc2-dev2/remc2/x64/Release
MC1_TEMPDUMP="$W\\ent_$k$L" REMC2_SKIPINTRO=1 timeout 120 ./remc2.exe -game $game -custom -level $L > $D/port.log 2>&1
cd $S
echo "== $v level $L"
python cmpent.py $D $v
