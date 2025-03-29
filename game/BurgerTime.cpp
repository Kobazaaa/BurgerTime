// SDL
#include <SDL.h>

// ENGINE
#include "Kobengine.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"

// COMPONENTS
#include "IntUIComponent.h"
#include "FPSComponent.h"
#include "ImageRendererComponent.h"
#include "TextRendererComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

// COMMANDS
#include "MoveCommands.h"
#include "DamageCommand.h"
#include "ScoreCommand.h"

void kob::Kobengine::Setup()
{
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	using namespace kob;
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto fontL = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fontS = ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto& inputManager = InputManager::GetInstance();
	constexpr float speed = 50.f;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Background Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto bg = std::make_unique<GameObject>();
	bg->AddComponent<ImageRendererComponent>("background.tga");
	scene.Add(std::move(bg));
	
	auto logo = std::make_unique<GameObject>();
	logo->AddComponent<ImageRendererComponent>("logo.tga");
	logo->SetLocalPosition(glm::vec3(216, 180, 0));
	scene.Add(std::move(logo));

	auto title = std::make_unique<GameObject>();
	title->AddComponent<TextRendererComponent>("Programming 4 Assignment", fontL);
	title->SetLocalPosition(glm::vec3(80, 20, 0));
	scene.Add(std::move(title));




	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Character Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Chef
	auto& chef = scene.AddEmpty();
	chef.AddComponent<ImageRendererComponent>("Chef.png");
	const auto chefHealth = chef.AddComponent<HealthComponent>(3);
	const auto chefScore = chef.AddComponent<ScoreComponent>();
	chef.SetLocalPosition(glm::vec3(50, 250, 0));
	chef.SetLocalScale(glm::vec3(1, 1, 1));
	chef.SetLocalRotation(glm::vec3(0, 0, 37));

	// Bean
	auto& bean = scene.AddEmpty();
	bean.AddComponent<ImageRendererComponent>("Bean.png");
	const auto beanHealth = bean.AddComponent<HealthComponent>(3);
	const auto beanScore = bean.AddComponent<ScoreComponent>();
	bean.SetLocalPosition(glm::vec3(50, 300, 0));
	bean.SetLocalScale(glm::vec3(3, 3, 1));
	bean.SetLocalRotation(glm::vec3(0, 0, 180));

	chef.SetParent(&bean, true);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    UI Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Chef
	auto chefInputUI = std::make_unique<GameObject>();
	chefInputUI->AddComponent<TextRendererComponent>("Use the D-Pad to move the Chef, X to inflict damage, A and B to gain score", fontS);
	chefInputUI->SetLocalPosition(glm::vec3(5, 100, 0));
	scene.Add(std::move(chefInputUI));

	// Bean
	auto beanInputUI = std::make_unique<GameObject>();
	beanInputUI->AddComponent<TextRendererComponent>("Use WASD to move the Bean, C to inflict damage, Z and X to gain score", fontS);
	beanInputUI->SetLocalPosition(glm::vec3(5, 120, 0));
	scene.Add(std::move(beanInputUI));

	// Fps
	auto fpsUI = std::make_unique<GameObject>();
	fpsUI->AddComponent<TextRendererComponent>("FPS", fontL);
	fpsUI->SetLocalPosition(glm::vec3(0, 450, 0));
	fpsUI->AddComponent<FPSComponent>();
	scene.Add(std::move(fpsUI));

	// Health
	auto chefHealthUI = std::make_unique<GameObject>();
	chefHealthUI->AddComponent<TextRendererComponent>("# lives: " + std::to_string(chefHealth->GetMaxLives()), fontS);
	chefHealthUI->AddComponent<IntUIComponent>("# lives: ", chefHealth->GetMaxLives(), "", chefHealth->OnHealthChanged());
	chefHealthUI->SetLocalPosition(glm::vec3(5, 150, 0));
	scene.Add(std::move(chefHealthUI));

	auto beanHealthUI = std::make_unique<GameObject>();
	beanHealthUI->AddComponent<TextRendererComponent>("# lives: " + std::to_string(beanHealth->GetMaxLives()), fontS);
	beanHealthUI->AddComponent<IntUIComponent>("# lives: ",beanHealth->GetMaxLives(), "", beanHealth->OnHealthChanged());
	beanHealthUI->SetLocalPosition(glm::vec3(5, 200, 0));
	scene.Add(std::move(beanHealthUI));


	// Score
	auto chefScoreUI = std::make_unique<GameObject>();
	chefScoreUI->AddComponent<TextRendererComponent>("Score: 0", fontS);
	chefScoreUI->AddComponent<IntUIComponent>("Score: ", 0, "", chefScore->OnScoreChanged());
	chefScoreUI->SetLocalPosition(glm::vec3(5, 170, 0));
	scene.Add(std::move(chefScoreUI));

	auto beanScoreUI = std::make_unique<GameObject>();
	beanScoreUI->AddComponent<TextRendererComponent>("Score: 0", fontS);
	beanScoreUI->AddComponent<IntUIComponent>("Score: ", 0, "", beanScore->OnScoreChanged());
	beanScoreUI->SetLocalPosition(glm::vec3(5, 220, 0));
	scene.Add(std::move(beanScoreUI));


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Input Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	inputManager.RegisterGamepad();

	// Chef
	inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_UP,	 TriggerState::Down, std::make_unique<MoveCommand>(chef, glm::vec3{ 0, -1, 0 }, 2 * speed), 0);
	inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_DOWN,  TriggerState::Down, std::make_unique<MoveCommand>(chef, glm::vec3{ 0,  1, 0 }, 2 * speed), 0);
	inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_RIGHT, TriggerState::Down, std::make_unique<MoveCommand>(chef, glm::vec3{ 1,  0, 0 }, 2 * speed), 0);
	inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_LEFT,  TriggerState::Down, std::make_unique<MoveCommand>(chef, glm::vec3{ -1,  0, 0 }, 2 * speed), 0);

	inputManager.RegisterGamepadCmd(Gamepad::Button::X, TriggerState::Pressed, std::make_unique<DamageCommand>(*chefHealth), 0);
	inputManager.RegisterGamepadCmd(Gamepad::Button::A, TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 10), 0);
	inputManager.RegisterGamepadCmd(Gamepad::Button::B, TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 100), 0);

	// Bean
	inputManager.RegisterKeyboardCmd(SDLK_w, TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 0, -1, 0 }, speed));
	inputManager.RegisterKeyboardCmd(SDLK_s, TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 0,  1, 0 }, speed));
	inputManager.RegisterKeyboardCmd(SDLK_d, TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 1,  0, 0 }, speed));
	inputManager.RegisterKeyboardCmd(SDLK_a, TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{-1,  0, 0 }, speed));

	inputManager.RegisterKeyboardCmd(SDLK_c, TriggerState::Pressed, std::make_unique<DamageCommand>(*beanHealth));
	inputManager.RegisterKeyboardCmd(SDLK_z, TriggerState::Pressed, std::make_unique<ScoreCommand>(*beanScore, 10));
	inputManager.RegisterKeyboardCmd(SDLK_x, TriggerState::Pressed, std::make_unique<ScoreCommand>(*beanScore, 100));
}
