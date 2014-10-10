[![Build Status](https://travis-ci.org/zackthehuman/hikari.svg?branch=master)](https://travis-ci.org/zackthehuman/hikari)

![Hikari](http://zackthehuman.com/images/hikari/hikari-logo.png)

Project Hikari
==============

Project Hikari is the code name for a yet-to-be-named open-source Mega Man/Rockman game written in C++. The goal of this project is to create a faithful clone of the classic NES title ["Mega Man"][2] ("Rockman" in Japan). The feature set of the game consists of elements from various Mega Man titles, but is strongly influenced by Mega Man 3.

Gameplay and feature videos can be seen on [hakaselabshikari's YouTube page][7].

## Customization & Extension ##

The game is designed to be as data-driven as possible and as such it allows for customization and extension.

* Animations (sprites) are defined in `JSON` files
* Items, Enemies, Projectiles, etc. are defined in `JSON` files
* Item and Enemy behaviors are scriptable using [Squirrel][6]
* Maps/stages are defined in `JSON` files
* Music and sound effects are played from `NSF` files, and multiple NSF files can be used to mix and match music from different files

Most things are customizable. Not _everything_, but most things.

## Building ##

Project Hikari uses [CMake][3] to generate platform and compiler-specific build files.

### Dependencies ###

Before building Hikari, make sure that the following libraries are compiled and installed:

* [SFML 2.0+][4] (also tested with 2.1)
* [PhysicsFS][5]

Also note that this project uses `C++11` features such as `auto`, `decltype`, and lambdas, and therefore requires a C++11 compliant compiler in order to build successfully.

### Building on Windows (VS2010+) ###

1. Ensure SFML and PhysicsFS are installed.
2. Clone the repository.

        git clone https://github.com/zackthehuman/hikari.git hikari

3. Generate the build files.

        mkdir hikari-build
        cd hikari-build
        cmake -G "Visual Studio 10" ../hikari

4. Open the `.sln` in Visual Studio and build the `ALL_BUILD` target.

### Building on Linux (Makefile) ###

1. Ensure SFML and PhysicsFS are installed.
2. Clone the repository.

        git clone https://github.com/zackthehuman/hikari.git hikari

3. Generate the build files.

        mkdir hikari-build
        cd hikari-build
        cmake -G "Unix Makefiles" ../hikari

4. Build the application.

        make

### Building on Mac (Makefile) ###

1. Ensure SFML and PhysicsFS are installed. Note that SFML must be built with the same version of `libc++` as Hikari, otherwise you'll run into linker errors. The generated build files will try to define `-stdlib=libc++` when compiling and linking on a Mac.
2. Clone the repository.

        git clone https://github.com/zackthehuman/hikari.git hikari

3. Generate the build files.

        mkdir hikari-build
        cd hikari-build
        cmake -G "Unix Makefiles" ../hikari

4. Build the application.

        make

## Why is it called "Hikari"? ##

"Hikari" is the Japanese word for "light". Since [Dr. Light][1] is the creator of Mega Man, we figured that a word associated with him would be an appropriate code name for a project like this. Originally "hikari" was going to be the name of the engine used to power the game, but focus has shifted away from a separate engine/game architecture.

## Are you actually making a clone of Mega Man 3? ##

No. While the mechanics of the game are very similar to Mega Man 3, this project is intended to be an original work and not an exact clone of any Mega Man title.

[1]: http://megaman.wikia.com/wiki/Dr._Light
[2]: http://en.wikipedia.org/wiki/Mega_Man
[3]: http://www.cmake.org/
[4]: http://www.sfml-dev.org/
[5]: http://icculus.org/physfs/downloads/
[6]: http://squirrel-lang.org
[7]: https://www.youtube.com/user/hakaselabshikari/videos
