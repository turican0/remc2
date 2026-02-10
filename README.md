# REMC2
## Now actually in HD or 2K!
Based off the Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language by Tomas Versly <br />
Forked from Tomas Vesely's repo here: https://github.com/turican0/remc2 <br /><br />
Tomas has done amazing work, not only reverse engineering this code but updating it to use more modern memory allocation and use the SDL library for input and sound. He has even increased the sprite resolutions.

## Download the latest Alpha Here (now with MSI Installer)! ##
https://github.com/thobbsinteractive/magic-carpet-2-hd/releases/latest

### Install Guide for GOG Edition or from Magic Carpet CD ###
#### Windows ####
- 1: Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
- 2: Install the Game.
- 3: Download the x86 or x64 MSI file. (x64 is faster but also a little more buggy)
- 4: Run the MSI file. Follow the on-screen instructions to install.
- 5: Click the Start menu shortcut "Magic Carpet 2 HD" to run the game.
- 6: Any errors will be output to "log.txt"

#### Basic Controls ####
Controls can be redefined in the Config.json file, however here are the defaults:
</br>
Forward = W</br>
Backwards = S</br>
Move Left = A</br>
Move Right = D</br>
Open Spell Menu = LCtrl / Mouse 4</br>
Open Map = Tab / Middle Mouse</br>

#### Command Line Args ####
You can use the following arguments:</br>
To jump to a selected Level use:</br>
--set_level [0-25]</br>
To run a level you have created using the editor:</br>
--custom_level "[Path to .mc2 file]"</br>
To record a playthough (only works for x86):</br>
--record_file "[File Path]"</br>
To playback a playthough (only works for x86):</br>
--play_file "[File Path]"</br></br>
To change the log level of the game:</br>
--log_level [Info, Debug, Trace (will impact performance)]</br>
To run the game with option to show in game debug info (press "J" to show):</br>
--enable_in_game_debug</br>

## FAQ ##
https://github.com/thobbsinteractive/magic-carpet-2-hd/wiki/FAQ

## For more information on Magic Carpet 2 ##
Moburma has been tirelessly working to document cut levels, level data structures and missing graphics at: https://tcrf.net/Magic_Carpet_2:_The_Netherworlds

## My intention is to make a patch for Magic Carpet 1 and 2 (GOG editions) that initially will:
- Add more screen resolution options
- Increase draw distance
- Seperate Render and Simulation code so that game speed is not dependent of FPS (or fix FPS)
- Enable local multiplayer without NETBIOS

## Build Status:

|Branch|Windows|Linux|
|------|:-----:|:---:|
|master|[![MSBuild CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml/badge.svg?branch=master)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml)|[![Linux 64bit CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux.yml/badge.svg?branch=master)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux.yml)|
|development|[![MSBuild CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml/badge.svg?branch=development)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/msbuild.yml)|[![Linux 64bit CI](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux.yml/badge.svg?branch=development)](https://github.com/thobbsinteractive/magic-carpet-2-hd/actions/workflows/linux.yml)|

#### STATUS: Code now runs and all of MC2 (in both Windows and Linux) seems to be playable. Anyone with the GOG edition can download this repo, extract the Game Assets (from a legal GOG copy of the game) and run it. ####

## Steps: to build and run this code

### Windows:
- 1: Install the latest version of Visual Studio 2022 Community. Ensure you install [vcpkg](https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/)
- 2: Pull the development branch
- 3: Open "remc2.sln", you can build either x64 or 32 bit versions
- 4: Build the code
- 5: Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
- 6: Install the Game. Copy the "NETHERW" directory to "remc2\Debug\" Folder
- 7: Copy the "Extract" folder to your Game Directory, run extract-GOG-CD.bat. The CD Data will now be copied to a directory called "CD_Files" in the "Extract" directory
- 8: Move "CD_Files" directory into the "remc2\Debug" Folder
- 9: Run

### Linux:

#### Building on Linux

There are two ways to build the Linux binary.
- Building a native binary
  1. Pull the development branch using GitHub (this is much easier if you install Visual Studio Code and install C++ Extension, cmake, cmake tools). When pulling the branch either do a recursive clone of the repository or ensure that after the pull you run: `git submodule init` and `git submodule update`
  2. Once pulled, within the `magic-carpet-2-hd/` directory pull the `findfirst` repo.
  3. Make sure that you have `CMake`, `make` and a recent `GCC` installed
     - To install them on Debian/Pi OS: `sudo apt install -y cmake`  
  4. Make sure that you have the following dependencies as development packages (the exact names depend on your distro)
  - SDL2
  - SDL2_mixer
  - SDL2_image
  - SDL2_ttf
  - libpng
  - boost
  - boost-system
  - spdlog
  - rapidjson-dev
    - To install them on Debian/Pi OS: `sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libpng-dev libpng++-dev libboost-system-dev libspdlog-dev rapidjson-dev` 
  4. Build the code
  ```bash
  export BUILDTYPE=Debug # or Release
  mkdir -p build/${BUILDTYPE}
  cd build/${BUILDTYPE}
  cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DCMAKE_INSTALL_PREFIX=./inst [SOURCE_DIR]
  make
  make install
  ```
  5. Magic Carpet 2 is now built. you can find it in `build/Debug/inst/bin`
     - You can also run the code with sanitizers (leak, address, undefined behaviour, pointers) by passing `-DUSE_SANITIZERS=True` to CMake

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

- Running clang-tidy for static code analysis
  1. Run CMake with the flag `CMAKE_EXPORT_COMPILE_COMMANDS` for exporting the build commands like this
  ```
  cmake -GNinja -DUSE_SANITIZERS=True -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DCMAKE_INSTALL_PREFIX=./inst -DUNIT_TESTS=True  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
  ```
  2. Run clang-tidy
  ```
  run-clang-tidy -p . -j 4 -export-fixes clang-tidy-fixes.yaml
  ```

### Providing the original game assets to `remc2` and running the game

In order to run the game you need to own a copy of Magic Carpet 2. We provide a script to extract the assets from the GOG version. The following steps extract the required files from the original.
  1. Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
  2. Download the Windows "Offline Backup Game Installer"
  - Open GOGGalaxy
  - Install the game
  3. In order to retrieve the original game's assets run the following script located in the root of the repository:
  ```
  bash check_install.sh -s [directory where GOG installed MC2] -d [destination directory]
  # for example :
  bash check_install.sh -s "${HOME}/.wine/drive_c/games/Magic Carpet 2/" -d "build/${BUILDTYPE}/inst/bin/NETHERW"
  # or :
  bash check_install.sh -s "/mnt/c/Program Files (x86)/GOG Galaxy/Games/Magic Carpet 2/" -d "build/${BUILDTYPE}/inst/bin/NETHRW"
  ```
  Please note that if you have used any other method to get the assets, at least run a check to make sure that remc2 has access to every file it needs:
  ```
  bash check_install.sh -d [destination directory]
  # for example:
  bash check_install.sh -d "build/${BUILDTYPE}/inst/bin/"
   [ ok ] CD_Files directory
   [ ok ] GAME directory
  ```
  this script will fix the permissions, make everything uppercase, check all file hashes and complain if any file is missing.
  it's also recommended that before the first run you remove the file GAME/NETHERW/CONFIG.DAT if it exists.
  4. NOTE: The game will search in the following locations (and in this particular order) for the game assets. For the flatpak only the first two locations can be used.
     1. `$XDG_DATA_HOME/remc2/`
     2. `$HOME/.local/share/remc2`
     3. next to the `remc2` binary
  5. Run the `remc2` executable in install directory
  ```
  cd magic-carpet-2-hd/build/${BUILDTYPE}/inst/bin
  ./remc2
  ```

#### Configuring `remc2`

Some settings can be configured via the file `config.json`. An example for this file can be found in the root directory of the `remc2` repository.
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
- [X] Implement Controller Support
- [ ] Implement a (platform independent) Launch menu to adjust settings in config.ini before launch

## MILLSTONE 3
- [ ] Improve sounds and music using updated original scores and directional sounds in game - In Review!
- [X] Implement a wix sharp .msi installation for new .exe to make patching the and running existing game simple and something similar for the Linux versions

## MILLSTONE 4
- [ ] Get basic LAN/IPv4 multiplayer working again (Tom is currently making great progress on this!)

## MILLSTONE 5
- [ ] Get Magic Carpet 1 working using this engine. Ideally with original music and graphics.

## LONG TERM GOALS
- Add VR support back into the game (yes it was originally supported! This game was waaay ahead of its time)<br />
- Implement online multiplayer match making

## If you know a bit about game development or want to help out, branch away or email me here: thobbsinteractive@gmail.com or find us on Discord here: https://discord.gg/GR55HCbJJ4

## Development Guide ##
- The Project is compiled as C++17.
- If you re-name a method include the id from the original method name as this makes it easier to track changes from the generated code.
e.g. `void sub_19CA0_sound_proc5(unsigned __int8 a1)` was renamed to `void ChangeSoundLevel_19CA0(uint8_t option)`
- Please follow the general style of the refactored code. Upper Camel Case (Pascal Case) for Class/Method names. Camel Case for variables. 'm_' for class members. `GameRenderHD.cpp` is a good example of the style.
- Where possible (if writting new code) please use the fixed width types. https://en.cppreference.com/w/cpp/types/integer
- For each commit please use the Semantic Commit Messages: https://gist.github.com/joshbuchea/6f47e86d2510bce28f8e7f42ae84c716
- Be careful with making logic changes to the code and Test, Test, Test! I recommend playing the first level all the way though. Then the first Cave level (4) and I also recomend Level 5 as you have a nice mix of AI to kill and a cutscene at level completion.
- Please build and run the remc2-regression-test project BEFORE making a pull request. This must pass and since it needs the game data cannot be placed in the Github Actions.

## License ##
### Original Source Code is Copyright 1995 Bullfrog Productions ###

### Additonal Code is Licensed under the following MIT Licence: ###
Copyright 2022 Magic Carpet 2 HD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
