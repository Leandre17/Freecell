# Freecell

Freecell — a compact and portable Freecell solitaire implementation in C++.

## Overview

This repository contains a console-based Freecell game. A standard 52-card deck is shuffled and dealt into 8 tableaus, with 4 free cells and 4 foundation piles. The code aims to be simple and readable, suitable for enhancements (GUI, AI, rules enforcement).

## Quick start

Prerequisites:

- CMake >= 3.10
- A C++ compiler with C++11 support (g++, clang++, MSVC)

Build and run (Linux/macOS):

```sh
mkdir -p build
cd build
cmake ..
cmake --build .
./freecell
```

On Windows (PowerShell):

```powershell
mkdir build; cd build
cmake ..
cmake --build .
.\freecell.exe
```

## Features

- Shuffle and deal a standard 52-card deck
- Display current game state: tableaus, free cells, foundations
- Save/load game state to `save.txt`
- Small, self-contained C++ codebase easy to extend

## Developer notes

- Sources: [src/](src/)
- Headers: [include/](include/)
- Build outputs: [build/](build/)
- Tests: [tests/](tests/)

Formatting:

This project includes a `.clang-format` at the repository root. Use the provided CMake targets:

```sh
cmake --build build --target clang-format
cmake --build build --target clang-format-check
```

Save / Load

The game writes/reads its persistent state from `save.txt` in the project root (see `save.cpp`). Use this to continue an in-progress game.


**Running Tests**

- The project uses Catch2 (fetched automatically at build time). To build and run the unit
  tests with CMake:

```sh
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target unit_tests -j 4
ctest --test-dir build --output-on-failure -j 4
```

- You can also run the test executable directly:

```sh
./build/tests/unit_tests
```

**Coverage (gcovr)**

To generate a coverage report using `gcovr`, enable the coverage build and run the CMake `coverage` target. `gcovr` must be installed and available in `PATH` (for example via `pip install gcovr`).

```sh
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build --target coverage -j 4
# Output: build/coverage.html and build/coverage.xml
```

If `gcovr` is not found when configuring, CMake will warn and the `coverage` target will be unavailable.

---

## Roadmap / Suggestions for improvements

- Implement full move validation and enforce game rules
- Add interactive command-line controls and help text
- Implement undo/redo stack
- Auto-move safe cards to foundations
- Add unit tests around core game logic
- Provide a simple GUI (SDL/Qt) or web front-end

## Contributing

Contributions are welcome. Suggested workflow:

1. Fork the repo and create a feature branch
2. Make focused commits with clear messages
3. Run and verify formatting (`clang-format` target)
4. Open a pull request describing your changes

## License

This project is provided "as-is". Add a license file (e.g. MIT) if you intend to publish or accept external contributions.

## Contact

If you want help extending the project (tests, GUI, packaging), open an issue or contact the maintainer.

---

File: [README.md](README.md)
