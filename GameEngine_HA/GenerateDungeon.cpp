#include "GenerateDungeon.h"

void GenerateDungeon::generateFloors(glm::vec3 pos, int x, int y, std::string fileName, std::string parent)
{
	int counter = 0;
	glm::vec3 originalPos = pos;

	for (size_t i = 0; i < x; i++) // 5 by 7
	{
		counter++;
		// FLOOR
		cMeshObject* FloorRoom1 = new cMeshObject();
		FloorRoom1->meshName = fileName;
		FloorRoom1->friendlyName = parent + std::to_string(counter);
		FloorRoom1->bUse_RGBA_colour = false;      // Use file colours    pTerrain->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		FloorRoom1->specular_colour_and_power = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
		FloorRoom1->RGBA_colour.w = 1.f;
		FloorRoom1->position = pos;
		FloorRoom1->setRotationFromEuler(glm::vec3(0, 0, 0));
		FloorRoom1->isWireframe = false;
		FloorRoom1->SetUniformScale(0.01f);
		FloorRoom1->textures[0] = "Dungeons_2_Texture_01_A.bmp";
		FloorRoom1->textures[1] = "Dungeons_2_Texture_01_A.bmp";
		FloorRoom1->textures[2] = "Dungeons_2_Texture_01_A.bmp";
		FloorRoom1->textures[3] = "Dungeons_2_Texture_01_A.bmp";
		FloorRoom1->textureRatios[0] = 1.0f;
		FloorRoom1->textureRatios[1] = 1.0f;
		FloorRoom1->textureRatios[2] = 1.0f;
		FloorRoom1->textureRatios[3] = 1.0f;
		pos.x += 5.f;
		g_pMeshObjects.push_back(FloorRoom1);

		for (size_t i2 = 0; i2 < y; i2++)
		{
			counter++;
			cMeshObject* Floor2Room1 = new cMeshObject();
			Floor2Room1->meshName = fileName;
			Floor2Room1->friendlyName = parent + std::to_string(counter);
			Floor2Room1->bUse_RGBA_colour = false;      // Use file colours    pTerrain->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			Floor2Room1->specular_colour_and_power
				= glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
			// Make it REALY transparent
			Floor2Room1->RGBA_colour.w = 1.f;
			Floor2Room1->position = pos;
			Floor2Room1->setRotationFromEuler(glm::vec3(0, 0, 0));
			Floor2Room1->isWireframe = false;
			Floor2Room1->SetUniformScale(0.01f);
			Floor2Room1->textures[0] = "Dungeons_2_Texture_01_A.bmp";
			Floor2Room1->textures[1] = "Dungeons_2_Texture_01_A.bmp";
			Floor2Room1->textures[2] = "Dungeons_2_Texture_01_A.bmp";
			Floor2Room1->textures[3] = "Dungeons_2_Texture_01_A.bmp";
			Floor2Room1->textureRatios[0] = 1.0f;
			Floor2Room1->textureRatios[1] = 1.0f;
			Floor2Room1->textureRatios[2] = 1.0f;
			Floor2Room1->textureRatios[3] = 1.0f;
			g_pMeshObjects.push_back(Floor2Room1);
			if (i % 2 == 0)
				pos.z -= 5.f;
			else pos.z += 5.f;
		}
	}
	// Realign last piece
	for (cMeshObject* g_p_mesh_object : g_pMeshObjects)
	{
		if (g_p_mesh_object->friendlyName == parent + "1")
		{
			g_p_mesh_object->position = pos;
		}
	}
}

int GenerateDungeon::generateWalls(glm::vec3 pos, glm::vec3 rot, int dir, int length, std::string parent, int doors[3], int counter)
{
	if (doors == NULL)
	{
		doors = new int[3];
		doors[0] = -1;
		doors[1] = -1;
		doors[2] = -1;
	}
	for (size_t i = 0; i < length; i++)
	{
		counter++;

		//direction
		if (dir == 1)
			pos.x += 5.f;
		else if (dir == -1)
			pos.x -= 5.f;
		else if (dir == 2)
			pos.y += 5.f;
		else if (dir == -2)
			pos.y -= 5.f;
		else if (dir == 3)
			pos.z += 5.f;
		else if (dir == -3)
			pos.z -= 5.f;

		// WALL 1
		cMeshObject* Wall1Room1 = new cMeshObject();
		if (doors[0] == counter || doors[1] == counter || doors[2] == counter)
			Wall1Room1->meshName = "Door1";
		else Wall1Room1->meshName = "Wall1";
		Wall1Room1->friendlyName = parent + std::to_string(counter);
		Wall1Room1->bUse_RGBA_colour = false;      // Use file colours    pTerrain->RGBA_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		//Wall1Room1->specular_colour_and_power = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);
		Wall1Room1->RGBA_colour.w = 1.f;
		Wall1Room1->position = pos;
		Wall1Room1->setRotationFromEuler(rot);
		Wall1Room1->isWireframe = false;
		Wall1Room1->SetUniformScale(0.01f);
		Wall1Room1->textures[0] = "Dungeons_2_Texture_01_A.bmp";
		Wall1Room1->textures[1] = "Dungeons_2_Texture_01_A.bmp";
		Wall1Room1->textures[2] = "Dungeons_2_Texture_01_A.bmp";
		Wall1Room1->textures[3] = "Dungeons_2_Texture_01_A.bmp";
		Wall1Room1->textureRatios[0] = 1.0f;
		Wall1Room1->textureRatios[1] = 1.0f;
		Wall1Room1->textureRatios[2] = 1.0f;
		Wall1Room1->textureRatios[3] = 1.0f;
		g_pMeshObjects.push_back(Wall1Room1);
	}
	return counter;
}

void GenerateDungeon::generateDungeon()
{
	float side = 3.1415927f;
	float updown = 1.5707963f;
	// Room 1(bottom left)
	{
		// Room 1(Bottom Left)
		int doors[3] = { 3, 8, -1 };
		int counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(0, 0, 0), 5, 6, "Floor1", "Room1Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 1, 5, "Room1Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-5, 0, -35), glm::vec3(0, side, 0), 1, 5, "Room1Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(0, 0, 5), glm::vec3(0, -updown, 0), -3, 7, "Room1Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(25, 0, 0), glm::vec3(0, updown, 0), -3, 7, "Room1Wall", doors, counter);
		}
		// Room 1 - Room 2 Hallway
		{
			GenerateDungeon::generateFloors(glm::vec3(10, 0, -35), 1, 0, "Floor1", "Room1Room2Floor");
			GenerateDungeon::generateWalls(glm::vec3(5, 0, -35), glm::vec3(0, -updown, 0), 1, 1, "Room1Room2Wall", doors, counter);
			GenerateDungeon::generateWalls(glm::vec3(10, 0, -40), glm::vec3(0, updown, 0), 1, 1, "Room1Room2Wall", doors, counter);
		}
		// Room 2 (Bottom Middle)
		doors[0] = 2;
		doors[1] = 10;
		doors[2] = -1;
		counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(5, 0, -40), 4, 2, "Floor1", "Room2Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(5, 0, -40), glm::vec3(0, 0, 0), 1, 4, "Room2Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(0, 0, -55), glm::vec3(0, side, 0), 1, 4, "Room2Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(5, 0, -35), glm::vec3(0, -updown, 0), -3, 3, "Room2Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(25, 0, -40), glm::vec3(0, updown, 0), -3, 3, "Room2Wall", doors, counter);
		}
		// Room 2 - Room 3 Hallway
		{
			GenerateDungeon::generateFloors(glm::vec3(0, 0, -45), 1, 0, "Floor1", "Room2Room3Floor");
			GenerateDungeon::generateWalls(glm::vec3(0, 0, -45), glm::vec3(0, 0, 0), 1, 1, "Room2Room3Wall", NULL, counter);
			GenerateDungeon::generateWalls(glm::vec3(-5, 0, -50), glm::vec3(0, side, 0), 1, 1, "Room2Room3Wall", NULL, counter);
		}
		// Room 3 (Middle)
		doors[0] = 8;
		doors[1] = 12;
		doors[2] = 15;
		counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(-25, 0, -40), 5, 2, "Floor1", "Room3Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(-25, 0, -40), glm::vec3(0, 0, 0), 1, 5, "Room3Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-30, 0, -55), glm::vec3(0, side, 0), 1, 5, "Room3Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-25, 0, -35), glm::vec3(0, -updown, 0), -3, 3, "Room3Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(0, 0, -40), glm::vec3(0, updown, 0), -3, 3, "Room3Wall", doors, counter);
		}
		// Room 3 - 4 Hallway
		GenerateDungeon::generateFloors(glm::vec3(-15, 0, -55), 1, 0, "Floor1", "Room3Room4Floor");
		GenerateDungeon::generateWalls(glm::vec3(-20, 0, -55), glm::vec3(0, -updown, 0), 1, 1, "Room3Room4Wall", doors, counter);
		GenerateDungeon::generateWalls(glm::vec3(-15, 0, -60), glm::vec3(0, updown, 0), 1, 1, "Room3Room4Wall", doors, counter);
		// Room 4 (Middle Right)
		doors[0] = 3;
		doors[1] = 14;
		doors[2] = -1;
		counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(-25, 0, -60), 3, 4, "Floor1", "Room4Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(-25, 0, -60), glm::vec3(0, 0, 0), 1, 3, "Room4Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-30, 0, -85), glm::vec3(0, side, 0), 1, 3, "Room4Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-25, 0, -55), glm::vec3(0, -updown, 0), -3, 5, "Room4Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-10, 0, -60), glm::vec3(0, updown, 0), -3, 5, "Room4Wall", doors, counter);
		}
		// Room 3 - 5 Hallway
		GenerateDungeon::generateFloors(glm::vec3(-30, 0, -45), 1, 0, "Floor1", "Room3Room5Floor");
		GenerateDungeon::generateWalls(glm::vec3(-30, 0, -45), glm::vec3(0, 0, 0), 1, 1, "Room3Room5Wall", doors, counter);
		GenerateDungeon::generateWalls(glm::vec3(-35, 0, -50), glm::vec3(0, side, 0), 1, 1, "Room3Room5Wall", doors, counter);
		// Room 5 (Top Middle)
		doors[0] = 3;
		doors[1] = 13;
		doors[2] = -1;
		counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(-50, 0, -40), 4, 2, "Floor1", "Room5Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(-50, 0, -40), glm::vec3(0, 0, 0), 1, 4, "Room5Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-55, 0, -55), glm::vec3(0, side, 0), 1, 4, "Room5Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-50, 0, -35), glm::vec3(0, -updown, 0), -3, 3, "Room5Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-30, 0, -40), glm::vec3(0, updown, 0), -3, 3, "Room5Wall", doors, counter);
		}
		// Room 5 - 6 Hallway
		GenerateDungeon::generateFloors(glm::vec3(-40, 0, -35), 1, 0, "Floor1", "Room5Room6Floor");
		GenerateDungeon::generateWalls(glm::vec3(-45, 0, -35), glm::vec3(0, -updown, 0), 1, 1, "Room5Room6Wall", doors, counter);
		GenerateDungeon::generateWalls(glm::vec3(-40, 0, -40), glm::vec3(0, updown, 0), 1, 1, "Room5Room6Wall", doors, counter);
		// Room 6 (Top Left)
		doors[0] = 8;
		doors[1] = -1;
		doors[2] = -1;
		counter = 0;
		{
			GenerateDungeon::generateFloors(glm::vec3(-50, 0, 0), 5, 6, "Floor1", "Room6Floor");
			counter = GenerateDungeon::generateWalls(glm::vec3(-50, 0, 0), glm::vec3(0, 0, 0), 1, 5, "Room6Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-55, 0, -35), glm::vec3(0, side, 0), 1, 5, "Room6Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-50, 0, 5), glm::vec3(0, -updown, 0), -3, 7, "Room6Wall", doors, counter);
			counter = GenerateDungeon::generateWalls(glm::vec3(-25, 0, 0), glm::vec3(0, updown, 0), -3, 7, "Room6Wall", doors, counter);
		}
	}
}