#include <stdbool.h>
#include "map.h"

bool SDLInit();

void SDLClose();

void SDLDraw(Map*);

void SDLListenForEvents(Map*,bool*);

bool loadMedia();
