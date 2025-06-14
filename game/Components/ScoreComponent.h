#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class ScoreComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit ScoreComponent(kob::GameObject& parent);


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
		kob::Event<int> OnScoreChanged;
		kob::Event<int> OnScoreAdded;

	private:
		// Datamembers
		int m_Score{ 0 };
	};
}
