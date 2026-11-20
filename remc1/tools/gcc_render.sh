#!/bin/bash
cd /mnt/c/prenos/remc2-dev2/remc2/remc2
run() { case "$1" in *.c) g++ -x c -fsyntax-only -w -DCOMPILE_FOR_64BIT -DAWE32EMU_WITH_86BOX -D_DEBUG -I/mnt/c/prenos/remc2-dev2/remc2/findfirst -I/mnt/c/prenos/remc2-dev2/remc2/remc2/vcpkg_installed/x64-windows/x64-windows/include -I/mnt/c/prenos/remc2-dev2/remc2/remc2/vcpkg_installed/x64-windows/x64-windows/include/SDL2 -I/mnt/c/prenos/remc2-dev2/remc2/remc2/portability/awe32/86box/include "$1";; *) g++ -std=c++17 -fsyntax-only -w -DCOMPILE_FOR_64BIT -DAWE32EMU_WITH_86BOX -D_DEBUG -fpermissive -I/mnt/c/prenos/remc2-dev2/remc2/findfirst -I/mnt/c/prenos/remc2-dev2/remc2/remc2/vcpkg_installed/x64-windows/x64-windows/include -I/mnt/c/prenos/remc2-dev2/remc2/remc2/vcpkg_installed/x64-windows/x64-windows/include/SDL2 -I/mnt/c/prenos/remc2-dev2/remc2/remc2/portability/awe32/86box/include "$1";; esac 2>&1 | grep -E "error|^In file" | sed "s|^|$1: |" | head -12; }
export -f run
printf "%s\n" "engine/GameRenderInterface.cpp" "engine/GameRenderOriginal.cpp" "engine/GameRenderHD.cpp" "engine/GameRenderNG.cpp" "engine/GameRenderHW.cpp" | xargs -P 8 -I{} bash -c 'run "{}"'
echo checked 26
