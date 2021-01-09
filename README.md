# Magic Carpet 2 Community Patch
Based off the Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language by Tomas Versly <br />
Forked from Tomas Vesely's repo here: https://github.com/turican0/remc2 <br /><br />
Tomas has done amazing work, not only reverse engineering this code but updating it to use more modern memory allocation and use the SDL library for input and sound. He has even increased the sprite resolutions.

## My intention is to make a patch for Magic Carpet 1 and 2 (GOG editions) that initially will:
- Add more screen resolution options
- Increase draw distance
- Seperate Render and Simulation code so that game speed is not dependent of FPS (or fix FPS)
- Add configuration options for Anaglyph 3D stereoscopic render (because you cannot get the original glasses anymore)
- Enable local multiplayer without NETBIOS

## STATUS: Code now runs though the first level and more. Anyone with the GOG edition can download this repo, extract the Game Assets (from a legal GOG copy of the game) and run it.
## Build Status:

|Branch|Windows|Linux|
|------|:-----:|:---:|
|master|![Windows master CI status](https://github.com/GrimSqueaker/remc2/workflows/MSBuild/badge.svg?branch=master)|![Linux master CI status](https://github.com/GrimSqueaker/remc2/workflows/Linux/badge.svg?branch=master)|
|development|![Windows development CI status](https://github.com/GrimSqueaker/remc2/workflows/MSBuild/badge.svg?branch=development)|![Linux development CI status](https://github.com/GrimSqueaker/remc2/workflows/Linux/badge.svg?branch=development)|

## Steps: to build and run this code

### Windows:
- 1: Pull the development branch
- 2: Open the Solution file "\libogg\win32\VS2015\libogg_static.sln" and build it. This builds the libogg library
- 3: Close that solution and open "remc2.sln" Select "x86" in build config.
- 4: Build the code (you can only compile only 32-bit binary version atm)
- 5: Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
- 6: Install the Game. Copy the "NETHERW" directory to "remc2\Debug" Folder
- 7: Copy the "Extract" folder to your Game Directory, run extract-GOG-CD.bat. The CD Data will now be copied to a directory called "CD_Files" in the "Extract" directory
- 8: Move "CD_Files" directory into the "remc2\Debug" Folder
- 9: Run

### Linux:
1. Pull the development branch
2. Make sure that you have the following dependencies as x86 development packages
 - SDL2
 - SDL2_MIXER
 - libpng
3. Make sure that you have `CMake`, `make` and a recent `GCC` installed
4. Build the code
```bash
export BUILDTYPE=Debug # or Release
mkdir build_${BUILDTYPE}
cd build_${BUILDTYPE}
cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DCMAKE_INSTALL_PREFIX=./inst [SOURCE_DIR]
make
make install
```
5. Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
6. Download the Windows "Offline Backup Game Installer"
7. Make sure that you have `innoextract` and `dosbox` installed
8. Run the `extract-GOG-CD.sh` script and provide the path to the GOG installer and the install bin directory like
```
./extract-GOG-CD.sh ~/Downloads/setup_magic_carpet_2_1.0_\(28044\).exe ~/dev/remc2/build/inst/bin
```
9. Run

# ROADMAP:

## MILLSTONE 1
- Get solution runnable from Visual Studio 2019 build, with minimum of setup. Cut down on unnecessary extra files and libraries and use nuget instead.
- Refactor reverse engineered code into seperate classes where possible.

## MILLSTONE 2
- Add resolution support and increase draw distance
- Implemented wix sharp .msi installation for new .exe to make patching the and running existing game simple

## LONG TERM GOALS
- I eventually want to implement an Open GL render with multisampling for the terrain and some shadow-mapped lighting (I am not sure what SDL supports as it is middleware not really intended for more advanced rendering, but I will have to investigate)<br />
- Add VR support back into the game (yes it was originally supported! This game was waaay ahead of its time)<br />
- Support Linux and Mac OS versions.

## If you know a bit about game development or want to help out, branch away or email me via GitHub
