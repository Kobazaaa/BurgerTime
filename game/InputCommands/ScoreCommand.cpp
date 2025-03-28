#include "ScoreCommand.h"

kob::ScoreCommand::ScoreCommand(ScoreComponent& scorecomp, int amount)
	: m_pScoreComponent{ &scorecomp }
	, m_Amount{ amount }
{
}

void kob::ScoreCommand::Execute()
{
	m_pScoreComponent->AddScore(m_Amount);
}
