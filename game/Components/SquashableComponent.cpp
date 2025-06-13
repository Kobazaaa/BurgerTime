#include "SquashableComponent.h"
#include <algorithm>

#include "Animator.h"
#include "GameObject.h"
#include "MovementComponent.h"


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
	m_Squashed = true;
	if (auto anim = GetGameObject()->GetComponent<kob::Animator>())
		anim->Play("Squashed", false);
	if (auto move = GetGameObject()->GetComponent<MovementComponent>())
		move->Immobilize();
}
bool bt::SquashableComponent::IsSquashed() const { return m_Squashed; }
void bt::SquashableComponent::Reset() { m_Squashed = false; }
