#include "GameMenuState.h"
#include <iostream>
#include "GameManagerComponent.h"
#include "ImageRendererComponent.h"
#include "InputManager.h"
#include "Kobengine.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "TextRendererComponent.h"
#include "MenuComponent.h"

//--------------------------------------------------
//    Constructor & Destructor
//--------------------------------------------------
bt::GameMenuState::GameMenuState(GameManagerComponent& gameManagerComp)
	: IGameState(gameManagerComp)
{
	auto& scene = GetGameManager()->GetGameObject()->GetScene();
	const auto windowSize = kob::Kobengine::GetWindowSize();

	m_pMenuObject = &scene.AddEmpty("StartMenu");
	m_pMenuObject->SetParent(GetGameManager()->GetGameObject());

	//Background
	auto& title = scene.AddEmpty("Title");
	title.SetParent(m_pMenuObject);
	title.SetLocalPosition({ windowSize.x / 2, 100, 0 });
	auto irc = title.AddComponent<kob::ImageRendererComponent>("Title.png");
	auto ircSize = irc->GetSize();
	float scale = static_cast<float>(windowSize.x) / ircSize.x / 1.5f;
	title.SetLocalScale({ scale, scale, 1 });
	title.SetRenderPriority(49);

	//Text
	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 13);
	auto fontL = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 16);

	// Bean
	auto* info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);

	// Modes
	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	auto menu = info->AddComponent<MenuComponent>(*fontS);
	m_pMenuComponent = menu;

	menu->AddOption("Single Player", { windowSize.x / 2, windowSize.y * 0.48f }, [&] { EnterSinglePlayerMode(); });
	menu->AddOption("Versus Mode", { windowSize.x / 2, windowSize.y * 0.58f }, [&] { EnterVersusMode(); });
	menu->AddOption("Co-Op Mode", { windowSize.x / 2, windowSize.y * 0.68f }, [&] { EnterCoOpMode(); });
	menu->SetSelectorOffset({-windowSize.x / 4, 0});

	auto& selector = scene.AddEmpty("MenuSelector");
	selector.SetParent(m_pMenuObject);
	selector.AddComponent<kob::ImageRendererComponent>("characters/ChefSheet.png", glm::vec4{ 16, 16, 16, 16 });
	selector.SetLocalScale({ 2, 2, 2 });
	menu->SetSelector(&selector);

	// Info Menu
	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("Press A/Space to start!", fontL);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.80f, 0));

	info = &scene.AddEmpty();
	info->SetParent(m_pMenuObject);
	info->AddComponent<kob::TextRendererComponent>("Press X for Info!", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.9f, 0));

	// Info Menu
	m_pInfoObject = &scene.AddEmpty("Info Menu");
	m_pInfoObject->SetParent(GetGameManager()->GetGameObject());
	m_pInfoObject->SetLocalPosition({ windowSize.x / 2, windowSize.y / 2, 0 });
	irc = m_pInfoObject->AddComponent<kob::ImageRendererComponent>("Info.png");
	ircSize = irc->GetSize();
	scale = static_cast<float>(windowSize.x) / ircSize.x;
	m_pInfoObject->SetLocalScale({ scale, scale, 1 });
	m_pInfoObject->SetRenderPriority(49);

	m_pMenuObject->SetActive(false);
	m_pInfoObject->SetActive(false);
}


//--------------------------------------------------
//    Interface
//--------------------------------------------------
bt::IGameState* bt::GameMenuState::Update()
{
	if (m_ExitMenu)
		return GetGameManager()->PlayState();
	return nullptr;
}

void bt::GameMenuState::OnEnter()
{
	kob::ServiceLocator::GetSoundService().Play("sound/System Sound.wav", 1.f);

	m_pMenuObject->SetActive(true);
	m_pInfoObject->SetActive(false);
	m_ExitMenu = false;

	auto& im = kob::InputManager::GetInstance();
	im.RegisterGamepad();

	// menu navigation
	im.RegisterKeyboardCmd(SDLK_x,							kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { ToggleInfoMenu(); }));
	im.RegisterKeyboardCmd(SDLK_UP,							kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-1); }));
	im.RegisterKeyboardCmd(SDLK_DOWN,						kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(1);  }));
	im.RegisterGamepadCmd(kob::Gamepad::Button::X,			kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { ToggleInfoMenu(); }), 0);
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_UP,	kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(-1); }), 0);
	im.RegisterGamepadCmd(kob::Gamepad::Button::DPAD_DOWN,	kob::TriggerState::Pressed, std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->MoveSelector(1);  }), 0);

	// Select
	im.RegisterGamepadCmd(
		kob::Gamepad::Button::A,
		kob::TriggerState::Pressed,
		std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->SelectOption(); }), 0);

	im.RegisterKeyboardCmd(
		SDLK_SPACE,
		kob::TriggerState::Pressed,
		std::make_unique<kob::CommandPFN>([&] { m_pMenuComponent->SelectOption(); }));
}

void bt::GameMenuState::OnExit()
{
	m_pMenuObject->SetActive(false);
	m_pInfoObject->SetActive(false);
	m_ExitMenu = true;

	// Unregister input
	kob::InputManager::GetInstance().UnregisterGamepadBtn(kob::Gamepad::Button::X, 0);
	kob::InputManager::GetInstance().UnregisterGamepadBtn(kob::Gamepad::Button::A, 0);
	kob::InputManager::GetInstance().UnregisterGamepadBtn(kob::Gamepad::Button::DPAD_UP, 0);
	kob::InputManager::GetInstance().UnregisterGamepadBtn(kob::Gamepad::Button::DPAD_DOWN, 0);
	kob::InputManager::GetInstance().UnregisterKeyboardKey(SDLK_x);
	kob::InputManager::GetInstance().UnregisterKeyboardKey(SDLK_SPACE);
	kob::InputManager::GetInstance().UnregisterKeyboardKey(SDLK_UP);
	kob::InputManager::GetInstance().UnregisterKeyboardKey(SDLK_DOWN);
}


//--------------------------------------------------
//    Extra
//--------------------------------------------------
void bt::GameMenuState::ToggleInfoMenu() const
{
	const auto menuActive = m_pMenuObject->IsActive();
	m_pMenuObject->SetActive(!menuActive);
	m_pInfoObject->SetActive(menuActive);
}
void bt::GameMenuState::EnterSinglePlayerMode()
{
	m_ExitMenu = true;
	GetGameManager()->gameMode = GameMode::Solo;
	std::cout << "Enter Single Player\n";
}
void bt::GameMenuState::EnterVersusMode()
{
	m_ExitMenu = true;
	GetGameManager()->gameMode = GameMode::Versus;
	std::cout << "Enter Versus Mode\n";
}
void bt::GameMenuState::EnterCoOpMode()
{
	m_ExitMenu = true;
	GetGameManager()->gameMode = GameMode::CoOp;
	std::cout << "Enter Co-Op Mode\n";
}
