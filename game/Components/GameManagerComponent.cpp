#include "GameManagerComponent.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::GameManagerComponent::GameManagerComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::GameManagerComponent::Update()
{
	if (!m_pCurrentState)
		return;

	if (auto pState = m_pCurrentState->Update())
	{
		m_pCurrentState->OnExit();
		m_pCurrentState = pState;
		m_pCurrentState->OnEnter();
	}
}
