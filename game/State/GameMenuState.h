#pragma once
#include "IGameState.h"

namespace kob
{
	class GameObject;
}

namespace bt
{
	class GameMenuState final : public IGameState
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit GameMenuState(GameManagerComponent& gameManagerComp);

		//--------------------------------------------------
		//    Interface
		//--------------------------------------------------
		IGameState* Update() override;
		void OnEnter() override;
		void OnExit() override;

		//--------------------------------------------------
		//    Extra
		//--------------------------------------------------
		void GoToPlayMode();

	private:
		kob::GameObject* m_pMenuObject{};
		bool m_GoToPlayMode{ false };
	};
}
