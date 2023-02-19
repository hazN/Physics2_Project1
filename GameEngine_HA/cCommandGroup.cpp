#include "cCommandScheduler.h"

cCommandGroup::cCommandGroup()
{

}

void cCommandGroup::AddSerialCommand(cCommand* pSerialCommand)
{
	vecSerialCommands.push_back(pSerialCommand);
}

void cCommandGroup::AddParallelCommand(cCommand* pParallelCommand)
{
	vecParallelCommands.push_back(pParallelCommand);
}

void cCommandGroup::AtStart(void)
{
	return;
}

void cCommandGroup::AtEnd(void)
{
	return;
}

// * Run the current serial command (if any)
// * Run ALL the parallel commands (if any)
void cCommandGroup::Update(double deltaTime)
{
	cCommand* lastCommand = nullptr;
	for (cCommand* command : vecParallelCommands)
	{
		command->Update(deltaTime);
	}
	if (vecSerialCommands.size() > 0)
	{
		vecSerialCommands[0]->Update(deltaTime);
		if (vecSerialCommands[0]->isComplete())
		{
			vecSerialCommands.erase(vecSerialCommands.begin());
		}
	}
	return;
}


// Returns true only when ALL the serial commands are done and ALL the parallel commands are done
bool cCommandGroup::isDone(void)
{
	return true;
}
