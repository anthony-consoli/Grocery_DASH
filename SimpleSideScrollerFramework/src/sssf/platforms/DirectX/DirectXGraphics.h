/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

DirectXGraphics.h

This class is a GameGraphics class that does all graphics
management using DirectX.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"
#include "Box2D\Box2D.h"

// THIS IS OUR DESIRED COLOR FORMAT. HOPEFULLY THE PLAYER'S
// GRAPHICS CARD WILL HAVE IT
const D3DFORMAT DEFAULT_COLOR_FORMAT = D3DFMT_X8R8G8B8;

// THIS IS THE COLOR WE WILL USE TO CLEAR THE SCREEN WITH
const D3DCOLOR	BACKGROUND_COLOR = D3DCOLOR_XRGB(96, 96, 96);

// BY DEFAULT, WE WILL USE THIS FOR TEXTURE DRAWING, IT USES NO TRANSPARENCY
const D3DCOLOR	DEFAULT_ALPHA_COLOR = D3DCOLOR_ARGB(255, 255, 255, 255);

const D3DCOLOR ORIGIN_PATHFINDING_CELL_COLOR = D3DCOLOR_ARGB(255, 250, 50, 50);
const D3DCOLOR DEFAULT_PATHFINDING_CELL_COLOR = D3DCOLOR_ARGB(255, 255, 255, 255);
const D3DCOLOR DESTINATION_PATHFINDING_CELL_COLOR = D3DCOLOR_ARGB(255, 50, 50, 200);
const D3DCOLOR SELECTED_PATHFINDING_CELL_COLOR = D3DCOLOR_ARGB(255, 100, 255, 100);
const D3DCOLOR NONWALKABLE_TILE_COLOR = D3DCOLOR_ARGB(255, 0, 0, 0);
const D3DCOLOR WALKABLE_TILE_COLOR = D3DCOLOR_ARGB(255, 255, 255, 255);

class DirectXGraphics : public GameGraphics
{
private:
	// DIRECT3D STUFF FOR RENDERING
	LPDIRECT3D9				d3d;
	LPDIRECT3DDEVICE9		graphicsDevice;
	D3DPRESENT_PARAMETERS	presentParameters;
	LPD3DXSPRITE			spriteHandler;
	D3DCOLOR				colorKey;
	D3DCOLOR				fontColor;
	LPD3DXFONT				textFont;
	RECT					textRect;

	// INTERNAL METHODS DEFINED IN DirectXGraphics.cpp
	HRESULT						createDirectXDeviceAndSpriteHandler();
	void						endDirectXFrameRendering();
	vector<D3DDISPLAYMODE*>*	getDirectXDisplayModes();
	void						renderGUIRenderList();
	void						renderWorldRenderList();
	void						startDirectXFrameRendering();

public:

	LPD3DXLINE lineSprite;
	// INLINED ACCESSOR METHODS
	D3DCOLOR				getColorKey()				{ return colorKey; }
	D3DCOLOR				getFontColor()				{ return fontColor; }
	LPDIRECT3DDEVICE9		getGraphicsDevice()			{ return graphicsDevice; }
	D3DPRESENT_PARAMETERS	getPresentParameters()		{ return presentParameters; }
	LPD3DXSPRITE			getSpriteHandler()			{ return spriteHandler; }

	// METHODS DEFINED IN DirectXGraphics.cpp
	DirectXGraphics(Game *initGame);
	~DirectXGraphics();

	// OVERRIDDEN GameGraphics METHODS DEFINED IN DirectXGraphics.cpp
	bool			containsDisplayMode(vector<D3DDISPLAYMODE*> *displayModes, D3DFORMAT testColorFormat, int testScreenWidth, int testScreenHeight);
	TextureManager* createTextureManager();//wchar_t *initTexturesPath);
	void			findAlternativeDisplayMode(vector<D3DDISPLAYMODE*>* displayModes, D3DFORMAT &formatToSet);
	int				getScreenHeight();
	int				getScreenWidth();
	void			initGraphics(GameOS *os, bool isFullscreen);
	void			initTextFont(int fontSize);
	void			reloadGraphics();
	void			renderGame(Game *game);
	void			renderTextToDraw(TextToDraw *textToDraw);
	void			setColorKey(int r, int g, int b);
	void			setFontColor(int r, int g, int b);
	void			shutdown();

	void renderGridMarkers();
	void renderPathfindingCells();
	void renderPathfindingCell(AABB node, D3DCOLOR color);

	//Overridden b2Draw Methods
	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Set the drawing flags.
	void SetFlags(uint32 flags);

	/// Get the drawing flags.
	uint32 GetFlags() const;

	/// Append flags to the current flags.
	void AppendFlags(uint32 flags);

	/// Clear flags from the current flags.
	void ClearFlags(uint32 flags);

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf);

};