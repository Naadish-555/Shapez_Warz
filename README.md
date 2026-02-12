
# Poly Wars

> A 2D geometry shooter built from scratch using a custom Entity Component System (ECS).

## Overview

This project explores **Game Engine Architecture** and **Memory Management** in C++. It implements a custom engine from the ground up, moving away from traditional OOP inheritance in favor of a data-oriented **Entity Component System (ECS)**. The simulation manages entity lifecycles, collision detection, and rendering pipelines manually.

## Key Features

**Custom ECS Architecture:** Complete separation of Data (Components), Logic (Systems), and Identity (Entities).
**Physics & Collision:** Custom vector math implementation for movement, recoil, and bounding-circle collision detection.
**Data-Driven Design:** Game variables (speed, spawn rates, window size) are loaded from an external configuration file.
**Special Mechanics:** Implemented weapon cooldowns, special attack **(WIP)**, and fragment-based destruction effects.

## Controls

Input mapping for the simulation:

| Input | Action | Function |
| --- | --- | --- |
| **W, A, S, D** | Movement | Updates Velocity Component |
| **Mouse Cursor** | Aim | Updates Transform Angle |
| **Left Click** | Fire | Spawns Bullet Entity |
| **Right Click** | Special **(WIP)** | Triggers Special Weapon System |
| **P** | Pause | Toggles Game State |

### Prerequisites

* [CMake](https://cmake.org/download/) (3.16 or newer)
* A C++17 compatible compiler (MSVC, GCC, or Clang)

### 1. Windows (Visual Studio)

1. Open Visual Studio.
2. Select **"Open a Local Folder"** and choose this project directory.
3. Wait for CMake to configure (check the Output window).
4. **Important:** Switch the build configuration dropdown (top bar) to **Release** or **RelWithDebInfo**.
* *Note: Debug mode includes safety checks that significantly reduce performance.*


5. Select `PolyWars.exe` from the run dropdown menu.
6. Press **Ctrl + F5** to build and run.

### 2. macOS & Linux (Command Line)

**Prerequisites:**
You do NOT need to install SFML manually (it is fetched automatically).
However, you need the system tools (compilers and dependencies) to build it from source.

* **Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake build-essential libxrandr-dev libxcursor-dev libudev-dev libopenal-dev libflac-dev libvorbis-dev libgl1-mesa-dev libfreetype6-dev

```


* **Fedora:**
```bash
sudo dnf install cmake make gcc-c++ systemd-devel libX11-devel libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel mesa-libGL-devel alsa-lib-devel

```


* **macOS:**
```bash
brew install cmake

```



**Build Steps:**

1. Open a terminal in the project directory.
2. Create a build folder:
```bash
mkdir build && cd build

```


3. Configure with CMake (Release mode is critical for performance):
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..

```


4. Build the project:
```bash
make

```


5. Run the executable:
```bash
./PolyWars

```



## Credits

* Special thanks to **Dave Churchill** and his lectures.
* **Graphics Library:** Built using [SFML](https://www.sfml-dev.org/).
* **Template:** SFMLCmakeSetup by [Meemknight](https://github.com/meemknight).
* **Inspiration:** Mechanics inspired by *Geometry Wars*.