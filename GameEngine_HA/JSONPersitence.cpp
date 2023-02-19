#include "JSONPersitence.h"

namespace persistence
{
	void SaveAll(std::vector<cMeshObject*> modelsToSave, std::vector<cLight> lightsToSave)
	{
		SaveModels(modelsToSave);
		SaveLights(lightsToSave);
	}

	void LoadAll(std::vector<cMeshObject*>& modelsToLoad, std::vector<cLight>& lightsToLoad)
	{
		LoadModels(modelsToLoad);
		LoadLights(lightsToLoad);
	}

	// Saves vector of cMeshObject pointers to JSON
	void SaveModels(std::vector<cMeshObject*> modelsToSave)
	{
		std::ofstream saveFile;
		saveFile.open("./savedata/modelData.json");
		json models;
		for (cMeshObject* model : modelsToSave)
		{
			json m = json{ {"id", model->id}, {"meshName", model->meshName}, {"friendlyName", model->friendlyName},
				{"position", json::array({model->position.x,model->position.y,model->position.z})},
				{"qRotation", json::array({model->qRotation.w, model->qRotation.x, model->qRotation.y, model->qRotation.z})},
				{"scaleXYZ", json::array({model->scaleXYZ.x,model->scaleXYZ.y, model->scaleXYZ.z})},
				{"isWireframe", model->isWireframe},{"bUse_RGBA_colour", model->bUse_RGBA_colour},
				{"RGBA_colour", json::array({model->RGBA_colour.x, model->RGBA_colour.y, model->RGBA_colour.z, model->RGBA_colour.w})},
				{"specular_colour_and_power", json::array({model->specular_colour_and_power.x, model->specular_colour_and_power.y, model->specular_colour_and_power.z, model->specular_colour_and_power.w})},
				{"bDoNotLight", model->bDoNotLight}, {"bIsVisible", model->bIsVisible},
				{"textures", json::array({model->textures[0],model->textures[1], model->textures[2], model->textures[3], model->textures[4],
				model->textures[5], model->textures[6], model->textures[7]})},
					{"textureRatios", json::array({model->textureRatios[0], model->textureRatios[1], model->textureRatios[2], model->textureRatios[3], model->textureRatios[4],
					model->textureRatios[5], model->textureRatios[6], model->textureRatios[7]})}
			};
			models.push_back(json{ { model->friendlyName, m }});
		}
		saveFile << models;
		saveFile.close();
	}

	void SaveLights(std::vector<cLight> lightsToSave)
	{
		//std::string name;
		//glm::vec4 position;
		//glm::vec4 diffuse;
		//glm::vec4 specular;	
		//glm::vec4 atten;		
		//glm::quat qDirection;
		//glm::vec4 direction;	
		//glm::vec4 param1;	
		//glm::vec4 param2;
		std::ofstream saveFile;
		saveFile.open("./savedata/lightData.json");
		json lights;
		for (cLight light : lightsToSave)
		{
			json l = json{ {"name", light.name}, {"position", json::array({light.position.x, light.position.y, light.position.z})},
				{"diffuse", json::array({light.diffuse.x, light.diffuse.y, light.diffuse.z, light.diffuse.w})},
				{"specular", json::array({light.specular.x, light.specular.y, light.specular.z, light.specular.w})},
				{"atten", json::array({light.atten.x, light.atten.y, light.atten.z, light.atten.w})},
				{"qDirection", json::array({ light.qDirection.w, light.qDirection.x, light.qDirection.y, light.qDirection.z})},
				{"direction", json::array({light.direction.x, light.direction.y, light.direction.z, light.direction.w})},
				{"param1", json::array({light.param1.x, light.param1.y, light.param1.z, light.param1.w})},
				{"param2", json::array({light.param2.x, light.param2.y, light.param2.z, light.param2.w})}
				};
			lights.push_back(l);
		}
		saveFile << lights;
		saveFile.close();
	}

	// Loads data from modelData file into provided cMeshObject pointer vector
	void LoadModels(std::vector<cMeshObject*>& modelsToLoad)
	{
		std::ifstream file("./savedata/modelData.json", std::ifstream::in);
		json f;

		// Handle any exceptions
		try
		{
			file >> f;
		}
		catch (json::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		if (f == nlohmann::detail::value_t::null)
		{
			std::cout << "Error loading modelData.json, save file may be corrupted." << std::endl;
			return;
		}
		for (json header : f)
		{
			for (json m : header)
			{
				cMeshObject* model = new cMeshObject();
				model->id = m.at("id");
				model->meshName = m.at("meshName");
				model->friendlyName = m.at("friendlyName");
				model->position.x = m.at("position")[0];
				model->position.y = m.at("position")[1];
				model->position.z = m.at("position")[2];
				model->qRotation.w = m.at("qRotation")[0];
				model->qRotation.x = m.at("qRotation")[1];
				model->qRotation.y = m.at("qRotation")[2];
				model->qRotation.z = m.at("qRotation")[3];
				model->scaleXYZ.x = m.at("scaleXYZ")[0];
				model->scaleXYZ.y = m.at("scaleXYZ")[1];
				model->scaleXYZ.z = m.at("scaleXYZ")[2];
				model->isWireframe = m.at("isWireframe");
				model->bUse_RGBA_colour = m.at("bUse_RGBA_colour");
				model->RGBA_colour.x = m.at("RGBA_colour")[0];
				model->RGBA_colour.y = m.at("RGBA_colour")[1];
				model->RGBA_colour.z = m.at("RGBA_colour")[2];
				model->RGBA_colour.w = m.at("RGBA_colour")[3];
				model->specular_colour_and_power.x = m.at("specular_colour_and_power")[0];
				model->specular_colour_and_power.y = m.at("specular_colour_and_power")[1];
				model->specular_colour_and_power.z = m.at("specular_colour_and_power")[2];
				model->specular_colour_and_power.w = m.at("specular_colour_and_power")[3];
				model->bDoNotLight = m.at("bDoNotLight");
				model->bIsVisible = m.at("bIsVisible");
				model->textures[0] = m.at("textures")[0];
				model->textures[1] = m.at("textures")[1];
				model->textures[2] = m.at("textures")[2];
				model->textures[3] = m.at("textures")[3];
				model->textures[4] = m.at("textures")[4];
				model->textures[5] = m.at("textures")[5];
				model->textures[6] = m.at("textures")[6];
				model->textures[7] = m.at("textures")[7];
				model->textureRatios[0] = m.at("textureRatios")[0];
				model->textureRatios[1] = m.at("textureRatios")[1];
				model->textureRatios[2] = m.at("textureRatios")[2];
				model->textureRatios[3] = m.at("textureRatios")[3];
				model->textureRatios[4] = m.at("textureRatios")[4];
				model->textureRatios[5] = m.at("textureRatios")[5];
				model->textureRatios[6] = m.at("textureRatios")[6];
				model->textureRatios[7] = m.at("textureRatios")[7];
				// Check if it already exists in the system and replace if it does
				// If it doesn't then add it to the vector
				bool replaced = false;
				for (int i = 0; i < modelsToLoad.size(); i++)
				{
					if (modelsToLoad[i]->friendlyName == model->friendlyName)
					{
						modelsToLoad[i] = model;
						replaced = true;
					}
				}
				if (!replaced)
					modelsToLoad.push_back(model);
			}
		}
		file.close();
	}

	void LoadLights(std::vector<cLight>& lightsToLoad)
	{
		std::ifstream file("./savedata/lightData.json", std::ifstream::in);
		json f;

		// Handle any exceptions
		try
		{
			file >> f;
		}
		catch (json::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		if (f == nlohmann::detail::value_t::null)
		{
			std::cout << "Error loading lightData.json, save file may be corrupted." << std::endl;
			return;
		}
		int i = 0;
		for (json light : f)
		{
			lightsToLoad[i].name = light.at("name");
			lightsToLoad[i].atten.x = light.at("atten")[0];
			lightsToLoad[i].atten.y = light.at("atten")[1];
			lightsToLoad[i].atten.z = light.at("atten")[2];
			lightsToLoad[i].atten.w = light.at("atten")[3];
			lightsToLoad[i].diffuse.x = light.at("diffuse")[0];
			lightsToLoad[i].diffuse.y = light.at("diffuse")[1];
			lightsToLoad[i].diffuse.z = light.at("diffuse")[2];
			lightsToLoad[i].diffuse.w = light.at("diffuse")[3];
			lightsToLoad[i].specular.x = light.at("specular")[0];
			lightsToLoad[i].specular.y = light.at("specular")[1];
			lightsToLoad[i].specular.z = light.at("specular")[2];
			lightsToLoad[i].specular.w = light.at("specular")[3];
			lightsToLoad[i].direction.x = light.at("direction")[0];
			lightsToLoad[i].direction.y = light.at("direction")[1];
			lightsToLoad[i].direction.z = light.at("direction")[2];
			lightsToLoad[i].direction.w = light.at("direction")[3];
			lightsToLoad[i].param1.x = light.at("param1")[0];
			lightsToLoad[i].param1.y = light.at("param1")[1];
			lightsToLoad[i].param1.z = light.at("param1")[2];
			lightsToLoad[i].param1.w = light.at("param1")[3];
			lightsToLoad[i].param2.x = light.at("param2")[0];
			lightsToLoad[i].param2.y = light.at("param2")[1];
			lightsToLoad[i].param2.z = light.at("param2")[2];
			lightsToLoad[i].param2.w = light.at("param2")[3];
			lightsToLoad[i].position.x = light.at("position")[0];
			lightsToLoad[i].position.y = light.at("position")[1];
			lightsToLoad[i].position.z = light.at("position")[2];
			lightsToLoad[i].qDirection.w = light.at("qDirection")[0];
			lightsToLoad[i].qDirection.x = light.at("qDirection")[1];
			lightsToLoad[i].qDirection.y = light.at("qDirection")[2];
			lightsToLoad[i].qDirection.z = light.at("qDirection")[3];
			i++;
		}
	}
}