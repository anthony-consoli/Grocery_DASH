#pragma once
#include "grocery_DASH_VS\stdafx.h"
#include "sssf\game\Game.h"

#include <iostream>

#include "LuaPlus.h"

using namespace LuaPlus;
#include "stdio.h"
using namespace std;

void getAndPrintSpeed(LuaState* ls);
void runScript(Game* game);

void runScript_TimeCheat(Game* game, bool isTimeIncreasing);
void runScript_LookUpShelfItemInfo(Game *game, double ShelfNumber, double itemNumber, int isTakingItem);

//DEPRACATED FUNCTION
void runScript_DoNotUse(Game *game, int ShelfNumber, int itemNumber);

