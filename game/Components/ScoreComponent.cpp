#include "ScoreComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextRendererComponent.h"
#include "SelfDestroyComponent.h"


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
	OnScoreChanged(m_Score);
	if (amount > 0)
		OnScoreAdded(amount);
}
