#ifndef I_WANNA_BE_THE_DINO__SPRITES_H_
#define I_WANNA_BE_THE_DINO__SPRITES_H_

void DoSprites(LL *ll);

void SpawnSprite(LL *ll, double *spawn_counter);

static Type DecideSprite(void);

void InitSprite(Entity *sprite, enum Type type);

static void AimAtPlayer(Entity *sprite);

void DrawSprites(LL *ll);

void ClearLL(LL *ll);

#endif //I_WANNA_BE_THE_DINO__SPRITES_H_
