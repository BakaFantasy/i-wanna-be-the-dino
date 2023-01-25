#ifndef I_WANNA_BE_THE_DINO__GAME_H_
#define I_WANNA_BE_THE_DINO__GAME_H_

/**
 * Determine the whole game process
 */
void GameLogic(void);

static void InitGame(void);

/**
 * Just used at beginning
 */
static void Pause(void);

/**
 *
 * @return If collision occurs, return 1, else return 0.
 */
static bool DetectCollision(void);

static void GameOver(void);

/**
 * Control the scrolling speed of map. Linear at first, logarithmic then.
 */
static double SpeedUp(void);

static void SwitchMode(void);

/**
 * Display the hint text after mode change.
 */
static void DisplayHint(void);

/**
 *
 * @param bg sky or ground.
 * @param num_of_screens depends on the width of image.
 */
static void MoveBackground(Background *bg, int num_of_screens);

/**
 * Produce blood particles.
 */
static void SpoutBlood(double x, double y);

/**
 * Simulate death effect in I Wanna Be the Guy.
 */
static void DoParticles();

static void DoScore(void);

static void DoPlayer(void);

static void DoInput(void);

static void DoKeyDown(const SDL_KeyboardEvent *event);

static void DoKeyUp(const SDL_KeyboardEvent *event);

static void DoEscape(void);

#endif //I_WANNA_BE_THE_DINO__GAME_H_