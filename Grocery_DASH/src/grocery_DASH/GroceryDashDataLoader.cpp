#include "grocery_DASH_VS\stdafx.h"


// GroceryDash GAME INCLUDES
#include "grocery_DASH\GroceryDash.h"
#include "grocery_DASH\GroceryDashButtonEventHandler.h"
#include "grocery_DASH\GroceryDashDataLoader.h"
#include "grocery_DASH\GroceryDashKeyEventHandler.h"
#include "grocery_DASH\GroceryDashTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\bots\RandomWanderingBot.h"
#include "sssf\gsm\ai\pathfinding\OrthographicGridPathfinder.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

//CONSTANT VALUES
static const int MIN_BOT_CYCLES = 10;
static const int MAX_BOT_CYCLES = 100;

vector<Item> items;

list<int> levelItems;
list<int> itemsRequired;
list<int> itemsInCart;

/*****************************************
		METHOD DEFINITIONS BELOW		
*****************************************/

void GroceryDashDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring, wstring> *properties = new map<wstring, wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *groceryDashOS = new WindowsOS(hInstance,
		nCmdShow,
		useFullscreen,
		titleProp,
		screenWidth, screenHeight,
		game);

	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *groceryDashGraphics = new DirectXGraphics(game);
	groceryDashGraphics->init(screenWidth, screenHeight);
	groceryDashGraphics->initGraphics(groceryDashOS, useFullscreen);
	groceryDashGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(groceryDashGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *groceryDashInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *groceryDashTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initplatforms((GameGraphics*)groceryDashGraphics,
		(GameInput*)groceryDashInput,
		(GameOS*)groceryDashOS,
		(GameTimer*)groceryDashTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	GroceryDashTextGenerator *groceryDashTextGenerator = new GroceryDashTextGenerator();
	groceryDashTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)groceryDashTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);

	//LOAD THE LEVEL FILES IN
	game->getGSM()->addLevel(W_LEVEL_0_NAME, W_LEVEL_0_PATH);	// TEST LEVEL
	game->getGSM()->addLevel(W_LEVEL_1_NAME, W_LEVEL_1_PATH);
	game->getGSM()->addLevel(W_LEVEL_2_NAME, W_LEVEL_2_PATH);
	game->getGSM()->addLevel(W_LEVEL_3_NAME, W_LEVEL_3_PATH);

	// INITIALIZE THE DATA FOR ALL THE ITEMS
	loadItemData();


	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

void GroceryDashDataLoader::loadGUI(Game *game, wstring guiInitFile){

	map<wstring, wstring> *guiProperties = new map<wstring, wstring>();
	loadGameProperties(game, guiProperties, guiInitFile);

	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	//CALL THE HELPER METHODS
	initSplashScreen(game, gui, guiTextureManager, guiProperties);
	initMainMenu(game, gui, guiTextureManager, guiProperties);
	initLevelSelect(game, gui, guiTextureManager, guiProperties);
	initInGameGUI(game, gui, guiTextureManager, guiProperties);
	initStatsScreen(game, gui, guiTextureManager, guiProperties);
	initSettingsScreen(game, gui, guiTextureManager, guiProperties);
	initAchievementsScreen(game, gui, guiTextureManager, guiProperties);
	initHelpScreen(game, gui, guiTextureManager, guiProperties);
	initAboutScreen(game, gui, guiTextureManager, guiProperties);
	initInGameHelpScreen(game, gui, guiTextureManager, guiProperties);
	initPauseScreen(game, gui, guiTextureManager, guiProperties);
	initItemListScreen(game, gui, guiTextureManager, guiProperties);
	initCartScreen(game, gui, guiTextureManager, guiProperties);
	initVictoryScreen(game, gui, guiTextureManager, guiProperties);
	initGameOverScreen(game, gui, guiTextureManager, guiProperties);

	initLoadingScreen(game, gui, guiTextureManager, guiProperties);
	initUnloadingScreen(game, gui, guiTextureManager, guiProperties);

	initShelfScreen_1(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_2(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_3(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_4(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_5(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_6(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_7(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_8(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_9(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_10(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_P1(game, gui, guiTextureManager, guiProperties);
	initShelfScreen_P2(game, gui, guiTextureManager, guiProperties);

	initCursor(gui, guiTextureManager, guiProperties);

	delete guiProperties;
}

void GroceryDashDataLoader::loadWorld(Game *game, wstring levelFilePath){

	// NEW LEVEL IS LOADING, THEREFORE IT IS NOT YET CLEARED IN ANY WAY
	game->getGSM()->setLevelClear(false);

	// RESET THE VIEWPORT FROM THE PREVIOUS LEVEL
	game->getGUI()->getViewport()->setViewportX(0);
	game->getGUI()->getViewport()->setViewportY(0);

	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	if (levelFilePath.compare(W_LEVEL_1_PATH) == 0){
		tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);
		game->getGSM()->getAudio()->getVoice(0)->Stop();
		game->getGSM()->getAudio()->getVoice(0)->FlushSourceBuffers();
		game->getGSM()->getAudio()->getVoice(7)->Stop();
		game->getGSM()->getAudio()->getVoice(7)->FlushSourceBuffers();
		game->getGSM()->getAudio()->getVoice(8)->Stop();
		game->getGSM()->getAudio()->getVoice(8)->FlushSourceBuffers();
		game->getGSM()->getAudio()->playAudio(6);
	}
	else{
		if (levelFilePath.compare(W_LEVEL_2_PATH) == 0){
			tmxMapImporter.loadWorld(game, W_LEVEL_2_DIR, W_LEVEL_2_NAME);
			game->getGSM()->getAudio()->getVoice(0)->Stop();
			game->getGSM()->getAudio()->getVoice(6)->Stop();
			game->getGSM()->getAudio()->getVoice(8)->Stop();
			game->getGSM()->getAudio()->getVoice(0)->FlushSourceBuffers();
			game->getGSM()->getAudio()->getVoice(6)->FlushSourceBuffers();
			game->getGSM()->getAudio()->getVoice(8)->FlushSourceBuffers();
			game->getGSM()->getAudio()->playAudio(7);
			
		}
		else{
			if (levelFilePath.compare(W_LEVEL_3_PATH) == 0){
				tmxMapImporter.loadWorld(game, W_LEVEL_3_DIR, W_LEVEL_3_NAME);
				game->getGSM()->getAudio()->getVoice(0)->Stop();
				game->getGSM()->getAudio()->getVoice(6)->Stop();
				game->getGSM()->getAudio()->getVoice(7)->Stop();
				game->getGSM()->getAudio()->getVoice(0)->FlushSourceBuffers();
				game->getGSM()->getAudio()->getVoice(6)->FlushSourceBuffers();
				game->getGSM()->getAudio()->getVoice(7)->FlushSourceBuffers();
				game->getGSM()->getAudio()->playAudio(8);
			}
			else{
				// LOAD THE TEST LEVEL
				tmxMapImporter.loadWorld(game, W_LEVEL_0_DIR, W_LEVEL_0_NAME);
			}
		}
	}
	

	// SPECIFY WHO WILL DO THE PATHFINDING
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();

	SpriteManager *spriteManager = gsm->getSpriteManager();
	if (spriteManager->getPathfinder() == nullptr){
		OrthographicGridPathfinder *pathfinder = new OrthographicGridPathfinder(game);
		spriteManager->setPathfinder(pathfinder);
	}

	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, W_SPRITE_TYPES_LIST);
	Physics *physics = game->getGSM()->getPhysics();

	// CREATE ALL THE BODIES FOR THE WORLD TILES
	createBox2DBodiesTiles(game);

	// LET'S MAKE A PLAYER SPRITE, if necessary --> NOTE THAT THE PLAYER IS TYPE 0

	TopDownSprite *player = spriteManager->getPlayer();
	AnimatedSpriteType *playerSpriteTypeM = spriteManager->getSpriteType(0);
	AnimatedSpriteType *playerSpriteTypeF = spriteManager->getSpriteType(16);
	player->setRotationInRadians(21.21f);
	if (isPlayerMale == true){ player->setSpriteType(playerSpriteTypeM); }
	else{ player->setSpriteType(playerSpriteTypeF); }
	player->setAlpha(255);
	player->setZ(0); //Player is always visible over everything.  Anything else should have a z >= 1
	player->setCurrentState(W_WALKING);

	float playerX, playerY;
	switch (game->getGSM()->getCurrentLevel()){
		case 1:{ playerX = START_POS_X_L1; playerY = START_POS_Y_L1; game->getGSM()->setLevelTimeAllowed(TIME_LIMIT_L1); }
			break;
		case 2:{ playerX = START_POS_X_L2; playerY = START_POS_Y_L2; game->getGSM()->setLevelTimeAllowed(TIME_LIMIT_L2); }
			break;
		case 3:{ playerX = START_POS_X_L3; playerY = START_POS_Y_L3; game->getGSM()->setLevelTimeAllowed(TIME_LIMIT_L3); }
			break;
		default:{ playerX = START_POS_X_L1; playerY = START_POS_Y_L1; game->getGSM()->setLevelTimeAllowed(TIME_LIMIT_L1); }
			break;
	}
	
	setupBox2DBodyPlayer(game, playerX, playerY); //Create the Box2D body for the player
	createdPlayer = true;

	// LOAD IN THE STARTING SPRITES - CUSTOMERS AND EMPLOYEES
	createEmployees(game);
	generateCustomers(game, 10);
	generateEmployees(game, 10);
	
	// GENERATE THE SHOPPING LIST
	generateItemList(game, game->getGSM()->getCurrentLevel());
	calculateSpendingLimit();
	

	//START THE LEVEL TIMER
	game->getGSM()->startLevelTimer();
	hasPaidCashier = isPayingCashier = false;

	//RESET THIS VARIABLE SINCE THE LEVEL HAS NOW STARTED
	isQuickPlay = false;
}

void GroceryDashDataLoader::clearLevelData(Game* game){
	if (game->getGSM()->getPhysics()->box2DWorld.IsLocked() == false){
		game->getGSM()->goToMainMenu();
		this->destroyBox2DBodiesTiles(game);
		game->getGraphics()->clearWorldTextures();
		game->getGSM()->clearLevelTimeAllowed();
		game->getGSM()->clearPauseData();
		game->getGSM()->getSpriteManager()->getPlayer()->resetNumCollisions();
		game->getGSM()->getSpriteManager()->getPlayer()->resetNumFollowers();
		this->clearCart();
	}
}


void GroceryDashDataLoader::setupBox2DBodyPlayer(Game *game, float x, float y){
	TopDownSprite *player = game->getGSM()->getSpriteManager()->getPlayer();
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	

	//Setup the player
	b2BodyDef bodyDefPlayer = b2BodyDef();
	bodyDefPlayer.type = b2_dynamicBody;
	bodyDefPlayer.position.Set(x + 0.5f, y + 0.5f);
	bodyDefPlayer.userData = player;
	b2Body *playerBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefPlayer);
	player->setB2Body(playerBody);
	spriteManager->playerBody = playerBody;
	playerBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			playerBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefPlayer.userData;
			fixtureDef.filter.categoryBits = 0x0010;
			//b2Vec2 bv = (*(b2CircleShape*)(playerBody->GetFixtureList()->GetShape())).m_p;
}

void GroceryDashDataLoader::createBox2DBodyCashier(Game *game, Bot* bot, float x, float y){

	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();

	//Setup the cashier
	b2BodyDef bodyDefCashier = b2BodyDef();
	bodyDefCashier.type = b2_staticBody;
	bodyDefCashier.position.Set(x, y);
	bodyDefCashier.userData = bot;
	bodyDefCashier.position.Set(x + .5, y + .5);
	b2Body *cashierBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefCashier);
	bot->setB2Body(cashierBody);
	cashierBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			cashierBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefCashier.userData;
}

void GroceryDashDataLoader::createBox2DBodyCustomer(Game *game, Bot* bot, float x, float y){

	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	
	//Setup the customer
	b2BodyDef bodyDefCustomer = b2BodyDef();
	bodyDefCustomer.type = b2_dynamicBody;
	bodyDefCustomer.position.Set(x, y);
	bodyDefCustomer.userData = bot;
	//bodyDefCustomer.position.Set(x + .5, y + .5);
	b2Body *customerBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefCustomer);
	bot->setB2Body(customerBody);
	customerBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			customerBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefCustomer.userData;

}

void GroceryDashDataLoader::createBox2DBodiesTiles(Game *game){
	vector <WorldLayer*> *layers = game->getGSM()->getWorld()->getLayers();
	WorldLayer *layer = layers->front();
	TiledLayer *tileLayer = dynamic_cast<TiledLayer*>(layer);

	int groceryShelf = 1;
	int produceBin = 11;
	float x, y;
	for (int j = 0; j <= 49; j++)
	{
		y = (float)j;
		for (int i = 0; i <= 49; i++)
		{
			x = (float)i;
			Tile *tileToAdd = tileLayer->getTile(j, i);
			//int k = j * 50 + i;
			//int l = tileToAdd->textureID;
			//bool b = tileToAdd->collidable;
			if (tileToAdd->textureID != 0)
			{
				//CODE FOR CREATING THE TILE BODY
				b2BodyDef tileBodyDef;
				tileBodyDef.type = b2_staticBody;
				tileBodyDef.angle = 0;
				tileBodyDef.fixedRotation = true;
					b2PolygonShape boxShape;
					boxShape.SetAsBox(0.5f, 0.5f);
						b2FixtureDef boxFixtureDef;
						boxFixtureDef.shape = &boxShape;
						boxFixtureDef.density = 1;
						boxFixtureDef.isSensor = false;
				tileBodyDef.position.Set(x + .5, y + .5);
				b2Body* tileBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&tileBodyDef);
				tileBody->CreateFixture(&boxFixtureDef);

				// NOW LINK THE BODY AND THE TILE
				tileToAdd->body = tileBody;
				tileBody->SetUserData(tileToAdd);

				//DETERMINE THE SHELF NUMBER
				if (tileToAdd->textureID >= 14 && tileToAdd->textureID <= 19){
					tileToAdd->shelfNum = groceryShelf;
					groceryShelf++;
				}
				else{
					if ((tileToAdd->textureID >= 80 && tileToAdd->textureID <= 89)){
						tileToAdd->shelfNum = produceBin;
						produceBin++;
					}
					else{
						tileToAdd->shelfNum = -1;
					}
				}
				

				// NOW ADD THE MAP TILE
				collidableMapTiles.push_back(tileToAdd);
			}
		}// FOR - i
	}// FOR - j

}

void GroceryDashDataLoader::destroyBox2DBodiesTiles(Game *game){
	if (game->getGSM()->getPhysics()->box2DWorld.IsLocked() == false){
		if (collidableMapTiles.empty() == false){
			Tile *tileToRemove;
			for (int i = 0; i < collidableMapTiles.size(); i++){
				tileToRemove = collidableMapTiles.at(i);


				tileToRemove->body->SetUserData(nullptr);
				game->getGSM()->getPhysics()->box2DWorld.DestroyBody(tileToRemove->body);
				tileToRemove->body = nullptr;

			}

			collidableMapTiles.clear();
		}
	}
}


void GroceryDashDataLoader::createEmployees(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();

	//Create the manager
	createEmployeeManager(game);

	//Create the cashiers
	createEmployeeCashiers(game, game->getGSM()->getCurrentLevel());

	//Create the floor employees
	createEmployeeFloor(game);

	//Create the special employee sprites
	if (game->getGSM()->getCurrentLevel() == 0 || game->getGSM()->getCurrentLevel() == 1){ //LEVEL 0 or LEVEL 1
		createEmployeeAndres(game);
		createEmployeeFrank(game);
		createEmployeeAndrew(game);
	}
}

void GroceryDashDataLoader::createEmployeeManager(Game *game){

	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *managerSpriteType = spriteManager->getSpriteType(10);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(managerSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup the manager
	b2BodyDef bodyDefManager = b2BodyDef();
	bodyDefManager.type = b2_dynamicBody;
	bodyDefManager.position.Set(x, y);
	bodyDefManager.userData = bot;
	b2Body *managerBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefManager);
	bot->setB2Body(managerBody);
	managerBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			managerBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefManager.userData;
}

void GroceryDashDataLoader::createEmployeeCashiers(Game *game, int levelNum){
	//Create the Cashiers
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *cashierSpriteType = spriteManager->getSpriteType(13);
	float x, y;

	switch (levelNum){
		case 0:
		case 1:{	// LEVEL 1 ( or test level [level 0] )
				x = (1344.0f + 5.0f) / 64; y = 2816.0f / 64;
				RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1536.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1728.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1920.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

		}//case 1
			break;
		case 2:{	// LEVEL 2
				x = (448.0f + 5.0f) / 64; y = 2816.0f / 64;
				RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (704.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (960.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1216.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1472.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

		}//case 2
			break;
		case 3:{	//	LEVEL 3
				x = (768.0f + 5.0f) / 64; y = 2816.0f / 64;
				RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1024.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1152.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1408.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1664.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (1792.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (2048.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

				x = (2176.0f + 5.0f) / 64; y = 2816.0f / 64;
				bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
				bot->setSpriteType(cashierSpriteType);
				bot->setCurrentState(W_STANDING);
				bot->setAlpha(255);
				bot->setZ(1);
				createBox2DBodyCashier(game, bot, x, y); //Setup the Box2D body
				spriteManager->addBot(bot); //Finally, ADD the bot

		}//case 3
			break;
		default:{	/*	CREATE NO CASHIERS	*/		}
			break;
	}//SWITCH BRACKET
}

void GroceryDashDataLoader::createEmployeeFloor(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *floorSpriteType = spriteManager->getSpriteType(15);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(floorSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup Box2D Body
	b2BodyDef bodyDefFloor = b2BodyDef();
	bodyDefFloor.type = b2_dynamicBody;
	bodyDefFloor.position.Set(x, y);
	bodyDefFloor.userData = bot;
	b2Body *FloorBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefFloor);
	bot->setB2Body(FloorBody);
	FloorBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			FloorBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefFloor.userData;

}

void GroceryDashDataLoader::createEmployeeDeli(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *DeliSpriteType = spriteManager->getSpriteType(14);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(DeliSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup Box2D Body
	b2BodyDef bodyDefDeli = b2BodyDef();
	bodyDefDeli.type = b2_dynamicBody;
	bodyDefDeli.position.Set(x, y);
	bodyDefDeli.userData = bot;
	b2Body *DeliBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefDeli);
	bot->setB2Body(DeliBody);
	DeliBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			DeliBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefDeli.userData;

}

void GroceryDashDataLoader::createEmployeeBakery(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *BakerySpriteType = spriteManager->getSpriteType(11);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(BakerySpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup Box2D Body
	b2BodyDef bodyDefBakery = b2BodyDef();
	bodyDefBakery.type = b2_dynamicBody;
	bodyDefBakery.position.Set(x, y);
	bodyDefBakery.userData = bot;
	b2Body *BakeryBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefBakery);
	bot->setB2Body(BakeryBody);
	BakeryBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			BakeryBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefBakery.userData;

}

void GroceryDashDataLoader::createEmployeeAndrew(Game *game){

	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *AndrewSpriteType = spriteManager->getSpriteType(7);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(AndrewSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup ME (Andrew)
	b2BodyDef bodyDefAndrew = b2BodyDef();
	bodyDefAndrew.type = b2_dynamicBody;
	bodyDefAndrew.position.Set(x, y);
	bodyDefAndrew.userData = bot;
	b2Body *AndrewBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefAndrew);
	bot->setB2Body(AndrewBody);
	AndrewBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			AndrewBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefAndrew.userData;

}

void GroceryDashDataLoader::createEmployeeAndres(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *AndresSpriteType = spriteManager->getSpriteType(8);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(AndresSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup Andres
	b2BodyDef bodyDefAndres = b2BodyDef();
	bodyDefAndres.type = b2_dynamicBody;
	bodyDefAndres.position.Set(x, y);
	bodyDefAndres.userData = bot;
	b2Body *AndresBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefAndres);
	bot->setB2Body(AndresBody);
	AndresBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			AndresBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefAndres.userData;
}

void GroceryDashDataLoader::createEmployeeFrank(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *FrankSpriteType = spriteManager->getSpriteType(9);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	bot->setSpriteType(FrankSpriteType);
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	//Finally, ADD the bot
	spriteManager->addBot(bot);

	//Setup Frank
	b2BodyDef bodyDefFrank = b2BodyDef();
	bodyDefFrank.type = b2_dynamicBody;
	bodyDefFrank.position.Set(x, y);
	bodyDefFrank.userData = bot;
	b2Body *FrankBody = game->getGSM()->getPhysics()->box2DWorld.CreateBody(&bodyDefFrank);
	bot->setB2Body(FrankBody);
	FrankBody->SetLinearDamping(10.0f);
		b2CircleShape dynamicCircle;
		dynamicCircle.m_p.Set(0, 0);
		dynamicCircle.m_radius = CIRCLE_RADIUS;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicCircle;
			//fixtureDef.density = 0.1f;
			fixtureDef.friction = 0.0f;
			FrankBody->CreateFixture(&fixtureDef);
			fixtureDef.userData = bodyDefFrank.userData;
}

void GroceryDashDataLoader::createNormalCustomer(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *normalCustomerSpriteType_f = spriteManager->getSpriteType(1);
	AnimatedSpriteType *normalCustomerSpriteType_m = spriteManager->getSpriteType(2);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	int decideSpriteType = rand() % 10;
	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	if (decideSpriteType % 2 == 0){
		bot->setSpriteType(normalCustomerSpriteType_m);
	}
	else{
		bot->setSpriteType(normalCustomerSpriteType_f);
	}
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	createBox2DBodyCustomer(game, bot, x, y); //Setup the Box2D body
	spriteManager->addBot(bot); //Finally, ADD the bot

}

void GroceryDashDataLoader::createOldCustomer(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *oldCustomerSpriteType_f = spriteManager->getSpriteType(3);
	AnimatedSpriteType *oldCustomerSpriteType_m = spriteManager->getSpriteType(4);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	int decideSpriteType = rand() % 10;
	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	if (decideSpriteType % 2 == 0){
		bot->setSpriteType(oldCustomerSpriteType_m);
	}
	else{
		bot->setSpriteType(oldCustomerSpriteType_f);
	}
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	createBox2DBodyCustomer(game, bot, x, y); //Setup the Box2D body
	spriteManager->addBot(bot); //Finally, ADD the bot
}

void GroceryDashDataLoader::createRudeCustomer(Game *game){
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	AnimatedSpriteType *rudeCustomerSpriteType_f = spriteManager->getSpriteType(5);
	AnimatedSpriteType *rudeCustomerSpriteType_m = spriteManager->getSpriteType(6);

	float x = generateRandomCoordinates();
	float y = generateRandomCoordinates();
	bool checkSpawn = true;
	while (checkSpawn == true){
		checkSpawn = checkSpawnTileIsCollidable(game, x, y);
		if (checkSpawn == true){
			x = generateRandomCoordinates();
			y = generateRandomCoordinates();
		}
	}

	int decideSpriteType = rand() % 10;
	RandomWanderingBot *bot = new RandomWanderingBot(MIN_BOT_CYCLES, MAX_BOT_CYCLES);
	if (decideSpriteType % 2 == 0){
		bot->setSpriteType(rudeCustomerSpriteType_m);
	}
	else{
		bot->setSpriteType(rudeCustomerSpriteType_f);
	}
	bot->setCurrentState(W_WALKING);
	bot->setAlpha(255);
	bot->setZ(1);
	createBox2DBodyCustomer(game, bot, x, y); //Setup the Box2D body
	spriteManager->addBot(bot); //Finally, ADD the bot
}


void GroceryDashDataLoader::generateEmployees(Game *game, int numberOfEmployees){

	int random = 0;
	for (int i = 0; i < numberOfEmployees; i++){
		
		random = rand() % 10 + 1;

		if (random == 2){
			createEmployeeBakery(game);
		}
		else{
			if (random == 4 || random == 9){
				createEmployeeDeli(game);
			}
			else{
				createEmployeeFloor(game);
			}
		}

	}//for
}

void GroceryDashDataLoader::generateCustomers(Game *game, int numberOfCustomers){

	int deciderCustomerType;
	for (int i = 0; i < numberOfCustomers; i++){

		deciderCustomerType = rand() % 4;

		if (deciderCustomerType == 0){
			createOldCustomer(game);
		}
		else{
			if (deciderCustomerType == 1){
				createRudeCustomer(game);
			}
			else{
				createNormalCustomer(game);
			}
		}
	}
}

void GroceryDashDataLoader::generateRandomCustomers(Game *game){
	int numberOfCustomers = rand() % 10 + 1;

	int deciderCustomerType;
	for (int i = 0; i < numberOfCustomers; i++){

		deciderCustomerType = rand() % 4;

		if (deciderCustomerType == 0){
			createOldCustomer(game);
		}
		else{
			if (deciderCustomerType == 1){
				createRudeCustomer(game);
			}
			else{
				createNormalCustomer(game);
			}
		}
	}
}


/*
		BELOW ARE ALL THE METHODS USED FOR SETTUPING UP THE GAME SCREENS, (excluding shelf screens)
			
*/

void GroceryDashDataLoader::initSplashScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	// START WITH THE SPLASH SCREEN
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE
	unsigned int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_SPLASH_SCREEN]);
	unsigned int mouseOverTextureID = normalTextureID;

	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

void GroceryDashDataLoader::initMainMenu(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_MAIN_MENU]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE START BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_START_GAME]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_START_GAME_MO]);
	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							345,
							130,
							0,
							255,
							395,
							80,
							false,
							W_START_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE STATS BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_STATS]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_STATS_MO]);
	// - INIT THE STATS BUTTON
	buttonToAdd->initButton(normalTextureID,
						mouseOverTextureID,
						345,
						240,
						0,
						255,
						395,
						80,
						false,
						W_GO_TO_STATS_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE ACHIEVEMENTS BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ACHIEVEMENTS]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ACHIEVEMENTS_MO]);
	// - INIT THE ACHIEVEMENTS BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							345,
							355,
							0,
							255,
							395,
							80,
							false,
							W_GO_TO_ACHIEVEMENTS_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE SETTINGS BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SETTINGS]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SETTINGS_MO]);
	// - INIT THE SETTINGS BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							345,
							470,
							0,
							255,
							395,
							80,
							false,
							W_GO_TO_SETTINGS_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE QUIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_QUIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_QUIT_MO]);
	// - INIT THE QUIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							345,
							590,
							0,
							255,
							395,
							80,
							false,
							W_QUIT_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE HELP BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_HELP]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_HELP_MO]);
	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							925,
							200,
							0,
							255,
							82,
							44,
							false,
							W_HELP_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE ABOUT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ABOUT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ABOUT_MO]);
	// - INIT THE ABOUT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							925,
							300,
							0,
							255,
							82,
							44,
							false,
							W_ABOUT_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU, mainMenuGUI);
}

void GroceryDashDataLoader::initLevelSelect(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD A LEVEL SELECT GUI SCREEN
	ScreenGUI *levelSelectGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_LEVEL_SELECT]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	levelSelectGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE AISLE_1 BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_1]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_1_MO]);
	// - INIT THE AISLE_1 BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							85,
							95,
							0,
							255,
							82,
							81,
							false,
							W_LOAD_L1_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE AISLE_2 BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_2]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_2_MO]);
	// - INIT THE AISLE_2 BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							355,
							95,
							0,
							255,
							82,
							81,
							false,
							W_LOAD_L2_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE AISLE_3 BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_3]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_3_MO]);
	// - INIT THE AISLE_3 BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							608,
							95,
							0,
							255,
							82,
							81,
							false,
							W_LOAD_L3_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE AISLE_4 BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_4]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_AISLE_4_MO]);
	// - INIT THE AISLE_4 BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							850,
							95,
							0,
							255,
							82,
							81,
							false,
							W_LOAD_QP_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	// AND LET'S ADD THE HELP BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_HELP]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_HELP_MO]);
	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							480,
							675,
							0,
							255,
							82,
							44,
							false,
							W_HELP_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	/*/ AND LET'S ADD THE ABOUT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ABOUT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_ABOUT_MO]);
	// - INIT THE ABOUT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							700,
							600,
							0,
							255,
							82,
							44,
							false,
							W_ABOUT_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);
	*/

	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
						mouseOverTextureID,
						5,
						5,
						0,
						250,
						50,
						50,
						false,
						W_EXIT_TO_MM_COMMAND);
	levelSelectGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_LEVEL_SELECT, levelSelectGUI);

}

void GroceryDashDataLoader::initInGameGUI(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE GAME SCREEN GUI
	ScreenGUI *gameScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_GAME_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	gameScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE LIST BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_LIST]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_LIST_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							560,
							0,
							50,
							50,
							50,
							false,
							W_GO_TO_ITEM_LIST_COMMAND);
	gameScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE CART BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_CART]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_CART_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							490,
							0,
							50,
							50,
							50,
							false,
							W_GO_TO_CART_COMMAND);
	gameScreenGUI->addButton(buttonToAdd);


	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							50,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	gameScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS, gameScreenGUI);
}

void GroceryDashDataLoader::initStatsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE STATS SCREEN
	ScreenGUI *statsScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_STATS_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	statsScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	statsScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_STATS_SCREEN, statsScreenGUI);
}

void GroceryDashDataLoader::initSettingsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE SETTINGS SCREEN GUI
	ScreenGUI *settingsScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SETTINGS_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	settingsScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE TOGGLE GENDER BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_TOGGLE_GENDER]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_TOGGLE_GENDER_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							271,
							432,
							0,
							250,
							400,
							65,
							false,
							W_TOGGLE_GENDER_COMMAND);
	settingsScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE TOGGLE WASD BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_TOGGLE_WASD]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_TOGGLE_WASD_MO]);
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							271,
							535,
							0,
							250,
							400,
							65,
							false,
							W_TOGGLE_WASD_COMMAND);
	settingsScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	settingsScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_SETTINGS_SCREEN, settingsScreenGUI);
}

void GroceryDashDataLoader::initAchievementsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE ACHIEVEMETS SCREEN GUI
	ScreenGUI *achievementsScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_ACHIEVEMENTS_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	achievementsScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	achievementsScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_ACHIEVEMENTS_SCREEN, achievementsScreenGUI);
}

void GroceryDashDataLoader::initHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE ACHIEVEMETS SCREEN GUI
	ScreenGUI *helpScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_HELP_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	helpScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	helpScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MENU_HELP_MENU, helpScreenGUI);
}

void GroceryDashDataLoader::initAboutScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE ABOUT SCREEN GUI
	ScreenGUI *aboutScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_ABOUT_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	aboutScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_EXIT_TO_MM_COMMAND);
	aboutScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS 
	gui->addScreenGUI(GS_MENU_ABOUT_MENU, aboutScreenGUI);
}

void GroceryDashDataLoader::initInGameHelpScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	// NOW LET'S LOAD THE ACHIEVEMETS SCREEN GUI
	ScreenGUI *inGameHelpScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_IN_GAME_HELP_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	inGameHelpScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	inGameHelpScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_IN_GAME_HELP, inGameHelpScreenGUI);

}

void GroceryDashDataLoader::initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE PAUSE SCREEN GUI
	ScreenGUI *pauseScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_PAUSE_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	pauseScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE RESUME BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_RESUME]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_RESUME_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							279,
							185,
							0,
							250,
							450,
							100,
							false,
							W_RETURN_TO_GAME_COMMAND);
	pauseScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE RESTART BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_RESTART]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_RESTART_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							279,
							310,
							0,
							250,
							450,
							100,
							false,
							W_RESTART_LEVEL_COMMAND);
	pauseScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE IN GAME HELP BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_HELP]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_HELP_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							279,
							445,
							0,
							250,
							450,
							100,
							false,
							W_GO_TO_IG_HELP_COMMAND);
	pauseScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_P_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							279,
							570,
							0,
							250,
							450,
							100,
							false,
							W_EXIT_TO_MM_COMMAND);
	pauseScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_PAUSED, pauseScreenGUI);

}

void GroceryDashDataLoader::initLoadingScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE LOADING SCREEN GUI
	ScreenGUI *loadingScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_LOADING_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	loadingScreenGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_STARTING_LEVEL, loadingScreenGUI);
}

void GroceryDashDataLoader::initUnloadingScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	// NOW LET'S LOAD THE LOADING SCREEN GUI
	ScreenGUI *unloadingScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_UNLOADING_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	unloadingScreenGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_UNLOADING_LEVEL, unloadingScreenGUI);
}

void GroceryDashDataLoader::initItemListScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	ScreenGUI *itemListScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_ITEM_LIST_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	itemListScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE CART BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_CART]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_CART_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							255,
							15,
							0,
							250,
							50,
							50,
							false,
							W_GO_TO_CART_COMMAND);
	itemListScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	itemListScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_ITEM_LIST_SCREEN, itemListScreenGUI);

}

void GroceryDashDataLoader::initCartScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	ScreenGUI *cartScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_CART_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	cartScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE LIST BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_LIST]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_LIST_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							255,
							15,
							0,
							250,
							50,
							50,
							false,
							W_GO_TO_ITEM_LIST_COMMAND);
	cartScreenGUI->addButton(buttonToAdd);

	// LET'S ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);	
	cartScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_CART_SCREEN, cartScreenGUI);

}

void GroceryDashDataLoader::initVictoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	ScreenGUI *victoryScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_VICTORY_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	victoryScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		5,
		5,
		0,
		250,
		50,
		50,
		false,
		W_EXIT_TO_LS_COMMAND);
	victoryScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_VICTORY, victoryScreenGUI);

}

void GroceryDashDataLoader::initGameOverScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){

	ScreenGUI *gameOverScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_GAME_OVER_SCREEN]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	gameOverScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD THE EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		5,
		5,
		0,
		250,
		50,
		50,
		false,
		W_EXIT_TO_LS_COMMAND);
	gameOverScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_OVER, gameOverScreenGUI);

}



/**
	METHODS FOR INITIALIZING THE SHELF SCREENS
**/
void GroceryDashDataLoader::initShelfScreen_1(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen1ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_1]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen1ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
					mouseOverTextureID,
					5,
					5,
					0,
					250,
					50,
					50,
					false,
					W_RETURN_TO_GAME_COMMAND);
	shelfScreen1ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen1ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_1, shelfScreen1ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_2(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen2ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_2]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen2ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen2ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen2ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_2, shelfScreen2ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_3(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen3ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_3]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen3ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen3ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen3ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_3, shelfScreen3ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_4(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen4ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_4]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen4ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen4ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen4ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_4, shelfScreen4ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_5(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen5ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_5]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen5ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen5ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen5ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_5, shelfScreen5ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_6(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen6ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_6]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen6ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen6ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen6ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_6, shelfScreen6ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_7(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen7ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_7]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen7ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen7ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen7ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_7, shelfScreen7ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_8(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen8ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_8]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen8ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen8ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen8ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_8, shelfScreen8ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_9(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen9ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_9]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen9ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen9ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen9ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_9, shelfScreen9ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_10(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreen10ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_10]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreen10ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreen10ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreen10ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_10, shelfScreen10ScreenGUI);
}
 
void GroceryDashDataLoader::initShelfScreen_P1(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreenP1ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_P1]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreenP1ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreenP1ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreenP1ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_P1, shelfScreenP1ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreen_P2(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties){
	ScreenGUI *shelfScreenP2ScreenGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture((*guiProperties)[W_SHELF_SCREEN_P2]);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = SHELF_TRANSPARENCY_VALUE;
	imageToAdd->width = 1024;
	imageToAdd->height = 768;
	imageToAdd->imageID = imageID;
	shelfScreenP2ScreenGUI->addOverlayImage(imageToAdd);

	// LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_EXIT_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							5,
							5,
							0,
							250,
							50,
							50,
							false,
							W_RETURN_TO_GAME_COMMAND);
	shelfScreenP2ScreenGUI->addButton(buttonToAdd);

	initShelfScreenButtons(game, gui, guiTextureManager, guiProperties, shelfScreenP2ScreenGUI);

	gui->addScreenGUI(GS_GIP_SHELF_SCREEN_P2, shelfScreenP2ScreenGUI);
}

void GroceryDashDataLoader::initShelfScreenButtons(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *guiProperties, ScreenGUI *screen){
	
	Button *buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_1]);
	int mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_1_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							100,
							325,
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_1_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_2]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_2_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							350,	// X coordinate
							325,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_2_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_3]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_3_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							600,	// X coordinate
							325,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_3_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_4]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_4_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							850,	// X coordinate
							325,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_4_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);


	/*	BUTTONS ON LOWER SHELF	*/

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_5]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_5_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							100,	// X coordinate
							690,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_5_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_6]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_6_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							350,	// X coordinate
							690,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_6_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_7]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_7_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							600,	// X coordinate
							690,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_7_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);

	buttonToAdd = new Button();
	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_8]);
	mouseOverTextureID = guiTextureManager->loadTexture((*guiProperties)[W_BUTTON_SHELF_ITEM_8_MO]);
	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
							mouseOverTextureID,
							850,	// X coordinate
							690,	// Y coordinate
							0,
							250,
							70,
							70,
							false,
							W_ADD_ITEM_8_TO_CART_COMMAND);
	screen->addButton(buttonToAdd);
}

/*
	METHODS TO INITIALIZE THE VIEWPORT, COLORS, AND CURSOR FOR THE GAME
*/
void GroceryDashDataLoader::initColors(GameGraphics *graphics, map<wstring, wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

void GroceryDashDataLoader::initViewport(GameGUI *gui, map<wstring, wstring> *properties){
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
	viewport->setToggleOffsetY(toggleOffsetY);
}

void GroceryDashDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager, map<wstring, wstring> *properties){

	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture( (*properties)[W_CURSOR_NORMAL] );
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}


/*
	Other Methods
*/
float GroceryDashDataLoader::generateRandomCoordinates(){

	int random = rand() % 45 + 3;
	int worldCoordinate = (random*64);
	float returnValue  = (float) (worldCoordinate / 64);
	return returnValue;
}

bool GroceryDashDataLoader::checkSpawnTileIsCollidable(Game* game, float x, float y){
	vector <WorldLayer*> *layers = game->getGSM()->getWorld()->getLayers();
	WorldLayer *layer = layers->front();
	TiledLayer *tileLayer = dynamic_cast<TiledLayer*>(layer);

	int newX = (int)x;
	int newY = (int)y;

	Tile *tileToCheck = tileLayer->getTile(newY, newX);

	if (tileToCheck->collidable == true){
		return true;
	}
	else{
		return false;
	}
	
}


/******************************
	* GAMEPLAY FUNCTION *
*******************************/

void GroceryDashDataLoader::addItemToCart(double itemCost){

	if (itemCost < 0.00 || itemCost > 0.00){

		currentTotal += itemCost;

		if (itemCost < 0.00){
			numberOfItems--;
		}

		if (itemCost > 0.00){
			numberOfItems++;
		}
		

	}

}

void GroceryDashDataLoader::clearCart(){
	currentTotal = 0.00;
	spendingLimit = 0.00;
	levelItemsCost = 0.00;
	numberOfItems = 0;
	hasPaidCashier = isPayingCashier = false;
	itemsRequired.clear();
	itemsInCart.clear();
	levelItems.clear();

}

void GroceryDashDataLoader::generateItemList(Game* game, int levelNum){

	clearItemsRequired();

	if (isQuickPlay == true && levelNum != 0){
		levelNum += 4;
	}

	switch (levelNum){

		case 0:{	loadLevelItems(levelNum);	}// CASE 0
			break;
		case 1:{	loadLevelItems(levelNum);	}// CASE 1
			break;
		case 2:{	loadLevelItems(levelNum);	}// CASE 2
			break;
		case 3:{	loadLevelItems(levelNum);	}// CASE 3
			break;
		default:{	generateRandomItemsList();	 }//DEFAULT
			break;


	}//SWITCH

}

void GroceryDashDataLoader::generateRandomItemsList(){

	int numberOfItems = rand() % 26 + 1;
	if (numberOfItems <= 5){ numberOfItems += rand() % 5 + 5; }
	int itemNumber = -1;

	for (int i = 0; i < numberOfItems; i++){

		itemNumber = rand() % 96;

		if (isItemOnShoppingList(itemNumber) == true){
			i--;
		}
		else{
			addItemToShoppingList(itemNumber);
		}
	}

}

int GroceryDashDataLoader::takeItemFromShelf(Game* game, int shelfNum, int itemNum){

	int itemNumber = determineItemNumber(shelfNum, itemNum);

	if (isItemInCart(itemNumber) == true && isItemOnLevelList(itemNumber) == true){

		// CHECK TO MAKE SURE THE ITEM IS GOING BACK IN THE CORRECT SPOT
		if (items.at(itemNumber).location != shelfNum){ return 2; }
		if (items.at(itemNumber).position != itemNum) { return 2; }

		// PLACING THE *REQUIRED* ITEM BACK ON THE SHELF
		// AND THEREFORE BACK ON THE SHOPPING LIST
		addItemToShoppingList(itemNumber);	// put it on the shopping list
		removeItemFromCart(itemNumber);		// remove it from the cart

		// NO ITEM WAS TAKEN
		return 0;
	}
	else{
		if (isItemInCart(itemNumber) == true && isItemOnLevelList(itemNumber) == false){

			// CHECK TO MAKE SURE THE ITEM IS GOING BACK IN THE CORRECT SPOT
			if (items.at(itemNumber).location != shelfNum){ return 2; }
			if (items.at(itemNumber).position != itemNum) { return 2; }

			// PLACING A non-required ITEM BACK ON THE SHELF, ONLY
			// IT DOES *NOT* GO ON THE SHOPPING LIST
			removeItemFromCart(itemNumber);

			// NO ITEM WAS TAKEN
			return 0;
		}
		else{
			// ITEM IS ON THE SHOPPING LIST
			// AND WE ARE ADDING IT TO THE CART
			removeItemFromShoppingList(itemNumber); // remove it from the shopping list
			addItemToCart(itemNumber);				// put it into the cart

			// AN ITEM WAS TAKEN
			return 1;
		}
	}

}

bool GroceryDashDataLoader::isItemOnShoppingList(int itemToCheck){

	list<int>::iterator itemIterator = getBeginItemsRequiredIterator();
	int itemNumber = -1;

	while (itemIterator != getEndItemsRequiredIterator()){
		
		itemNumber = *(itemIterator);
		
		if (itemToCheck == itemNumber){
			return true;
		}

		itemIterator++;
	}

	return false;

}

bool GroceryDashDataLoader::isItemInCart(int itemToCheck){

	list<int>::iterator itemIterator = getBeginItemsInCartIterator();
	int itemNumber = -1;

	while (itemIterator != getEndItemsInCartIterator()){

		itemNumber = *(itemIterator);

		if (itemToCheck == itemNumber){
			return true;
		}

		itemIterator++;
	}

	return false;

}

bool GroceryDashDataLoader::isItemOnLevelList(int itemToCheck){

	list<int>::iterator itemIterator = getBeginLevelItemsIterator();
	int itemNumber = -1;

	while (itemIterator != getEndLevelItemsIterator()){

		itemNumber = *(itemIterator);

		if (itemToCheck == itemNumber){
			return true;
		}

		itemIterator++;
	}

	return false;

}

void GroceryDashDataLoader::calculateSpendingLimit(){


	if (isQuickPlay == false){
		// USE THE LEVEL ITEMS LIST
		list<int>::iterator itemIterator = getBeginLevelItemsIterator();
		int itemNumber = -1;

		while (itemIterator != getEndLevelItemsIterator()){

			itemNumber = *(itemIterator);

			spendingLimit += stod(items.at(itemNumber).price);

			itemIterator++;
		}
	}
	else{
		// USE THE REQUIRED ITEMS TO INITIALIZE THE LEVEL ITEMS
		// THERE ARE NO ASSIGNED ITEMS IN THIS LEVEL
		//list<int>::iterator itemIteratorLI = getBeginLevelItemsIterator();
		list<int>::iterator itemIteratorR = getBeginItemsRequiredIterator();

		while (itemIteratorR != getEndItemsRequiredIterator()){
			levelItems.push_back( *(itemIteratorR) );
			itemIteratorR++; 
			//itemIteratorLI++;
		}

		// THEN PROCEED AS NORMAL
		list<int>::iterator itemIterator = getBeginLevelItemsIterator();
		int itemNumber = -1;

		while (itemIterator != getEndLevelItemsIterator()){

			itemNumber = *(itemIterator);

			spendingLimit += stod(items.at(itemNumber).price);

			itemIterator++;
		}
	}
	

	levelItemsCost = spendingLimit;

	// GIVE THE PLAYER A BUFFER FOR THE SPENDING LIMIT
	double randomCash = (double)(rand() % 10 + 7); //$7.00 - $16.00
	spendingLimit += randomCash;

}

/********************************
*		FILE READING/PARSING	*
*********************************/

vector<string> GroceryDashDataLoader::split(const char *str, char c)
{
	vector<string> result;

	do
	{
		const char *begin = str;

		while (*str != c && *str)
			str++;

		result.push_back(string(begin, str));
	} while (0 != *str++);

	return result;
}


void GroceryDashDataLoader::loadItemData(){
	string fn = "data/LuaScripts/GameItems.txt";
	fileReaderItemData(fn);
}

void GroceryDashDataLoader::fileReaderItemData(string name){
	vector<string> lineResults;
	ifstream inFile(name);

	if (!inFile) {
		cerr << "File not found." << endl;
		return;
	}

	// Using getline() to read one line at a time.
	string line;
	getline(inFile, line);
	while (getline(inFile, line)) {
		if (line.empty()){
			//continue;
		}
		else{
			lineResults = split(line.c_str(), ',');
			Item i;
			i.name = lineResults[0];
			i.price = lineResults[1];
			i.location = stoi(lineResults[2]);
			i.position = stoi(lineResults[3]);
			items.push_back(i);
		}
	}

	inFile.close();
	return;
}


void GroceryDashDataLoader::loadLevelItems(int levelNum){

	string fn;
	levelItems.clear();
	clearItemsRequired();

	switch (levelNum){
		
		case 0:{
				   fn = "data/LuaScripts/Level_0_items.txt";
				   fileReaderLevelItems(fn);
		}
			break;
		case 1:{
				   fn = "data/LuaScripts/Level_1_items.txt";
				   fileReaderLevelItems(fn);
		}
			break;
		case 2:{
				   fn = "data/LuaScripts/Level_2_items.txt";
				   fileReaderLevelItems(fn);
		}
			break;
		case 3:{
				   fn = "data/LuaScripts/Level_3_items.txt";
				   fileReaderLevelItems(fn);
		}
			break;
		default:{	/*	Generate a random list of items */
					generateRandomItemsList();
		}
			break;
	}// SWITCH




}

void GroceryDashDataLoader::fileReaderLevelItems(string name){

	ifstream inFile(name);

	if (!inFile) {
		cerr << "File not found." << endl;
		return;
	}

	// Using getline() to read one line at a time.
	string line;
	int itemNum = -1;
	
	while ( getline(inFile, line) ) {
		if (line.empty()) continue;

		itemNum = stoi(line);

		if (itemNum >= 0){
			addItemToShoppingList(itemNum);
			levelItems.push_back(itemNum);
		}
		
	}

	inFile.close();
	return;

}