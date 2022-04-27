# VE 3D Viewer

```
@Author: 	chenghua.wang caibo.feng yue.wu
@Vision:	0.0.1
```

VE(Violet-Evergarden) is a 3D viewer made with love and passion. We borrowed the name of the light novel Violet Evergarden for this viewer. This Project is also a final-project for software engineering course. However, I think this project has nothing to do with software engineering :-)

Some of you may found that A repo called "Morpheus" that i've deleted is the predecessor of this project. "Morpheus" is more of an experimental project. The main difference is, we added a opengl implement of renderer instead of using the most primitive matrix operation method to simulate it directly. We made the interactive interface more friendly, and referred to the implementation of some lightweight engines such as "Overload", "Pilot", etc.

## How this project is organized

Our 3D viewer is actually consists of two parts: 1. a launcher "Violet", whitch responsible for checking program running files(Such as .dll on windows, and fonts, shaders) and indexing project files(.json file). 2. an editor "Evergarden", responsible for rendering and editing the scene.

### Violet

This Launcher is quite simple. It will load a config.json file first to determine whether the files we need exist and whether these files are damaged. Then, you can choose a .json file to open a Scene(We orgnized our 3d Scene in a .json file). After you choose the scene you want to open, this launcher will exec "Evergarden" to render it.

### Evergarden

Not implemented yet.

## Dependence

Use the git script below.

```shell
git submodule update --init --recursive
```

### Windows 10/11

* Visual Studio 2019 or 2022
* CMake 3.20 +

## Install

Make sure you have pre-install all dependence already.

This Project is fully tested on windows10 and 11. If you want to install it on Ubuntu, you may need to replace a little system based functions our macros we used.

Install this Project is quite simple. Follow the guide bellow.

```shell
mkdir build
cd build
cmake ..
cmake --build
```

 That's done, the debug vision will be installed into build directory.
