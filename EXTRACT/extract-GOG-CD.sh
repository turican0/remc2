#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo 'Usage: extract-GOG-CD.sh [PATH_TO_GOG_INSTALLER] [PATH_TO_BUILD_DIRECTORY]'
    echo ''
    echo 'Example: extract-GOG-CD.sh ~/Downloads/setup_magic_carpet_2_1.0_(28044).exe ~/dev/remc2_release'
    return
fi

INSTALLER="$1"
BUILD_DIR="$2"

if [ ! -f "${INSTALLER}" ]; then
    echo "${INSTALLER} does not exist."
    return
fi
if [ ! -d "${BUILD_DIR}" ]; then
    echo "${BUILD_DIR} is not a directory."
    return
fi

rm -rf temp_mc2
mkdir temp_mc2
cd temp_mc2

innoextract "${INSTALLER}"

cp -r GAME/NETHERW "${BUILD_DIR}"

mkdir Extract
cd Extract
mkdir CD_Files
cp ../../XXCOPY16.EXE .

# NOTE: could be avoided by mounting the bin/cue (aka gog/ins) image directly
dosbox -conf "../../dosboxExtract-GOG-CD.conf" -c exit

mv CD_Files "${BUILD_DIR}"

cd ../..
rm -rf temp_mc2