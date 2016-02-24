#pragma once

// Box2D INCLUDE
#include <Box2D\Box2D.h>

#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include "sssf\gsm\ai\bot.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gui\ScreenGUI.h"

const static float CIRCLE_RADIUS = 0.5f;
const static int TOTAL_NUMBER_OF_ITEMS = 96;
const static int SHELF_TRANSPARENCY_VALUE = 200;

#ifndef ITEM_STRUCT
#define ITEM_STRUCT
struct Item{

	string name;
	string price;
	int location;
	int position;

};
#endif // !ITEM_STRUCT

extern vector<Item> items;

extern list<int> levelItems;
extern list<int> itemsRequired;
extern list<int> itemsInCart;

class GroceryDashDataLoader : public GameDataLoader
{
private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;

	bool createdPlayer;
	bool isPlayerMale;

	// THESE METHODS ARE USED TO INTIALIZE THE DATA FOR THESE SPRITES
	// AND THEIR Box2D BODIES, FIXTURES, etc.
	
	void setupBox2DBodyPlayer(Game *game, float x, float y);
	void createBox2DBodyCashier(Game *game, Bot *bot, float x, float y);
	void createBox2DBodyCustomer(Game *game, Bot* bot, float x, float y);
	void createBox2DBodiesTiles(Game *game);

	void createEmployees(Game *game);
	void createEmployeeCashiers(Game *game, int levelNum);
	void createEmployeeFloor(Game *game);
	void createEmployeeDeli(Game *game);
	void createEmployeeBakery(Game *game);
	void createEmployeeManager(Game *game);
	void createEmployeeAndrew(Game *game);
	void createEmployeeAndres(Game *game);
	void createEmployeeFrank(Game *game);
	void createNormalCustomer(Game *game);
	void createOldCustomer(Game *game);
	void createRudeCustomer(Game *game);

	//	BELOW ARE ALL THE METHODS USED FOR SETTUPING UP THE GAME SCREENS AND
	//	THE VIEWPORT, COLORS, AND CURSOR FOR THE GAME
	void initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initMainMenu(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initLevelSelect(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initInGameGUI(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initStatsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initSettingsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initAchievementsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initAboutScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initInGameHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initLoadingScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initUnloadingScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initItemListScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initCartScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initGameOverScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);

	void initShelfScreen_1(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_2(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_3(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_4(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_5(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_6(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_7(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_8(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_9(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_10(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_P1(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);
	void initShelfScreen_P2(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);

	void initShelfScreenButtons(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties, ScreenGUI *screen);

	void initColors(GameGraphics *graphics, map<wstring, wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring, wstring> *properties);
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties);

	float generateRandomCoordinates();
	bool checkSpawnTileIsCollidable(Game* game, float x, float y);

public:

	bool isQuickPlay;

	
	int numberOfItems;
	bool hasPaidCashier;
	bool isPayingCashier;

	void togglePlayerIsMale()				{ isPlayerMale = !(isPlayerMale);	}
	bool getIsPlayerMale()					{ return isPlayerMale; }

	bool getIsPayingCashier()				{ return isPayingCashier; }
	void setIsPayingCashier(bool b)			{ isPayingCashier = b; }

	bool getHasPaidCashier()				{ return hasPaidCashier; }
	void setHasPaidCashier(bool b)			{ hasPaidCashier = b; }

	void addItemToCart(double itemCost);
	void clearCart();
	void generateItemList(Game* game, int levelNum);
	void generateRandomItemsList();
	int	 determineItemNumber(int shelfNum, int itemNum)	{ return ((shelfNum - 1) * 8) + (itemNum - 1); }
	int takeItemFromShelf(Game* game, int shelfNum, int itemNum);
	bool isItemOnShoppingList(int itemNumber);
	bool isItemInCart(int itemNumber);
	bool isItemOnLevelList(int itemToCheck);

	vector<Item>	getItems()				{ return items; }

	

	list<int>				getItemsRequired()							{ return itemsRequired; }
	list<int>::iterator		getBeginItemsRequiredIterator()				{ return itemsRequired.begin(); }
	list<int>::iterator		getEndItemsRequiredIterator()				{ return itemsRequired.end(); }
	void					addItemToShoppingList(int itemNum)			{ itemsRequired.push_back(itemNum);}
	void					removeItemFromShoppingList(int itemNum)		{ itemsRequired.remove(itemNum);}
	void					clearItemsRequired()						{ itemsRequired.clear(); itemsInCart.clear(); }
	bool					isShoppingListEmpty()						{ return itemsRequired.empty(); }

	list<int>::iterator		getBeginItemsInCartIterator()				{ return itemsInCart.begin(); }
	list<int>::iterator		getEndItemsInCartIterator()					{ return itemsInCart.end(); }
	void					addItemToCart(int itemNum)					{ itemsInCart.push_back(itemNum); }
	void					removeItemFromCart(int itemNum)				{ itemsInCart.remove(itemNum); }
	bool					isItemsInCartEmpty()						{ return itemsInCart.empty(); }

	list<int>::iterator		getBeginLevelItemsIterator()				{ return levelItems.begin(); }
	list<int>::iterator		getEndLevelItemsIterator()					{ return levelItems.end(); }

	double spendingLimit;
	double currentTotal;
	double levelItemsCost;

	void					calculateSpendingLimit(); //spendingLimit = total cost of level Items plus "extra cash"

	bool					isSpendingLimitExceeded()					{if (currentTotal > spendingLimit){ return true; } else { return false; } }

	GroceryDashDataLoader() { 
		isQuickPlay = createdPlayer = hasPaidCashier = isPayingCashier = false;
		isPlayerMale = true;
		numberOfItems = 0;
		spendingLimit = levelItemsCost = currentTotal = 0.00;
	};
	~GroceryDashDataLoader() {};

	vector<Tile*> collidableMapTiles;
	void destroyBox2DBodiesTiles(Game *game);
	

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES.
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelFilePath);
	void clearLevelData(Game* game);

	/*******************************************************************************
			OTHER METHODS DEFINED IN GroceryDashDataLoader.cpp
	 *******************************************************************************/

	//	BELOW ARE THE METHODS USED FOR ADDING SPRITES TO THE GAME
	void generateEmployees(Game *game, int numberOfEmployees);
	void generateCustomers(Game *game, int numberOfCustomers);
	void generateRandomCustomers(Game *game);

	

	vector<string> split(const char *str, char c);
	void fileReaderItemData(string name);
	void fileReaderLevelItems(string name);
	void loadItemData();

	void loadLevelItems(int levelNum);
};


