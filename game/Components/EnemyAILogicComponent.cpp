#include "EnemyAILogicComponent.h"
#include "Animator.h"
#include "MovementComponent.h"


//--------------------------------------------------
//    Constructor
//--------------------------------------------------
bt::EnemyAILogicComponent::EnemyAILogicComponent(kob::GameObject& parent)
	: Component(parent)
{ }


//--------------------------------------------------
//    Loop
//--------------------------------------------------
void bt::EnemyAILogicComponent::Start()
{
	m_pMovementComponent = GetGameObject()->GetComponent<MovementComponent>();
	m_pPlayer = GetGameObject()->GetScene().GetObjectsByTag("Player").front();
}
void bt::EnemyAILogicComponent::Update()
{
	if (!m_pMovementComponent || immobilized || !m_pPlayer)
		return;
	const auto& myPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto& playerPos = m_pPlayer->GetWorldTransform().GetPosition();
	const glm::vec2 toPlayer = playerPos - myPos;
	m_pMovementComponent->Move(normalize(toPlayer));
}

void bt::EnemyAILogicComponent::GetSquashed()
{
	if (auto anim = GetGameObject()->GetComponent<kob::Animator>())
		anim->Play("Squashed", false);
	immobilized = true;
}


