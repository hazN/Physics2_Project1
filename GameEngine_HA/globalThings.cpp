#include "globalThings.h"


// The variable is HERE.
cLightManager* g_pTheLightManager = NULL;
std::vector< cMeshObject* > g_pMeshObjects;
int currentLight = 0;
int currentModel = 0;
cVAOManager* pVAOManager = new cVAOManager;
int theEditMode = eEditMode::MOVING_CAMERA;
int gameMode = eGameMode::Click;
bool isTyping = false;
GLFWwindow* window;
int numHitTargets = 0;
bool menuMode = true;
cMeshObject* player = new cMeshObject;
glm::vec3 angles[3] = {glm::vec3(0.651306f, -0.335724f, -0.680554f),
						glm::vec3(0.355136f, -0.425400f, -0.832417f),
						glm::vec3(-0.165933f, -0.375744f, -0.911748f) };
float rotateSpeed = 0.001f;
int animation_to_play = 0;
float animation_speed = 0.05f;
bool button_pressed = false;
bool pause = false;
bool reverse = false;
AnimationManager* animation_manager = new AnimationManager;
AnimationEvents* event_handler = new AnimationEvents;
