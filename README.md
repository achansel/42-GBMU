# GBMU
[![Generic badge](https://img.shields.io/static/v1?label=Skills&message=Assembly%2C%20C%2B%2B&color=4c3457&style=for-the-badge)](https://shields.io/)
#### The goal of this project is to make a working GameBoy/GameBoy Color emulator, able to smoothly run games such as Tetris, Super Mario Land 2, Pokemon Gold...

This repository is my take on it, and, although it is not finished yet, I try and will keep trying to produce a clean code, as simplicity and speed are my priorities.

Emulation of edge cases and hardware bugs are not in the scope of this project to me, as it is simply made for learning. Of course, CPU accuracy and system timings will not be neglected.

This README shall be updated frequently, for now, Pokemon Red/Blue and Tetris appear to run smoothly, without bugs. 

## Pictures
&nbsp;
<div align="center">
  <img src="https://i.postimg.cc/wMBXns5H/pkmn.png" alt="Pokemon Blue Titlescreen" width="350" style="margin-right: 20px;"/>
  <img src="" alt="" width="25"/>
  <img src="https://i.postimg.cc/W1cn1tTm/tetris.png" alt="Tetris Gameplay" width="350" style="margin-left: 20px;" />
  <p><i>the palette is custom but it is still plain GB emulation (for now !)</i></p>
</div>

## How to try it
The only dependencies needed are **SDL2** and **CMake**.
\
\
After installing those, the project can be cloned from git and built with CMake.

*Example for Linux/MacOS:*
\
`mkdir -v build && cd build`
\
`cmake .. && cmake --build .`
\
\
After a short wait, a binary called `GBMU` is generated in the build directory.
\
It simply takes the game as an argument.
\
\
*Example for Linux/MacOS:*
\
`./GBMU tetris.gb`



## Disclaimer
*Pokemon, GameBoy and GameBoy Color are trademarks of Nintendo.
I do not claim any ownership or affiliation with Nintendo or their intellectual property.
All content related to Pokemon, GameBoy and GameBoy Color are used for educational and informational purposes only.*
