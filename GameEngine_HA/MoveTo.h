#pragma once

#include "cCommand.h"
#include "cMeshObject.h"


class cMoveTo : public cCommand
{
private:
	cMoveTo() {};	// Can't be called because it's private
public:
	cMoveTo(cMeshObject* Beholder, glm::vec3 location);
	cMoveTo(cMeshObject* Beholder, float velocity, glm::vec3 location);
	cMoveTo(cMeshObject* Beholder, glm::vec3 location, float seconds);

	virtual void AtStart(void);
	virtual void AtEnd(void);
	virtual bool Update(double deltaTime);
	virtual bool isComplete(void);

private:
	cMeshObject* m_Beholder;

	enum eStates
	{
		STEP_1_DETERMINE_VELOCITY,
		STEP_2_MOVE,
		ALL_DONE
	};
	glm::vec3 destination;
	double m_StateCountDownTimer;
	float velocity = 0.0f;
	float time = -1.f;
	eStates m_CurrentState;
};
