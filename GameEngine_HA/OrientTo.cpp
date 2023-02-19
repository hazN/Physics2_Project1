#include "OrientTo.h"
#include "globalThings.h"		// for getRandom()


cOrientTo::cOrientTo(cMeshObject* Beholder, glm::vec3 rotate)
{
	this->m_Beholder = Beholder;
	this->rotation = rotate;
	this->rotationLeft = rotate;
	glm::mat4 matRotation = glm::mat4(m_Beholder->qRotation);
	eulerDir = glm::vec4(rotation, 1.f);
	eulerDir = matRotation * eulerDir;
}


void cOrientTo::AtStart(void)
{
	this->m_CurrentState = eStates::STEP_1_DETERMINE_VELOCITY;
	this->rotation = glm::vec3(0);
	this->rotationLeft = glm::vec3(0);
}

void cOrientTo::AtEnd(void)
{
	this->m_CurrentState = eStates::ALL_DONE;
	return;
}
bool cOrientTo::Update(double deltaTime)
{
	switch (this->m_CurrentState)
	{
	case STEP_1_DETERMINE_VELOCITY:
	{
		const float ROTATION_SPEED = 1.1f;	// 2 degrees per second
		float AdjustAngleDelta = ROTATION_SPEED * deltaTime;
		this->eulerDir -= AdjustAngleDelta / 10;
		rotationLeft -= (rotationLeft / 50.f);
		m_Beholder->qRotation *= glm::quat(glm::radians(rotation/50.f));
		if (rotationLeft.x < 0.1f && rotationLeft.y < 0.1f && rotationLeft.z < 0.1f)
		{
			// Yes, so go to the next state
			this->m_CurrentState = ALL_DONE;
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

bool cOrientTo::isComplete(void)
{
	if (this->m_CurrentState == ALL_DONE)
	{
		return true;
	}
	return false;
}