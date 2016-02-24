/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\ai\bots\RandomWanderingBot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	//PhysicalProperties *pp = sprite->getPhysicalProperties();
	b2Vec2 spriteXY = sprite->body->GetPosition();
	float rotation = sprite->getRotationInRadians();
	spriteXY.x -= 0.5f;
	spriteXY.y -= 0.5f;
	spriteXY.x *= 64.0f;
	spriteXY.y *= 64.0f;

	// IS THE SPRITE VIEWABLE?
	if (viewport->areWorldCoordinatesInViewport(	
									spriteXY.x,//pp->getX(),
									spriteXY.y,//pp->getY(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight()))
	{
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									spriteXY.x - viewport->getViewportX(),
									spriteXY.y - viewport->getViewportY(),
									sprite->getZ(),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight(),
									sprite->body->GetAngle());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(&player, renderList, viewport);


	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
	list<Bot*>::iterator botsIt = bots.begin();
	while (botsIt != bots.end())
	{
		list<Bot*>::iterator tempIt = botsIt;
		botsIt++;
		Bot *botToDelete = (*tempIt);

		delete botToDelete;
	}
	//bots.clear();
	
	// DESTROY THE PLAYER'S Box2D BODY
	if (player.body != nullptr && player.body->GetWorld()->IsLocked() == false){
		player.body->SetUserData(nullptr);
		player.body->GetWorld()->DestroyBody(player.body);
		player.body = nullptr;
	}

	vector<AnimatedSpriteType*>::iterator spriteTypesIt = spriteTypes.begin();
	while (spriteTypesIt != spriteTypes.end())
	{
		vector<AnimatedSpriteType*>::iterator tempIt = spriteTypesIt;
		spriteTypesIt++;
		AnimatedSpriteType *astToDelete = (*tempIt);
		delete astToDelete;
	}
	//spriteTypes.clear();

	clearSprites();

	// DELETE THE PATHFINDER IF THERE IS ONE
	/*
	if (pathfinder != nullptr || pathfinder != NULL){
		delete pathfinder;
	}
	*/	
}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	//return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT

	botsToRemove.push_back(botToRemove);
	numDeadBots++;
	return nullptr;
}

/*
	update - This method should be called once per frame. It
	goes through all of the sprites, including the player, and calls their
	update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	b2Vec2 newDest = b2Vec2_zero;

	// THEN UPDATE THE PLAYER SPRITE ANIMATION FRAME/STATE/ROTATION
	player.updateSprite();

	// NOW UPDATE THE REST OF THE SPRITES ANIMATION FRAMES/STATES/ROTATIONS
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		RandomWanderingBot* customer = dynamic_cast<RandomWanderingBot*>(bot);
		customer->think(game, customer, shouldMobPlayer(customer));
		bot->updateSprite();
		botIterator++;
	}
}

b2Vec2 SpriteManager::chooseRandomWorldCoordinate(Game* game){
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

	b2Vec2 destination = b2Vec2_zero;
	destination.x = x;
	destination.y = y;

	return destination;
}

float SpriteManager::generateRandomCoordinates(){

	int random = rand() % 45 + 3;
	int worldCoordinate = (random * 64);
	float returnValue = (float)(worldCoordinate / 64);
	return returnValue;
}

bool SpriteManager::checkSpawnTileIsCollidable(Game* game, float x, float y){
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

void SpriteManager::removeDeadBots(){

	for (Bot* deadBot : botsToRemove){

		bots.remove(deadBot);
		RandomWanderingBot* customer = dynamic_cast<RandomWanderingBot*>(deadBot);
		customer->~RandomWanderingBot();
	}

	botsToRemove.clear();
}

b2Vec2 SpriteManager::findExitTileCoordinates(Game* game){
	vector <WorldLayer*> *layers = game->getGSM()->getWorld()->getLayers();
	WorldLayer *layer = layers->front();
	TiledLayer *tileLayer = dynamic_cast<TiledLayer*>(layer);

	Tile *tileToCheck = nullptr;
	b2Vec2 exitLocation = b2Vec2_zero;

	float x, y;
	for (int j = 0; j <= 49; j++)
	{
		y = (float)j;
		for (int i = 0; i <= 49; i++)
		{
			x = (float)i;
			tileToCheck = tileLayer->getTile(y, x);
			if (tileToCheck->textureID == 20){

				exitLocation.x = y;
				exitLocation.y = x;
				return exitLocation;
			}
		}
	}
	
	return b2Vec2_zero;



}

bool SpriteManager::shouldMobPlayer(Bot* bot){

	RandomWanderingBot* customer = nullptr;
	if (bot->getSpriteType()->getSpriteTypeID() >= 1 && bot->getSpriteType()->getSpriteTypeID() <= 6){
		customer = dynamic_cast<RandomWanderingBot*>(bot);

		if (player.getNumCollisions() >= TOTAL_COLLISION_TOLERANCE){
			player.increaseNumFollowers();
			customer->setIsFollowingPlayer(true);
			return true;
		}
		
		// NORMAL CUSTOMER
		if (customer->getSpriteType()->getSpriteTypeID() == 1 || customer->getSpriteType()->getSpriteTypeID() == 2){
			if (customer->getNumCollisionsWithPlayer() >= NORMAL_CUSTOMER_TOLERANCE){
				if (customer->getIsFollowingPlayer() == false) {
					player.increaseNumFollowers();
					customer->setIsFollowingPlayer(true);
					return true;
				}
			}
		}

		// OLD CUSTOMER
		if (customer->getSpriteType()->getSpriteTypeID() == 3 || customer->getSpriteType()->getSpriteTypeID() == 4){
			if (customer->getNumCollisionsWithPlayer() >= OLD_CUSTOMER_TOLERANCE){
				if (customer->getIsFollowingPlayer() == false) {
					player.increaseNumFollowers();
					customer->setIsFollowingPlayer(true);
					return true;
				}
			}
		}

		// RUDE CUSTOMER
		if (customer->getSpriteType()->getSpriteTypeID() == 5 || customer->getSpriteType()->getSpriteTypeID() == 6){
			if (customer->getNumCollisionsWithPlayer() >= RUDE_CUSTOMER_TOLERANCE){
				if (customer->getIsFollowingPlayer() == false) {
					player.increaseNumFollowers();
					customer->setIsFollowingPlayer(true);
					return true;
				}
			}
		}
		
	}

	return false;
}