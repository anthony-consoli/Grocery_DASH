#include "grocery_DASH_VS\stdafx.h"
#include "GroceryDashMouseEventHandler.h"

#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Viewport.h"

void GroceryDashMouseEventHandler::handleMousePressEvent(Game *game, int mouseX, int mouseY)
{
	if (game->getGSM()->isGameInProgress())
	{
		Viewport *viewport = game->getGUI()->getViewport();

		// DETERMINE WHERE ON THE MAP WE HAVE CLICKED
		int worldCoordinateX = mouseX + viewport->getViewportX();
		int worldCoordinateY = mouseY + viewport->getViewportY();

		// NOW LET'S SEND THE PLAYER THERE
		SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
		AnimatedSprite *player = spriteManager->getPlayer();
		GridPathfinder *pathfinder = spriteManager->getPathfinder();

		if (game->getGSM()->isPlayerControlDisabled() == false){
			//pathfinder->mapPath(player, (float)worldCoordinateX, (float)worldCoordinateY);
		}
	}
}

void GroceryDashMouseEventHandler::handleMouseMoveEvent(Game *game, int mouseX, int mouseY)
{
	// DETERMINE WHAT THE PATHFINDING GRID CELL
	// IS THAT THE MOUSE CURRENTLY IS ABOVE
	if (game->getGSM()->isGameInProgress())
	{
		// IF YOU LIKE YOU COULD DO SOME MOUSE OVER DEBUGGING STUFF HERE
	}
}
