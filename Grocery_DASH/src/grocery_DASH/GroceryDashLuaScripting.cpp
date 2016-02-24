#include <iostream>

#include "LuaPlus.h"
using namespace LuaPlus;
#include "stdio.h"
using namespace std;

#include "grocery_DASH\GroceryDash.h"
#include "grocery_DASH\GroceryDashLuaScripting.h"
#include "grocery_DASH\GroceryDashButtonEventHandler.h"
#include "grocery_DASH\GroceryDashDataLoader.h"
#include "grocery_DASH\GroceryDashKeyEventHandler.h"
#include "grocery_DASH\GroceryDashTextGenerator.h"
#include "sssf\gsm\state\GameState.h"


string dir = "data/LuaScripts/";

string directoryPath = "../../../Apps/Grocery_DASH/data/LuaScripts/";


void getAndPrintSpeed(LuaState *ls)
{
	// Get a global variable
	LuaObject numObj = ls->GetGlobal("speed");
	int num = numObj.GetInteger();
	cout << num;
	cout << endl;
}

void runScript(Game* game)
{
	// Init Lua
	LuaState* luaPState = LuaState::Create();

	// Open the Lua Script File
	string file = dir + "Script.lua";

	int result = luaPState->DoFile(file.c_str());
	//LuaFunction<float> luaDoubleSpeed = luaPState->GetGlobal("doubleSpeed");
	//luaDoubleSpeed();

	LuaObject strObj = luaPState->GetGlobal("script");
	string str = dir + strObj.GetString();
	result = luaPState->DoFile(str.c_str());

	//string temp = dir + "PlayerScript.lua";
	if (str.compare(dir + "PlayerScript.lua") == 0){
		LuaObject sObj = luaPState->GetGlobal("speed");
		float s = sObj.GetFloat();
		PLAYER_SPEED = s;

		LuaObject tObj = luaPState->GetGlobal("time");
		int time = tObj.GetInteger();
		long tR = (int) (game->getGSM()->getTimeRemaining() / 1000);

		long nn = (game->getGSM()->getLevelTimeAllowed()) - tR + -time;
		game->getGSM()->setLevelTimeAllowed(nn);

	}

	//LuaState::Destroy(luaPState);
	
}

void runScript_DoNotUse(Game *game, int shelfNumber, int itemNumber){

	//if (true){
	//	runScript_FixMethodCall(game, shelfNumber, itemNumber);
	//	return;
	//}

	// NEEDED GAME OBJECTS
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());


	// Init Lua
	LuaState* luaPState = LuaState::Create();

	// Open the Lua Script File
	string file = dir + "Script.Lua";

	int result = luaPState->LoadFile(file.c_str());
	LuaObject strObj = luaPState->GetGlobal("ItemScript");
	string str = dir + strObj.GetString();
	result = luaPState->LoadFile(str.c_str());

	double itemPrice = -1.00;
	//if (str.compare(dir + "ItemScript.lua") == 0){
	if (result == 0){

		int stopper_1 = 0;

		LuaObject pObj = luaPState->GetGlobal("item");
		double p = pObj.GetDouble();
		itemPrice = p;

		int stopper_2 = 0;


		//LuaObject luaFunction = luaPState->GetGlobal("determineShelf");
		//LuaFunction<double> luaDetermineShelf = luaFunction.GetCFunction();
		//itemPrice = luaDetermineShelf(shelfNumber, itemNumber);
		/*
		lua_State *L = *(luaPState);
		luaL_loadfile(L, "ItemScript.lua");
		luaL_dofile(L, "ItemScript.lua");

		lua_Number sN = shelfNumber;
		lua_Number iN = itemNumber;
		*/
		//lua_getglobal(L, "determineShelf");  /* function to be called */
		//lua_pushnumber(L, sN);   /* push 1st argument */
		//lua_pushnumber(L, iN);   /* push 2nd argument */
		/* do the call (2 arguments, 1 result)
		int rd = lua_pcall(L, 2, 1, 0);
		if (lua_pcall(L, 2, 1, 0) != 0){
			game->getText()->writeDebugOutput("ERROR IN FUNCTION CALL: ");
			game->getText()->writeDebugOutput(lua_tostring(L, -1));
			game->getText()->writeDebugOutput("\n");
			//return;
		}
		*/
		/* retrieve result */
		//if (!lua_isnumber(L, -1)){
			//return;
			//	error(*(luaPState), "function `f' must return a number");
		//}
		//itemPrice = lua_tonumber(L, -1);
		//lua_pop(L, 1);  /* pop returned value */
		//return itemPrice;
		
		
		//itemPrice = 12345.00;
		if (itemPrice > 1.00){
			dataLoader->addItemToCart(itemPrice);
		}
		else{
			dataLoader->addItemToCart(1.00);
		}
		
	}
	else{
		game->getText()->writeDebugOutput("ERROR: result != 0\n");
	}

}

void runScript_LookUpShelfItemInfo(Game *game, double shelfNumber, double itemNumber, int isTakingItem){
	// NEEDED GAME OBJECTS
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	double itemPrice = -1.00;

	double one		= 1.00;
	double two		= 2.00;
	double three	= 3.00;
	double four		= 4.00;
	double five		= 5.00;
	double six		= 6.00;
	double seven	= 7.00;
	double eight	= 8.00;
	double nine		= 9.00;
	double ten		= 10.00;
	double eleven	= 11.00;
	double twelve	= 12.00;

	/* the Lua interpreter */
	lua_State* L;

	L = lua_open();

	/* load Lua base libraries */
	//luaL_openlibs(L);

	//
	string filePath = dir + "ItemScript.lua";

	/* load the script */
	int k = luaL_dofile(L, filePath.c_str());
	if (k != 0){
		game->getText()->writeDebugOutput("ERROR!!\n     luaL_dofile(...) >>> ");
		game->getText()->writeDebugOutput(lua_tostring(L, -1));
		game->getText()->writeDebugOutput("\n\n");
		//return;
	}
	else{

		/* the function name */
		lua_getglobal(L, "determineShelf");

		lua_pushnumber(L, shelfNumber);	//the first argument
		lua_pushnumber(L, itemNumber);	//the second argument

		lua_pushnumber(L, one);
		lua_pushnumber(L, two);
		lua_pushnumber(L, three);
		lua_pushnumber(L, four);
		lua_pushnumber(L, five);
		lua_pushnumber(L, six);
		lua_pushnumber(L, seven);
		lua_pushnumber(L, eight);
		lua_pushnumber(L, nine);
		lua_pushnumber(L, ten);
		lua_pushnumber(L, eleven);
		lua_pushnumber(L, twelve);
		/* call the function with 14 arguments, return 1 result */

		if (lua_pcall(L, 14, 1, 0) != 0){
			game->getText()->writeDebugOutput("ERROR IN FUNCTION CALL: ");
			game->getText()->writeDebugOutput(lua_tostring(L, -1));
			game->getText()->writeDebugOutput("\n");
			//return;
		}

		/* get the result */
		lua_Number returnValue = lua_tonumber(L, -1);
		itemPrice = (double)returnValue;

		if (itemPrice > 0.00){
			
			if (isTakingItem == 1){
				dataLoader->addItemToCart(itemPrice);
				game->getGSM()->getAudio()->playAudio(4);
			}
			
			if (isTakingItem == 0){
				dataLoader->addItemToCart(-itemPrice);
				game->getGSM()->getAudio()->playAudio(5);
			}

			if (isTakingItem == 2){
				// DO NOTHING
			}

			
		}
		else{
			dataLoader->addItemToCart(0.00);
			game->getText()->writeDebugOutput("returnValue = ");
			game->getText()->writeDebugOutput(lua_tostring(L, -1));
			game->getText()->writeDebugOutput("\n");
		}
		
		
		

		lua_pop(L, 1);

		/* cleanup Lua */
		lua_close(L);
	}

	


}

void runScript_TimeCheat(Game *game, bool isTimeIncreasing){

	// Init Lua
	LuaState* luaPState = LuaState::Create();

	// Open the Lua Script File
	string file = dir + "Script.lua";

	int result = luaPState->DoFile(file.c_str());
	//LuaFunction<float> luaDoubleSpeed = luaPState->GetGlobal("doubleSpeed");
	//luaDoubleSpeed();

	LuaObject strObj = luaPState->GetGlobal("script");
	string str = dir + strObj.GetString();
	result = luaPState->DoFile(str.c_str());

	//if (str.compare(dir + "PlayerScript.lua") == 0){
	if (result == 0){
		//LuaObject sObj = luaPState->GetGlobal("speed");
		//float s = sObj.GetFloat();
		//PLAYER_SPEED = s;

		//LuaObject tObj = luaPState->GetGlobal("time");
		//int time = tObj.GetInteger();
		//long tR = (int)(game->getGSM()->getTimeRemaining() / 1000);

		LuaObject iObj = luaPState->GetGlobal("timeIncrease");
		int timeIncrement = iObj.GetInteger();

		LuaObject dObj = luaPState->GetGlobal("timeDecrease");
		int timeDecrement = dObj.GetInteger();

		/*
		positive 'time' -> increase time
		negative 'time' -> decrease time
		*/
		long actualTime = game->getGSM()->getLevelTimeAllowed();
		if (isTimeIncreasing == true){
			//long nn = (game->getGSM()->getLevelTimeAllowed()) - tR + time;
			int nn = (game->getGSM()->getLevelTimeAllowed()) + timeIncrement;
			game->getGSM()->setLevelTimeAllowed(nn);
		}
		else{
			//long nn = (game->getGSM()->getLevelTimeAllowed()) - tR + -time;
			long nn = (game->getGSM()->getLevelTimeAllowed()) - timeDecrement;
			game->getGSM()->setLevelTimeAllowed(nn);
		}

	}


	//LuaState::Destroy(luaPState);

}