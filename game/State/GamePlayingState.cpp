#include "GamePlayingState.h"

#include "Animator.h"
#include "GameManagerComponent.h"
#include "GridRendererComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
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
#include "ServiceLocator.h"
#include "SquashableComponent.h"
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
	if (m_PlayerDiedResetDelay > 0)
	{
		m_PlayerDiedResetDelay -= kob::Timer::GetDeltaSeconds();
		if (m_PlayerDiedResetDelay <= 0)
			ResetCurrentLevel();
	}
	if (m_LevelClearedDelay > 0)
	{
		m_LevelClearedDelay -= kob::Timer::GetDeltaSeconds();
		if (m_LevelClearedDelay <= 0)
			LoadNextLevel(m_NextLevelID);
	}
	if (m_EndGameDelay > 0)
	{
		m_EndGameDelay -= kob::Timer::GetDeltaSeconds();
		if (m_EndGameDelay <= 0)
			return GetGameManager()->LeaderboardState();
	}
	return nullptr;
}

void bt::GamePlayingState::OnEnter()
{
	m_EndGameDelay = -1;
	m_LevelClearedDelay = -1;
	m_LevelClearedDelay = -1;
	LoadNextLevel(1);
	SetupPlayers();
}

void bt::GamePlayingState::OnExit()
{
	m_pLevelObject->FlagForDeletion();
	m_pLevelObject = nullptr;
	for (auto& p : m_vPlayers)
		p->FlagForDeletion();
	m_vPlayers.clear();
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
	m_PlayerDiedResetDelay = 3.f;
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

	// music
	kob::ServiceLocator::GetSoundService().Play("sound/BGM.wav", 1.f, -1);
}
void bt::GamePlayingState::LoadNextLevel(int id)
{
	// un parent players to prevent deletion
	for (const auto& p : m_vPlayers)
		p->SetParent(nullptr, true);

	// delete existing
	if (m_pLevelObject)
	{
		
		m_pLevelObject->FlagForDeletion();
	}

	// Level
	m_NextLevelID = (id % m_MaxLevels) + 1;
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
	auto scoreComp = GetGameManager()->GetGameObject()->GetComponent<ScoreComponent>();
	const auto ingredients = m_pLevelObject->GetScene().GetObjectsByTag("Ingredient");
	m_CompletedIngredientCount = 0;
	m_IngredientCount = static_cast<int>(ingredients.size());
	for (auto& ingredient : ingredients)
	{
		const auto ingredientComponent = ingredient->GetComponent<IngredientComponent>();
		if (!ingredientComponent) continue;
		ingredientComponent->OnPlateReached += &m_EventIngredientComplete;
		ingredientComponent->OnScoreGained += std::bind(&ScoreComponent::AddScore, scoreComp, std::placeholders::_1);
	}

	// hook up players
	if (!m_vPlayers.empty())
	{
		const auto pLvl = m_pLevelObject->GetComponent<LevelComponent>();
		if (GetGameManager()->gameMode == GameMode::Versus)
		{
			m_vPlayers[0]->SetLocalPosition({ pLvl->GetChefSpawn() , 0.f });
			m_vPlayers[1]->SetLocalPosition(pLvl->GetGameObject()->GetScene().GetObjectsByName("HotDog").front()->GetLocalTransform().GetPosition());
		}
		else
		{
			for (const auto& p : m_vPlayers)
				p->SetLocalPosition({ pLvl->GetChefSpawn() , 0.f });
		}
		for (const auto& p : m_vPlayers)
		{
			p->SetParent(m_pLevelObject, true);
			if (auto move = p->GetComponent<MovementComponent>())
			{
				move->SetCurrentLevel(*pLvl);
				move->Mobilize();
			}
		}
	}

	// hook up score
	const auto enemies = m_pLevelObject->GetScene().GetObjectsByTag("Enemy");
	for (auto& e : enemies)
	{
		if (auto comp = e->GetComponent<SquashableComponent>())
			comp->OnSquashedScored += std::bind(&ScoreComponent::AddScore, scoreComp, std::placeholders::_1);
	}

	// music
	kob::ServiceLocator::GetSoundService().Play("sound/BGM.wav", 1.f, -1);
}
void bt::GamePlayingState::SetupPlayers()
{
	auto levelComp = m_pLevelObject->GetComponent<LevelComponent>();
	switch (GetGameManager()->gameMode)
	{
	case GameMode::Solo:
		{
			m_vPlayers.push_back(levelComp->SpawnChef("MrPepper", "characters/ChefSheet.png"));
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
			break;
		}
	}
	SetupUI();

	int idx{};
	auto& inputManager = kob::InputManager::GetInstance();
	for (const auto& player : m_vPlayers)
	{
		if (const auto hp = player->GetComponent<HealthComponent>())
		{
			hp->OnDeath() += &m_EventEndGame;
			hp->OnDamageTaken() += &m_EventPlayerDied;
		}

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

	// general input
	inputManager.RegisterKeyboardCmd(SDLK_F1, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { kob::ServiceLocator::GetSoundService().StopAll(); LoadNextLevel(m_NextLevelID); }));
	inputManager.RegisterKeyboardCmd(SDLK_F2, kob::TriggerState::Pressed, std::make_unique<ToggleMuteSoundCommand>());
}
void bt::GamePlayingState::SetupUI() const
{
	auto uiFont = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 14);
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	auto ws = kob::Kobengine::GetWindowSize();

	// score
	if (auto scoreComp = GetGameManager()->GetGameObject()->GetComponent<ScoreComponent>())
	{
		auto& scoreUI = scene.AddEmpty("ScoreUI");
		scoreUI.SetParent(m_pLevelObject);
		scoreUI.AddComponent<kob::TextRendererComponent>("1UP", uiFont);
		scoreUI.SetLocalPosition({ 55, 10, 0 });
		scoreUI.SetRenderPriority(-1);

		auto& scoreUICount = scene.AddEmpty("ScoreUICount");
		scoreUICount.SetParent(m_pLevelObject);
		scoreUICount.AddComponent<kob::TextRendererComponent>("pepper", uiFont);
		scoreUICount.AddComponent<kob::IntUIComponent>("", 0, "", scoreComp->OnScoreChanged);
		scoreUICount.SetLocalPosition({ 65, 25, 0 });
		scoreUICount.SetRenderPriority(-1);
	}

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
		// Health
		if (const auto hp = p->GetComponent<HealthComponent>())
		{
			auto& healthUI = scene.AddEmpty("HealthUI");
			healthUI.SetParent(m_pLevelObject);
			healthUI.AddComponent<HealthUIManager>(*hp);
			healthUI.SetLocalPosition({ 8.f * (pIdx * kob::Kobengine::GetWindowSize().x / 8 - (pIdx * 2 - 1)), static_cast<float>(kob::Kobengine::GetWindowSize().y) - 12.f, 0 });
			healthUI.SetLocalScale({ 2, 2, 2 });
			healthUI.SetRenderPriority(-1);
		}

		// pepper
		if (const auto pepper = p->GetComponent<ThrowPepperComponent>())
		{
			auto& pepperUICount = scene.AddEmpty("PepperUICount");
			pepperUICount.SetParent(m_pLevelObject);
			pepperUICount.AddComponent<kob::TextRendererComponent>("pepper", uiFont);
			pepperUICount.AddComponent<kob::IntUIComponent>("", pepper->GetPepper(), "", pepper->OnPepperChange);
			pepperUICount.SetLocalPosition({ ws.x - 45 * (pIdx + 1), 30, 0 });
			pepperUICount.SetRenderPriority(-1);
		}
		++pIdx;
	}
}
void bt::GamePlayingState::OnIngredientCompleted()
{
	++m_IngredientCount;
	if (m_IngredientCount == m_CompletedIngredientCount)
	{
		kob::ServiceLocator::GetSoundService().StopAll();
		kob::ServiceLocator::GetSoundService().Play("sound/Round Clear.wav", 1.f);
		for (const auto& p : m_vPlayers)
		{
			if (const auto comp = p->GetComponent<MovementComponent>())
				comp->Immobilize();
			if (const auto anim = p->GetComponent<kob::Animator>())
				anim->Play("Celebrate", true);
		}
		auto enemies = m_pLevelObject->GetScene().GetObjectsByTag("Enemy");
		for (const auto& e : enemies)
		{
			const auto comp = e->GetComponent<MovementComponent>();
			comp->Immobilize();
		}
		m_LevelClearedDelay = 4.f;
	}
}
void bt::GamePlayingState::EndGame()
{
	m_EndGameDelay = 2.5f;
}
