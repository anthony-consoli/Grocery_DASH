#pragma once

#include "sssf\input\ButtonEventHandler.h"

class GroceryDashButtonEventHandler : public ButtonEventHandler {

public:
	GroceryDashButtonEventHandler() {}
	~GroceryDashButtonEventHandler() {}

	// DEFINED INSIDE GroceryDashButtonEventHandler.cpp
	void handleButtonEvents(Game *game,
		wstring command);
};