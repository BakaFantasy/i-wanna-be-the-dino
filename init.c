#include "common.h"
#include "init.h"

extern App app;
extern SDL_Texture *bg_texture;
extern TTF_Font *font_big;
extern TTF_Font *font_middle;
extern TTF_Font *font_small;
extern SDL_Color font_default_color;
extern Mix_Chunk *sounds[NUM_OF_SND];
extern Mix_Music *music;

void InitSDL(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_TIMER) < 0) {
		printf("Fail to initialize SDL: %s", SDL_GetError());
		assert(0);
	}
	if (TTF_Init() < 0) {
		printf("Fail to initialize TTF: %s", SDL_GetError());
		assert(0);
	}
	if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("Fail to initialize Mixer: %s", SDL_GetError());
		assert(0);
	}

	Mix_AllocateChannels(NUM_OF_CHANNELS);

	app.window = SDL_CreateWindow("I Wanna Be the Dino!!!",
							  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN);
	if (!app.window) {
		printf("Fail to open %d * %d window: %s", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(EXIT_FAILURE);
	}
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	if (!app.renderer) {
		printf("Fail to create renderer: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_SetRenderDrawColor(app.renderer, 254, 0, 0 , 255);
	SDL_RenderClear(app.renderer);
}

void InitGraphs(void) {
	font_big = TTF_OpenFont("./resources/fonts/LLPixelFun-Regular.ttf", 80);
	font_middle = TTF_OpenFont("./resources/fonts/LLPixelFun-Regular.ttf", 36);
	font_small = TTF_OpenFont("./resources/fonts/LLPixelFun-Regular.ttf", 20);
	font_default_color = (SDL_Color) {0, 0, 0, 255};
}

void InitSounds(void) {
	memset(sounds, 0, sizeof (Mix_Chunk*) * NUM_OF_SND);
	sounds[SND_JUMP] = Mix_LoadWAV("./resources/sounds/jump.mp3");
	sounds[SND_DIE] = Mix_LoadWAV("./resources/sounds/die.mp3");
	sounds[SND_SHOOT] = Mix_LoadWAV("./resources/sounds/shoot.mp3");

	music = NULL;
	PlayMusic("./resources/sounds/menu.mp3");
}