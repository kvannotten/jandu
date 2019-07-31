#include <stdbool.h>
#include "unit.h"

#define FIELD_SIZE 6

typedef enum {
  GRASS,
  DIRT,
  WATER,
  STONE,
} TileType;

typedef struct {
  TileType field[FIELD_SIZE][FIELD_SIZE];
  Unit unitField[FIELD_SIZE][FIELD_SIZE];
} Map;

/*
typedef struct {
  int x;
  int y;
} Coord;
*/


void printMap(Map*);
void loadMap(Map*, char*);
bool isBuildable(const Map*, int, int);
void performUpgrades(Map*, int, int);
bool placeUnit(Map*, const Unit*, int, int);
