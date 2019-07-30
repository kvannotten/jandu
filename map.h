#include <stdbool.h>

#define FIELD_SIZE 6

typedef enum {
  NONE,
  LVL_ONE,
  LVL_TWO,
  LVL_THREE,
  LVL_FOUR,
  LVL_FIVE,
  LVL_SIX,
  LVL_SEVEN
} UnitLevel;

typedef enum {
  PROGRESSION_PLANTS
} UnitType;

typedef struct {
  UnitLevel level;
  UnitType type;
} Unit;

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
