## About This Project
This repository houses the source code for a compiler, the result of a Bachelor's thesis project focused on understanding compiler construction and programming language design.
This was a group project along with [@jonas-bork](https://github.com/jonas-bork), [@sofusl90](https://github.com/sofusl90) and [@Caerullean ](https://github.com/Caerullean). 

The project involved:
1.  Designing a small, custom high-level language.
2.  Implementing a compiler in **C++** that translates this language into **x86-64 Linux assembly**.

The compiler's architecture follows the layered approach (adapted from Appel's "Modern Compiler Implementation"), utilizing the **Boost.Spirit** library for efficient parsing based on Parsing Expression Grammars (PEG).

## Project Goals & Scope

The core aim was to gain practical experience with compiler phases, from scanning and parsing to semantic analysis and code generation, including low-level optimizations like peephole optimization.

**For a  more comprehensive overview of the project scope, please refer to the detailed project statement document:**

➡️ **[View the complete Project Statement](./assets/Bachelor___Project_Statement.pdf)** ⬅️

## Key Implemented Features:

*   **Parsing:** Combines lexical and syntax analysis using [**Boost.Spirit X3**](https://www.boost.org/doc/libs/develop/libs/spirit/doc/x3/html/index.html) and [Parsing Expression Grammars (PEG)](https://en.wikipedia.org/wiki/Parsing_expression_grammar) to generate an Abstract Syntax Tree (AST).
*   **Semantic Analysis:**
    *   Symbol Collection: Manages scopes, variables, functions, and classes.
    *   Type Checking: Verifies type correctness using a producer-consumer scheme on the AST.
*   **Code Generation:**
    *   Translates the AST to a custom Intermediate Representation (IR).
    *   Utilizes **Activation Records** and **Static Linking** to support nested structures and scopes.
*   **Optimizations:**
    *   **Liveness Analysis:** Determines variable lifetimes.
    *   **Peephole Optimization:** Eliminates redundant instructions in the IR.
    *   **Register Allocation:** Implements both a naive stack-based approach and a "smart" method using **Graph Colouring by Simplification** based on liveness information.
*   **Emit Phase:** Converts the optimized IR into final x86-64 assembly code.
*   **Command-Line Interface (CLI):** Built using [**Boost.Program_options**](https://www.boost.org/doc/libs/1_88_0/doc/html/program_options.html) to provide flags for controlling compiler behaviour (e.g., stopping after specific phases, disabling peephole optimization, selecting register allocation strategy), facilitating easy testing and benchmarking.
*   **Cross-Platform Build System (CMake):** Utilized **CMake** to create a robust build system, automating compilation and linking dependencies (like Boost) across both **Linux** and **macOS**.
*   **Runtime Checks:** Includes checks for accessing `beta` (null-equivalent) values for arrays.
*   **Testing:** Developed with a comprehensive test suite using [Boost.Test](https://www.boost.org/doc/libs/1_88_0/libs/test/doc/html/index.html) and automated testing via GitHub Actions within a Docker environment.

## Full Thesis Document

For a complete understanding of the language design, compiler architecture, implementation details, optimizations, and benchmarks, please refer to the full Bachelor thesis document:

➡️ **[Read the full Bachelor Thesis](./assets/Thesis_Giga_Chad_Compiler.pdf)** ⬅️

> [!IMPORTANT]
> Please note that the git commit history might not fully reflect the individual contributions, as we pair programmed using the [VS Code Live Share extension](https://marketplace.visualstudio.com/items/?itemName=MS-vsliveshare.vsliveshare).

---

# Bachelor - Giga Chad Compiler

<p align="center">
  <img src="assets/Header.png" alt="header" width="100%" />
</p>


<p align="center">
  <img src="assets/2.jpg" alt="Image 2" width="49%" />
  <img src="assets/3.jpg" alt="Image 3" width="49%" />
</p>

#### Design Philosophy: 
- **Effortless Superiority**: Why struggle with other languages when you can dominate with Giga Chad? It's not just a language; it's a power move. Designed for those who write code not just to solve problems, but to flex on runtime inefficiencies and syntax complexities.

- **Brute Simplicity**: Giga Chad doesn't do 'complex' or 'verbose'. If a task needs more than 10 lines of .chad code, you're clearly not using Giga Chad to its full potential. Our motto: "Why be complicated when you can be Giga Chad?"

- **Unapologetic Speed**: Slow compilation? Never heard of her. Giga Chad compiles at the speed of your ambition, leaving other languages in the dust. Perfect for coding at 3 AM and still getting eight hours of sleep because, let's face it, even Giga Chads need their beauty rest.


## Installation

1. Download the repository
2. `cd` into the directory
3. `mkdir build && cd build`
3. Run CMake: `cmake ..`
4. Run make: `make`

## Usage

Run `giga`. If you are already in the directory, you can run `./giga` to do this.

## Contributing

This section will describe how to contribute to GCC.

### Testing

After making changes, make sure that it works by running the tests:
1. `cd build`
    - If the directory doens't exist, make it with `mkdir build`
2. `cmake ..`
3. `make tests`
    - Alternatively, you can run `make` instead to make all files which would include the test files
4. `ctest`

### Naming Scheme

The naming scheme that should be used when contributing to GCC is:
- Variables: snake\_case
- Functions: camelCase
- Classes, enums, etc.: PascalCase

### Docker

This guide describes how to use Docker with a mounted volume, allowing you to run tests on an Ubuntu machine as if you were working directly on Ubuntu, regardless of your actual host operating system.

#### Starting an Interactive Shell with Docker

> [!CAUTION]
> It's crucial to be in the **root directory of your project** when following these steps, especially for the `$(pwd)` command in step 4 to work correctly. This command relies on your current directory path to mount the project into the Docker container.

1. **Preparation**: Ensure you're in the root directory of your project where the Dockerfile is located.
2. **Clear Cache**: Before running your Docker container, it's recommended to clear your build directory to avoid any CMake cache issues. This ensures a fresh environment for each build:
```bash
rm -rf ./build/*
```

3. **Build your image**: 
Before you can run your container, you need to build your Docker image. Make sure your Dockerfile is in the root directory of your project and run:
```bash
docker build -t my_image .
```

4. **Mount Project Through Docker**: To run your Docker container with your project directory mounted (ensuring any changes on your host are directly reflected in the container), execute the following command:
```bash
docker run --rm -it -v $(pwd):/app -w /app my_image
```
This starts an interactive shell session within the Docker container. From here, you can proceed to build and test your project:
```bash 
mkdir build && cd build
cmake .. && make && ctest 
``` 
Following this procedure, you can access any generated log files directly under build/Testing/Temporary/, both from within the container and from your host machine.

5. **Stopping the Container**: To exit the interactive shell and stop the container, simply type ``exit`` or press ``Ctrl+D``. If you started the container with the --rm flag as suggested in step 4, Docker will automatically remove the container once it stops, ensuring your system remains clean and clutter-free.
