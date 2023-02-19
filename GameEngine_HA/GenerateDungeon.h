#pragma once
#include <string>
#include <glm/vec3.hpp>
#include "globalThings.h"
class GenerateDungeon
{
public:
	static void generateFloors(glm::vec3 pos, int x, int y, std::string fileName, std::string parent);
	static int generateWalls(glm::vec3 pos,glm::vec3 rot, int dir, int length, std::string parent, int doors[3], int counter);
	static void generateDungeon();
};
