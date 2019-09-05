# Jandu

Jandu is an easy yet enticing game in which you're challenged with building your own little city.
You build your city by placing units next to each other.
Connect 3 or more leaves to create a bush, 3 or more bushes create a tree, and so on.
Can you make it all the way to building a skyscraper before grumpy cats overrun your city?

## Building and running
### Requirements
Make sure you have SDL2 and SDL2_Image installed. These should be in your package manager.
- In Arch Linux you can do: `yay sdl2` and select all the sdl2 entries.
- In Debian, `apt-get install libsdl2-dev libsdl2-image-dev` should do the trick.

### Building
Run `make all` in the root directory. This should create a build directory with an executable in. 

### Running
From the root directory run: `build/jandu`.
To exit the game either press Q or Esc.

## How to
Each turn you get 1 unit you can place on the map. When you can place similar units adjacent to each other they will upgrade to the next level. Try to cluster them together and upgrade all the way to a skyscraper.

You can only place a unit on the dirt tiles. Grass, water and stone are to remain empty.

## Screenshots

![Screenshot1](https://ddd.coder.town/files/jandu.png)

## Credits
Art by [@ctimmerm]( https://github.com/ctimmerm )
