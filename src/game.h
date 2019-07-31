#include <stdbool.h>
#include "map.h"

bool SDLInit();

void SDLClose();

void SDLDraw(const Map*);

void SDLHandleUserEvents(Map*,bool*);

bool loadMedia();
