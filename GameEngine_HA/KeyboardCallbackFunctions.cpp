#include "globalOpenGL.h"
#include "globalThings.h"   // For the light manager, etc.
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "quaternion_utils.h"
// Extern is so the compiler knows what TYPE this thing is
// The LINKER needs the ACTUAL declaration
// These are defined in theMainFunction.cpp
extern glm::vec3 g_cameraEye;// = glm::vec3(0.0, 0.0, -25.0f);
extern glm::vec3 g_cameraTarget;// = glm::vec3(0.0f, 0.0f, 0.0f);
static glm::mat4 camMat = glm::mat4(1.0f);
int ballIndex = 0;

bool bEnableDebugLightingObjects = true;
float OBJECT_MOVE_SPEED = 0.1f;
float CAMERA_MOVE_SPEED = 5.f;
float LIGHT_MOVE_SPEED = 0.1f;
float PLAYER_MOVE_SPEED = 15.f;
bool wireFrame = true;
//0000 0001   1	GLFW_MOD_SHIFT
//0000 0010 	  2
//0000 0100   4
//
//0000 0110
//0000 0001 	"Mask"
//-------- -
//0000 0000
//
//// I ONLY want the shift key and nothing else
//if (mods == GLFW_MOD_SHIFT)
//
//// Shift key but I don't care if anything else is down, too
//if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)

static int m_PrevMouseX;
static int m_PrevMouseY;
static int m_CurrMouseX;
static int m_CurrMouseY;
static int m_MouseDownX;
static int m_MouseDownY;
static float m_HorizontalAngle;
int turn = 0;
int angle [4] = { 0.f, 0.0001f, 0.001f, 0.01f };
void mouse_camera_update(GLFWwindow* window)
{
	m_PrevMouseX = m_CurrMouseX;
	m_PrevMouseY = m_CurrMouseY;
	double tempX = 0;
	double tempY = 0;
	glfwGetCursorPos(window, &tempX, &tempY);
	m_CurrMouseX = floor(tempX);
	m_CurrMouseY = floor(tempY);
	int deltaMouseX = 0;
	int deltaMouseY = 0;

	deltaMouseX = m_CurrMouseX - m_PrevMouseX;
	deltaMouseY = m_CurrMouseY - m_PrevMouseY;

	m_HorizontalAngle -= deltaMouseX * rotateSpeed;

	const float moveSpeed = 5.0f;


	::g_cameraTarget.x = sin(m_HorizontalAngle);
	::g_cameraTarget.z = cos(m_HorizontalAngle);
	::g_cameraTarget.y -= deltaMouseY * rotateSpeed;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (theEditMode == MOVING_CAMERA)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			/*g_engine.setMouseState(true);
			g_engine.RayTest();*/
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			//g_engine.setMouseState(false);
		}
	}
}

void key_callback(GLFWwindow* window,
	int key, int scancode,
	int action, int mods)
{
	if (!isTyping)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			//glfwSetWindowShouldClose(window, GLFW_TRUE);
			menuMode = !menuMode;
			if (menuMode)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else 
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			if (mods == GLFW_MOD_CONTROL)
			{
				pVAOManager->Save();
			}
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			if (mods == GLFW_MOD_CONTROL)
			{
				int lightIndex = 0;
				int modelIndex = 0;
				std::ifstream saveFile("saveData.txt");
				if (!saveFile.is_open())
				{
					std::cout << "Load failed..." << std::endl;
				}
				else {
					pVAOManager->Load();
				}
			}
		}
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			theEditMode = MOVING_CAMERA;
		}
		else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			theEditMode = MOVING_LIGHT;

			//        // Check for the mods to turn the spheres on or off
			//        if ( mods == GLFW_MOD_CONTROL )
			//        {
			//            bEnableDebugLightingObjects = true;
			//        }
			//        if ( mods == GLFW_MOD_ALT )
			//        {
			//            bEnableDebugLightingObjects = false;
			//        }
		}
		else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			theEditMode = MOVING_SELECTED_OBJECT;
		}
		if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		{
			// Check for the mods to turn the spheres on or off
			bEnableDebugLightingObjects = false;
		}
		if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		{
			// Check for the mods to turn the spheres on or off
			bEnableDebugLightingObjects = true;
		}

		switch (theEditMode)
		{
		case PHYSICS_TEST:
		{
			if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
			{
				ballIndex--;
				if (ballIndex < 0)
				{
					ballIndex = 6;
				}
			}
			if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
			{
				ballIndex++;
				if (ballIndex > 6)
				{
					ballIndex = 0;
				}
			}
			if (key == GLFW_KEY_UP)
			{
				g_cameraEye.y -= 5.f;
			}
			if (key == GLFW_KEY_DOWN)
			{
				g_cameraEye.y += 5.f;
			}
			if (key == GLFW_KEY_RIGHT)
			{
				glm::mat4 camMat(1);
				camMat = glm::rotate(camMat, glm::radians(5.f), glm::vec3(0.f, 1.f, 0.f));
				glm::vec3 result = glm::vec3(camMat * glm::vec4(g_cameraEye, 1.f));
				g_cameraEye = result;
			}
			if (key == GLFW_KEY_LEFT)
			{
				glm::mat4 camMat(1);
				camMat = glm::rotate(camMat, glm::radians(-5.f), glm::vec3(0.f, 1.f, 0.f));
				glm::vec3 result = glm::vec3(camMat * glm::vec4(g_cameraEye, 1.f));
				g_cameraEye = result;
			}
		}
		break;
		case MOVING_CAMERA:
		{
			// Move the camera
			// AWSD   AD - left and right
			//        WS - forward and back
			glm::vec3 forwardVector(g_cameraTarget.x, 0.0f, g_cameraTarget.z);
			glm::vec3 rightVector(glm::cross(forwardVector, glm::vec3(0, 1, 0)));

			if (key == GLFW_KEY_A)     // Left
			{
				::g_cameraEye -= rightVector * CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_D)     // Right
			{
				::g_cameraEye += rightVector * CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_W)     // Forward
			{
				::g_cameraEye += forwardVector * CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_S)     // Backwards
			{
				::g_cameraEye -= forwardVector * CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_Q)     // Down
			{
				::g_cameraEye.y -= CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_E)     // Up
			{
				::g_cameraEye.y += CAMERA_MOVE_SPEED;
			}
			if (key == GLFW_KEY_1)
			{
				animation_speed = 0.01f;
			}
			if (key == GLFW_KEY_2)
			{
				animation_speed = 0.025f;
			}
			if (key == GLFW_KEY_3)
			{
				animation_speed = 0.05f;
			}
			if (key == GLFW_KEY_4)
			{
				animation_speed = 0.075f;
			}
			if (key == GLFW_KEY_5)
			{
				animation_speed = 0.1f;
			}
			if (key == GLFW_KEY_R && action == GLFW_PRESS)
			{
				reverse = !reverse;
			}
			if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			{
				event_handler->watching.clear();
				event_handler->events_.clear();
				animation_manager->ClearAnimations();
				animation_to_play++;
				if (animation_to_play > 3)
				{
					animation_to_play = 1;
				}
				button_pressed = true;

			}
			if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			{
				event_handler->watching.clear();
				event_handler->events_.clear();
				animation_manager->ClearAnimations();
				animation_to_play--;
				if (animation_to_play < 1)
				{
					animation_to_play = 3;
				}
				button_pressed = true;

			}
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			{
				pause = !pause;
			}
			if (key == GLFW_KEY_X)     // Left
			{
				int width = 1200;
				int height = 800;
				glfwGetWindowSize(window, &width, &height);
				glfwSetCursorPos(window, width / 2, height / 2);
				::g_cameraTarget = glm::vec3(0, 0, 0);
			}
		}//case MOVING_CAMERA:
		break;

		case MOVING_LIGHT:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				if (LIGHT_MOVE_SPEED == 0.1f)
				{
					LIGHT_MOVE_SPEED = 3.f;
				}
				else LIGHT_MOVE_SPEED = 0.1f;
			}
			if (key == GLFW_KEY_A)     // Left
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.x -= LIGHT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_D)     // Right
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.x += LIGHT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_W)     // Forward
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.z += LIGHT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_S)     // Backwards
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.z -= LIGHT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_Q)     // Down
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.y -= LIGHT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_E)     // Up
			{
				::g_pTheLightManager->vecTheLights[currentLight].position.y += LIGHT_MOVE_SPEED;
			}

			if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
			{
				// Select previous light
				if (currentLight > 0)
				{
					currentLight--;
				}
			}
			if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
			{
				if (currentLight < (::g_pTheLightManager->vecTheLights.size() - 1))
				{
					// Select previous light
					currentLight++;
				}
			}

			// Change attenuation
			// Linear is ==> "how bright the light is"
			// Quadratic is ==> "how far does the light go or 'throw' into the scene?"
			if (key == GLFW_KEY_1)
			{
				// Linear Decrease by 1%
				::g_pTheLightManager->vecTheLights[currentLight].atten.y *= 0.99f;
			}
			if (key == GLFW_KEY_2)
			{
				// Linear Increase by 1%
				::g_pTheLightManager->vecTheLights[currentLight].atten.y *= 1.01f;
			}
			if (key == GLFW_KEY_3)
			{
				if (mods == GLFW_MOD_SHIFT)
				{   // ONLY shift modifier is down
					// Quadratic Decrease by 0.1%
					::g_pTheLightManager->vecTheLights[currentLight].atten.z *= 0.99f;
				}
				else
				{
					// Quadratic Decrease by 0.01%
					::g_pTheLightManager->vecTheLights[currentLight].atten.z *= 0.999f;
				}
			}
			if (key == GLFW_KEY_4)
			{
				if (mods == GLFW_MOD_SHIFT)
				{   // ONLY shift modifier is down
					// Quadratic Increase by 0.1%
					::g_pTheLightManager->vecTheLights[currentLight].atten.z *= 1.01f;
				}
				else
				{
					// Quadratic Decrease by 0.01%
					::g_pTheLightManager->vecTheLights[currentLight].atten.z *= 1.001f;
				}
			}
			if (key == GLFW_KEY_5)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.x *= 0.99f;
			}
			if (key == GLFW_KEY_6)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.x *= 1.01f;
			}
			if (key == GLFW_KEY_7)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.y *= 1.01f;
			}
			if (key == GLFW_KEY_8)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.y *= 0.99f;
			}
			if (key == GLFW_KEY_T)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.z *= 0.99f;
			}
			if (key == GLFW_KEY_Y)
			{
				::g_pTheLightManager->vecTheLights[currentLight].diffuse.z *= 1.01f;
			}
		}//case MOVING_LIGHT:
		break;
		case MOVING_SELECTED_OBJECT:
		{
			if (mods == GLFW_MOD_SHIFT)
			{
				if (OBJECT_MOVE_SPEED == 0.1f)
				{
					OBJECT_MOVE_SPEED = 1.f;
				}
				else OBJECT_MOVE_SPEED = 0.1f;
			}
			if (key == GLFW_KEY_A)     // Left
			{
				::g_pMeshObjects[currentModel]->position.x += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_D)     // Right
			{
				::g_pMeshObjects[currentModel]->position.x -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_W)     // Forward
			{
				::g_pMeshObjects[currentModel]->position.z += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_S)     // Backwards
			{
				::g_pMeshObjects[currentModel]->position.z -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_Q)     // Down
			{
				::g_pMeshObjects[currentModel]->position.y -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_E)     // Up
			{
				::g_pMeshObjects[currentModel]->position.y += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_Z)     // Rotate x
			{
				::g_pMeshObjects[currentModel]->qRotation.x += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_X)     // Rotate x
			{
				::g_pMeshObjects[currentModel]->qRotation.x -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_V)     // Rotate y
			{
				::g_pMeshObjects[currentModel]->qRotation.y += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_B)     // Rotate y
			{
				::g_pMeshObjects[currentModel]->qRotation.y -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_F)     // Rotate z
			{
				::g_pMeshObjects[currentModel]->qRotation.z += OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_G)     // Rotate z
			{
				::g_pMeshObjects[currentModel]->qRotation.z -= OBJECT_MOVE_SPEED;
			}
			if (key == GLFW_KEY_EQUAL)     // Size UP
			{
				::g_pMeshObjects[currentModel]->SetUniformScale(::g_pMeshObjects[currentModel]->scaleXYZ.x += OBJECT_MOVE_SPEED);
			}
			if (key == GLFW_KEY_MINUS)     // Size DOWN
			{
				::g_pMeshObjects[currentModel]->SetUniformScale(::g_pMeshObjects[currentModel]->scaleXYZ.x -= OBJECT_MOVE_SPEED);
			}
			if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
			{
				// Select previous light
				if (currentModel > 0)
				{
					g_pMeshObjects[currentModel]->isWireframe = false;
					currentModel--;
					g_pMeshObjects[currentModel]->isWireframe = wireFrame;
				}
			}
			if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
			{
				if (currentModel < (::g_pMeshObjects.size() - 1))
				{
					g_pMeshObjects[currentModel]->isWireframe = false;
					currentModel++;
					g_pMeshObjects[currentModel]->isWireframe = wireFrame;
				}
			}
			if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS)
			{
				if (wireFrame == true)
				{
					wireFrame = false;
				}
				else wireFrame = true;
			}
			g_pMeshObjects[currentModel]->isWireframe = wireFrame;
		}
		break;
		}//switch (theEditMode)
	}
	return;
}