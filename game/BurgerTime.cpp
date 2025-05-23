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

#include "GameManagerComponent.h"
#include "LevelComponent.h"

void kob::Kobengine::Setup()
{
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// ~~    Setup
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	using namespace kob;
	using namespace bt;

	//  General
	constexpr float TILE_SIZE = 32.f;
	constexpr uint32_t ROWS = 13;
	constexpr uint32_t COLS = 17;
	constexpr float WINDOW_WIDTH  = COLS * TILE_SIZE;
	constexpr float WINDOW_HEIGHT = ROWS * TILE_SIZE;

	SetWindowSize(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	SetWindowTitle("Burger Time - Kobe Dereyne - 2GD10");

	// Game Setup
	auto& scene = SceneManager::GetInstance().CreateScene("GameScene");

	auto& gameManager = scene.AddEmpty("GameManager");
	gameManager.FlagSceneIndependent();
	gameManager.AddComponent<GameManagerComponent>();
}

