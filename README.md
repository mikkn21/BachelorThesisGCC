# Bachelor

A very cool compiler for a very cool programming language.

## Installation

1. Download the repository
2. `cd` into the directory
3. `mkdir build && cd build`
3. Run CMake: `cmake ..`
4. Run make: `make`

## Usage

Run `prog.out`. If you are already in the directory, you can run `./prog.out` to do this.

## Testing

1. `cd build`
    - If the directory doens't exist, make it with `mkdir build`
2. `cmake ..`
3. `make tests`
    - Alternatively, you can run `make` instead to make all files which would include the test files
4. `ctest`

