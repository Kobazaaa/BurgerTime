#include "GameLeaderboardState.h"
#include "GameManagerComponent.h"
#include "ImageRendererComponent.h"
#include "Kobengine.h"
#include "ResourceManager.h"
#include "TextRendererComponent.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor & Destructor
//--------------------------------------------------
bt::GameLeaderboardState::GameLeaderboardState(GameManagerComponent& gameManagerComp)
	: IGameState(gameManagerComp)
{
	auto& scene = GetGameManager()->GetParent()->GetScene();
	const auto windowSize = kob::Kobengine::GetWindowSize();

	m_pLeaderboardObject = &scene.AddEmpty("StartMenu");
	m_pLeaderboardObject->SetParent(GetGameManager()->GetParent());

	//Background
	auto& background = scene.AddEmpty("Background");
	background.SetParent(m_pLeaderboardObject);
	background.SetLocalPosition({ windowSize.x / 2, windowSize.y / 2, 0 });
	const auto irc = background.AddComponent<kob::ImageRendererComponent>("background.tga");
	const auto ircSize = irc->GetSize();
	background.SetLocalScale({ static_cast<float>(windowSize.x) / ircSize.x, static_cast<float>(windowSize.y) / ircSize.y, 1 });

	//Text
	auto fontS = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 11);
	auto fontL = kob::ResourceManager::GetInstance().LoadFont("fonts/arcade-legacy.otf", 16);

	// Info
	auto* info = &scene.AddEmpty();
	info->SetParent(m_pLeaderboardObject);
	info->AddComponent<kob::TextRendererComponent>("This is where the leaderboard will be!", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.30f, 0));
	info = &scene.AddEmpty();
	info->SetParent(m_pLeaderboardObject);
	info->AddComponent<kob::TextRendererComponent>("It's very empty now", fontS);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.36f, 0));
	info = &scene.AddEmpty();
	info->SetParent(m_pLeaderboardObject);
	info->AddComponent<kob::TextRendererComponent>("You will be transported shortly.", fontL);
	info->SetLocalPosition(glm::vec3(windowSize.x / 2, windowSize.y * 0.72f, 0));

	m_pLeaderboardObject->SetActive(false);
}


//--------------------------------------------------
//    Interface
//--------------------------------------------------
bt::IGameState* bt::GameLeaderboardState::Update()
{
	m_Time += kob::Timer::GetDeltaSeconds();
	if (m_Time >= m_Delay)
		return GetGameManager()->MenuState();
	return nullptr;
}

void bt::GameLeaderboardState::OnEnter()
{
	m_Time = 0.f;
	m_pLeaderboardObject->SetActive(true);
}

void bt::GameLeaderboardState::OnExit()
{
	m_Time = 0.f;
	m_pLeaderboardObject->SetActive(false);
}
