#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Physics.h"

// Box2D INCLUDE
#include <Box2D\Box2D.h>

class RandomWanderingBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;

	b2Vec2 velocity;

	int numCollisionsWithPlayer;
	bool isFollowingPlayer;



	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	// RandomWanderingBot(unsigned int initMin,
	//	unsigned int initMax,
	//	unsigned int initMaxVelocity);

public:
	RandomWanderingBot(unsigned int initMin, unsigned int initMax);
	~RandomWanderingBot();

	Bot*	clone();
	void	initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
	void	pickRandomCyclesInRange();
	void	pickRandomVelocity();
	void	think(Game *game);
	void	think(Game *game, Bot* bot, bool following);
	

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking; }
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking; }

	int		getNumCollisionsWithPlayer()			{ return numCollisionsWithPlayer; }
	void	increaseNumCollisionsWithPlayer()		{ numCollisionsWithPlayer++; }
	void	resetNumCollisionsWithPlayer()			{ numCollisionsWithPlayer = 0; }

	bool	getIsFollowingPlayer()					{ return isFollowingPlayer; }
	void	setIsFollowingPlayer(bool b)			{ isFollowingPlayer = b; }

	// DEPRACATED CONSTRUCTOR FROM PARENT CLASS
	RandomWanderingBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
};