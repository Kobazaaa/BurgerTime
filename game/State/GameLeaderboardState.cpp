#include "GameLeaderboardState.h"
#include <fstream>
#include "GameManagerComponent.h"
#include "ImageRendererComponent.h"
#include "InputManager.h"
#include "Kobengine.h"
#include "ResourceManager.h"
#include "ScoreComponent.h"
#include "TextRendererComponent.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor & Destructor
//--------------------------------------------------
bt::GameLeaderboardState::GameLeaderboardState(GameManagerComponent& gameManagerComp)
	: IGameState(gameManagerComp)
{
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	const auto windowSize = kob::Kobengine::GetWindowSize();

	m_pLeaderboardObject = &scene.AddEmpty("StartMenu");
	m_pLeaderboardObject->SetParent(GetGameManager()->GetGameObject());

	//Text
	auto fontL = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 16);

	// Info
	auto* info = &scene.AddEmpty();
	info->SetParent(m_pLeaderboardObject);
	auto menu = info->AddComponent<MenuComponent>(*fontL);
	m_pMenuComponent = menu;

	const std::vector<std::string> labels = 
	{	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
		"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T",
		"U", "V", "W", "X", "Y", "Z"};
	constexpr int cols = 10;
	constexpr float startX = 50.f;
	constexpr float startY = 50.f;
	constexpr float stepY = 75.f;
	constexpr float stepX = 50.f;
	for (size_t i = 0; i < labels.size(); ++i)
	{
		size_t col = i % cols;
		size_t row = i / cols;

		float posX = startX + col * stepX;
		float posY = startY + row * stepY;

		menu->AddOption(labels[i], { posX, posY }, [&] { SelectInput(); });
	}
	menu->SetSelectorOffset({ 0, -25 });

	auto& selector = scene.AddEmpty("MenuSelector");
	selector.SetParent(m_pLeaderboardObject);
	selector.AddComponent<kob::ImageRendererComponent>("characters/ChefSheet.png", glm::vec4{ 16, 16, 16, 16 });
	selector.SetLocalScale({ 2, 2, 2 });
	menu->SetSelector(&selector);

	m_pLeaderboardObject->SetActive(false);
}


//--------------------------------------------------
//    Interface
//--------------------------------------------------
bt::IGameState* bt::GameLeaderboardState::Update()
{
	if (m_Timer > 0)
	{
		m_Timer -= kob::Timer::GetDeltaSeconds();
		if (m_Timer <= 0.f)
			return GetGameManager()->MenuState();
	}
	return nullptr;
}

void bt::GameLeaderboardState::OnEnter()
{
	m_Timer = -1;
	m_pLeaderboardObject->SetActive(true);

	auto& im = kob::InputManager::GetInstance();
	im.RegisterGamepad();

	// menu navigation
	im.RegisterKeyboardCmd(SDLK_UP, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-10); }));
	im.RegisterKeyboardCmd(SDLK_RIGHT, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(1); }));
	im.RegisterKeyboardCmd(SDLK_DOWN, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(10);  }));
	im.RegisterKeyboardCmd(SDLK_LEFT, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-1);  }));
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_UP, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-10); }), 0);
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_RIGHT, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(1); }), 0);
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_DOWN, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(10);  }), 0);
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_LEFT, kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-1);  }), 0);

	// Select
	im.RegisterGamepadCmd(
		kob::Gamepad::Button::A,
		kob::TriggerState::Pressed,
		std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->SelectOption(); }), 0);
	im.RegisterKeyboardCmd(
		SDLK_SPACE,
		kob::TriggerState::Pressed,
		std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->SelectOption(); }));

	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 12);
	auto fontL = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 16);
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	auto& names = scene.AddEmpty("Names");
	names.SetParent(m_pLeaderboardObject);
	names.AddComponent<kob::TextRendererComponent>("NAME", fontL);
	names.SetLocalPosition({ 50, 250, 0 });

	auto& scores = scene.AddEmpty("Scores");
	scores.SetParent(m_pLeaderboardObject);
	scores.AddComponent<kob::TextRendererComponent>("SCORE", fontL);
	scores.SetLocalPosition({ 250, 250, 0 });

	LoadTopScores(5);
	int idx = 0;
	for (auto& [name, score] : m_vLoadedHighScores)
	{
		float yOffset = 250.f + (idx + 1) * 25.f;

		auto& nameGO = scene.AddEmpty(name);
		nameGO.SetParent(m_pLeaderboardObject);
		nameGO.AddComponent<kob::TextRendererComponent>(name, fontS);
		nameGO.SetLocalPosition({ 50, yOffset, 0});

		auto& scoreGO = scene.AddEmpty(std::to_string(score));
		scoreGO.SetParent(m_pLeaderboardObject);
		scoreGO.AddComponent<kob::TextRendererComponent>(std::to_string(score), fontS);
		scoreGO.SetLocalPosition({ 250, yOffset, 0 });
		++idx;
	}

	m_NewValue.score = GetGameManager()->GetGameObject()->GetComponent<ScoreComponent>()->GetScore();
	auto& name = scene.AddEmpty(m_NewValue.name);
	name.SetParent(m_pLeaderboardObject);
	m_pNameComp = name.AddComponent<kob::TextRendererComponent>("XXX", fontS);
	name.SetLocalPosition({ 50, 400, 0 });

	auto& score = scene.AddEmpty(std::to_string(m_NewValue.score));
	score.SetParent(m_pLeaderboardObject);
	score.AddComponent<kob::TextRendererComponent>(std::to_string(m_NewValue.score), fontS);
	score.SetLocalPosition({ 250, 400, 0 });


}

void bt::GameLeaderboardState::OnExit()
{
	m_pLeaderboardObject->SetActive(false);
	m_pNameComp = nullptr;
	m_NewValue = {};
	kob::InputManager::GetInstance().UnregisterAll();
}


//--------------------------------------------------
//    State
//--------------------------------------------------
void bt::GameLeaderboardState::SelectInput()
{
	if (m_Timer > 0)
		return;
	m_NewValue.name += m_pMenuComponent->GetCurrentlySelectedOption().label;
	m_pNameComp->SetText(m_NewValue.name);
	if (m_NewValue.name.size() >= 3)
	{
		SaveScore();
		m_Timer = m_ExistDelay;
	}
}
void bt::GameLeaderboardState::SaveScore() const
{
	std::vector<ScoreEntry> scores;
	std::ifstream inFile(m_FileName);
	std::string line;
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		ScoreEntry entry;
		iss >> entry.name >> entry.score;
		if (!entry.name.empty())
			scores.push_back(entry);
	}
	inFile.close();
	scores.push_back(m_NewValue);

	std::ranges::sort(scores, [](const ScoreEntry& a, const ScoreEntry& b)
		{
			return a.score > b.score;
		});

	std::ofstream outFile(m_FileName, std::ios::trunc);
	for (const auto& entry : scores)
		outFile << entry.name << " " << entry.score << "\n";

	outFile.close();
}

void bt::GameLeaderboardState::LoadTopScores(size_t count)
{
	m_vLoadedHighScores.clear();
	std::ifstream inFile(m_FileName);
	std::string line;

	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		ScoreEntry entry;
		iss >> entry.name >> entry.score;
		if (!entry.name.empty())
			m_vLoadedHighScores.push_back(entry);
	}

	std::ranges::sort(m_vLoadedHighScores, [](const ScoreEntry& a, const ScoreEntry& b)
		{
			return a.score > b.score;
		});

	if (m_vLoadedHighScores.size() > count)
		m_vLoadedHighScores.resize(count);
}
