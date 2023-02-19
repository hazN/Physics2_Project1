#pragma once

#include "cCommand.h"
#include "cMeshObject.h"


class cFollow : public cCommand
{
private:
	cFollow() {};	// Can't be called because it's private
public:
	cFollow(cMeshObject* Beholder, cMeshObject* BeholderTarget);
	cFollow(cMeshObject* Beholder, cMeshObject* BeholderTarget, float distance, glm::vec3 offset);

	virtual void AtStart(void);
	virtual void AtEnd(void);
	virtual bool Update(double deltaTime);
	virtual bool isComplete(void);

private:
	cMeshObject* m_Beholder;
	cMeshObject* m_BeholderTarget;

	enum eStates
	{
		STEP_1_DETERMINE_VELOCITY,
		STEP_2_MOVE,
		ALL_DONE
	};
	float distance;
	glm::vec3 offset;
	eStates m_CurrentState;
};
