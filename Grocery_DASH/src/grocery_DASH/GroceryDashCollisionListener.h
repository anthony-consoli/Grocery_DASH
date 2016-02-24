#pragma once

// Box2D INCLUDE
#include <Box2D\Box2D.h>

#include "GroceryDash.h"
#include "GroceryDashDataLoader.h"

#include "sssf\game\Game.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\ai\bots\RandomWanderingBot.h"



class GroceryDashCollisionListener : public b2ContactListener {

private:
	Game* game;

public:

	GroceryDashCollisionListener(Game* initGame) { game = initGame; }
	~GroceryDashCollisionListener() {}

	Game* getGame()	{ return game; }

	/* handle begin event */
	void BeginContact(b2Contact* contact);

	/* handle end event */
	void EndContact(b2Contact* contact);

	/* handle pre-solve event */
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	/* handle post-solve event */
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	void handleBeginContactEventTile(b2Contact* contact, TopDownSprite* sprite, Tile* tile);
	void handleBeginContactEventSprites(b2Contact* contact, TopDownSprite* spriteA, TopDownSprite* spriteB);

	RandomWanderingBot* determineCustomerSprite(TopDownSprite* spriteA, TopDownSprite* spriteB);
	TopDownSprite* determinePlayerSprite(TopDownSprite* spriteA, TopDownSprite* spriteB);
	
};