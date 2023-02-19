#pragma once
#include <vector>
#include "globalThings.h"

enum AItype { A, B, C};

class Actor
{
public:
	Actor() = default;
	~Actor() = default;
	Actor(cMeshObject* obj, AItype type);
	Actor(cMeshObject* obj, AItype type, float speed);
	Actor(cMeshObject* obj, AItype type, float max, float min);
	Actor(cMeshObject* obj, AItype type, float max, float min, float speed);
	cMeshObject* obj;
	AItype type;
	void Face(glm::vec3 Target);
	void Seek(glm::vec3 Target);
	void Flee(glm::vec3 Target);
	void Pursue(glm::vec3 Target, glm::vec3 Force);
	void Evade(glm::vec3 Target, glm::vec3 Force);
	void Approach(glm::vec3 Target);
private:
	float Max;
	float Min;
	float Speed;
};

class AIBrain
{
public:
	AIBrain() = default;
	~AIBrain() = default;

	std::vector<Actor> actors_;
	cMeshObject* Player;
	AIBrain(cMeshObject* player);
	void update(float dt);

};
