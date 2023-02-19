#include "MoveTo.h"

#include "globalThings.h"		// for getRandom()


cMoveTo::cMoveTo(cMeshObject* Beholder, glm::vec3 location)
{
	this->m_Beholder = Beholder;
	this->destination = location;
	this->time = -1.f;
	this->velocity = -1.f;
	this->m_CurrentState = STEP_1_DETERMINE_VELOCITY;
}

cMoveTo::cMoveTo(cMeshObject* Beholder, float velocity, glm::vec3 location)
{
	this->m_Beholder = Beholder;
	this->destination = location;
	this->time = -1.f;
	this->velocity = velocity;
}

cMoveTo::cMoveTo(cMeshObject* Beholder, glm::vec3 location, float seconds)
{
	this->m_Beholder = Beholder;
	this->destination = location;
	this->time = seconds;
	this->velocity = -1.f;
}

void cMoveTo::AtStart(void)
{
	this->m_CurrentState = eStates::STEP_1_DETERMINE_VELOCITY;
	this->velocity = 0.0f;
	this->time = -1.f;
}

void cMoveTo::AtEnd(void)
{
	this->m_CurrentState = eStates::ALL_DONE;
	return;
}
bool cMoveTo::Update(double deltaTime)
{
	switch (this->m_CurrentState)
	{
	case STEP_1_DETERMINE_VELOCITY:
		{
			// This means we set it ourselves
		if (velocity < 0.f && time > 0.f)
		{
			velocity = (glm::length(m_Beholder->position - destination) / time) / 100.f;
		}
		else if (velocity <= 0.f)
			velocity = 1.01f;
		m_CurrentState = STEP_2_MOVE;
		}
		break;
	case STEP_2_MOVE:
		{
		glm::vec3 direction = glm::normalize(m_Beholder->position - destination);
		if (velocity > 0.f)
			direction *= velocity;
		m_Beholder->position -= direction * (float)deltaTime;
		if (glm::length(m_Beholder->position - destination) < 1.f)
		{
			m_CurrentState = ALL_DONE;
		}
		}
		break;
	case ALL_DONE:

		return false;
		break;
	};//switch (this->m_CurrentState)

	// Uprate worked (i.e. it's still doing whatever it is that it's supposed to be doing...)
	return true;
}

bool cMoveTo::isComplete(void)
{
	if (this->m_CurrentState == ALL_DONE)
	{
		return true;
	}
	return false;
}