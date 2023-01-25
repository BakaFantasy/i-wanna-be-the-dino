//
// Created by Fradow on 2022/12/4.
//

#include "common.h"
#include "iwbtd.h"

App app;
static SDL_Texture *bg_texture;
TTF_Font *font_big;
TTF_Font *font_middle;
TTF_Font *font_small;
SDL_Color font_default_color;
Mix_Chunk *sounds[NUM_OF_SND];
Mix_Music *music;
long next_tick;
double frac_tick;

/**
 *
 * \author Fradow
 */
int main() {
	InitSDL();

	InitGraphs();

	InitSounds();

	bg_texture = LoadTexture("./resources/images/title_bg.png");
	next_tick = (long) SDL_GetTicks();
	frac_tick = 0;

	while (true) {
		if (app.restart) {
			app.restart = 0;
			GameLogic();
			continue;
		} else if (app.escape) {
			app.escape = 0;
			break;
		}

		DrawTitle();

		SDL_RenderPresent(app.renderer);

		DoMenuInput();

		MaintainFPS();
	}

	Quit();

	return 0;
}

SDL_Texture *LoadTexture(const char *file) {
//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", file);
	return IMG_LoadTexture(app.renderer, file);
}

void PlayMusic(const char *file) {
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	music = Mix_LoadMUS(file);
	Mix_PlayMusic(music, -1);
}

void Blit(SDL_Texture *texture, double x, double y) {
	SDL_Rect dest;
	dest.x = (int) x;
	dest.y = (int) y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

static void DrawTitle(void) {
	Blit(bg_texture, 0, 0);

	SDL_Surface *font_surface =
		TTF_RenderUTF8_Blended(font_middle,
							   "--------Tap “Space” to Start--------",
							   font_default_color);
	SDL_Texture *font_texture = SDL_CreateTextureFromSurface(app.renderer, font_surface);
	Blit(font_texture, 308, 450 + 30 * sin(2 * PI / 6222.2222 * SDL_GetTicks()));

	SDL_FreeSurface(font_surface);
	SDL_DestroyTexture(font_texture);
}

static void DoMenuInput(void) {
	SDL_Event menu_event;
	while (SDL_PollEvent(&menu_event)) {
		switch (menu_event.type) {
			case SDL_QUIT:
				app.escape = 1;
				return;
			case SDL_KEYDOWN:
				switch (menu_event.key.keysym.sym) {
					case SDLK_SPACE:
						puts("Game started.");
						PlayMusic("./resources/sounds/game.mp3");
						GameLogic();
						break;
					case SDLK_ESCAPE:
						app.escape = 1;
						break;
					default:
						break;
				}
				break;
				// DEBUG
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				printf("Clicked (%d, %d).\n", menu_event.button.x, menu_event.button.y);
				break;
			default:
				break;
		}
	}
}

void MaintainFPS(void) {
	long interval = (long) (INTERVAL_INT + frac_tick);
	frac_tick -= (int) frac_tick;
	long passed = (long) SDL_GetTicks() - next_tick;
	interval -= passed;
	if (interval < 1)
		interval = 1;
	SDL_Delay(interval);
	frac_tick += 0.3334;
	next_tick = (long) SDL_GetTicks();
}

void Quit(void) {
	puts("Exited.");

	SDL_DestroyTexture(bg_texture);

	TTF_CloseFont(font_big);
	TTF_CloseFont(font_middle);
	TTF_CloseFont(font_small);

	Mix_FreeMusic(music);
	Mix_FreeChunk(sounds[SND_JUMP]);
	Mix_FreeChunk(sounds[SND_DIE]);
	Mix_FreeChunk(sounds[SND_SHOOT]);
	Mix_CloseAudio();

	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}
