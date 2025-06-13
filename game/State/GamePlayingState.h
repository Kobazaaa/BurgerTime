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
		void PlayerDied();
		void ResetCurrentLevel();
		void LoadNextLevel(int id);
		void SetupPlayers();
		void SetupUI();
		void OnIngredientCompleted();
		void EndGame();

	private:
		kob::EventCallback<> m_EventPlayerDied{ this, &GamePlayingState::PlayerDied };
		kob::EventCallback<> m_EventEndGame{ this, &GamePlayingState::EndGame };
		kob::EventCallback<> m_EventIngredientComplete{ this, &GamePlayingState::OnIngredientCompleted };
		kob::GameObject* m_pLevelObject{};
		std::vector<kob::GameObject*> m_vPlayers{};

		int m_CompletedIngredientCount{};
		int m_IngredientCount{};
		int m_NextLevelID{};
		float m_ResetDelay{};

		bool m_EndGame{ false };
	};
}
