#pragma once
#include <nlohmann/json.hpp>
#include "cLight.h"
#include "cMeshObject.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
using json = nlohmann::json;

namespace persistence
{
	void SaveAll(std::vector<cMeshObject*> modelsToSave, std::vector<cLight> lightsToSave);
	void LoadAll(std::vector<cMeshObject*>& modelsToSave, std::vector<cLight>& lightsToSave);
	void SaveModels(std::vector<cMeshObject*> modelsToSave);
	void SaveLights(std::vector<cLight> lightsToSave);
	void LoadModels(std::vector<cMeshObject*>& modelsToLoad);
	void LoadLights(std::vector<cLight>& lightsToLoad);
}
