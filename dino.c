#include "common.h"
#include "dino.h"

extern App app;
extern Dino dino;
extern Textures textures;
extern Mix_Chunk *sounds[NUM_OF_SND];
extern enum Mode mode;
extern double gravity_ddy;
extern double ducking_ddy;

void DoDino(void) {
	if (mode != MODE_REVERSED) {
		if (dino.dy >= 0 && dino.y >= DINO_INITIAL_Y - GROUND_Y_OFFSET) {
			dino.dy = 0;
			dino.y = DINO_INITIAL_Y;
			// change the hit box
			if (app.duck) {
				dino.x1 = DINO_DUCKING_X1;
				dino.y1 = DINO_DUCKING_Y1;
				dino.w1 = DINO_DUCKING_W1;
				dino.h1 = DINO_DUCKING_H1;
				dino.x2 = DINO_DUCKING_X2;
				dino.y2 = DINO_DUCKING_Y2;
				dino.w2 = DINO_DUCKING_W2;
				dino.h2 = DINO_DUCKING_H2;
				dino.x3 = DINO_DUCKING_X3;
				dino.y3 = DINO_DUCKING_Y3;
				dino.w3 = DINO_DUCKING_W3;
				dino.h3 = DINO_DUCKING_H3;
			} else {
				dino.x1 = DINO_X1;
				dino.y1 = DINO_Y1;
				dino.w1 = DINO_W1;
				dino.h1 = DINO_H1;
				dino.x2 = DINO_X2;
				dino.y2 = DINO_Y2;
				dino.w2 = DINO_W2;
				dino.h2 = DINO_H2;
				dino.x3 = DINO_X3;
				dino.y3 = DINO_Y3;
				dino.w3 = DINO_W3;
				dino.h3 = DINO_H3;
			}
			dino.jumping_frames = 0;
			dino.jumping_phase = 0;
		} else if (app.duck) {
			if (dino.dy >= MAX_DUCKING_SPEED) {
				dino.dy = MAX_DUCKING_SPEED;
			}
		} else if (dino.dy >= MAX_DROPPING_SPEED) {
			dino.dy += AIR_RESISTANCE_DDY; // simulate air resistance
		} else {
			dino.dy += gravity_ddy; // simulate gravity
		}
	} else {
		if (dino.dy >= 0 && dino.y >= DINO_INITIAL_Y - GROUND_Y_OFFSET) {
			// guarantee the hit box is in accord with the texture
			dino.x1 = DINO_X1;
			dino.y1 = DINO_Y1;
			dino.w1 = DINO_W1;
			dino.h1 = DINO_H1;
			dino.x2 = DINO_X2;
			dino.y2 = DINO_Y2;
			dino.w2 = DINO_W2;
			dino.h2 = DINO_H2;
			dino.x3 = DINO_X3;
			dino.y3 = DINO_Y3;
			dino.w3 = DINO_W3;
			dino.h3 = DINO_H3;
		}
		if (dino.dy >= 0 && dino.y > DINO_INITIAL_Y) {
			dino.dy = -dino.dy;
		} else if (dino.dy <= MAX_REVERSED_DROPPING_SPEED) {
			dino.dy = MAX_REVERSED_DROPPING_SPEED;
		} else if (dino.dy >= MAX_DROPPING_SPEED) {
			dino.dy = MAX_DROPPING_SPEED;
		}
		dino.dy += gravity_ddy;
	}

	dino.y += dino.dy;
	dino.y1 += dino.dy;
	dino.y2 += dino.dy;
	dino.y3 += dino.dy;
}

void DrawDino(void) {
	if (dino.y == DINO_INITIAL_Y) {
		if (SDL_GetTicks() % (2 * ANIMATION_INTERVAL) < ANIMATION_INTERVAL) {
			if (app.duck == 1)
				dino.texture = textures.dino[TEXT_DINO_DUCK_1];
			else
				dino.texture = textures.dino[TEXT_DINO_MOVE_1];
		}
		else {
			if (app.duck == 1)
				dino.texture = textures.dino[TEXT_DINO_DUCK_2];
			else
				dino.texture = textures.dino[TEXT_DINO_MOVE_2];
		}
	} else if (dino.dy > 0) {
		if (dino.dy >= MAX_DUCKING_SPEED)
			dino.texture = textures.dino[TEXT_DINO_DROP_2];
		else if (dino.dy >= MAX_DROPPING_SPEED + 0.3)
			dino.texture = textures.dino[TEXT_DINO_DROP_1];
	} else {
		dino.texture = textures.dino[TEXT_DINO_DEFAULT];
	}

	Blit(dino.texture, dino.x, dino.y);
}

void DoJump(void) {
	if (dino.jumping_frames == 0)
		Mix_PlayChannel(0, sounds[SND_JUMP], 0);
	dino.jumping_frames++;
	if (!app.duck) {
		dino.dy = JUMP_SPEED;
	}
}

void DoDuck(void) {
	if (dino.y < DINO_INITIAL_Y - GROUND_Y_OFFSET) {
		dino.dy += ducking_ddy;
	}
}
