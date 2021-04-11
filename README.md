# Magic Carpet 2 HD
Based off the Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language by Tomas Versly <br />
Forked from Tomas Vesely's repo here: https://github.com/turican0/remc2 <br /><br />
Tomas has done amazing work, not only reverse engineering this code but updating it to use more modern memory allocation and use the SDL library for input and sound. He has even increased the sprite resolutions.

## My intention is to make a patch for Magic Carpet 1 and 2 (GOG editions) that initially will:
- Add more screen resolution options
- Increase draw distance
- Seperate Render and Simulation code so that game speed is not dependent of FPS (or fix FPS)
- Enable local multiplayer without NETBIOS

## STATUS: Code now runs though the first level of MC2 (in both Windows and Linux). Anyone with the GOG edition can download this repo, extract the Game Assets (from a legal GOG copy of the game) and run it.
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
mkdir -p build/${BUILDTYPE}
cd build/${BUILDTYPE}
cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DCMAKE_INSTALL_PREFIX=./inst [SOURCE_DIR]
make
make install
```
  - You can also run the code with sanitizers (leak, address, undefined behaviour, pointers) by passing `-DUSE_SANITIZERS=True` to CMake
  - Additionally you can compile the code with clang-tidy analyzers by passing `-DUSE_CLANG_TIDY=True` to CMake
5. Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
6. Download the Windows "Offline Backup Game Installer"
7. Make sure that you have `innoextract` and `dosbox` installed
8. Run the `extract-GOG-CD.sh` script and provide the path to the GOG installer and the install bin directory like
```
./extract-GOG-CD.sh ~/Downloads/setup_magic_carpet_2_1.0_\(28044\).exe ~/dev/remc2/build/Debug/inst/bin
```
9. Run the `remc2` executable in install directory

# ROADMAP:

## MILLSTONE 1
- [x] Get solution runnable from Visual Studio 2019 build, with minimum of setup. Cut down on unnecessary extra files and libraries and use nuget instead.
- [x] Refactor reverse engineered code into seperate classes where possible.

## MILLSTONE 2
- [ ] Add resolution support and increase draw distance
- [ ] Implement a (platform independent) Launch menu to adjust settings in config.ini before launch
- [ ] Implement a wix sharp .msi installation for new .exe to make patching the and running existing game simple and something similar for the Linux versions

## MILLSTONE 3
- [ ] Implement Open GL render
- [ ] Improve sounds and music using updated original scores and directional sounds in game

## MILLSTONE 4
- [ ] Get basic LAN/IPv4 multiplayer working again

## MILLSTONE 5
- [ ] Get Magic Carpet 1 working using this engine. Ideally with original music and graphics.

## LONG TERM GOALS
- Add VR support back into the game (yes it was originally supported! This game was waaay ahead of its time)<br />
- Implement online multiplayer match making

## If you know a bit about game development or want to help out, branch away or email me here: thobbsinteractive@gmail.com
