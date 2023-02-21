#ifndef _globalThings_HG_
#define _globalThings_HG_

// This is anything that is shared by all (or many) of the files

#include <GLFW/glfw3.h>

#include "AnimationEvents.h"
#include "AnimationManager.h"
#include "cMeshObject.h"
#include "cVAOManager/sModelDrawInfo.h"
#include "cLightManager.h"
#include "cVAOManager/cVAOManager.h"
#include <Interface/iPhysicsFactory.h>
#include <Interface/iPhysicsWorld.h>
// extern means the variable isn't actually here...
// ...it's somewhere else (in a .cpp file somewhere)
extern GLFWwindow* window;
extern cLightManager* g_pTheLightManager;
extern std::vector< cMeshObject* > g_pMeshObjects;
extern int currentLight;
extern int currentModel;
extern cVAOManager* pVAOManager;
extern bool isTyping;
extern 	cMeshObject* player;
enum eEditMode
{
	MOVING_CAMERA,
	MOVING_LIGHT,
	MOVING_SELECTED_OBJECT,
	MOVING_PHYSICS_OBJECT,
	PHYSICS_TEST,
};
enum eGameMode
{
	Click,
	Hover
};
extern int theEditMode;
extern int numHitTargets;
extern bool menuMode;
extern int gameMode;
extern glm::vec3 angles[3];
extern float rotateSpeed;
extern int animation_to_play;
extern float animation_speed;
extern bool button_pressed;
extern bool pause;
extern bool reverse;
extern AnimationManager* animation_manager;
extern AnimationEvents* event_handler;

#endif
