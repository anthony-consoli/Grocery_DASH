/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.cpp

	See GameStateManager.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\sprite\SpriteManager.h"

//DUE TO 'CIRCULAR REFERENCE ERROR', this include must be in the .cpp file
#include "..\..\..\..\..\Grocery_DASH\src\grocery_DASH\GroceryDashDataLoader.h"

/*
	GameStateManager - Default Constructor, it starts the app at the
	splash screen with no level loaded.
*/
GameStateManager::GameStateManager()	
{
	spriteManager = new SpriteManager();
	gameAudio = new GroceryDashAudio();
	currentGameState = GS_SPLASH_SCREEN;
	currentLevel = NO_LEVEL_LOADED;
	levelClear = true;
	levelTimeAllowed = NO_LEVEL_LOADED;
	timePaused = 0;
	wasPaused = hasSetCheckoutTime = false;
	gameAudio->playAudio(0);


}

/*
	~GameStateManager - This destructor should inform the game world and
	sprite managers to clean up all their allocated memory.
*/
GameStateManager::~GameStateManager()	
{
	delete spriteManager;
}

/*
	addGameRenderItemsToRenderList - This method adds all the world and sprite
	items in the viewport to the render list
*/
void GameStateManager::addGameRenderItemsToRenderList(Game *game)
{
	// FIRST THE STATIC WORL
	world.addWorldRenderItemsToRenderList(game);

	// THEN THE SPRITE MANAGER
	spriteManager->addSpriteItemsToRenderList(game);
}


/*
	goToGame - This method transitions the game application from the level loading
	to the actualy game.
*/
void GameStateManager::goToGame()
{
	currentGameState = GS_GAME_IN_PROGRESS;
	endPauseTimer();
}

/*
	goToLoadLevel - This method transitions the game application from main menu
	to the level loading.
*/
void GameStateManager::goToLoadLevel()
{
	currentGameState = GS_STARTING_LEVEL;
}

/*
	goToMainMenu - This method transitions the game application from the splash
	screen to the main menu.
*/
void GameStateManager::goToMainMenu()
{
	currentGameState = GS_MAIN_MENU;
	getAudio()->getVoice(6)->Stop();
	getAudio()->getVoice(7)->Stop();
	getAudio()->getVoice(8)->Stop();
	getAudio()->getVoice(6)->FlushSourceBuffers();
	getAudio()->getVoice(7)->FlushSourceBuffers();
	getAudio()->getVoice(8)->FlushSourceBuffers();
	getAudio()->playAudio(0);
	timePaused = 0;
	this->unloadCurrentLevel();
}

void GameStateManager::goToLevelSelect()
{
	currentGameState = GS_LEVEL_SELECT;
	this->unloadCurrentLevel();
	
}

void GameStateManager::goToHelp()
{
	currentGameState = GS_MENU_HELP_MENU;
}

void GameStateManager::goToAbout()
{
	currentGameState = GS_MENU_ABOUT_MENU;
}

void GameStateManager::goToStats()
{
	currentGameState = GS_STATS_SCREEN;
}

void GameStateManager::goToAchievements()
{
	currentGameState = GS_ACHIEVEMENTS_SCREEN;
}

void GameStateManager::goToSettings()
{
	currentGameState = GS_SETTINGS_SCREEN;
}

void GameStateManager::goToPauseMenu(){
	currentGameState = GS_PAUSED;
	startPauseTimer();
}

void GameStateManager::goToInGameHelp(){
	currentGameState = GS_IN_GAME_HELP;
}

void GameStateManager::goToItemList(){
	currentGameState = GS_ITEM_LIST_SCREEN;
}

void GameStateManager::goToCartScreen(){
	currentGameState = GS_CART_SCREEN;
}

void GameStateManager::goToVictoryScreen(){
	currentGameState = GS_VICTORY;
	startPauseTimer();
}

void GameStateManager::goToGameOverScreen(){
	currentGameState = GS_GAME_OVER;
	startPauseTimer();
}



void GameStateManager::goToShelfScreen(int shelfNum)
{
	switch (shelfNum){
	case 1:{	currentGameState = GS_GIP_SHELF_SCREEN_1;	}
		break;
	case 2:{	currentGameState = GS_GIP_SHELF_SCREEN_2;	}
		break;
	case 3:{	currentGameState = GS_GIP_SHELF_SCREEN_3;	}
		break;
	case 4:{	currentGameState = GS_GIP_SHELF_SCREEN_4;	}
		break;
	case 5:{	currentGameState = GS_GIP_SHELF_SCREEN_5;	}
		break;
	case 6:{	currentGameState = GS_GIP_SHELF_SCREEN_6;	}
		break;
	case 7:{	currentGameState = GS_GIP_SHELF_SCREEN_7;	}
		break;
	case 8:{	currentGameState = GS_GIP_SHELF_SCREEN_8;	}
		break;
	case 9:{	currentGameState = GS_GIP_SHELF_SCREEN_9;	}
		break;
	case 10:{	currentGameState = GS_GIP_SHELF_SCREEN_10;	}
		break;
	case 11:{	currentGameState = GS_GIP_SHELF_SCREEN_P1;	}
		break;
	case 12:{	currentGameState = GS_GIP_SHELF_SCREEN_P2;	}
		break;
	default:{	/*DON"T GO TO A SHELF SCREEN*/}
		break;
	
	}//SWITCH BRACKET
}


/*
	isAtSplashScreen - Used to test if this application is currently
	at the splash screen. This will dictate what to render, but also
	how to respond to user input.
*/
bool GameStateManager::isAtSplashScreen()
{
	return currentGameState == GS_SPLASH_SCREEN;
}

bool GameStateManager::isAtMainMenu(){
	return currentGameState == GS_MAIN_MENU;
}

bool GameStateManager::isAtLevelSelect(){
	return currentGameState == GS_LEVEL_SELECT;
}

bool GameStateManager::isAtHelpScreen(){
	return currentGameState == GS_MENU_HELP_MENU;
}

bool GameStateManager::isAtAboutScreen(){
	return currentGameState == GS_MENU_ABOUT_MENU;
}

bool GameStateManager::isAtSettingsScreen(){
	return currentGameState == GS_SETTINGS_SCREEN;
}

bool GameStateManager::isAtStatsScreen(){
	return currentGameState == GS_STATS_SCREEN;
}

bool GameStateManager::isAtAchievementsScreen(){
	return currentGameState == GS_ACHIEVEMENTS_SCREEN;
}

bool GameStateManager::isAtPauseMenu(){
	return currentGameState == GS_PAUSED;
}

bool GameStateManager::isAtInGameHelp(){
	return currentGameState == GS_IN_GAME_HELP;
}

bool GameStateManager::isAtItemList(){
	return currentGameState == GS_ITEM_LIST_SCREEN;
}

bool GameStateManager::isAtCartScreen(){
	return currentGameState == GS_CART_SCREEN;
}

bool GameStateManager::isAtVictoryScreen(){
	return currentGameState == GS_VICTORY;
}

bool GameStateManager::isAtGameOverScreen(){
	return currentGameState == GS_GAME_OVER;
}

/*
	isAppActive - Used to test if this application is going to continue
	running for another frame. This will return true if the game app is
	not closing, false otherwise. Note that when the currentGameState
	becomes GS_EXIT_GAME, this will return true, and the game loop will
	end.
*/
bool GameStateManager::isAppActive()
{
	return currentGameState != GS_EXIT_GAME;
}

/*
	isGameInProgress - Used to test if the game is running right now, which
	means we have to execute all game logic.
*/
bool GameStateManager::isGameInProgress()
{
	if (currentGameState == GS_GAME_IN_PROGRESS || currentGameState == GS_GIP_SHELF_SCREEN_1 ||
		currentGameState == GS_GIP_SHELF_SCREEN_2 || currentGameState == GS_GIP_SHELF_SCREEN_3 ||
		currentGameState == GS_GIP_SHELF_SCREEN_4 || currentGameState == GS_GIP_SHELF_SCREEN_5 ||
		currentGameState == GS_GIP_SHELF_SCREEN_6 || currentGameState == GS_GIP_SHELF_SCREEN_7 ||
		currentGameState == GS_GIP_SHELF_SCREEN_8 || currentGameState == GS_GIP_SHELF_SCREEN_9 ||
		currentGameState == GS_GIP_SHELF_SCREEN_10 || currentGameState == GS_GIP_SHELF_SCREEN_P1 ||
		currentGameState == GS_GIP_SHELF_SCREEN_P2 || currentGameState == GS_ITEM_LIST_SCREEN ||
		currentGameState == GS_CART_SCREEN			|| currentGameState == GS_VICTORY ||
		currentGameState == GS_GAME_OVER){

		return true;
	}
	else{
		return false;
	}
}

bool GameStateManager::isPlayerControlDisabled()
{
	if (currentGameState == GS_GIP_SHELF_SCREEN_1	||	currentGameState == GS_GIP_SHELF_SCREEN_10 ||
		currentGameState == GS_GIP_SHELF_SCREEN_2	||	currentGameState == GS_GIP_SHELF_SCREEN_3 ||
		currentGameState == GS_GIP_SHELF_SCREEN_4	||	currentGameState == GS_GIP_SHELF_SCREEN_5 ||
		currentGameState == GS_GIP_SHELF_SCREEN_6	||	currentGameState == GS_GIP_SHELF_SCREEN_7 ||
		currentGameState == GS_GIP_SHELF_SCREEN_8	||	currentGameState == GS_GIP_SHELF_SCREEN_9 ||
		currentGameState == GS_GIP_SHELF_SCREEN_P1	||	currentGameState == GS_GIP_SHELF_SCREEN_P2 ||
		currentGameState == GS_PAUSED				||	currentGameState == GS_IN_GAME_HELP ||
		currentGameState == GS_ITEM_LIST_SCREEN		||  currentGameState == GS_CART_SCREEN ||
		currentGameState == GS_VICTORY				|| currentGameState == GS_GAME_OVER){

		return true;
	}
	else{
		return false;
	}
}

bool GameStateManager::isPlayerAtShelf()
{
	if (currentGameState == GS_GIP_SHELF_SCREEN_1	||	currentGameState == GS_GIP_SHELF_SCREEN_10 ||
		currentGameState == GS_GIP_SHELF_SCREEN_2	||	currentGameState == GS_GIP_SHELF_SCREEN_3 ||
		currentGameState == GS_GIP_SHELF_SCREEN_4	||	currentGameState == GS_GIP_SHELF_SCREEN_5 ||
		currentGameState == GS_GIP_SHELF_SCREEN_6	||	currentGameState == GS_GIP_SHELF_SCREEN_7 ||
		currentGameState == GS_GIP_SHELF_SCREEN_8	||	currentGameState == GS_GIP_SHELF_SCREEN_9 ||
		currentGameState == GS_GIP_SHELF_SCREEN_P1	||	currentGameState == GS_GIP_SHELF_SCREEN_P2 ){

		return true;
	}
	else{
		return false;
	}
}

/*
	isGameLevelLoading - Used to test if the game is loading a level right now.
*/
bool GameStateManager::isGameLevelLoading()
{
	return currentGameState == GS_STARTING_LEVEL;
}

/*
	isWorldRenderable - Used to test if the game world should be rendered
	or not. Note that even if the game is paused, you'll likely still render
	the game.
*/
bool GameStateManager::isWorldRenderable()
{
	return (	(isGameInProgress())
		||		(currentGameState == GS_PAUSED)
		||		(currentGameState == GS_GAME_OVER));
}

/*
	addLevelFileName - This method adds a level file name to the vector
	of all the level file names. Storing these file names allows us to
	easily load a desired level at any time.
*/
void GameStateManager::addLevel(wstring levelToAddName, wstring levelToAddFileNameAndPath)
{
	levelNames.push_back(levelToAddName);
	levelFileNamesWithRelativePath.push_back(levelToAddFileNameAndPath);
}

/*
	getLevelNum - Note that when our game app starts, we load all the level
	file names and this should never change. This method looks through those
	names and gets the index in the vector for a given level name. Note that
	we're using a little iterator/pointer arithmetic here since vectors
	guarantee they will store their contents in a continuous block of memory.
*/
unsigned int GameStateManager::getLevelNum(wstring levelName)
{
	return find(levelNames.begin(),levelNames.end(), levelName) - levelNames.begin();
}

/*
	loadLevel - This method changes the current level. This method should
	be called before loading all the data from a level file.
*/
void GameStateManager::loadLevel(Game *game, int initLevel)
{
	if ((initLevel > NO_LEVEL_LOADED) && (initLevel <= levelNames.size()))
	{
		if (currentLevel != NO_LEVEL_LOADED)
			unloadCurrentLevel();
		currentLevel = initLevel;
		wstring fileToLoad = levelFileNamesWithRelativePath[currentLevel];
		GameDataLoader *dataLoader = game->getDataLoader();
		dataLoader->loadWorld(game, fileToLoad);
	}
}

/*
	loadLevel - This is just an overladed version of our loadLevel method that
	uses the name of the level rather than its index. Some games may have non-
	linear levels, and so the developers may prefer to hold onto the level names
	rather than numbers.
*/
void GameStateManager::loadLevel(Game *game, wstring levelName)
{
	unsigned int levelIndex = getLevelNum(levelName);
	loadLevel(game, levelIndex);
}

/*
	shutdown - this method is called when the user wants to quit the
	application. This method updates the game state such that all
	world resources are released and the game loop does not iterate
	again.
*/
void GameStateManager::shutdown()
{
	// MAKE SURE THE GAME LOOP DOESN'T GO AROUND AGAIN
	currentGameState = GS_EXIT_GAME;

	// CLEAR LEFT OVER DATA
	if (isWorldRenderable())
		unloadCurrentLevel();
}

/*
	unloadCurrentLevel - This method removes all data from the World, recovering
	all used memory. It should be called first when a level is unloaded or changed. 
	If it is not called, an application runs the risk of having memory leaking,
	i.e. extra data sitting around that may slow the progam down. Or, if the app
	thinks a level is still active, it might add items to the render list using 
	image ids that have already been cleared from the GameGraphics' texture manager 
	for the world. That would likely result in an exception.
*/
void GameStateManager::unloadCurrentLevel()
{
	spriteManager->unloadSprites();
	world.unloadWorld();
	currentLevel = NO_LEVEL_LOADED;
}

/*
	update - This method should be called once per frame. It updates
	both the sprites and the game world. Note that even though the game
	world is for static data, should the user wish to put anything dynamic
	there (like a non-collidable moving layer), the updateWorld method
	is called.
*/
void GameStateManager::update(Game *game)
{

	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());

	//CHECK IF THE GAME CLOCK HAS EXPIRED
	//CHECK IF THE GAME CLOCK HAS EXPIRED
	if (currentGameState == GS_PAUSED || (currentGameState == GS_VICTORY)){
		levelTimeAllowed++;
	}
	else{
		checkLevelTimer();
	}

	if (dataLoader->getIsPayingCashier() == true){
		startCheckoutTimer(game);
	}
	currentCheckout = clock();
	endCheckoutTimer(game, currentCheckout);

	spriteManager->update(game);
	world.update(game);

	game->getGSM()->getPhysics()->box2DWorld.Step((1.0f / 60.0f), 8, 3);

	//REMOVE DEPARTED CUSTOMERS
	game->getGSM()->getSpriteManager()->removeDeadBots();

	if (levelClear == true && currentGameState != NO_LEVEL_LOADED){
		game->getDataLoader()->clearLevelData(game);
		levelTimeAllowed = NO_LEVEL_LOADED;
	}

	if (isGameInProgress() == true && game->getGSM()->getSpriteManager()->getNumberOfSprites() < MAX_NUM_SPRITES){
		//dataLoader->generateRandomCustomers(game);
	}
	
}

int	GameStateManager::getGameStateShelfAsNumber(){
	if (currentGameState == GS_GIP_SHELF_SCREEN_1){		return 1;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_2){		return 2;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_3){		return 3;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_4){		return 4;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_5){		return 5;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_6){		return 6;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_7){		return 7;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_8){		return 8;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_9){		return 9;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_10){	return 10;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_P1){	return 11;	}
	if (currentGameState == GS_GIP_SHELF_SCREEN_P2){	return 12;	 }


	return -1;
	
}

void GameStateManager::startLevelTimer(){
	beginLevel = clock();
}

double GameStateManager::getTimeRemaining(){
	currentTime = clock();
	return (double)(currentTime - beginLevel);

}

void GameStateManager::checkLevelTimer(){

	if (levelTimeAllowed != NO_LEVEL_LOADED){

		int timeRemainingInSeconds = (int)(getTimeRemaining()/1000);
		if (levelTimeAllowed - timeRemainingInSeconds < 0){
			//levelClear = true;
			goToGameOverScreen();
			clearLevelTimeAllowed();
		}

	}
	
}

void GameStateManager::startPauseTimer(){
	beginPause = clock();
	timePaused = 0;
	wasPaused = true;
}

void GameStateManager::endPauseTimer(){

	if (wasPaused == true){
		endPause = clock();
		double timeElapsed = (double)(endPause - beginPause);
		timePaused = timeElapsed / 1000;

		levelTimeAllowed += timePaused;
		timePaused = 0;
		wasPaused = false;
	}
	
}

void GameStateManager::clearPauseData(){
	wasPaused = false;
	timePaused = 0;
}

void GameStateManager::startCheckoutTimer(Game* game){
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());

	if (hasSetCheckoutTime == false){
		beginCheckout = clock();
		long addTime = (dataLoader->numberOfItems * SECONDS_PER_ITEM * 1000);
		endCheckout = (beginCheckout) + (addTime);
		hasSetCheckoutTime = true;
	}

}

void GameStateManager::endCheckoutTimer(Game* game, clock_t currentCheckoutTime){
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());

	currentCheckout = (long)(currentCheckoutTime);

	if ( dataLoader->getIsPayingCashier() == true && currentCheckout >= endCheckout){
		dataLoader->setIsPayingCashier(false);
		dataLoader->setHasPaidCashier(true);
		hasSetCheckoutTime = false;
		game->getGSM()->getAudio()->playAudio(4);
	}
	if (dataLoader->getIsPayingCashier() == true && currentCheckout % CHECKOUT_ITEM_TIME <= 30)
	{
		game->getGSM()->getAudio()->playAudio(9);
	}


}