#include "StateCommands.h"
#include "GameMenuState.h"

bt::StartGameCommand::StartGameCommand(GameMenuState& menuState)
	: m_pMenuState(&menuState)
{ }
void bt::StartGameCommand::Execute()
{
	m_pMenuState->GoToPlayMode();
}

