#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\TopDownSprite.h"

class Bot : public TopDownSprite
{
public:
	Bot()	{}

	// TO BE DEFINED BY BOT AI CLASSES
	virtual void think(Game *game)=0;
	virtual Bot* clone()=0;
};