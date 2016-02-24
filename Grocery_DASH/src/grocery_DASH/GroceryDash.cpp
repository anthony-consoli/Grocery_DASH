/*

	Authors:	Andrew Allhusen
				Anthony Consoli
				Warren Chen

	Grocery_DASH.cpp

		This is a game made for the final project of CSE 380: "Computer Game Programming."  CSE 380 is
	an upper division course taught at Stony Brook University by Richard McKenna.  This game utilizes the
	SimpleSideScrollerFramework, TMXMapImporter, and PoseurSpriteTypeImporter, which were also developed my Richard McKenna.
	For physics, this game the uses Box2D physics engine.


*/

#include "grocery_DASH_VS\stdafx.h"

// Grocery_DASH GAME INCLUDES
#include "GroceryDash.h"
#include "GroceryDashDataLoader.h"
#include "grocery_DASH\GroceryDashButtonEventHandler.h"
#include "grocery_DASH\GroceryDashCollisionListener.h"
#include "grocery_DASH\GroceryDashKeyEventHandler.h"
#include "grocery_DASH\GroceryDashTextGenerator.h"
#include "grocery_DASH\GroceryDashMouseEventHandler.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

//VARIABLES
float PLAYER_SPEED = 100.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	// CREATE THE GAME
	Game *groceryDashGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ IN DATA TO SETUP OTHER STUFF
	GroceryDashDataLoader *groceryDashDataLoader = new GroceryDashDataLoader();
	groceryDashDataLoader->initWinHandle(hInstance, nCmdShow);
	groceryDashGame->setDataLoader(groceryDashDataLoader);
	groceryDashDataLoader->loadGame(groceryDashGame, W_INIT_FILE);

	//Load GUI
	groceryDashDataLoader->loadGUI(groceryDashGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	GroceryDashButtonEventHandler *groceryDashButtonHandler = new GroceryDashButtonEventHandler();
	GameGUI *gui = groceryDashGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)groceryDashButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	GroceryDashKeyEventHandler *groceryDashKeyHandler = new GroceryDashKeyEventHandler();
	groceryDashGame->getInput()->registerKeyHandler((KeyEventHandler*)groceryDashKeyHandler);

	// SPECIFY WHO WILL HANDLE MOUSE EVENTS NOT RELATED TO THE GUI
	GroceryDashMouseEventHandler *groceryDashMouseHandler = new GroceryDashMouseEventHandler();
	groceryDashGame->getInput()->registerMouseHandler((MouseEventHandler*)groceryDashMouseHandler);

	// SPECIFY THE COLLISION LISTENER
	GroceryDashCollisionListener *groceryDashCollisionListener = new GroceryDashCollisionListener(groceryDashGame);
	groceryDashGame->getGSM()->getPhysics()->box2DWorld.SetContactListener(groceryDashCollisionListener);

	//SPECIFY THE DEBUG DRAW CLASS FOR THE Box2DWorld
	groceryDashGame->getGSM()->getPhysics()->box2DWorld.SetDebugDraw(groceryDashGame->getGraphics());

	//SET SEED VALUE
	srand(time(NULL));

	// START THE GAME LOOP
	groceryDashGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)groceryDashGame->getOS();
	delete (WindowsInput*)groceryDashGame->getInput();
	delete (WindowsTimer*)groceryDashGame->getTimer();
	delete (DirectXGraphics*)groceryDashGame->getGraphics();
	delete (GroceryDashTextGenerator*)groceryDashGame->getText()->getTextGenerator();
	delete groceryDashButtonHandler;
	delete groceryDashKeyHandler;
	delete groceryDashGame;

	// AND RETURN
	return 0;
}