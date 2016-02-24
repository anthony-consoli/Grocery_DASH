#pragma once

#include "sssf\text\TextGenerator.h"

class GroceryDashTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring textToGenerate;
	wstring pathfindingText;
	wstring inGameText;
	wstring inGameItemListText;
	wstring settingsScreenText;

public:
	// NOTHING TO INITIALIZE OR DESTROY
	GroceryDashTextGenerator()	{}
	~GroceryDashTextGenerator()	{}

	// DEFINED IN GroceryDashTextGenerator.cpp
	void appendMouseCoords(Game *game);
	void appendPathfindingText(Game *game);
	void appendInGameText(Game* game);
	void appendInGameItemList(Game* game);
	void appendSettingsScreenText(Game* game);
	void initText(Game *game);
	void updateText(Game *game);
};