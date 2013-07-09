Project Hikari
==============

Project Hikari is the code name for a yet-to-be-named open-source Mega Man/Rockman game clone written in C++. The goal of this project is to create a faithful clone of the classic NES title ["Mega Man"][2] ("Rockman" in Japan). The feature set of the game consists of elements from various Mega Man titles, but is strongly influenced by Mega Man 3.

![Early build of the game](http://zackthehuman.com/images/hikari/screenshot.png)

## Customization & Extension ##

The game is designed to be as data-driven as possible and as such it allows for cusomization and extension:

* Animations are `JSON` structures
* Items, Enemies, Projectiles, etc are `JSON` structures
* Behavior of Items and Enemies is controlled by `Squirrel` scripts
* Stages are `JSON` structures
* Music and sound effects are `NSF` files

Most things are customizable. Not _everything_, but most things.

## Building ##

Project Hikari uses [CMake][3] to generate platform and compiler-specific build files.

### Dependencies ###

Before building Hikari, make sure that the following libraries are compiled and installed:

* [SFML][4]
* [PhysicsFS][5]

### Building on Windows (VS2010) ###

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

## Why is it called "Hikari"? ##

"Hikari" is the Japanese word for "light". Since [Dr. Light][1] is the creator of Mega Man, we figured that a word associated with him would be an appropriate code name for a project like this. Originally "hikari" was going to be the name of the engine used to power the game, but focus has shifted away from a separate engine/game architecture.

[1]: http://megaman.wikia.com/wiki/Dr._Light
[2]: http://en.wikipedia.org/wiki/Mega_Man
[3]: http://www.cmake.org/
[4]: http://www.sfml-dev.org/
[5]: http://icculus.org/physfs/downloads/
