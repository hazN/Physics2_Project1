#include "GoInvisible.h"
#include "globalThings.h"		// for getRandom()


GoInvisible::GoInvisible(cMeshObject* Beholder)
{
	this->m_Beholder = Beholder;
	this->size = size;
	this->m_CurrentState = STEP_1_DETERMINE_VELOCITY;
}



void GoInvisible::AtStart(void)
{
	this->m_CurrentState = eStates::STEP_1_DETERMINE_VELOCITY;
}

void GoInvisible::AtEnd(void)
{
	this->m_CurrentState = eStates::ALL_DONE;
	return;
}
bool GoInvisible::Update(double deltaTime)
{
	switch (this->m_CurrentState)
	{
	case STEP_1_DETERMINE_VELOCITY:
	{
		// This means we set it ourselves
		m_Beholder->RGBA_colour.w -= 0.01f;
		if (m_Beholder->RGBA_colour.w <= 0.f)
		{
			m_CurrentState = ALL_DONE;
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

bool GoInvisible::isComplete(void)
{
	if (this->m_CurrentState == ALL_DONE)
	{
		return true;
	}
	return false;
}