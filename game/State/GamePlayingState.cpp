#include "GamePlayingState.h"

#include "Animator.h"
#include "DamageCommand.h"
#include "GameManagerComponent.h"
#include "GridRendererComponent.h"
#include "HealthComponent.h"
#include "IntUIComponent.h"
#include "Kobengine.h"
#include "LevelComponent.h"
#include "HealthUIManager.h"
#include "IngredientComponent.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "MovementComponent.h"
#include "EnemyAILogicComponent.h"
#include "InputManager.h"
#include "PlayingCommands.h"
#include "RespawnComponent.h"
#include "ScoreCommand.h"
#include "Timer.h"

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
	if (m_ResetDelay > 0)
	{
		m_ResetDelay -= kob::Timer::GetDeltaSeconds();
		if (m_ResetDelay <= 0)
			ResetCurrentLevel();
	}

	if (m_EndGame)
		return GetGameManager()->LeaderboardState();
	return nullptr;
}

void bt::GamePlayingState::OnEnter()
{
	m_EndGame = false;
	LoadNextLevel(1);
	SetupPlayers();
}

void bt::GamePlayingState::OnExit()
{
	m_EndGame = true;
	m_pLevelObject->FlagForDeletion();
	m_pLevelObject = nullptr;
	kob::InputManager::GetInstance().UnregisterAll();
}

void bt::GamePlayingState::PlayerDied()
{
	auto enemies = m_pLevelObject->GetScene().GetObjectsByTag("Enemy");
	for (auto& e : enemies)
	{
		auto comp = e->GetComponent<MovementComponent>();
		comp->Immobilize();
	}

	auto players = m_pLevelObject->GetScene().GetObjectsByTag("Player");
	for (auto& p : players)
	{
		auto comp = p->GetComponent<kob::Animator>();
		auto move = p->GetComponent<MovementComponent>();
		move->Immobilize(false);
		comp->Play("Death", false);
	}
	m_ResetDelay = 3.f;
}
void bt::GamePlayingState::ResetCurrentLevel()
{
	auto players = m_pLevelObject->GetScene().GetObjectsByTag("Player");
	LevelComponent* pLvl = m_pLevelObject->GetComponent<LevelComponent>();
	for (auto& p : players)
	{
		p->SetParent(m_pLevelObject, true);
		p->SetLocalPosition({ pLvl->GetChefSpawn(), 0.f });
		auto move = p->GetComponent<MovementComponent>();
		move->Mobilize();
		auto anim = p->GetComponent<kob::Animator>();
		anim->Play("Down", false);
		anim->Stop(1);
	}

	auto enemies = m_pLevelObject->GetScene().GetObjectsByTag("Enemy");
	for (auto& e : enemies)
	{
		e->SetParent(m_pLevelObject, true);
		auto move = e->GetComponent<MovementComponent>();
		move->Mobilize();
		auto comp = e->GetComponent<RespawnComponent>();
		comp->ImmediateRespawn();
	}

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
	auto levelComp = m_pLevelObject->AddComponent<LevelComponent>("./assets/level/Level0" + std::to_string(id) + ".csv");
	levelComp->SpawnTileMap();

	// Debug Grid
#ifdef _DEBUG
	auto ws = kob::Kobengine::GetWindowSize();
	auto colRow = levelComp->GetColRow();
	auto& grid = scene.AddEmpty("Debug Grid");
	grid.SetRenderPriority(0);
	grid.SetParent(m_pLevelObject);
	grid.AddComponent<kob::GridRendererComponent>(glm::vec2{ ws.x, ws.y }, colRow.y, colRow.x);
#endif

	// Hook Up Ingredients
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
void bt::GamePlayingState::SetupPlayers()
{
	auto levelComp = m_pLevelObject->GetComponent<LevelComponent>();
	switch (GetGameManager()->gameMode)
	{
	case GameMode::Solo:
		{
			m_vPlayers.push_back(levelComp->SpawnChef("MrPepper", "characters/ChefSheet.png"));
			SetupUI();
			break;
		}
	case GameMode::Versus:
		{
			m_vPlayers.push_back(levelComp->SpawnChef("MrPepper", "characters/ChefSheet.png"));
			m_vPlayers.push_back(levelComp->GetGameObject()->GetScene().GetObjectsByName("HotDog").front());
			m_vPlayers.back()->RemoveComponent<EnemyAILogicComponent>();
			break;
		}
	case GameMode::CoOp:
		{
			m_vPlayers.push_back(levelComp->SpawnChef("MrPepper", "characters/ChefSheet.png"));
			m_vPlayers.push_back(levelComp->SpawnChef("MrsPepper", "characters/MsChefSheet.png"));
			SetupUI();
			break;
		}
	}

	int idx{};
	for (const auto& player : m_vPlayers)
	{
		if (const auto hp = player->GetComponent<HealthComponent>())
		{
			hp->OnDeath() += &m_EventEndGame;
			hp->OnDamageTaken() += &m_EventPlayerDied;
		}

		auto& inputManager = kob::InputManager::GetInstance();
		const auto movement = player->GetComponent<MovementComponent>();
		const auto pepper = player->GetComponent<ThrowPepperComponent>();
		if (idx == 0)
		{
			inputManager.RegisterKeyboardCmd(SDLK_w, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0, -1, 0 }));
			inputManager.RegisterKeyboardCmd(SDLK_s, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0,  1, 0 }));
			inputManager.RegisterKeyboardCmd(SDLK_d, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 1,  0, 0 }));
			inputManager.RegisterKeyboardCmd(SDLK_a, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ -1,  0, 0 }));
			if (pepper)
				inputManager.RegisterKeyboardCmd(SDLK_SPACE, kob::TriggerState::Pressed, std::make_unique<ThrowPepperCommand>(*movement, *pepper, levelComp->GetTileSize()));

			//if (!inputManager.GetGamepad(0))
			//	inputManager.RegisterGamepad();
			//inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_UP, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0, -1, 0 }), 0);
			//inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_DOWN, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0,  1, 0 }), 0);
			//inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_RIGHT, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 1,  0, 0 }), 0);
			//inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_LEFT, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ -1,  0, 0 }), 0);
			//inputManager.RegisterGamepadCmd(kob::Gamepad::Button::A, kob::TriggerState::Pressed, std::make_unique<ThrowPepperCommand>(*movement, *pepper, levelComp->GetTileSize()), 0);
		}
		else if (idx == 1)
		{
			if (!inputManager.GetGamepad(0))
				inputManager.RegisterGamepad();
			inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_UP, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0, -1, 0 }), 0);
			inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_DOWN, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 0,  1, 0 }), 0);
			inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_RIGHT, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ 1,  0, 0 }), 0);
			inputManager.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_LEFT, kob::TriggerState::Down, std::make_unique<MoveCommand>(*movement, glm::vec3{ -1,  0, 0 }), 0);
			if (pepper)
				inputManager.RegisterGamepadCmd(kob::Gamepad::Button::A, kob::TriggerState::Pressed, std::make_unique<ThrowPepperCommand>(*movement, *pepper, levelComp->GetTileSize()), 0);
		}
		++idx;
	}
}
void bt::GamePlayingState::SetupUI()
{
	auto pepperFont = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 14);
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	auto ws = kob::Kobengine::GetWindowSize();

	// Pepper
	auto& pepperUI = scene.AddEmpty("PepperUI");
	pepperUI.SetParent(m_pLevelObject);
	pepperUI.AddComponent<kob::ImageRendererComponent>("ui/UIElements.png", glm::vec4{ 104, 8, 24, 8 });
	pepperUI.SetLocalPosition({ ws.x - 35, 10, 0 });
	pepperUI.SetLocalScale({ 2, 2, 2 });
	pepperUI.SetRenderPriority(-1);

	int pIdx = 0;
	for (const auto& p : m_vPlayers)
	{
		const auto pepper = p->GetComponent<ThrowPepperComponent>();
		const auto hp = p->GetComponent<HealthComponent>();

		// Health
		if (hp)
		{
			auto& healthUI = scene.AddEmpty("HealthUI");
			healthUI.SetParent(m_pLevelObject);
			healthUI.AddComponent<HealthUIManager>(*hp);
			healthUI.SetLocalPosition({ 8.f * (pIdx * kob::Kobengine::GetWindowSize().x / 8 - (pIdx * 2 - 1)), static_cast<float>(kob::Kobengine::GetWindowSize().y) - 12.f, 0 });
			healthUI.SetLocalScale({ 2, 2, 2 });
			healthUI.SetRenderPriority(-1);
		}

		// pepper
		if (pepper)
		{
			auto& pepperUICount = scene.AddEmpty("PepperUICount");
			pepperUICount.SetParent(m_pLevelObject);
			pepperUICount.AddComponent<kob::TextRendererComponent>("pepper", pepperFont);
			pepperUICount.AddComponent<kob::IntUIComponent>("", pepper->GetPepper(), "", pepper->OnPepperChange);
			pepperUICount.SetLocalPosition({ ws.x - 45 * (pIdx + 1), 30, 0 });
			pepperUICount.SetRenderPriority(-1);
		}

		++pIdx;
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
