# LUDO (C Console Simulation)

This is a console-based Ludo game simulation written in C. The game runs automatically: players and moves are selected randomly based on dice rolls, and the state is printed after each round. The goal is to move all four pieces of a player to the home path and reach home first.

## Features

- Four players: Yellow, Blue, Red, Green
- Four pieces per player
- Dice roll logic (1-6)
- Automatic move selection with basic Ludo rules
- Capturing opponent pieces
- Home path and exact-roll requirement to finish
- Round-by-round status output

## How It Works

- The game picks a starting player by rolling the dice for each player.
- Play order is set based on the starting player.
- Each player takes turns until all pieces are at home.
- If a player rolls a 6, they get an extra turn (up to two extra turns; three consecutive 6s cancels the third roll).
- Pieces start in base and can only enter the board on a roll of 6.
- Capturing returns opponent pieces to their base.
- A piece must land exactly on the final home cell to finish.

## Files

- main.c: Program entry point and game loop
- logic.c: Core game logic and rules
- types.h: Game constants and data structures

## Build and Run (Windows + MinGW)

From the project folder:

```
gcc main.c logic.c -o ludo_game.exe
```

Run:

```
./ludo_game.exe
```

## Build and Run (Linux / macOS)

```
gcc main.c logic.c -o ludo_game
./ludo_game
```

## Notes

- This is a simulation; there is no human input.
- Output is randomized because dice rolls use the current time as the seed.

## Possible Improvements

- Add human input and move selection
- Add a GUI or text-based board view
- Add safe cells and block rules
- Save and load game state
