#include "CollisionListener.h"
#include <Interface/iShape.h>
#include <Interface/iRigidBody.h>
#include "GDPPhysics/gdp/RigidBody.h"
#include <iostream>
namespace physics {
	CollisionListener::CollisionListener() : iCollisionListener()
	{

	}
	CollisionListener::~CollisionListener() 
	{

	}
	bool CollisionListener::NotifyCollision(iCollisionBody* bodyA, iCollisionBody* bodyB)
	{
		if (bodyA->GetBodyType() == BodyType::RigidBody)
		{
			if (bodyB->GetBodyType() == BodyType::RigidBody)
			{
				RigidBody* rigidA = RigidBody::Cast(bodyA);
				RigidBody* rigidB = RigidBody::Cast(bodyB);
				iShape* shapeB = rigidA->GetShape();
				iShape* shapeA = rigidB->GetShape();
				if (shapeA->GetShapeType() == ShapeType::Sphere && shapeB->GetShapeType() == ShapeType::Sphere)
				{
					return true;
				}
			}
		}
		return false;
	}
}