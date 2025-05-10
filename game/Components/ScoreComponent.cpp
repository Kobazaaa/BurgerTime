#include "ScoreComponent.h"
#include <algorithm>


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::ScoreComponent::ScoreComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::ScoreComponent::Update()
{ }


//--------------------------------------------------
//    Score
//--------------------------------------------------
int bt::ScoreComponent::GetScore() const
{
	return m_Score;
}
void bt::ScoreComponent::AddScore(int amount)
{
	m_Score += amount;
	m_OnScoreChange(m_Score);
}


//--------------------------------------------------
//    Events
//--------------------------------------------------
kob::Event<int>& bt::ScoreComponent::OnScoreChanged()
{
	return m_OnScoreChange;
}
