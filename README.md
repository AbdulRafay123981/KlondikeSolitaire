# Klondike Solitaire

A C++ implementation of the classic Klondike Solitaire card game, developed as the CSC200 Data Structures and Algorithms midterm project.

## Project Overview

Klondike Solitaire is a single-player card game played with a standard 52-card deck. The objective is to move all cards to four foundation piles, arranging each suit in ascending order from Ace to King.

The game includes card dealing, stockpile drawing, legal card movements, undo/redo functionality, and win-condition detection.

## Features

- Standard 52-card deck with four suits and thirteen ranks.
- Deck shuffling and automatic game initialization.
- Seven tableau columns with face-up and face-down cards.
- Stockpile and waste-pile management.
- Draw-one and draw-three modes.
- Legal moves between tableau columns.
- Moves from the waste pile to the tableau and foundations.
- Moves from tableau columns to foundations.
- Automatic card flipping when a face-up tableau card is removed.
- Undo and redo functionality.
- Game win-condition detection.
- Card-location tracking and consistency validation.

## Data Structures

The project implements custom data structures rather than relying on their standard-library equivalents.

| Data Structure  | Implementation and Purpose                                                                                                  |
| --------------- | --------------------------------------------------------------------------------------------------------------------------- |
| **Array**       | Stores the 52 cards in the deck before shuffling and dealing.                                                               |
| **Stack**       | Used for foundation piles, the waste pile, and temporary storage during card-sequence transfers and waste recycling.        |
| **Queue**       | Manages the stockpile using FIFO order, including draw-one and draw-three modes.                                            |
| **Linked List** | Represents the seven tableau columns and supports inserting, removing, and accessing cards.                                 |
| **HashMap**     | Tracks each card's current pile, pile index, position, and face-up/down state. Uses a custom double-hashing implementation. |

### Card Location Tracking

Every card has a unique ID calculated from its suit and rank. The custom HashMap uses this ID to locate the card and record its current location.

The game also includes a consistency-validation function that checks whether all 52 cards are present exactly once and whether their recorded locations match the actual piles.

## Requirements

- C++ compiler supporting C++17.
- CMake.
- Ninja build system.

## Building the Project

From the project directory, configure the project:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER="C:/mingw64/bin/g++.exe"
```

Build the executable:

```powershell
cmake --build build
```

## Running the Game

```powershell
.\build\KlondikeSolitaire.exe
```

## Controls

| Command | Action                                                                 |
| ------- | ---------------------------------------------------------------------- |
| `D`     | Draw cards from the stockpile.                                         |
| `R`     | Recycle the waste pile into the stockpile when the stockpile is empty. |
| `M`     | Move a face-up card sequence between tableau columns.                  |
| `A`     | Move the top waste card to a tableau column.                           |
| `F`     | Move the top waste card to a foundation pile.                          |
| `G`     | Move a tableau card to a foundation pile.                              |
| `U`     | Undo the previous move.                                                |
| `Y`     | Redo an undone move.                                                   |
| `1`     | Select draw-one mode.                                                  |
| `3`     | Select draw-three mode.                                                |
| `Q`     | Quit the game.                                                         |

The game validates moves according to Klondike rules. Invalid moves are rejected.

## Testing

The project includes test suites for its custom data structures and game mechanics.

Run all tests with:

```powershell
ctest --test-dir build --output-on-failure
```

The tests cover:

- Deck functionality.
- Stack, Queue, Linked List, and HashMap operations.
- Game initialization.
- Stockpile and waste-pile mechanics.
- Tableau and foundation movements.
- Win-condition detection.
- Undo/redo history.
- Draw-one and draw-three modes.
- Card-location consistency.

## Project Information

**Course:** CSC200 – Data Structure and Algorithms  
**Project:** Klondike Solitaire  
**Instructor:** Ms Tazeem
**Deadline:** November 22, 2026
