#include "ScoreCommand.h"

bt::ScoreCommand::ScoreCommand(bt::ScoreComponent& scorecomp, int amount)
	: m_pScoreComponent{ &scorecomp }
	, m_Amount{ amount }
{
}

void bt::ScoreCommand::Execute()
{
	m_pScoreComponent->AddScore(m_Amount);
}
