#pragma once
#include "Component.h"
#include "GameLeaderboardState.h"
#include "IGameState.h"
#include "Scene.h"
#include "GameMenuState.h"
#include "GamePlayingState.h"

namespace bt
{
	enum class GameMode
	{
		Solo, Versus, CoOp
	};
	class GameManagerComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit GameManagerComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		GameMenuState* MenuState() const;
		GamePlayingState* PlayState() const;
		GameLeaderboardState* LeaderboardState() const;
		GameMode gameMode{};

	private:
		IGameState* m_pCurrentState{};

		std::unique_ptr<GameMenuState> m_pGameMenuState{};
		std::unique_ptr<GamePlayingState> m_pGamePlayingState{};
		std::unique_ptr<GameLeaderboardState> m_pGameLeaderboardState{};

	};
}
