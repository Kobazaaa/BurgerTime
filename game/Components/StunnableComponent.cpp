#include "StunnableComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "ServiceLocator.h"
#include "Timer.h"

//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::StunnableComponent::StunnableComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::StunnableComponent::Start()
{
	m_pMovementComponent = GetGameObject()->GetComponent<MovementComponent>();
}
void bt::StunnableComponent::Update()
{
	if (!m_IsStunned)
		return;

	m_StunTime -= kob::Timer::GetDeltaSeconds();
	if (m_StunTime <= 0.f)
	{
		m_IsStunned = false;
		if (!m_WasImmobileOnStun)
			m_pMovementComponent->Mobilize();
		m_StunTime = 0.f;
	}
}
void bt::StunnableComponent::Stun(float duration)
{
	kob::ServiceLocator::GetSoundService().Play("sound/Enemy Sprayed.wav", 1);
	m_WasImmobileOnStun = m_pMovementComponent->IsImmobile();
	m_pMovementComponent->Immobilize(false);
	m_StunTime = duration;
	m_IsStunned = true;
	if (auto animator = GetGameObject()->GetComponent<kob::Animator>())
		animator->Play("Stunned", true);
}
bool bt::StunnableComponent::IsStunned() const { return m_IsStunned; }
