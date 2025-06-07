#include "GameMenuState.h"
#include "GameManagerComponent.h"
#include "ImageRendererComponent.h"
#include "InputManager.h"
#include "Kobengine.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "TextRendererComponent.h"
#include "StateCommands.h"

//--------------------------------------------------
//    Constructor & Destructor
//--------------------------------------------------
bt::GameMenuState::GameMenuState(GameManagerComponent& gameManagerComp)
	: IGameState(gameManagerComp)
{
	auto& scene = GetGameManager()->GetParent()->GetScene();
	const auto windowSize = kob::Kobengine::GetWindowSize();

	m_pMenuObject = &scene.AddEmpty("StartMenu");
	m_pMenuObject->SetParent(GetGameManager()->GetParent());

	//Background
	auto& background = scene.AddEmpty("Background");
	background.SetParent(m_pMenuObject);
	background.SetLocalPosition({ windowSize.x / 2, windowSize.y / 2, 0});
	const auto irc = background.AddComponent<kob::ImageRendererComponent>("background.tga");
	const auto ircSize = irc->GetSize();
	background.SetLocalScale({ static_cast<float>(windowSize.x) / ircSize.x, static_cast<float>(windowSize.y) / ircSize.y, 1});

	//Text
	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 11);
	auto fontL = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 16);

	// Bean
	auto* info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("The HotDog has been disabled for now!", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.30f, 0));

	// Chef
	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("Use WASD to move the Chef!", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.48f, 0));
	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("C to inflict damage, Z and X to gain score", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.54f, 0));

	// Info
	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("Press A/Space to start!", fontL);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.72f, 0));

	m_pMenuObject->SetActive(false);
}


//--------------------------------------------------
//    Interface
//--------------------------------------------------
bt::IGameState* bt::GameMenuState::Update()
{
	if (m_GoToPlayMode)
	{
		return GetGameManager()->PlayState();
	}
	return nullptr;
}

void bt::GameMenuState::OnEnter()
{
	kob::ServiceLocator::GetSoundService().Play("sound/BGM.wav", 0.25f, -1);

	m_pMenuObject->SetActive(true);
	m_GoToPlayMode = false;

	kob::InputManager::GetInstance().RegisterGamepad();
	kob::InputManager::GetInstance().RegisterGamepadCmd(
		kob::Gamepad::Button::A, kob::TriggerState::Pressed,
		std::make_unique<StartGameCommand>(*this), 0);

	kob::InputManager::GetInstance().RegisterKeyboardCmd(
		SDLK_SPACE, kob::TriggerState::Pressed,
		std::make_unique<StartGameCommand>(*this));
}

void bt::GameMenuState::OnExit()
{
	m_pMenuObject->SetActive(false);
	m_GoToPlayMode = true;

	kob::InputManager::GetInstance().UnregisterGamepadBtn(kob::Gamepad::Button::A, 0);
	kob::InputManager::GetInstance().UnregisterKeyboardKey(SDLK_SPACE);
}


//--------------------------------------------------
//    Extra
//--------------------------------------------------
void bt::GameMenuState::GoToPlayMode()
{
	m_GoToPlayMode = true;
}
