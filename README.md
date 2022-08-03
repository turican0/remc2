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
|master|[![MSBuild CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml/badge.svg?branch=master)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml)|[![Linux 64bit CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux64bit.yml/badge.svg?branch=master)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux64bit.yml)|
|development|[![MSBuild CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml/badge.svg?branch=development)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml)|[![Linux 64bit CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux64bit.yml/badge.svg?branch=development)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux64bit.yml)|

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

#### Building on Linux

There are two ways to build the Linux binary.
- Building a native binary
  1. Pull the development branch
  2. Make sure that you have the following dependencies as development packages (the exact names depend on your distro)
  - SDL2
  - SDL2_mixer
  - SDL2_image
  - SDL2_ttf
  - libpng
  - boost
  - boost-system
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
  9. Run the `remc2` executable in install directory

- Building a [flatpak](https://flatpak.org/)
  1. Pull the development branch
  2. Build the flatpak
  ```bash
  cd flatpak
  ./build.sh
  ```
  3. Run the `remc2` flatpak via
  ```bash
  flatpak run com.github.thobbsinteractive.magic-carpet-2-hd
  ```

#### Providing the original game assets to `remc2`

In order to run the game you need to own a copy of Magic Carpet 2. We provide a script to extract the assets from the GOG version. The following steps extract the required files from the original.
  1. Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
  2. Download the Windows "Offline Backup Game Installer"
  3. Make sure that you have `innoextract` and `dosbox` installed
  4. Run the `extract-GOG-CD.sh` script from the `EXTRACT` directory of the `remc2` source code and provide the path to the GOG installer as well as a path where the files should be extractet to. Example:
  ```
  ./extract-GOG-CD.sh ~/Downloads/setup_magic_carpet_2_1.0_\(28044\).exe ~/.local/share/remc2
  ```
  5. NOTE: The game will search in the following locations (and in this particular order) for the game assets. For the flatpak only the first two locations can be used.
     1. `$XDG_DATA_HOME/remc2/`
     2. `$HOME/.local/share/remc2`
     3. next to the `remc2` binary

#### Configuring `remc2`

Some settings can be configured via the file `config.ini`. An example for this file can be found in the root directory of the `remc2` repository.
The game will search for this file in the following locations and this particular order. For the flatpak only the first two locations can be used.
1. `$XDG_CONFIG_HOME/remc2`
2. `$HOME/.config/remc2`
3. next to the `remc2` binary


# ROADMAP:

## MILLSTONE 1
- [x] Get solution runnable from Visual Studio 2019 build, with minimum of setup. Cut down on unnecessary extra files and libraries and use nuget instead.
- [x] Refactor reverse engineered code into seperate classes where possible.

## MILLSTONE 2
- [x] Add resolution support
- [ ] Implement Open GL render
- [ ] Implement a (platform independent) Launch menu to adjust settings in config.ini before launch

## MILLSTONE 3
- [ ] Improve sounds and music using updated original scores and directional sounds in game
- [ ] Implement a wix sharp .msi installation for new .exe to make patching the and running existing game simple and something similar for the Linux versions

## MILLSTONE 4
- [ ] Get basic LAN/IPv4 multiplayer working again

## MILLSTONE 5
- [ ] Get Magic Carpet 1 working using this engine. Ideally with original music and graphics.

## LONG TERM GOALS
- Add VR support back into the game (yes it was originally supported! This game was waaay ahead of its time)<br />
- Implement online multiplayer match making

## If you know a bit about game development or want to help out, branch away or email me here: thobbsinteractive@gmail.com

## Development Guide ##
- The Project is compiled as C++17.
- If you re-name a method include the id from the original method name as this makes it easier to track changes from the generated code.
e.g. `void sub_19CA0_sound_proc5(unsigned __int8 a1)` was renamed to `void ChangeSoundLevel_19CA0(uint8_t option)`
- Please follow the general style of the refactored code. Upper Camel Case (Pascal Case) for Class/Method names. Camel Case for variables. 'm_' for class members. `GameRenderHD.cpp` is a good example of the style.
- Where possible (if writting new code) please use the fixed width types. https://en.cppreference.com/w/cpp/types/integer
- Be careful with making logic changes to the code and Test, Test, Test! I recommend playing the first level all the way though. Then the first Cave level (4) and I also recomend Level 5 as you have a nice mix of AI to kill and a cutscene at level completion.

