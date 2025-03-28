#pragma once

#include "Command.h"
#include "ScoreComponent.h"

namespace kob
{
	class ScoreCommand final : public Command
	{
	public:
		ScoreCommand(ScoreComponent& scorecomp, int amount = 100);
		void Execute() override;
	private:
		ScoreComponent* m_pScoreComponent;
		int m_Amount;
	};
}
