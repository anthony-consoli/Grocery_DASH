/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	This class provides a framework for technology-specific
	classes that will manage the rendering of all game textures and text.
	It provides methods for the initialization of objects necessary
	for rendering images and text.

	Rendering should be done by generating a list of RenderItem
	objects each frame and putting them into a list. These
	objects store information about the textures that have
	to be drawn to the screen.
*/

#pragma once

#include "Box2D\Box2D.h"

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\RenderList.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\text\GameText.h"
#include "sssf\text\TextFileWriter.h"

const static unsigned int DEFAULT_INIT_GUI_RENDER_LIST_SIZE = 100;
const static unsigned int DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE = 1000;

class TextureManager;

class GameGraphics : public b2Draw
{
protected:
	// WE PERIODICALLY NEED ACCESS TO GAME STUFF
	Game *game;

	// WE'LL SORE THE SCREEN DIMENSIONS HERE
	int screenWidth;
	int screenHeight;

	// EACH FRAME, RENDER LISTS ARE CREATED, FILLED WITH
	// DATA, THEN USED FOR RENDERING THE FRAME, AND DESTROYED
	RenderList				*guiRenderList;
	RenderList				*worldRenderList;

	// NOTE THAT IF WE REALLY WANTED TO BE SLICK, WE COULD PROVIDE
	// MULTIPLE WORLD RENDER LISTS, ONE FOR EACH Z-LEVEL

	// THE TextureManagers STORE ALL THE IMAGES FOR OUR GAME

	TextureManager			*guiTextureManager;
	TextureManager			*worldTextureManager;

	// TOGGLES WHETHER OR NOT TO RENDER THE TEXT, WHICH IS USEFUL
	// FOR DISPLAYING DEBUG INFORMATION
	bool debugTextShouldBeRendered;
	bool b2DrawDebugShouldBeRendered;
	bool pathfindingPathShouldBeRendered;
	bool pathfindingGridShouldBeRendered;

public:	
	// INLINED ACCESSOR METHODS
	RenderList*				getGUIRenderList()			{ return guiRenderList;			}
	TextureManager*			getGUITextureManager()		{ return guiTextureManager;		}
	RenderList*				getWorldRenderList()		{ return worldRenderList;		}
	TextureManager*			getWorldTextureManager()	{ return worldTextureManager; }
	bool					getB2DrawDebugShouldBeRendered()	{ return b2DrawDebugShouldBeRendered; }

	// AND A METHOD FOR TURNING DEBUG TEXT ON AND OFF
	void toggleDebugTextShouldBeRendered()
	{ debugTextShouldBeRendered = !debugTextShouldBeRendered; }
	void toggleB2DrawDebugShouldBeRendered()
	{ b2DrawDebugShouldBeRendered = !b2DrawDebugShouldBeRendered; }
	void togglePathfindingGridShouldBeRendered()
	{ pathfindingGridShouldBeRendered = !pathfindingGridShouldBeRendered; }
	void togglePathfindingPathShouldBeRendered()
	{ pathfindingPathShouldBeRendered = !pathfindingPathShouldBeRendered; }

	// VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
	// SPECIFIC CLASSES
	virtual	TextureManager* createTextureManager()=0;
	virtual int				getScreenHeight()=0;
	virtual int				getScreenWidth()=0;
	virtual	void			initGraphics(GameOS *os, bool isFullscreen)=0;
	virtual void			initTextFont(int fontSize)=0;
	virtual void			reloadGraphics()=0;
	virtual void			renderGame(Game *game)=0;
	virtual void			renderTextToDraw(TextToDraw *textToDraw)=0;
	virtual void			setColorKey(int r, int g, int b)=0;
	virtual void			setFontColor(int r, int g, int b)=0;
	virtual void			shutdown()=0;

	virtual void			DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) = 0;

	/// Draw a solid closed polygon provided in CCW order.
	virtual void			DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) = 0;

	/// Draw a circle.
	virtual void			DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) = 0;

	/// Draw a solid circle.
	virtual void			DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) = 0;

	/// Draw a line segment.
	virtual void			DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) = 0;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void			DrawTransform(const b2Transform& xf) = 0;

	// CONCRETE METHODS DEFINED IN GameGraphics.cpp
	GameGraphics();
	~GameGraphics();
	void clearWorldTextures();
	void fillRenderLists(Game *game);
	void init(int screenWidth, int screenHeight);
	void renderText(GameText *text);
	void renderInGameText(Game* game, GameText *text);
	void renderInGameItemList(Game* game, GameText *text);
	void renderSettingsScreenText(Game* game, GameText *text);
	void renderStatsScreenText(Game* game, GameText *text);
	void renderB2Draw(Game *game);
};