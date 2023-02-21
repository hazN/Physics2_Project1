#pragma once

#include <Interface/iPhysicsWorld.h>

namespace physics
{
	class CollisionListener : public iCollisionListener
	{
	public:
		CollisionListener();
		virtual ~CollisionListener();

		virtual bool NotifyCollision(iCollisionBody* bodyA, iCollisionBody* bodyB) override;

	};
}