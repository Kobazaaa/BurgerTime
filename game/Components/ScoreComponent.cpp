#include "ScoreComponent.h"
#include <algorithm>


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
kob::ScoreComponent::ScoreComponent(GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void kob::ScoreComponent::Update()
{ }


//--------------------------------------------------
//    Score
//--------------------------------------------------
int kob::ScoreComponent::GetScore() const
{
	return m_Score;
}
void kob::ScoreComponent::AddScore(int amount)
{
	m_Score += amount;
	m_OnScoreChange(m_Score);
}


//--------------------------------------------------
//    Events
//--------------------------------------------------
kob::Event<int>& kob::ScoreComponent::OnScoreChanged()
{
	return m_OnScoreChange;
}
