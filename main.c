#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"

int main(int argc, char* args[]) {
  if(!SDLInit()) {
    printf("Failed to initialize\n");
    return 33;
  }

  bool quit = false;
  Map map;

  loadMap(&map, "foo.dat");

  while(!quit) {
    SDLDraw(&map);
    SDLListenForEvents(&map, &quit);
  }

  SDLClose();

  return 0;
}

