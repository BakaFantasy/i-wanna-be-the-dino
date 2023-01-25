#include "common.h"

#ifndef I_WANNA_BE_THE_DINO__STRUCTS_H_
#define I_WANNA_BE_THE_DINO__STRUCTS_H_

typedef struct App {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int jump;
  int duck;
  int escape;
  int restart;
} App;

typedef enum Type {
  CACTUS,
  CACTUS_2,
  CACTUS_3,
  CACTUS_L,
  CACTUS_L_2,
  CACTUS_L_4,
  BIRD,
  CHERRY,
  SPIKE,
  SPIKE_TALL,
  SPIKE_REVERSED,
  KID,
} Type;

enum Sound {
  SND_JUMP,
  SND_DIE,
  SND_SHOOT,
  NUM_OF_SND
};

typedef struct Entity {
  Type type;
  double x;
  double y;
  double x1;
  double y1;
  int w;
  int h;
  double dx;
  double dy;
  SDL_Texture *texture;
  struct Entity *next;
} Entity;

/**
 * x,y for actual coordinate
 * x1,y1,etc. for coordinate of hit box
 * jumping_frames determine whether player holds jump key makes sense.
 * jumping_phase determine the times Dino can jump in the air.
 */
typedef struct Dino {
  double x, y;
  double x1, y1, x2, y2, x3, y3;
  int w1, h1, w2, h2, w3, h3;
  double dy;
  SDL_Texture *texture;
  int jumping_frames;
  int jumping_phase;
} Dino;

typedef struct Particle {
  double x, y;
  double dx, dy;
  struct Particle *next;
} Particle;

typedef struct LL {
  Entity *entity_head, *entity_tail;
  Particle *particle_head, *particle_tail;
} LL;

enum Mode {
  MODE_CLASSIC,
  MODE_I_WANNA,
  MODE_INFINITY,
  MODE_REVERSED,
  MODE_BOSS,
  NUM_OF_MODES
};

typedef struct Background {
  double x;
  double dx;
  SDL_Texture *texture;
} Background;

enum TextureName {
  TEXT_DINO_DEFAULT = 0,
  TEXT_DINO_MOVE_1,
  TEXT_DINO_MOVE_2,
  TEXT_DINO_DUCK_1,
  TEXT_DINO_DUCK_2,
  TEXT_DINO_DROP_1,
  TEXT_DINO_DROP_2,
  TEXT_DINO_DIE,
  TEXT_CAC_DEFAULT = 0,
  TEXT_CAC_2,
  TEXT_CAC_3,
  TEXT_CAC_L,
  TEXT_CAC_L_2,
  TEXT_CAC_L_4,
};

typedef struct Textures {
  SDL_Texture *dino[10], *cactus[6], *bird[2], *cherry[2], *spike[3], *kid[6];
  SDL_Texture *game_over;
} Textures;

#endif //I_WANNA_BE_THE_DINO__STRUCTS_H_
