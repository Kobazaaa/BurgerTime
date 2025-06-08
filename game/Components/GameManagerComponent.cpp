#include "GameManagerComponent.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::GameManagerComponent::GameManagerComponent(kob::GameObject& parent)
	: Component(parent)
{
	m_pGameMenuState = std::make_unique<GameMenuState>(*this);
	m_pGamePlayingState = std::make_unique<GamePlayingState>(*this);
	m_pGameLeaderboardState = std::make_unique<GameLeaderboardState>(*this);
}


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::GameManagerComponent::Start()
{
	m_pCurrentState = m_pGameMenuState.get();
	m_pCurrentState->OnEnter();
}
void bt::GameManagerComponent::Update()
{
	if (!m_pCurrentState)
		return;

	if (const auto pState = m_pCurrentState->Update())
	{
		m_pCurrentState->OnExit();
		m_pCurrentState = pState;
		m_pCurrentState->OnEnter();
	}
}


//--------------------------------------------------
//    Accessors & Mutators
//--------------------------------------------------
bt::GameMenuState* bt::GameManagerComponent::MenuState()				const { return m_pGameMenuState.get(); }
bt::GamePlayingState* bt::GameManagerComponent::PlayState()				const { return m_pGamePlayingState.get(); }
bt::GameLeaderboardState* bt::GameManagerComponent::LeaderboardState()	const { return m_pGameLeaderboardState.get(); }

//--------------------------------------------------
//    Events
//--------------------------------------------------
