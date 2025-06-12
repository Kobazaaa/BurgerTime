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
	if (!m_pMovementComponent || !m_pPlayer)
		return;

	const auto& myPos = GetGameObject()->GetWorldTransform().GetPosition();
	const auto& playerPos = m_pPlayer->GetWorldTransform().GetPosition();
	glm::vec2 toPlayer = playerPos - myPos;
	auto currDir = m_pMovementComponent->GetDirection();

	if (std::signbit(currDir.y) != std::signbit(toPlayer.y) && abs(currDir.y) > FLT_EPSILON)
		toPlayer.y = currDir.y;
	if (std::signbit(currDir.x) != std::signbit(toPlayer.x) && abs(currDir.x) > FLT_EPSILON)
		toPlayer.x = currDir.x;

	m_pMovementComponent->Move(normalize(toPlayer));
}
void bt::EnemyAILogicComponent::GetSquashed() const
{
	if (auto anim = GetGameObject()->GetComponent<kob::Animator>())
		anim->Play("Squashed", false);
	m_pMovementComponent->immobilized = true;
}


