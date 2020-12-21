# Magic Carpet 2 Modern PC Patch
# Based off the Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language by Tomas Versly<br />
## Forked from Tomas Vesely's repo here: https://github.com/turican0/remc2<br />
## Tomas has done amazing work, not only reverse engineering this code but updating it to use more modern memory allocation and use the SDL library for input and sound. He has even increased the sprite resolutions. <br />
## My intention is to make a patch for Magic Carpet 1 and 2 (GOG editions) that initially will:<br />
- Add more screen resolution options<br />
- Increase draw distance<br />
- Seperate Render and Simulation code so that game speed is not dependent of FPS (or fix FPS)<br />
- Add configuration options for Anaglyph 3D stereoscopic render (because you cannot get the original glasses anymore)<br />
- Enable local multiplayer without NETBIOS<br />
## STATUS: Code is currently not running due to fixes required for asset paths when run from a repo. I am currently trying to fix this so anyone with the GOG edition can download this repo, extract the Game Assets (from a legal GOG copy of the game) and run it<br />
## Steps: to build and run this code:<br />
- 1: Pull the vc-2019-build branch<br />
- 2: Build the code (you can only compile only 32-bit binary version atm)<br />
- 3: Purchase a copy of Magic Carpet 2 from GOG here:<br />
https://www.gog.com/game/magic_carpet_2_the_netherworlds<br />
- 4: Copy the "Extract" folder to your Game Directory, run extract-GOG-CD.bat. The CD Data will now be copied to a directory called "CD_Files" in the Extract directory<br />
- 5: Move "CD_Files" directory into the "remc2\Debug" Folder<br />
- 6: Run<br />
<h2>ROADMAP:</h2>
<h3>MILLSTONE 1</h3>
- Get solution runnable from Visual Studio 2019 build, with minimum of setup<br />
- Refactor reverse engineered code into seperate classes where possible<br />
<h3>MILLSTONE 2</h3>
- Add resolution support and increase draw distance<br />
<h3>LONG TERM GOALS</h3>
- I eventually want to implement an Open GL render with multisampling for the terrain and some shadow-mapped lighting (I am not sure what SDL supports as it is middleware not really intended for more advanced rendering, but I will have to investigate).<br />
- Add VR support back into the game (yes it was originally supported! This game was waaay ahead of its time)<br />
- Support Linux and Mac OS versions.<br />
## If you know a bit about game development or want to help out, branch away or email me via GitHub
