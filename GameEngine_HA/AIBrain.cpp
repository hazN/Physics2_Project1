//#include "AIBrain.h"
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/euler_angles.hpp>
//#include <glm/gtx/norm.hpp>
//
//#include "quaternion_utils.h"
//extern glm::vec3 g_cameraEye;// = glm::vec3(0.0, 0.0, -25.0f);
//extern glm::vec3 g_cameraTarget;// = glm::vec3(0.0f, 0.0f, 0.0f);
//Actor::Actor(cMeshObject* obj, AItype type)
//{
//	this->obj = obj;
//	this->type = type;
//	Max = 70.f;
//	Min = 50.f;
//	Speed = 1.1f;
//}
//
//Actor::Actor(cMeshObject* obj, AItype type, float speed)
//{
//	this->obj = obj;
//	this->type = type;
//	Max = 70.f;
//	Min = 50.f;
//	Speed = speed;
//}
//
//Actor::Actor(cMeshObject* obj, AItype type, float max, float min)
//{
//	this->obj = obj;
//	this->type = type;
//	Max = max;
//	Min = min;
//	Speed = 1.1f;
//}
//Actor::Actor(cMeshObject* obj, AItype type, float max, float min, float speed)
//{
//	this->obj = obj;
//	this->type = type;
//	Max = max;
//	Min = min;
//	Speed = speed;
//}
//void Actor::Face(glm::vec3 Target)
//{
//		glm::vec3 direction = glm::normalize(obj->position - Target);
//		//obj->qRotation = RotationBetweenVectors(direction, direction);
//		glm::quat targetDir = glm::quat(direction);
//		//obj->qRotation = RotateTowards(obj->qRotation, targetDir, 3.14f * 0.5);
//		glm::vec3 up = glm::vec3(0, 1, 0);
//		obj->qRotation = (direction, up);
//}
//
//void Actor::Seek(glm::vec3 Target)
//{
//		glm::vec3 direction = glm::normalize(Target - obj->position);
//		glm::quat targetDir = q_utils::LookAt(-direction, glm::vec3(0, 1, 0));
//		obj->qRotation = q_utils::RotateTowards(obj->qRotation, targetDir, 3.14f * 0.005f);
//		//obj->physics_object->ApplyForce(glm::normalize(direction) * Speed);
//}
//
//void Actor::Flee(glm::vec3 Target)
//{
//		glm::vec3 direction = glm::normalize(Target - obj->position);
//		glm::quat targetDir = q_utils::LookAt(direction, glm::vec3(0, 1, 0));
//		obj->qRotation = q_utils::RotateTowards(obj->qRotation, targetDir, 3.14f * 0.005f);
//		//obj->physics_object->ApplyForce(glm::normalize(-direction) * Speed);
//}
//
//void Actor::Pursue(glm::vec3 Target, glm::vec3 Velocity)
//{
//		Target += Velocity * 50.f;
//		glm::vec3 direction = glm::normalize(Target - obj->position);
//		glm::quat targetDir = q_utils::LookAt(-direction, glm::vec3(0, 1, 0));
//		obj->qRotation = q_utils::RotateTowards(obj->qRotation, targetDir, 3.14f * 0.005f);
//		//obj->physics_object->ApplyForce(glm::normalize(direction) * Speed);
//}
//
//void Actor::Evade(glm::vec3 Target, glm::vec3 Velocity)
//{
//	if (glm::length(obj->position - Target) < 300.f)
//	{
//		Target -= Velocity * 50.f;
//		glm::vec3 direction = glm::normalize(Target - obj->position);
//		glm::quat targetDir = q_utils::LookAt(direction, glm::vec3(0, 1, 0));
//		obj->qRotation = q_utils::RotateTowards(obj->qRotation, targetDir, 3.14f * 0.005f);
//		//obj->physics_object->ApplyForce(glm::normalize(-direction) * Speed);
//	}
//}
//
//void Actor::Approach(glm::vec3 Target)
//{
//	if (glm::length(obj->position - Target) > Max)
//	{
//		Seek(Target);
//	}
//	else if (glm::length(obj->position - Target) < Min)
//	{
//		Flee(Target);
//	}
//	else
//	{
//		glm::vec3 direction = glm::normalize(Target - obj->position);
//		glm::quat targetDir = q_utils::LookAt(-direction, glm::vec3(0, 1, 0));
//		obj->qRotation = q_utils::RotateTowards(obj->qRotation, targetDir, 3.14f * 0.005f);
//	}
//}
//
//AIBrain::AIBrain(cMeshObject* player)
//{
//	Player = player;
//}
//
//void AIBrain::update(float dt)
//{
//	for (Actor actor : actors_)
//	{
//		switch (actor.type)
//		{
//		case AItype::A:
//		{
//			
//				glm::vec3 forwardVector;
//				forwardVector.x = 2 * (Player->qRotation.x * Player->qRotation.z + Player->qRotation.w * Player->qRotation.y);
//				forwardVector.y = 2 * (Player->qRotation.y * Player->qRotation.z - Player->qRotation.w * Player->qRotation.x);
//				forwardVector.z = 1 - 2 * (Player->qRotation.x * Player->qRotation.x + Player->qRotation.y * Player->qRotation.y);
//				float dotProduct = glm::dot(glm::normalize(actor.obj->position - Player->position), forwardVector);
//				if (dotProduct > 0.1f)
//				{
//					if (glm::length(actor.obj->position - player->physics_object->position) < 250.f)
//					actor.Flee(Player->position);
//				}
//				else if (dotProduct < 0.1f)
//				{
//					if (glm::length(actor.obj->position - player->physics_object->position) < 300.f)
//					actor.Seek(Player->position);
//				}
//		}
//		break;
//		case AItype::B:
//		{
//			if (glm::length(actor.obj->position - player->physics_object->position) < 300.f)
//			{
//				glm::vec3 forwardVector;
//				forwardVector.x = 2 * (Player->qRotation.x * Player->qRotation.z + Player->qRotation.w * Player->qRotation.y);
//				forwardVector.y = 2 * (Player->qRotation.y * Player->qRotation.z - Player->qRotation.w * Player->qRotation.x);
//				forwardVector.z = 1 - 2 * (Player->qRotation.x * Player->qRotation.x + Player->qRotation.y * Player->qRotation.y);
//				float dotProduct = glm::dot(glm::normalize(actor.obj->position - Player->position), forwardVector);
//				if (dotProduct > 0.2f)
//				{
//					if (glm::length(actor.obj->position - player->physics_object->position) < 250.f)
//					actor.Evade(Player->position, player->physics_object->GetVelocity());
//				}
//				else if (dotProduct < 0.f)
//				{
//					if (glm::length(actor.obj->position - player->physics_object->position) < 300.f)
//					actor.Pursue(Player->position, player->physics_object->GetVelocity());
//				}
//				else actor.obj->physics_object->ApplyForce(glm::vec3(0));
//			}
//		}
//		break;
//		case AItype::C:
//		{
//			actor.Approach(player->physics_object->position);
//		}
//		break;
//		}
//	}
//}
