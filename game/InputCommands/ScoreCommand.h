#pragma once
#include "Command.h"
#include "ScoreComponent.h"

namespace bt
{
	class ScoreCommand final : public kob::Command
	{
	public:
		ScoreCommand(ScoreComponent& scorecomp, int amount = 100);
		void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent;
		int m_Amount;
	};
}
