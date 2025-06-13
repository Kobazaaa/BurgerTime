#include "SquashableComponent.h"
#include <algorithm>

#include "Animator.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "RespawnComponent.h"
#include "ServiceLocator.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::SquashableComponent::SquashableComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::SquashableComponent::Update()
{ }

void bt::SquashableComponent::Squash()
{
	kob::ServiceLocator::GetSoundService().Play("sound/Enemy Squahed.wav", 1);
	m_Squashed = true;
	OnSquashed();
	if (auto anim = GetGameObject()->GetComponent<kob::Animator>())
		anim->Play("Squashed", false);
	if (auto move = GetGameObject()->GetComponent<MovementComponent>())
		move->Immobilize(false);
}
bool bt::SquashableComponent::IsSquashed() const { return m_Squashed; }
void bt::SquashableComponent::Reset()
{
	m_Squashed = false;
	if (auto move = GetGameObject()->GetComponent<MovementComponent>())
		move->Mobilize();
}
