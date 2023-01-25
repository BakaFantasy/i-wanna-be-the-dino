#ifndef I_WANNA_BE_THE_DINO__IWBTD_H_
#define I_WANNA_BE_THE_DINO__IWBTD_H_

static void DoMenuInput(void);

SDL_Texture *LoadTexture(const char *file);

void PlayMusic(const char *file);

void Blit(SDL_Texture *texture, double x, double y);

static void DrawTitle(void);

void MaintainFPS(void);

void Quit(void);

#endif //I_WANNA_BE_THE_DINO__IWBTD_H_