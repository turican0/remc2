### Building on Pi OS/Debian

  1. Pull the development branch using GitHub (this is much easier if you install Visual Studio Code and install C++ Extension, cmake, cmake tools)
  2. Make sure that you have `CMake`, `make` and a recent `GCC` installed
  - `sudo apt install -y cmake`
  3. Make sure that you have the following dependencies as development packages (the exact names depend on your distro)
  - SDL2
  - SDL2_mixer
  - SDL2_image
  - SDL2_ttf
  - libpng
  - boost
  - boost-system
  - `sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libpng-dev libpng++-dev libboost-system-dev`
  4. Build the code
  ```bash
  export BUILDTYPE=Debug # or Release
  mkdir -p build/${BUILDTYPE}
  cd build/${BUILDTYPE}
  cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DCMAKE_INSTALL_PREFIX=./inst ../../
  make
  make install
  ```
  Magic Carpet 2 is now built. you can find it in `build/Debug/inst/bin`

### Providing the original game assets to `remc2` and running the game

In order to run the game you need to own a copy of Magic Carpet 2. We provide a script to extract the assets from the GOG version. The following steps extract the required files from the original:
  1. Purchase a copy of Magic Carpet 2 from GOG here: https://www.gog.com/game/magic_carpet_2_the_netherworlds
  2. Download the Windows "Offline Backup Game Installer"
  - Open GOGGalaxy
  - Install the game
  - Go to the "Extras" section of a GOG Magic Carpet 2
  - In the Offline Backup Installers Windows section:
    - Click the Download button
    - Wait for the download to complete
    - Click the Downloads menu item on the left. 
    - Click to open the containing folder and copy it to the `Downloads` folder on your Linux PC
  3. Make sure that you have `innoextract` and `dosbox` installed
  - `sudo apt install innoextract dosbox`
  4. Run the `extract-GOG-CD.sh` script from the `EXTRACT` directory of the `remc2` source code and provide the path to the GOG installer as well as a path where the files should be extractet to. Example:
  ```
  ./extract-GOG-CD.sh ~/Downloads/setup_magic_carpet_2_1.0_\(28044\).exe ~/.local/share/remc2
  ```
  5. NOTE: The game will search in the following locations (and in this particular order) for the game assets. For the flatpak only the first two locations can be used.
     1. `$XDG_DATA_HOME/remc2/`
     2. `$HOME/.local/share/remc2`
     3. next to the `remc2` binary
  
  Run the `remc2` executable in install directory
  ```bash
  cd /Documents/repos/magic-carpet-2-hd/build/${BUILDTYPE}/inst/bin
  ./remc2
  ```

### Notes
Currently the Game will not load the first level, it fails with a `Segment Fault`