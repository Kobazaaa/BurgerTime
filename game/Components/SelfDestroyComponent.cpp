#include "SelfDestroyComponent.h"
#include "Timer.h"
#include "GameObject.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::SelfDestroyComponent::SelfDestroyComponent(kob::GameObject& parent, float lifeTime)
	: Component(parent)
	, m_LifeTime(lifeTime)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::SelfDestroyComponent::Update()
{
	m_LifeTime -= kob::Timer::GetDeltaSeconds();
	if (m_LifeTime <= 0.f)
		GetGameObject()->FlagForDeletion();
}

