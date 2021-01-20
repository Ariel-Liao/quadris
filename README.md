# Quadris

##Requirements

1. gtkmm
2. g++
3. C++17


## Bonus Features
### GUI Keyboard Events and Buttons
The GUI has buttons for drop, left/right/down, levelup/down, rotate CW/CCW. These can be clicked to enter commands instead of using command line.

GUI accepts keyboard events: ‘d’->right, ‘a’->left, ‘s’->down, ‘k’->drop, ‘q’->CCW, ‘e’->CW, ‘h’->leveldown, ‘y’->levelup
### Renaming commands
Can enter “rename right abcd” as a command, then when prompted for a command, entering either “abcd” or “right” will move current block right.

the renamed command will also be compatible with prefix (“abc”) and numbers (“3abc”) only if the new name doesn’t start with a number. The existing command accepts prefixes (can do “rename rig abcd” but should not do “rename right 3abcd”)
### Move to hint
The “movetohint” command will move and drop the current block to where the hint block would be. 

Possible drop locations for hint, accounts for how many moves are required (rotation + left/right, only rotate, only left/right for >= level3) so it should always be a possible location
Keyboard input and Buttons on GUI

### Smart Pointers
Used smart pointers to manage all internal memories. 
### Multithread

Multithread programming is used to support graphic view running simultaneously with textview and game. Mutex is used for ensure thread safety

### Notice

1. Use flag '-disableBonus' to disable the Bonus Feature 5 above. That is, only support a graphic view without playing game with it.
2. When the game is over, no commands is accepted except "restart"
3. Although you will not be able to continue playing in both command line and GUI when encountering EOF character, you will still need to shut down the 

# Projects
