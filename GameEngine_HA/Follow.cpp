#include "Follow.h"
#include "globalThings.h"		// for getRandom()



cFollow::cFollow(cMeshObject* Beholder, cMeshObject* BeholderTarget)
{
	m_Beholder = Beholder;
	m_BeholderTarget = BeholderTarget;
	offset = glm::vec3(0);
	distance = 5.f;
}

cFollow::cFollow(cMeshObject* Beholder, cMeshObject* BeholderTarget, float distance, glm::vec3 offset)
{
	m_Beholder = Beholder;
	m_BeholderTarget = BeholderTarget;
	this->offset = offset;
	this->distance = distance;
}

void cFollow::AtStart(void)
{
	this->m_CurrentState = eStates::STEP_1_DETERMINE_VELOCITY;
}

void cFollow::AtEnd(void)
{
	this->m_CurrentState = eStates::ALL_DONE;
	return;
}
bool cFollow::Update(double deltaTime)
{
	switch (this->m_CurrentState)
	{
	case STEP_1_DETERMINE_VELOCITY:
	{
		 //TOP RIGHT CORNER
		glm::vec3 direction = glm::normalize(m_Beholder->position - ((m_BeholderTarget->position + offset) + distance));
		direction *= deltaTime;
		m_Beholder->position -= direction;
		if (glm::length(m_Beholder->position - ((m_BeholderTarget->position + offset) + distance)) < distance)
		{
			this->m_CurrentState = STEP_1_DETERMINE_VELOCITY;
		}
	}
	break;
	break;
	case ALL_DONE:

		return false;
		break;
	};//switch (this->m_CurrentState)

	// Uprate worked (i.e. it's still doing whatever it is that it's supposed to be doing...)
	return true;
}

bool cFollow::isComplete(void)
{
	if (this->m_CurrentState == ALL_DONE)
	{
		return true;
	}
	return false;
}