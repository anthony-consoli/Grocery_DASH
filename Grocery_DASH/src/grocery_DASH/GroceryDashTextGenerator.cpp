#include "grocery_DASH_VS\stdafx.h"
#include "GroceryDash.h"
#include "GroceryDashDataLoader.h"
#include "GroceryDashTextGenerator.h"
#include "GroceryDashKeyEventHandler.h"

// Box2D INCLUDE
#include <Box2D\Box2D.h>

#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\TopDownSprite.h"
#include "sssf\gsm\ai\bots\RandomWanderingBot.h"
#include "sssf\text\GameText.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

static const wstring	W_MOUSE_COORDS_TEXT = L"Mouse: (";
static const int		W_TEXT_X = 200;
static const int		W_TEXT_Y = 10;

static const int		Y_OFFSET = 64;

static const int		W_GAME_TEXT_X = 10;
static const int		W_GAME_TEXT_Y = Y_OFFSET + 580;

static const int		W_SETTINGS_X = 405;
static const int		W_SETTINGS_Y = 406;

static const int		W_ITEM_LIST_TEXT_X = 265;
static const int		W_ITEM_LIST_TEXT_Y = 65;

/*
static const int		W_PATHFINDING_TEXT_X = 600;
static const int		W_PATHFINDING_TEXT_Y = 10;
static const wstring	W_ROTATION_IN_RADIANS = L"Player Rotation in Radians: ";
static const wstring	W_ROTATION_IN_DEGREES = L"Player Rotation in Degrees: ";
static const wstring	W_RADIANS = L" radians";
static const wstring	W_DEGREES = L" degrees";
static const wstring	SIN_THETA = L"sin(theta): ";
static const wstring	COS_THETA = L"cos(theta): ";
static const wstring	W_PLAYER_CENTER_TEXT = L"Player Center: (";
static const wstring	W_PLAYER_GRID_TEXT = L"Player Center Col,Row: (";
static const wstring	W_PLAYER_LEFT_TOP_TEXT = L"Player Left, Top: (";
static const wstring	W_PLAYER_RIGHT_BOTTOM_TEXT = L"Player Right, Bottom: (";
*/
void GroceryDashTextGenerator::appendMouseCoords(Game *game){

	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();

	Physics *physics = game->getGSM()->getPhysics();
	TopDownSprite *player = game->getGSM()->getSpriteManager()->getPlayer();
	GroceryDashDataLoader *dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());

	textToGenerate.append(W_MOUSE_COORDS_TEXT);
	wss << input->getMousePoint()->x;
	wss << L", ";
	wss << input->getMousePoint()->y;
	wss << L")";
	wss << L"\n";

	if (player->getSpriteType() != nullptr && player->body != nullptr){
		b2Vec2 playerPos = player->body->GetPosition();
		wss << L"PLAYER Position (";
		wss << playerPos.x;
		wss << L", ";
		wss << playerPos.y;
		wss << L")";
		wss << L"\n";

		b2Vec2 playerV = player->body->GetLinearVelocity();
		wss << L"PLAYER Velocity (";
		wss << playerV.x;
		wss << L", ";
		wss << playerV.y;
		wss << L")";
		wss << L"\n";

		wss << L"Shopping List Size: ";
		wss << dataLoader->getItemsRequired().size();
		wss << L"\n";

		wss << L"Level Items Cost: $";
		if (dataLoader->levelItemsCost > 0.00){
			string cost = to_string(dataLoader->levelItemsCost);
			int period = cost.find_first_of('.');
			cost = cost.substr(0, (period + 2) + 1);
			wss << cost.c_str();
		}
		else{
			wss << L"0.00";
		}
		wss << L"\n";

		wss << L"Max Spending Limit: $"; 
		if ( ((dataLoader->levelItemsCost * 2) + MONEY_CHEAT_INCREASE) > 0.00 ){
			string cost = to_string(((dataLoader->levelItemsCost * 2) + MONEY_CHEAT_INCREASE));
			int period = cost.find_first_of('.');
			cost = cost.substr(0, (period + 2) + 1);
			wss << cost.c_str();
		}
		else{
			wss << L"0.00";
		}
		wss << L"\n";
	}
	
	
	textToGenerate.append(wss.str());

}

void GroceryDashTextGenerator::appendPathfindingText(Game *game)
{
	/*
	wstringstream wss;
	if (game->getGSM()->isGameInProgress())
	{
		pathfindingText.append(wss.str());

	}
	*/
}

void GroceryDashTextGenerator::initText(Game *game){

	// FIRST UPDATE THE TEXT THIS GENERATOR IS USING
	appendMouseCoords(game);
	appendInGameText(game);
	appendSettingsScreenText(game);
	appendInGameItemList(game);
	//appendPathfindingText(game);


	// AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	GameText *text = game->getText();
	text->addText(&textToGenerate, W_TEXT_X, W_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addGameText(&inGameText, W_GAME_TEXT_X, W_GAME_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addSettingsText(&settingsScreenText, W_SETTINGS_X, W_SETTINGS_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addInGameItemListText(&inGameItemListText, W_ITEM_LIST_TEXT_X, W_ITEM_LIST_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

	//text->addText(&pathfindingText, W_PATHFINDING_TEXT_X, W_PATHFINDING_TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());

}

void GroceryDashTextGenerator::updateText(Game *game){

	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.

	//pathfindingText.clear();
	textToGenerate.clear();
	inGameText.clear();
	settingsScreenText.clear();
	inGameItemListText.clear();

	//appendPathfindingText(game);
	appendMouseCoords(game);
	appendInGameText(game);
	appendSettingsScreenText(game);
	appendInGameItemList(game);

}

void GroceryDashTextGenerator::appendInGameText(Game* game){

	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	wstringstream wss;

	if (!game->getGSM()->isAtGameOverScreen() && !game->getGSM()->isAtVictoryScreen()){

		if (dataLoader->getIsPayingCashier() == true){ wss << L"You are currently paying for your items."; }
		else if (dataLoader->getHasPaidCashier() == true){ wss << L"You have PAID for your items."; }
		else{ wss << L"You have *NOT* paid for your items!!"; }
		wss << "\n";

		wss << L"TIME REMAINING: ";

		int seconds = (int)(game->getGSM()->getTimeRemaining() / 1000);
		int timeRemaining = (game->getGSM()->getLevelTimeAllowed()) - seconds;
		if (timeRemaining < 0){
			// DISPLAY NOTHINHG
		}
		else{
			int stringSeconds = 0;
			int stringMinutes = 0;
			while (timeRemaining >= 60){
				stringMinutes++;
				timeRemaining -= 60;
			}
			stringSeconds = timeRemaining;

			wss << stringMinutes << L":";
			if (stringSeconds < 10){ wss << L"0"; }
			wss << stringSeconds;
		}
		wss << "\n";

		wss << L"TOTAL COST: $";
		if (dataLoader->currentTotal > 0.00){
			string cost = to_string(dataLoader->currentTotal);
			int period = cost.find_first_of('.');
			cost = cost.substr(0, (period + 2) + 1);
			wss << cost.c_str();
		}
		else{
			wss << L"0.00";
		}
		wss << "\n";

		wss << L"SPENDING LIMIT: $";
		if (dataLoader->spendingLimit > 0.00){
			string limit = to_string(dataLoader->spendingLimit);
			int period = limit.find_first_of('.');
			limit = limit.substr(0, (period + 2) + 1);
			wss << limit.c_str();
		}
		else{
			wss << L"0.00";
		}
		wss << "\n";

		wss << L"# OF ITEMS: ";
		wss << dataLoader->numberOfItems;
		wss << "\n";
	}

	inGameText.append(wss.str());

}

void GroceryDashTextGenerator::appendSettingsScreenText(Game* game){

	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	GroceryDashKeyEventHandler* keh = dynamic_cast<GroceryDashKeyEventHandler*>(game->getInput()->getKeyEventHandler());
	wstringstream wss;

	if (dataLoader->getIsPlayerMale() == true){
		wss << L"MALE";
	}
	else{
		wss << L"FEMALE";
	}

	wss << L"\n"; wss << L"\n"; wss << L"\n"; wss << L"\n"; wss << L"\n";

	if (keh != nullptr){
		wss << L"   ";
		if (keh->getIsWASDmovement() == true){
			wss << L"WASD";
		}
		else{
			wss << L"ARROW KEYS";
		}
		wss << L"\n";
	}


	settingsScreenText.append(wss.str());

}

void GroceryDashTextGenerator::appendInGameItemList(Game* game){
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	wstringstream wss;

	if (game->getGSM()->isGameInProgress() == true && game->getGSM()->isAtItemList() == true){
			
			list<int>::iterator itemIterator = dataLoader->getBeginItemsRequiredIterator();
			int itemNumber = -1;

			while (itemIterator != dataLoader->getEndItemsRequiredIterator()){

				itemNumber = *(itemIterator);

				wss << dataLoader->getItems().at(itemNumber).name.c_str();
				wss << L" ($";
				wss << dataLoader->getItems().at(itemNumber).price.c_str();
				wss << L")";
				wss << L"\n";

				itemIterator++;
			}

	}
	else{

		if (game->getGSM()->isGameInProgress() == true && game->getGSM()->isAtCartScreen() == true){

			list<int>::iterator itemIterator = dataLoader->getBeginItemsInCartIterator();
			int itemNumber = -1;
			bool isLevelItem = false;

			while (itemIterator != dataLoader->getEndItemsInCartIterator()){

				itemNumber = *(itemIterator);

				isLevelItem = dataLoader->isItemOnLevelList(itemNumber);

				if (isLevelItem == true){	wss << L"*** ";	}

				wss << dataLoader->getItems().at(itemNumber).name.c_str();
				wss << L" ($";
				wss << dataLoader->getItems().at(itemNumber).price.c_str();
				wss << L")";

				if (isLevelItem == true){ wss << L" ***"; }
				wss << L"\n";

				itemIterator++;
			}
		}

	}

	inGameItemListText.append(wss.str());
}