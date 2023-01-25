#ifndef I_WANNA_BE_THE_DINO__DEFS_H_
#define I_WANNA_BE_THE_DINO__DEFS_H_

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

// about window
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// about FPS
#define INTERVAL_INT 8

// about audio
#define NUM_OF_CHANNELS 8

// about coordinate
#define DINO_INITIAL_X 250
#define DINO_INITIAL_Y 488
#define GROUND_Y 533
#define GROUND_Y_OFFSET 3
#define CEILING_Y (-80)

// about collision box
#define DINO_X1 270
#define DINO_Y1 488
#define DINO_W1 20
#define DINO_H1 15
#define DINO_X2 260
#define DINO_Y2 504
#define DINO_W2 14
#define DINO_H2 28
#define DINO_X3 274
#define DINO_Y3 504
#define DINO_W3 4
#define DINO_H3 16
#define DINO_DUCKING_X1 288
#define DINO_DUCKING_Y1 508
#define DINO_DUCKING_W1 20
#define DINO_DUCKING_H1 15
#define DINO_DUCKING_X2 264
#define DINO_DUCKING_Y2 509
#define DINO_DUCKING_W2 22
#define DINO_DUCKING_H2 19
#define DINO_DUCKING_X3 256
#define DINO_DUCKING_Y3 528
#define DINO_DUCKING_W3 10
#define DINO_DUCKING_H3 5

// about dino
#define ANIMATION_INTERVAL 100
#define JUMP_SPEED (-4.6)
#define MAX_DROPPING_SPEED 6
#define MAX_DUCKING_SPEED 10
#define MAX_REVERSED_DROPPING_SPEED (-3)
#define GRAVITY_DDY 0.3
#define REVERSED_GRAVITY_DDY (-0.05)
#define DUCKING_DDY 1.0
#define REVERSED_DUCKING_DDY 0.10
#define AIR_RESISTANCE_DDY (-0.4)
#define MAX_JUMPING_FRAMES 15
#define DEFAULT_JUMPING_PHASES 1

// about sprites
#define INITIAL_SPAWN_TIME 100
#define BOSS_STAGE_KEY_1 0.6
#define BOSS_STAGE_KEY_2 0.33
#define BOSS_BONUS_SCORE 1200

// miscellaneous
#define GROUND_SCROLLING_SPEED 4
#define SCROLLING_CRITICAL_VALUE 2.0
#define MODE_CHANGING_INTERVAL 3600
#define TEXT_ELAPSE 300
#define PI 3.1416
#define NUM_OF_PARTICLES 600

#endif //I_WANNA_BE_THE_DINO__DEFS_H_
