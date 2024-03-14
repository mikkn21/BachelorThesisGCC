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

Run `prog.out`. If you are already in the directory, you can run `./prog.out` to do this.

## Testing

1. `cd build`
    - If the directory doens't exist, make it with `mkdir build`
2. `cmake ..`
3. `make tests`
    - Alternatively, you can run `make` instead to make all files which would include the test files
4. `ctest`

## Docker

This guide describes how to use Docker with a mounted volume, allowing you to run tests on an Ubuntu machine as if you were working directly on Ubuntu, regardless of your actual host operating system.

### Starting an Interactive Shell with Docker

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