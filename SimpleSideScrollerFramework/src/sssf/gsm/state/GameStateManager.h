/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.h

	This class manages the static data for the game level being
	used. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\world\World.h"
#include "sssf\gsm\world\WorldLayer.h"

#include "..\..\..\..\..\Grocery_DASH\src\grocery_DASH\SDKwavefile.h"
#include "..\..\..\..\..\Grocery_DASH\src\grocery_DASH\GroceryDashAudio.h"

#include <chrono>

class Game;

static const int NO_LEVEL_LOADED = -1;
static const int SECONDS_PER_ITEM = 2;
static const int MAX_NUM_SPRITES = 40;
static const long CHECKOUT_ITEM_TIME = 2000;

using get_time = chrono::steady_clock;

class GameStateManager
{
private:
	// THE CURRENT GAME STATE
	GameState currentGameState;

	// THE CURRENT LEVEL IS THE ONE CURRENTLY BEING PLAYED. NOTE THAT IF
	// THE CURRENT LEVEL IS 0, THEN IT MEANS THERE IS NO ACTIVE LEVEL LOADED,
	// LIKE WHEN WE'RE AT THE MAIN MENU
	unsigned int currentLevel;

	// THESE VECTORS STORE THE NAMES OF EACH LEVEL AND THE RELATIVE
	// PATH AND FILE NAME OF EACH DATA INPUT FILE FOR ALL GAME LEVELS. 
	// NOTE THAT WE'LL LOAD THESE VECTORS WITH THIS DATA WHEN THE GAME STARTS UP, 
	// BUT WE'LL ONLY LOAD THE LEVELS INTO THE GAME WORLD AS EACH LEVEL IS PLAYED
	// NOTE THAT THE LEVEL NAME AT INDEX i IN THE FIRST VECTOR CORRESPONDS
	// TO THE PATH AND FILE NAME AT INDEX i IN THE SECOND ONE
	vector<wstring> levelNames;
	vector<wstring> levelFileNamesWithRelativePath;

	// FOR MANAGING STATIC GAME WORLD DATA, i.e. BACKGROUND IMAGES,
	// TILES, PLATFORMS, etc. BASICALLY THINGS THAT ARE NOT ANIMATED
	// AND DO NOT MOVE
	World world;

	//FOR MANAGING GAME AUDIO
	GroceryDashAudio* gameAudio;

	// FOR MANAGING DYNAMIC GAME OBJECTS FOR CURRENT LEVEL
	// NOTE THAT WE CALL THE DYNAMIC OBJECTS "SPRITES"
	SpriteManager *spriteManager;

	// FOR DOING ALL COLLISION DETECTION AND RESOLUTION
	Physics			physics;

	// 
	bool levelClear;

	// Clock used to track timed levels
	clock_t beginLevel;
	clock_t currentTime;
	int levelTimeAllowed;

	clock_t beginPause;
	clock_t endPause;
	int timePaused;
	bool wasPaused;

	bool hasSetCheckoutTime;
	clock_t beginCheckout;
	long currentCheckout;
	long endCheckout;


public:
	// INLINED ACCESSOR METHODS
	GameState		getCurrentGameState()			{ return currentGameState;			}
	unsigned int	getCurrentLevel()				{ return currentLevel;				}
	unsigned int	getNumLevels()					{ return levelNames.size();			}
	Physics*		getPhysics()					{ return &physics;					}
	GroceryDashAudio* getAudio()						{ return gameAudio;					}
	SpriteManager*	getSpriteManager()				{ return spriteManager;				}
	World*			getWorld()						{ return &world;					}
	wstring			getCurrentLevelName()			{ return levelNames[currentLevel];	}
	void			setLevelClear(bool b)			{ levelClear = b; }
	bool			getLevelClear()					{ return levelClear; }
	int				getLevelTimeAllowed()			{ return levelTimeAllowed; }
	void			setLevelTimeAllowed(int time)	{ levelTimeAllowed = time; }
	void			clearLevelTimeAllowed()			{ levelTimeAllowed = NO_LEVEL_LOADED; }

	long			getCurrentCheckout()			{ return currentCheckout; }
	long			getEndCheckout()				{ return endCheckout; }

	// METHODS FOR TESTING THE CURRENT GAME STATE
	bool			isAppActive();
	bool			isGameInProgress();
	bool			isPlayerAtShelf();
	bool			isPlayerControlDisabled();
	bool			isGameLevelLoading();
	bool			isWorldRenderable();
	bool			isAtSplashScreen();
	bool			isAtMainMenu();
	bool			isAtLevelSelect();
	bool			isAtHelpScreen();
	bool			isAtAboutScreen();
	bool			isAtSettingsScreen();
	bool			isAtStatsScreen();
	bool			isAtAchievementsScreen();
	bool			isAtPauseMenu();
	bool			isAtInGameHelp();
	bool			isAtItemList();
	bool			isAtCartScreen();
	bool			isAtVictoryScreen();
	bool			isAtGameOverScreen();


	// METHODS FOR TRANSITIONING TO OTHER GAME STATES
	void			goToGame();
	void			goToLoadLevel();
	void			goToMainMenu();
	void			goToLevelSelect();
	void			goToHelp();
	void			goToAbout();
	void			goToStats();
	void			goToAchievements();
	void			goToSettings();
	void			goToShelfScreen(int shelfNum);
	void			goToPauseMenu();
	void			goToInGameHelp();
	void			goToItemList();
	void			goToCartScreen();
	void			goToVictoryScreen();
	void			goToGameOverScreen();

	// METHODS DEFINED in GameStateManager.cpp
	GameStateManager();
	~GameStateManager();
	void			addGameRenderItemsToRenderList(Game *game);
	void			addLevel(wstring levelToAddName, wstring levelToAddFileNameAndPath);
	unsigned int	getLevelNum(wstring levelName);
	void			shutdown();
	void			loadLevel(Game *game, int levelNum);
	void			loadLevel(Game *game, wstring levelName);
	void			unloadCurrentLevel();
	void			update(Game *game);
	int				getGameStateShelfAsNumber();

	// methods use for timing
	void			startLevelTimer();
	double			getTimeRemaining();
	void			checkLevelTimer();

	void			startPauseTimer();
	void			endPauseTimer();
	void			clearPauseData();

	void			startCheckoutTimer(Game* game);
	void			endCheckoutTimer(Game* game, clock_t currentCheckoutTime);
};