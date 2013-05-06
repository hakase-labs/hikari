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

## Why is it called "Hikari"? ##

"Hikari" is the Japanese word for "light". Since [Dr. Light][1] is the creator of Mega Man, we figured that a word associated with him would be an appropriate code name for a project like this. Originally "hikari" was going to be the name of the engine used to power the game, but focus has shifted away from a separate engine/game architecture.

[1]: http://megaman.wikia.com/wiki/Dr._Light
[2]: http://en.wikipedia.org/wiki/Mega_Man
