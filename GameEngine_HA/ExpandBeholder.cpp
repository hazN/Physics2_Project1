#include "ExpandBeholder.h"


#include "globalThings.h"		// for getRandom()
#include "MoveTo.h"


ExpandBeholder::ExpandBeholder(cMeshObject* Beholder, float size)
{
	this->m_Beholder = Beholder;
	this->size = size;
	this->m_CurrentState = STEP_1_DETERMINE_VELOCITY;
}



void ExpandBeholder::AtStart(void)
{
	this->m_CurrentState = eStates::STEP_1_DETERMINE_VELOCITY;
}

void ExpandBeholder::AtEnd(void)
{
	this->m_CurrentState = eStates::ALL_DONE;
	return;
}
bool ExpandBeholder::Update(double deltaTime)
{
	switch (this->m_CurrentState)
	{
	case STEP_1_DETERMINE_VELOCITY:
	{
		// This means we set it ourselves
		m_Beholder->SetUniformScale((size - m_Beholder->scaleXYZ.x) / 50.f);
		if (m_Beholder->scaleXYZ.x >= size)
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

bool ExpandBeholder::isComplete(void)
{
	if (this->m_CurrentState == ALL_DONE)
	{
		return true;
	}
	return false;
}