#pragma once
#include <Interface/iRigidBody.h>
#include "../cMeshObject.h"
#include <GDPPhysics/gdp/RigidBody.h>
using namespace physics;
class GameObject
{
public:
	iRigidBody* rigidBody;
	cMeshObject* mesh;
};