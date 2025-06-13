#include "PepperComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "StunnableComponent.h"
#include "Timer.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::PepperComponent::PepperComponent(kob::GameObject& parent, float lifeTime, float stunDuration)
	: Component(parent)
	, m_LifeTime(lifeTime)
	,m_StunDuration(stunDuration)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::PepperComponent::Update()
{
	m_LifeTime -= kob::Timer::GetDeltaSeconds();
	if (m_LifeTime <= 0.f)
		GetGameObject()->FlagForDeletion();
}
void bt::PepperComponent::OnCollisionEnter(kob::GameObject& other)
{
	if (auto comp = other.GetComponent<StunnableComponent>())
		comp->Stun(m_StunDuration);
}

