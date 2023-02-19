#pragma once

#include "cCommand.h"
#include "cMeshObject.h"


class cOrientTo : public cCommand
{
private:
	cOrientTo() {};	// Can't be called because it's private
public:
	cOrientTo(cMeshObject* Beholder, glm::vec3 degrees);

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
	double m_StateCountDownTimer;
	float degrees;
	glm::vec3 rotation;
	glm::vec3 rotationLeft;
	glm::vec4 eulerDir;
	eStates m_CurrentState;
};
