#include "GamePlayingState.h"
#include "GameManagerComponent.h"
#include "GridRendererComponent.h"
#include "HealthComponent.h"
#include "IntUIComponent.h"
#include "Kobengine.h"
#include "LevelComponent.h"
#include "IngredientComponent.h"
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
	LoadNextLevel(1);
}

void bt::GamePlayingState::OnExit()
{
	m_EndGame = true;
	m_pLevelObject->FlagForDeletion();
}

void bt::GamePlayingState::LoadNextLevel(int id)
{
	if (m_pLevelObject)
		m_pLevelObject->FlagForDeletion();

	// Level
	m_NextLevelID = id + 1;
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	m_pLevelObject = &scene.AddEmpty("Level" + std::to_string(id));
	m_pLevelObject->SetParent(GetGameManager()->GetGameObject());
	auto comp = m_pLevelObject->AddComponent<LevelComponent>("./assets/level/Level0" + std::to_string(id) + ".csv");
	comp->SpawnTileMap();

	// Health
	const auto hp = m_pLevelObject->GetChildAt(m_pLevelObject->GetChildCount() - 1)->GetComponent<HealthComponent>();
	hp->OnDeath() += &m_EventEndGame;
	auto& healthChanged = hp->OnHealthChanged();

	// Debug Grid
	auto ws = kob::Kobengine::GetWindowSize();
	auto colRow = comp->GetColRow();
	auto& grid = scene.AddEmpty("Debug Grid");
	grid.SetRenderPriority(0);
	grid.SetParent(m_pLevelObject);
	grid.AddComponent<kob::GridRendererComponent>(glm::vec2{ ws.x, ws.y }, colRow.y, colRow.x);

	// UI
	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 11);

	auto& tempUI = scene.AddEmpty("HealthUI");
	tempUI.SetParent(m_pLevelObject);
	tempUI.AddComponent<kob::TextRendererComponent>("lives", fontS);
	tempUI.AddComponent<kob::IntUIComponent>("Lives:", hp->GetMaxLives(), "", healthChanged);
	tempUI.SetLocalPosition({ 50, 10, 0 });

	// s
	const auto ingredients = m_pLevelObject->GetScene().GetObjectsByTag("Ingredient");
	m_CompletedIngredientCount = 0;
	m_IngredientCount = static_cast<int>(ingredients.size());
	for (auto& ingredient : ingredients)
	{
		const auto ingredientComponent = ingredient->GetComponent<IngredientComponent>();
		if (!ingredientComponent) continue;
		ingredientComponent->OnPlateReached += &m_EventIngredientComplete;
	}
}
void bt::GamePlayingState::OnIngredientCompleted()
{
	++m_CompletedIngredientCount;
	if (m_IngredientCount == m_CompletedIngredientCount)
		LoadNextLevel(m_NextLevelID);
}
void bt::GamePlayingState::EndGame()
{
	m_EndGame = true;
}
