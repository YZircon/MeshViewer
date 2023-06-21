# MeshViewer

A C++/OpenGL Graphics Toy, it might be useful when you learning Computer Graphics

## Features (Working now)

**Model**

.obj/.mtl

**Rendering**

- [x] Blinn-Phong

## Build & Run

This project is configured for Windows 10 64 bit & CLion 2022.3.1. If you are using a highly differentiated platform (Such as Linux or MacOS) or toolchain (Such as MSVC), the `CMakeLists.txt` and other 3rd packages may not work properly.

- `mkdir build && cd build`  ("&&" operator need Windows PowerShell 7 or higher version, If your PowerShell not support it, you may split this command into `mkdir build` and `cd build`)
- `cmake -G Ninja ..`
- `ninja`

Then, you'll find a executable file named `MeshViewer.exe` and a Dynamic Linking Library named `glfw3.dll` in the `build` folder.  This is  what you have build.

Currently, the shader & resource path is hard-coded in `main.cpp` , to make the program run correctly, the architecture of your directory should be like the below:

- MeshViewer (any folder name you like will also be ok)
  - MeshViewer.exe
  - glfw3.dll
  - models (hard-coded now, so do not modify the name of directory and files )
  - src
    - shader (hard-coded now, so do not modify the name of directory and files )

## Resources

### 3rd Party Libraries

#### GLAD

#### GLFW

#### Eigen

#### OBJ-Loader

#### stb

**Dear ImGui**
