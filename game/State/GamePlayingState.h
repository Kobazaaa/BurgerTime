#pragma once
#include "Event.h"
#include "IGameState.h"

namespace kob
{
	class GameObject;
}

namespace bt
{
	class GamePlayingState final : public IGameState
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit GamePlayingState(GameManagerComponent& gameManagerComp);

		//--------------------------------------------------
		//    Interface
		//--------------------------------------------------
		IGameState* Update() override;
		void OnEnter() override;
		void OnExit() override;

		//--------------------------------------------------
		//    Extra
		//--------------------------------------------------
		void EndGame();

	private:
		kob::EventCallback<> m_EventEndGame{ this, &GamePlayingState::EndGame };
		kob::GameObject* m_pLevelObject{};
		bool m_EndGame{ false };
	};
}
