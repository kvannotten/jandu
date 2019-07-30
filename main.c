#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"

int main(int argc, char* args[]) {
  bool won = false;
  Map map;

  loadMap(&map, "foo.dat");

  while(!won) {
    printMap(&map);

    int x, y;
    printf("Enter X coord: ");
    if(scanf("%d", &x) != 1) {
      printf("error: invalid input\n");
      return 1;
    }
    printf("Enter Y coord: ");
    if(scanf("%d", &y) != 1) {
      printf("error: invalid input\n");
      return 1;
    }
    if( x >= FIELD_SIZE || y >= FIELD_SIZE ) {
      printf("error: does not fall within grid\n");
      return 1;
    }
    
    if(isBuildable(&map, x, y)) {
      map.unitField[y][x].level++;
      performUpgrades(&map, x, y);
    } else {
      printf("can't build there\n");
    }
  }

  return 0;
}

