#include "game.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClips[STONE + 1];
SDL_Rect gSpriteTrees[LVL_SEVEN + 1];

typedef struct {
  SDL_Texture* texture;

  int width;
  int height;
} Texture;

Texture gSpriteSheetTexture;

void render(Texture* texture, int x, int y, SDL_Rect* clip);

bool SDLInit() {
  srand(time(NULL));

  if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    printf("SDL could not be initialized. Error: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    printf("Warning: linear texture filtering not enabled");
  }

  gWindow = SDL_CreateWindow("Jandu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_SHOWN);
  if(gWindow == NULL) {
    printf("Window could not be created. Error: %s\n", SDL_GetError());
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if(gRenderer == NULL) {
    printf("Renderer could not be created. Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not be initialized. Error: %s\n", IMG_GetError());
    return false;
  }

  if(!loadMedia()) {
    printf("Could not load media.\n");
    return false;
  }

  return true;
}

void freeTexture(Texture* texture) {
  if (texture != NULL && texture->texture != NULL) {
    SDL_DestroyTexture(texture->texture);
    texture->texture = NULL;
    texture->width = 0;
    texture->height = 0;
  }
}

bool loadFromFile(Texture* texture, char* path) {
  // first make sure we start from scratch
  freeTexture(texture);

  SDL_Surface* loadedSurface = IMG_Load(path);
  if(loadedSurface == NULL) {
    printf("Unable to load image %s! Error: %s\n", path, IMG_GetError());
    return false;
  }

  SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

  texture->texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
  if (texture->texture == NULL) {
    printf("Unable to create texture from %s. Error: %s\n", path, SDL_GetError());
    return false;
  }

  texture->width = loadedSurface->w;
  texture->height = loadedSurface->h;

  SDL_FreeSurface(loadedSurface);

  return texture->texture != NULL;
}

void render(Texture* texture, int x, int y, SDL_Rect* clip) {
  SDL_Rect renderQuad = {x, y, texture->width, texture->height};

  if (clip != NULL) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  SDL_RenderCopy(gRenderer, texture->texture, clip, &renderQuad);
}

bool loadMedia() {
  if (!loadFromFile(&gSpriteSheetTexture, "sprites-hd.png")) {
    printf("Failed to load spritesheet.\n");
    return false;
  }
  // tiles
  gSpriteClips[GRASS].x = 118;
  gSpriteClips[GRASS].y = 186;
  gSpriteClips[GRASS].w = 108;
  gSpriteClips[GRASS].h = 132;

  gSpriteClips[DIRT].x = 110;
  gSpriteClips[DIRT].y = 362;
  gSpriteClips[DIRT].w = 108;
  gSpriteClips[DIRT].h = 128;

  gSpriteClips[WATER].x = 228;
  gSpriteClips[WATER].y = 268;
  gSpriteClips[WATER].w = 108;
  gSpriteClips[WATER].h = 128;

  gSpriteClips[STONE].x = 126;
  gSpriteClips[STONE].y = 0;
  gSpriteClips[STONE].w = 108;
  gSpriteClips[STONE].h = 128;

  // units
  gSpriteTrees[LVL_ONE].x = 126;
  gSpriteTrees[LVL_ONE].y = 130;
  gSpriteTrees[LVL_ONE].w = 86;
  gSpriteTrees[LVL_ONE].h = 52;

  gSpriteTrees[LVL_TWO].x = 214;
  gSpriteTrees[LVL_TWO].y = 130;
  gSpriteTrees[LVL_TWO].w = 66;
  gSpriteTrees[LVL_TWO].h = 52;

  gSpriteTrees[LVL_THREE].x = 338;
  gSpriteTrees[LVL_THREE].y = 268;
  gSpriteTrees[LVL_THREE].w = 96;
  gSpriteTrees[LVL_THREE].h = 124;

  gSpriteTrees[LVL_FOUR].x = 346;
  gSpriteTrees[LVL_FOUR].y = 130;
  gSpriteTrees[LVL_FOUR].w = 68;
  gSpriteTrees[LVL_FOUR].h = 86;

  gSpriteTrees[LVL_FIVE].x = 522;
  gSpriteTrees[LVL_FIVE].y = 0;
  gSpriteTrees[LVL_FIVE].w = 94;
  gSpriteTrees[LVL_FIVE].h = 106;

  gSpriteTrees[LVL_SIX].x = 0;
  gSpriteTrees[LVL_SIX].y = 186;
  gSpriteTrees[LVL_SIX].w = 116;
  gSpriteTrees[LVL_SIX].h = 90;

  gSpriteTrees[LVL_SEVEN].x = 0;
  gSpriteTrees[LVL_SEVEN].y = 0;
  gSpriteTrees[LVL_SEVEN].w = 124;
  gSpriteTrees[LVL_SEVEN].h = 184;

  return true;
}

const int offset_x = 200;
const int offset_y = 75;
Unit currentUnit = {.level = LVL_ONE, .type = PROGRESSION_PLANTS };

void SDLDraw(Map* map) {
  SDL_RenderClear(gRenderer);

  for(int i = 0; i < FIELD_SIZE; i++) {
    for(int j = 0; j < FIELD_SIZE; j++) {
      TileType tt = map->field[j][i];
      render(&gSpriteSheetTexture, offset_x + i*gSpriteClips[tt].w, offset_y + j*gSpriteClips[tt].h - j*38, &gSpriteClips[tt]);  
      Unit u = map->unitField[j][i];
      if(u.level > NONE) {
        int unitX = floor(offset_x + i*gSpriteClips[tt].w + gSpriteClips[tt].w/2 - gSpriteTrees[u.level].w/2);
        int unitY = floor(offset_y + j*gSpriteClips[tt].h - 38*j);
        if(u.level < LVL_THREE) {
          unitY += gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h;
        } else if(u.level == LVL_THREE){
          unitY -= gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h/4;
        } else if(u.level == LVL_SEVEN) {
          unitY -= gSpriteClips[tt].h;
        } else {
          unitY -= gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h/2;
        }
        render(&gSpriteSheetTexture, unitX, unitY, &gSpriteTrees[u.level]);  
      }
    }
  }

  render(&gSpriteSheetTexture, 0, 0, &gSpriteTrees[currentUnit.level]);

  SDL_RenderPresent(gRenderer);
}

Unit randomUnit() {
  int r = rand() % 100;
  if (r <= 80) {
    return (Unit){.level = LVL_ONE, .type = PROGRESSION_PLANTS };
  } else if(r <= 95) {
    return (Unit){.level = LVL_TWO, .type = PROGRESSION_PLANTS };
  } else if(r <= 98) {
    return (Unit){.level = LVL_THREE, .type = PROGRESSION_PLANTS };
  } else {
    return (Unit){.level = LVL_FOUR, .type = PROGRESSION_PLANTS };
  }
}

void SDLListenForEvents(Map* map, bool* quit) {
  SDL_Event e;

  if(SDL_WaitEvent(&e) != 0) {
    int x, y;
    int coordx, coordy;
    switch(e.type) {

    case SDL_MOUSEBUTTONDOWN:
      SDL_GetMouseState(&x, &y);
      // convert mouse coordinates to grid coordinates
      coordx = floor((x-offset_x -1)/gSpriteClips[0].w);
      coordy = floor((y-offset_y)/(gSpriteClips[0].h - 38));

      if(isBuildable(map, coordx, coordy)) {
        map->unitField[coordy][coordx] = currentUnit;
        performUpgrades(map, coordx, coordy);
        currentUnit = randomUnit();
      }
      break;

    case SDL_KEYDOWN:
      if(e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE) {
        *quit = true;
      } else if(e.key.keysym.sym == SDLK_p) {
        printMap(map);
      }
      break;

    }
  }
}

void SDLClose() {
  freeTexture(&gSpriteSheetTexture);

  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  IMG_Quit();
  SDL_Quit();
}
