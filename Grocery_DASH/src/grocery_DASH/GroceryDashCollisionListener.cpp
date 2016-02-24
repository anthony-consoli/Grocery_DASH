#include "GroceryDashCollisionListener.h"
#include "sssf\game\Game.h"

/*
	Body Type <--> int conversion
		static body == 0
		kinematic body == 1
		dynamic body == 2
*/

/* handle begin event */
void GroceryDashCollisionListener::BeginContact(b2Contact* contact){
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	TopDownSprite *spriteA; Tile *tileA;
	TopDownSprite *spriteB; Tile *tileB;

	//Check FixtureA
	if (fixtureA->GetBody()->GetType() == 2){
		spriteA = (TopDownSprite*)fixtureA->GetBody()->GetUserData();
		tileA = nullptr;
	}
	else{
		tileA = (Tile*)fixtureA->GetBody()->GetUserData();
		spriteA = nullptr;
	}

	//Check FixtureB
	if (fixtureB->GetBody()->GetType() == 2){
		spriteB = (TopDownSprite*)fixtureB->GetBody()->GetUserData();
		tileB = nullptr;
	}
	else{
		tileB = (Tile*)fixtureB->GetBody()->GetUserData();
		spriteB = nullptr;
	}

	//Pass along to the proper helper method
	if (spriteA != nullptr && spriteB != nullptr){
		handleBeginContactEventSprites(contact, spriteA, spriteB);
	}
	else{
		if ( ( (tileA != nullptr) != (tileB != nullptr) ) && ( (spriteA != nullptr) != (spriteB != nullptr) ) ){

			if (tileA != nullptr && spriteA != nullptr){
				handleBeginContactEventTile(contact, spriteA, tileA);
			}

			if (tileA != nullptr && spriteB != nullptr){
				handleBeginContactEventTile(contact, spriteB, tileA);
			}

			if (tileB != nullptr && spriteA != nullptr){
				handleBeginContactEventTile(contact, spriteA, tileB);
			}

			if (tileB != nullptr && spriteB != nullptr){
				handleBeginContactEventTile(contact, spriteB, tileB);
			}
		}
		else{
			// NOT A COLLISON TO WORRY ABOUT
		}
	}



}

/* handle end event */
void GroceryDashCollisionListener::EndContact(b2Contact* contact){
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	TopDownSprite *spriteA; Tile *tileA;
	TopDownSprite *spriteB; Tile *tileB;

	//Check FixtureA
	if (fixtureA->GetBody()->GetType() == 2){
		spriteA = (TopDownSprite*)fixtureA->GetBody()->GetUserData();
		tileA = nullptr;
	}
	else{
		tileA = (Tile*)fixtureA->GetBody()->GetUserData();
		spriteA = nullptr;
	}

	//Check FixtureB
	if (fixtureB->GetBody()->GetType() == 2){
		spriteB = (TopDownSprite*)fixtureB->GetBody()->GetUserData();
		tileB = nullptr;
	}
	else{
		tileB = (Tile*)fixtureB->GetBody()->GetUserData();
		spriteB = nullptr;
	}

}

/* handle pre-solve event */
void GroceryDashCollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){

}

/* handle post-solve event */
void GroceryDashCollisionListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){
	

}


void GroceryDashCollisionListener::handleBeginContactEventTile(b2Contact* contact, TopDownSprite* sprite, Tile* tile){
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	int tileID = tile->textureID;

	if (sprite->getSpriteType()->getSpriteTypeID() == 0 || sprite->getSpriteType()->getSpriteTypeID() == 16){
		game->getGSM()->getAudio()->playAudio(1);
		if (tileID == 20 && dataLoader->getHasPaidCashier() == true){
			//game->getGSM()->setLevelClear(true);
			game->getGSM()->goToVictoryScreen();
		}
		else{
			if (tileID >= 14 && tileID <= 19 && dataLoader->hasPaidCashier == false){
				//int randomShelf = rand() % 10 + 1;
				if (tile->shelfNum != -1){
					game->getGSM()->goToShelfScreen(tile->shelfNum);
				}
			}
			else{
				if (tileID >= 80 && tileID <= 89 && dataLoader->hasPaidCashier == false){
					//int randomBin = rand() % 2 + 11;
					if (tile->shelfNum != -1){
						game->getGSM()->goToShelfScreen(tile->shelfNum);
					}
				}
				else{
					if (tileID == 61){
						if (dataLoader->getHasPaidCashier() == false && dataLoader->numberOfItems > 0 
							&& dataLoader->isShoppingListEmpty() == true && dataLoader->isSpendingLimitExceeded() == false){
							dataLoader->setIsPayingCashier(true);
						}
					}
					else{

					}
				}
			}
		}
	}
	else{
		if (tileID == 20){
			RandomWanderingBot* bot = dynamic_cast<RandomWanderingBot*>(sprite);
			game->getGSM()->getSpriteManager()->removeBot(bot);
		}
		else{
			// DO NOTHING
		}

	}

	b2Vec2 pV = sprite->body->GetLinearVelocity();
	b2Vec2 pP = sprite->body->GetPosition();

	if (pV.x != 0.0f)	{ pV.x *= -0.5f; }

	if (pV.y != 0.0f)	{ pV.y *= -0.5f; }

	sprite->body->SetLinearVelocity(pV);
	

}

void GroceryDashCollisionListener::handleBeginContactEventSprites(b2Contact* contact, TopDownSprite* spriteA, TopDownSprite* spriteB){
	GroceryDashDataLoader* dataLoader = dynamic_cast<GroceryDashDataLoader*>(game->getDataLoader());
	bool isPlayerInvolved = false;

	if (spriteA->getSpriteType()->getSpriteTypeID() == 0 || spriteA->getSpriteType()->getSpriteTypeID() == 16 || 
	  spriteB->getSpriteType()->getSpriteTypeID() == 0 || spriteB->getSpriteType()->getSpriteTypeID() == 16)
	{	// PLAYER IS INVOLVED IN THE COLLISION
		isPlayerInvolved = true;

		if (dataLoader->getIsPayingCashier() == true){
			// IGNORE THE COLLISION
			return;
		}

		// PLAY THE AUDIO
		game->getGSM()->getAudio()->playAudio(3);	

		// COLLISION COUNTER
		RandomWanderingBot* customer = determineCustomerSprite(spriteA, spriteB);
		TopDownSprite*player = determinePlayerSprite(spriteA, spriteB);

		if (dataLoader->getIsPayingCashier() == false && game->getGSM()->isPlayerAtShelf() == false){
			player->increaseNumCollisions();
			//customer->increaseNumCollisions();
			customer->increaseNumCollisionsWithPlayer();
		}
	}

	if (isPlayerInvolved == true && (dataLoader->getIsPayingCashier() == true || game->getGSM()->isPlayerAtShelf() == true)){
		//DO NOTHING.  THE PLAYER IS BUSY WITH SOMETHING ELSE.  
		return; //DISREGARD THE COLLISION!
	}

	b2Vec2 pV_A = spriteA->body->GetLinearVelocity();
	if (pV_A.x > 1.0f || pV_A.x < -1.0f)	{ pV_A.x *= -50.0f; }
	if (pV_A.y > 1.0f || pV_A.y < -1.0f)	{ pV_A.y *= -50.0f; }
	spriteA->body->ApplyForceToCenter(pV_A, true);

	b2Vec2 pV_B = spriteB->body->GetLinearVelocity();
	if (pV_B.x > 1.0f || pV_B.x < -1.0f)	{ pV_B.x *= -50.0f; }
	if (pV_B.y > 1.0f || pV_B.y < -1.0f)	{ pV_B.y *= -50.0f; }
	spriteB->body->ApplyForceToCenter(pV_B, true);
	

	//player == 0 or 16
	//cashier_m == 13
	int typeA = spriteA->getSpriteType()->getSpriteTypeID();
	int typeB = spriteB->getSpriteType()->getSpriteTypeID();

	if (((typeA == 0 ^ typeA == 16) && typeB == 13) ^ ((typeB == 0 ^ typeB == 16) && typeA == 13)){
		if (dataLoader->getHasPaidCashier() == false && dataLoader->numberOfItems > 0
			&& dataLoader->isShoppingListEmpty() == true && dataLoader->isSpendingLimitExceeded() == false){
			dataLoader->setIsPayingCashier(true);
		}
	}

}

RandomWanderingBot* GroceryDashCollisionListener::determineCustomerSprite(TopDownSprite* spriteA, TopDownSprite* spriteB){

	RandomWanderingBot *bot = nullptr;

	if (spriteA->getSpriteType()->getSpriteTypeID() != 0 && spriteA->getSpriteType()->getSpriteTypeID() != 16){
		bot = dynamic_cast<RandomWanderingBot*>(spriteA);
		return bot;
	}
	else{
		bot = dynamic_cast<RandomWanderingBot*>(spriteB);
		return bot;
	}
}

TopDownSprite* GroceryDashCollisionListener::determinePlayerSprite(TopDownSprite* spriteA, TopDownSprite* spriteB){

	RandomWanderingBot *bot = nullptr;

	if (spriteA->getSpriteType()->getSpriteTypeID() == 0 || spriteA->getSpriteType()->getSpriteTypeID() == 16){
		return spriteA;
	}
	else{
		return spriteB;
	}
}
