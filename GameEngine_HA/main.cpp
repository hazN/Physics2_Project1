//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>
#include "globalOpenGL.h"
#include "cLuaBrain.h"
#include "Animation.h"
#include "JSONPersitence.h"
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3        (x,y,z)
#include <glm/vec4.hpp> // glm::vec4        (x,y,z,w)
#include <glm/mat4x4.hpp> // glm::mat4
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>  // File streaming thing (like cin, etc.)
#include <sstream>  // "string builder" type thing

// Some STL (Standard Template Library) things
#include <vector>           // aka a "smart array"

#include "AIBrain.h"
#include "CarAnimations.h"
#include "globalThings.h"

#include "cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightHelper.h"
#include "cVAOManager/c3DModelFileLoader.h"
#include "GUI.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "cBasicTextureManager.h"
#include "GenerateDungeon.h"
#include "GameObject/GameObject.h"
#include <Interface/iPhysicsFactory.h>
#include <Interface/SphereShape.h>
#include <Interface/PlaneShape.h>
#include <GDPPhysics/gdp/PhysicsFactory.h>
#include <GDPPhysics/gdp/PhysicsWorld.h>
#include <GDPPhysics/gdp/CollisionListener.h>
glm::vec3 g_cameraEye = glm::vec3(0.00f, 100, 0.001f);
glm::vec3 g_cameraTarget = glm::vec3(1.0f, 1.0f, 1.0f);
std::vector<GameObject*> physicObjects;
extern int ballIndex;
cBasicTextureManager* g_pTextureManager = NULL;
cCommandScheduler* g_scheduler = new cCommandScheduler;
// Call back signatures here
void mouse_camera_update(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void DrawObject(cMeshObject* pCurrentMeshObject,
	glm::mat4x4 mat_PARENT_Model,               // The "parent's" model matrix
	GLuint shaderID,                            // ID for the current shader
	cBasicTextureManager* pTextureManager,
	cVAOManager* pVAOManager,
	GLint mModel_location,                      // Uniform location of mModel matrix
	GLint mModelInverseTransform_location);      // Uniform location of mView location
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
float hp = 100;
// From here: https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats/5289624
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

bool LoadModelTypesIntoVAO(std::string fileTypesToLoadName,
	cVAOManager* pVAOManager,
	GLuint shaderID)
{
	std::ifstream modelTypeFile(fileTypesToLoadName.c_str());
	if (!modelTypeFile.is_open())
	{
		// Can't find that file
		return false;
	}

	// At this point, the file is open and ready for reading

	std::string PLYFileNameToLoad;     // = "assets/models/MOTO/Blender (load from OBJ export) - only Moto_xyz_n_rgba_uv.ply";
	std::string friendlyName;   // = "MOTO";

	bool bKeepReadingFile = true;

	const unsigned int BUFFER_SIZE = 1000;  // 1000 characters
	char textBuffer[BUFFER_SIZE];       // Allocate AND clear (that's the {0} part)
	// Clear that array to all zeros
	memset(textBuffer, 0, BUFFER_SIZE);

	// Or if it's integers, you can can do this short cut:
	// char textBuffer[BUFFER_SIZE] = { 0 };       // Allocate AND clear (that's the {0} part)

	while (bKeepReadingFile)
	{
		// Reads the entire line into the buffer (including any white space)
		modelTypeFile.getline(textBuffer, BUFFER_SIZE);

		PLYFileNameToLoad.clear();  // Erase whatever is already there (from before)
		PLYFileNameToLoad.append(textBuffer);

		// Is this the end of the file (have I read "EOF" yet?)?
		if (PLYFileNameToLoad == "EOF")
		{
			// All done
			bKeepReadingFile = false;
			// Skip to the end of the while loop
			continue;
		}

		// Load the "friendly name" line also

		memset(textBuffer, 0, BUFFER_SIZE);
		modelTypeFile.getline(textBuffer, BUFFER_SIZE);
		friendlyName.clear();
		friendlyName.append(textBuffer);

		sModelDrawInfo motoDrawInfo;

		c3DModelFileLoader fileLoader;
		//if (LoadThePLYFile(PLYFileNameToLoad, motoDrawInfo))
		std::string errorText = "";
		if (fileLoader.LoadPLYFile_Format_XYZ_N_RGBA_UV(PLYFileNameToLoad, motoDrawInfo, errorText))
		{
			std::cout << "Loaded the file OK" << std::endl;
		}
		else
		{
			std::cout << errorText;
		}

		if (pVAOManager->LoadModelIntoVAO(friendlyName, motoDrawInfo, shaderID))
		{
			std::cout << "Loaded the " << friendlyName << " model" << std::endl;
		}
	}//while (modelTypeFile

	return true;
}

bool CreateObjects(std::string fileName)
{
	std::ifstream objectFile(fileName.c_str());
	if (!objectFile.is_open())
	{
		// Can't find that file
		return false;
	}

	// Basic variables
	std::string meshName;
	std::string friendlyName;
	glm::vec3 position;
	//glm::vec3 rotation;
	glm::quat qRotation;
	glm::vec3 scale;
	// Advanced
	bool useRGBA;
	glm::vec4 colour;
	bool isWireframe;
	bool doNotLight;
	// skip first line
	std::getline(objectFile, meshName);
	std::getline(objectFile, meshName);
	meshName = "";
	for (std::string line; std::getline(objectFile, line);)
	{
		std::istringstream in(line);
		std::string type;
		in >> type;
		if (type == "basic")
		{
			in >> meshName >> friendlyName >> position.x >> position.y >> position.z >> qRotation.x >> qRotation.y >> qRotation.z >> scale.x >> scale.y >> scale.z;
			cMeshObject* pObject = new cMeshObject();
			pObject->meshName = meshName;
			pObject->friendlyName = friendlyName;
			pObject->position = position;
			pObject->qRotation = qRotation;
			pObject->scaleXYZ = scale;
			g_pMeshObjects.push_back(pObject);
		}
		//else if (type == "advanced")
		//{
		//	in >> meshName >> friendlyName >> useRGBA >> colour.x >> colour.y >> colour.z >> colour.w >> isWireframe >> scale >> doNotLight;
		//	cMeshObject* pObject = new cMeshObject();
		//	pObject->meshName = meshName;
		//	pObject->friendlyName = friendlyName;
		//	pObject->bUse_RGBA_colour = useRGBA;
		//	pObject->RGBA_colour = colour;
		//	pObject->isWireframe = isWireframe;
		//	pObject->scale = 1.0f;
		//	pObject->bDoNotLight = doNotLight;
		//}
	}

	return true;
}
bool SaveTheVAOModelTypesToFile(std::string fileTypesToLoadName,
	cVAOManager* pVAOManager);

void DrawConcentricDebugLightObjects(void);

// HACK: These are the light spheres we will use for debug lighting
cMeshObject* pDebugSphere_1 = NULL;// = new cMeshObject();
cMeshObject* pDebugSphere_2 = NULL;// = new cMeshObject();
cMeshObject* pDebugSphere_3 = NULL;// = new cMeshObject();
cMeshObject* pDebugSphere_4 = NULL;// = new cMeshObject();
cMeshObject* pDebugSphere_5 = NULL;// = new cMeshObject();

int main(int argc, char* argv[])
{
	//srand
	srand(time(NULL));

	std::cout << "starting up..." << std::endl;

	GLuint vertex_buffer = 0;

	GLuint shaderID = 0;

	GLint vpos_location = 0;
	GLint vcol_location = 0;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(640, 480, "Project #1 - Hassan Assaf", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "Window created." << std::endl;

	glfwMakeContextCurrent(window);
	//   gladLoadGL( (GLADloadproc)glfwGetProcAddress );
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSwapInterval(1);

	// IMGUI
	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//platform / render bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 420"))
	{
		return 3;
	}

	// Create a shader thingy
	cShaderManager* pTheShaderManager = new cShaderManager();

	cShaderManager::cShader vertexShader01;
	cShaderManager::cShader fragmentShader01;

	vertexShader01.fileName = "assets/shaders/vertexShader01.glsl";
	fragmentShader01.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!pTheShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01))
	{
		vertexShader01.fileName = "vertexShader01.glsl";
		fragmentShader01.fileName = "fragmentShader01.glsl";
		if (!pTheShaderManager->createProgramFromFile("Shader_1", vertexShader01, fragmentShader01))
		{
			std::cout << "Didn't compile shaders" << std::endl;
			std::string theLastError = pTheShaderManager->getLastError();
			std::cout << "Because: " << theLastError << std::endl;
			return -1;
		}
	}
	else
	{
		std::cout << "Compiled shader OK." << std::endl;
	}

	pTheShaderManager->useShaderProgram("Shader_1");

	shaderID = pTheShaderManager->getIDFromFriendlyName("Shader_1");

	glUseProgram(shaderID);

	::g_pTheLightManager = new cLightManager();

	cLightHelper* pLightHelper = new cLightHelper();

	// Set up the uniform variable (from the shader
	::g_pTheLightManager->LoadLightUniformLocations(shaderID);

	// Make this a spot light
//    vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//                    // 0 = pointlight
//                    // 1 = spot light
//                    // 2 = directional light
	::g_pTheLightManager->vecTheLights[0].name = "MainLight";
	::g_pTheLightManager->vecTheLights[0].param1.x = 0.0f;
	::g_pTheLightManager->vecTheLights[0].position = glm::vec4(0.0f, 300.0f, -1000.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	// inner and outer angles
	::g_pTheLightManager->vecTheLights[0].atten = glm::vec4(0.1f, 0.001f, 0.0000001f, 1.0f);
	::g_pTheLightManager->vecTheLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[0].param1.y = 10.0f;     // Degrees
	::g_pTheLightManager->vecTheLights[0].param1.z = 20.0f;     // Degrees
	::g_pTheLightManager->vecTheLights[0].TurnOn();

	// Make light #2 a directional light
	// BE CAREFUL about the direction and colour, since "colour" is really brightness.
	// (i.e. there NO attenuation)
	::g_pTheLightManager->vecTheLights[1].name = "MoonLight";
	::g_pTheLightManager->vecTheLights[1].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[1].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[1].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[1].diffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[1].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[1].TurnOn();
	::g_pTheLightManager->vecTheLights[2].name = "notSure";

	::g_pTheLightManager->vecTheLights[3].name = "Torch2";
	::g_pTheLightManager->vecTheLights[3].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[3].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[3].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[3].diffuse = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[3].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[3].TurnOn();

	::g_pTheLightManager->vecTheLights[4].name = "Torch3";
	::g_pTheLightManager->vecTheLights[4].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[4].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[4].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[4].diffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[4].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[4].TurnOn();

	::g_pTheLightManager->vecTheLights[5].name = "Torch4";
	::g_pTheLightManager->vecTheLights[5].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[5].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].TurnOn();

	::g_pTheLightManager->vecTheLights[5].name = "Torch5";
	::g_pTheLightManager->vecTheLights[5].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[5].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[5].TurnOn();

	::g_pTheLightManager->vecTheLights[6].name = "Torch6";
	::g_pTheLightManager->vecTheLights[6].param1.x = 0.0f;  // 2 means directional
	::g_pTheLightManager->vecTheLights[6].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[6].atten = glm::vec4(0.1f, 0.00001f, 0.1f, 1.0f);
	::g_pTheLightManager->vecTheLights[6].diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[6].specular = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLightManager->vecTheLights[6].TurnOn();

	for (int i = 7; i < 16; i++)
	{
		::g_pTheLightManager->vecTheLights[i].param1.x = 1.0f;
		::g_pTheLightManager->vecTheLights[i].position = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
		::g_pTheLightManager->vecTheLights[i].atten = glm::vec4(0.1f, 0.001f, 0.1f, 1.0f);
		::g_pTheLightManager->vecTheLights[i].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		::g_pTheLightManager->vecTheLights[i].direction = glm::vec4(0.f, 0.f, 1.f, 1.f);
		::g_pTheLightManager->vecTheLights[i].qDirection = glm::quat(glm::vec3(0));
		::g_pTheLightManager->vecTheLights[i].param1.y = 10.0f;     // Degrees
		::g_pTheLightManager->vecTheLights[i].param1.z = 20.0f;     // Degrees
	}
	::g_pTheLightManager->vecTheLights[7].name = "Beholder1L";
	::g_pTheLightManager->vecTheLights[8].name = "Beholder1M";
	::g_pTheLightManager->vecTheLights[9].name = "Beholder1R";
	::g_pTheLightManager->vecTheLights[10].name = "Beholder2L";
	::g_pTheLightManager->vecTheLights[11].name = "Beholder2M";
	::g_pTheLightManager->vecTheLights[12].name = "Beholder2R";
	::g_pTheLightManager->vecTheLights[13].name = "Beholder3L";
	::g_pTheLightManager->vecTheLights[14].name = "Beholder3M";
	::g_pTheLightManager->vecTheLights[15].name = "Beholder3R";

	// Load the models

	if (!LoadModelTypesIntoVAO("assets/PLYFilesToLoadIntoVAO.txt", pVAOManager, shaderID))
	{
		if (!LoadModelTypesIntoVAO("PLYFilesToLoadIntoVAO.txt", pVAOManager, shaderID))
		{
			std::cout << "Error: Unable to load list of models to load into VAO file" << std::endl;
		}
		// Do we exit here?
		// (How do we clean up stuff we've made, etc.)
	}//if (!LoadModelTypesIntoVAO...

// **************************************************************************************
// START OF: LOADING the file types into the VAO manager:
/*
*/
// END OF: LOADING the file types into the VAO manager
// **************************************************************************************

	// On the heap (we used new and there's a pointer)

	if (!CreateObjects("assets/createObjects.txt"))
	{
		if (!CreateObjects("createObjects.txt"))
		{
			std::cout << "Error: Unable to load list of objects to create" << std::endl;
		}
	}

	cMeshObject* pSkyBox = new cMeshObject();
	pSkyBox->meshName = "Skybox_Sphere";
	pSkyBox->friendlyName = "skybox";
	// I'm NOT going to add it to the list of objects to draw

	//	// ISLAND Terrain (final exam 2021 example)
	//cMeshObject* pIslandTerrain = new cMeshObject();
	//pIslandTerrain->meshName = "Terrain";     //  "Terrain";
	//pIslandTerrain->friendlyName = "Ground";
	//pIslandTerrain->position = glm::vec3(0.0f, -25.0f, 0.0f);
	//pIslandTerrain->isWireframe = false;
	//pIslandTerrain->textures[0] = "grass.bmp";
	//pIslandTerrain->textureRatios[0] = 1.f;
	//pIslandTerrain->SetUniformScale(5);
	////g_pMeshObjects.push_back(pIslandTerrain);

	cMeshObject* pTerrain = new cMeshObject();
	pTerrain->meshName = "Terrain";     //  "Terrain";
	pTerrain->friendlyName = "Ground";
	//pTerrain->bUse_RGBA_colour = false;      // Use file colours    pTerrain->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	//pTerrain->specular_colour_and_power
	//	= glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
	// Make it REALY transparent
	//pTerrain->RGBA_colour.w = 1.f;
	pTerrain->position = glm::vec3(0.0f, 0.0f, 0.0f);
	pTerrain->isWireframe = false;
	pTerrain->SetUniformScale(1);
	pTerrain->textures[0] = "grass.bmp";
	pTerrain->textures[1] = "grass.bmp";
	pTerrain->textures[2] = "grass.bmp";
	pTerrain->textures[3] = "grass.bmp";
	pTerrain->textureRatios[0] = 0.5f;
	pTerrain->textureRatios[1] = 0.5f;
	pTerrain->textureRatios[2] = 0.5f;
	pTerrain->textureRatios[3] = 0.5f;

	cMeshObject* wall1 = new cMeshObject();
	wall1->meshName = "Terrain";
	wall1->friendlyName = "Wall1";
	wall1->position = glm::vec3(100, 0, 0);
	wall1->setRotationFromEuler(glm::vec3(0, 0, glm::radians(90.f)));
	wall1->isWireframe = false;
	wall1->SetUniformScale(1);
	wall1->bDoNotLight = true;
	wall1->textures[0] = "grass.bmp";
	wall1->textureRatios[0] = 0.5f;

	cMeshObject* wall2 = new cMeshObject();
	wall2->meshName = "Terrain";
	wall2->friendlyName = "Wall2";
	wall2->position = glm::vec3(-100, 0, 0);
	wall2->setRotationFromEuler(glm::vec3(0, 0, glm::radians(-90.f)));
	wall2->isWireframe = false;
	wall2->SetUniformScale(1);
	wall2->bDoNotLight = true;
	wall2->textures[0] = "grass.bmp";
	wall2->textureRatios[0] = 0.5f;

	cMeshObject* wall3 = new cMeshObject();
	wall3->meshName = "Terrain";
	wall3->friendlyName = "Wall3";
	wall3->position = glm::vec3(0, 0, 100);
	wall3->setRotationFromEuler(glm::vec3(glm::radians(-90.f), 0, 0));
	wall3->isWireframe = false;
	wall3->SetUniformScale(1);
	wall3->bDoNotLight = true;
	wall3->textures[0] = "grass.bmp";
	wall3->textureRatios[0] = 0.5f;

	cMeshObject* wall4 = new cMeshObject();
	wall4->meshName = "Terrain";
	wall4->friendlyName = "Wall4";
	wall4->position = glm::vec3(0, 0, -100);
	wall4->setRotationFromEuler(glm::vec3(glm::radians(90.f), 0, 0));
	wall4->isWireframe = false;
	wall4->SetUniformScale(1);
	wall4->bDoNotLight = true;
	wall4->textures[0] = "grass.bmp";
	wall4->textureRatios[0] = 0.5f;

	g_pMeshObjects.push_back(pTerrain);
	g_pMeshObjects.push_back(wall1);
	g_pMeshObjects.push_back(wall2);
	g_pMeshObjects.push_back(wall3);
	g_pMeshObjects.push_back(wall4);

	//basic Terrain Ground 0 0 0 0 0 0 1
	// DEBUG SPHERES
	pDebugSphere_1 = new cMeshObject();
	pDebugSphere_1->meshName = "ISO_Sphere_1";
	pDebugSphere_1->friendlyName = "Debug_Sphere_1";
	pDebugSphere_1->bUse_RGBA_colour = true;
	pDebugSphere_1->RGBA_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pDebugSphere_1->isWireframe = true;
	pDebugSphere_1->SetUniformScale(1);

	pDebugSphere_1->bDoNotLight = true;

	g_pMeshObjects.push_back(pDebugSphere_1);

	pDebugSphere_2 = new cMeshObject();
	pDebugSphere_2->meshName = "ISO_Sphere_1";
	pDebugSphere_2->friendlyName = "Debug_Sphere_2";
	pDebugSphere_2->bUse_RGBA_colour = true;
	pDebugSphere_2->RGBA_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pDebugSphere_2->isWireframe = true;
	pDebugSphere_2->SetUniformScale(1);
	pDebugSphere_2->bDoNotLight = true;
	g_pMeshObjects.push_back(pDebugSphere_2);

	pDebugSphere_3 = new cMeshObject();
	pDebugSphere_3->meshName = "ISO_Sphere_1";
	pDebugSphere_3->friendlyName = "Debug_Sphere_3";
	pDebugSphere_3->bUse_RGBA_colour = true;
	pDebugSphere_3->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pDebugSphere_3->isWireframe = true;
	pDebugSphere_3->SetUniformScale(1);
	pDebugSphere_3->bDoNotLight = true;
	g_pMeshObjects.push_back(pDebugSphere_3);

	pDebugSphere_4 = new cMeshObject();
	pDebugSphere_4->meshName = "ISO_Sphere_1";
	pDebugSphere_4->friendlyName = "Debug_Sphere_4";
	pDebugSphere_4->bUse_RGBA_colour = true;
	pDebugSphere_4->RGBA_colour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	pDebugSphere_4->isWireframe = true;
	pDebugSphere_4->SetUniformScale(1);
	pDebugSphere_4->bDoNotLight = true;
	g_pMeshObjects.push_back(pDebugSphere_4);

	pDebugSphere_5 = new cMeshObject();
	pDebugSphere_5->meshName = "ISO_Sphere_1";
	pDebugSphere_5->friendlyName = "Debug_Sphere_5";
	pDebugSphere_5->bUse_RGBA_colour = true;
	pDebugSphere_5->RGBA_colour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	pDebugSphere_5->isWireframe = true;
	pDebugSphere_5->SetUniformScale(1);
	pDebugSphere_5->bDoNotLight = true;
	g_pMeshObjects.push_back(pDebugSphere_5);

	GLint mvp_location = glGetUniformLocation(shaderID, "MVP");       // program
	// uniform mat4 mModel;
	// uniform mat4 mView;
	// uniform mat4 mProjection;
	GLint mModel_location = glGetUniformLocation(shaderID, "mModel");
	GLint mView_location = glGetUniformLocation(shaderID, "mView");
	GLint mProjection_location = glGetUniformLocation(shaderID, "mProjection");
	// Need this for lighting
	GLint mModelInverseTransform_location = glGetUniformLocation(shaderID, "mModelInverseTranspose");

	// Textures
	::g_pTextureManager = new cBasicTextureManager();

	::g_pTextureManager->SetBasePath("assets/textures");
	if (!::g_pTextureManager->Create2DTextureFromBMPFile("aerial-drone-view-rice-field.bmp"))
	{
		std::cout << "Didn't load texture" << std::endl;
	}
	else
	{
		std::cout << "texture loaded" << std::endl;
	}
	::g_pTextureManager->Create2DTextureFromBMPFile("Dungeons_2_Texture_01_A.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("lroc_color_poles_4k.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("taylor-swift-tour-dates-fan-wedding-plans.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("water.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("crystal.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("crystal2.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("crystal3.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("flame.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("Beholder_Base_color.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("grass.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("Long_blue_Jet_Flame.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("cobblestones_stencil_mask.bmp");

	::g_pTextureManager->Create2DTextureFromBMPFile("carbon.bmp");

	// Load a skybox
	// Here's an example of the various sides: http://www.3dcpptutorials.sk/obrazky/cube_map.jpg
	std::string errorString = "";
	if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
		"SpaceBox_right1_posX.bmp", /* positive X */
		"SpaceBox_left2_negX.bmp",  /* negative X */
		"SpaceBox_top3_posY.bmp",    /* positive Y */
		"SpaceBox_bottom4_negY.bmp",  /* negative Y */
		"SpaceBox_front5_posZ.bmp",/* positive Z */
		"SpaceBox_back6_negZ.bmp",/* negative Z */
		true, errorString))
	{
		std::cout << "Loaded the night sky cube map OK" << std::endl;
	}
	else
	{
		std::cout << "ERROR: Didn't load the tropical sunny day cube map. How sad." << std::endl;
		std::cout << "(because: " << errorString << std::endl;
	}
	//std::string errorString = "";
	//if (::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
	//	"TropicalSunnyDayRight2048.bmp", /* positive X */
	//	"TropicalSunnyDayLeft2048.bmp",  /* negative X */
	//	"TropicalSunnyDayUp2048.bmp",    /* positive Y */
	//	"TropicalSunnyDayDown2048.bmp",  /* negative Y */
	//	"TropicalSunnyDayBack2048.bmp",  /* positive Z */
	//	"TropicalSunnyDayFront2048.bmp", /* negative Z */
	//	true, errorString))
	//{
	//	std::cout << "Loaded the tropical sunny day cube map OK" << std::endl;
	//}
	//else
	//{
	//	std::cout << "ERROR: Didn't load the tropical sunny day cube map. How sad." << std::endl;
	//	std::cout << "(because: " << errorString << std::endl;
	//}
	std::cout.flush();
	// GUI
	ImGui::StyleColorsDark();
	GUI EditGUI;
	//// Physics
	//g_engine.Initialize();
	float deltaTime = std::clock();
	float duration = 0;
	//bool increase = true;
	int increase = 1;
	//pVAOManager->Load();

	// Animations
//	CarAnimations* car_animations = new CarAnimations;

	//persistence::SaveModels(g_pMeshObjects);
	//persistence::LoadAll(g_pMeshObjects, g_pTheLightManager->vecTheLights);

	// Physics
	using namespace physics;
	iPhysicsFactory* physicsFactory = new PhysicsFactory;
	iPhysicsWorld* world = physicsFactory->CreateWorld();;
	// Ground
	GameObject* ground = new GameObject;
	{
		iShape* planeShape0 = new PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
		RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(0, 0, 0);
		desc.linearVelocity = glm::vec3(0.f);
		desc.friction = 0.1f;
		ground->rigidBody = physicsFactory->CreateRigidBody(desc, planeShape0);
		ground->mesh = pTerrain;
	}
	//Wall 1
	GameObject* goWall = new GameObject;
	{
		goWall->mesh = wall1;
		iShape* planeShape = new PlaneShape(0.0f, glm::vec3(1.f, 0.f, 0.f));

		RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(99, 0.f, 0);
		desc.linearVelocity = glm::vec3(0.f);
		desc.rotation = wall1->qRotation;
		goWall->rigidBody = physicsFactory->CreateRigidBody(desc, planeShape);
	}
	// Wall 2
	GameObject* goWall2 = new GameObject;
	{
		goWall2->mesh = wall2;
		iShape* planeShape2 = new PlaneShape(0.0f, glm::vec3(-1.f, 0.f, 0.f));

		RigidBodyDesc desc2;
		desc2.isStatic = true;
		desc2.mass = 0;
		desc2.position = glm::vec3(-99, 0.f, 0);
		desc2.linearVelocity = glm::vec3(0.f);
		desc2.rotation = wall2->qRotation;
		goWall2->rigidBody = physicsFactory->CreateRigidBody(desc2, planeShape2);
	}
	// Wall 3
	GameObject* goWall3 = new GameObject;
	{
		goWall3->mesh = wall3;
		iShape* planeShape3 = new PlaneShape(0.0f, glm::vec3(0.f, 0.f, 1.f));

		RigidBodyDesc desc3;
		desc3.isStatic = true;
		desc3.mass = 0;
		desc3.position = glm::vec3(0.f, 0.f, 99.f);
		desc3.linearVelocity = glm::vec3(0.f);
		desc3.rotation = wall3->qRotation;
		goWall3->rigidBody = physicsFactory->CreateRigidBody(desc3, planeShape3);
	}
	// Wall 4
	GameObject* goWall4 = new GameObject;
	{
		goWall4->mesh = wall4;
		iShape* planeShape4 = new PlaneShape(0.0f, glm::vec3(0.f, 0.f, -1.f));

		RigidBodyDesc desc4;
		desc4.isStatic = true;
		desc4.mass = 0;
		desc4.position = glm::vec3(0, 0.f, -99.f);
		desc4.linearVelocity = glm::vec3(0.f);
		desc4.rotation = wall4->qRotation;
		goWall4->rigidBody = physicsFactory->CreateRigidBody(desc4, planeShape4);
	}
	// Balls
	GameObject* ballOne = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallOne";
		meshBall->position = glm::vec3(50,50,50);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(2);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 2;
		ballDesc.position = glm::vec3(50,50,50);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(2);

		ballOne->mesh = meshBall;
		ballOne->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballOne->mesh);
		physicObjects.push_back(ballOne);
	}
	GameObject* ballTwo = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallTwo";
		meshBall->position = glm::vec3(0.f, 50.f, 0.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(5);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 5;
		ballDesc.position = glm::vec3(0.f, 50.f, 0.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(5);

		ballTwo->mesh = meshBall;
		ballTwo->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballTwo->mesh);
		physicObjects.push_back(ballTwo);
	}
	GameObject* ballThree = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallThree";
		meshBall->position = glm::vec3(50.f, 50.f, 0.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(3.5);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 3.5;
		ballDesc.position = glm::vec3(50.f, 50.f, 0.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(3.5);

		ballThree->mesh = meshBall;
		ballThree->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballThree->mesh);
		physicObjects.push_back(ballThree);
	}
	GameObject* ballFour = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallFour";
		meshBall->position = glm::vec3(-50.f, 50.f, 0.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(7);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 7;
		ballDesc.position = glm::vec3(-50.f, 50.f, 0.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(7);

		ballFour->mesh = meshBall;
		ballFour->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballFour->mesh);
		physicObjects.push_back(ballFour);
	}
	GameObject* ballFive = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallFive";
		meshBall->position = glm::vec3(0.f, 50.f, 40.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(6);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 6;
		ballDesc.position = glm::vec3(0.f, 50.f, 40.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(6);

		ballFive->mesh = meshBall;
		ballFive->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballFive->mesh);
		physicObjects.push_back(ballFive);
	}
	GameObject* ballSix = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallSix";
		meshBall->position = glm::vec3(30.f, 50.f, 40.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(15);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 15;
		ballDesc.position = glm::vec3(30.f, 50.f, 40.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(15);

		ballSix->mesh = meshBall;
		ballSix->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballSix->mesh);
		physicObjects.push_back(ballSix);
	}
	GameObject* ballSeven = new GameObject;
	{
		cMeshObject* meshBall = new cMeshObject();
		meshBall->meshName = "ISO_Sphere_1";
		meshBall->friendlyName = "BallSeven";
		meshBall->position = glm::vec3(-30.f, 50.f, 40.f);
		meshBall->isWireframe = false;
		meshBall->bDoNotLight = false;
		meshBall->bUse_RGBA_colour = true;
		meshBall->RGBA_colour = glm::vec4(1, 0, 0, 1);
		meshBall->Enabled = true;
		meshBall->scaleXYZ = glm::vec3(8);

		RigidBodyDesc ballDesc;
		ballDesc.isStatic = false;
		ballDesc.mass = 8;
		ballDesc.position = glm::vec3(-30.f, 50.f, 40.f);
		ballDesc.linearVelocity = Vector3(0.f, 0.f, 0.f);
		iShape* ballShape = new SphereShape(8);

		ballSeven->mesh = meshBall;
		ballSeven->rigidBody = physicsFactory->CreateRigidBody(ballDesc, ballShape);
		g_pMeshObjects.push_back(ballSeven->mesh);
		physicObjects.push_back(ballSeven);
	}
	// World
	world->AddBody(ground->rigidBody);
	world->AddBody(goWall->rigidBody);
	world->AddBody(goWall2->rigidBody);
	world->AddBody(goWall3->rigidBody);
	world->AddBody(goWall4->rigidBody);
	world->AddBody(ballOne->rigidBody);
	world->AddBody(ballTwo->rigidBody);
	world->AddBody(ballThree->rigidBody);
	world->AddBody(ballFour->rigidBody);
	world->AddBody(ballFive->rigidBody);
	world->AddBody(ballSix->rigidBody);
	world->AddBody(ballSeven->rigidBody);
	world->SetGravity(glm::vec3(0, -9.81, 0));
	g_cameraTarget = glm::vec3(0.f, 0, 0.f);
	g_cameraEye = glm::vec3(1.f, 150, 0.f);
	theEditMode = PHYSICS_TEST;
	//iCollisionListener* listener = new CollisionListener;
	//world->RegisterCollisionListener(listener);
	while (!glfwWindowShouldClose(window))
	{
		// Doing this here to get a faster reponse time
		{
			float force = 5.f;
			glm::vec3 direction(0.f);
			glm::vec3 forwardVector(g_cameraEye.x, 0.0f, g_cameraEye.z);
			glm::vec3 rightVector(glm::cross(forwardVector, glm::vec3(0, 1, 0)));
			if (glfwGetKey(window, GLFW_KEY_W))
			{
				direction -= forwardVector * 1.f;
			}
			if (glfwGetKey(window, GLFW_KEY_S))
			{
				direction += forwardVector * 1.f;
			}
			if (glfwGetKey(window, GLFW_KEY_A))
			{
				direction += rightVector * 1.f;
			}
			if (glfwGetKey(window, GLFW_KEY_D))
			{
				direction -= rightVector * 1.f;
			}
			physicObjects[ballIndex]->rigidBody->ApplyForce(direction * force);
		}
		// Physics Update
		world->TimeStep(0.1f);
		// Update all Mesh positions to rigidbody positions
		for (GameObject* go : physicObjects)
		{
			Vector3 pos;
			go->rigidBody->GetPosition(pos);
			go->mesh->position = glm::vec3(pos);
			go->rigidBody->GetRotation(go->mesh->qRotation);
		}
		::g_pTheLightManager->CopyLightInformationToShader(shaderID);
		duration = (std::clock() - deltaTime) / (double)CLOCKS_PER_SEC;
		// Mouse Lookaround
		if (!menuMode && theEditMode != PHYSICS_TEST)
			mouse_camera_update(window);
		DrawConcentricDebugLightObjects();
		float ratio;
		int width, height;
		glm::mat4x4 matProjection;
		glm::mat4x4 matView;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

		if (theEditMode == PHYSICS_TEST)
		{
			matView = glm::lookAt(::g_cameraEye,
				::g_cameraTarget,
				upVector);
		}
		else {
			matView = glm::lookAt(::g_cameraEye,
				::g_cameraEye + ::g_cameraTarget,
				upVector);
		}
		// Pass eye location to the shader
		// uniform vec4 eyeLocation;
		GLint eyeLocation_UniLoc = glGetUniformLocation(shaderID, "eyeLocation");

		glUniform4f(eyeLocation_UniLoc,
			::g_cameraEye.x, ::g_cameraEye.y, ::g_cameraEye.z, 1.0f);

		matProjection = glm::perspective(
			0.6f,       // Field of view (in degress, more or less 180)
			ratio,
			0.1f,       // Near plane (make this as LARGE as possible)
			10000.0f);   // Far plane (make this as SMALL as possible)
		// 6-8 digits of precision
		//
// Set once per scene (not per object)
		glUniformMatrix4fv(mView_location, 1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(mProjection_location, 1, GL_FALSE, glm::value_ptr(matProjection));

		//    ____  _             _            __
		//   / ___|| |_ __ _ _ __| |_    ___  / _|  ___  ___ ___ _ __   ___
		//   \___ \| __/ _` | '__| __|  / _ \| |_  / __|/ __/ _ \ '_ \ / _ \
        //    ___) | || (_| | |  | |_  | (_) |  _| \__ \ (_|  __/ | | |  __/
		//   |____/ \__\__,_|_|   \__|  \___/|_|   |___/\___\___|_| |_|\___|
		//
		// We draw everything in our "scene"
		// In other words, go throug the vec_pMeshObjects container
		//  and draw each one of the objects
		for (std::vector< cMeshObject* >::iterator itCurrentMesh = g_pMeshObjects.begin();
			itCurrentMesh != g_pMeshObjects.end();
			itCurrentMesh++)
		{
			cMeshObject* pCurrentMeshObject = *itCurrentMesh;        // * is the iterator access thing

			// Where do I put THIS??
			// i.e. if the object is invisble by the child objects ARE visible...?
			if (!pCurrentMeshObject->bIsVisible)
			{
				// Skip the rest of the loop
				continue;
			}

			// The parent's model matrix is set to the identity
			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// All the drawing code has been moved to the DrawObject function
			DrawObject(pCurrentMeshObject,
				matModel,
				shaderID, ::g_pTextureManager,
				pVAOManager, mModel_location, mModelInverseTransform_location);
		}//for ( unsigned int index

		//// Draw the island model
		//GLint bIsIlandModel_IniformLocation = glGetUniformLocation(shaderID, "bIsTerrainModel");
		//glUniform1f(bIsIlandModel_IniformLocation, (GLfloat)GL_TRUE);

		//glm::mat4x4 matModel = glm::mat4x4(1.0f);

		//// All the drawing code has been moved to the DrawObject function
		//DrawObject(pTerrain,
		//	matModel,
		//	shaderID, ::g_pTextureManager,
		//	pVAOManager, mModel_location, mModelInverseTransform_location);

		//glUniform1f(bIsIlandModel_IniformLocation, (GLfloat)GL_FALSE);

		//// Draw flames
		//{
		//	GLint bIsFlameObject_UniformLocation = glGetUniformLocation(shaderID, "bIsFlameObject");
		//	glUniform1f(bIsFlameObject_UniformLocation, (GLfloat)GL_TRUE);

		//	glm::mat4x4 matModel = glm::mat4x4(1.0f);

		//	// All the drawing code has been moved to the DrawObject function
		//	DrawObject(Torch1Flame,
		//		matModel,
		//		shaderID, ::g_pTextureManager,
		//		pVAOManager, mModel_location, mModelInverseTransform_location);
		//	Torch1Flame->position = pTorch->position;
		//	DrawObject(Torch2Flame,
		//		matModel,
		//		shaderID, ::g_pTextureManager,
		//		pVAOManager, mModel_location, mModelInverseTransform_location);
		//	Torch2Flame->position = pTorch2->position;
		//	DrawObject(Torch3Flame,
		//		matModel,
		//		shaderID, ::g_pTextureManager,
		//		pVAOManager, mModel_location, mModelInverseTransform_location);
		//	Torch3Flame->position = pTorch3->position;
		//	DrawObject(Torch4Flame,
		//		matModel,
		//		shaderID, ::g_pTextureManager,
		//		pVAOManager, mModel_location, mModelInverseTransform_location);
		//	Torch4Flame->position = pTorch4->position;
		//	DrawObject(Torch5Flame,
		//		matModel,
		//		shaderID, ::g_pTextureManager,
		//		pVAOManager, mModel_location, mModelInverseTransform_location);
		//	Torch5Flame->position = pTorch5->position;
		//	glUniform1f(bIsFlameObject_UniformLocation, (GLfloat)GL_FALSE);
		//}
		// Draw the skybox
		{
			GLint bIsSkyboxObject_UL = glGetUniformLocation(shaderID, "bIsSkyboxObject");
			glUniform1f(bIsSkyboxObject_UL, (GLfloat)GL_TRUE);

			glm::mat4x4 matModel = glm::mat4x4(1.0f);

			// move skybox to the cameras location
			pSkyBox->position = ::g_cameraEye;

			// The scale of this large skybox needs to be smaller than the far plane
			//  of the projection matrix.
			// Maybe make it half the size
			// Here, our far plane is 10000.0f...
			pSkyBox->SetUniformScale(7500.0f);

			// All the drawing code has been moved to the DrawObject function
			DrawObject(pSkyBox,
				matModel,
				shaderID, ::g_pTextureManager,
				pVAOManager, mModel_location, mModelInverseTransform_location);

			// Turn this off
			glUniform1f(bIsSkyboxObject_UL, (GLfloat)GL_FALSE);
		}//for ( unsigned int index
		//    _____           _          __
		//   | ____|_ __   __| |   ___  / _|  ___  ___ ___ _ __   ___
		//   |  _| | '_ \ / _` |  / _ \| |_  / __|/ __/ _ \ '_ \ / _ \
        //   | |___| | | | (_| | | (_) |  _| \__ \ (_|  __/ | | |  __/
		//   |_____|_| |_|\__,_|  \___/|_|   |___/\___\___|_| |_|\___|
		//
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		EditGUI.render();
		{
			ImGui::Begin("##CH", nullptr, ImGuiWindowFlags_NoMove | ImGuiInputTextFlags_ReadOnly | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
			ImDrawList* draw = ImGui::GetBackgroundDrawList();
			draw->AddCircle(ImVec2((1920 / 2) - 10, 1009 / 2), 6, IM_COL32(255, 0, 0, 255), 100, 0.0f);
			ImGui::End();
		}
		{
			/*		ImGui::Begin("#HP", nullptr, ImGuiWindowFlags_NoMove | ImGuiInputTextFlags_ReadOnly | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);
					ImDrawList* draw = ImGui::GetBackgroundDrawList();
					draw->AddRectFilled(ImVec2(width / 5, height - 25.f), ImVec2((width / 3) + (1000), height + 25.f), IM_COL32(255, 255, 255, 255), 0.0f, 0.0f);
					draw->AddRectFilled(ImVec2(width / 5, height - 25.f), ImVec2((width / 3) + (hp*10), height + 25.f), IM_COL32(0, 255, 0, 255), 0.0f, 0.0f);
					ImGui::End();*/
		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);

		// Set the window title
		//glfwSetWindowTitle(window, "Hey");

		std::stringstream ssTitle;
		ssTitle << "Camera (x,y,z): "
			<< ::g_cameraEye.x << ", "
			<< ::g_cameraEye.y << ", "
			<< ::g_cameraEye.z
			<< "Target (x,y,z): "
			<< ::g_cameraTarget.x << ", "
			<< ::g_cameraTarget.y << ", "
			<< ::g_cameraTarget.z
			<< "  Light #" << currentLight << " (xyz): "
			<< ::g_pTheLightManager->vecTheLights[currentLight].position.x << ", "
			<< ::g_pTheLightManager->vecTheLights[currentLight].position.y << ", "
			<< ::g_pTheLightManager->vecTheLights[currentLight].position.z
			<< " linear: " << ::g_pTheLightManager->vecTheLights[currentLight].atten.y
			<< " quad: " << ::g_pTheLightManager->vecTheLights[currentLight].atten.z;

		std::string theText = ssTitle.str();

		glfwSetWindowTitle(window, ssTitle.str().c_str());
		// Or this:
		//std::string theText = ssTitle.str();
		//glfwSetWindowTitle(window, ssTitle.str().c_str() );
	}

	// Get rid of stuff
	delete pTheShaderManager;
	delete ::g_pTheLightManager;

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
void DrawConcentricDebugLightObjects(void)
{
	extern bool bEnableDebugLightingObjects;

	if (!bEnableDebugLightingObjects)
	{
		pDebugSphere_1->bIsVisible = false;
		pDebugSphere_2->bIsVisible = false;
		pDebugSphere_3->bIsVisible = false;
		pDebugSphere_4->bIsVisible = false;
		pDebugSphere_5->bIsVisible = false;
		return;
	}

	pDebugSphere_1->bIsVisible = true;
	pDebugSphere_2->bIsVisible = true;
	pDebugSphere_3->bIsVisible = true;
	pDebugSphere_4->bIsVisible = true;
	pDebugSphere_5->bIsVisible = true;

	cLightHelper theLightHelper;

	// Move the debug sphere to where the light #0 is
	pDebugSphere_1->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	pDebugSphere_2->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	pDebugSphere_3->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	pDebugSphere_4->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	pDebugSphere_5->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);

	{
		// Draw a bunch of concentric spheres at various "brightnesses"
		float distance75percent = theLightHelper.calcApproxDistFromAtten(
			0.75f,  // 75%
			0.001f,
			100000.0f,
			::g_pTheLightManager->vecTheLights[currentLight].atten.x,
			::g_pTheLightManager->vecTheLights[currentLight].atten.y,
			::g_pTheLightManager->vecTheLights[currentLight].atten.z);

		pDebugSphere_2->SetUniformScale(distance75percent);
		pDebugSphere_2->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	}

	{
		// Draw a bunch of concentric spheres at various "brightnesses"
		float distance50percent = theLightHelper.calcApproxDistFromAtten(
			0.50f,  // 75%
			0.001f,
			100000.0f,
			::g_pTheLightManager->vecTheLights[currentLight].atten.x,
			::g_pTheLightManager->vecTheLights[currentLight].atten.y,
			::g_pTheLightManager->vecTheLights[currentLight].atten.z);

		pDebugSphere_3->SetUniformScale(distance50percent);
		pDebugSphere_3->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	}

	{
		// Draw a bunch of concentric spheres at various "brightnesses"
		float distance25percent = theLightHelper.calcApproxDistFromAtten(
			0.25f,  // 75%
			0.001f,
			100000.0f,
			::g_pTheLightManager->vecTheLights[currentLight].atten.x,
			::g_pTheLightManager->vecTheLights[currentLight].atten.y,
			::g_pTheLightManager->vecTheLights[currentLight].atten.z);

		pDebugSphere_4->SetUniformScale(distance25percent);
		pDebugSphere_4->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	}

	{
		// Draw a bunch of concentric spheres at various "brightnesses"
		float distance5percent = theLightHelper.calcApproxDistFromAtten(
			0.05f,  // 75%
			0.001f,
			100000.0f,
			::g_pTheLightManager->vecTheLights[currentLight].atten.x,
			::g_pTheLightManager->vecTheLights[currentLight].atten.y,
			::g_pTheLightManager->vecTheLights[currentLight].atten.z);

		pDebugSphere_5->SetUniformScale(distance5percent);
		pDebugSphere_5->position = glm::vec3(::g_pTheLightManager->vecTheLights[currentLight].position);
	}
	return;
}