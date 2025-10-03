# Freecell
A Freecell card game implemented in C++

## Description
This is a classic Freecell solitaire game implementation in C++. The game displays a shuffled deck dealt into 8 tableau columns, with 4 free cells and 4 foundation piles.

## Building the Project

### Prerequisites
- CMake (version 3.10 or higher)
- A C++ compiler with C++11 support (g++, clang++, or MSVC)

### Build Instructions

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Generate build files with CMake:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

## Running the Game

After building, run the executable:
```bash
./freecell
```

On Windows:
```bash
freecell.exe
```

## Features
- Random card shuffling and dealing
- Display of game state (tableau, free cells, foundations)
- Basic game structure ready for expansion

## Future Enhancements
- Move validation and execution
- Auto-move to foundations
- Undo/Redo functionality
- Win condition checking
- Interactive gameplay
