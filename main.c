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

  bool won = false;
  Map map;

  loadMap(&map, "foo.dat");

  while(!won) {
    SDLDraw(&map);
  }

  SDLClose();

  return 0;
}

