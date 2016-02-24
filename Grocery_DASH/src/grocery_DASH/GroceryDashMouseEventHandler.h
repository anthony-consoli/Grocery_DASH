#pragma once

#include "sssf\game\Game.h"
#include "sssf\input\MouseEventHandler.h"

class GroceryDashMouseEventHandler : public MouseEventHandler
{
public:
	GroceryDashMouseEventHandler() {};
	~GroceryDashMouseEventHandler() {};

	void handleMousePressEvent(Game *game, int mouseX, int mouseY);
	void handleMouseMoveEvent(Game *game, int mouseX, int mouseY);
};

