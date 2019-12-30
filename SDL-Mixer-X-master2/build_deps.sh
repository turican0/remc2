#!/bin/bash

if [ ! -d build ];then
    mkdir build
fi

cd build

if [ ! -d _build_cache ];then
    mkdir _build_cache
    cd _build_cache
    git clone --depth 1 git@github.com:WohlSoft/AudioCodecs.git AudioCodecs
    wget --content-disposition https://hg.libsdl.org/SDL/archive/default.tar.gz -O SDL-default-new.tar.gz
    tar -xf SDL-default-new.tar.gz
    cd ..
fi

cd ..

