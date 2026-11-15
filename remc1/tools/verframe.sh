#!/bin/bash
# verframe.sh <c|h> <level> [code]: the first frame after 3C9D0 (dump at 415C0 / 41900), port vs DOSBox
export LC_ALL=C.UTF-8
S=$(cd "$(dirname "$0")" && pwd)
W=$(cygpath -w $S)
k=$1; L=$2
if [ $k = c ]; then v=mc1; code=${3:-415C0}; t=CC1E0; h=DC1E0; s=EC1E0; a=FC1E0; i=10C1E0; p=AE400; q=AE408; exe="CARPET.EXE"; game=mc1
else v=hw; code=${3:-41900}; t=CC1D0; h=DC1D0; s=EC1D0; a=FC1D0; i=10C1D0; p=AE3F0; q=AE3F8; exe="HIDDEN.EXE"; game=mc1hw; fi
D=$S/frame_$k$L; rm -rf $D; mkdir -p $D
WD="$W\\frame_$k$L"
[ -f $S/$k$L.conf ] || python $S/mkconf.py "$exe -custom -level $L" $S/$k$L.conf
cd $S
DBXCONF="$W\\$k$L.conf" python dbxdump.py $v $code "$WD" terrain=$t:10000 height=$h:10000 shading=$s:10000 angle=$a:10000 index=$i:20000 stateptr=$p:4 settingsptr=$q:4 > /dev/null
base=$(python -c "import struct,sys;print(struct.unpack('<I',open(sys.argv[1],'rb').read())[0])" "$D/${v}_stateptr.bin")
sbase=$(python -c "import struct,sys;print(struct.unpack('<I',open(sys.argv[1],'rb').read())[0])" "$D/${v}_settingsptr.bin")
h() { printf "%X" $((base + $1)); }
DBXCONF="$W\\$k$L.conf" python dbxdump.py $v $code "$WD" entities=@$(h 0x7463):280A0 players=@$(h 0x2409):505A settings=@$(printf "%X" $sbase):A0 top2=@$(h 0x11F1):4 > /dev/null
cd /c/prenos/remc2-dev2/remc2/x64/Release
MC1_TEMPDUMP="$WD" REMC2_SKIPINTRO=1 timeout 120 ./remc2.exe -game $game -custom -level $L > $D/port.log 2>&1
cd $S
echo "== $v level $L"
python cmpframe.py $D $v
