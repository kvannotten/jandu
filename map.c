#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "vector.h"

void checkConnected(Map* map, Coord coord, Vector* coords);

void performUpgrades(Map* map, int x, int y) {
  Vector coords;
  init(&coords);
  append(&coords, (Coord){.x = x, .y = y});

  Coord coord = get(&coords, 0);

  checkConnected(map, coord, &coords);

  int length = size(&coords);
  if (length >= 3)  {
    if(map->unitField[y][x].level == LVL_SEVEN) {
      // no more upgrades to do
      return;
    }
    map->unitField[y][x].level++;

    for(int i = 0; i < length; i++) {
      Coord c = get(&coords, i);
      if(c.x != x || c.y != y) {
        map->unitField[c.y][c.x].level = NONE;
      }
    }
    // once an upgrade has happened, we have to 
    // check if there's more upgrades to be done
    performUpgrades(map, x, y);
  }

  free_memory(&coords);
}

Coord travel[] = {
  {.x = -1, .y = 0},
  {.x = 1, .y = 0},
  {.x = 0, .y = -1},
  {.x = 0, .y = 1}
};

void checkConnected(Map* map, Coord coord, Vector* coords) {
  int loops = sizeof(travel) / sizeof(travel[1]);
  Unit currentUnit = map->unitField[coord.y][coord.x];
  for (int i = 0; i < loops; i++) {
    int x = coord.x + travel[i].x;
    int y = coord.y + travel[i].y;

    if(x < 0 || x >= FIELD_SIZE || y < 0 || y >= FIELD_SIZE) {
      continue;
    }

    if(map->unitField[y][x].level != currentUnit.level) {
      continue;
    }

    if(find_value(coords, (Coord){.x = x, .y = y}) != -1) {
      continue;
    }

    if(map->unitField[y][x].level == currentUnit.level) {
      Coord c = { .x = x, .y  = y };
      append(coords, c);

      checkConnected(map, c, coords);
    }
  }
}

void printMap(Map* map) {
  for(int i = 0; i < FIELD_SIZE; i++) {
    for(int j = 0; j < FIELD_SIZE; j++) {
      putchar(map->field[i][j] + '0');
      putchar(' ');
    }
    putchar('\n');
  }


  for(int i = 0; i < FIELD_SIZE; i++) {
    for(int j = 0; j < FIELD_SIZE; j++) {
      if(map->unitField[i][j].level == NONE) {
        printf(" _( ) ");
      } else {
        printf(" %d(%d) ", map->unitField[i][j].level, map->unitField[i][j].type);
      }
    }
    printf("\n");
  }
}

TileType convertStringToTiletype(char* pch) {
  TileType t = DIRT;
  if (strncmp(pch, "G", 1) == 0) {
    t = GRASS;
  } else if(strncmp(pch, "S", 1) == 0) {
    t = STONE;
  } else if(strncmp(pch, "W", 1) == 0) {
    t = WATER;
  }

  return t;
}

void loadMap(Map* map, char* file) {
  FILE* fp = NULL;
  char line[256];

  fp = fopen(file, "r");
  if(fp == NULL ) {
    printf("Could not read level");
    exit(EXIT_FAILURE);
  }

  int counter = 0;
  size_t length = 0;
  while (fgets(line, sizeof(line), fp)) {
    length = strlen(line);
    if(line[length -1] == '\n') {
      line[--length] = '\0';
    }

    char* pch;
    pch = strtok(line, " ");

    while(pch != NULL) {
      TileType tmp = convertStringToTiletype(pch);
      map->field[counter / FIELD_SIZE][counter % FIELD_SIZE] = tmp;

      counter++;

      pch = strtok(NULL, " ");
    }

    if(pch != NULL) {
      free(pch);
    }
  }

  fclose(fp);

  for(int i = 0; i < FIELD_SIZE; i++) {
    for(int j = 0; j < FIELD_SIZE; j++) {
      map->unitField[i][j] = (Unit){ .type = PROGRESSION_PLANTS, .level = NONE };
    }
  }
}

bool isBuildable(const Map* map, int x, int y) {
  return map->field[y][x] == DIRT && map->unitField[y][x].level == NONE;
}
