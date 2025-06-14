// STANDARD
#include <SDL.h>

// ENGINE
#include "Kobengine.h"
#include "ServiceLocator.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

// COMPONENTS
#include "TextRendererComponent.h"
#include "ScoreComponent.h"
#include "GameManagerComponent.h"

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
	constexpr uint32_t COLS = 19;
	constexpr float WINDOW_WIDTH  = COLS * TILE_SIZE;
	constexpr float WINDOW_HEIGHT = ROWS * TILE_SIZE;

	SetWindowSize(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	SetWindowTitle("Burger Time - Kobe Dereyne - 2GD10");
	//Renderer::GetInstance().SetGlobalScale(2);
	//Renderer::GetInstance().SetLogicalSize(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));

	// Game Setup
	ServiceLocator::GetSoundService().SetGlobalVolumeScale(0.25);
	auto& scene = SceneManager::GetInstance().CreateScene("GameScene");

	auto& gameManager = scene.AddEmpty("GameManager");
	gameManager.tag = "GameManager";
	gameManager.FlagSceneIndependent();
	gameManager.AddComponent<GameManagerComponent>();
	gameManager.AddComponent<ScoreComponent>();
}

