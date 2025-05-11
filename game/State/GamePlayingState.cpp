#include "GamePlayingState.h"
#include "GameManagerComponent.h"
#include "GridRendererComponent.h"
#include "HealthComponent.h"
#include "IntUIComponent.h"
#include "Kobengine.h"
#include "LevelComponent.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"

//--------------------------------------------------
//    Constructor & Destructor
//--------------------------------------------------
bt::GamePlayingState::GamePlayingState(GameManagerComponent& gameManagerComp)
	: IGameState(gameManagerComp)
{

}


//--------------------------------------------------
//    Interface
//--------------------------------------------------
bt::IGameState* bt::GamePlayingState::Update()
{
	if (m_EndGame)
		return GetGameManager()->LeaderboardState();
	return nullptr;
}

void bt::GamePlayingState::OnEnter()
{
	m_EndGame = false;

	// Level
	auto& scene = GetGameManager()->GetParent()->GetScene();
	m_pLevelObject = &scene.AddEmpty("Level");
	m_pLevelObject->SetParent(GetGameManager()->GetParent());
	auto comp = m_pLevelObject->AddComponent<LevelComponent>("./assets/level/Level01.csv");
	comp->SpawnTileMap();

	// Health
	const auto hp = m_pLevelObject->GetChildAt(m_pLevelObject->GetChildCount() - 1)->GetComponent<HealthComponent>();
	hp->OnDeath() += &m_EventEndGame;
	auto& healthChanged = hp->OnHealthChanged();

	// Debug Grid
	auto ws = kob::Kobengine::GetWindowSize();
	auto colRow = comp->GetColRow();
	auto& grid = scene.AddEmpty("Debug Grid");
	grid.SetParent(m_pLevelObject);
	grid.AddComponent<kob::GridRendererComponent>(glm::vec2{ ws.x, ws.y }, colRow.y, colRow.x);

	// UI
	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 11);

	auto& tempUI = scene.AddEmpty("HealthUI");
	tempUI.SetParent(m_pLevelObject);
	tempUI.AddComponent<kob::TextRendererComponent>("lives", fontS);
	tempUI.AddComponent<kob::IntUIComponent>("Lives:", hp->GetMaxLives(), "", healthChanged);
	tempUI.SetLocalPosition({ 50, 10, 0});
}

void bt::GamePlayingState::OnExit()
{
	m_EndGame = true;
	m_pLevelObject->FlagForDeletion();
}

void bt::GamePlayingState::EndGame()
{
	m_EndGame = true;
}
