#pragma once
#include "IGameState.h"

namespace kob
{
	class GameObject;
}

namespace bt
{
	class GameLeaderboardState final : public IGameState
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit GameLeaderboardState(GameManagerComponent& gameManagerComp);

		//--------------------------------------------------
		//    Interface
		//--------------------------------------------------
		IGameState* Update() override;
		void OnEnter() override;
		void OnExit() override;

	private:
		kob::GameObject* m_pLeaderboardObject{};
		float m_Time{};
		float m_Delay{7.f};
	};
}
