/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameInput.h

	This class manages all game input. This is currently done only
	via a keyboard and mouse. Windows method are used to get input
	information each frame.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"
#include "sssf\input\MouseEventHandler.h"

#define NUM_MOUSE_BUTTONS 2

// FORWARD DECLARATIONS OF CLASSES USED BY GameInput.h
class Cursor;
class Game;
class GameGUI;

class GameInput
{
protected:
	/**
	KeyState structure is used to represent the current
	state of a keyboard key. It would be updated each
	frame and helps us know which keys are pressed, and
	whether this is the first frame it has been pressed
	or if it has been held down for consecutive frames,
	i.e., it has been held down.
	**/
	struct InputState
	{
		bool isPressed;
		bool isFirstPress;
		bool wasHeldDown;
	} inputState[256]; // stores the state for all keys

	int mouseX;
	int mouseY;

	// THE KEY HANDLER RESPONDS TO KEY PRESSES
	KeyEventHandler *keyHandler;

	// THE MOUSE HANDLER RESPONDS TO MOUSE PRESSES
	MouseEventHandler *mouseHandler;

public:
	// INLINED METHODS
	int getMouseX() { return mouseX; }
	int getMouseY() { return mouseY; }

	// CHECKS TO SEE IF A KEY IS CURRENTLY PRESSED
	bool	isKeyDown(int key)					
	{	return inputState[key].isPressed;				}

	// CHECKS TO SEE IF A KEY WAS RECENTLY PRESSED (FIRED ONCE PER PRESS)
	bool	isKeyDownForFirstTime(int key)		
	{	return inputState[key].isFirstPress;			}

	// REGISTERS A KeyEventHandler 
	void	registerKeyHandler(KeyEventHandler *initKeyHandler)
	{	keyHandler = initKeyHandler;					}

	// REGISTERS A MouseEventHandler
	void	registerMouseHandler(MouseEventHandler *initMouseHandler)
	{	mouseHandler = initMouseHandler;				}

	// ASKS THE keyHandler TO RESPOND
	void	respondToKeyboardInput(Game *game)	
	{	keyHandler->handleKeyEvents(game);				}

	// RETURN THE keyHandler OBJECT
	KeyEventHandler*	getKeyEventHandler()
	{	return keyHandler;	}


	// METHODS DEFINED IN GameInput.cpp
	GameInput();
	~GameInput();
	void initInputState();
	virtual void updateInputState()=0;
	virtual void processInput(Game *game)=0;
	virtual void respondToMouseInput(Game *game)=0;
	virtual void shutdown()=0;
};