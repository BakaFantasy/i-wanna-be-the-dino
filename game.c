#include "common.h"
#include "game.h"

extern App app;
extern TTF_Font *font_big;
extern TTF_Font *font_middle;
extern TTF_Font *font_small;
extern SDL_Color font_default_color;
extern Mix_Chunk *sounds[NUM_OF_SND];
extern Mix_Music *music;
extern long next_tick;
extern double frac_tick;

static Background sky;
static Background ground;
Dino dino;
static LL ll;
Textures textures;

enum Mode mode;
double scroll_factor;
double score;
static Uint32 high_score;
static Uint32 init_tick;
static bool is_over;
static double spawn_counter;
static double mode_changing_counter;
static double text_counter;
static double particles_counter;

static char text1[60];
static char text2[60];
static int text_offset1;
static int text_offset2;

static int max_jumping_phases;
double gravity_ddy;
double ducking_ddy;

void GameLogic(void) {
	InitGame();

	Pause();

	while (true) {
		if (DetectCollision() || dino.y < CEILING_Y)
			GameOver();

		DoInput();

		if (app.escape || app.restart) {
			DoEscape();
			return;
		}

		DoPlayer();

		MoveBackground(&sky, 4);
		MoveBackground(&ground, 2);

		DoDino();

		DoSprites(&ll);

		SpawnSprite(&ll, &spawn_counter);

		SDL_RenderClear(app.renderer);

		Blit(sky.texture, sky.x, 0);
		Blit(ground.texture, ground.x, 0);

		DrawDino();

		DrawSprites(&ll);

		DoParticles();

		DoScore();

		if (--text_counter > 0)
			DisplayHint();

		SDL_RenderPresent(app.renderer);

		scroll_factor = SpeedUp();

		if (--mode_changing_counter == 0) {
			SwitchMode();
		} else if (mode_changing_counter < 0.09 * MODE_CHANGING_INTERVAL)
			spawn_counter = 10 * INITIAL_SPAWN_TIME;

		MaintainFPS();
	}
}

static void InitGame(void) {
	textures.dino[TEXT_DINO_DEFAULT] = LoadTexture("./resources/images/dino.png");
	textures.dino[TEXT_DINO_MOVE_1] = LoadTexture("./resources/images/dino-move-1.png");
	textures.dino[TEXT_DINO_MOVE_2] = LoadTexture("./resources/images/dino-move-2.png");
	textures.dino[TEXT_DINO_DUCK_1] = LoadTexture("./resources/images/dino-duck-1.png");
	textures.dino[TEXT_DINO_DUCK_2] = LoadTexture("./resources/images/dino-duck-2.png");
	textures.dino[TEXT_DINO_DROP_1] = LoadTexture("./resources/images/dino-drop-1.png");
	textures.dino[TEXT_DINO_DROP_2] = LoadTexture("./resources/images/dino-drop-2.png");
	textures.dino[TEXT_DINO_DIE] = LoadTexture("./resources/images/dino-die.png");
	textures.cactus[TEXT_CAC_DEFAULT] = LoadTexture("./resources/images/cactus.png");
	textures.cactus[TEXT_CAC_2] = LoadTexture("./resources/images/cactus_twin.png");
	textures.cactus[TEXT_CAC_3] = LoadTexture("./resources/images/cactus_triplet.png");
	textures.cactus[TEXT_CAC_L] = LoadTexture("./resources/images/cactus_large.png");
	textures.cactus[TEXT_CAC_L_2] = LoadTexture("./resources/images/cactus_large_twin.png");
	textures.cactus[TEXT_CAC_L_4] = LoadTexture("./resources/images/cactus_quadruplet.png");
	textures.bird[0] = LoadTexture("./resources/images/bird_1.png");
	textures.bird[1] = LoadTexture("./resources/images/bird_2.png");
	textures.cherry[0] = LoadTexture("./resources/images/cherry_1.png");
	textures.cherry[1] = LoadTexture("./resources/images/cherry_2.png");
	textures.spike[0] = LoadTexture("./resources/images/spike.png");
	textures.spike[1] = LoadTexture("./resources/images/spike_tall.png");
	textures.spike[2] = LoadTexture("./resources/images/spike_reversed.png");
	textures.kid[0] = LoadTexture("./resources/images/kid_1.png");
	textures.kid[1] = LoadTexture("./resources/images/kid_2.png");
	textures.kid[2] = LoadTexture("./resources/images/kid_3.png");
	textures.kid[3] = LoadTexture("./resources/images/kid_4.png");
	textures.kid[4] = LoadTexture("./resources/images/kid_5.png");
	textures.kid[5] = LoadTexture("./resources/images/kid_6.png");
	textures.game_over = LoadTexture("./resources/images/game_over.png");

	sky.x = 0;
	sky.dx = 0.25 * GROUND_SCROLLING_SPEED;
	sky.texture = LoadTexture("./resources/images/sky.png");

	ground.x = 0;
	ground.dx = GROUND_SCROLLING_SPEED;
	ground.texture = LoadTexture("./resources/images/ground.png");

	dino.x = DINO_INITIAL_X;
	dino.y = dino.y1 = DINO_INITIAL_Y;
	dino.x1 = DINO_INITIAL_X + 10;
	dino.w1 = DINO_W1;
	dino.h1 = DINO_H1;
	dino.texture = textures.dino[TEXT_DINO_DEFAULT];

	scroll_factor = 1;
	mode = MODE_CLASSIC;
	max_jumping_phases = DEFAULT_JUMPING_PHASES;
	gravity_ddy = GRAVITY_DDY;
	ducking_ddy = DUCKING_DDY;

	ll.entity_head = ll.entity_tail = NULL;
	ll.particle_head = ll.particle_tail = NULL;

	spawn_counter = INITIAL_SPAWN_TIME;
	mode_changing_counter = MODE_CHANGING_INTERVAL;
	text_counter = 0;
	particles_counter = 0;

	score = 0;
	init_tick = SDL_GetTicks();

	FILE *fp = NULL;
	fp = fopen("./score", "r");
	if (fp != NULL) {
		fscanf(fp, "%u", &high_score);
		fclose(fp);
	} else
		high_score = 0;

	next_tick = (long) init_tick;
	frac_tick = 0;

	Blit(sky.texture, sky.x, 0);
	Blit(ground.texture, ground.x, 0);
	Blit(dino.texture, DINO_INITIAL_X, DINO_INITIAL_Y);

	SDL_RenderPresent(app.renderer);
}

static void Pause(void) {
	puts("Paused.");
	SDL_Event pause_event;
	while (SDL_WaitEvent(&pause_event)) {
		if (pause_event.type == SDL_QUIT) {
			DoEscape();
			Quit();
			exit(EXIT_SUCCESS);
		}
		if (pause_event.key.state == SDL_PRESSED) {
			if (pause_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				app.escape = 1;
			break;
		}
	}
}

static bool DetectCollision(void) {
	for (Entity *csr = ll.entity_head; csr != NULL; csr = csr->next) {
		if (MAX(csr->x1, dino.x1) < MIN(csr->x1 + csr->w, dino.x1 + dino.w1) &&
			MAX(csr->y1, dino.y1) < MIN(csr->y1 + csr->h, dino.y1 + dino.h1) ||
			MAX(csr->x1, dino.x2) < MIN(csr->x1 + csr->w, dino.x2 + dino.w2) &&
			MAX(csr->y1, dino.y2) < MIN(csr->y1 + csr->h, dino.y2 + dino.h2) ||
			MAX(csr->x1, dino.x3) < MIN(csr->x1 + csr->w, dino.x3 + dino.w3) &&
			MAX(csr->y1, dino.y3) < MIN(csr->y1 + csr->h, dino.y3 + dino.h3)) {
			if (csr->type == SPIKE) {
				if (dino.y1 + dino.h1 + 2 * (dino.x1 + dino.w1) < csr->y1 + csr->h + 2 * csr->x1 &&
					dino.y2 + dino.h2 + 2 * (dino.x2 + dino.w2) < csr->y1 + csr->h + 2 * csr->x1 &&
					dino.y3 + dino.h3 + 2 * (dino.x3 + dino.w3) < csr->y1 + csr->h + 2 * csr->x1 ||
					dino.y1 + dino.h1 - 2 * dino.x1 < csr->y1 + csr->h - 2 * (csr->x1 + csr->w) &&
					dino.y2 + dino.h2 - 2 * dino.x2 < csr->y1 + csr->h - 2 * (csr->x1 + csr->w) &&
					dino.y3 + dino.h3 - 2 * dino.x3 < csr->y1 + csr->h - 2 * (csr->x1 + csr->w))
					continue;
			} else if (csr->type == SPIKE_TALL) {
				if (dino.y1 + dino.h1 + 6 * (dino.x1 + dino.w1) < csr->y1 + csr->h + 6 * csr->x1 &&
					dino.y2 + dino.h2 + 6 * (dino.x2 + dino.w2) < csr->y1 + csr->h + 6 * csr->x1 &&
					dino.y3 + dino.h3 + 6 * (dino.x3 + dino.w3) < csr->y1 + csr->h + 6 * csr->x1 ||
					dino.y1 + dino.h1 - 6 * dino.x1 < csr->y1 + csr->h - 6 * (csr->x1 + csr->w) &&
					dino.y2 + dino.h2 - 6 * dino.x2 < csr->y1 + csr->h - 6 * (csr->x1 + csr->w) &&
					dino.y3 + dino.h3 - 6 * dino.x3 < csr->y1 + csr->h - 6 * (csr->x1 + csr->w))
					continue;
			} else if (csr->type == SPIKE_REVERSED) {
				if (dino.y1 - 21.04 * (dino.x1 + dino.w1) > csr->y1 - 21.04 * csr->x1 &&
					dino.y2 - 21.04 * (dino.x2 + dino.w2) > csr->y1 - 21.04 * csr->x1 &&
					dino.y3 - 21.04 * (dino.x3 + dino.w3) > csr->y1 - 21.04 * csr->x1 ||
					dino.y1 + 21.04 * dino.x1 > csr->y1 + csr->h + 21.04 * (csr->x1 + csr->w) &&
					dino.y2 + 21.04 * dino.x2 > csr->y1 + csr->h + 21.04 * (csr->x1 + csr->w) &&
					dino.y3 + 21.04 * dino.x3 > csr->y1 + csr->h + 21.04 * (csr->x1 + csr->w))
					continue;
			} else if (csr->type == KID) {
				particles_counter = NUM_OF_PARTICLES;
				score += BOSS_BONUS_SCORE;
				if (mode == MODE_BOSS)
					spawn_counter = 10 * INITIAL_SPAWN_TIME;
				for (int i = 0; i < NUM_OF_PARTICLES; ++i)
					SpoutBlood(csr->x + 0.5 * csr->w, csr->y + 0.5 * csr->h);
				csr->x = csr->x1 = -2 * csr->w;
				Mix_PlayChannel(1, sounds[SND_DIE], 0);
				PlayMusic("./resources/sounds/game.mp3");
				mode_changing_counter = 0.2 * MODE_CHANGING_INTERVAL;
				continue;
			}
			if (mode == MODE_BOSS)
				PlayMusic("./resources/sounds/game.mp3");
			return true;
		}
	}
	return false;
}

static void GameOver(void) {
	puts("Game over.");
	is_over = true;
	Mix_PlayChannel(1, sounds[SND_DIE], 0);

	FILE *fp = NULL;
	fp = fopen("./score", "w");
	if (fp != NULL) {
		if ((Uint32) score > high_score)
			fprintf(fp, "%.5d", (Uint32) score);
		fclose(fp);
	}

	next_tick = (long) SDL_GetTicks();
	frac_tick = 0;

	particles_counter = NUM_OF_PARTICLES;

	while (true) {
		SDL_Event over_event;
		while (SDL_PollEvent(&over_event)) {
			if (over_event.type == SDL_QUIT) {
				DoEscape();
				Quit();
				exit(EXIT_SUCCESS);
			} else if (over_event.key.keysym.scancode == SDL_SCANCODE_R) {
				app.restart = 1;
			} else if (over_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				PlayMusic("./resources/sounds/menu.mp3");
				app.escape = 1;
			}
		}

		if (app.escape || app.restart)
			break;

		SDL_RenderClear(app.renderer);

		Blit(sky.texture, sky.x, 0);
		Blit(ground.texture, ground.x, 0);

		DrawSprites(&ll);

		if (particles_counter > 0)
			for (int i = 0; i < 15; ++i)
				SpoutBlood(dino.x3 + 0.5 * dino.w3, dino.y3 + 0.75 * dino.h3);

		DoParticles();

		DoScore();

		Blit(textures.game_over, 72, 150);

		SDL_RenderPresent(app.renderer);

		MaintainFPS();
	}
}

static double SpeedUp(void) {
//	printf("scroll_factor is %f.\n", scroll_factor);
	if (scroll_factor < SCROLLING_CRITICAL_VALUE)
		return scroll_factor + 0.0000375;
	return scroll_factor + 1.0 / (1000.0 + SDL_GetTicks());
}

static void SwitchMode(void) {
	spawn_counter = INITIAL_SPAWN_TIME;
	score += 200;
	int tmp = rand() % 8;
	if (tmp == 0) {
		mode = MODE_BOSS;
	} else {
		srand(SDL_GetTicks());
		mode = (mode + rand() % (MODE_BOSS - 1) + 1) % MODE_BOSS;
	}
//	mode = MODE_BOSS; //DEBUG
	printf("Mode turned into %d.\n", mode);

	gravity_ddy = GRAVITY_DDY; // reset
	ducking_ddy = DUCKING_DDY; // reset
	text_counter = TEXT_ELAPSE;

	switch (mode) {
		case MODE_CLASSIC:
			max_jumping_phases = 1;
			mode_changing_counter = MODE_CHANGING_INTERVAL;
			break;
		case MODE_I_WANNA:
			max_jumping_phases = 2;
			mode_changing_counter = MODE_CHANGING_INTERVAL;
			break;
		case MODE_INFINITY:
			max_jumping_phases = 2147483647;
			mode_changing_counter = 0.8 * MODE_CHANGING_INTERVAL;
			break;
		case MODE_REVERSED:
			gravity_ddy = REVERSED_GRAVITY_DDY;
			ducking_ddy = REVERSED_DUCKING_DDY;
			mode_changing_counter = 0.8 * MODE_CHANGING_INTERVAL;
			break;
		case MODE_BOSS:
			max_jumping_phases = 2;
			PlayMusic("./resources/sounds/boss.mp3");
			Entity *sprite = calloc(1, sizeof *sprite);
			if (ll.entity_tail == NULL)
				ll.entity_tail = sprite;
			sprite->next = ll.entity_head;
			ll.entity_head = sprite;
			InitSprite(sprite, KID);
			mode_changing_counter = 30 * MODE_CHANGING_INTERVAL;
			break;
		default:
			break;
	}
}

static void DisplayHint(void) {
	if (text_counter >= TEXT_ELAPSE - 3) {
		switch (mode) {
			case MODE_CLASSIC:
				strcpy(text1, "Classic Mode");
				strcpy(text2, "Sole jump!");
				text_offset1 = 35;
				text_offset2 = 18;
				break;
			case MODE_I_WANNA:
				strcpy(text1, "I Wanna Mode");
				strcpy(text2, "Double jump!");
				text_offset1 = 35;
				text_offset2 = 5;
				break;
			case MODE_INFINITY:
				strcpy(text1, "Infinity Mode");
				strcpy(text2, "Infinite Jump!");
				text_offset1 = 20;
				text_offset2 = 0;
				break;
			case MODE_REVERSED:
				strcpy(text1, "Reversed Mode");
				strcpy(text2, "Can't Jump!");
				text_offset1 = 0;
				text_offset2 = 10;
				break;
			case MODE_BOSS:
				strcpy(text1, "Boss Battle");
				strcpy(text2, "Double jump!");
				text_offset1 = 50;
				text_offset2 = 5;
				break;
			default:
				break;
		}
	}

	SDL_Surface *font_surface = TTF_RenderUTF8_Blended(font_big, text1, font_default_color);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(app.renderer, font_surface);
	Blit(font_texture, 310 + text_offset1, 250);

	SDL_FreeSurface(font_surface);
	SDL_DestroyTexture(font_texture);

	font_surface = TTF_RenderUTF8_Blended(font_small, text2, font_default_color);
	font_texture = SDL_CreateTextureFromSurface(app.renderer, font_surface);
	Blit(font_texture, dino.x - 50 + text_offset2, dino.y - 30);

	SDL_FreeSurface(font_surface);
	SDL_DestroyTexture(font_texture);
}

static void MoveBackground(Background *bg, int num_of_screens) {
	bg->x -= scroll_factor * bg->dx;
	if (bg->x < -1 * num_of_screens * WINDOW_WIDTH) bg->x = 0;
}

static void SpoutBlood(double x, double y) {
	Particle *particle = calloc(1, sizeof *particle);
	if (ll.particle_tail == NULL)
		ll.particle_tail = particle;
	particle->next = ll.particle_head;
	ll.particle_head = particle;

	particle->x = x;
	particle->y = y;
	particle->dx = sqrt(scroll_factor) * 1.5 * (0.5 + cos(rand()));
	particle->dy = 0.18 * dino.dy -0.015 * (30 + rand() % 200);

	particles_counter--;
}

static void DoParticles() {
	for (Particle *csr = ll.particle_head, *prev = NULL; csr != NULL; prev = csr, csr = csr->next) {
		csr->x += csr->dx;
		csr->y += csr->dy;

		if (csr->y <= GROUND_Y) {
			csr->dy += 0.1 * GRAVITY_DDY;
		}
		else {
			csr->y = GROUND_Y;
			csr->dx *= 0.91;
			csr->dy = -0.01;
		}

		SDL_Rect rect;
		rect.x = (int) csr->x;
		rect.y = (int) csr->y;
		rect.w = 3;
		rect.h = 3;
		SDL_RenderDrawRect(app.renderer, &rect);

		if (csr->x < 0 || csr->y < 0 || csr->x > WINDOW_WIDTH || csr->y > WINDOW_HEIGHT ||
			fabs(csr->dx) < 0.0001) {
			if (csr == ll.particle_head)
				ll.particle_head = csr->next;
			if (csr == ll.particle_tail)
				ll.particle_tail = prev;
			if (prev != NULL)
				prev->next = csr->next;
			free(csr);
			if (prev != NULL) csr = prev;
			else csr = ll.particle_head;
			if (ll.particle_head == NULL)
				break;
		}
	}
}

static void DoScore(void) {
	if (!is_over)
		score += 0.05 * scroll_factor * scroll_factor;

	char high_score_str[10] = {"HI "}, score_str[10];
	sprintf(high_score_str + 3, "%.5u", high_score);
	sprintf(score_str, "%.5u", (Uint32) score);

	SDL_Surface *font_surface = TTF_RenderUTF8_Blended(font_middle, high_score_str, font_default_color);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(app.renderer, font_surface);
	Blit(font_texture, 910, 15);
	SDL_FreeSurface(font_surface);
	SDL_DestroyTexture(font_texture);

	font_surface = TTF_RenderUTF8_Blended(font_middle, score_str, font_default_color);
	font_texture = SDL_CreateTextureFromSurface(app.renderer, font_surface);
	Blit(font_texture, 1130, 15);
	SDL_FreeSurface(font_surface);
	SDL_DestroyTexture(font_texture);

}

static void DoPlayer(void) {
	if (app.jump &&
		dino.jumping_frames <= MAX_JUMPING_FRAMES &&
		dino.jumping_phase < max_jumping_phases) {
		DoJump();
	}
	if (app.duck) {
		DoDuck();
	}
}

static void DoInput(void) {
	SDL_Event play_event;
	while (SDL_PollEvent(&play_event)) {
		switch (play_event.type) {
			case SDL_QUIT:
				DoEscape();
				Quit();
				exit(EXIT_SUCCESS);
			case SDL_KEYDOWN:
				DoKeyDown(&play_event.key);
				break;
			case SDL_KEYUP:
				DoKeyUp(&play_event.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("Clicked (%d, %d)\n", play_event.button.x, play_event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				printf("Released (%d, %d)\n", play_event.button.x, play_event.button.y);
				break;
			default:
				break;
		}
	}
}

static void DoKeyDown(const SDL_KeyboardEvent *event) {
	if (!event->repeat) {
		if (!app.jump && (event->keysym.scancode == SDL_SCANCODE_UP || event->keysym.scancode == SDL_SCANCODE_SPACE)) {
//			puts("Jumped.");
			app.jump = 1;
			dino.jumping_frames = 0;
		}
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
//			puts("Ducked.");
			app.duck = 1;
		}
		if (event->keysym.scancode == SDL_SCANCODE_ESCAPE) {
			puts("Back to menu.");
			PlayMusic("./resources/sounds/menu.mp3");
			app.escape = 1;
		}
		if (event->keysym.scancode == SDL_SCANCODE_R) {
			puts("Restarted.");
//			Mix_PlayChannel(1, sounds[SND_DIE], 0);
			app.restart = 1;
		}
	}
}

static void DoKeyUp(const SDL_KeyboardEvent *event) {
	if (event->repeat == 0) {
		if (event->keysym.scancode == SDL_SCANCODE_UP || event->keysym.scancode == SDL_SCANCODE_SPACE) {
			app.jump = 0;
			dino.jumping_phase++;
		}
		if (event->keysym.scancode == SDL_SCANCODE_DOWN) {
			app.duck = 0;
			dino.w1 = DINO_W1;
			dino.h1 = DINO_H1;
		}
	}
}

static void DoEscape(void) {
	app.escape = 0;
	app.jump = 0;
	app.duck = 0;
	is_over = false;
	memset(&dino, 0, sizeof dino);

	ClearLL(&ll);

	SDL_DestroyTexture(dino.texture);
	SDL_DestroyTexture(ground.texture);
	SDL_DestroyTexture(sky.texture);
	for (int i = 0; i < 8; ++i)
		SDL_DestroyTexture(textures.dino[i]);
	for (int i = 0; i < 6; ++i)
		SDL_DestroyTexture(textures.cactus[i]);
	for (int i = 0; i < 2; ++i)
		SDL_DestroyTexture(textures.bird[i]);
	for (int i = 0; i < 2; ++i)
		SDL_DestroyTexture(textures.cherry[i]);
	for (int i = 0; i < 3; ++i)
		SDL_DestroyTexture(textures.spike[i]);
	for (int i = 0; i < 6; ++i)
		SDL_DestroyTexture(textures.kid[i]);
	SDL_DestroyTexture(textures.game_over);
}