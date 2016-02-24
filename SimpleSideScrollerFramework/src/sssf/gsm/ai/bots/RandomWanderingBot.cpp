#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\RandomWanderingBot.h"
#include "sssf\gsm\state\GameStateManager.h"

/*******************************************
DEPRACATED CONSTRUCTOR FROM PARENT CLASS
**********************************************/
RandomWanderingBot::RandomWanderingBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity){

	initBot(initMin, initMax, initMaxVelocity);
	velocity.x = 0;
	velocity.y = 0;
	numCollisionsWithPlayer = 0;
	isFollowingPlayer = false;

}


RandomWanderingBot::RandomWanderingBot(unsigned int initMin, unsigned int initMax){

	initBot(initMin, initMax, 0);
	velocity.x = 0;
	velocity.y = 0;
	numCollisionsWithPlayer = 0;
	isFollowingPlayer = false;
}

RandomWanderingBot::~RandomWanderingBot()
{
	// CLEAN UP THE Box2D body, if not already done
	if (body != nullptr && body->GetWorld()->IsLocked() == false){
		body->SetUserData(nullptr);
		body->GetWorld()->DestroyBody(body);
		body = nullptr;
	}

}

Bot* RandomWanderingBot::clone(){
	
	RandomWanderingBot *botClone = new RandomWanderingBot(minCyclesBeforeThinking, maxCyclesBeforeThinking);

	return botClone;
}

/*
	initBot -> this initialization method sets up the basic bot
		properties, but does not setup its velocity.
*/
void RandomWanderingBot::initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity)
{
	// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
	if (initMax < initMin)
	{
		unsigned int temp = initMax;
		initMax = initMin;
		initMin = temp;
	}
	// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
	else if (initMax == initMin)
		initMax += 100;

	// INIT OUR RANGE VARIABLES
	minCyclesBeforeThinking = initMin;
	maxCyclesBeforeThinking = initMax;

	// **** This varaible is no longer used, but is maintained just in case.
			maxVelocity = initMaxVelocity;// AND OUR VELOCITY CAPPER
}

/*
pickRandomCyclesInRange - a randomized method for determining when this bot
will think again. This method sets that value.
*/
void RandomWanderingBot::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/* pickRandomVelocity - determines a random velocity for the bot and assigns the appropriate angle
of rotation for movement
*/
void RandomWanderingBot::pickRandomVelocity()
{
	// FIRST GET RANDOM int VALUES FROM 0 TO 9
	int x = rand() % 10 - 5;
	int y = rand() % 10 - 5;

	// CONVERT TO FLOAT
	float vX = (float)x;
	float vY = (float)y;

	// NOW WE CAN SCALE OUR X AND Y VELOCITIES
	int random = rand() % 3 + 1;
	float speedScale = (float)random;
	velocity.x = vX * speedScale;
	velocity.y = vY * speedScale;

	// REDUCE SPEED FOR OLD CUSTOMERS
	if (spriteType->getSpriteTypeID() == 3 || spriteType->getSpriteTypeID() == 4){
		velocity.x *= 0.5f;
		velocity.y *= 0.5f;
	}

	body->ApplyForceToCenter(velocity, true);

	// DETERMINE THE PROPER ROTATION OF THE SPRITE
	float bodyAngle = body->GetAngle();
	b2Vec2 toTarget = (body->GetPosition() + velocity) - body->GetPosition();
	float desiredAngle = atan2f(-toTarget.x, toTarget.y);

	body->SetTransform(body->GetPosition(), desiredAngle);

}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomWanderingBot::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	if (cyclesRemainingBeforeThinking == 0)
	{
		GameStateManager *gsm = game->getGSM();
		pickRandomCyclesInRange();
		if (currentState != L"STANDING"){	pickRandomVelocity(); }
	}
	else{
		cyclesRemainingBeforeThinking--;
	}
	body->ApplyForceToCenter(velocity, true);
}


void RandomWanderingBot::think(Game *game, Bot *bot, bool following)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN
	TopDownSprite *player = game->getGSM()->getSpriteManager()->getPlayer();
	RandomWanderingBot* customer = dynamic_cast<RandomWanderingBot*>(bot);
	GridPathfinder* pathfinder = game->getGSM()->getSpriteManager()->getPathfinder();

	if (cyclesRemainingBeforeThinking == 0)
	{
		GameStateManager *gsm = game->getGSM();
		pickRandomCyclesInRange();
		if (currentState != L"STANDING")
		{
			if (getCurrentPathToFollow()->empty() == true)
			{
				pickRandomVelocity();
			}
		}

		if (player->getNumCollisions() >= TOTAL_COLLISION_TOLERANCE){
			if (customer->getIsFollowingPlayer() == false){
				//player->increaseNumFollowers();
				customer->setIsFollowingPlayer(true);
			}
			pathfinder->mapPath(customer, player->getX(), player->getY());
		}
		
		if (customer->getIsFollowingPlayer() == false){
			game->getGSM()->getSpriteManager()->shouldMobPlayer(customer);
		}

		if (customer->getIsFollowingPlayer() == true){

			if (customer->getCurrentPathToFollow()->empty() == true){
				pathfinder->mapPath(customer, player->getX(), player->getY());
			}
			else{
				pathfinder->updatePath(customer);
			}

		}
		else{
			if (getCurrentPathToFollow()->empty() == true)
			{
				pickRandomVelocity();
			}
		}
		pathfinder->updatePath(customer);
		
	}
	else{
		cyclesRemainingBeforeThinking--;
	}
	pathfinder->updatePath(customer);
	body->ApplyForceToCenter(velocity, true);
}
