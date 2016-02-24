#pragma once

#include "sssf\gsm\sprite\AnimatedSprite.h"

class TopDownSprite : public AnimatedSprite
{
protected:
	// USED TO RENDER A ROTATED SPRITE, NOT INVOLVED IN PHYSICS
	float rotationInRadians;


private:
	int numCollisions;
	int numFollowers;
	bool allowPathfindingRecalculaton;


public:

	TopDownSprite(){ numCollisions = 0; numFollowers = 0; allowPathfindingRecalculaton = true; }

	float	getRotationInRadians()							{ return rotationInRadians; }
	void	setRotationInRadians(float initRotation)		{ rotationInRadians = initRotation; }

	int		getNumCollisions()								{ return numCollisions; }
	void	increaseNumCollisions()							{ numCollisions++; }
	void	resetNumCollisions()							{ numCollisions = 0; }

	int		getNumFollowers()								{ return numFollowers; }
	void	increaseNumFollowers()							{ numFollowers++; }
	void	resetNumFollowers()								{ numFollowers = 0; }

	bool	getAllowPathfindingRecalculation()				{ return allowPathfindingRecalculaton; }
	void	setAllowPathfindingRecalculation(bool b)		{ allowPathfindingRecalculaton = b; }

	
	void updateSprite();



	/*	TODO
			- chase player with increased number of collisions
	
	*/
	
};