/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXGraphics.cpp

	See DirectXGraphics.h for a class description.
*/

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\graphics\TextureManager.h"
#include "sssf\gsm\ai\pathfinding\GridPathfinder.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\Viewport.h"
#include "sssf\os\GameOS.h"
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\text\GameText.h"

/*
	DirectXGraphics - Default constructor, it doesn't initialize anything.
	To setup all the DirectX objects call initGraphics after construction.
*/
DirectXGraphics::DirectXGraphics(Game *initGame)
{
	game = initGame;
	
}

/*
	~DirectXGraphics - Destructor, it destroys all of the DirectX pointers.
	This would be called only when the application closes, unless someone 
	decides to use different rendering technologies during the game.
*/
DirectXGraphics::~DirectXGraphics()
{
	delete d3d;
	delete graphicsDevice;
	delete spriteHandler;
	delete textFont;
}

/*
	containsDisplayMode - This method looks at the vector of display modes that
	were presumably retrieved from the GPU, and tests to see if our desired
	color format and screen resolution are inside. If found, true is returned,
	otherwise false.
*/
bool DirectXGraphics::containsDisplayMode(vector<D3DDISPLAYMODE*> *displayModes,
										  D3DFORMAT testColorFormat,
										  int testScreenWidth,
										  int testScreenHeight)
{
	vector<D3DDISPLAYMODE*>::iterator iterator;

	// START WITH THE FIRST ELEMENT
	iterator = displayModes->begin();

	// LOOK THROUGH THE WHOLE VECTOR
	while (iterator != displayModes->end())
	{
		// GET THE CURRENT MODE
		D3DDISPLAYMODE *testMode = (*iterator);

		// IS IT THE ONE WE'RE LOOKING FOR?
		if ((testMode->Format == testColorFormat)
			&& (testMode->Width == testScreenWidth)
			&& (testMode->Height == testScreenHeight))
			return true;

		// GO ONTO THE NEXT ONE
		iterator++;
	}
	return false;
}

/*
	createDirectXDeviceAndSpriteHandler - THIS METHOD CREATES OUR GPU AND
	SPRITE HANDLER (used for batch rendering textures) USING THE COLOR
	FORMAT AND SCREEN RESOLUTION OF OUR CHOICE.
*/
HRESULT DirectXGraphics::createDirectXDeviceAndSpriteHandler()
{
	HRESULT result;
	GameText *text = game->getText();

	// CREATE OUR GPU
    result = d3d->CreateDevice(D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						presentParameters.hDeviceWindow,
						D3DCREATE_HARDWARE_VERTEXPROCESSING,
						&presentParameters,
						&graphicsDevice);
	
	// IF GPU CREATION WAS SUCCESSFUL
	if (SUCCEEDED(result))
	{
		text->writeDebugOutput("SUCCEEDED");

		text->writeDebugOutput("\nD3DXCreateSprite(): ");

		// CREATE OUR SPRITE HANDLER
	    result = D3DXCreateSprite(graphicsDevice, &spriteHandler);
		if (SUCCEEDED(result))
		{
			text->writeDebugOutput("SUCCEEDED");
		}
		else
			text->writeDebugOutput("FAILED");

		this->initTextFont(20);
	}
	else
		text->writeDebugOutput("FAILED");

	return result;
}

/*
	createTextureManager - This method constructs a technology-specific
	TextureManager. Since this DirectXGraphics class uses the DirectX
	library, this method creates a DirectXTextureManager.
*/
TextureManager* DirectXGraphics::createTextureManager()
{
	TextureManager *textureManager = (TextureManager*)(new DirectXTextureManager());
	textureManager->setGraphics(this);
	return textureManager;
}

/*
	endDirectXFrameRendering - This method should be called after rendering a frame
	so that we can display what we've drawn on the GPU onto the monitor. It also
	releases a lock on the GPU so other threads may use it.
*/
void DirectXGraphics::endDirectXFrameRendering()
{
	// ALL DONE DRAWING ONTO THE GPU FOR THIS FRAME
    if (FAILED(graphicsDevice->EndScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->EndScene(): FAILED");

	// PUT WHAT WE JUST RENDERED ONTO THE SCREEN
	if (FAILED(graphicsDevice->Present(NULL, NULL, NULL, NULL)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Present(): FAILED");
}

/*
	findAlternativeDisplayMode - If the player's GPU doesn't have the
	display mode we want this method can pick a new one. It does so
	by first seeing if there is another display mode with the resolution
	we want but a different color model. If found, we'll use it. If not it
	simply picks the largest one it can find. This method uses call-by-reference
	to set the formatToSet, screenWidthToSet, & screenHeightToSet parameters
	using the chosen display mode parameters.
*/
void DirectXGraphics::findAlternativeDisplayMode(	vector<D3DDISPLAYMODE*> *displayModes,
													D3DFORMAT &formatToSet)
{
	// FIRST FIND ONE WITH THE PREFERRED SCREEN
	// DIMENSIONS, DEFAULT_SCREEN_HEIGHT &
	// DEFAULT_SCREEN_WIDTH, SINCE CHANGING A GUI TO
	// FIT DIFFERENT RESOLUTIONS IS PAINFUL
	bool defaultScreenSizeFound = false;
	vector<D3DDISPLAYMODE*>::iterator iterator;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE *testMode = (*iterator);
		if ((testMode->Width == screenWidth)
			&& (testMode->Height == screenHeight))
		{
			if (testMode->Format == DEFAULT_COLOR_FORMAT)
			{
				formatToSet = DEFAULT_COLOR_FORMAT;
				return;
			}
			else
			{
				defaultScreenSizeFound = true;
				formatToSet = testMode->Format;
			}
		}
		iterator++;
	}

	if (defaultScreenSizeFound)
		return;

	// NONE WERE FOUND IN THE SCREEN SIZE WE WANT, SO
	// NOW JUST FIND THE LARGEST RESOLUTION AVAILABLE
	int totalPixels = 0;
	int maxPixels = 0;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE *testMode = (*iterator);
		totalPixels = testMode->Width * testMode->Height;
		if (totalPixels > maxPixels)
		{
			formatToSet = testMode->Format;
			screenWidth = testMode->Width;
			screenHeight = testMode->Height;
			maxPixels = totalPixels;
		}
		iterator++;
	}
}

/*
	getDirectXDisplayModes - This method queries the GPU and gets a vector
	of all the display modes available, returning this data structure.
*/
vector<D3DDISPLAYMODE*>* DirectXGraphics::getDirectXDisplayModes()
{
	// WE'LL FILL THIS WITH THE DISPLAY MODES WE FIND
	vector<D3DDISPLAYMODE*> *displayOptions = new vector<D3DDISPLAYMODE*>();

	// WE'LL LOOK AT EACH COLOR MODEL, STARTING WITH ONE AT INDEX 1, IF THERE IS ONE
	int adapterCounter = 1;
	D3DFORMAT format;
	while (adapterCounter < 1000)
	{
		format = D3DFORMAT(adapterCounter);

		// HOW MANY MODES HAVE THIS COLOR MODEL?
		int numAdapters = d3d->GetAdapterModeCount(
										D3DADAPTER_DEFAULT,
										format);
	
		// GET ALL FOR THIS COLOR MODEL
		D3DDISPLAYMODE *displayModes = new D3DDISPLAYMODE[numAdapters];
		for (int i = 0; i < numAdapters; i++)
		{
			d3d->EnumAdapterModes(	D3DADAPTER_DEFAULT,
									format,
									i,
									&displayModes[i]);

			// PUT THEM INTO OUR VECTOR
			displayOptions->push_back(&displayModes[i]);
		}
		adapterCounter++;
	}
	return displayOptions;
}

/*
	getScreenHeight - This method gets the screen height being used for rendering.
*/
int DirectXGraphics::getScreenHeight()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Height;
	*/
	return screenHeight;
}

/*
	getScreenWidth - This method gets the screen width being used for rendering.
*/
int DirectXGraphics::getScreenWidth()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Width;
	*/
	return screenWidth;
}

/*
	init - This is the entry point for the application setting up the
	DirectX objects. It will get all available display modes and pick one,
	then use it to make a GPU device. Once this method is called, rendering
	can begin. It only needs to be called once at the start of the application.
	Even if we lose the graphics card (ALT-TAB), we don't have to re-init. We
	would have to re-init if we wished to change from fullscreen mode to 
	windowed mode, or if we want to change the screen resolution or color model.
*/
void DirectXGraphics::initGraphics(GameOS *os, bool isFullscreen)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	// WHAT ARE THE CAPABILITIES OF THE PLAYER'S GPU?
	vector<D3DDISPLAYMODE*> *displayOptions = getDirectXDisplayModes();

	// THESE WILL MAKE UP OUR DESIRED DISPLAY MODE
	D3DFORMAT formatToUse;

	// DOES THE PLAYER HAVE OUR DESIRED FORMAT?
	if (containsDisplayMode(displayOptions,
							DEFAULT_COLOR_FORMAT,
							screenWidth,
							screenHeight))
	{
		// THE GPU HAS OUR DESIRED FORMAT
		formatToUse = DEFAULT_COLOR_FORMAT;
	}
	else
	{
		// THE GPU DOESN'T HAVE OUR DESIRED FORMAT, WE NEED TO PICK ANOTHER ONE
		findAlternativeDisplayMode(	displayOptions,
									formatToUse);
	}

	// WE NEED TO FILL OUT A D3DPRESENT_PARAMETERS STRUCTURE WITH OUR
	// PREFERENCES FOR CREATING OUR GPU DEVICE

	// FIRST MAKE SURE OUR STRUCTURE IS EMPTY
    ZeroMemory(&presentParameters, sizeof(presentParameters));

	// WINDOWED MODE OR FULLSCREEN?
    presentParameters.Windowed = !isFullscreen;

	// DISCARD OLD FRAMES
	presentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;// D3DSWAPEFFECT_DISCARD;

	// THE WINDOW HANDLE
	HWND hWnd;

	// MAKE SURE WE'RE USING WINDOWS
	if (dynamic_cast<WindowsOS*> (os))
	{
		hWnd = ((WindowsOS*)(os))->getWindowHandle();
		presentParameters.hDeviceWindow = hWnd;
	}
	// I SUPPOSE WE COULD DIRECTX ON ANOTHER PLATFORM
	// LIKE XBox OR WINDOWS MOBILE, SO THOSE TESTS
	// AND INITIALIZATION WOULD HAVE TO GO HERE

	// THE DISPLAY MODE WE WILL BE USING
    presentParameters.BackBufferFormat	= formatToUse;
    presentParameters.BackBufferWidth	= screenWidth;
    presentParameters.BackBufferHeight	= screenHeight;

	// OK, NOW WE CAN MAKE OUR GPU & SPRITE HANDLER.
	createDirectXDeviceAndSpriteHandler();
	D3DXCreateLine(graphicsDevice, &lineSprite);
}

/*
	initTextFont - This method will initialize our font object, which
	we need to do all text rendering. It only needs to be done at the
	start of the application unless we want to change the font we
	are using.
*/
void DirectXGraphics::initTextFont(int fontSize)
{
	HRESULT result = D3DXCreateFont(
		graphicsDevice,				// OUR GPU
		fontSize,					// EG FONT SIZE FOR HEIGHT
		0,							// 0 FOR FONT WIDTH, USE DEFAULT ADJUST BASED ON HEIGHT
		FW_BOLD,					// FONT WEIGHT
		0,							// MIP LEVELS
		FALSE,						// ITALICIZED?
		DEFAULT_CHARSET,			// CHARACTER SET
		OUT_DEFAULT_PRECIS,			// RENDERING PRECISION
		DEFAULT_QUALITY,			// RENDERING QUALITY 
		DEFAULT_PITCH | FF_MODERN,	// FONT FAMILY NAME
		TEXT(""),					// FONT FACE NAME
		&textFont );				// THE FONT WE ARE CREATING
}

/*
	reloadGraphics - This method recreates the GPU and sprite handler and
	then reloads all the textures in the current texture managers. This would
	be called after regaining the GPU.
*/
void DirectXGraphics::reloadGraphics()
{
	createDirectXDeviceAndSpriteHandler();
	initTextFont(20);
	guiTextureManager->reloadAllTextures();
	worldTextureManager->reloadAllTextures();
}

/*
	renderDirectXRenderList - This method renders a render list of game
	elements to the screen. It can process render lists for the game
	world or the gui. Note that GUI render lists use screen coordinates
	and so don't have to be offset, but game world lists use world
	coordinates, and so they will need to be offset.
*/
void DirectXGraphics::renderGUIRenderList()
{
	guiRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;

	// GO THROUGH EACH ITEM IN THE LIST
	while (guiRenderList->hasNext())
	{
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = guiRenderList->next();

		// LET'S GET THE TEXTURE WE WANT TO RENDER
		
		int id = itemToRender.id;
		if (id >= 0){
			texture = ((DirectXTextureManager*)guiTextureManager)->getTexture(id);
			D3DXVECTOR3 position = D3DXVECTOR3((FLOAT)(itemToRender.x),
				(FLOAT)(itemToRender.y),
				0);


			// RENDER THE OPAQUE ITEMS
			if (itemToRender.a == 255)
			{
				if (FAILED(spriteHandler->Draw(
					texture,
					rect,
					NULL,
					&position,
					DEFAULT_ALPHA_COLOR)))
				{
					game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
				}
			}

			// RENDER THE ITEMS WITH EG TRANSPARENCY
			else
			{
				if (itemToRender.a < 0)
					itemToRender.a = 0;
				else if (itemToRender.a > 255)
					itemToRender.a = 255;

				if (FAILED(spriteHandler->Draw(
					texture,
					rect,
					NULL,
					&position,
					D3DCOLOR_ARGB(itemToRender.a, 255, 255, 255))))
				{
					game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
				}
			}
		}
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	guiRenderList->clear();
	if (rect != NULL)
		delete rect;
}

/*
	Renders all tiles and sprites. Note that these objects can
	be rotated.
*/
void DirectXGraphics::renderWorldRenderList()
{
	worldRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;
	GameGUI *gui = game->getGUI();
	Viewport *viewport = gui->getViewport();

	// GO THROUGH EACH ITEM IN THE LIST
	while (worldRenderList->hasNext())
	{
		float translationX;
		float translationY;
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = worldRenderList->next();
		if (itemToRender.id == 215){
			int x = 2;
		}
		
		// LET'S GET THE TEXTURE WE WANT TO RENDER
		int id = itemToRender.id;
		if (id >= 0)
		{
			if (id > worldRenderList->getSize()){ id = 111; } //# 111 --> BAKERY EMPLOYEE (not used)
			texture = ((DirectXTextureManager*)worldTextureManager)->getTexture(id);
			D3DXVECTOR3 position = D3DXVECTOR3(	(FLOAT)(itemToRender.x),
											(FLOAT)(itemToRender.y),
												0);

			position.x += viewport->getViewportOffsetX();
			position.y += viewport->getViewportOffsetY();

			// ADJUST FOR THE GUI OFFSET
			if ((position.x < viewport->getViewportOffsetX())
				||  (position.y < viewport->getViewportOffsetY()))
			{
				IDirect3DSurface9 *surface;
				UINT level = 0;
				HRESULT result = texture->GetSurfaceLevel(level, &surface);
				D3DSURFACE_DESC surfaceDescription;
				surface->GetDesc(&surfaceDescription);
				rect = new RECT();
				rect->left = 0;
				rect->top = 0;
				rect->right = surfaceDescription.Width;
				rect->bottom = surfaceDescription.Height;
				if (position.x < viewport->getViewportOffsetX())
				{
					int xDiff = viewport->getViewportOffsetX() - (int)position.x;
					rect->left = xDiff;
					position.x += xDiff;
				}
				if (position.y < viewport->getViewportOffsetY())
				{
					int yDiff = viewport->getViewportOffsetY() - (int)position.y;
					rect->top = yDiff;
					position.y += yDiff;
				}	
			}			

			// LET'S PUT THE STANDARD ROTATION MATRIX ASIDE
			// FOR A SECOND. IT WILL BE USED FOR RENDERING THE
			// GUI, BUT WE'LL WANT A CUSTOM ONE FOR WORLD OBJECTS
			D3DXMATRIX defaultTransform;
			D3DXMatrixIdentity(&defaultTransform);
				
			// TO RENDER A PROPERLY ROTATED OBJECT TO THE WORLD,
			// FIRST WE NEED TO MOVE IT TO THE ORIGIN, CENTERED
			// ABOUT THE ORIGIN SO WE SET UP THIS MATRIX
			// TO DO THIS
			D3DXMATRIX translationToOrigin;
			D3DXMatrixIdentity(&translationToOrigin);
		    translationToOrigin._41 = -(itemToRender.width/2);
			translationToOrigin._42 = -(itemToRender.height/2);
	
			// THEN WE NEED A MATRIX TO DO THE ROTATION
			D3DXMATRIX rotationAboutOrigin;
			D3DXMatrixIdentity(&rotationAboutOrigin);
	
			// THE PROBLEM ANGLES ARE 0, 90, 180, and 270
			float cosTheta = cos(itemToRender.rotationInRadians);
			float sinTheta = sin(itemToRender.rotationInRadians);
			if (cosTheta != cosTheta)
				cosTheta = 0;
			if (sinTheta != sinTheta)
				sinTheta = 0;
			rotationAboutOrigin._11 = cosTheta;
			rotationAboutOrigin._21 = -sinTheta;
			rotationAboutOrigin._12 = sinTheta;
			rotationAboutOrigin._22 = cosTheta;
	
			// AND THEN WE NEED A MATRIX TO ROTATE THE OBJECT
			// TO THE LOCATION WE WANT IT RENDERED
			D3DXMATRIX translationBackToCenter;
			D3DXMatrixIdentity(&translationBackToCenter);
			translationBackToCenter._41 = ((position.x) + (itemToRender.width/2));
			translationBackToCenter._42 = ((position.y) + (itemToRender.height/2));
	
			// THE COMBINED MATRIX COMBINES THESE 3 OPERATIONS
			// INTO A SINGLE MATRIX
			D3DXMATRIX combinedMatrix = translationToOrigin;
			combinedMatrix *= rotationAboutOrigin;
			combinedMatrix *= translationBackToCenter;
	
			// NOW LET'S USE THE COMBINED MATRIX TO POSITION AND ROTATE THE ITEM
			spriteHandler->SetTransform(&combinedMatrix);
		
			// RENDER THE OPAQUE ITEMS
			if (itemToRender.a == 255)
			{
				if (FAILED(spriteHandler->Draw(
					texture, 
					rect,
			        NULL,
					NULL,
					DEFAULT_ALPHA_COLOR)))
				{
					game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
				}
				// RENDER THE ITEMS WITH CUSTOM TRANSPARENCY
				else
				{
					if (itemToRender.a < 0)
						itemToRender.a = 0;
					else if (itemToRender.a > 255)
						itemToRender.a = 255;
					if (FAILED(spriteHandler->Draw(
						texture,
						rect,
						NULL,
						NULL,
						D3DCOLOR_ARGB(itemToRender.a, 255, 255, 255))))
					{
						game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
					}
				}
			}
		}
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	worldRenderList->clear();
	if (rect != NULL)
		delete rect;
	
	// AND RESTORE THE MATRIX USED FOR RENDERING THE GUI
	D3DXMATRIX identityMatrix;
	D3DXMatrixIdentity(&identityMatrix);
	spriteHandler->SetTransform(&identityMatrix);
}


void DirectXGraphics::renderGame(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	GameText *text = game->getText();

	// CHECK TO SEE IF WE STILL HAVE THE GPU
	HRESULT result = graphicsDevice->TestCooperativeLevel();

	// IF WE HAVE THE GPU, RENDER THE GAME
	if (SUCCEEDED(result)) 
	{
		// NOW PREPARE TO RENDER THE LISTS USING
		// BATCH TEXTURE RENDERING
		startDirectXFrameRendering();	
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// RENDER THE WORLD RENDER LIST
		renderWorldRenderList();
				
		// RENDER THE GUI RENDER LIST
		renderGUIRenderList();

		// RENDER THE TEXT
		renderText(text);

		// RENDER THE IN-GAME TEXT
		renderInGameText(game, text);

		// RENDER THE SETTINGS SCREEN TEXT
		renderSettingsScreenText(game, text);

		// RENDER THHE ITEM LIST TO THE SCREEN
		renderInGameItemList(game, text);

		// RENDER THE SETTINGS SCREEN TEXT
		renderStatsScreenText(game, text);

		// WRAP UP RENDERING RESOURCES
		if (FAILED(spriteHandler->End()))
		{
			text->writeDebugOutput("\nspriteHandler->End(): FAILED");
		}

		// RENDER THE Box2D DEBUG DATA
		renderB2Draw(game);

		// THIS HELPS US WITH SOME DEBUGGING
		renderPathfindingCells();
		renderGridMarkers();

		endDirectXFrameRendering();

	}

	// WE'VE LOST THE GPU, SLEEP UNTIL WE GET IT BACK
	else if (result == D3DERR_DEVICELOST) 
	{ 
		spriteHandler->OnLostDevice();
		textFont->OnLostDevice();
		Sleep(100); 
	}

	// WE'VE GOT IT BACK, RELOAD EVERYTHING. NOTE THAT
	// WE'LL ONLY GET THIS MESSAGE ONCE.
	else if (result == D3DERR_DEVICENOTRESET)
	{ 
		if (FAILED(graphicsDevice->Reset(&presentParameters)))
		{
			game->getText()->writeDebugOutput("\ngraphicsDevice->Reset: FAILED - Reloading GPU images");
			game->reloadAllDevices();
		}
		else
		{
			spriteHandler->OnResetDevice();
			textFont->OnResetDevice();
		}
	}
}

void DirectXGraphics::renderPathfindingCells()
{
	if (game->getGSM()->isGameInProgress() && pathfindingPathShouldBeRendered)
	{
		// RENDER THE CELL THE SPRITE IS ON
		GridPathfinder *pathfinder = game->getGSM()->getSpriteManager()->getPathfinder();
		list<PathNode> *path = game->getGSM()->getSpriteManager()->getPlayer()->getCurrentPathToFollow();
		list<PathNode>::iterator pathIt = path->begin();
		while (pathIt != path->end())
		{
			// 3 TYPES OF CELLS TO RENDER
			PathNode node = *pathIt;
			AABB nodeAABB;
			nodeAABB.setCenterX(pathfinder->getColumnCenterX(node.column));
			nodeAABB.setCenterY(pathfinder->getRowCenterY(node.row));
			nodeAABB.setWidth(pathfinder->getGridWidth());
			nodeAABB.setHeight(pathfinder->getGridHeight());

			// THE START CELL
			if (pathIt == path->begin())
			{
				renderPathfindingCell(nodeAABB, ORIGIN_PATHFINDING_CELL_COLOR);
			}
			else
			{
				list<PathNode>::iterator temp = pathIt;
				temp++;
				// THE END CELL
				if (temp == path->end())
				{
					renderPathfindingCell(nodeAABB, DESTINATION_PATHFINDING_CELL_COLOR);
				}
				// AND THE MIDDLE CELLS
				else
				{
					renderPathfindingCell(nodeAABB, DEFAULT_PATHFINDING_CELL_COLOR);
				}
			}
			pathIt++;
		}
	}
}

void DirectXGraphics::renderPathfindingCell(AABB node, D3DCOLOR color)
{
	Viewport *viewport = game->getGUI()->getViewport();
	RECT rect;
	rect.left = node.getLeft() - viewport->getViewportX() + viewport->getViewportOffsetX();
	rect.right = node.getRight() - viewport->getViewportX() + viewport->getViewportOffsetX();
	rect.top = node.getTop() - viewport->getViewportY() + viewport->getViewportOffsetY();
	rect.bottom = node.getBottom() - viewport->getViewportY() + viewport->getViewportOffsetY();
	LPDIRECT3DSURFACE9 backBufferSurface;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface);
	graphicsDevice->ColorFill(backBufferSurface, &rect, color);
}
	
void DirectXGraphics::renderGridMarkers()
{
	if (game->getGSM()->isGameInProgress() && pathfindingGridShouldBeRendered)
	{
		GridPathfinder *pathfinder = game->getGSM()->getSpriteManager()->getPathfinder();
		int cols = pathfinder->getNumColumns();
		int rows = pathfinder->getNumRows();
		Viewport *viewport = game->getGUI()->getViewport();
		int viewportX = viewport->getViewportX();
		int gridCellWidth = pathfinder->getGridWidth();
		int viewportY = viewport->getViewportY();
		int gridCellHeight = pathfinder->getGridHeight();
		int viewportWidth = viewport->getViewportWidth();
		int viewportHeight = viewport->getViewportHeight();

		int startCol = (viewportX/gridCellHeight)-1;
		if (startCol < 0)
			startCol = 0;
		int endCol = ((viewportX + viewportWidth)/gridCellWidth);
		if (endCol > pathfinder->getNumColumns())
			endCol = pathfinder->getNumColumns() - 1;
		int startRow = (viewportY/gridCellHeight);
		if (startRow < 0)
			startRow = 0;
		int endRow = ((viewportY + viewportHeight)/gridCellHeight);
		if (endRow > pathfinder->getNumRows())
			endRow = pathfinder->getNumRows() - 1;

		for (int i = startCol; i <= endCol; i++)
		{
			for (int j = startRow; j <= endRow; j++)
			{
				RECT rect;
				rect.left = (i * gridCellWidth) - viewportX + viewport->getViewportOffsetX();
				rect.right = rect.left + gridCellWidth;
				rect.top = (j * gridCellHeight) - viewportY + viewport->getViewportOffsetY();
				rect.bottom = rect.top + gridCellHeight;
				LPDIRECT3DSURFACE9 backBufferSurface;
				D3DCOLOR colorToUse;
				if (pathfinder->isWalkable(i, j))
					colorToUse = WALKABLE_TILE_COLOR;
				else
					colorToUse = NONWALKABLE_TILE_COLOR;
				graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBufferSurface);
				graphicsDevice->ColorFill(backBufferSurface, &rect, colorToUse);
			}
		}
	}
}



/*
	renderTextToDraw - This method renders a single piece of
	text to the screen using our EG font.
*/
void DirectXGraphics::renderTextToDraw(TextToDraw *textToDraw)
{
	textRect.left = textToDraw->x;
	textRect.right = textToDraw->x + textToDraw->width;
	textRect.top = textToDraw->y;
	textRect.bottom = textToDraw->y + textToDraw->height;
	LPCWSTR lpcwstrText = (*textToDraw->getText()).c_str();
	if (FAILED(textFont->DrawText (
				spriteHandler, 
				lpcwstrText, 
				-1, 
				&textRect, 
				DT_LEFT, 
				fontColor )))
		game->getText()->writeDebugOutput("\ntextFont->DrawText: FAILED");
}

/*
	setColorKey - This sets the color key to be used for loading images.
*/
void DirectXGraphics::setColorKey(int r, int g, int b)
{
	colorKey = D3DCOLOR_XRGB(r, g, b);
}

/*
	setFontColor - This sets the color to be used for rendering text.
*/
void DirectXGraphics::setFontColor(int r, int g, int b)
{
	fontColor = D3DCOLOR_XRGB(r, g, b);
}

/*
	shutdownGraphics - This method releases the DirectX objects we've created
	so that other applications can use the GPU. This should only be called
	when the application is closing.
*/
void DirectXGraphics::shutdown()
{
	if (textFont)
	{
		textFont->Release();
		textFont = NULL;
	}

	if (spriteHandler)
	{
		spriteHandler->Release();
		spriteHandler = NULL;
	}

	if (graphicsDevice)
	{
		graphicsDevice->Release();
		graphicsDevice = NULL;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = NULL;
	}}

/*
	startDirectXFrameRendering - This does some setup for rendering, like locking
	the GPU. Only one thread at a time can have a lock on the GPU.
*/
void DirectXGraphics::startDirectXFrameRendering()
{
	// CLEAR OUT ALL THE OLD RENDERING
    if (FAILED(graphicsDevice->Clear(0, NULL, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0f, 0)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Clear(): FAILED");

	// ALLOWS DRAWING TO START, LOCKING THE GPU
    if (FAILED(graphicsDevice->BeginScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->BeginScene(): FAILED");
}

/// Set the drawing flags.
void DirectXGraphics::SetFlags(uint32 flags)
{

}

/// Get the drawing flags.
uint32 DirectXGraphics::GetFlags() const
{
	return 0;
}

/// Append flags to the current flags.
void DirectXGraphics::AppendFlags(uint32 flags)
{

}

/// Clear flags from the current flags.
void DirectXGraphics::ClearFlags(uint32 flags)
{

}

void DirectXGraphics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	D3DXVECTOR2 points[2];
	
	points[0].x = (p1.x * 64) - game->getGUI()->getViewport()->getViewportX();
	points[0].y = (p1.y * 64) - game->getGUI()->getViewport()->getViewportY() + game->getGUI()->getViewport()->getViewportOffsetY();
	points[1].x = (p2.x * 64) - game->getGUI()->getViewport()->getViewportX();
	points[1].y = (p2.y * 64) - game->getGUI()->getViewport()->getViewportY() + game->getGUI()->getViewport()->getViewportOffsetY();
	
	lineSprite->Begin();
	lineSprite->Draw(points, 2, D3DCOLOR_ARGB(255, (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255)) );
	lineSprite->End();

}

/// Draw a closed polygon provided in CCW order.
void DirectXGraphics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	//THIS METHOD IS CALLED WHEN DRAWING AABB SHAPES
	for (int i = 0; i < vertexCount; i++){

		if (i + 1 == vertexCount){
			DrawSegment(vertices[i], vertices[0], color);
		}
		else{
			DrawSegment(vertices[i], vertices[i + 1], color);
		}
	}
}

/// Draw a solid closed polygon provided in CCW order.
void DirectXGraphics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DrawPolygon(vertices, vertexCount, color);
}

/// Draw a circle.
void DirectXGraphics::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{

}

/// Draw a solid circle.
void DirectXGraphics::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{//this one is called by the b2Draw method

	b2Vec2 vLine1, vLine2;
	b2Vec2 hLine1, hLine2;

	vLine1.x = vLine2.x = center.x;
	vLine1.y = center.y - radius;
	vLine2.y = center.y + radius;

	hLine1.y = hLine2.y = center.y;
	hLine1.x = center.x - radius;
	hLine2.x = center.x + radius;

	DrawSegment(vLine1, vLine2, color);
	DrawSegment(hLine1, hLine2, color);

}

/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void DirectXGraphics::DrawTransform(const b2Transform& xf)
{

}