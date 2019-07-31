#include "game.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

SDL_Rect gSpriteClips[STONE + 1];
SDL_Rect gSpriteTrees[LVL_SEVEN + 1];
SDL_Rect gSpriteCats[LVL_SEVEN + 1];

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

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

  gWindow = SDL_CreateWindow("Jandu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if(gWindow == NULL) {
    printf("Window could not be created. Error: %s\n", SDL_GetError());
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
  gSpriteClips[GRASS]     = (SDL_Rect){.x = 118, .y = 186, .w = 108, .h = 132};
  gSpriteClips[DIRT]      = (SDL_Rect){.x = 110, .y = 362, .w = 108, .h = 128};
  gSpriteClips[WATER]     = (SDL_Rect){.x = 228, .y = 268, .w = 108, .h = 128};
  gSpriteClips[STONE]     = (SDL_Rect){.x = 126, .y = 0, .w = 108, .h = 128};

  // tree units
  gSpriteTrees[LVL_ONE]   = (SDL_Rect){.x = 126, .y = 130, .w = 86, .h = 52};
  gSpriteTrees[LVL_TWO]   = (SDL_Rect){.x = 214, .y = 130, .w = 66, .h = 52};
  gSpriteTrees[LVL_THREE] = (SDL_Rect){.x = 338, .y = 268, .w = 96, .h = 124};
  gSpriteTrees[LVL_FOUR]  = (SDL_Rect){.x = 346, .y = 130, .w = 68, .h = 86};
  gSpriteTrees[LVL_FIVE]  = (SDL_Rect){.x = 522, .y = 0, .w = 94, .h = 106};
  gSpriteTrees[LVL_SIX]   = (SDL_Rect){.x = 0, .y = 186, .w = 116, .h = 90};
  gSpriteTrees[LVL_SEVEN] = (SDL_Rect){.x = 0, .y = 0, .w = 124, .h = 184};

  // cat units
  gSpriteCats[LVL_ONE]    = (SDL_Rect){.x = 428, .y = 114, .w = 96, .h = 110};
  gSpriteCats[LVL_TWO]    = (SDL_Rect){.x = 96, .y = 492, .w = 84, .h = 88};
  gSpriteCats[LVL_THREE]    = (SDL_Rect){.x = 428, .y = 0, .w = 92, .h = 112};
  gSpriteCats[LVL_FOUR]    = (SDL_Rect){.x = 338, .y = 394, .w = 92, .h = 102};
  gSpriteCats[LVL_FIVE]    = (SDL_Rect){.x = 228, .y = 184, .w = 116, .h = 82};
  gSpriteCats[LVL_SIX]    = (SDL_Rect){.x = 0, .y = 278, .w = 116, .h = 82};

  return true;
}

const int offset_x = 200;
const int offset_y = 75;
Unit currentUnit = {.level = LVL_ONE, .type = PROGRESSION_PLANTS };

void SDLDraw(const Map* map) {
  SDL_RenderClear(gRenderer);

  for(int i = 0; i < FIELD_SIZE; i++) {
    for(int j = 0; j < FIELD_SIZE; j++) {
      // draw playing field tile
      TileType tt = map->field[j][i];
      render(&gSpriteSheetTexture, offset_x + i*gSpriteClips[tt].w, offset_y + j*gSpriteClips[tt].h - j*38, &gSpriteClips[tt]);  

      // if there's a unit on there, draw it
      Unit u = map->unitField[j][i];
      if (u.level == NONE) {
        continue;
      }

      int unitX = floor(offset_x + i*gSpriteClips[tt].w + gSpriteClips[tt].w/2 - gSpriteTrees[u.level].w/2);
      int unitY = floor(offset_y + j*gSpriteClips[tt].h - 38*j);

      SDL_Rect* clip = NULL;

      switch(u.type) {
      case PROGRESSION_PLANTS:
        if(u.level < LVL_THREE) {
          unitY += gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h;
        } else if(u.level == LVL_THREE){
          unitY -= gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h/4;
        } else if(u.level == LVL_SEVEN) {
          unitY -= gSpriteClips[tt].h;
        } else {
          unitY -= gSpriteClips[tt].h/2 - gSpriteTrees[u.level].h/2;
        }
        clip = &gSpriteTrees[u.level];

      break;

      case PROGRESSION_CATS:
        clip = &gSpriteCats[u.level];

        break;

      default:
        break;

      }

      if(clip != NULL) {
        render(&gSpriteSheetTexture, unitX, unitY, clip);  
      }
    }
  }

  switch(currentUnit.type) {
  case PROGRESSION_PLANTS:
    render(&gSpriteSheetTexture, 0, 0, &gSpriteTrees[currentUnit.level]);
    break;

  case PROGRESSION_CATS:
    render(&gSpriteSheetTexture, 0, 0, &gSpriteCats[currentUnit.level]);
    break;

  default:
    break;
  }

  SDL_RenderPresent(gRenderer);
}

void SDLHandleUserEvents(Map* map, bool* quit) {
  SDL_Event e;

  while(SDL_PollEvent(&e) != 0) {
    int x, y;
    int coordx, coordy;
    switch(e.type) {

    case SDL_MOUSEBUTTONDOWN:
      SDL_GetMouseState(&x, &y);
      // convert mouse coordinates to grid coordinates
      coordx = floor((x-offset_x -1)/gSpriteClips[0].w);
      coordy = floor((y-offset_y)/(gSpriteClips[0].h - 38));

      if(placeUnit(map, &currentUnit, coordx, coordy)) {
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
