#include <stdbool.h>

/* Units:
 * =======
 *    PLANTS   CATS      SPECIAL
 * 1   LEAF     CAT       STAR
 * 2   BUSH     GRAVE     BOMB
 * 3   TREE     STATUE
 * 4   HUT      COIN 
 * 5   HOUSE    TREASURE
 * 6   VILLA    L TREASURE
 * 7   TOWER
 *
 * Notes: 
 * =======
 * - LVL_ONE CATS move around of there is space,
 * they upgrade to LVL_TWO when they have no more space.
 * - The BOMB unit is placed on top of another unit and will
 *   remove it
 * - The star unit, if placed next to 2 other similar units
 *   turns into the upgrade of those similar units. If it's placed
 *   with only 1 or 0 units adjacent to it, it turns into a leaf.
 */

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
  PROGRESSION_PLANTS,
  PROGRESSION_CATS,
  SPECIAL
} UnitType;

typedef struct {
  UnitLevel level;
  UnitType type;
} Unit;

Unit randomUnit();
bool isCat(const Unit*);
