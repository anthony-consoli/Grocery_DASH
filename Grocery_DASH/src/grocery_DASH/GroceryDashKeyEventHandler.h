#pragma once

#include "sssf\input\KeyEventHandler.h"

class GroceryDashKeyEventHandler : public KeyEventHandler{

private:
	unsigned int MOVE_UP_CONTROL;
	unsigned int MOVE_DOWN_CONTROL;
	unsigned int MOVE_LEFT_CONTROL;
	unsigned int MOVE_RIGHT_CONTROL;

	unsigned int VIEWPORT_UP_CONTROL;
	unsigned int VIEWPORT_DOWN_CONTROL;
	unsigned int VIEWPORT_LEFT_CONTROL;
	unsigned int VIEWPORT_RIGHT_CONTROL;

	bool isWASDmovement;

public:

	bool getIsWASDmovement()	{ return isWASDmovement; }

	~GroceryDashKeyEventHandler() {}

	GroceryDashKeyEventHandler();
	void toggleMovementControls();
	
	void handleKeyEvents(Game *game);
	
};