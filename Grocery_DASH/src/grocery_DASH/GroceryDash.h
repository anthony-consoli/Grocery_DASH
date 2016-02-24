#pragma once

#include "grocery_DASH_VS\stdafx.h"


/*

GroceryDash.h

	This file contains setup data for making the Grocery DASH game.

*/

// FILES
static const wstring	W_INIT_FILE = L"data/Grocery_DASH_Init.txt";
static const wstring	W_GUI_INIT_FILE = L"data/gui/Grocery_DASH_GUI.txt";
static const string		W_DEBUG_FILE = "DebugOutput.txt";
static const wstring	W_SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	W_SPRITE_TYPES_LIST = W_SPRITE_TYPES_DIR + L"sprite_type_list.xml";

// LEVEL FILES and DIRECTORIES

static const wstring	W_LEVEL_0_NAME = L"Level_0.tmx";
static const wstring	W_LEVEL_0_DIR = L"data/levels/TestLevel/";
static const wstring	W_LEVEL_0_PATH = W_LEVEL_0_DIR + W_LEVEL_0_NAME;


static const wstring	W_LEVEL_1_NAME = L"Level_1.tmx";
static const wstring	W_LEVEL_1_DIR = L"data/levels/Level_1/";
static const wstring	W_LEVEL_1_PATH = W_LEVEL_1_DIR + W_LEVEL_1_NAME;


static const wstring	W_LEVEL_2_NAME = L"Level_2.tmx";
static const wstring	W_LEVEL_2_DIR = L"data/levels/Level_2/";
static const wstring	W_LEVEL_2_PATH = W_LEVEL_2_DIR + W_LEVEL_2_NAME;


static const wstring	W_LEVEL_3_NAME = L"Level_3.tmx";
static const wstring	W_LEVEL_3_DIR = L"data/levels/Level_3/";
static const wstring	W_LEVEL_3_PATH = W_LEVEL_3_DIR + W_LEVEL_3_NAME;

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const float		W_GRAVITY = 0.0f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 40;
static const float		PLAYER_INIT_X = 128.0f;
static const float		PLAYER_INIT_Y = 3008.0f;
static const float		JUMP_SPEED = -24.0f;
extern float			PLAYER_SPEED;

static const float		SCALING_FACTOR = 64.0f;
static const float		MAX_VIEWPORT_AXIS_VELOCITY_M = 0.3125f;
//static const float	PLAYER_SPEED = 100.0f;

static const float		START_POS_X_L1 = 2.0f;
static const float		START_POS_Y_L1 = 46.0f;
static const float		START_POS_X_L2 = 2.0f;
static const float		START_POS_Y_L2 = 25.0f;
static const float		START_POS_X_L3 = 2.0f;
static const float		START_POS_Y_L3 = 46.0f;

// ALL TIMES ARE GIVEN IN SECONDS
static const int		TIME_LIMIT_L1 = 1200;				// 20 minutes
static const int		TIME_LIMIT_L2 = 1200;				// 20 minutes
static const int		TIME_LIMIT_L3 = 1200;				// 20 minutes
static const int		TIME_LIMIT_CHEAT_DECREASE = 15;		// 15 seconds
static const int		TIME_LIMIT_CHEAT_INCREASE = 30;		// 30 seconds
static const double		MONEY_CHEAT_DECREASE = 5.00;		// $5.00
static const double		MONEY_CHEAT_INCREASE = 10.00;		// $10.00



// FOR THE SPRITES' STATES
static const wstring	W_MALE_CART = L"MALE_CART";
static const wstring	W_FEMALE_CART = L"FEMALE_CART";
static const wstring	W_WALKING = L"WALKING";
static const wstring	W_STANDING = L"STANDING";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE = L"TITLE";
static const wstring	W_SCREEN_WIDTH = L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT = L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE = L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE = L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X = L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y = L"VIEWPORT_OFFSET_Y";
static const wstring	W_TOGGLE_OFFSET_Y = L"TOGGLE_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED = L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN = L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE = L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED = L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN = L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE = L"COLOR_KEY_BLUE";

// HERE ARE THE GUI PROPERTIES WE ARE LOADING FROM FROM THE GUI SETUP FILE
static const wstring W_SPLASH_SCREEN = L"SPLASH_SCREEN";
static const wstring W_MAIN_MENU = L"MAIN_MENU";
static const wstring W_LEVEL_SELECT = L"LEVEL_SELECT";
static const wstring W_ACHIEVEMENTS_SCREEN = L"ACHIEVEMENTS_SCREEN";
static const wstring W_STATS_SCREEN = L"STATS_SCREEN";
static const wstring W_SETTINGS_SCREEN = L"SETTINGS_SCREEN";
static const wstring W_GAME_SCREEN = L"GAME_SCREEN";
static const wstring W_HELP_SCREEN = L"HELP_SCREEN";
static const wstring W_ABOUT_SCREEN = L"ABOUT_SCREEN";
static const wstring W_PAUSE_SCREEN = L"PAUSE_SCREEN";
static const wstring W_IN_GAME_HELP_SCREEN = L"IN_GAME_HELP_SCREEN";
static const wstring W_LOADING_SCREEN = L"LOADING_SCREEN";
static const wstring W_UNLOADING_SCREEN = L"UNLOADING_SCREEN";
static const wstring W_ITEM_LIST_SCREEN = L"ITEM_LIST_SCREEN";
static const wstring W_CART_SCREEN = L"CART_SCREEN";
static const wstring W_VICTORY_SCREEN = L"VICTORY_SCREEN";
static const wstring W_GAME_OVER_SCREEN = L"GAME_OVER_SCREEN";

static const wstring W_SHELF_SCREEN_1 = L"SHELF_SCREEN_1";
static const wstring W_SHELF_SCREEN_2 = L"SHELF_SCREEN_2";
static const wstring W_SHELF_SCREEN_3 = L"SHELF_SCREEN_3";
static const wstring W_SHELF_SCREEN_4 = L"SHELF_SCREEN_4";
static const wstring W_SHELF_SCREEN_5 = L"SHELF_SCREEN_5";
static const wstring W_SHELF_SCREEN_6 = L"SHELF_SCREEN_6";
static const wstring W_SHELF_SCREEN_7 = L"SHELF_SCREEN_7";
static const wstring W_SHELF_SCREEN_8 = L"SHELF_SCREEN_8";
static const wstring W_SHELF_SCREEN_9 = L"SHELF_SCREEN_9";
static const wstring W_SHELF_SCREEN_10 = L"SHELF_SCREEN_10";
static const wstring W_SHELF_SCREEN_P1 = L"SHELF_SCREEN_P1";
static const wstring W_SHELF_SCREEN_P2 = L"SHELF_SCREEN_P2";

static const wstring W_CURSOR_NORMAL = L"CURSOR_NORMAL";

static const wstring W_BUTTON_START_GAME = L"BUTTON_START_GAME";
static const wstring W_BUTTON_START_GAME_MO = L"BUTTON_START_GAME_MO";
static const wstring W_BUTTON_STATS = L"BUTTON_STATS";
static const wstring W_BUTTON_STATS_MO = L"BUTTON_STATS_MO";
static const wstring W_BUTTON_ACHIEVEMENTS = L"BUTTON_ACHIEVEMENTS";
static const wstring W_BUTTON_ACHIEVEMENTS_MO = L"BUTTON_ACHIEVEMENTS_MO";
static const wstring W_BUTTON_SETTINGS = L"BUTTON_SETTINGS";
static const wstring W_BUTTON_SETTINGS_MO = L"BUTTON_SETTING_MO";
static const wstring W_BUTTON_QUIT = L"BUTTON_QUIT";
static const wstring W_BUTTON_QUIT_MO = L"BUTTON_QUIT_MO";
static const wstring W_BUTTON_HELP = L"BUTTON_HELP";
static const wstring W_BUTTON_HELP_MO = L"BUTTON_HELP_MO";
static const wstring W_BUTTON_ABOUT = L"BUTTON_ABOUT";
static const wstring W_BUTTON_ABOUT_MO = L"BUTTON_ABOUT_MO";
static const wstring W_BUTTON_EXIT = L"BUTTON_EXIT";
static const wstring W_BUTTON_EXIT_MO = L"BUTTON_EXIT_MO";
static const wstring W_BUTTON_P_RESUME = L"BUTTON_P_RESUME";
static const wstring W_BUTTON_P_RESUME_MO = L"BUTTON_P_RESUME_MO";
static const wstring W_BUTTON_P_RESTART = L"BUTTON_P_RESTART";
static const wstring W_BUTTON_P_RESTART_MO = L"BUTTON_P_RESTART_MO";
static const wstring W_BUTTON_P_HELP = L"BUTTON_P_HELP";
static const wstring W_BUTTON_P_HELP_MO = L"BUTTON_P_HELP_MO";
static const wstring W_BUTTON_P_EXIT = L"BUTTON_P_EXIT";
static const wstring W_BUTTON_P_EXIT_MO = L"BUTTON_P_EXIT_MO";
static const wstring W_BUTTON_TOGGLE_GENDER = L"BUTTON_TOGGLE_GENDER";
static const wstring W_BUTTON_TOGGLE_GENDER_MO = L"BUTTON_TOGGLE_GENDER_MO";
static const wstring W_BUTTON_TOGGLE_WASD = L"BUTTON_TOGGLE_WASD";
static const wstring W_BUTTON_TOGGLE_WASD_MO = L"BUTTON_TOGGLE_WASD_MO";
static const wstring W_BUTTON_LIST = L"BUTTON_LIST";
static const wstring W_BUTTON_LIST_MO = L"BUTTON_LIST_MO";
static const wstring W_BUTTON_CART = L"BUTTON_CART";
static const wstring W_BUTTON_CART_MO = L"BUTTON_CART_MO";


static const wstring W_BUTTON_AISLE_1 = L"BUTTON_AISLE_1";
static const wstring W_BUTTON_AISLE_1_MO = L"BUTTON_AISLE_1_MO";
static const wstring W_BUTTON_AISLE_2 = L"BUTTON_AISLE_2";
static const wstring W_BUTTON_AISLE_2_MO = L"BUTTON_AISLE_2_MO";
static const wstring W_BUTTON_AISLE_3 = L"BUTTON_AISLE_3";
static const wstring W_BUTTON_AISLE_3_MO = L"BUTTON_AISLE_3_MO";
static const wstring W_BUTTON_AISLE_4 = L"BUTTON_AISLE_4";
static const wstring W_BUTTON_AISLE_4_MO = L"BUTTON_AISLE_4_MO";

static const wstring W_BUTTON_SHELF_ITEM_1 = L"BUTTON_SHELF_ITEM_1";
static const wstring W_BUTTON_SHELF_ITEM_1_MO = L"BUTTON_SHELF_ITEM_1_MO";
static const wstring W_BUTTON_SHELF_ITEM_2 = L"BUTTON_SHELF_ITEM_2";
static const wstring W_BUTTON_SHELF_ITEM_2_MO = L"BUTTON_SHELF_ITEM_2_MO";
static const wstring W_BUTTON_SHELF_ITEM_3 = L"BUTTON_SHELF_ITEM_3";
static const wstring W_BUTTON_SHELF_ITEM_3_MO = L"BUTTON_SHELF_ITEM_3_MO";
static const wstring W_BUTTON_SHELF_ITEM_4 = L"BUTTON_SHELF_ITEM_4";
static const wstring W_BUTTON_SHELF_ITEM_4_MO= L"BUTTON_SHELF_ITEM_4_MO";
static const wstring W_BUTTON_SHELF_ITEM_5 = L"BUTTON_SHELF_ITEM_5";
static const wstring W_BUTTON_SHELF_ITEM_5_MO = L"BUTTON_SHELF_ITEM_5_MO";
static const wstring W_BUTTON_SHELF_ITEM_6 = L"BUTTON_SHELF_ITEM_6";
static const wstring W_BUTTON_SHELF_ITEM_6_MO = L"BUTTON_SHELF_ITEM_6_MO";
static const wstring W_BUTTON_SHELF_ITEM_7 = L"BUTTON_SHELF_ITEM_7";
static const wstring W_BUTTON_SHELF_ITEM_7_MO = L"BUTTON_SHELF_ITEM_7_MO";
static const wstring W_BUTTON_SHELF_ITEM_8 = L"BUTTON_SHELF_ITEM_8";
static const wstring W_BUTTON_SHELF_ITEM_8_MO = L"BUTTON_SHELF_ITEM_8_MO";

static const wstring W_BUTTON_RESET_STATS = L"BUTTON_RESET_STATS";
static const wstring W_BUTTON_RESET_STATS_MO = L"BUTTON_RESET_STATS_MO";
static const wstring W_BUTTON_HELP_SETTINGS = L"BUTTON_HELP_SETTINGS";
static const wstring W_BUTTON_HELP_SETTINGS_MO = L"BUTTON_HELP_SETTINGS_MO";
static const wstring W_BUTTON_ABOUT_SETTINGS = L"BUTTON_ABOUT_SETTINGS";
static const wstring W_BUTTON_ABOUT_SETTINGS_MO = L"BUTTON_ABOUT_SETTINGS_MO";

// BUTTON COMMANDS
static const wstring W_GO_TO_MM_COMMAND = L"Main_Menu";
static const wstring W_GO_TO_LS_COMMAND = L"Level_Select";

static const wstring W_LOAD_L1_COMMAND = L"Level_1";
static const wstring W_LOAD_L2_COMMAND = L"Level_2";
static const wstring W_LOAD_L3_COMMAND = L"Level_3";
static const wstring W_LOAD_QP_COMMAND = L"Quick_Play";
static const wstring W_TOGGLE_GENDER_COMMAND = L"TOGGLE_GENDER";
static const wstring W_TOGGLE_WASD_COMMAND = L"TOGGLE_WASD";


static const wstring W_START_COMMAND = L"Start";
static const wstring W_GO_TO_STATS_COMMAND = L"Stats";
static const wstring W_GO_TO_ACHIEVEMENTS_COMMAND = L"Achievements";
static const wstring W_GO_TO_SETTINGS_COMMAND = L"Settings";
static const wstring W_GO_TO_IG_HELP_COMMAND = L"IG_Help";
static const wstring W_HELP_COMMAND = L"Help";
static const wstring W_ABOUT_COMMAND = L"About";
static const wstring W_GO_TO_ITEM_LIST_COMMAND = L"ITEM_LIST_COMMAND";
static const wstring W_GO_TO_CART_COMMAND = L"CART_COMMAND";

static const wstring W_NO_COMMAND = L"NO_COMMAND";
static const wstring W_RESTART_LEVEL_COMMAND = L"Restart_Level";
static const wstring W_RETURN_TO_GAME_COMMAND = L"Return_to_Game";
static const wstring W_EXIT_TO_MM_COMMAND = L"Exit_to_MM"; //USED TO LEAVE A SCREEN within THE GAME to the Main Menu
static const wstring W_EXIT_TO_LS_COMMAND = L"Exit_to_LS";
static const wstring W_QUIT_COMMAND = L"Quit"; //USED TO LEAVE THE GAME

static const wstring W_ADD_ITEM_1_TO_CART_COMMAND = L"ADD_ITEM_1_TO_CART";
static const wstring W_ADD_ITEM_2_TO_CART_COMMAND = L"ADD_ITEM_2_TO_CART";
static const wstring W_ADD_ITEM_3_TO_CART_COMMAND = L"ADD_ITEM_3_TO_CART";
static const wstring W_ADD_ITEM_4_TO_CART_COMMAND = L"ADD_ITEM_4_TO_CART";
static const wstring W_ADD_ITEM_5_TO_CART_COMMAND = L"ADD_ITEM_5_TO_CART";
static const wstring W_ADD_ITEM_6_TO_CART_COMMAND = L"ADD_ITEM_6_TO_CART";
static const wstring W_ADD_ITEM_7_TO_CART_COMMAND = L"ADD_ITEM_7_TO_CART";
static const wstring W_ADD_ITEM_8_TO_CART_COMMAND = L"ADD_ITEM_8_TO_CART";

// KEY COMMANDS
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';

const unsigned int UP_ARROW = VK_UP;
const unsigned int DOWN_ARROW = VK_DOWN;
const unsigned int LEFT_ARROW = VK_LEFT;
const unsigned int RIGHT_ARROW = VK_RIGHT;

const unsigned int SPACE_BAR = VK_SPACE;
const unsigned int SHIFT_KEY = VK_SHIFT;
const unsigned int ESC_KEY = VK_ESCAPE;


const unsigned int G_KEY = (unsigned int)'G';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int R_KEY = (unsigned int)'R';
const unsigned int Q_KEY = (unsigned int)'Q';
const unsigned int E_KEY = (unsigned int)'E';
const unsigned int Z_KEY = (unsigned int)'Z';
const unsigned int X_KEY = (unsigned int)'X';
const unsigned int C_KEY = (unsigned int)'C';
const unsigned int H_KEY = (unsigned int)'H';

const unsigned int I_KEY = (unsigned int)'I';
const unsigned int J_KEY = (unsigned int)'J';
const unsigned int K_KEY = (unsigned int)'K';
const unsigned int L_KEY = (unsigned int)'L';
const unsigned int O_KEY = (unsigned int)'O';
const unsigned int M_KEY = (unsigned int)'M';
const unsigned int Y_KEY = (unsigned int)'Y';
const unsigned int U_KEY = (unsigned int)'U';

const unsigned int NUM_1 = (unsigned int)'1'; const unsigned int NUMPAD_1 = VK_NUMPAD1;
const unsigned int NUM_2 = (unsigned int)'2'; const unsigned int NUMPAD_2 = VK_NUMPAD2;
const unsigned int NUM_3 = (unsigned int)'3'; const unsigned int NUMPAD_3 = VK_NUMPAD3;
const unsigned int NUM_4 = (unsigned int)'4'; const unsigned int NUMPAD_4 = VK_NUMPAD4;
const unsigned int NUM_5 = (unsigned int)'5'; const unsigned int NUMPAD_5 = VK_NUMPAD5;
const unsigned int NUM_6 = (unsigned int)'6'; const unsigned int NUMPAD_6 = VK_NUMPAD6;
const unsigned int NUM_7 = (unsigned int)'7'; const unsigned int NUMPAD_7 = VK_NUMPAD7;
const unsigned int NUM_8 = (unsigned int)'8'; const unsigned int NUMPAD_8 = VK_NUMPAD8;
