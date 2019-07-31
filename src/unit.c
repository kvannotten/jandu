#include <stdlib.h>
#include "unit.h"

Unit randomUnit() {
  int r = rand() % 100;

  if (r <= 75) {
    return (Unit){.level = LVL_ONE, .type = PROGRESSION_PLANTS };
  } else if(r <= 85) {
    return (Unit){.level = LVL_ONE, .type = PROGRESSION_CATS};
  } else if(r <= 95) {
    return (Unit){.level = LVL_TWO, .type = PROGRESSION_PLANTS };
  } else if(r <= 98) {
    return (Unit){.level = LVL_THREE, .type = PROGRESSION_PLANTS };
  } else {
    return (Unit){.level = LVL_FOUR, .type = PROGRESSION_PLANTS };
  }
}

bool isCat(const Unit* unit) {
  return unit->level == LVL_ONE && unit->type == PROGRESSION_CATS;
}
