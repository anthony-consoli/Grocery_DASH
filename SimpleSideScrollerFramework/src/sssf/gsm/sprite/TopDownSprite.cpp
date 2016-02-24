#include "sssf_VS\stdafx.h"
#include "sssf\gsm\sprite\TopDownSprite.h"

// Box2D INCLUDE
#include <Box2D\Box2D.h>

void TopDownSprite::updateSprite()
{
	// CALL THE PARENT METHOD TO UPDATE ANIMATION
	AnimatedSprite::updateSprite();

	// AND NOW CALCULATE THE RADIANS
	// FOR RENDERING THIS SPRITE
	b2Vec2 vector = body->GetLinearVelocity();
	float vX = vector.x;
	float vY = vector.y;
	float epsilon = 0.00001f;

	// IF THE Y VELOCITY IS 0 WE HAVE TO BE CAREFUL DIVIDING
	if (fabs(vY) < epsilon)
	{
		// IF X ISN'T MOVING EITHER, LEAVE IT WHAT IT IS
		// AND PUT IT INTO AN IDLE STATE
		if (fabs(vX) > epsilon)
		{
			if (vX < 0.0f)
			{
				rotationInRadians = PI;
				body->SetTransform(body->GetPosition(), rotationInRadians);
				//body->ApplyAngularImpulse(rotationInRadians, true);
			}
			else{
				rotationInRadians = 0.0f;
				body->SetTransform(body->GetPosition(), rotationInRadians);
				//body->ApplyAngularImpulse(rotationInRadians, true);
			}
		}
	}
	else
	{
		float tanTheta = vY/vX;
		rotationInRadians = atan(tanTheta);
		if (vX < 0.0f)
		{
			if (vY < 0.0f){
				rotationInRadians += PI;
				body->SetTransform(body->GetPosition(), rotationInRadians);
				//body->ApplyAngularImpulse(rotationInRadians, true);
			}
			else{
				rotationInRadians -= PI;
				body->SetTransform(body->GetPosition(), rotationInRadians);
				//body->ApplyAngularImpulse(rotationInRadians, true);
			}
		}
	}

}