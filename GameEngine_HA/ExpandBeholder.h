#pragma once

#include "cCommand.h"
#include "cMeshObject.h"


class ExpandBeholder : public cCommand
{
private:
	ExpandBeholder() {};	// Can't be called because it's private
public:
	ExpandBeholder(cMeshObject* Beholder, float size);


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
	float size = 1.f;
	eStates m_CurrentState;
};
