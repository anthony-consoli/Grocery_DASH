/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once

// Box2D INCLUDE
#include <Box2D\Box2D.h>

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\pathfinding\GridPathfinder.h"
#include "sssf\gsm\physics\CollidableObject.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gui\Viewport.h"

class AnimatedSprite : public CollidableObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	// CURRENT PATH TO FOLLOW
	list<PathNode> currentPathToFollow;
	list<PathNode>::iterator currentPathNode;

	// SET THE Z-VALUE FOR RENDERING
	int z;

public:
	// This variable must be public to allow proper access to it.
	// **see GroceryDashDataLoader.cpp for primary use**
	b2Body *body;

	// INLINED ACCESSOR METHODS
	//b2Body			getB2Body()							{ return *body; }
	void				setB2Body(b2Body* newBody)			{ body = newBody; }

	float				getX()								{ return body->GetPosition().x; }
	float				getY()								{ return body->GetPosition().y; }

	int					getZ()								{ return z; }
	void				setZ(int newZ)						{ z = newZ; }

	int					getAlpha()							{ return alpha;				}
	list<PathNode>*		getCurrentPathToFollow()			{ return &currentPathToFollow; }
	list<PathNode>::iterator getCurrentPathNode()			{ return currentPathNode; }
	wstring				getCurrentState()					{ return currentState;		}
	unsigned int		getFrameIndex()						{ return frameIndex;		}
	AnimatedSpriteType*	getSpriteType()						{ return spriteType;		}
	bool hasReachedDestination()							{	return currentPathNode == currentPathToFollow.end(); }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void advanceCurrentPathNode()
	{	currentPathNode++; }
	void resetCurrentPathNode()
	{	currentPathNode = currentPathToFollow.begin(); }
	void clearPath()
	{	currentPathToFollow.clear();
		currentPathNode = currentPathToFollow.end(); 
	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	virtual void updateSprite();
	void affixTightAABBBoundingVolume();
	void correctToTightBoundingVolume();
	virtual float getRotationInRadians() { return 0.0f; }
};