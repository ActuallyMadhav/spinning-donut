# Spinning Donut

A small collection of programs that draw a rotating ASCII "donut" in the terminal using basic trigonometry and shading.

The project contains three implementations:

* `mainInC.c`       – plain C version (no external libraries required).  
* `mainInCPP.cpp`   – simple C++ port using `std::vector`.  
* `mainOptimized.cpp` – C++ version that uses [GLM](https://github.com/g-truc/glm) for vector/matrix math and demonstrates a few optimisations.

All programs continuously render a 22×80 character buffer with a lit torus that spins around two axes.  Brightness is simulated by selecting characters from a gradient (`.,-~:;=!*#$@`).

## Prerequisites

* macOS, Linux or other POSIX‑compatible terminal.  
* `clang`/`gcc` or another C/C++ compiler.  
* For `mainOptimized.cpp` you will need a header‑only [GLM](https://github.com/g-truc/glm) installation.  On macOS with Homebrew this can be obtained via `brew install glm`.

## Build & Run

### C version

```sh
clang mainInC.c -o donut_c
./donut_c
```

### C++ version

```sh
clang++ mainInCPP.cpp -o donut_cpp
./donut_cpp
```

### Optimized C++ version (GLM)

```sh
clang++ mainOptimized.cpp -o donut_opt \
    -I/opt/homebrew/Cellar/glm/1.0.3/include   # path may vary

./donut_opt
```

> **Tip:** you can adjust the resolution, brightness characters, sleep time, etc. by editing the source files.

## Code Overview

Each implementation performs the following steps inside a double loop over the torus angles `theta` and `phi`:

1. Generate 3‑D coordinates of a point on the torus.
2. Rotate the point around the X and Z axes.
3. Project the 3‑D point onto the 2‑D screen plane.
4. Scale and shift the coordinates to screen space.
5. Compute a simple Lambertian shading value using a fixed light direction.
6. Choose a character from the gradient based on brightness and store it in a buffer.
7. After iterating through the surface, print the buffer and repeat with updated rotation angles.

The optimized version uses `glm::vec3`, `glm::rotate`, `glm::normalize`, and `glm::dot` to reduce boilerplate and experiment with step sizes.

## License

This repository is provided for educational purposes – feel free to fork and modify.

Enjoy the spinning donut! 🍩
