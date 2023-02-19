#ifndef PhysicsFactory_H
#define PhysicsFactory_H

#include <Interface/iPhysicsFactory.h>

namespace physics
{
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory();
		virtual ~PhysicsFactory();

		virtual iPhysicsWorld* CreateWorld() override;
		virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) override;
		virtual iSoftBody* CreateSoftBody(const SoftBodyDesc& desc) override;
	};
}

#endif
