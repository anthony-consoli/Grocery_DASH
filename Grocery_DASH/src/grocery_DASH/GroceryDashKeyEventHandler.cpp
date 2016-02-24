// Box2D INCLUDE
#include <Box2D\Box2D.h>

#include "grocery_DASH\GroceryDash.h"
#include "grocery_DASH\GroceryDashDataLoader.h"
#include "grocery_DASH\GroceryDashKeyEventHandler.h"
#include "grocery_DASH\GroceryDashLuaScripting.h"


#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

GroceryDashKeyEventHandler::GroceryDashKeyEventHandler(){

	 MOVE_UP_CONTROL = VK_UP;
	 MOVE_DOWN_CONTROL = VK_DOWN;
	 MOVE_LEFT_CONTROL = VK_LEFT;
	 MOVE_RIGHT_CONTROL = VK_RIGHT;

	 VIEWPORT_UP_CONTROL = W_KEY;
	 VIEWPORT_DOWN_CONTROL = S_KEY;
	 VIEWPORT_LEFT_CONTROL = A_KEY;
	 VIEWPORT_RIGHT_CONTROL = D_KEY;

	 isWASDmovement = false;
}

void GroceryDashKeyEventHandler::toggleMovementControls(){

	if (isWASDmovement == false){

		// Make WASD control player movement
		MOVE_UP_CONTROL = W_KEY;
		MOVE_DOWN_CONTROL = S_KEY;
		MOVE_LEFT_CONTROL = A_KEY;
		MOVE_RIGHT_CONTROL = D_KEY;

		// Make ARROW KEYS control the viewport
		VIEWPORT_UP_CONTROL = VK_UP;
		VIEWPORT_DOWN_CONTROL = VK_DOWN;
		VIEWPORT_LEFT_CONTROL = VK_LEFT;
		VIEWPORT_RIGHT_CONTROL = VK_RIGHT;

		// Set this variabe appropriately
		isWASDmovement = true;
	}
	else{ //isWASDmovement == true

		// Make ARROW KEYS control player movement
		MOVE_UP_CONTROL = VK_UP;
		MOVE_DOWN_CONTROL = VK_DOWN;
		MOVE_LEFT_CONTROL = VK_LEFT;
		MOVE_RIGHT_CONTROL = VK_RIGHT;

		// Make WASD control the viewport
		VIEWPORT_UP_CONTROL = W_KEY;
		VIEWPORT_DOWN_CONTROL = S_KEY;
		VIEWPORT_LEFT_CONTROL = A_KEY;
		VIEWPORT_RIGHT_CONTROL = D_KEY;

		// Set this variabe appropriately
		isWASDmovement = false;
	}
}

void GroceryDashKeyEventHandler::handleKeyEvents(Game *game){


	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	TopDownSprite *player = gsm->getSpriteManager()->getPlayer();
	b2Body *playerBody = game->getGSM()->getSpriteManager()->playerBody;
	PhysicalProperties *pp = player->getPhysicalProperties();
	Viewport *viewport = game->getGUI()->getViewport();
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());

	if (!gsm->isGameInProgress()){

		if (input->isKeyDownForFirstTime(ESC_KEY)){
			if (gsm->isAtInGameHelp() == true){
				gsm->goToPauseMenu();
			}
			else{
				if (gsm->isAtSettingsScreen() == true || gsm->isAtAboutScreen() == true || gsm->isAtAchievementsScreen() == true
					|| gsm->isAtHelpScreen() == true || gsm->isAtLevelSelect() == true || gsm->isAtStatsScreen() == true){
					gsm->goToMainMenu();
				}
			}
		}

	}//BRACKET -- !isGameInProgress

	if (gsm->isAtPauseMenu()){

		if (input->isKeyDownForFirstTime(R_KEY))
		{
			gsm->goToGame();
		}
	}



	if (gsm->isGameInProgress())
	{

		if (input->isKeyDownForFirstTime(P_KEY))
		{
			gsm->goToPauseMenu();
		}

		if (input->isKeyDownForFirstTime(L_KEY))
		{
			if ( gsm->isAtItemList() == false){
				gsm->goToItemList();
			}
			else{
				gsm->goToGame();
			}
		}

		if (input->isKeyDownForFirstTime(C_KEY))
		{
			if (gsm->isAtCartScreen() == false){
				gsm->goToCartScreen();
			}
			else{
				gsm->goToGame();
			}
		}

		if (input->isKeyDownForFirstTime(M_KEY)){
			runScript(game);
		}



		// ARROW KEYS PRESSES WILL CONTROL THE PLAYER

		// THE USER MOVES VIA MOUSE BUTTON PRESSES
		if (input->isKeyDown(MOVE_LEFT_CONTROL) && gsm->isPlayerControlDisabled() == false && dataLoader->getIsPayingCashier() == false)
		{
			player->clearPath();
			b2Vec2 moveLeft(-PLAYER_SPEED, 0.0f);
			playerBody->ApplyForceToCenter(moveLeft, true);
			playerBody->SetTransform(playerBody->GetPosition(), (1.0f * PI));
		}
		else if (input->isKeyDown(MOVE_RIGHT_CONTROL) && gsm->isPlayerControlDisabled() == false && dataLoader->getIsPayingCashier() == false)
		{
			player->clearPath();
			b2Vec2 moveRight(PLAYER_SPEED, 0.0f);
			playerBody->ApplyForceToCenter(moveRight, true);
			playerBody->SetTransform(playerBody->GetPosition(), (0.0f * PI));
		}
		else if (input->isKeyDown(MOVE_UP_CONTROL) && gsm->isPlayerControlDisabled() == false && dataLoader->getIsPayingCashier() == false)
		{
			player->clearPath();
			b2Vec2 moveUp(0.0f, -PLAYER_SPEED);
			playerBody->ApplyForceToCenter(moveUp, true);
			playerBody->SetTransform(playerBody->GetPosition(), (1.5f * PI));
			
		}
		else if (input->isKeyDown(MOVE_DOWN_CONTROL) && gsm->isPlayerControlDisabled() == false && dataLoader->getIsPayingCashier() == false)
		{
			player->clearPath();
			b2Vec2 moveDown(0.0f, PLAYER_SPEED);
			playerBody->ApplyForceToCenter(moveDown, true);
			playerBody->SetTransform(playerBody->GetPosition(), (0.5f * PI));
		}
		else if (input->isKeyDownForFirstTime(G_KEY))
		{
			viewport->toggleDebugView();
			game->getGraphics()->toggleDebugTextShouldBeRendered();
		}
		else if (input->isKeyDownForFirstTime(H_KEY))
		{
			game->getGraphics()->toggleB2DrawDebugShouldBeRendered();
		}
		else
		{
			b2Vec2 stay(0.0f, 0.0f);
			playerBody->ApplyForceToCenter(stay, false);
		}



		if (input->isKeyDownForFirstTime(ESC_KEY) && (gsm->isPlayerAtShelf() == true || gsm->isAtItemList() == true)){
			gsm->goToGame();
		}

		if (gsm->isPlayerAtShelf() == true){

			// LEAVE THE SHELF SCREEN without TAKING ANY ITEMS
			if (input->isKeyDownForFirstTime(ESC_KEY)){
				gsm->goToGame();
			}

			int isItemTaken = false;

			if (input->isKeyDownForFirstTime(NUM_1) ^ input->isKeyDownForFirstTime(NUMPAD_1)){
				isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 1);
				runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 1, isItemTaken);
				gsm->goToGame();
			}
			else{
				if (input->isKeyDownForFirstTime(NUM_2) ^ input->isKeyDownForFirstTime(NUMPAD_2)){
					isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 2);
					runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 2, isItemTaken);
					gsm->goToGame();
				}
				else{
					if (input->isKeyDownForFirstTime(NUM_3) ^ input->isKeyDownForFirstTime(NUMPAD_3)){
						isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 3);
						runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 3, isItemTaken);
						gsm->goToGame();
					}
					else{
						if (input->isKeyDownForFirstTime(NUM_4) ^ input->isKeyDownForFirstTime(NUMPAD_4)){
							isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 4);
							runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 4, isItemTaken);
							gsm->goToGame();
						}
						else{
							if (input->isKeyDownForFirstTime(NUM_5) ^ input->isKeyDownForFirstTime(NUMPAD_5)){
								isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 5);
								runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 5, isItemTaken);
								gsm->goToGame();
							}
							else{
								if (input->isKeyDownForFirstTime(NUM_6) ^ input->isKeyDownForFirstTime(NUMPAD_6)){
									isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 6);
									runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 6, isItemTaken);
									gsm->goToGame();
								}
								else{
									if (input->isKeyDownForFirstTime(NUM_7) ^ input->isKeyDownForFirstTime(NUMPAD_7)){
										isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 7);
										runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 7, isItemTaken);
										gsm->goToGame();
									}
									else{
										if (input->isKeyDownForFirstTime(NUM_8) ^ input->isKeyDownForFirstTime(NUMPAD_8)){
											isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 8);
											runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 8, isItemTaken);
											gsm->goToGame();
										}
										else{
											//DO NOTHING
			}	}	}	}	}	}	}	}
		}


		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;
		if (input->isKeyDown(VIEWPORT_UP_CONTROL) && gsm->isPlayerControlDisabled() == false)
		{
			viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(VIEWPORT_DOWN_CONTROL) && gsm->isPlayerControlDisabled() == false)
		{
			viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(VIEWPORT_LEFT_CONTROL) && gsm->isPlayerControlDisabled() == false)
		{
			viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (input->isKeyDown(VIEWPORT_RIGHT_CONTROL) && gsm->isPlayerControlDisabled() == false)
		{
			viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			viewportMoved = true;
		}
		if (!viewportMoved){
			b2Vec2 playerPos = playerBody->GetPosition();
			float playerX = playerPos.x * SCALING_FACTOR;
			float playerY = playerPos.y * SCALING_FACTOR;
			int SCREEN_WIDTH = 1024;
			int SCREEN_HEIGHT = 768;
			if (viewport->getViewportX() + (viewport->getViewportWidth() * 15 / 32) < playerX){
				viewportVx += MAX_VIEWPORT_AXIS_VELOCITY *
					abs(viewport->getViewportX() + (viewport->getViewportWidth() / 2) - playerX) / (SCREEN_WIDTH / 2);
				viewportMoved = true;
			}
			if (viewport->getViewportX() + (viewport->getViewportWidth() * 17 / 32) > playerX){
				viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY *
					abs(viewport->getViewportX() + (viewport->getViewportWidth() / 2) - playerX) / (SCREEN_WIDTH / 2);
				viewportMoved = true;
			}
			if (viewport->getViewportY() + (viewport->getViewportHeight() * 15 / 32) < playerY){
				viewportVy += MAX_VIEWPORT_AXIS_VELOCITY *
					abs(viewport->getViewportY() + (viewport->getViewportHeight() / 2) - playerY) / (SCREEN_HEIGHT / 2);
				viewportMoved = true;
			}
			if (viewport->getViewportY() + (viewport->getViewportHeight() * 17 / 32) > playerY){
				viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY *
					abs(viewport->getViewportY() + (viewport->getViewportHeight() / 2) - playerY) / (SCREEN_HEIGHT / 2);
				viewportMoved = true;
			}
		}
		//Viewport *viewport = game->getGUI()->getViewport();
		if (viewportMoved){
			viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), game->getGSM()->getWorld()->getWorldWidth(), game->getGSM()->getWorld()->getWorldHeight());
		}
		
		/***********	CHEAT CODES LISTED BELOW *******
		
		
		************************************************/

		// INCREASE TIME REMAINING
		if (input->isKeyDownForFirstTime(I_KEY)){
			runScript_TimeCheat(game, true);
		}//isIncreasingTime


		// DECREASE TIME REMAINING
		if (input->isKeyDownForFirstTime(O_KEY)){
			runScript_TimeCheat(game, false);
		}

		// INCREASE SPENDING LIMIT
		if (input->isKeyDownForFirstTime(Y_KEY)){
			if ((dataLoader->spendingLimit + MONEY_CHEAT_INCREASE) > (dataLoader->levelItemsCost * 2) + MONEY_CHEAT_INCREASE){
				dataLoader->spendingLimit = (dataLoader->levelItemsCost * 2) + MONEY_CHEAT_INCREASE;
			}
			else{
				dataLoader->spendingLimit += MONEY_CHEAT_INCREASE;
			}
			
		}//isIncreasingSpendingLimit


		// DECREASE SPENDING LIMIT
		if (input->isKeyDownForFirstTime(U_KEY)){
			if ((dataLoader->spendingLimit - MONEY_CHEAT_DECREASE) < dataLoader->levelItemsCost){
				dataLoader->spendingLimit = dataLoader->levelItemsCost;
			}
			else{
				dataLoader->spendingLimit -= MONEY_CHEAT_DECREASE;
			}
		}//isDecreasingSpendingLimit

	}// BRACKET -- if (gsm->isGameInProgress())
}