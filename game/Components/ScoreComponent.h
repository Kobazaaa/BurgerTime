#pragma once
#include <memory>

#include "Component.h"
#include "Event.h"

namespace kob
{
	class ScoreComponent final : public Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit ScoreComponent(GameObject& parent);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;


		//--------------------------------------------------
		//    Score
		//--------------------------------------------------
		int GetScore() const;
		void AddScore(int amount);

		//--------------------------------------------------
		//    Events
		//--------------------------------------------------
		Event<int>& OnScoreChanged();

	private:
		// Datamembers
		int m_Score{ 0 };

		// Events
		Event<int> m_OnScoreChange;
	};
}
