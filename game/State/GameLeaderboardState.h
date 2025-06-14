#pragma once
#include "IGameState.h"
#include "MenuComponent.h"
#include "TextRendererComponent.h"

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

		//--------------------------------------------------
		//    State
		//--------------------------------------------------
		struct ScoreEntry
		{
			std::string name;
			int score;
		};
		void SelectInput();
		void SaveScore() const;
		void LoadTopScores(size_t count);

	private:
		kob::GameObject* m_pLeaderboardObject{};
		MenuComponent* m_pMenuComponent{};
		ScoreEntry m_NewValue{};
		kob::TextRendererComponent* m_pNameComp{};
		std::string m_FileName{ "highscores.txt" };
		std::vector<ScoreEntry> m_vLoadedHighScores{};

		float m_ExistDelay{ 4.f };
		float m_Timer{};
	};
}
