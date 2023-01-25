#include "common.h"
#include "sprites.h"

extern Dino dino;
extern Textures textures;
extern Mix_Chunk *sounds[NUM_OF_SND];
extern enum Mode mode;
extern double scroll_factor;
extern double score;
static int last_random_num;
static double x_boss;
static double y_boss;

void DoSprites(LL *ll) {
	for (Entity *csr = ll->entity_head, *prev = NULL; csr != NULL; prev = csr, csr = csr->next) {
		csr->x += csr->dx;
		csr->x1 += csr->dx;
		csr->y += csr->dy;
		csr->y1 += csr->dy;

		if (mode == MODE_REVERSED && csr->type == BIRD) {
			// produce sine shaped movement
			double factor = fmod(csr->dx, -scroll_factor * GROUND_SCROLLING_SPEED);
			csr->dy = 0.5 * factor * sin(factor * 0.001 * SDL_GetTicks());
		} else if (mode == MODE_BOSS) {
			if (csr->type == KID) {
				// get coordinate of boss
				x_boss = csr->x;
				y_boss = csr->y;
				if (x_boss > BOSS_STAGE_KEY_1 * WINDOW_WIDTH && x_boss < 0.9 * WINDOW_WIDTH)
					csr->dx = -scroll_factor * 0.1;
				else if (x_boss > BOSS_STAGE_KEY_2 * WINDOW_WIDTH && x_boss < BOSS_STAGE_KEY_1 * WINDOW_WIDTH)
					csr->dx = -scroll_factor * 0.4;
				else if (x_boss < BOSS_STAGE_KEY_2 * WINDOW_WIDTH)
					csr->dx = -scroll_factor * 0.7;
			} else if (csr->type == CHERRY) {
				// simulate gravity
				if (csr->y < GROUND_Y) {
					if (x_boss < BOSS_STAGE_KEY_1)
						csr->dy += 0.04 * GRAVITY_DDY;
					else
						csr->dy += 0.12 * GRAVITY_DDY;
				} else {
					csr->dy *= -0.95;
				}
			}
		}

		if (csr->x < -csr->w) {
//			puts("Killed a sprite.");
			if (csr == ll->entity_head)
				ll->entity_head = csr->next;
			if (csr == ll->entity_tail)
				ll->entity_tail = prev;
			if (prev != NULL)
				prev->next = csr->next;
			free(csr);
			if (prev != NULL) csr = prev;
			else csr = ll->entity_head;
			if (ll->entity_head == NULL)
				break;
		}
	}
}

void SpawnSprite(LL *ll, double *spawn_counter) {
	if (--*spawn_counter <= 0) {
		Entity *sprite = calloc(1, sizeof *sprite);
		if (ll->entity_tail == NULL)
			ll->entity_tail = sprite;
		sprite->next = ll->entity_head;
		ll->entity_head = sprite;

		InitSprite(sprite, DecideSprite());

		srand(SDL_GetTicks());
		switch (mode) {
			case MODE_CLASSIC:
			case MODE_I_WANNA:
				*spawn_counter = (80 + rand() % 80) / scroll_factor;
				break;
			case MODE_INFINITY:
				*spawn_counter = 40 / scroll_factor;
				break;
			case MODE_REVERSED:
				*spawn_counter = (100 + rand() % 100) / scroll_factor;
				break;
			case MODE_BOSS:
				if (x_boss > BOSS_STAGE_KEY_1 * WINDOW_WIDTH)
					*spawn_counter = (70 + rand() % 70) / scroll_factor;
				else if (x_boss > BOSS_STAGE_KEY_2 * WINDOW_WIDTH)
					*spawn_counter = (140 + rand() % 70) / scroll_factor;
				else
					*spawn_counter = 25 + rand() % 10;
				break;
			default:
				break;
		}
	}
}

static Type DecideSprite(void) {
	int key = CACTUS;
	srand(SDL_GetTicks());
	switch (mode) {
		case MODE_CLASSIC:
			key = rand() % (BIRD + 1);
			if (key == last_random_num)
				key = (key + 1) % (BIRD + 1);
			last_random_num = key;

			if (score < 150 && key == CACTUS_L_4)
				key = CACTUS;
			break;
		case MODE_I_WANNA:
			key = rand() % 3;
			if (key) {
				key = rand() % (SPIKE_REVERSED + 1);
				if (key == last_random_num)
					key = (key + 1) % (SPIKE_REVERSED + 1);
				last_random_num = key;
				if (key == BIRD)
					key = CHERRY;
			} else
				key = CHERRY;
			break;
		case MODE_INFINITY:
			key = CHERRY;
			break;
		case MODE_REVERSED:
			key = BIRD;
			break;
		case MODE_BOSS:
			key = rand() % 3;
			if (key != 0 && x_boss >= BOSS_STAGE_KEY_1 * WINDOW_WIDTH) {
				key = rand() % (SPIKE + 1);
				if (key == last_random_num)
					key = (key + 1) % (SPIKE + 1);
				last_random_num = key;
				if (key == BIRD)
					key = CHERRY;
				else if (key == CACTUS)
					key = SPIKE;
			} else
				key = CHERRY;
		default:
			break;
	}
	return key;
}

void InitSprite(Entity *sprite, enum Type type) {
	srand(SDL_GetTicks());
	sprite->x = sprite->x1 = WINDOW_WIDTH;
	sprite->dx = -GROUND_SCROLLING_SPEED * scroll_factor;

	switch (type) {
		case CACTUS:
			sprite->y = 502;
			sprite->y1 = sprite->y + 1;
			sprite->w = 16;
			sprite->h = 33;
			sprite->texture = textures.cactus[0];
			break;
		case CACTUS_2:
			sprite->y = 502;
			sprite->y1 = sprite->y + 1;
			sprite->w = 32;
			sprite->h = 33;
			sprite->texture = textures.cactus[1];
			break;
		case CACTUS_3:
			sprite->y = 502;
			sprite->y1 = sprite->y + 1;
			sprite->w = 49;
			sprite->h = 33;
			sprite->texture = textures.cactus[2];
			break;
		case CACTUS_L:
			sprite->y = 487;
			sprite->y1 = sprite->y + 1;
			sprite->w = 23;
			sprite->h = 46;
			sprite->texture = textures.cactus[3];
			break;
		case CACTUS_L_2:
			sprite->y = 487;
			sprite->y1 = sprite->y + 1;
			sprite->w = 48;
			sprite->h = 46;
			sprite->texture = textures.cactus[4];
			break;
		case CACTUS_L_4:
			sprite->y = 486;
			sprite->y1 = sprite->y + 1;
			sprite->w = 73;
			sprite->h = 47;
			sprite->texture = textures.cactus[5];
			break;
		case BIRD:
			sprite->type = BIRD;
			sprite->dx *= 0.01 * (100 + rand() % 25);
			int tmp = rand() % 3;
			if (tmp == 0)
				sprite->y = 416;
			else if (tmp == 1)
				sprite->y = 476;
			else
				sprite->y = 496;
			sprite->y1 = sprite->y + 6;
			sprite->w = 42;
			sprite->h = 20;
			sprite->texture = textures.bird[0];
			break;
		case CHERRY:
			sprite->type = CHERRY;
			sprite->dx *= 1.25;
			sprite->y = 480 - rand() % 100;
			sprite->y1 = sprite->y + 1;
			sprite->w = 21;
			sprite->h = 22;
			sprite->texture = textures.cherry[0];
			break;
		case SPIKE:
			sprite->type = SPIKE;
			sprite->y = 485;
			sprite->y1 = sprite->y;
			sprite->w = 48;
			sprite->h = 48;
			sprite->texture = textures.spike[0];
			break;
		case SPIKE_TALL:
			sprite->type = SPIKE_TALL;
			sprite->y = 389;
			sprite->y1 = sprite->y;
			sprite->w = 48;
			sprite->h = 144;
			sprite->texture = textures.spike[1];
			break;
		case SPIKE_REVERSED:
			sprite->type = SPIKE_REVERSED;
			sprite->y = -5;
			sprite->y1 = sprite->y;
			sprite->w = 48;
			sprite->h = 505;
			sprite->texture = textures.spike[2];
			break;
		case KID:
			sprite->type = KID;
			sprite->dx = -scroll_factor * 0.2;
			sprite->x1 = sprite->x + 128;
			sprite->y = 197;
			sprite->y1 = sprite->y;
			sprite->w = 288;
			sprite->h = 336;
			sprite->texture = textures.kid[0];
		default:
			assert(0);
	}

	srand(SDL_GetTicks());
	switch (mode) {
		case MODE_INFINITY:
			sprite->y = sprite->y1 = 10 + SDL_GetTicks() / 2 % GROUND_Y;
			sprite->dx = -scroll_factor * GROUND_SCROLLING_SPEED * 0.0115 * (100 + rand() % 20);
			AimAtPlayer(sprite);
			break;
		case MODE_REVERSED:
			sprite->dx = -scroll_factor * GROUND_SCROLLING_SPEED * 0.001 * (500 + rand() % 300);
			sprite->y = 50 + rand() % 475;
			sprite->y1 = sprite->y + 6;
			sprite->dy = 10;
			break;
		case MODE_BOSS:
			if (sprite->type == CHERRY) {
				sprite->x = sprite->x1 = x_boss - 0.5 * sprite->w;
				sprite->y = sprite->y1 = y_boss + 160 - 0.5 * sprite->h;
				sprite->dx = -(3 + 0.015 * (rand() % 100));
				if (x_boss > BOSS_STAGE_KEY_2 * WINDOW_WIDTH && x_boss < BOSS_STAGE_KEY_1 * WINDOW_WIDTH) {
					sprite->dx *= 0.9;
					sprite->dy = 0.5;
				}
				Mix_PlayChannel(2, sounds[SND_SHOOT], 0);
			}
		default:
			break;
	}
}

static void AimAtPlayer(Entity *sprite) {
	srand(SDL_GetTicks());
	double angle = atan2(dino.y - sprite->y, dino.x - sprite->x) + 0.0018 * (-50 + rand() % 100);
	sprite->dy = -sprite->dx * sin(angle);
	sprite->dx = sprite->dy / tan(angle);
}

void DrawSprites(LL *ll) {
	for (Entity *csr = ll->entity_head; csr != NULL; csr = csr->next) {
		if (csr->type == BIRD) {
			if (SDL_GetTicks() % (4 * ANIMATION_INTERVAL) < 2 * ANIMATION_INTERVAL) {
				csr->texture = textures.bird[0];
			} else {
				csr->texture = textures.bird[1];
			}
		} else if (csr->type == CHERRY) {
			if (SDL_GetTicks() % (12 * ANIMATION_INTERVAL) < 6 * ANIMATION_INTERVAL) {
				csr->texture = textures.cherry[0];
				csr->h = 22;
			} else {
				csr->texture = textures.cherry[1];
				csr->h = 24;
			}
		} else if (csr->type == KID) {
			Uint32 tmp = SDL_GetTicks() % (30 * ANIMATION_INTERVAL);
			if (tmp < 5 * ANIMATION_INTERVAL)
				csr->texture = textures.kid[0];
			else if (tmp < 10 * ANIMATION_INTERVAL)
				csr->texture = textures.kid[1];
			else if (tmp < 15 * ANIMATION_INTERVAL)
				csr->texture = textures.kid[2];
			else if (tmp < 20 * ANIMATION_INTERVAL)
				csr->texture = textures.kid[3];
			else if (tmp < 25 * ANIMATION_INTERVAL)
				csr->texture = textures.kid[4];
			else
				csr->texture = textures.kid[5];
		}
		Blit(csr->texture, csr->x, csr->y);
	}
}

void ClearLL(LL *ll) {
	puts("Cleared linked list.");
	for (Entity *csr = ll->entity_head, *prev = NULL; csr != NULL; prev = csr, csr = csr->next) {
		if (prev != NULL)
			free(prev);
		if (csr == ll->entity_tail) {
			free(csr);
			break;
		}
	}
	for (Particle *csr = ll->particle_head, *prev = NULL; csr != NULL; prev = csr, csr = csr->next) {
		if (prev != NULL)
			free(prev);
		if (csr == ll->particle_tail) {
			free(csr);
			return;
		}
	}
}