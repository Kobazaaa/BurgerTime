#include "StunnableComponent.h"
#include "Animator.h"
#include "GameObject.h"
#include "MovementComponent.h"
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

	m_pMovementComponent->Immobilize(false);
	m_StunTime -= kob::Timer::GetDeltaSeconds();
	if (m_StunTime <= 0.f)
	{
		m_IsStunned = false;
		m_pMovementComponent->Mobilize();
		m_StunTime = 0.f;
	}
}
void bt::StunnableComponent::Stun(float duration)
{
	m_StunTime = duration;
	m_IsStunned = true;
	if (auto animator = GetGameObject()->GetComponent<kob::Animator>())
		animator->Play("Stunned", true);
}
bool bt::StunnableComponent::IsStunned() const { return m_IsStunned; }
