#include "GroceryDash.h"
#include "GroceryDashDataLoader.h"
#include "GroceryDashButtonEventHandler.h"
#include "GroceryDashKeyEventHandler.h"
#include "grocery_DASH\GroceryDashLuaScripting.h"

#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"


void GroceryDashButtonEventHandler::handleButtonEvents(Game *game, wstring command)
{
	GameStateManager *gsm = game->getGSM();
	GameInput *input = game->getInput();
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());


	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_QUIT_COMMAND) == 0)
	{
		game->shutdown();
	}

	if (command.compare(W_EXIT_TO_MM_COMMAND) == 0)
	{
		// IF THE GAME IS NOT IN PROGRESS
		//if (game->getGSM()->getLevelClear() == true){
		if (!game->getGSM()->isGameInProgress()){
			gsm->goToMainMenu();
			dataLoader->destroyBox2DBodiesTiles(game);
			game->getGraphics()->clearWorldTextures();
		}
		else{
			game->getGSM()->setLevelClear(true);
		}
	}

	if (command.compare(W_EXIT_TO_LS_COMMAND) == 0)
	{
		gsm->goToLevelSelect();
	}

	if (command.compare(W_START_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToLevelSelect();
	}

	if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToMainMenu();
	}

	if (command.compare(W_GO_TO_STATS_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToStats();
	}

	if (command.compare(W_GO_TO_ACHIEVEMENTS_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToAchievements();
	}

	if (command.compare(W_GO_TO_SETTINGS_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToSettings();
	}

	if (command.compare(W_HELP_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToHelp();
	}

	if (command.compare(W_ABOUT_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToAbout();
	}

	if (command.compare(W_RETURN_TO_GAME_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToGame();
	}

	if (command.compare(W_RESTART_LEVEL_COMMAND) == 0)
	{
		wstring levelName = game->getGSM()->getCurrentLevelName();
		dataLoader->clearLevelData(game);
		game->setCurrentLevelFileName(levelName);
		game->startGame();
	}

	if (command.compare(W_GO_TO_IG_HELP_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToInGameHelp();
	}

	if (command.compare(W_GO_TO_ITEM_LIST_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToItemList();
	}

	if (command.compare(W_GO_TO_CART_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		gsm->goToCartScreen();
	}

	if (command.compare(W_TOGGLE_GENDER_COMMAND) == 0)
	{
		dataLoader->togglePlayerIsMale();
	}

	if (command.compare(W_TOGGLE_WASD_COMMAND) == 0)
	{
		GroceryDashKeyEventHandler* keh = dynamic_cast<GroceryDashKeyEventHandler*>(game->getInput()->getKeyEventHandler());
		keh->toggleMovementControls();
	}

	if (command.compare(W_LOAD_L1_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();
		game->getGSM()->goToItemList();
	}

	if (command.compare(W_LOAD_L2_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		game->setCurrentLevelFileName(W_LEVEL_2_NAME);
		game->startGame();
		game->getGSM()->goToItemList();
	}

	if (command.compare(W_LOAD_L3_COMMAND) == 0)
	{
		game->getGSM()->getAudio()->playAudio(1);
		game->setCurrentLevelFileName(W_LEVEL_3_NAME);
		game->startGame();
		game->getGSM()->goToItemList();
	}

	if (command.compare(W_LOAD_QP_COMMAND) == 0)
	{
		int random = rand() % 3 + 1;
		switch (random){
		case 1: {game->setCurrentLevelFileName(W_LEVEL_1_NAME); }
			break;
		case 2: {game->setCurrentLevelFileName(W_LEVEL_2_NAME); }
			break;
		case 3: {game->setCurrentLevelFileName(W_LEVEL_3_NAME); }
			break;
		default: {game->setCurrentLevelFileName(W_LEVEL_1_NAME); }
			break;
		}

		dataLoader->isQuickPlay = true;

		// ACCESS TO TEST LEVEL	
		//game->setCurrentLevelFileName(W_LEVEL_0_NAME);		
		
		game->startGame();
		game->getGSM()->goToItemList();
	}

	/****
		USED TO ADD ITEMS TO THE CART FROM THE SHELF SCREEN
	****/

	int isItemTaken = false;

	if (command.compare(W_ADD_ITEM_1_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 1);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 1, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_2_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 2);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 2, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_3_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 3);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 3, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_4_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 4);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 4, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_5_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 5);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 5, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_6_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 6);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 6, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_7_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 7);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 7, isItemTaken);
		gsm->goToGame();
	}
	if (command.compare(W_ADD_ITEM_8_TO_CART_COMMAND) == 0)
	{
		isItemTaken = dataLoader->takeItemFromShelf(game, gsm->getGameStateShelfAsNumber(), 7);
		runScript_LookUpShelfItemInfo(game, gsm->getGameStateShelfAsNumber(), 7, isItemTaken);
		gsm->goToGame();
	}
}