# DoublePendulum

A physics simulator for a double pendulum using SDL2. Assumes a point mass at the end of the links following the equations of motions from the Modern Robotics textbook by Kevin Lynch.

## Installation

This repository is a Visual Studio solution and is self-contained with the SDL2 static library build.

```sh
git clone https://github.com/benvonh/DoublePendulum
```

Open the solution and run in Visual Studio.

## Usage

Keybindings:

- `0` / `r`: reset joints to zero angles
- `<space>`: pause simulation (but not renderer)
- `q`: quit application
