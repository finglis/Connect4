Connect 4
Fiona Inglis - 40509175 

------------

Requirements
This program requires C to be installed in addition to a compilier 
for windows I reccomend installing mingw-w64 - http://mingw-w64.org/doku.php/download).

------------

Compilation
The program must first be compiled to run, to do this open the command line and navigate to the containing folder and use:
~$ gcc connect4.c -o connect4.exe

------------

Running
To run the game, enter
~$ .\connect4.exe

------------

Gameplay
When running, the game will present you with 5 game opetions. 
To select an option, simply enter the number of your choice and hit enter and you will see the board load.
Follow the on screen instructions for the respective user (`X`,`O` or `S`) to enter their desired colum choice value and press `enter`.
The piece will be placed on the board if there is space, if there is not space the game will altert the use to this and request
that they choose again. 
Each player will take turns until the game is won where it will then be saved for later replay. 
To cancel the game at any time, press `ctrl + c`. This will allow you to restart the game by running `.\connect4.exe` again. 
  
------------

GamemodesGamemodes
1. Standard 1v1 game on a 7x6 board where you are trying to connect 4 pieces in a row to win.
   This mode support save features so the game can be played at the end.
2. Standard game on a 7x6 board where you are trying to connect 4 pieces in a row to win but you are against a bot.
   The bot will choose random numbers within the collumn range.
   This mode support save features so the game can be played at the end.
3. Standard 3 player game on a 7x6 board where you are trying to connect 3 pieces in a row to win.
4. Advanced mode where players will be requested to enter the board dimensions they would like (up to a maximum of 20x20 to 
   keep the game exciting) in addition to how many pieces in a row will produce a win.
   The game will not accept a board size smaller than the number of pieces required to win. If this occurs, please re-run the game.
5. Load game mode which requires the user to input the save file from a previous standard board size game with 2 players (bot accpeted).
   There will be a 2 second delay between each entry on the board until the game announces which player wins. d delay between each entry on the board until the game announces which player wins. 