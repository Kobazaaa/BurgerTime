#pragma once
#include "IGameState.h"

namespace kob
{
	class GameObject;
}

namespace bt
{
	class MenuComponent;
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
		void ToggleInfoMenu() const;
		void EnterSinglePlayerMode();
		void EnterVersusMode();
		void EnterCoOpMode();
		void GoToPlayMode();

	private:
		kob::GameObject* m_pMenuObject{};
		kob::GameObject* m_pInfoObject{};
		MenuComponent* m_pMenuComponent{};
		bool m_GoToPlayMode{ false };
	};
}
