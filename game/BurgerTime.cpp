// SDL
#include <SDL.h>

// ENGINE
#include "Kobengine.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"

// COMPONENTS
#include "IntUIComponent.h"
#include "ImageRendererComponent.h"
#include "TextRendererComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "GridRendererComponent.h"

// COMMANDS
#include "Animator.h"
#include "MoveCommands.h"
#include "DamageCommand.h"
#include "ScoreCommand.h"
#include "SpriteSheet.h"

#include "LevelLoader.h"
#include <iostream>

void kob::Kobengine::Setup()
{
	uint32_t rows;
	uint32_t cols;
	auto tiles = BT::LevelLoader::LoadCSV("./assets/level/Level01.csv", rows, cols);

	const float windowWidth  = cols * 32.f;
	const float windowHeight = rows * 32.f;
	SetWindowSize(static_cast<int>(windowWidth), static_cast<int>(windowHeight));
	SetWindowTitle("Burger Time - Kobe Dereyne - 2GD10");
	ServiceLocator<ISoundSystem>::GetService().Play("sound/BGM.wav", 0.25f, -1);

	constexpr float chefWalkDelay = 0.1f;
	constexpr int chefTxtSize = 16;
	auto chefSheet = ResourceManager::GetInstance().LoadSpriteSheet("characters/ChefSheet.png",
		{
			{"Down", {
				{
					{ 0, 16, chefTxtSize, chefTxtSize},
					{16, 16, chefTxtSize, chefTxtSize},
					{32, 16, chefTxtSize, chefTxtSize},
					{16, 16, chefTxtSize, chefTxtSize},
				}, chefWalkDelay} },
			{"Up", {
				{
					{ 96, 16, chefTxtSize, chefTxtSize},
					{112, 16, chefTxtSize, chefTxtSize},
					{128, 16, chefTxtSize, chefTxtSize},
					{112, 16, chefTxtSize, chefTxtSize},
				}, chefWalkDelay} },
			{"Left", {
				{
					{48, 16, chefTxtSize, chefTxtSize},
					{64, 16, chefTxtSize, chefTxtSize},
					{80, 16, chefTxtSize, chefTxtSize},
					{64, 16, chefTxtSize, chefTxtSize},
				}, chefWalkDelay} },
			{"Right", {
				{
					{48, 0, chefTxtSize, chefTxtSize},
					{64, 0, chefTxtSize, chefTxtSize},
					{80, 0, chefTxtSize, chefTxtSize},
					{64, 0, chefTxtSize, chefTxtSize},
				}, chefWalkDelay} }
		});

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	using namespace kob;
	using namespace bt;
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	BT::LevelLoader::SpawnTileMap(tiles, rows, cols, scene, 32.f);

	//auto fontL = ResourceManager::GetInstance().LoadFont("arcade-legacy.otf", 18);
	auto fontS = ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 8);
	auto& inputManager = InputManager::GetInstance();
	constexpr float speed = 50.f;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Background Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//auto bg = std::make_unique<GameObject>();
	//bg->AddComponent<ImageRendererComponent>("level/Levels.png");
	//bg->SetLocalScale({ 3, 3, 3 });
	//scene.Add(std::move(bg));


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Character Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Chef
	auto& chef = scene.AddEmpty();
	const auto chefHealth = chef.AddComponent<HealthComponent>(4);
	chefHealth->OnDamageTaken() += [] {	ServiceLocator<ISoundSystem>::GetService().Pause("sound/BGM.wav"); ServiceLocator<ISoundSystem>::GetService().Play("sound/Death.wav", 0.25f, 0); };
	const auto chefScore = chef.AddComponent<ScoreComponent>();
	const auto renderComp = chef.AddComponent<ImageRendererComponent>(chefSheet->GetTexture());
	const auto animator = chef.AddComponent<Animator>(renderComp, chefSheet);
	const auto chefMovement = chef.AddComponent<MovementComponent>(speed);
	animator->Play("Down", false);
	chef.SetLocalPosition(glm::vec3(289, 420, 0));
	chef.SetLocalScale(glm::vec3(2,2,2));

	// Bean
	auto& bean = scene.AddEmpty();
	bean.AddComponent<ImageRendererComponent>("characters/Bean.png");
	const auto beanHealth = bean.AddComponent<HealthComponent>(3);
	beanHealth->OnDamageTaken() += [] {	ServiceLocator<ISoundSystem>::GetService().Pause("sound/BGM.wav"); ServiceLocator<ISoundSystem>::GetService().Play("sound/Death.wav", 0.25f, 0); };
	const auto beanScore = bean.AddComponent<ScoreComponent>();
	bean.SetLocalPosition(glm::vec3(50, 300, 0));
	bean.SetLocalScale(glm::vec3(2, 2, 2));
	bean.SetLocalRotation(glm::vec3(0, 0, 180));


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    UI Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Chef
	auto chefInputUI = std::make_unique<GameObject>();
	chefInputUI->AddComponent<TextRendererComponent>("Use the D-Pad to move the Bean, X to inflict damage, A and B to gain score", fontS);
	chefInputUI->SetLocalPosition(glm::vec3(5, 500, 0));
	scene.Add(std::move(chefInputUI));

	// Bean
	auto beanInputUI = std::make_unique<GameObject>();
	beanInputUI->AddComponent<TextRendererComponent>("Use WASD to move the Chef, C to inflict damage, Z and X to gain score", fontS);
	beanInputUI->SetLocalPosition(glm::vec3(5, 520, 0));
	scene.Add(std::move(beanInputUI));

	auto info = std::make_unique<GameObject>();
	info->AddComponent<TextRendererComponent>("Inflicting damage makes sound!!!", fontS);
	info->SetLocalPosition(glm::vec3(5, 540, 0));
	scene.Add(std::move(info));

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

	auto& debugGrid = scene.AddEmpty();
	debugGrid.AddComponent<GridRendererComponent>(glm::vec2{ windowWidth, windowHeight }, rows, cols);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Input Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	inputManager.RegisterGamepad();

	// Chef
	//inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_UP,	 TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 0, -1, 0 }, 2 * speed), 0);
	//inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_DOWN,  TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 0,  1, 0 }, 2 * speed), 0);
	//inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_RIGHT, TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ 1,  0, 0 }, 2 * speed), 0);
	//inputManager.RegisterGamepadCmd(Gamepad::Button::DPAD_LEFT,  TriggerState::Down, std::make_unique<MoveCommand>(bean, glm::vec3{ -1,  0, 0 }, 2 * speed), 0);
	//
	//inputManager.RegisterGamepadCmd(Gamepad::Button::X, TriggerState::Pressed, std::make_unique<DamageCommand>(*beanHealth), 0);
	//inputManager.RegisterGamepadCmd(Gamepad::Button::A, TriggerState::Pressed, std::make_unique<ScoreCommand>(*beanScore, 10), 0);
	//inputManager.RegisterGamepadCmd(Gamepad::Button::B, TriggerState::Pressed, std::make_unique<ScoreCommand>(*beanScore, 100), 0);

	// Bean
	inputManager.RegisterKeyboardCmd(SDLK_w, TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0, -1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_s, TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 0,  1, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_d, TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{ 1,  0, 0 }));
	inputManager.RegisterKeyboardCmd(SDLK_a, TriggerState::Down, std::make_unique<MoveCommand>(*chefMovement, glm::vec3{-1,  0, 0 }));

	inputManager.RegisterKeyboardCmd(SDLK_c, TriggerState::Pressed, std::make_unique<DamageCommand>(*chefHealth));
	inputManager.RegisterKeyboardCmd(SDLK_z, TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 10));
	inputManager.RegisterKeyboardCmd(SDLK_x, TriggerState::Pressed, std::make_unique<ScoreCommand>(*chefScore, 100));
}

